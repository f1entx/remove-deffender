#include "winsec.hpp"
#include "trusted.hpp"
#include <iostream>

bool check_silent(int argc, char** argv)
{
  for (int i = 0; i < argc; i++)
  {
    if (!strcmp(argv[i], "-s") || !strcmp(argv[i], "--silent"))
      return true;
  }
  return false;
}

bool has_flag(int argc, char** argv, const char* flag)
{
  for (int i = 0; i < argc; i++)
  {
    if (!_stricmp(argv[i], flag))
      return true;
  }
  return false;
}

int main(int argc, char** argv)
{
  if (has_flag(argc, argv, "-tilog"))
    trusted::redirect_output_to_ti_log();

  bool silent = check_silent(argc, argv);
  bool worker = has_flag(argc, argv, "-worker");

  if (!trusted::has_admin())
  {
    printf("[!] Error: This tool must be run as Administrator.\n");
    if (!silent)
      system("pause");
    return EXIT_FAILURE;
  }

  // Elevate to TrustedInstaller if not already running in worker mode
  if (!worker && !trusted::is_system_group())
  {
    std::wstring exe = util::string_to_wide(util::get_current_path());
    std::wstring args = L"-s -worker -tilog";

    trusted::reset_ti_log();

    printf("[*] Elevating to TrustedInstaller...\n");
    LONG exit_code = 0;
    if (trusted::run_as_ti_scheduled(exe, args, &exit_code))
    {
      trusted::print_ti_log();
    }
    else
    {
      printf("[*] Falling back to token impersonation...\n");
      std::string relaunch = util::get_current_path() + " -s -worker";
      trusted::create_process(relaunch);
    }

    if (!silent)
      system("pause");

    return EXIT_SUCCESS;
  }

  try
  {
    winsec::enable_all();
  }
  catch (std::exception& e)
  {
    printf("Exception caught: %s\n", e.what());
  }

  if (!silent)
    system("pause");

  return EXIT_SUCCESS;
}
