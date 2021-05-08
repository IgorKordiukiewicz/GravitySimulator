#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <vector>
#include "Universe.hpp"
#include <optional>

class Editor
{
public:
	Editor(sf::RenderWindow& window, Universe& universe);
	~Editor() = default;

	void update();

private:
	void updateSimulationState();
	void updateCelestialBodiesProperties();

private:
	sf::RenderWindow& window;
	Universe& universe;

	CelestialBody* grabbedBody{ nullptr };
	bool grabbedArrowHead{ false };
	sf::Vector2f mousePosOnSelect;

	std::optional<int> centralBodyId;
};
