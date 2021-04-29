#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <vector>
#include "CelestialBody.hpp"

class Editor
{
public:
	Editor(sf::RenderWindow& window, std::vector<CelestialBody>& celestialBodies);
	~Editor() = default;

	void update();

private:
	sf::RenderWindow& window;
	std::vector<CelestialBody>& celestialBodies;

	CelestialBody* draggedBody{ nullptr };
};
