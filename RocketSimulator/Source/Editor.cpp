#include "../Include/Editor.hpp"
#include <SFML/Graphics.hpp>

Editor::Editor(sf::RenderWindow& window, std::vector<CelestialBody>& celestialBodies)
	: window(window)
	, celestialBodies(celestialBodies)
{
}

void Editor::update()
{
	// Check if mouse is clicked and hovered over a celestial body
	for (auto& celestialBody : celestialBodies) {
		const auto& bodyShape = celestialBody.getBodyShape();
		if (bodyShape.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)))
			&& sf::Mouse::isButtonPressed(sf::Mouse::Left) && !draggedBody) {
			draggedBody = &celestialBody;
		}
	}

	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		draggedBody = nullptr;
	}
	// Drag the selected body with the mouse cursor
	if (draggedBody) {
		draggedBody->setInitialPosition(sf::Vector2f(sf::Mouse::getPosition(window)));
	}
}
