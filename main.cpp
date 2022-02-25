#include <iostream>
#include <sstream>
#include "inner_tangent.h"
#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, MODE_HEIGHT), "My window");

    Tangent tangent;
//    std::istringstream test("40 150 150\n70 400 400");
//    test >> tangent;

    std::cin >> tangent;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }


        window.clear(sf::Color::Black);

        window.draw(tangent.get_circ1());
        window.draw(tangent.get_circ2());
        auto a = tangent.get_line();
        window.draw(a, 4, sf::Lines);

        window.display();
    }

    return 0;
}