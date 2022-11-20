#include "migu/MiguDecoder.h"
#include "migu/ValidationHelper.h"
#include "migu/constants.h"

#include <algorithm>
#include <array>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include <cassert>

namespace Migu3D {

void DecryptSegment(std::span<uint8_t> buffer, const std::span<const uint8_t, kMiguKeySize> key, std::size_t i) {
  for (auto& byte : buffer) {
    byte -= key[i % key.size()];
    i++;
  }
}

std::set<std::string> SearchKeys(const std::span<const uint8_t>& header,
                                 const std::span<std::shared_ptr<ValidatorBase>> validators) {
  std::vector<uint8_t> decrypted_header;
  std::set<std::string> result;

  std::array<uint8_t, kMiguKeySize> possible_key;
  std::string possible_key_str;
  for (auto i = kMiguKeySize; i <= header.size() - 32; i += kMiguKeySize) {
    std::copy(&header[i], &header[i + kMiguKeySize], possible_key.begin());
    if (!std::ranges::all_of(possible_key.begin(), possible_key.end(), IsUpperHexChar)) {
      continue;
    }

    possible_key_str.assign(possible_key.begin(), possible_key.end());
#if NDEBUG
    if (result.find(possible_key_str) != result.end()) {
      continue;
    }
#endif

    decrypted_header.assign(header.begin(), header.end());
    DecryptSegment(decrypted_header, possible_key, 0);

    if (std::ranges::any_of(validators.begin(), validators.end(), [&decrypted_header](const auto& validator) -> bool {
          return validator->ValidateFileHeader(decrypted_header);
        })) {
#if !NDEBUG
      std::cerr << " DEBUG: found a possible key at offset " << std::setw(4) << std::hex << i << std::endl;
#endif
      result.emplace(possible_key_str);
    }
  }

  return result;
}

}  // namespace Migu3D
