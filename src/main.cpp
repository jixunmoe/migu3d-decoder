#include "migu/MiguDecoder.h"
#include "migu/WindowsWorkarounds.h"
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
  Migu3D::WindowsConsoleInit();
  auto utf16_args = Migu3D::GetWindowsUTF16Arguments();

  std::cerr << "Migu3D Decoder Demo (v" MIGU3D_VERSION ") by Jixun" << std::endl << std::endl;

  if (argc < 2) {
    std::cerr << "Usage" << std::endl;
    std::cerr << " \'" << argv[0] << "' <input_file> [output_file]" << std::endl;
    std::cerr << std::endl;
    return 1;
  }

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
  std::cerr << "  INFO:  input = " << (const char*)input_file_path.generic_u8string().c_str() << std::endl;
  std::cerr << "  INFO: output = " << (const char*)output_file_path.generic_u8string().c_str() << std::endl;

  std::array<uint8_t, Migu3D::kDecryptionSegmentSize> buffer = {0};
  auto p_buffer = reinterpret_cast<char*>(buffer.data());

  std::ifstream input_file(input_file_path, std::ios::in | std::ios::binary);
  input_file.read(p_buffer, Migu3D::kDecryptionSegmentSize);
  std::size_t bytes_read = static_cast<std::size_t>(input_file.gcount());
  if (bytes_read != Migu3D::kDecryptionSegmentSize) {
    std::cerr << " FATAL: file too small or read error. read " << bytes_read << " bytes, expecting "
              << Migu3D::kDecryptionSegmentSize << " bytes." << std::endl;
    return 2;
  }

  auto key = Migu3D::SearchByFreqAnalysis(buffer);
  auto key_as_str = std::string(key.begin(), key.end());
  if (key_as_str.find('?') != std::string::npos) {
    std::cerr << " ERROR: key not found (Invalid key: '" << key_as_str << "')" << std::endl;
    return 1;
  }
  std::cerr << "  INFO: decrypting with final key '" << key_as_str << "'..." << std::endl;

  std::size_t bytes_processed = 0;
  std::ofstream output(output_file_path, std::ios::out | std::ios::binary);
  while (bytes_read > 0) {
    Migu3D::DecryptSegment(buffer, key, bytes_processed);
    output.write(p_buffer, bytes_read);
    bytes_processed += bytes_read;
    input_file.read(p_buffer, Migu3D::kDecryptionSegmentSize);
    bytes_read = static_cast<std::size_t>(input_file.gcount());
  }
  std::cerr << "  INFO: Done!" << std::endl;

  return 0;
}
