#include "../Include/Editor.hpp"
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include "../Include/ArrowShape.hpp"
#include <iostream>

Editor::Editor(sf::RenderWindow& window, std::vector<CelestialBody>& celestialBodies)
	: window(window)
	, celestialBodies(celestialBodies)
{
	
}

void Editor::update()
{	
	// Check if mouse is clicked and hovered over a celestial body or the body's velocity arrow
	for (auto& celestialBody : celestialBodies) {
		const auto& bodyShape = celestialBody.getBodyShape();
		if (bodyShape.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)))
			&& sf::Mouse::isButtonPressed(sf::Mouse::Left) && !grabbedBody) {
			grabbedBody = &celestialBody;
			grabbedArrowHead = false;
			mousePosOnSelect = sf::Vector2f(sf::Mouse::getPosition(window));
		}

		const auto & arrowShape = celestialBody.getVelocityArrowShape();
		if (arrowShape.contains(sf::Vector2f(sf::Mouse::getPosition(window)))
			&& sf::Mouse::isButtonPressed(sf::Mouse::Left) && !grabbedBody) {
			grabbedBody = &celestialBody;
			grabbedArrowHead = true;
			mousePosOnSelect = sf::Vector2f(sf::Mouse::getPosition(window));
		}
	}

	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		grabbedBody = nullptr;
	}
	// Drag the selected body or the body's velocity arrow head with the mouse cursor
	if (grabbedBody) {
		const auto mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
		const sf::Vector2f posDiff = mousePos - mousePosOnSelect;
		mousePosOnSelect = mousePos;
		if (grabbedArrowHead) {
			grabbedBody->setInitialVelocity(grabbedBody->getInitialVelocity() + posDiff);
		}
		else {
			grabbedBody->setInitialPosition(grabbedBody->getInitialPosition() + posDiff);
		}
		//grabbedBody->setInitialPosition(mousePos);
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
		
		// Update shape color
		const auto color = celestialBody.getBodyShape().getFillColor();
		float colorInput[3] = { static_cast<float>(color.r) / 255.f, 
			static_cast<float>(color.g) / 255.f, 
			static_cast<float>(color.b) / 255.f};
		ImGui::ColorEdit3("Color", colorInput);
		celestialBody.setColor({ static_cast<sf::Uint8>(colorInput[0] * 255.f), 
			static_cast<sf::Uint8>(colorInput[1] * 255.f), 
			static_cast<sf::Uint8>(colorInput[2] * 255.f) });

		ImGui::Separator();

		ImGui::PopID();
	}

	ImGui::End();
}
