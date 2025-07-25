#include "Home.hpp"


Home::Home(): rm(ResourceManager::getInstance()) {
    backgroundTexture = rm.getTexture("../../images/pages/home_page.png");
    backgroundSprite = new sf::Sprite(backgroundTexture);
    backgroundSprite->setPosition(sf::Vector2f{0.f, 0.f});

    exitButtonTexture = rm.getTexture("../../images/buttons/exit_button.png");
    exitButtonSprite = new sf::Sprite(exitButtonTexture);

    settingsButtonTexture = rm.getTexture("../../images/buttons/settings_button.png");
    settingsButtonSprite = new sf::Sprite(settingsButtonTexture);

    squadButtonTexture = rm.getTexture("../../images/buttons/squad_button.png");
    squadButtonSprite = new sf::Sprite(squadButtonTexture);

    creatorButtonTexture = rm.getTexture("../../images/buttons/creator_button.png");
    creatorButtonSprite = new sf::Sprite(creatorButtonTexture);

    playButtonTexture = rm.getTexture("../../images/buttons/play_button.png");
    playButtonSprite = new sf::Sprite(playButtonTexture);

    resize(rm.ResourceManager::getScaling());
}


Home::~Home() {
    delete backgroundSprite;
    delete exitButtonSprite;
    delete settingsButtonSprite;
    delete squadButtonSprite;
    delete creatorButtonSprite;
    delete playButtonSprite;
}


bool Home::handleUserInput(const sf::Event &event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scan::Escape) {
            return true;
        }
    } else if (const auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
            if (exitButtonSprite->getGlobalBounds().contains(sf::Vector2f(mouseButtonPressed->position))) {
                game->setMenu(std::make_unique<StartUp>());
                return false;
            }
            if (settingsButtonSprite->getGlobalBounds().contains(sf::Vector2f(mouseButtonPressed->position))) {
                game->setMenu(std::make_unique<Options>());
                return false;
            }
            if (squadButtonSprite->getGlobalBounds().contains(sf::Vector2f(mouseButtonPressed->position))) {
                game->setMenu(std::make_unique<Squad>());
                return false;
            }
            if (creatorButtonSprite->getGlobalBounds().contains(sf::Vector2f(mouseButtonPressed->position))) {
                game->setMenu(std::make_unique<Creator>());
                return false;
            }
            if (playButtonSprite->getGlobalBounds().contains(sf::Vector2f(mouseButtonPressed->position))) {
                game->setMenu(std::make_unique<Level>());
                return false;
            }
        }
    }
    return false;
}


void Home::menuActionUpdate(float delta) {

}


void Home::resize(sf::Vector2f scale) {
    // Background
    backgroundSprite->setScale(scale);
    // Buttons
    exitButtonSprite->setScale(scale);
    exitButtonSprite->setPosition(sf::Vector2f{1643.f * scale.x, 53.f * scale.y});
    settingsButtonSprite->setScale(scale);
    settingsButtonSprite->setPosition(sf::Vector2f{1325.f * scale.x, 53.f * scale.y});
    squadButtonSprite->setScale(scale);
    squadButtonSprite->setPosition(sf::Vector2f{1007.f * scale.x, 53.f * scale.y});
    creatorButtonSprite->setScale(scale);
    creatorButtonSprite->setPosition(sf::Vector2f{689.f * scale.x, 53.f * scale.y});
    playButtonSprite->setScale(scale);
    playButtonSprite->setPosition(sf::Vector2f{846.f * scale.x, 918.f * scale.y});
}


void Home::render(sf::RenderWindow &window) {
    window.draw(*backgroundSprite);
    window.draw(*exitButtonSprite);
    window.draw(*settingsButtonSprite);
    window.draw(*squadButtonSprite);
    window.draw(*creatorButtonSprite);
    window.draw(*playButtonSprite);
}
