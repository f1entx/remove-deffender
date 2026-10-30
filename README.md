# Windows Defender Controller (Direct Dual-Binary Engine)

Lightweight, high-performance C++ utility designed to completely disable and safely restore Microsoft Windows Defender on Windows 10 and Windows 11 without interactive menus or bloat.

---

## Overview

The tool provides two dedicated, standalone executables:

- disable.exe: Elevates to TrustedInstaller, halts Defender components, locks down registry/group policies, and renames protected engine binaries and driver filters to .OLD so Defender cannot load at boot.
- enable.exe: Reads the restore manifest, reinstates all original binary extensions, restores TrustedInstaller file ownership, clears policy restrictions, and reactivates core security services.
- build.bat: Automated one-click MSVC x64 build script.

---

## Technical Mechanism

1. Privilege Elevation:
Both executables automatically escalate privileges to NT SERVICE\TrustedInstaller using the Task Scheduler RunEx API (creating a temporary system task WinDefWorkerTask) with a fallback to token duplication. This bypasses access restrictions on core Windows security keys and files.

2. Driver and Engine Soft-Deletion:
Modern Defender engines run under protected process light (PPL) and early-launch anti-malware (ELAM) drivers, making live termination temporary. The utility soft-deletes the critical files by renaming them to .OLD:
- WdFilter.sys (Minifilter driver)
- WdBoot.sys (ELAM boot driver)
- MsMpEng.exe (Antimalware service executable)
- MpDefenderCoreService.exe
All original paths are logged to %ProgramData%\WinDefState\manifest.dat before modification.

3. Subsystem Lockdown:
- Enforces DisableAntiSpyware and DisableRealtimeMonitoring policies.
- Deactivates AMSI (Antimalware Scan Interface) and ETW security autologgers.
- Disables SmartScreen, Exploit Guard, Web Threat Defense, and VBS integrations.
- Configures IFEO debugger hijacks and Explorer DisallowRun rules.
- Sets WinDefend, SecurityHealthService, and related services to manual/disabled.

4. Seamless Restoration:
Running enable.exe parses %ProgramData%\WinDefState\manifest.dat, restores original file names, reassigns ownership back to NT SERVICE\TrustedInstaller, purges registry blocks, and sets WinDefend back to automatic start.

---

## Instructions

### Disabling Windows Defender
1. Open Windows Security > Virus & threat protection > Manage settings.
2. Turn Tamper Protection to OFF.
3. Turn Real-time protection to OFF.
4. Run disable.exe as Administrator.
5. Reboot your system. Defender drivers and services will fail to load and remain inactive.

### Re-enabling Windows Defender
1. Run enable.exe as Administrator.
2. Reboot your system. All binaries, drivers, registry settings, and services will return to normal operational state.

### Silent Mode
Both binaries accept the -s or --silent flag to suppress console pauses upon completion:
disable.exe -s
enable.exe -s

---

## Compilation

Prerequisites: Visual Studio 2022 / vNext with C++ Desktop Development workload.
Run build.bat from an elevated or standard command prompt. The script automatically detects the MSVC environment, compiles both targets with static runtime linking (/MT) and maximum speed optimizations (/O2 /GL), and outputs the resulting binaries to the root directory and bin/ folder.
