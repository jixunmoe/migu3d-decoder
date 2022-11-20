#pragma once

#include <optional>
#include <string>
#include <vector>

namespace Migu3D {

void WindowsConsoleInit();
std::optional<std::vector<std::wstring>> GetWindowsUTF16Arguments();

}  // namespace Migu3D
