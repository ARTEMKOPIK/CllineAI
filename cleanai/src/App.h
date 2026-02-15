#pragma once

#include "MainWindow.h"
#include "Core/OllamaClient.h"
#include <memory>
#include <winrt/Microsoft.UI.Xaml.h>

namespace CleanAI
{
    struct App : winrt::Microsoft::UI::Xaml::ApplicationT<App>
    {
        App();
        void OnLaunched(winrt::Microsoft::UI::Xaml::LaunchActivatedEventArgs const&);

    private:
        bool IsOnboardingCompleted() const;
        void MarkOnboardingCompleted() const;
        std::shared_ptr<Core::OllamaClient> m_ollama;
        winrt::Microsoft::UI::Xaml::Window m_window{ nullptr };
    };
}
