#include <windows.h>

int main()
{
    const wchar_t* title = L"CleanAI: CI-сборка";
    const wchar_t* message =
        L"Вы запустили headless CI-версию CleanAI.\n\n"
        L"Это техническая заглушка для проверки сборки в CI, "
        L"она не открывает графический интерфейс.\n\n"
        L"Чтобы получить обычное приложение с окном, пересоберите проект "
        L"без флага CLEANAI_HEADLESS_CI=ON.";

    MessageBoxW(
        nullptr,
        message,
        title,
        MB_OK | MB_ICONINFORMATION
    );

    return 0;
}
