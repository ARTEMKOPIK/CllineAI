#include "App.h"

#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Windows.Win32.h>
#include <windows.h>

namespace CleanAI
{
    App::App()
    {
        InitializeComponent();
        m_ollama = std::make_shared<Core::OllamaClient>();
    }

    void App::OnLaunched(winrt::Microsoft::UI::Xaml::LaunchActivatedEventArgs const&)
    {
        try
        {
            m_ollama->EnsureServerAvailableAsync().get();
        }
        catch (winrt::hresult_error const& ex)
        {
            MessageBoxW(nullptr, ex.message().c_str(), L"CleanAI", MB_OK | MB_ICONERROR);
            std::exit(EXIT_FAILURE);
        }

        auto window = winrt::make<MainWindow>(IsOnboardingCompleted());
        window.Activate();
        m_window = window;
    }

    bool App::IsOnboardingCompleted() const
    {
        HKEY key{};
        DWORD value{};
        DWORD size = sizeof(value);
        if (RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\CleanAI", 0, KEY_READ, &key) == ERROR_SUCCESS)
        {
            auto rc = RegQueryValueExW(key, L"OnboardingCompleted", nullptr, nullptr, reinterpret_cast<LPBYTE>(&value), &size);
            RegCloseKey(key);
            return rc == ERROR_SUCCESS && value == 1;
        }
        return false;
    }

    void App::MarkOnboardingCompleted() const
    {
        HKEY key{};
        DWORD value = 1;
        RegCreateKeyExW(HKEY_CURRENT_USER, L"Software\\CleanAI", 0, nullptr, 0, KEY_WRITE, nullptr, &key, nullptr);
        RegSetValueExW(key, L"OnboardingCompleted", 0, REG_DWORD, reinterpret_cast<BYTE*>(&value), sizeof(value));
        RegCloseKey(key);
    }
}
