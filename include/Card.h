#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

using namespace sf;

typedef Vector2f vec2;

enum Suit
{
    CLUB,
    HEART,
    SPADE,
    DIAMOND
};

class Card
{
    public:
        Card(uint8_t value, uint8_t suit, vec2 pos);
        void draw(RenderWindow& rt, vec2 size);
        void setPosition(vec2 pos);
        uint8_t getValue();
        uint8_t getSuit();
    private:
        uint8_t value = 0;
        Texture tex;
        RectangleShape sprite;
        uint8_t suit = 0;
};
