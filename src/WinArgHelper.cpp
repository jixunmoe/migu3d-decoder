#pragma once

#include <optional>
#include <string>
#include <vector>

#if _MSC_VER
#include <Windows.h>
#include <processenv.h>
#include <shellapi.h>
#pragma comment(lib, "Shell32.lib")
#endif

namespace Migu3D {

std::optional<std::vector<std::wstring>> GetWindowsArgs() {
#if _MSC_VER
  std::vector<std::wstring> result;
  int nArgs;
  wchar_t** utf16_args = CommandLineToArgvW(GetCommandLineW(), &nArgs);
  for (int i = 0; i < nArgs; i++) {
    result.emplace_back(utf16_args[i]);
  }
  LocalFree(utf16_args);
  return result;
#else
  return {};
#endif
}

}  // namespace Migu3D
