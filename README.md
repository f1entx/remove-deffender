# remove-defender

A high-performance C++ utility designed to completely disable and restore Microsoft Windows Defender on Windows 10 and Windows 11.

---

## Overview

The project consists of two core binaries:

- disable.exe: Fully disables Windows Defender, halts security subsystems, and renames protected driver filters and engine binaries to prevent them from executing at system boot.
- enable.exe: Restores all modified files from the saved state manifest, reinstates TrustedInstaller ownership, removes policy restrictions, and starts the services back up.
- build.bat: Automated build script for compiling from source using MSVC x64.

---

## How It Works

1. Privilege Escalation:
The tool automatically elevates its execution context to NT SERVICE\TrustedInstaller via the Task Scheduler RunEx interface (using the internal task WinDefWorkerTask), ensuring full write permissions over protected system keys, drivers, and services.

2. Binary Soft-Deletion:
To ensure Windows Defender cannot load after a reboot without requiring Safe Mode:
- Driver filters (WdFilter.sys, WdBoot.sys) and the engine executable (MsMpEng.exe) are renamed to .OLD.
- Target paths and metadata are written to %ProgramData%\WinDefState\manifest.dat prior to modification.

3. Policy and Subsystem Lockdown:
- Sets DisableAntiSpyware and DisableRealtimeMonitoring policies.
- Disables AMSI, ETW autologgers, SmartScreen, and Exploit Guard integrations.
- Sets IFEO hijack entries and Explorer DisallowRun configurations.
- Configures core services (WinDefend, SecurityHealthService) to manual start.

4. Restoration Flow:
Executing enable.exe reads %ProgramData%\WinDefState\manifest.dat, renames all .OLD files back to their original names, restores file ownership to NT SERVICE\TrustedInstaller, deletes policy locks, and re-enables services.

---

## Usage

### Disabling Windows Defender
1. Open Windows Security > Virus & threat protection > Manage settings.
2. Set Tamper Protection to OFF.
3. Set Real-time protection to OFF.
4. Run disable.exe as Administrator.
5. Reboot your system.

### Restoring Windows Defender
1. Run enable.exe as Administrator.
2. Reboot your system.

### Optional Flags
-s, --silent: Run in silent mode without waiting for a key press at the end.

---

## Building from Source

Requirements: Visual Studio 2022 or higher with C++ Desktop Development tools.
Run build.bat from a command prompt to build both binaries (x64 Release, static CRT /MT). Output files are placed in the root directory and the bin/ folder.
