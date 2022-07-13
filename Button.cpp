#include "Button.h"

Button::Button(float width, float height, float radius)
    : box{ sf::Vector2f(width, height), radius, 20 }
    , parentObject{ nullptr }
    , parentWindow{ nullptr }
    , relativePosition{ 0.f, 0.f }
    , relativeCharacterSize{ 50.f }
    , dynamicallyPositioned{ false }
    , visibility{ true }
    , hovered{ false }
    , pressed{ false }
    , onHoverBehavior{ nullptr }
    , onPressBehavior{ nullptr }
    , trigger{ nullptr }
    , userData{ nullptr }
{
    setOrigin(width / 2.f, height / 2.f);
    setTextSize(relativeCharacterSize);
}

void Button::setTextString(const std::string& string)
{
    text.setString(string);
    centerText();
}

void Button::setTextFont(const std::string& fontPath)
{
    font.loadFromFile(fontPath);
    text.setFont(font);
    centerText();
}

void Button::setTextSize(float percentage)
{
    text.setCharacterSize(box.getLocalBounds().height * (percentage / 100.f));
    relativeCharacterSize = percentage;
    centerText();
}

void Button::setTextFillColor(const sf::Color& color)
{
    text.setFillColor(color);
}

void Button::setTextOutlineColor(const sf::Color& color)
{
    text.setOutlineColor(color);
}

void Button::setTextOutlineThickness(float thickness)
{
    text.setOutlineThickness(thickness);
}

void Button::setTextStyle(sf::Text::Style style)
{
    text.setStyle(style);
}

void Button::setFillColor(const sf::Color& color)
{
    box.setFillColor(color);
}

void Button::setOutlineColor(const sf::Color& color)
{
    box.setOutlineColor(color);
}

void Button::setOutlineThickness(float thickness)
{
    box.setOutlineThickness(thickness);
}

void Button::setTexture(const sf::Texture* texture)
{
    box.setTexture(texture, true);
}

void Button::setSize(const sf::Vector2f& size)
{
    box.setSize(size);
    setOrigin(getLocalBounds().width / 2.f, getLocalBounds().height / 2.f);
    setTextSize(relativeCharacterSize);
    centerText();
}

void Button::setCornerRadius(float radius)
{
    box.setCornersRadius(radius);
}

void Button::setCornerPointCount(unsigned int count)
{
    box.setCornerPointCount(count);
}

void Button::setRelativePosition(float percentageX, float percentageY)
{
    Button::setRelativePosition(sf::Vector2f(percentageX, percentageY));
}

void Button::setRelativePosition(const sf::Vector2f& percentage)
{
    Transformable::setPosition(0.f, 0.f);
    relativePosition = percentage;
    dynamicallyPositioned = true;
}

void Button::setPosition(float x, float y)
{
    setPosition(sf::Vector2f(x, y));
}

void Button::setPosition(const sf::Vector2f& position)
{
    Transformable::setPosition(position);
    dynamicallyPositioned = false;
}

const sf::String& Button::getTextString() const
{
    return text.getString();
}

const sf::Font* Button::getTextFont() const
{
    return &font;
}

float Button::getTextSize() const
{
    return relativeCharacterSize;
}

const sf::Color& Button::getTextFillColor() const
{
    return text.getFillColor();
}

const sf::Color& Button::getTextOutlineColor() const
{
    return text.getOutlineColor();
}

float Button::getTextOutlineThickness() const
{
    return text.getOutlineThickness();
}

sf::Text::Style Button::getTextStyle() const
{
    return (sf::Text::Style)text.getStyle();
}

const sf::Color& Button::getFillColor() const
{
    return box.getFillColor();
}

const sf::Color& Button::getOutlineColor() const
{
    return box.getOutlineColor();
}

float Button::getOutlineThickness() const
{
    return box.getOutlineThickness();
}

const sf::Texture* Button::getTexture() const
{
    return box.getTexture();
}

const sf::Vector2f& Button::getSize() const
{
    return box.getSize();
}

float Button::getCornerRadius() const
{
    return box.getCornersRadius();
}

unsigned int Button::getPointCount() const
{
    return box.getPointCount();
}

const sf::Vector2f& Button::getPosition() const
{
    sf::Transform transform{ getTransform() };
    return applyChanges(transform).transformPoint(getOrigin());
}

sf::FloatRect Button::getTextLocalBounds() const
{
    return text.getLocalBounds();
}

sf::FloatRect Button::getTextGlobalBounds() const
{
    sf::Transform transform{ getTransform() };
    return applyChanges(transform).transformRect(getTextLocalBounds());
}

sf::FloatRect Button::getLocalBounds() const
{
    return box.getLocalBounds();
}

sf::FloatRect Button::getGlobalBounds() const
{
    sf::Transform transform{ getTransform() };
    return applyChanges(transform).transformRect(getLocalBounds());
}

bool Button::isVisible() const
{
    return visibility;
}

bool Button::isHovered() const
{
    return hovered;
}

void Button::show()
{
    visibility = true;
}

void Button::hide()
{
    visibility = false;
}

void Button::setUserData(void* userdata)
{
    userData = userdata;
}

void Button::setParent(sf::Transformable& parent)
{
    parentObject = &parent;
    parentWindow = nullptr;
}

void Button::setParent(sf::RenderWindow& parent)
{
    parentWindow = &parent;
    parentObject = nullptr;
}

void Button::setHoverBehavior(void (*hoverBehavior)(Button*, Transition, void*))
{
    onHoverBehavior = hoverBehavior;
}

void Button::setPressBehavior(void (*pressBehavior)(Button*, Transition, void*))
{
    onPressBehavior = pressBehavior;
}

void Button::setAction(void (*action)(void*))
{
    trigger = action;
}

void Button::onHover(int x, int y)
{
    if (!hovered && getGlobalBounds().contains(x, y))
    {
        if (onHoverBehavior) onHoverBehavior(this, Transition::In, userData);
        hovered = true;
    }
    else if (hovered && !getGlobalBounds().contains(x, y))
    {
        if (onHoverBehavior) onHoverBehavior(this, Transition::Out, userData);
        hovered = false;
    }
}

void Button::onPress()
{
    if (!pressed && hovered)
    {
        if (onPressBehavior) onPressBehavior(this, Transition::In, userData);
        pressed = true;
    }
}

void Button::onRelease()
{
    if (pressed)
    {
        if (onPressBehavior) onPressBehavior(this, Transition::Out, userData);
        pressed = false;
        if (hovered)
        {
            if (trigger) trigger(userData);
        }
    }
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::Transform combinedTransform{ states.transform * getTransform() };
    applyChanges(combinedTransform);

    target.draw(box, combinedTransform);
    if (text.getFont()) target.draw(text, combinedTransform);
}

sf::Transform& Button::applyChanges(sf::Transform& transform) const
{
    sf::Vector2f parentSize{ -1.f, -1.f };
    if (parentObject)
    {
        transform = parentObject->getTransform() * transform;
        parentSize = sf::Vector2f(((sf::Sprite*)parentObject)->getLocalBounds().width, ((sf::Sprite*)parentObject)->getLocalBounds().height);
    }
    else if (parentWindow)
    {
        parentSize = sf::Vector2f(parentWindow->getSize().x, parentWindow->getSize().y);
    }
    if (dynamicallyPositioned && parentSize != sf::Vector2f{ -1.f, -1.f })
    {
        float rotation{ this->getRotation() };
        transform.rotate(-rotation);
        transform.translate(relativePosition.x / 100.f * parentSize.x, relativePosition.y / 100.f * parentSize.y);
        transform.rotate(rotation);
    }
    return transform;
}

void Button::centerText()
{
    text.setOrigin(text.getLocalBounds().left, text.getLocalBounds().top);

    float offsetX{ (box.getLocalBounds().width - text.getLocalBounds().width) / 2 };
    float offsetY{ (box.getLocalBounds().height - text.getLocalBounds().height) / 2 };
    text.setPosition(box.getGlobalBounds().left + offsetX, box.getGlobalBounds().top + offsetY);
}