#pragma once

#include <cstdint>

namespace Migu3D {

inline bool IsPrintableAsciiChar(uint8_t ch) {
  return ch >= 0x20 && ch <= 0x7E;
}

inline bool IsUpperHexChar(uint8_t ch) {
  return (ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F');
}

}  // namespace Migu3D