#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include "../Include/Universe.hpp"
#include "../Include/Editor.hpp"
#include "../Include/Camera.hpp"
#include <imgui.h>

class Application
{
public:
	Application();

	void run();

private:
	void processEvents();

private:
	sf::RenderWindow window;
	Universe universe;
	Editor editor;
	Camera camera;
};