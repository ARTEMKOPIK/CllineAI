#include <windows.h>

#include <string>

int main()
{
    const std::string title = "CleanAI: CI-сборка";
    const std::string message =
        "Вы запустили headless CI-версию CleanAI.\n\n"
        "Это техническая заглушка для проверки сборки в CI, "
        "она не открывает графический интерфейс.\n\n"
        "Чтобы получить обычное приложение с окном, пересоберите проект "
        "без флага CLEANAI_HEADLESS_CI=ON.";

    MessageBoxA(
        nullptr,
        message.c_str(),
        title.c_str(),
        MB_OK | MB_ICONINFORMATION
    );

    return 0;
}
