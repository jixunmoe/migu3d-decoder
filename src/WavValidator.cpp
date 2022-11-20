#include "migu/WavValidator.h"
#include "migu/ValidationHelper.h"
#include "migu/endian.h"

#include <algorithm>
#include <cassert>
#include <string>

namespace Migu3D {
const std::string kMagicRIFF("RIFF");
const std::string kMagicWAVEFormat("WAVEfmt ");

bool WavValidator::ValidateFileHeader(const std::span<const uint8_t>& header) {
  assert((header.size() >= 0x1000, "Buffer too small to validate."));

  if (!std::equal(kMagicRIFF.begin(), kMagicRIFF.end(), &header[0])) {
    return false;
  }

  if (!std::equal(kMagicWAVEFormat.begin(), kMagicWAVEFormat.end(), &header[8])) {
    return false;
  }

  // fmt chunk size can only be one of the following: 16, 18, and 40.
  auto fmtChunkSize = letoh(*reinterpret_cast<const uint32_t*>(&header[0x10]));
  if (fmtChunkSize != 16 && fmtChunkSize != 18 && fmtChunkSize != 40) {
    return false;
  }

  auto firstDataChunkOffset = 0x14 + fmtChunkSize;
  if (auto firstDataChunkName = std::string(&header[firstDataChunkOffset], &header[firstDataChunkOffset] + 4);
      !std::ranges::all_of(firstDataChunkName.begin(), firstDataChunkName.end(), IsPrintableAsciiChar)) {
    return false;
  }

  if (firstDataChunkOffset + 8 + 8 < header.size()) {
    auto firstDataChunkSize = letoh(*reinterpret_cast<const uint32_t*>(&header[firstDataChunkOffset + 4]));
    auto secondDataChunkOffset = firstDataChunkOffset + 8 + firstDataChunkSize;
    if (secondDataChunkOffset + 4 <= header.size()) {
      auto secondDataChunkName = std::string(&header[secondDataChunkOffset], &header[secondDataChunkOffset] + 4);
      if (!std::ranges::all_of(secondDataChunkName.begin(), secondDataChunkName.end(), IsPrintableAsciiChar)) {
        return false;
      }
    }
  }

  return true;
}

}  // namespace Migu3D
