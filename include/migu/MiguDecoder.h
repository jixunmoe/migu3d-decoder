#pragma once

#include "ValidatorBase.h"

#include <memory>
#include <set>
#include <span>
#include <string>

#include <cstdint>

namespace Migu3D {

void DecryptSegment(std::span<uint8_t> buffer, const std::span<const uint8_t, 32> key, std::size_t i = 0);

std::set<std::string> SearchKeys(const std::span<const uint8_t>& header,
                                 const std::span<std::shared_ptr<ValidatorBase>> validators);

}  // namespace Migu3D
