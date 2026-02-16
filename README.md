# CllineAI

## Автосборка и автопубликация Windows-сборки

В репозиторий добавлен GitHub Actions workflow `.github/workflows/build-and-release-exe.yml`.

Что происходит автоматически:
- при каждом `push` в `main/master` (и при ручном запуске) GitHub собирает проект в режиме `Release`;
- формируется дистрибутив `CleanAI-windows-x64.zip`, который содержит `CleanAI.exe` и необходимые runtime-зависимости (`.dll`);
- архив загружается в workflow artifacts;
- этот же архив публикуется/обновляется в GitHub Release с тегом `latest`.

То есть после каждого обновления кода вы получаете актуальную portable-сборку для запуска на «чистой» Windows-машине без ручной докомплектации библиотек.

