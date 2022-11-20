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

void WindowsConsoleInit() {
#if _MSC_VER
  SetConsoleOutputCP(CP_UTF8);
#endif
}

std::optional<std::vector<std::wstring>> GetWindowsUTF16Arguments() {
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
