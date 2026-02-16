# CllineAI

## Windows CI (стабильный вариант без кастомных PowerShell-скриптов)

В репозитории используется `.github/workflows/build.yml` с максимально простой схемой:

1. `vcpkg install` зависимостей;
2. workflow ищет CMake-конфиг Windows App SDK по нескольким стандартным корням Windows SDK/Visual Studio на `windows-2022` (оба имени: `Microsoft.WindowsAppSDKConfig.cmake` и `microsoft.windowsappsdk-config.cmake`) и экспортирует путь в `WINDOWS_APP_SDK_DIR`;
3. `cmake` генерирует `build/CleanAI.sln` c `-DMicrosoft.WindowsAppSDK_DIR=...`;
4. `msbuild build/CleanAI.sln /p:Configuration=Release /p:Platform=x64 /m`.

### Почему больше нет бага с «утечкой вывода команд»

Потому что мы полностью убрали проблемный слой:

- нет кастомных PowerShell-функций, которые парсят вывод `nuget/cmake`;
- нет записи произвольного stdout в `$GITHUB_ENV`;
- нет парсинга stdout сторонних команд ради вычисления SDK-путей;
- каждый шаг — одна явная команда.

### Команды для локальной сборки на Windows

```powershell
WINDOWS_APP_SDK_DIR="<path-to-Microsoft.WindowsAppSDKConfig.cmake-parent>"
cmake -S cleanai -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE="<VCPKG_ROOT>/scripts/buildsystems/vcpkg.cmake" -DMicrosoft.WindowsAppSDK_DIR="$WINDOWS_APP_SDK_DIR"
msbuild build/CleanAI.sln /p:Configuration=Release /p:Platform=x64 /m
```
