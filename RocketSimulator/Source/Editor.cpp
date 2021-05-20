#include "../Include/Editor.hpp"
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include "../Include/ArrowShape.hpp"
#include <iostream>
#include "../Include/Utils.hpp"

Editor::Editor(sf::RenderWindow& window, Universe& universe)
	: window(window)
	, universe(universe)
{
	
}

void Editor::update()
{	
	// Only allow changing bodies positions and velocities when the simulation is not running
	if (universe.getSimulationState() == SimulationState::Reset) {
		// Check if mouse is clicked and hovered over a celestial body or the body's velocity arrow
		const sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
		const sf::Vector2f mousePos = window.mapPixelToCoords(mousePixelPos);
		for (auto& celestialBody : universe.getCelestialBodies()) {
			const auto& bodyShape = celestialBody.getBodyShape();
			if (bodyShape.getGlobalBounds().contains(mousePos)
				&& sf::Mouse::isButtonPressed(sf::Mouse::Left) 
				&& !grabbedBody
				&& (centralBodyId.has_value() ? centralBodyId.value() != celestialBody.getId() : true)) {
				grabbedBody = &celestialBody;
				grabbedArrowHead = false;
				mousePosOnSelect = mousePos;
			}

			const auto& arrowShape = celestialBody.getVelocityArrowShape();
			if (arrowShape.contains(mousePos)
				&& sf::Mouse::isButtonPressed(sf::Mouse::Left) 
				&& !grabbedBody) {
				grabbedBody = &celestialBody;
				grabbedArrowHead = true;
				mousePosOnSelect = mousePos;
			}
		}

		if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			grabbedBody = nullptr;
		}
		// Drag the selected body or the body's velocity arrow head with the mouse cursor
		if (grabbedBody) {
			const sf::Vector2f posDiff = mousePos - mousePosOnSelect;
			mousePosOnSelect = mousePos;
			if (grabbedArrowHead) {
				grabbedBody->setInitialVelocity(grabbedBody->getInitialVelocity() + posDiff);
			}
			else {
				grabbedBody->setInitialPosition(grabbedBody->getInitialPosition() + posDiff);
			}
		}
	}

	ImGui::Begin("Editor");

	updateSimulationState();

	// Select body to center view around
	ImGui::Text("Center view around a body:");
	const std::string previewText = centralBodyId.has_value() ? "Body " + std::to_string(centralBodyId.value()) : "None";
	if (ImGui::BeginCombo("", previewText.c_str())) {
		// None - don't center view around a body
		if (ImGui::Selectable("None", !centralBodyId.has_value())) {
			centralBodyId = std::nullopt;
			universe.setCentralBody(nullptr);
		}
		if (!centralBodyId.has_value()) {
			ImGui::SetItemDefaultFocus();
		}

		for (auto& celestialBody : universe.getCelestialBodies()) {
			bool isSelected = centralBodyId.has_value() ? centralBodyId.value() == celestialBody.getId() : false;
			const std::string bodyStr = "Body " + std::to_string(celestialBody.getId());
			if (ImGui::Selectable(bodyStr.c_str(), isSelected)) {
				centralBodyId.emplace(celestialBody.getId());
				universe.setCentralBody(&celestialBody);
			}
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	bool drawTrails = universe.getDrawTrails();
	ImGui::Checkbox("Trails", &drawTrails);
	universe.setDrawTrails(drawTrails);
	ImGui::Separator();

	if (universe.getSimulationState() == SimulationState::Reset) {
		//Add new bodies
		if (ImGui::Button("Add new body")) {
			universe.createNewBody();
		}
		ImGui::Separator();

		updateCelestialBodiesProperties();
	}

	ImGui::End();
}

void Editor::updateSimulationState()
{
	const std::string simulationStateStr = [this]() {
		switch (universe.getSimulationState()) {
		case SimulationState::Running:
			return "Running";
		case SimulationState::Paused:
			return "Paused";
		case SimulationState::Reset:
			return "Not running";
		}
	}();
	ImGui::Text(std::string("Simulation (" + simulationStateStr + ")").c_str());
	ImGui::SameLine();
	if (ImGui::Button("Start")) {
		universe.runSimulation();
	}
	ImGui::SameLine();
	if (ImGui::Button("Pause")) {
		universe.pauseSimulation();
	}
	ImGui::SameLine();
	if (ImGui::Button("Reset")) {
		universe.resetSimulation();
	}
	ImGui::Separator();
}

void Editor::updateCelestialBodiesProperties()
{
	int imguiId{ 0 };
	for (auto& celestialBody : universe.getCelestialBodies()) {
		ImGui::PushID(imguiId);
		++imguiId;

		const int bodyId = celestialBody.getId();
		const std::string bodyIdStr = "Body " + std::to_string(bodyId);
		ImGui::Text(bodyIdStr.c_str());

		// Delete body
		ImGui::SameLine();
		if (ImGui::Button("Delete")) {
			celestialBody.markToDelete();
			// If the body was the central body, reset the central body and the window's view
			if (centralBodyId.has_value() && centralBodyId.value() == celestialBody.getId()) {
				centralBodyId = std::nullopt;
				universe.setCentralBody(nullptr);
			}
		}

		// Update mass
		float massInput = celestialBody.getMass();
		ImGui::InputFloat("Mass", &massInput);
		celestialBody.setMass(massInput);

		// Update radius
		float radiusInput = celestialBody.getRadius();
		ImGui::InputFloat("Radius", &radiusInput);
		celestialBody.setRadius(radiusInput);

		// Update initial position
		float positionInput[2] = { celestialBody.getCurrentPosition().x, celestialBody.getCurrentPosition().y };
		ImGui::InputFloat2("Position", positionInput);
		celestialBody.setInitialPosition({ positionInput[0], positionInput[1] });

		// Update initial velocity
		float velocityInput[2] = { celestialBody.getCurrentVelocity().x, celestialBody.getCurrentVelocity().y };
		ImGui::InputFloat2("Velocity", velocityInput);
		celestialBody.setInitialVelocity({ velocityInput[0], velocityInput[1] });

		// Update shape color
		const auto color = celestialBody.getColor();
		float colorInput[3] = { color.r, color.g, color.b };
		ImGui::ColorEdit3("Color", colorInput);
		celestialBody.setColor(Color{ colorInput[0], colorInput[1], colorInput[2] });

		ImGui::Separator();

		ImGui::PopID();
	}
}
