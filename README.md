# CllineAI

## Windows CI (упрощённый стек)

Проект переведён на максимально простой CI-пайплайн в `.github/workflows/build.yml`:

- без CMake;
- без NuGet-резолва Windows App SDK;
- без кастомного PowerShell-парсинга вывода команд;
- с прямым `msbuild` вызовом решения.

### Что важно

Текущая схема требует **статический файл решения** в репозитории:

- `cleanai/CleanAI.sln`

Если его нет, workflow завершится с явной ошибкой и подсказкой, что нужно закоммитить `.sln/.vcxproj`.

### Одна команда для сборки

```powershell
msbuild cleanai/CleanAI.sln /p:Configuration=Release /p:Platform=x64 /m
```
