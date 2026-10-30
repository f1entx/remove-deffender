#pragma once
#include <Windows.h>
#include <iostream>
#include "reg.hpp"
#include "util.hpp"
#include "wmic.hpp"

namespace winsec
{
  void toggle_tamper(bool enable);
  bool is_tamper_enabled();
  bool is_tamper_enabled_wmi();

  bool disable_defender();
  bool enable_defender();

  bool check_defender(uint32_t flags = 0);
  void report_status();

  void kill_smartscreen();
  void kill_defender_processes();
  bool manage_windefend(bool enable);
  bool manage_security_center(bool enable);

  void disable_services_registry();
  void enable_services_registry();

  void set_group_policies(bool disable);

  void disable_smartscreen_registry();
  void enable_smartscreen_registry();

  void disable_amsi();
  void enable_amsi();

  void disable_etw_logging();
  void enable_etw_logging();

  void set_defender_internal_state(bool disable);

  void disable_safe_boot_registration();
  void enable_safe_boot_registration();

  void set_ifeo_debugger_hijack(bool enable);
  void set_disallow_run(bool enable);

  void disable_notifications();
  void enable_notifications();

  void disable_security_ui();
  void enable_security_ui();

  void disable_wtds();
  void enable_wtds();

  void disable_application_guard();
  void enable_application_guard();

  void disable_exploit_guard();
  void enable_exploit_guard();

  void disable_smartscreen_extras();
  void enable_smartscreen_extras();

  void disable_minifilter_altitude();
  void enable_minifilter_altitude();

  void disable_vbs();
  void enable_vbs();

  void disable_firewall_registry();
  void enable_firewall_registry();

  void disable_scheduled_tasks();
  void enable_scheduled_tasks();

  void soft_delete_binaries();
  void restore_binaries();

  // Unified high-level flows
  bool disable_all();
  bool enable_all();
}
