#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


class GameApp;
struct VideoSettings;

class Window final {
    friend class GameApp;

public:
    explicit Window() = default;
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    void initializeWindow(VideoSettings& vidSettings);
    void updateView();
    void toggleFullScreen();

private:
    sf::RenderWindow renderWindow;
    VideoSettings* settings = nullptr;
    sf::View view;
};
