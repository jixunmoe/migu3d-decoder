#pragma once

#include <span>

#include <cstdint>

namespace Migu3D {
class ValidatorBase {
 public:
  virtual ~ValidatorBase() = default;
  virtual bool ValidateFileHeader(const std::span<const uint8_t>& header) = 0;
};
}  // namespace Migu3D
