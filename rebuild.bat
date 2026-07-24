@echo off
REM ---------------------------------------------------------------------
REM rebuild_demo.bat
REM Rebuilds gui_shell from scratch and runs the demo app.
REM Run from the gui_shell project root (where CMakeLists.txt lives).
REM Requires: CMake, Ninja, and a C++20 compiler on PATH
REM   (e.g. run from a "Developer Command Prompt for VS" on Windows).
REM ---------------------------------------------------------------------

setlocal

set BUILD_DIR=build

echo [1/4] Cleaning previous build...
if exist %BUILD_DIR% rmdir /s /q %BUILD_DIR%

echo [2/4] Configuring (CMake + Ninja)...
cmake -S . -B %BUILD_DIR% -G Ninja -DCMAKE_BUILD_TYPE=Release
if errorlevel 1 goto :error

echo [3/4] Building...
cmake --build %BUILD_DIR%
if errorlevel 1 goto :error

echo [4/4] Running demo...
%BUILD_DIR%\examples\demo_app\gui_shell_demo.exe

goto :eof

:error
echo.
echo Build failed. See errors above.
exit /b 1