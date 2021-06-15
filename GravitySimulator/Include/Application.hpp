#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include "Universe.hpp"
#include "Editor.hpp"
#include "Camera.hpp"
#include "PresetManager.hpp"
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
	PresetManager presetManager;
};