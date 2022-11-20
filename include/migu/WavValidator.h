#pragma once

#include "ValidatorBase.h"

namespace Migu3D {

class WavValidator : public ValidatorBase {
 public:
  ~WavValidator() override = default;
  bool ValidateFileHeader(const std::span<const uint8_t>& header) override;
};

}  // namespace Migu3D
