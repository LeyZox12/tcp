#pragma once

#include <functional>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

typedef Vector2f vec2;
typedef Vector2i vec2i;
typedef Vector2u vec2u;

class Button
{
    public:
        Button(vec2 posRatio, vec2 sizeRatio, function<void()> onClick, Color color);
        void update(optional<Event> e, vec2i mouspos);
        void draw(RenderWindow& window);
    private:
        vec2 posRatio;
        vec2 sizeRatio;
        RectangleShape sprite;
        function<void()> onClick;
        Text text;
        Font font;
        Texture tex;
        Color color;
};
