# CleanAI

## Windows CI (стабильный режим сборки)

В репозитории используется `.github/workflows/build.yml` с простой схемой:

1. `vcpkg install` зависимостей;
2. `cmake` генерирует `build/CleanAI.sln` в обычном режиме; если WinUI SDK не найден — автоматически переключается на headless;
3. `msbuild build/CleanAI.sln /p:Configuration=Release /p:Platform=x64 /m`.

### Что это значит

- Если на раннере доступен `Microsoft.WindowsAppSDK`, собирается полноценный WinUI-вариант.
- Если пакет отсутствует, сборка не падает: CMake автоматически уходит в headless-режим.
- Локально/в релизной среде можно принудительно выбрать нужный режим флагами CMake.

### Локальная сборка на Windows

**CI-совместимый headless-вариант:**

```powershell
cmake -S cleanai -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE="<VCPKG_ROOT>/scripts/buildsystems/vcpkg.cmake" -DCLEANAI_HEADLESS_CI=ON
msbuild build/CleanAI.sln /p:Configuration=Release /p:Platform=x64 /m
```

**Полный WinUI-вариант (как раньше):**

```powershell
cmake -S cleanai -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE="<VCPKG_ROOT>/scripts/buildsystems/vcpkg.cmake"
msbuild build/CleanAI.sln /p:Configuration=Release /p:Platform=x64 /m
```

### Что сейчас публикует CI

CI запускает обычную конфигурацию. Дальше есть два сценария:

- если `Microsoft.WindowsAppSDK` найден — собирается обычная WinUI-версия;
- если пакет не найден — CMake автоматически переключает сборку в `headless` (без падения пайплайна).

Чтобы запретить автоматический откат и сразу получить ошибку при отсутствии WinUI SDK, добавьте флаг:

```powershell
-DCLEANAI_AUTO_FALLBACK_HEADLESS=OFF
```

### Как получить пользовательскую версию

На Windows 11 ставьте окружение Windows App SDK и запускайте «Полный WinUI-вариант» из инструкции выше. Если нужно готовое приложение для установки, используйте `cleanai/packaging/create_installer.bat`.

### Отладка в CI

При каждом запуске workflow выгружается артефакт `build-debug-logs` (даже при падении), где есть диагностика окружения раннера.
