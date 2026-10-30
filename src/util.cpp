#include "util.hpp"

namespace util
{
  // Converts a string to wide
  //
  std::wstring string_to_wide(const std::string& s)
  {
    if (s.empty()) return std::wstring();
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), NULL, 0);
    if (size_needed <= 0) {
      std::wstring temp(s.length(), L' ');
      for (size_t i = 0; i < s.length(); ++i) temp[i] = (wchar_t)(unsigned char)s[i];
      return temp;
    }
    std::wstring result(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), &result[0], size_needed);
    return result;
  }

  // Converts a wide to string
  //
  std::string wide_to_string(const std::wstring& s) {
    if (s.empty()) return std::string();
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, s.data(), (int)s.size(), NULL, 0, NULL, NULL);
    if (size_needed <= 0) {
      std::string temp(s.length(), ' ');
      for (size_t i = 0; i < s.length(); ++i) temp[i] = (char)s[i];
      return temp;
    }
    std::string result(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, s.data(), (int)s.size(), &result[0], size_needed, NULL, NULL);
    return result;
  }

  // Sets the programs debug priviliges
  //
  bool set_privilege(LPCSTR privilege, BOOL enable)
  {
    TOKEN_PRIVILEGES priv = { 0,0,0,0 };
    HANDLE token = nullptr;
    LUID luid = { 0,0 };

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &token))
    {
      if (token)
        CloseHandle(token);

      return false;
    }

    if (!LookupPrivilegeValueA(nullptr, SE_DEBUG_NAME, &luid))
    {
      if (token)
        CloseHandle(token);

      return false;
    }
    priv.PrivilegeCount = 1;
    priv.Privileges[0].Luid = luid;
    priv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    if (!AdjustTokenPrivileges(token, false, &priv, 0, nullptr, nullptr))
    {
      if (token)
        CloseHandle(token);

      return false;
    }
    if (token)
      CloseHandle(token);

    return true;
  }

  char sub_43604B()
  {
    char v0; // bl
    SC_HANDLE v1; // eax
    SC_HANDLE v2; // esi
    void* v3; // eax

    v0 = 0;
    v1 = OpenSCManagerW(0, 0, 8u);
    v2 = v1;
    if (v1)
    {
      v3 = LockServiceDatabase(v1);
      if (v3)
      {
        UnlockServiceDatabase(v3);
        CloseServiceHandle(v2);
        return 1;
      }
      if (GetLastError() == 1055)
        v0 = 1;
      CloseServiceHandle(v2);
    }
    return v0;
  }

  // Get current username
  //
  std::string get_user()
  {
    char username[UNLEN + 1];
    DWORD username_len = UNLEN + 1;
    GetUserNameA(username, &username_len);
    return std::string(username);
  }

  // Get current path of process
  //
  std::string get_current_path()
  {
    char buf[MAX_PATH];
    DWORD len = sizeof(buf);
    int bytes = GetModuleFileNameA(NULL, buf, len);
    return std::string(buf);
  }

  // Get target process id
  //
  DWORD get_pid(std::string process_name)
  {
    HANDLE hSnapshot;
    if ((hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)) == INVALID_HANDLE_VALUE)
      return -1;

    DWORD pid = -1;
    PROCESSENTRY32 pe;
    ZeroMemory(&pe, sizeof(PROCESSENTRY32));
    pe.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hSnapshot, &pe))
    {
      CloseHandle(hSnapshot);
      return -1;
    }

    while (Process32Next(hSnapshot, &pe))
    {
      if (pe.szExeFile == process_name)
      {
        pid = pe.th32ProcessID;
        break;
      }
    }

    if (pid == -1)
    {
      CloseHandle(hSnapshot);
      return -1;
    }

    CloseHandle(hSnapshot);
    return pid;
  }
}
