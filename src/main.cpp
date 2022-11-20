#include "migu/MiguDecoder.h"
#include "migu/WavValidator.h"
#include "migu/WinArgHelper.h"
#include "migu/constants.h"

#include "migu/config.h"

#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>

#include <cstddef>

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
  std::cerr << "Migu3D Decoder Demo (v" MIGU3D_VERSION ") by Jixun" << std::endl << std::endl;

  if (argc < 2) {
    std::cerr << "Usage" << std::endl;
    std::cerr << " \'" << argv[0] << "' <input_file> [output_file]" << std::endl;
    std::cerr << std::endl;
    return 1;
  }

  auto utf16_args = Migu3D::GetWindowsArgs();
  fs::path input_file_path(argv[1]);
  fs::path output_file_path;
  if (utf16_args.has_value()) {
    input_file_path.assign(utf16_args.value()[1]);
  }
  if (argc > 2) {
    if (utf16_args.has_value()) {
      output_file_path.assign(utf16_args.value()[2]);
    } else {
      output_file_path.assign(argv[2]);
    }
  } else {
    output_file_path = input_file_path;
    auto name = output_file_path.stem().u8string();
    name += u8"_(Migu3D).wav";
    output_file_path.replace_filename(name);
  }
  std::cerr << "  INFO:  input = " << input_file_path.string() << std::endl;
  std::cerr << "  INFO: output = " << output_file_path.string() << std::endl;

  std::array<uint8_t, Migu3D::kDecryptionSegmentSize> buffer = {0};
  auto p_buffer = reinterpret_cast<char*>(buffer.data());

  std::ifstream input_file(input_file_path, std::ios::in | std::ios::binary);
  std::size_t bytes_read = static_cast<std::size_t>(input_file.readsome(p_buffer, Migu3D::kDecryptionSegmentSize));
  if (bytes_read != Migu3D::kDecryptionSegmentSize) {
    std::cerr << " FATAL: file too small or read error. read " << bytes_read << " bytes, expecting "
              << Migu3D::kDecryptionSegmentSize << " bytes." << std::endl;
  }

  std::vector<std::shared_ptr<Migu3D::ValidatorBase>> validators;
  validators.emplace_back(std::make_shared<Migu3D::WavValidator>());

  auto keys = Migu3D::SearchKeys(buffer, validators);
  if (keys.size() == 0) {
    std::cerr << " FATAL: did not find a key." << std::endl;
    return 3;
  } else if (keys.size() > 1) {
    std::cerr << "  WARN: " << keys.size() << " keys found, using the first one." << std::endl;
  }

  auto key = *keys.begin();
  auto p_key = std::span<const uint8_t, Migu3D::kMiguKeySize>{reinterpret_cast<const uint8_t*>(key.data()),
                                                              Migu3D::kMiguKeySize};
  std::cerr << "  INFO: decrypting with final key '" << key << "'..." << std::endl;

  std::size_t bytes_processed = 0;
  std::ofstream output(output_file_path, std::ios::out | std::ios::binary);
  while (bytes_read > 0) {
    Migu3D::DecryptSegment(buffer, p_key, bytes_processed);
    output.write(p_buffer, bytes_read);
    bytes_processed += bytes_read;
    bytes_read = static_cast<std::size_t>(input_file.readsome(p_buffer, Migu3D::kDecryptionSegmentSize));
  }
  std::cerr << "  INFO: Done!" << std::endl;

  return 0;
}
