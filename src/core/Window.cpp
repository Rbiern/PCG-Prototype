#include "Window.hpp"

#include <iostream>
#include <windows.h>
#include <SFML/Graphics/Image.hpp>
#include "core/ApplicationSettings.hpp"


namespace {
    constexpr sf::Vector2f BaseResolution{1920.f, 1080.f};
    constexpr sf::Vector2f Center{BaseResolution.x * 0.5f, BaseResolution.y * 0.5f};
}


void Window::initializeWindow(ApplicationSettings& appSettings) {
    if (settings != nullptr) {
        throw std::logic_error("Window is already initialized");
    }

    settings = &appSettings;

    sf::ContextSettings contextSettings;
    contextSettings.antiAliasingLevel = settings->video.antiAliasingLevel;

    const auto style = settings->video.fullscreen ? sf::Style::None : sf::Style::Default;
    const auto state = settings->video.fullscreen ? sf::State::Fullscreen : sf::State::Windowed;
    const auto mode = settings->video.fullscreen
        ? sf::VideoMode::getDesktopMode()
        : sf::VideoMode({settings->video.windowWidth, settings->video.windowHeight});

    renderWindow.create(mode, "Prototype", style, state, contextSettings);
    renderWindow.setMinimumSize(sf::Vector2u{960u, 540u});
    renderWindow.setVerticalSyncEnabled(settings->video.verticalSync);

    if (!settings->video.verticalSync) {
        renderWindow.setFramerateLimit(settings->video.frameRate);
    }

    sf::Image icon;
    if (icon.loadFromFile(std::filesystem::current_path() / "assets/images/icon.png")) {
        renderWindow.setIcon(icon);
    } else {
        std::cerr << "Load Icon Image Failed!" << std::endl;
    }

    view.setSize(BaseResolution);
    view.setCenter(Center);
    updateView();
}


void Window::updateView() {
    const float windowWidth = static_cast<float>(renderWindow.getSize().x);
    const float windowHeight = static_cast<float>(renderWindow.getSize().y);
    if (windowWidth <= 0.f || windowHeight <= 0.f) {
        return;
    }

    const float windowRatio = windowWidth / windowHeight;
    constexpr float viewRatio = BaseResolution.x / BaseResolution.y;
    float sizeX = 1.f;
    float sizeY = 1.f;
    float posX = 0.f;
    float posY = 0.f;

    if (windowRatio > viewRatio) {
        sizeX = viewRatio / windowRatio;
        posX = (1.f - sizeX) / 2.f;
    } else {
        sizeY = windowRatio / viewRatio;
        posY = (1.f - sizeY) / 2.f;
    }

    view.setViewport(sf::FloatRect({posX, posY}, {sizeX, sizeY}));
    renderWindow.setView(view);
}


void Window::toggleFullScreen() {
    settings->video.fullscreen = !settings->video.fullscreen;
    settings->dirty = true;
    HWND hwnd = renderWindow.getNativeHandle();

    if (settings->video.fullscreen) {
        const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        const int screenHeight = GetSystemMetrics(SM_CYSCREEN);
        SetWindowLongPtr(hwnd, GWL_STYLE, WS_VISIBLE | WS_POPUP);
        SetWindowPos(hwnd, HWND_TOP, 0, 0, screenWidth, screenHeight, SWP_FRAMECHANGED);
    } else {
        SetWindowLongPtr(hwnd, GWL_STYLE, WS_VISIBLE | WS_OVERLAPPEDWINDOW);
        const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        const int screenHeight = GetSystemMetrics(SM_CYSCREEN);
        const int centerX = (screenWidth - static_cast<int>(settings->video.windowWidth)) / 2;
        const int centerY = (screenHeight - static_cast<int>(settings->video.windowHeight)) / 2;
        SetWindowPos(hwnd, HWND_NOTOPMOST, centerX, centerY, settings->video.windowWidth, settings->video.windowHeight, SWP_FRAMECHANGED);
    }

    updateView();
}
