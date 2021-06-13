#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <vector>
#include "Universe.hpp"
#include <optional>
#include <imgui.h>

class Editor
{
public:
	Editor(sf::RenderWindow& window, Universe& universe);

	void update();

	void setFont(ImFont* newFont);

private:
	void grabBody();
	void updateSimulationState();
	void updateCentralBody();
	void updateDrawTrailsOption();
	void updateCelestialBodiesProperties();

private:
	sf::RenderWindow& window;
	Universe& universe;

	// Variables for changing the bodies positions or velocities by grabbing their shape or 
	// their velocity arrow shape and moving it using mouse
	CelestialBody* grabbedBody{ nullptr };
	// Whether the grabbed shape was the body's shape or its velocity arrow
	bool grabbedArrowHead{ false };
	sf::Vector2f mousePosOnSelect;

	std::optional<int> centralBodyId;

	ImFont* font{ nullptr };

	int screenshotId{ 0 };
};
