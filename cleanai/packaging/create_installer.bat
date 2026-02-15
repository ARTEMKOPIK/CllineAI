@echo off
setlocal

set ROOT=%~dp0..\
set BUILD_DIR=%ROOT%build
set OUT_DIR=%ROOT%dist

if not exist %OUT_DIR% mkdir %OUT_DIR%

echo [1/3] Конфигурация CMake...
cmake -S %ROOT% -B %BUILD_DIR% -G "Visual Studio 17 2022" -A x64
if errorlevel 1 exit /b 1

echo [2/3] Сборка релиза...
cmake --build %BUILD_DIR% --config Release
if errorlevel 1 exit /b 1

echo [3/3] Упаковка в MSIX...
makeappx pack /d %BUILD_DIR%\Release /p %OUT_DIR%\CleanAI.msix
if errorlevel 1 exit /b 1

echo Готово: %OUT_DIR%\CleanAI.msix
endlocal
