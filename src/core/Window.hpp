#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


struct ApplicationSettings;

class Window final {
public:
    explicit Window() = default;
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    void initializeWindow(ApplicationSettings& appSettings);
    void updateView();
    void toggleFullScreen();

    sf::RenderWindow renderWindow;

private:
    ApplicationSettings* settings = nullptr;
    sf::View view;
};
