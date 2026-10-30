@echo off
setlocal enabledelayedexpansion

echo ===================================================
echo   Windows Defender Control Build (MSVC x64)
echo   Clean Two-Binary Architecture (No Menu / No Bloat)
echo ===================================================

set "VS_DIR="
if exist "%ProgramFiles%\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" (
    set "VS_DIR=%ProgramFiles%\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat"
) else if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" (
    set "VS_DIR=%ProgramFiles%\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat"
) else if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" (
    set "VS_DIR=%ProgramFiles%\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat"
) else if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvarsall.bat" (
    set "VS_DIR=%ProgramFiles%\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvarsall.bat"
)

if not defined VS_DIR (
    echo [ERROR] Could not find Visual Studio vcvarsall.bat. Please install VS2022+ with C++ workload.
    exit /b 1
)

echo [*] Initializing MSVC x64 environment...
call "!VS_DIR!" x64 >nul 2>&1
if errorlevel 1 (
    echo [ERROR] Failed to initialize MSVC environment.
    exit /b 1
)

if not exist bin mkdir bin

set "COMMON_SRCS=winsec.cpp reg.cpp trusted.cpp util.cpp wmic.cpp"
set "LIBS=taskschd.lib ole32.lib oleaut32.lib comsuppw.lib advapi32.lib shell32.lib user32.lib"
set "CFLAGS=/nologo /O2 /Oi /GL /EHsc /MT /std:c++latest /permissive- /DNDEBUG /D_CONSOLE /D_MBCS /utf-8"
set "LFLAGS=/link /nologo /SUBSYSTEM:CONSOLE /MANIFEST /MANIFESTUAC:"level='requireAdministrator' uiAccess='false'""

cd /d "%~dp0src"

echo [*] Compiling disable.exe (Direct Disabler)...
cl.exe %CFLAGS% disable_main.cpp %COMMON_SRCS% %LFLAGS% %LIBS% /OUT:"..\bin\disable.exe"
if errorlevel 1 goto :fail

echo [*] Compiling enable.exe (Direct Enabler)...
cl.exe %CFLAGS% enable_main.cpp %COMMON_SRCS% %LFLAGS% %LIBS% /OUT:"..\bin\enable.exe"
if errorlevel 1 goto :fail

cd /d "%~dp0"
copy /y "bin\disable.exe" "." >nul
copy /y "bin\enable.exe" "." >nul

echo.
echo ===================================================
echo [SUCCESS] Both binaries built successfully:
echo   - disable.exe
echo   - enable.exe
echo ===================================================
exit /b 0

:fail
echo.
echo [ERROR] Compilation failed.
cd /d "%~dp0"
exit /b 1
