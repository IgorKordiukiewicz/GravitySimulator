#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include "../Include/Universe.hpp"

class Application
{
public:
	Application();
	~Application() = default;

	void run();

private:
	void processEvents();

private:
	sf::RenderWindow window;
	Universe universe;
};