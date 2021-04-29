#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include "../Include/Universe.hpp"
#include "../Include/Editor.hpp"

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
	Editor editor;
};