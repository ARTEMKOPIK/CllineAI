#include "OnboardingPage.h"

#include <array>

namespace CleanAI::Pages
{
    namespace
    {
        struct Slide { wchar_t const* icon; wchar_t const* title; wchar_t const* text; wchar_t const* button; };
        constexpr std::array<Slide, 3> slides{{
            {L"\xE9D9", L"ИИ анализирует ваши файлы", L"Но НИКОГДА не удаляет без вашего разрешения", L"Далее"},
            {L"\xEA18", L"Карантин защищает ваши файлы", L"Все удалённые файлы 30 дней хранятся в карантине — можно восстановить в один клик", L"Далее"},
            {L"\xE73E", L"Готово к умной очистке", L"Нажмите \"Сканировать\" и освободите место без риска", L"Начать"}
        }};
    }

    OnboardingPage::OnboardingPage()
    {
        InitializeComponent();
        UpdateScreen();
    }

    void OnboardingPage::OnBack(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
    {
        if (m_index > 0)
        {
            --m_index;
            UpdateScreen();
        }
    }

    void OnboardingPage::OnNext(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
    {
        if (m_index < 2)
        {
            ++m_index;
            UpdateScreen();
        }
    }

    void OnboardingPage::UpdateScreen()
    {
        auto const& slide = slides[m_index];
        auto icon = FindName(L"OnboardingIcon").as<winrt::Microsoft::UI::Xaml::Controls::FontIcon>();
        auto title = FindName(L"OnboardingTitle").as<winrt::Microsoft::UI::Xaml::Controls::TextBlock>();
        auto text = FindName(L"OnboardingText").as<winrt::Microsoft::UI::Xaml::Controls::TextBlock>();
        auto button = FindName(L"NextButton").as<winrt::Microsoft::UI::Xaml::Controls::Button>();

        icon.Glyph(slide.icon);
        title.Text(slide.title);
        text.Text(slide.text);
        button.Content(winrt::box_value(slide.button));
    }
}
