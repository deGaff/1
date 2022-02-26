

#ifndef INC_1_INNER_TANGENT_H
#define INC_1_INNER_TANGENT_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <tuple>

#define MODE_HEIGHT 600
#define MODE_WIDTH 800


struct POINT{
    float x,
            y;
};

struct CIRCLE{
    POINT pos;
    float rad_sqr;
};

struct LINE {
    float A,
    B,
    C;
};

class Tangent {
public:
    Tangent();
    friend std::istream& operator>>(std::istream&, Tangent& tangent);
    void update();
    sf::CircleShape& get_circ1() { return circle1; }
    sf::CircleShape& get_circ2() { return circle2; }
    sf::Vertex* get_line() { return line; }
private:
    sf::CircleShape circle1, circle2;
    sf::Vertex line[2];
};

std::istream& operator>>(std::istream&, sf::CircleShape& circle);

POINT  line_circle_crossing_point(const CIRCLE& circle, const LINE& line);

#endif //INC_1_INNER_TANGENT_H
