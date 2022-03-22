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

	void zoomIn();
	void zoomOut();

private:
	sf::RenderWindow& window;
	Universe& universe;

	SimulationState lastSimulationState;

	sf::View view;
	sf::View unlockedView;

	float moveSpeed{ 100.f };
	float zoomSpeed{ 0.05f };
	float zoom{ 1.f };
	float maxZoom{ 10.f };
};