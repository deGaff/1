#include "inner_tangent.h"
#include <cmath>

Tangent::Tangent() {
    circle1.setFillColor(sf::Color(0, 0, 0));
    circle2.setFillColor(sf::Color(0, 0, 0));

    circle1.setOutlineThickness(1.f);
    circle2.setOutlineThickness(1.f);

    circle1.setOutlineColor(sf::Color(100, 250, 50));
    circle2.setOutlineColor(sf::Color(100, 50, 250));
}

std::istream& operator>>(std::istream& str, Tangent& tangent) {
    auto intersect = [&tangent]() {
        POINT center1 = { tangent.circle1.getPosition().x + tangent.circle1.getRadius(),
                        tangent.circle1.getPosition().y + tangent.circle1.getRadius() },
            center2 = { tangent.circle2.getPosition().x + tangent.circle2.getRadius(),
                      tangent.circle2.getPosition().y + tangent.circle2.getRadius() };

        float dist_sqr = (center1.x - center2.x) * (center1.x - center2.x) + (center1.y - center2.y) * (center1.y - center2.y);

        float rad_sqr = tangent.circle1.getRadius() + tangent.circle2.getRadius();
        rad_sqr *= rad_sqr;

        return dist_sqr < rad_sqr;
    };

    do {
        std::cout << "Enter first circle radius and position\n";
        str >> tangent.circle1;

        std::cout << "Enter second circle radius and position\n";
        str >> tangent.circle2;

        if (intersect())
            std::cout << "CIRCLES INTERSECT\n";
    } while (intersect());

    tangent.update();
    return str;
}

std::istream& operator>>(std::istream& str, sf::CircleShape& circle) {
    float radius, x, y;

    auto wrong_pos_rad = [&radius, &x, &y]() {
        return radius <= 0 || x < 0 || x + radius * 2 > MODE_WIDTH ||
            y < 0 || y + radius * 2 > MODE_HEIGHT;
    };

    do {
        str >> radius >> x >> y;
        if (wrong_pos_rad()) std::cout <<
            "WRONG POS/RAD\n";
    } while (wrong_pos_rad());
    circle.setRadius(radius);
    circle.setPosition(x, y);

    return str;
}

void Tangent::update() {
    auto& LEFT_CIRC = (circle1.getPosition().x <= circle2.getPosition().x)
        ? circle1 : circle2;
    auto& RIGHT_CIRC = (circle1.getPosition().x > circle2.getPosition().x)
        ? circle1 : circle2;

    auto DOWN_CIRC = (circle1.getPosition().y >= circle2.getPosition().y)
        ? circle1 : circle2;
    auto UP_CIRC = (circle1.getPosition().y < circle2.getPosition().y)
        ? circle1 : circle2;

    auto left = LEFT_CIRC.getPosition();
    auto right = RIGHT_CIRC.getPosition();
    auto down = DOWN_CIRC.getPosition();
    auto up = UP_CIRC.getPosition();

    left.y = MODE_HEIGHT - left.y - LEFT_CIRC.getRadius();
    right.y = MODE_HEIGHT - right.y - RIGHT_CIRC.getRadius();
    down.y = MODE_HEIGHT - down.y - DOWN_CIRC.getRadius();
    up.y = MODE_HEIGHT - up.y - UP_CIRC.getRadius();

    left.x += LEFT_CIRC.getRadius();
    right.x += RIGHT_CIRC.getRadius();
    down.x += DOWN_CIRC.getRadius();
    up.x += UP_CIRC.getRadius();

    CIRCLE right_aux = { {right.x, right.y},
                        (circle1.getRadius() + circle2.getRadius()) *
                        (circle1.getRadius() + circle2.getRadius()) },
        center_aux = { {((right.x - left.x) / 2) + left.x, ((up.y - down.y) / 2) + down.y},
                      0.f };

    center_aux.rad_sqr = (center_aux.pos.x - left.x) * (center_aux.pos.x - left.x) +
        (center_aux.pos.y - left.y) * (center_aux.pos.y - left.y);

    LINE aux_cross_line = { 2 * (center_aux.pos.x - right.x), 2 * (center_aux.pos.y - right.y),
                           center_aux.rad_sqr - right_aux.rad_sqr + (right.x * right.x)
                           - (center_aux.pos.x * center_aux.pos.x) + (right.y * right.y)
                           - (center_aux.pos.y * center_aux.pos.y) };

    POINT aux_cross_point = line_circle_crossing_point(right_aux, aux_cross_line);

    LINE right_crossing_line = { aux_cross_point.y - right.y, right.x - aux_cross_point.x,
                                aux_cross_point.x * right.y - aux_cross_point.y * right.x };

    POINT right_tangent = line_circle_crossing_point({ {right.x, right.y},
        RIGHT_CIRC.getRadius() * RIGHT_CIRC.getRadius() }, right_crossing_line);

    LINE tangent_line = { -right_crossing_line.B, right_crossing_line.A ,
                    -right_crossing_line.A * right_tangent.y + right_crossing_line.B * right_tangent.x };

    POINT left_tangent = line_circle_crossing_point({ {left.x, left.y},
        LEFT_CIRC.getRadius() * LEFT_CIRC.getRadius() }, tangent_line);

    if (abs(left_tangent.x - right_tangent.x) < EPS && abs(left_tangent.y - right_tangent.y) < EPS) {
        POINT specified_right_tangent = line_circle_crossing_point(center_aux, tangent_line), specified_left_tangent;
        specified_left_tangent.x = right_tangent.x * 2 - specified_right_tangent.x;
        specified_left_tangent.y = right_tangent.y * 2 - specified_right_tangent.y;
        line[0] = sf::Vertex(sf::Vector2f(specified_left_tangent.x, MODE_HEIGHT - specified_left_tangent.y));
        line[1] = sf::Vertex(sf::Vector2f(specified_right_tangent.x, MODE_HEIGHT - specified_right_tangent.y));
        return;
    }
    line[0] = sf::Vertex(sf::Vector2f(left_tangent.x, MODE_HEIGHT - left_tangent.y));
    line[1] = sf::Vertex(sf::Vector2f(right_tangent.x, MODE_HEIGHT - right_tangent.y));
    


}

POINT  line_circle_crossing_point(const CIRCLE& circle, const LINE& line) {
    POINT a = { 0, 0 }, b = { 0, 0 };

    float denominator = (line.A * line.A
        + line.B * line.B);

    float c0 = line.A * circle.pos.x
        + line.B * circle.pos.y + line.C;

    float cross_x0 = -(line.A * c0) / denominator,
        cross_y0 = -(line.B * c0) / denominator;

    float crossing_d = (circle.rad_sqr - (c0 * c0) /
        denominator);

    if (crossing_d < EPS) return { cross_x0 + circle.pos.x, cross_y0 + circle.pos.y }; //TANGENT POINT

    float mult = sqrt(crossing_d / denominator);

    a.x = (cross_x0 + line.B * mult) + circle.pos.x;
    a.y = (cross_y0 - line.A * mult) + circle.pos.y;
    b.x = (cross_x0 - line.B * mult) + circle.pos.x;
    b.y = (cross_y0 + line.A * mult) + circle.pos.y;

    if (a.x < b.x) {
        return a;
    }
    if (a.x == b.x && a.y > b.y) {
        return a;
    }
    return b;
}

