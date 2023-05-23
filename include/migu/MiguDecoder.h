#pragma once

#include <memory>
#include <set>
#include <span>
#include <string>
#include <vector>

#include <cstdint>

namespace Migu3D {

void DecryptSegment(std::span<uint8_t> buffer, const std::span<const uint8_t> key, std::size_t i = 0);

std::vector<uint8_t> SearchByFreqAnalysis(const std::span<const uint8_t>& header);

}  // namespace Migu3D
