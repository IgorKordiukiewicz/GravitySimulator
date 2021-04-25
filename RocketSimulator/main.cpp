#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <imgui.h>

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 800), "Window");

	sf::Clock clock;
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		window.clear();

		window.display();
	}
}