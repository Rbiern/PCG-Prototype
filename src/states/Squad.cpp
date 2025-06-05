#include "Squad.hpp"


Squad::Squad() : rm(ResourceManager::getInstance()), deckSelection{{SelectionButton("1st Squad"), SelectionButton("2nd Squad"), SelectionButton("3rd Squad"), SelectionButton("4th Squad")}} {
    backgroundTexture = rm.getTexture("../../images/pages/squad_page.png");
    homeButtonTexture = rm.getTexture("../../images/buttons/home_button.png");
    editButtonTexture = rm.getTexture("../../images/buttons/edit_button.png");
    redoButtonTexture = rm.getTexture("../../images/buttons/redo_button.png");
    nextButtonTexture = rm.getTexture("../../images/buttons/right_arrow_button.png");
    previousButtonTexture = rm.getTexture("../../images/buttons/left_arrow_button.png");
    selectedFrameTexture = rm.getTexture("../../images/elements/selected_frame.png");

    backgroundSprite = new sf::Sprite(backgroundTexture);
    homeButtonSprite = new sf::Sprite(homeButtonTexture);
    editButtonSprite = new sf::Sprite(editButtonTexture);
    redoButtonSprite = new sf::Sprite(redoButtonTexture);
    nextButtonSprite = new sf::Sprite(nextButtonTexture);
    previousButtonSprite = new sf::Sprite(previousButtonTexture);
    selectedFrameSprite = new sf::Sprite(selectedFrameTexture);

    backgroundSprite->setPosition(sf::Vector2f{0.f, 0.f});

    std::array<sf::Vector2f, 8> uiCoords = {
            sf::Vector2f(768.f, 299.f),
            sf::Vector2f(988.f, 299.f),
            sf::Vector2f(1208.f, 299.f),
            sf::Vector2f(1428.f, 299.f),
            sf::Vector2f(768.f, 570.f),
            sf::Vector2f(988.f, 570.f),
            sf::Vector2f(1208.f, 570.f),
            sf::Vector2f(1428.f, 570.f)
    };
    // List of all tower names from the JSON
    std::array<std::string, 8> id = {
            "o1", "o2", "o3", "o4", "o5", "o6", "o7", "o8"
    };
    for (int i = 0; i < 8; i++) {
        currentDeck[i] = new PlayerCard(id[i]);
        currentDeck[i]->setPosition(uiCoords[i]);
        currentDeck[i]->setScale(sf::Vector2f{1.5f, 1.5f});
    }
    selectedTowerCard = currentDeck[0];

    info = new TowerInfo("Ana Clerk");

    resize(rm.ResourceManager::getScaling());
}


Squad::~Squad() {
    for (auto& i : currentDeck) {
        delete i;
    }
    delete backgroundSprite;
    delete homeButtonSprite;
    delete editButtonSprite;
    delete redoButtonSprite;
    delete nextButtonSprite;
    delete previousButtonSprite;
    delete selectedFrameSprite;
}


bool Squad::handleUserInput(const sf::Event &event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scan::Escape) {
            game->setMenu(std::make_unique<Home>());
            return false;
        }
    } else if (const auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
            if (homeButtonSprite->getGlobalBounds().contains(sf::Vector2f(mouseButtonPressed->position))) {
                game->setMenu(std::make_unique<Home>());
                return false;
            }
        }
        // Player card selection (towers)
        for (auto & i : currentDeck) {
            if (i->contains(sf::Vector2f(mouseButtonPressed->position))) {
                sf::Vector2f scale = rm.getScaling();
                sf::Vector2f position = i->getPosition();
                selectedFrameSprite->setPosition(sf::Vector2f{position.x - (9.f * scale.x), position.y - (9.f * scale.y)});
                selectedTowerCard = i;
            }
        }
    }
    return false;
}


void Squad::menuActionUpdate(float delta) {

}


void Squad::resize(sf::Vector2f scale) {
    info->setPosition(sf::Vector2f{scale.x * 67.f, scale.y *  100.f});
    info->setScale(scale);
    std::array<sf::Vector2f, 8> uiCoords = {
            sf::Vector2f(768.f, 299.f),
            sf::Vector2f(988.f, 299.f),
            sf::Vector2f(1208.f, 299.f),
            sf::Vector2f(1428.f, 299.f),
            sf::Vector2f(768.f, 570.f),
            sf::Vector2f(988.f, 570.f),
            sf::Vector2f(1208.f, 570.f),
            sf::Vector2f(1428.f, 570.f)
    };
    for (int i = 0; i < 8; i++) {
        currentDeck[i]->setPosition(sf::Vector2f{uiCoords[i].x * scale.x, uiCoords[i].y * scale.y});
        currentDeck[i]->setScale(sf::Vector2f{scale.x * 1.5f, scale.y * 1.5f});
    }
    deckSelection[0].setPosition(sf::Vector2f{762.f * scale.x, 1032.f * scale.y});
    deckSelection[0].setScale(scale);
    deckSelection[1].setPosition(sf::Vector2f{979.f * scale.x, 1032.f * scale.y});
    deckSelection[1].setScale(scale);
    deckSelection[2].setPosition(sf::Vector2f{1196.f * scale.x, 1032.f * scale.y});
    deckSelection[2].setScale(scale);
    deckSelection[3].setPosition(sf::Vector2f{1413.f * scale.x, 1032.f * scale.y});
    deckSelection[3].setScale(scale);

    backgroundSprite->setScale(scale);
    homeButtonSprite->setScale(scale);
    homeButtonSprite->setPosition(sf::Vector2f{1614.f * scale.x, 53.f * scale.y});
    editButtonSprite->setScale(scale);
    editButtonSprite->setPosition(sf::Vector2f{928.f * scale.x, 883.f * scale.y});
    redoButtonSprite->setScale(scale);
    redoButtonSprite->setPosition(sf::Vector2f{1201.f * scale.x, 883.f * scale.y});
    selectedFrameSprite->setScale(scale);
    sf::Vector2f position = selectedTowerCard->getPosition();
    selectedFrameSprite->setPosition(sf::Vector2f{position.x - (9.f * scale.x), position.y - (9.f * scale.y)});
    {
        auto ts = nextButtonTexture.getSize();
        float sx = (82.f * scale.x) / float(ts.x);
        float sy = (79.f * scale.y) / float(ts.y);
        nextButtonSprite->setScale(sf::Vector2f{sx, sy});
    }
    nextButtonSprite->setPosition(sf::Vector2f{1671.f * scale.x, 514.f * scale.y});
    {
        auto ts = previousButtonTexture.getSize();
        float sx = (82.f * scale.x) / float(ts.x);
        float sy = (79.f * scale.y) / float(ts.y);
        previousButtonSprite->setScale(sf::Vector2f{sx, sy});
    }
    previousButtonSprite->setPosition(sf::Vector2f{622.f * scale.x, 514.f * scale.y});
}


void Squad::render(sf::RenderWindow &window) {
    window.draw(*backgroundSprite);
    window.draw(*homeButtonSprite);
    window.draw(*editButtonSprite);
    window.draw(*redoButtonSprite);
    window.draw(*nextButtonSprite);
    window.draw(*previousButtonSprite);
    window.draw(*selectedFrameSprite);
    // Draw player cards on top of panel
    for (auto& i : currentDeck) {
        window.draw(*i);
    }
    for (int i = 0; i < 4; i++) {
        window.draw(deckSelection[i]);
    }
    window.draw(*info);
}
