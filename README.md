# CleanAI

## Windows CI (релизный WinUI-режим)

В репозитории используется `.github/workflows/build.yml` с релизной схемой:

1. `vcpkg install` зависимостей;
2. `cmake` генерирует `build/CleanAI.sln` **только** для WinUI (без автоматического перехода в headless);
3. `msbuild` собирает `Release x64`;
4. CI упаковывает релизную папку в `CleanAI-win64.zip`.

### Что это значит

- CI теперь собирает только полноценное WinUI-приложение.
- Если на раннере отсутствует `Microsoft.WindowsAppSDK`, сборка завершается с ошибкой (чтобы случайно не выпустить техническую заглушку).
- В релиз публикуется архив `CleanAI-win64.zip` с содержимым папки `build/Release` и краткой инструкцией запуска.

### Локальная сборка на Windows

**Полный WinUI-вариант (как в CI):**

```powershell
cmake -S cleanai -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE="<VCPKG_ROOT>/scripts/buildsystems/vcpkg.cmake" -DCLEANAI_AUTO_FALLBACK_HEADLESS=OFF
msbuild build/CleanAI.sln /p:Configuration=Release /p:Platform=x64 /m
```

**Технический headless-вариант (только для диагностики):**

```powershell
cmake -S cleanai -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE="<VCPKG_ROOT>/scripts/buildsystems/vcpkg.cmake" -DCLEANAI_HEADLESS_CI=ON
msbuild build/CleanAI.sln /p:Configuration=Release /p:Platform=x64 /m
```

### Что публикует CI

- Артефакт workflow: `CleanAI-win64.zip`.
- Для тегов `v*`: тот же архив публикуется в GitHub Release.
- Диагностика среды раннера доступна в артефакте `build-debug-logs`.

### Как получить пользовательскую версию

Скачайте `CleanAI-win64.zip`, распакуйте архив полностью и запустите `CleanAI.exe`.
