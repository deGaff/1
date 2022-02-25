

#include "inner_tangent.h"
#include <cmath>

Tangent::Tangent() {
    circle1.setFillColor(sf::Color(100, 250, 50));
    circle2.setFillColor(sf::Color(100, 50, 250));

}

std::istream& operator>>(std::istream& str, Tangent& tangent) {
    float radius, x, y;

    std::cout << "Enter first circle radius and position\n";
    str >> radius >> x >> y;
    tangent.circle1.setRadius(radius);
    tangent.circle1.move(x, y);

    std::cout << "\nEnter second circle radius and position\n";
    str >> radius >> x >> y;
    tangent.circle2.setRadius(radius);
    tangent.circle2.move(x, y);

    tangent.update();
    return str;
}

void Tangent::update() {
    auto& LEFT_CIRC = (circle1.getPosition().x < circle2.getPosition().x)
            ? circle1 : circle2;
    auto& RIGHT_CIRC = (circle1.getPosition().x > circle2.getPosition().x)
                  ? circle1 : circle2;

    auto DOWN_CIRC = (circle1.getPosition().y > circle2.getPosition().y)
                ? circle1 : circle2;
    auto UP_CIRC = (circle1.getPosition().y < circle2.getPosition().y)
                 ? circle1 : circle2;

    auto left = LEFT_CIRC.getPosition();
    auto right = RIGHT_CIRC.getPosition();
    auto down = DOWN_CIRC.getPosition();
    auto up = UP_CIRC.getPosition();

    left.y = MODE_HEIGHT - left.y - LEFT_CIRC.getRadius();
    right.y = MODE_HEIGHT - right.y- RIGHT_CIRC.getRadius();
    down.y = MODE_HEIGHT - down.y- DOWN_CIRC.getRadius();
    up.y = MODE_HEIGHT - up.y- UP_CIRC.getRadius();

    left.x += LEFT_CIRC.getRadius();
    right.x += RIGHT_CIRC.getRadius();
    down.x += LEFT_CIRC.getRadius();
    up.x += LEFT_CIRC.getRadius();

    CIRCLE right_aux = {{right.x, right.y},
                        (circle1.getRadius()+circle2.getRadius())*
                        (circle1.getRadius()+circle2.getRadius())},
            center_aux = {{((right.x - left.x) / 2) + left.x, ((up.y - down.y) / 2) + down.y},
                          0.f};

    center_aux.rad_sqr = (center_aux.pos.x - left.x) * (center_aux.pos.x - left.x) +
            (center_aux.pos.y - left.y) * (center_aux.pos.y - left.y);

    LINE aux_cross_line = {2*(center_aux.pos.x - right.x), 2*(center_aux.pos.y-right.y),
                           center_aux.rad_sqr - right_aux.rad_sqr + (right.x * right.x)
                           - (center_aux.pos.x * center_aux.pos.x) + (right.y * right.y)
                           - (center_aux.pos.y * center_aux.pos.y)};

    POINT aux_cross_point = line_circle_crossing_point(right_aux, aux_cross_line);

    LINE right_crossing_line = {aux_cross_point.y-right.y, right.x-aux_cross_point.x,
                                aux_cross_point.x*right.y-aux_cross_point.y*right.x};

    POINT right_tangent = line_circle_crossing_point({{right.x, right.y},
        RIGHT_CIRC.getRadius()*RIGHT_CIRC.getRadius()}, right_crossing_line);

    LINE tangent_line = {-right_crossing_line.B, right_crossing_line.A ,
                    -right_crossing_line.A * right_tangent.y + right_crossing_line.B * right_tangent.x};

    POINT left_tangent = line_circle_crossing_point({{left.x, left.y},
        LEFT_CIRC.getRadius()*LEFT_CIRC.getRadius()}, tangent_line);

    line[0] = sf::Vertex(sf::Vector2f(left_tangent.x, MODE_HEIGHT - left_tangent.y ));
    line[1] = sf::Vertex(sf::Vector2f(right_tangent.x, MODE_HEIGHT - right_tangent.y));


}

POINT  line_circle_crossing_point(const CIRCLE& circle, const LINE& line) {
    POINT a= {0, 0}, b= {0, 0};

    float denominator = (line.A*line.A
                         + line.B*line.B);

    float c0 = line.A * circle.pos.x
                   + line.B*circle.pos.y + line.C;

    float cross_x0 = - (line.A * c0) / denominator,
            cross_y0 = - (line.B * c0) / denominator;

    float crossing_d = (circle.rad_sqr - (c0 * c0) /
                                         denominator);

    if(crossing_d < 0) return {cross_x0+circle.pos.x, cross_y0+circle.pos.y}; //TANGENT POINT

    float mult = sqrt(crossing_d/denominator);

    a.x = (cross_x0 + line.B*mult)+circle.pos.x;
    a.y = (cross_y0 - line.A*mult)+circle.pos.y;
    b.x = (cross_x0 - line.B*mult)+circle.pos.x;
    b.y = (cross_y0 + line.A*mult)+circle.pos.y;

    if(a.x < b.x) {
        return a;
    }
    if(a.x == b.x && a.y > b.y) {
        return a;
    }
    return b;
}

