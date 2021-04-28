#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

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
};