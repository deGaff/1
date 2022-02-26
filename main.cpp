#include <iostream>
#include <sstream>
#include "inner_tangent.h"
#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(MODE_WIDTH, MODE_HEIGHT), "My window");

    Tangent tangent;
//    std::istringstream test("40 150 150\n70 400 400");
//    test >> tangent;


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Enter)
                {
                    std::cin >> tangent;
                }
            }
        }

        window.clear(sf::Color::Black);

        window.draw(tangent.get_circ1());
        window.draw(tangent.get_circ2());
        window.draw(tangent.get_line(), 2, sf::Lines);

        window.display();
    }

    return 0;
}