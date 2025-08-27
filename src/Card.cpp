#include "Card.h"

Card::Card(uint8_t value, uint8_t suit, vec2 pos = vec2(0, 0))
{
    this->value = value;
    this->suit = suit;
    sprite.setPosition(pos);
    tex.loadFromFile("res/cards.png");
    sprite.setTextureRect(Rect<int>({value * 54, suit * 73}, {54, 73}));
}

void Card::draw(RenderWindow& rt, vec2 size)
{
    sprite.setTexture(&tex);
    sprite.setSize(size);
    sprite.setOrigin(size*0.5f);
    rt.draw(sprite);
}

void Card::setPosition(vec2 pos)
{
    sprite.setPosition(pos);
}

uint8_t Card::getValue()
{
    return value;
}

uint8_t Card::getSuit()
{
    return suit;
}
