# CllineAI

## Windows CI (стабильный режим сборки)

В репозитории используется `.github/workflows/build.yml` с простой схемой:

1. `vcpkg install` зависимостей;
2. `cmake` генерирует `build/CleanAI.sln` в режиме `-DCLEANAI_HEADLESS_CI=ON`;
3. `msbuild build/CleanAI.sln /p:Configuration=Release /p:Platform=x64 /m`.

### Что это значит

- Полноценный WinUI runtime (`Microsoft.WindowsAppSDK`) больше **не блокирует CI**.
- На CI собирается headless-заглушка `CleanAI` (для проверки toolchain, зависимостей и воспроизводимости).
- Локально/в релизной среде можно собирать обычный WinUI-вариант (без `CLEANAI_HEADLESS_CI=ON`).

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

### Отладка в CI

При каждом запуске workflow выгружается артефакт `build-debug-logs` (даже при падении), где есть диагностика окружения раннера.


### Что попадает в Release

Workflow теперь формирует архив `release/CleanAI-windows-x64.zip`, куда копируется каталог рядом с найденным `CleanAI.exe`, и:

- загружает его в artifacts (`CleanAI-windows-x64`),
- публикует/обновляет GitHub Release `latest` на ветках `main/master`.
