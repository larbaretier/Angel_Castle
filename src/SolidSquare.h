#ifndef SOLIDSQUARE_H
#define SOLIDSQUARE_H
#include <SFML/Graphics.hpp>
#include "Thing.h"

class SolidSquare : public Thing {
    public:
        SolidSquare(int x, int y, int w, int h);
        virtual ~SolidSquare();
        void draw_thing(sf::RenderWindow &w);
        sf::Vector2f getMysize();
        sf::Vector2f getPosition();

    protected:

    private:
        sf::Vector2f mysize;
        sf::Vector2f position_solid_square;
        sf::RectangleShape shape;
};

#endif // SOLIDSQUARE_H
