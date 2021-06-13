#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include "Universe.hpp"

class CelestialBody;

class Camera
{
public:
	Camera(sf::RenderWindow& window, Universe& universe);

	void update(float deltaTime);

private:
	sf::RenderWindow& window;
	Universe& universe;

	SimulationState lastSimulationState;

	sf::View unlockedView;
	float moveSpeed{ 100.f };
};