#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <vector>
#include "Universe.hpp"
#include <optional>

class Editor
{
public:
	Editor(sf::RenderWindow& window, Universe& universe);

	void update();

private:
	void grabBody();
	void updateSimulationState();
	void updateCentralBody();
	void updateDrawTrailsOption();
	void updateCelestialBodiesProperties();
	void updateRocket();

private:
	sf::RenderWindow& window;
	Universe& universe;

	// Variables for changing the bodies positions or velocities by grabbing their shape or 
	// their velocity arrow shape and moving it using mouse
	Body* grabbedBody{ nullptr };
	// Whether the grabbed shape was the body's shape or its velocity arrow
	bool grabbedArrowHead{ false };
	sf::Vector2f mousePosOnSelect;

	std::optional<int> centralBodyId;
	std::optional<int> bodyOrbitedByRocketId;
};
