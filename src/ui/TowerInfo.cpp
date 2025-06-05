#include "TowerInfo.hpp"


TowerInfo::TowerInfo(const std::string& name) {
    ResourceManager& rm = ResourceManager::getInstance();
    sf::Font& font = rm.getFont();
    towerData = rm.getTowerData(name);
    basePosition = sf::Vector2f{0.f, 0.f};

    topBar.setSize(sf::Vector2f{350.f, 70.f});
    topBar.setPosition(sf::Vector2f{0.f, 0.f});
    topBar.setFillColor(sf::Color(217, 217, 217));

    iconBackground.setSize(sf::Vector2f{66.f, 66.f});
    iconBackground.setPosition(sf::Vector2f{0.f, 82.f});
    iconBackground.setFillColor(sf::Color(0x2D, 0x32, 0x35, 128));

    rangeBackground.setSize(sf::Vector2f{125.f, 66.f});
    rangeBackground.setPosition(sf::Vector2f{72.f, 82.f});
    rangeBackground.setFillColor(sf::Color(0x2D, 0x32, 0x35, 128));

    tag1Background.setSize(sf::Vector2f{80.f, 24.f});
    tag1Background.setPosition(sf::Vector2f{203.f, 82.f});
    tag1Background.setFillColor(sf::Color(0x2D, 0x32, 0x35, 128));

    tag2Background.setSize(sf::Vector2f{80.f, 36.f});
    tag2Background.setPosition(sf::Vector2f{203.f, 112.f});
    tag2Background.setFillColor(sf::Color(0x2D, 0x32, 0x35, 128));

    separationBar.setSize(sf::Vector2f{350.f, 15.f});
    separationBar.setPosition(sf::Vector2f{0.f, 512.f});
    separationBar.setFillColor(sf::Color(217, 217, 217));

    classIconTexture = rm.getTexture(towerData.classIconImagePath);
    classIconSprite = new sf::Sprite(classIconTexture);
    {
        auto ts = classIconSprite->getTexture().getSize();
        float sx = (66.f * 1.f) / float(ts.x);
        float sy = (66.f * 1.f) / float(ts.y);
        classIconSprite->setScale(sf::Vector2f{sx, sy});
        classIconSprite->setPosition(sf::Vector2f{0.f, 82.f});
    }
    leftCornerTexture = rm.getTexture("../../images/elements/left_corner_frame.png");
    leftCornerSprite = new sf::Sprite(leftCornerTexture);
    {
        auto ts = leftCornerSprite->getTexture().getSize();
        float sx = (37.f * 1.f) / float(ts.x);
        float sy = (37.f * 1.f) / float(ts.y);
        leftCornerSprite->setScale(sf::Vector2f{sx, sy});
        leftCornerSprite->setPosition(sf::Vector2f{0.f, 162.f});
    }
    rightCornerTexture = rm.getTexture("../../images/elements/right_corner_frame.png");
    rightCornerSprite = new sf::Sprite(rightCornerTexture);
    {
        auto ts = rightCornerSprite->getTexture().getSize();
        float sx = (37.f * 1.f) / float(ts.x);
        float sy = (37.f * 1.f) / float(ts.y);
        rightCornerSprite->setScale(sf::Vector2f{sx, sy});
        rightCornerSprite->setPosition(sf::Vector2f{313, 162});
    }
    towerTexture = rm.getTexture(towerData.towerImagePath_a);
    towerSprite = new sf::Sprite(towerTexture);
    {
        auto ts = towerSprite->getTexture().getSize();
        float sx = (350.f * 1.f) / float(ts.x);
        float sy = (350.f * 1.f) / float(ts.y);
        towerSprite->setScale(sf::Vector2f{sx, sy});
        towerSprite->setPosition(sf::Vector2f{0.f, 162.f});
    }
    nameText = new sf::Text(font, towerData.name, 64);
    nameText->setFillColor(sf::Color::Black);
    {
        sf::FloatRect nb = topBar.getGlobalBounds();
        sf::FloatRect textBounds = nameText->getLocalBounds();
        float x = nb.position.x + (nb.size.x - textBounds.size.x) / 2.f - textBounds.position.x;
        float y = nb.position.y + (nb.size.y - textBounds.size.y) / 2.f - textBounds.position.y;
        nameText->setPosition(sf::Vector2f{x, y});
    }
    rangeLabel = new sf::Text(font, "Attack Range", 24);
    rangeLabel->setFillColor(sf::Color::Black);
    {
        sf::FloatRect nb = rangeBackground.getGlobalBounds();
        sf::FloatRect textBounds = rangeLabel->getLocalBounds();
        float x = nb.position.x + (nb.size.x - textBounds.size.x ) / 2.f - textBounds.position.x;
        float y = nb.position.y + nb.size.y - (textBounds.size.y + (textBounds.size.y / 2.f))  - 4.f;
        rangeLabel->setPosition(sf::Vector2f{x, y});
    }
    tag1Text = new sf::Text(font, towerData.position, 24);
    tag1Text->setFillColor(sf::Color::Black);
    {
        sf::FloatRect nb = tag1Background.getGlobalBounds();
        sf::FloatRect textBounds = tag1Text->getLocalBounds();
        float x = nb.position.x + (nb.size.x - textBounds.size.x) / 2.f - textBounds.position.x;
        float y = nb.position.y + (nb.size.y - textBounds.size.y) / 2.f - textBounds.position.y;
        tag1Text->setPosition(sf::Vector2f{x, y});
    }
    tag2Text = new sf::Text(font, towerData.role, 24);
    tag2Text->setFillColor(sf::Color::Black);
    {
        sf::FloatRect nb = tag2Background.getGlobalBounds();
        sf::FloatRect textBounds = tag2Text->getLocalBounds();
        float x = nb.position.x + (nb.size.x - textBounds.size.x) / 2.f - textBounds.position.x;
        float y = nb.position.y + (nb.size.y - textBounds.size.y) / 2.f - textBounds.position.y;
        tag2Text->setPosition(sf::Vector2f{x, y});
    }
    attributesLabel = new sf::Text(font, "Attributes", 36);
    attributesLabel->setFillColor(sf::Color::White);
    {
        sf::FloatRect textBounds = attributesLabel->getGlobalBounds();
        float y = 531.f - textBounds.size.y / 2.f;
        attributesLabel->setPosition(sf::Vector2f{105.f, y});
    }
    static const std::array<std::string, 6> names = {"Max HP", "ATK", "RES", "ASPD", "Cost", "Block"};
    std::array<std::string, 6> data = {std::to_string(towerData.HP), std::to_string(towerData.ATK), std::to_string(towerData.RES), std::to_string(static_cast<int>(towerData.attackInterval)), std::to_string(towerData.cost), std::to_string(towerData.blockCount)};
    for (int i = 0; i < 6; i++) {
        statNameBackground[i].setSize(sf::Vector2f{70.f, 26.f});
        statNameBackground[i].setPosition(sf::Vector2f{namePos[i].x, namePos[i].y});
        statNameBackground[i].setFillColor(sf::Color(0x2D, 0x32, 0x35));
        statValueBackground[i].setSize(sf::Vector2f{56.f, 26.f});
        statValueBackground[i].setPosition(sf::Vector2f{valPos[i].x, valPos[i].y});
        statValueBackground[i].setFillColor(sf::Color(0x2D, 0x32, 0x35, 128));

        statNameText[i] = new sf::Text(font, names[i], 24);
        statNameText[i]->setFillColor(sf::Color::White);
        {
            sf::FloatRect nb = statNameBackground[i].getGlobalBounds();
            sf::FloatRect textBounds = statNameText[i]->getLocalBounds();
            float x = nb.position.x + 3.f;
            float y = nb.position.y + (nb.size.y - textBounds.size.y) / 2.f - textBounds.position.y;
            statNameText[i]->setPosition(sf::Vector2f{x, y});
        }
        statValueText[i] = new sf::Text(font, data[i], 24);
        statValueText[i]->setFillColor(sf::Color::White);
        {
            sf::FloatRect nb = statValueBackground[i].getGlobalBounds();
            sf::FloatRect textBounds = statValueText[i]->getLocalBounds();
            float x = nb.position.x + 8.f;
            float y = nb.position.y + (nb.size.y - textBounds.size.y) / 2.f - textBounds.position.y;
            statValueText[i]->setPosition(sf::Vector2f{x, y});
        }
    }
    buildRangeSquares(sf::Vector2f{0.f, 0.f});
}


TowerInfo::~TowerInfo() {
    delete classIconSprite;
    delete leftCornerSprite;
    delete rightCornerSprite;
    delete towerSprite;
    delete nameText;
    delete rangeLabel;
    delete tag1Text;
    delete tag2Text;
    delete attributesLabel;
    for (int i = 0; i < statNameText.size(); i++) {
        delete statNameText[i];
        statNameText[i] = nullptr;

        delete statValueText[i];
        statValueText[i] = nullptr;
    }
}


void TowerInfo::setPosition(sf::Vector2f position) {
    topBar.setPosition(position);
    iconBackground.setPosition(sf::Vector2f{0.f, 82.f} + position);
    rangeBackground.setPosition(sf::Vector2f{72.f, 82.f} + position);
    tag1Background.setPosition(sf::Vector2f{203.f, 82.f} + position);
    tag2Background.setPosition(sf::Vector2f{203.f, 112.f} + position);
    separationBar.setPosition(sf::Vector2f{0.f, 512.f} + position);
    for (auto & sq : rangeSquares) {
        sf::Vector2f currentPosition = sq.getPosition() + position;
        sq.setPosition(currentPosition);
    }
    classIconSprite->setPosition(sf::Vector2f{0.f, 82.f} + position);
    leftCornerSprite->setPosition(sf::Vector2f{0.f, 162.f} + position);
    rightCornerSprite->setPosition(sf::Vector2f{313.f, 162.f} + position);
    towerSprite->setPosition(sf::Vector2f{0.f, 162.f} + position);
    {
        sf::FloatRect nb = topBar.getGlobalBounds();
        sf::FloatRect textBounds = nameText->getLocalBounds();
        float x = nb.position.x + (nb.size.x - textBounds.size.x) / 2.f - textBounds.position.x;
        float y = nb.position.y + (nb.size.y - textBounds.size.y) / 2.f - textBounds.position.y;
        nameText->setPosition(sf::Vector2f{x, y});
    }
    {
        sf::FloatRect nb = rangeBackground.getGlobalBounds();
        sf::FloatRect textBounds = rangeLabel->getLocalBounds();
        float x = nb.position.x + (nb.size.x - textBounds.size.x ) / 2.f - textBounds.position.x;
        float y = nb.position.y + nb.size.y - (textBounds.size.y + (textBounds.size.y / 2.f)) - 4.f;
        rangeLabel->setPosition(sf::Vector2f{x, y});
    }
    {
        sf::FloatRect nb = tag1Background.getGlobalBounds();
        sf::FloatRect textBounds = tag1Text->getLocalBounds();
        float x = nb.position.x + (nb.size.x - textBounds.size.x) / 2.f - textBounds.position.x;
        float y = nb.position.y + (nb.size.y - textBounds.size.y) / 2.f - textBounds.position.y;
        tag1Text->setPosition(sf::Vector2f{x, y});
    }
    {
        sf::FloatRect nb = tag2Background.getGlobalBounds();
        sf::FloatRect textBounds = tag2Text->getLocalBounds();
        float x = nb.position.x + (nb.size.x - textBounds.size.x) / 2.f - textBounds.position.x;
        float y = nb.position.y + (nb.size.y - textBounds.size.y) / 2.f - textBounds.position.y;
        tag2Text->setPosition(sf::Vector2f{x, y});
    }
    {
        sf::FloatRect textBounds = attributesLabel->getGlobalBounds();
        float y = 531.f - (textBounds.size.y / 2.f);
        attributesLabel->setPosition(sf::Vector2f{105.f, y} + position);
    }
    for (int i = 0; i < 6; i++) {
        statNameBackground[i].setPosition(sf::Vector2f{namePos[i].x, namePos[i].y}  + position);
        statValueBackground[i].setPosition(sf::Vector2f{valPos[i].x, valPos[i].y} + position);
        {
            sf::FloatRect nb = statNameBackground[i].getGlobalBounds();
            sf::FloatRect textBounds = statNameText[i]->getLocalBounds();
            float x = nb.position.x + 3.f;
            float y = nb.position.y + (nb.size.y - textBounds.size.y) / 2.f - textBounds.position.y;
            statNameText[i]->setPosition(sf::Vector2f{x, y});
        }
        {
            sf::FloatRect nb = statValueBackground[i].getGlobalBounds();
            sf::FloatRect textBounds = statValueText[i]->getLocalBounds();
            float x = nb.position.x + 8.f;
            float y = nb.position.y + (nb.size.y - textBounds.size.y) / 2.f - textBounds.position.y;
            statValueText[i]->setPosition(sf::Vector2f{x, y});
        }
    }
    buildRangeSquares(sf::Vector2f{0.f, 0.f});
    basePosition = position;
}


void TowerInfo::setScale(sf::Vector2f scale) {
    sf::Vector2f newPosition;
    {
        topBar.setSize(sf::Vector2f(350.f * scale.x, 70.f * scale.y));
        newPosition = sf::Vector2f{0.f * scale.x, 0.f * scale.y} + basePosition;
        topBar.setPosition(newPosition);
    }
    {
        iconBackground.setSize(sf::Vector2f(66.f * scale.x, 66.f * scale.y));
        newPosition = sf::Vector2f{0.f * scale.x, 82.f * scale.y} + basePosition;
        iconBackground.setPosition(newPosition);
    }
    {
        rangeBackground.setSize(sf::Vector2f(125.f * scale.x, 66.f * scale.y));
        newPosition = sf::Vector2f{72.f * scale.x, 82.f * scale.y} + basePosition;
        rangeBackground.setPosition(newPosition);
    }
    {
        tag1Background.setSize(sf::Vector2f(80.f * scale.x, 24.f * scale.y));
        newPosition = sf::Vector2f{203.f * scale.x, 82.f * scale.y} + basePosition;
        tag1Background.setPosition(newPosition);
    }
    {
        tag2Background.setSize(sf::Vector2f(80.f * scale.x, 36.f * scale.y));
        newPosition = sf::Vector2f{203.f * scale.x, 112.f * scale.y} + basePosition;
        tag2Background.setPosition(newPosition);
    }
    {
        separationBar.setSize(sf::Vector2f(350.f * scale.x, 15.f * scale.y));
        newPosition = sf::Vector2f{0.f * scale.x, 512.f * scale.y} + basePosition;
        separationBar.setPosition(newPosition);
    }
    {
        auto ts = classIconSprite->getTexture().getSize();
        float sx = (66.f * scale.x) / float(ts.x);
        float sy = (66.f * scale.y) / float(ts.y);
        newPosition = sf::Vector2f{0.f * scale.x, 82.f * scale.y} + basePosition;
        classIconSprite->setScale(sf::Vector2f{sx, sy});
        classIconSprite->setPosition(newPosition);
    }
    {
        auto ts = leftCornerSprite->getTexture().getSize();
        float sx = (37.f * scale.x) / float(ts.x);
        float sy = (37.f * scale.y) / float(ts.y);
        newPosition = sf::Vector2f{0.f * scale.x, 162.f * scale.y} + basePosition;
        leftCornerSprite->setScale(sf::Vector2f{sx, sy});
        leftCornerSprite->setPosition(newPosition);
    }
    {
        auto ts = rightCornerSprite->getTexture().getSize();
        float sx = (37.f * scale.x) / float(ts.x);
        float sy = (37.f * scale.y) / float(ts.y);
        newPosition = sf::Vector2f{313.f * scale.x, 162.f * scale.y} + basePosition;
        rightCornerSprite->setScale(sf::Vector2f{sx, sy});
        rightCornerSprite->setPosition(newPosition);
    }
    {
        auto ts = towerSprite->getTexture().getSize();
        float sx = (350.f * scale.x) / float(ts.x);
        float sy = (350.f * scale.y) / float(ts.y);
        newPosition = sf::Vector2f{0.f * scale.x, 162.f * scale.y} + basePosition;
        towerSprite->setScale(sf::Vector2f{sx, sy});
        towerSprite->setPosition(newPosition);
    }
    nameText->setScale(scale);
    {
        sf::FloatRect nb = topBar.getGlobalBounds();
        sf::FloatRect textBounds = nameText->getLocalBounds();
        float x = nb.position.x + (nb.size.x - (textBounds.size.x * scale.x)) / 2.f - (textBounds.position.x * scale.x);
        float y = nb.position.y + (nb.size.y - (textBounds.size.y * scale.y)) / 2.f - (textBounds.position.y * scale.y);
        nameText->setPosition(sf::Vector2f{x, y});
    }
    rangeLabel->setScale(scale);
    {
        sf::FloatRect nb = rangeBackground.getGlobalBounds();
        sf::FloatRect textBounds = rangeLabel->getLocalBounds();
        float x = nb.position.x + (nb.size.x - textBounds.size.x * scale.x) / 2.f - (textBounds.position.x * scale.x);
        float y = nb.position.y + nb.size.y - textBounds.size.y * scale.y - (textBounds.position.y * scale.y);
        rangeLabel->setPosition(sf::Vector2f{x, y});
    }
    tag1Text->setScale(scale);
    {
        sf::FloatRect nb = tag1Background.getGlobalBounds();
        sf::FloatRect textBounds = tag1Text->getLocalBounds();
        float x = nb.position.x + (nb.size.x - (textBounds.size.x * scale.x)) / 2.f - (textBounds.position.x * scale.x);
        float y = nb.position.y + (nb.size.y - (textBounds.size.y * scale.y)) / 2.f - (textBounds.position.y * scale.y);
        tag1Text->setPosition(sf::Vector2f{x, y});
    }
    tag2Text->setScale(scale);
    {
        sf::FloatRect nb = tag2Background.getGlobalBounds();
        sf::FloatRect textBounds = tag2Text->getLocalBounds();
        float x = nb.position.x + (nb.size.x - (textBounds.size.x * scale.x)) / 2.f - (textBounds.position.x * scale.x);
        float y = nb.position.y + (nb.size.y - (textBounds.size.y * scale.y)) / 2.f - (textBounds.position.y * scale.y);
        tag2Text->setPosition(sf::Vector2f{x, y});
    }
    attributesLabel->setScale(scale);
    {
        sf::FloatRect ts = attributesLabel->getLocalBounds();
        float x = (105.f + ts.size.x / 2.f) * scale.x;
        float y = basePosition.y + (531.f * scale.y) - ts.size.y / 2.f;
        attributesLabel->setPosition(sf::Vector2f{x, y});
    }
    for (int i = 0; i < 6; i++) {
        {
            statNameBackground[i].setSize(sf::Vector2f{70.f * scale.x, 26.f * scale.y});
            newPosition = sf::Vector2f{namePos[i].x * scale.x, namePos[i].y * scale.y} + basePosition;
            statNameBackground[i].setPosition(newPosition);
        }
        {
            statValueBackground[i].setSize(sf::Vector2f{56.f * scale.x, 26.f * scale.y});
            newPosition = sf::Vector2f{valPos[i].x * scale.x, valPos[i].y * scale.y} + basePosition;
            statValueBackground[i].setPosition(newPosition);
        }
        statNameText[i]->setScale(scale);
        {
            sf::FloatRect nb = statNameBackground[i].getGlobalBounds();
            sf::FloatRect textBounds = statNameText[i]->getLocalBounds();
            float x = nb.position.x + 3.f * scale.x;
            float y = nb.position.y + (nb.size.y - (textBounds.size.y * scale.y)) / 2.f - (textBounds.position.y * scale.y);
            statNameText[i]->setPosition(sf::Vector2f{x, y});
        }
        statValueText[i]->setScale(scale);
        {
            sf::FloatRect nb = statValueBackground[i].getGlobalBounds();
            sf::FloatRect textBounds = statValueText[i]->getLocalBounds();
            float x = nb.position.x + (nb.size.x - (textBounds.size.x * scale.x)) / 2.f - (textBounds.position.x * scale.x);
            float y = nb.position.y + (nb.size.y - (textBounds.size.y * scale.y)) / 2.f - (textBounds.position.y * scale.y);
            statValueText[i]->setPosition(sf::Vector2f{x, y});
        }
    }
    buildRangeSquares(scale);
}


void TowerInfo::buildRangeSquares(sf::Vector2f scale) {
    rangeSquares.clear();
    if (towerData.tileRange.empty()) return;

    // 1) find min/max of your tileRange coords
    int minX = towerData.tileRange[0].x, maxX = minX;
    int minY = towerData.tileRange[0].y, maxY = minY;
    for (auto& t : towerData.tileRange) {
        minX = std::min(minX, t.x);
        maxX = std::max(maxX, t.x);
        minY = std::min(minY, t.y);
        maxY = std::max(maxY, t.y);
    }
    int cols = maxX - minX + 1;
    int rows = maxY - minY + 1;

    // 2) compute square+gap in screen pixels
    float Sx = 10.f * scale.x;
    float Sy = 10.f * scale.y;
    float Gx =  2.f * scale.x;
    float Gy =  2.f * scale.y;

    // 3) how big is the whole grid?
    float totalW = cols * Sx + (cols - 1) * Gx;
    float totalH = rows * Sy + (rows - 1) * Gy;

    // 4) get the global bounds of your rangeBg_
    sf::FloatRect B = rangeBackground.getGlobalBounds();

    // 5) figure out label height (screen‐space)
    auto lbl = rangeLabel->getGlobalBounds();
    float labelH = lbl.size.y;

    // 6) pick a vertical start so the grid sits above the label, centered
    float availableH = B.size.y - labelH - 4.f * scale.y;
    float startY = B.position.y + (availableH - totalH)/2.f;

    // 7) pick a horizontal start to center in the 125 px wide box
    float startX = B.position.x + (B.size.x - totalW)/2.f;

    // 8) build each square
    for (auto& t : towerData.tileRange) {
        sf::RectangleShape sq({Sx, Sy});
        sq.setFillColor((t.x == 0 && t.y == 0) ? sf::Color::White : sf::Color::Black);

        // zero-based col/row in your grid
        int cx = t.x - minX;
        int cy = t.y - minY;

        sq.setPosition({startX + cx * (Sx + Gx), startY + cy * (Sy + Gy)});
        rangeSquares.push_back(sq);
    }
}


void TowerInfo::draw(sf::RenderTarget& rt, sf::RenderStates states) const {
    rt.draw(topBar);
    rt.draw(iconBackground);
    rt.draw(rangeBackground);
    rt.draw(tag1Background);
    rt.draw(tag2Background);
    rt.draw(separationBar);
    for(auto& sq: rangeSquares) {
        rt.draw(sq);
    }
    rt.draw(*classIconSprite);
    rt.draw(*towerSprite);
    rt.draw(*leftCornerSprite);
    rt.draw(*rightCornerSprite);
    rt.draw(*nameText);
    rt.draw(*rangeLabel);
    rt.draw(*tag1Text);
    rt.draw(*tag2Text);
    rt.draw(*attributesLabel);
    for (int i = 0; i < 6; i++){
        rt.draw(statNameBackground[i]);
        rt.draw(*statNameText[i]);
        rt.draw(statValueBackground[i]);
        rt.draw(*statValueText[i]);
    }
}
