#include "migu/MiguDecoder.h"
#include "migu/constants.h"

#include <algorithm>
#include <array>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <array>

#include <cassert>

namespace Migu3D {

void DecryptSegment(std::span<uint8_t> buffer, const std::span<const uint8_t> key, std::size_t i) {
  for (auto& byte : buffer) {
    byte -= key[i % 32];
    i++;
  }
}

bool isUpperHexChar(uint8_t c) {
  return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F');
}

std::vector<uint8_t> SearchByFreqAnalysis(const std::span<const uint8_t>& header) {
  std::vector<uint8_t> result;
  std::array<std::map<uint8_t, size_t>, 32> freq;

  size_t offset = 0;
  for (auto it = header.begin(); it < header.end(); it++, offset = (offset + 1) % 32) {
    auto v = *it;
    if (isUpperHexChar(v)) {
      auto &charFreq = freq[offset];
      if (charFreq.find(v) == charFreq.end()) {
        charFreq[v] = 1;
      } else {
        charFreq[v] += 1;
      }
    }
  }

#if !NDEBUG
  std::cerr << " DEBUG: offset, key, freq count" << std::endl;
#endif

  int idx = 0;
  for (auto& charFreq : freq) {
#if !NDEBUG
    std::cerr << " DEBUG:   0x" << std::hex << std::setw(2) << std::setfill('0') << idx << ", ";
#endif
    auto max_item = std::max_element(charFreq.cbegin(), charFreq.cend(),
                     [](const auto& a, const auto& b) { return a.second < b.second; });

    auto key = (max_item == charFreq.end()) ? (uint8_t)'?' : max_item->first;
#if !NDEBUG
    std::cerr << "'" << key << "', ";
#endif
    result.push_back(key);


#if !NDEBUG
    for (auto& [chr, count] : charFreq) {
      std::cerr << (char)chr << "(" << std::dec << std::setw(2) << std::setfill(' ') << (int)count << ") ";
    }
    std::cerr << std::endl;
#endif
    idx++;
  }

  return result;
}

}  // namespace Migu3D
