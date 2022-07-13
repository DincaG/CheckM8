#pragma once

#include "SFML/Graphics.hpp"
#include "RoundedRectangleShape.h"

class Button : public sf::Transformable, public sf::Drawable
{
public:

    enum class Transition
    {
        In,
        Out
    };

    Button(float width = 0.f, float height = 0.f, float radius = 1.f);
    void setTextString(const std::string& string);
    void setTextFont(const std::string& fontPath);
    void setTextSize(float percentage);
    void setTextFillColor(const sf::Color& color);
    void setTextOutlineColor(const sf::Color& color);
    void setTextOutlineThickness(float thickness);
    void setTextStyle(sf::Text::Style style);
    void setFillColor(const sf::Color& color);
    void setOutlineColor(const sf::Color& color);
    void setOutlineThickness(float thickness);
    void setTexture(const sf::Texture* texture);
    void setSize(const sf::Vector2f& size);
    void setCornerRadius(float radius);
    void setCornerPointCount(unsigned int count);
    void setRelativePosition(float percentageX, float percentageY);
    void setRelativePosition(const sf::Vector2f& percentage);
    void setPosition(float x, float y);
    void setPosition(const sf::Vector2f& position);
    const sf::String& getTextString() const;
    const sf::Font* getTextFont() const;
    float getTextSize() const;
    const sf::Color& getTextFillColor() const;
    const sf::Color& getTextOutlineColor() const;
    float getTextOutlineThickness() const;
    sf::Text::Style getTextStyle() const;
    const sf::Color& getFillColor() const;
    const sf::Color& getOutlineColor() const;
    float getOutlineThickness() const;
    const sf::Texture* getTexture() const;
    const sf::Vector2f& getSize() const;
    float getCornerRadius() const;
    unsigned int getPointCount() const;
    const sf::Vector2f& getPosition() const;
    sf::FloatRect getTextLocalBounds() const;
    sf::FloatRect getTextGlobalBounds() const;
    sf::FloatRect getLocalBounds() const;
    sf::FloatRect getGlobalBounds() const;
    bool isVisible() const;
    bool isHovered() const;
    void show();
    void hide();
    void setUserData(void* userdata);
    void setParent(sf::Transformable& parent);
    void setParent(sf::RenderWindow& parent);
    void setHoverBehavior(void (*hoverBehavior)(Button*, Transition, void*));
    void setPressBehavior(void (*pressBehavior)(Button*, Transition, void*));
    void setAction(void (*action)(void*));
    void onHover(int x, int y);
    void onPress();
    void onRelease();

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    sf::Transform& applyChanges(sf::Transform& transform) const;
    void centerText();

    sf::Text text;
    sf::Font font;
    sf::RoundedRectangleShape box;
    sf::Transformable* parentObject;
    sf::RenderWindow* parentWindow;
    sf::Vector2f relativePosition;
    float relativeCharacterSize;
    bool dynamicallyPositioned;
    bool visibility;
    bool hovered;
    bool pressed;
    void (*onHoverBehavior)(Button*, Transition, void*);
    void (*onPressBehavior)(Button*, Transition, void*);
    void (*trigger)(void*);
    void* userData;
};