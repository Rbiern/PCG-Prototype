#include "Options.hpp"


Options::Options() : rm(ResourceManager::getInstance()) {
    videoFlag = true;
    videoFrameBox = true;
    volumeFlag = false;
    profileFlag = false;
    config = &rm.getWindowData();
    sf::Font& font = rm.getFont();
    backgroundTexture = rm.getTexture("../../images/pages/options_page.png");
    homeButtonTexture = rm.getTexture("../../images/buttons/home_button.png");
    applyButtonTexture = rm.getTexture("../../images/buttons/apply_button.png");
    selectedBarTexture = rm.getTexture("../../images/elements/selected_bar.png");
    switchTexture = rm.getTexture("../../images/buttons/switch.png");
    leftArrowTexture = rm.getTexture("../../images/buttons/left_arrow_button.png");
    rightArrowTexture = rm.getTexture("../../images/buttons/right_arrow_button.png");
    frameBoxTexture = rm.getTexture("../../images/elements/frame_box.png");
    textBoxTexture = rm.getTexture("../../images/elements/text_box.png");
    // BASE
    backgroundSprite = new sf::Sprite(backgroundTexture);
    backgroundSprite->setPosition(sf::Vector2f{0.f, 0.f});
    homeButtonSprite = new sf::Sprite(homeButtonTexture);
    applyButtonSprite = new sf::Sprite(applyButtonTexture);
    selectedHitBoxSprite1 = new sf::Sprite(selectedBarTexture);
    selectedHitBoxSprite1->setColor(sf::Color(255, 255, 255, 255));
    selectedHitBoxSprite2 = new sf::Sprite(selectedBarTexture);
    selectedHitBoxSprite2->setColor(sf::Color(255, 255, 255, 0));
    selectedHitBoxSprite3 = new sf::Sprite(selectedBarTexture);
    selectedHitBoxSprite3->setColor(sf::Color(255, 255, 255, 0));
    videoText = new sf::Text(font, "Video", 48);
    videoText->setFillColor(sf::Color::Black);
    volumeText = new sf::Text(font, "Volume", 48);
    volumeText->setFillColor(sf::Color::White);
    profileText = new sf::Text(font, "Profile", 48);
    profileText->setFillColor(sf::Color::White);
    // VIDEO SECTION
    std::array<std::string, 3> labels1 = { "4", "8", "16" };
    std::array<std::string, 3> labels2 = { "30", "60", "120" };
    slider1 = new Slider(labels1);
    slider1->setValue(300);
//    slider1->setValue(((config->antiAliasingLevel / 4 - 1) > 2) ? 16 : config->antiAliasingLevel / 4 - 1);
    slider2 = new Slider(labels2);
    slider2->setValue(((config->frameRate / 30 - 1) > 2) ? 120 : config->frameRate  / 30 - 1);
    leftArrowSprite = new sf::Sprite(leftArrowTexture);
    rightArrowSprite = new sf::Sprite(rightArrowTexture);
    switchButtonSprite1 = new sf::Sprite(switchTexture);
    switchButtonSprite1->setTextureRect( (config->fullscreen) ? sf::IntRect({0, 0}, {111, 36}) : sf::IntRect({0, 36}, {111, 36}) );
    switchButtonSprite2 = new sf::Sprite(switchTexture);
    switchButtonSprite2->setTextureRect( (config->antiAliasing) ? sf::IntRect({0, 0}, {111, 36}) : sf::IntRect({0, 36}, {111, 36}) );
    switchButtonSprite3 = new sf::Sprite(switchTexture);
    switchButtonSprite3->setTextureRect( (config->verticalSync) ? sf::IntRect({0, 0}, {111, 36}) : sf::IntRect({0, 36}, {111, 36}) );
    boxSprite = new sf::Sprite(frameBoxTexture);
    boxSprite->setTextureRect(sf::IntRect({0, 0}, {518, 158}));
    fullscreenText = new sf::Text(font, "Fullscreen", 48);
    fullscreenText->setFillColor(sf::Color::White);
    resolutionText = new sf::Text(font, "Resolution", 48);
    resolutionText->setFillColor(sf::Color::White);
    
    resolutionRatio = new sf::Text(font, "1920 x 1080", 36);
    resolutionRatio->setFillColor(sf::Color::Black);
    antialiasingText = new sf::Text(font, "Anti-aliasing", 48);
    antialiasingText->setFillColor(sf::Color::White);
    frameRateText = new sf::Text(font, "Frame Rate", 48);
    frameRateText->setFillColor(sf::Color::White);
    verticalSyncText = new sf::Text(font, "Vertical Sync", 48);
    verticalSyncText->setFillColor(sf::Color::White);
    // VOLUME SECTION
    musicBar = new SoundBar(config->musicLevel);
    soundBar = new SoundBar(config->soundLevel);
    switchButtonSprite4 = new sf::Sprite(switchTexture);
    switchButtonSprite4->setTextureRect( (config->music) ? sf::IntRect({0, 0}, {111, 36}) : sf::IntRect({0, 36}, {111, 36}) );
    switchButtonSprite5 = new sf::Sprite(switchTexture);
    switchButtonSprite5->setTextureRect( (config->soundEffect) ? sf::IntRect({0, 0}, {111, 36}) : sf::IntRect({0, 36}, {111, 36}) );
    musicText = new sf::Text(font, "Music", 48);
    musicText->setFillColor(sf::Color::White);
    soundEffectText = new sf::Text(font, "Sound Effect", 48);
    soundEffectText->setFillColor(sf::Color::White);
    // PROFILE SECTION
    textBoxSprite1 = new sf::Sprite(textBoxTexture);
    textBoxSprite2 = new sf::Sprite(textBoxTexture);
    statisticsText = new sf::Text(font, "Statistics", 48);
    statisticsText->setFillColor(sf::Color::White);
    accountText = new sf::Text(font, "Account", 48);
    accountText->setFillColor(sf::Color::White);

    resize(rm.ResourceManager::getScaling());
}


Options::~Options() {
    // BASE Settings
    delete backgroundSprite;
    delete homeButtonSprite;
    delete applyButtonSprite;
    delete selectedHitBoxSprite1;
    delete selectedHitBoxSprite2;
    delete selectedHitBoxSprite3;
    delete videoText;
    delete volumeText;
    delete profileText;
    // VIDEO SECTION
    delete slider1;
    delete slider2;
    delete leftArrowSprite;
    delete rightArrowSprite;
    delete switchButtonSprite1;
    delete switchButtonSprite2;
    delete switchButtonSprite3;
    delete boxSprite;
    delete fullscreenText;
    delete resolutionText;
    delete resolutionRatio;
    delete antialiasingText;
    delete frameRateText;
    delete verticalSyncText;
    // VOLUME SECTION
    delete musicBar;
    delete soundBar;
    delete switchButtonSprite4;
    delete switchButtonSprite5;
    delete musicText;
    delete soundEffectText;
    // PROFILE SECTION
    delete textBoxSprite1;
    delete textBoxSprite2;
    delete statisticsText;
    delete accountText;
}


bool Options::handleUserInput(const sf::Event &event) {
    if (videoFlag) {
        slider1->handleEvent(event);
    }
    if (videoFrameBox) {
        slider2->handleEvent(event);
    }
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
            if (applyButtonSprite->getGlobalBounds().contains(sf::Vector2f(mouseButtonPressed->position))) {
                std::cout << "apply" << std::endl;
                config->updateRequired = true;
                return false;
            }
            if (selectedHitBoxSprite1->getGlobalBounds().contains(sf::Vector2f(mouseButtonPressed->position))) {
                videoFlag = true; profileFlag = volumeFlag = false;
                selectedHitBoxSprite1->setColor(sf::Color(255, 255, 255, 255));
                selectedHitBoxSprite2->setColor(sf::Color(255, 255, 255, 0));
                selectedHitBoxSprite3->setColor(sf::Color(255, 255, 255, 0));
                videoText->setFillColor(sf::Color::Black);
                volumeText->setFillColor(sf::Color::White);
                profileText->setFillColor(sf::Color::White);
                return false;
            }
            if (selectedHitBoxSprite2->getGlobalBounds().contains(sf::Vector2f(mouseButtonPressed->position))) {
                volumeFlag = true; profileFlag = videoFlag = false;
                selectedHitBoxSprite1->setColor(sf::Color(255, 255, 255, 0));
                selectedHitBoxSprite2->setColor(sf::Color(255, 255, 255, 255));
                selectedHitBoxSprite3->setColor(sf::Color(255, 255, 255, 0));
                videoText->setFillColor(sf::Color::White);
                volumeText->setFillColor(sf::Color::Black);
                profileText->setFillColor(sf::Color::White);
                return false;
            }
            if (selectedHitBoxSprite3->getGlobalBounds().contains(sf::Vector2f(mouseButtonPressed->position))) {
                profileFlag = true; volumeFlag = videoFlag = false;
                selectedHitBoxSprite1->setColor(sf::Color(255, 255, 255, 0));
                selectedHitBoxSprite2->setColor(sf::Color(255, 255, 255, 0));
                selectedHitBoxSprite3->setColor(sf::Color(255, 255, 255, 255));
                videoText->setFillColor(sf::Color::White);
                volumeText->setFillColor(sf::Color::White);
                profileText->setFillColor(sf::Color::Black);
                return false;
            }

            if (videoFlag) {
                if (switchButtonSprite1->getGlobalBounds().contains(sf::Vector2f(mouseButtonPressed->position))) {
                    config->fullscreen = !config->fullscreen;
                    switchButtonSprite1->setTextureRect( (config->fullscreen) ? sf::IntRect({0, 0}, {111, 36}) : sf::IntRect({0, 36}, {111, 36}) );
                    return false;
                }
                if (switchButtonSprite2->getGlobalBounds().contains(sf::Vector2f(mouseButtonPressed->position))) {
                    config->antiAliasing = !config->antiAliasing;
                    switchButtonSprite2->setTextureRect( (config->antiAliasing) ? sf::IntRect({0, 0}, {111, 36}) : sf::IntRect({0, 36}, {111, 36}) );
                    return false;
                }
                if (frameRateText->getGlobalBounds().contains(sf::Vector2f(mouseButtonPressed->position))) {
                    boxSprite->setTextureRect(sf::IntRect({0, 0}, {518, 158}));
                    videoFrameBox = true;
                    return false;
                }
                if (verticalSyncText->getGlobalBounds().contains(sf::Vector2f(mouseButtonPressed->position))) {
                    boxSprite->setTextureRect(sf::IntRect({0, 158}, {518, 158}));
                    videoFrameBox = false;
                    return false;
                }
                if (!videoFrameBox) {
                    if (switchButtonSprite3->getGlobalBounds().contains(sf::Vector2f(mouseButtonPressed->position))) {
                        config->verticalSync = !config->verticalSync;
                        switchButtonSprite3->setTextureRect( (config->verticalSync) ? sf::IntRect({0, 0}, {111, 36}) : sf::IntRect({0, 36}, {111, 36}));
                        return false;
                    }
                }
            } else if (volumeFlag) {
                if (musicBar->handleUserInput(event)) {
                    config->musicLevel = musicBar->getLevel();
                    return false;
                }
                if (soundBar->handleUserInput(event)) {
                    config->soundLevel = musicBar->getLevel();
                    return false;
                }
                if (switchButtonSprite4->getGlobalBounds().contains(sf::Vector2f(mouseButtonPressed->position))) {
                    config->music = !config->music;
                    switchButtonSprite4->setTextureRect( (config->music) ? sf::IntRect({0, 0}, {111, 36}) : sf::IntRect({0, 36}, {111, 36}) );
                    return false;
                }
                if (switchButtonSprite5->getGlobalBounds().contains(sf::Vector2f(mouseButtonPressed->position))) {
                    config->soundEffect = !config->soundEffect;
                    switchButtonSprite5->setTextureRect( (config->soundEffect) ? sf::IntRect({0, 0}, {111, 36}) : sf::IntRect({0, 36}, {111, 36}) );
                    return false;
                }
            }

        }

    }
    return false;
}


void Options::menuActionUpdate(float delta) {

}


void Options::resize(sf::Vector2f scale) {
    // Background
    backgroundSprite->setScale(scale);
    // Buttons
    homeButtonSprite->setScale(scale);
    homeButtonSprite->setPosition(sf::Vector2f{1616.f * scale.x, 53.f * scale.y});
    applyButtonSprite->setScale(scale);
    applyButtonSprite->setPosition(sf::Vector2f{880.f * scale.x, 960.f * scale.y});
    // Button hit box
    selectedHitBoxSprite1->setScale(scale);
    selectedHitBoxSprite1->setPosition(sf::Vector2f{106.f * scale.x, 166.f * scale.y});
    selectedHitBoxSprite2->setScale(scale);
    selectedHitBoxSprite2->setPosition(sf::Vector2f{106.f * scale.x, 324.f * scale.y});
    selectedHitBoxSprite3->setScale(scale);
    selectedHitBoxSprite3->setPosition(sf::Vector2f{106.f * scale.x, 478.f * scale.y});
    // Text
    videoText->setScale(scale);
    {
        sf::FloatRect ts = videoText->getLocalBounds();
        float x = 119.f * scale.x;
        float y = (165.f * scale.y) - ts.size.y / 4.f;
        videoText->setPosition(sf::Vector2f{x, y});
    }
    volumeText->setScale(scale);
    {
        sf::FloatRect ts = volumeText->getLocalBounds();
        float x = 119.f * scale.x;
        float y = (321.f * scale.y) - ts.size.y / 4.f;
        volumeText->setPosition(sf::Vector2f{x, y});
    }
    profileText->setScale(scale);
    {
        sf::FloatRect ts = profileText->getLocalBounds();
        float x = 119.f * scale.x;
        float y = (477.f * scale.y) - ts.size.y / 4.f;
        profileText->setPosition(sf::Vector2f{x, y});
    }
    // VIDEO SECTION
    slider1->setPosition(sf::Vector2f{872.f * scale.x, 525.f * scale.y});
    slider1->setScale(scale);
    slider2->setPosition(sf::Vector2f{868.f * scale.x, 719.f * scale.y});
    slider2->setScale(scale);
    leftArrowSprite->setScale(scale);
    leftArrowSprite->setPosition(sf::Vector2f{815.f * scale.x, 304.f * scale.y});
    rightArrowSprite->setScale(scale);
    rightArrowSprite->setPosition(sf::Vector2f{1100.f * scale.x, 304.f * scale.y});
    switchButtonSprite1->setScale(scale);
    switchButtonSprite1->setPosition(sf::Vector2f{1050.f * scale.x, 166.f * scale.y});
    switchButtonSprite2->setScale(scale);
    switchButtonSprite2->setPosition(sf::Vector2f{1056.f * scale.x, 430.f * scale.y});
    switchButtonSprite3->setScale(scale);
    switchButtonSprite3->setPosition(sf::Vector2f{922.f * scale.x, 700.f * scale.y});
    boxSprite->setScale(scale);
    boxSprite->setPosition(sf::Vector2f{scale.x * 736.f, scale.y * 614.f});
    fullscreenText->setScale(scale);
    {
        sf::FloatRect ts = fullscreenText->getLocalBounds();
        float x = 823.f * scale.x;
        float y = (158.f * scale.y) - ts.size.y / 4.f;
        fullscreenText->setPosition(sf::Vector2f{x, y});
    }
    resolutionText->setScale(scale);
    {
        sf::FloatRect ts = resolutionText->getLocalBounds();
        float x = 891.f * scale.x;
        float y = (246.f * scale.y) - ts.size.y / 4.f;
        resolutionText->setPosition(sf::Vector2f{x, y});
    }
    resolutionRatio->setScale(scale);
    {
        sf::FloatRect ts = resolutionRatio->getLocalBounds();
        float x = 907.f * scale.x;
        float y = (311.f * scale.y) - ts.size.y / 4.f;
        resolutionRatio->setPosition(sf::Vector2f{x, y});
    }
    antialiasingText->setScale(scale);
    {
        sf::FloatRect ts = antialiasingText->getLocalBounds();
        float x = 807.f * scale.x;
        float y = (421.f * scale.y) - ts.size.y / 4.f;
        antialiasingText->setPosition(sf::Vector2f{x, y});
    }
    frameRateText->setScale(scale);
    {
        sf::FloatRect ts = frameRateText->getLocalBounds();
        float x = 756.f * scale.x;
        float y = (614.f * scale.y) - ts.size.y / 4.f;
        frameRateText->setPosition(sf::Vector2f{x, y});
    }
    verticalSyncText->setScale(scale);
    {
        sf::FloatRect ts = verticalSyncText->getLocalBounds();
        float x = 995.f * scale.x;
        float y = (614.f * scale.y) - ts.size.y / 4.f;
        verticalSyncText->setPosition(sf::Vector2f{x, y});
    }
    // VOLUME SECTION
    musicBar->setPosition(sf::Vector2f{941.f * scale.x, 247.f * scale.y});
    musicBar->setScale(scale);
    soundBar->setPosition(sf::Vector2f{941.f * scale.x, 441.f * scale.y});
    soundBar->setScale(scale);
    switchButtonSprite4->setScale(scale);
    switchButtonSprite4->setPosition(sf::Vector2f{806.f * scale.x, 241.f * scale.y});
    switchButtonSprite5->setScale(scale);
    switchButtonSprite5->setPosition(sf::Vector2f{806.f * scale.x, 435.f * scale.y});
    musicText->setScale(scale);
    {
        sf::FloatRect ts = musicText->getLocalBounds();
        float x = 943.f * scale.x;
        float y = (176.f * scale.y) - ts.size.y / 4.f;
        musicText->setPosition(sf::Vector2f{x, y});
    }
    soundEffectText->setScale(scale);
    {
        sf::FloatRect ts = soundEffectText->getLocalBounds();
        float x = 880.f * scale.x;
        float y = (373.f * scale.y) - ts.size.y / 4.f;
        soundEffectText->setPosition(sf::Vector2f{x, y});
    }
    // PROFILE SECTION
    textBoxSprite1->setScale(scale);
    textBoxSprite1->setPosition(sf::Vector2f{797.f * scale.x, 251.f * scale.y});
    textBoxSprite2->setScale(scale);
    textBoxSprite2->setPosition(sf::Vector2f{797.f * scale.x, 516.f * scale.y});
    statisticsText->setScale(scale);
    {
        sf::FloatRect ts = statisticsText->getLocalBounds();
        float x = 905.f * scale.x;
        float y = (190.f * scale.y) - ts.size.y / 4.f;
        statisticsText->setPosition(sf::Vector2f{x, y});
    }
    accountText->setScale(scale);
    {
        sf::FloatRect ts = accountText->getLocalBounds();
        float x = 922.f * scale.x;
        float y = (454.f * scale.y) - ts.size.y / 4.f;
        accountText->setPosition(sf::Vector2f{x, y});
    }
}


void Options::render(sf::RenderWindow &window) {
    window.draw(*backgroundSprite);
    window.draw(*selectedHitBoxSprite1);
    window.draw(*selectedHitBoxSprite2);
    window.draw(*selectedHitBoxSprite3);
    window.draw(*homeButtonSprite);
    window.draw(*applyButtonSprite);
    window.draw(*videoText);
    window.draw(*volumeText);
    window.draw(*profileText);
    if (videoFlag) {
        window.draw(*boxSprite);
        window.draw(*fullscreenText);
        window.draw(*resolutionText);
        window.draw(*resolutionRatio);
        window.draw(*antialiasingText);
        window.draw(*frameRateText);
        window.draw(*verticalSyncText);
        window.draw(*leftArrowSprite);
        window.draw(*rightArrowSprite);
        window.draw(*switchButtonSprite1);
        window.draw(*switchButtonSprite2);
        window.draw(*slider1);
        if (videoFrameBox) {
            window.draw(*slider2);
        } else {
            window.draw(*switchButtonSprite3);
        }
    } else if (volumeFlag) {
        window.draw(*musicText);
        window.draw(*soundEffectText);
        window.draw(*switchButtonSprite4);
        window.draw(*switchButtonSprite5);
        window.draw(*musicBar);
        window.draw(*soundBar);
    } else if (profileFlag) {
        window.draw(*statisticsText);
        window.draw(*accountText);
        window.draw(*textBoxSprite1);
        window.draw(*textBoxSprite2);
    }
}
