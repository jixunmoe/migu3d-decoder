#pragma once

#include <algorithm>
#include <array>
#include <bit>
#include <concepts>
#include <iostream>

#include <cstddef>
#include <cstdint>

namespace Migu3D {

template <std::integral T>
constexpr T byteswap(T value) noexcept {
  static_assert(std::has_unique_object_representations_v<T>, "T may not have padding bits");
  auto value_representation = std::bit_cast<std::array<std::byte, sizeof(T)>>(value);
  std::ranges::reverse(value_representation);
  return std::bit_cast<T>(value_representation);
}

inline uint32_t letoh(uint32_t v) {
  if constexpr (std::endian::native == std::endian::little) {
    return v;
  }

  return byteswap(v);
}

}  // namespace Migu3D
