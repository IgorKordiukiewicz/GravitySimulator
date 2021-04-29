#include "../Include/Editor.hpp"
#include <SFML/Graphics.hpp>
#include <imgui.h>

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

	ImGui::Begin("Editor");

	// Update celestial bodies properties
	int imguiId{ 0 };
	for (auto& celestialBody : celestialBodies) {
		ImGui::PushID(imguiId);
		++imguiId;

		// Update mass
		float massInput = celestialBody.getMass();
		ImGui::InputFloat("Mass", &massInput);
		celestialBody.setMass(massInput);

		// Update radius
		float radiusInput = celestialBody.getRadius();
		ImGui::InputFloat("Radius", &radiusInput);
		celestialBody.setRadius(radiusInput);

		// Update initial position
		float positionInput[2] = { celestialBody.getInitialPosition().x, celestialBody.getInitialPosition().y };
		ImGui::InputFloat2("Position", positionInput);
		celestialBody.setInitialPosition({ positionInput[0], positionInput[1] });
	
		// Update initial velocity
		float velocityInput[2] = { celestialBody.getInitialVelocity().x, celestialBody.getInitialVelocity().y };
		ImGui::InputFloat2("Velocity", velocityInput);
		celestialBody.setInitialVelocity({ velocityInput[0], velocityInput[1] });

		ImGui::Separator();

		ImGui::PopID();
	}

	ImGui::End();
}
