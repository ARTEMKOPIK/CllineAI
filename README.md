# CllineAI

## Автосборка и автопубликация `.exe`

В репозиторий добавлен GitHub Actions workflow `.github/workflows/build-and-release-exe.yml`.

Что происходит автоматически:
- при каждом `push` в `main/master` (и при ручном запуске) GitHub собирает проект в режиме `Release`;
- формируется один чистый файл `CleanAI.exe`;
- `CleanAI.exe` загружается в artifacts workflow;
- `CleanAI.exe` публикуется/обновляется в GitHub Release с тегом `latest`.

То есть после каждого обновления кода вы получаете актуальный `.exe` без ручных действий.
