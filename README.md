# CllineAI

## Windows CI (стабильный вариант без кастомных PowerShell-скриптов)

В репозитории используется `.github/workflows/build.yml` с максимально простой схемой:

1. `vcpkg install` зависимостей;
2. `cmake` один раз генерирует `build/CleanAI.sln` для Visual Studio 2022;
3. `msbuild build/CleanAI.sln /p:Configuration=Release /p:Platform=x64 /m`.

### Почему больше нет бага с «утечкой вывода команд»

Потому что мы полностью убрали проблемный слой:

- нет кастомных PowerShell-функций, которые парсят вывод `nuget/cmake`;
- нет записи произвольного stdout в `$GITHUB_ENV`;
- нет ветвистой логики с авто-поиском SDK-путей по текстовому выводу команд;
- каждый шаг — одна явная команда.

### Команды для локальной сборки на Windows

```powershell
cmake -S cleanai -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE="<VCPKG_ROOT>/scripts/buildsystems/vcpkg.cmake"
msbuild build/CleanAI.sln /p:Configuration=Release /p:Platform=x64 /m
```
