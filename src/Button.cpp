#include "Button.h"

Button::Button(vec2 posRatio, vec2 sizeRatio, function<void()> onClick, Color color)
:posRatio(posRatio), sizeRatio(sizeRatio), onClick(onClick), color(color), text(font, "", 23)
{
    tex.loadFromFile("res/button.png");
    font.openFromFile("res/font.ttf");
    text.setFont(font);
    sprite.setTextureRect(Rect<int>({0, 0}, {32, 16}));
    sprite.setFillColor(color);
}

bool isInside(RectangleShape sprite, vec2i mousepos)
{
    return abs(mousepos.x - sprite.getPosition().x) < sprite.getSize().x * 0.5f &&
           abs(mousepos.y - sprite.getPosition().y) < sprite.getSize().y * 0.5f;
}

void Button::update(optional<Event> e, vec2i mousepos)
{
    if(e->is<Event::MouseButtonPressed>() && isInside(sprite, mousepos))
    {
        sprite.setTextureRect(Rect<int>({32, 0}, {32, 16}));
    }
    else if(e->is<Event::MouseButtonReleased>() && isInside(sprite, mousepos))
    {
        sprite.setTextureRect(Rect<int>({0, 0}, {32, 16}));
        onClick();
    }
}

void Button::draw(RenderWindow& window)
{
    vec2u winSize = window.getSize();
    sprite.setTexture(&tex);
    float borderX = winSize.x - winSize.y;
    sprite.setPosition(vec2(borderX + posRatio.x * winSize.y, posRatio.y * winSize.y));
    sprite.setSize(vec2(sizeRatio.x * winSize.y, sizeRatio.y * winSize.y));
    sprite.setOrigin(sprite.getSize() * 0.5f);
    window.draw(sprite);
}
