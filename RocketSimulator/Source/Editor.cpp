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
		grabBody();
	}

	ImGui::Begin("Editor");

	updateSimulationState();

	updateCentralBody();

	updateDrawTrailsOption();

	// Only allow adding new bodies or changing the bodies properties when the simulation is not running
	if (universe.getSimulationState() == SimulationState::Reset) {
		// Add new bodies
		if (ImGui::Button("Add new body")) {
			universe.createNewBody();
		}
		ImGui::Separator();

		updateCelestialBodiesProperties();
		ImGui::Separator();

		ImGui::Text("Rocket");
		// Add rocket button only if there is no rocket added
		if (!universe.getRocket()) {
			ImGui::SameLine();
			if (ImGui::Button("Add rocket")) {
				universe.createRocket();
			}
		}
		// Remove rocket button only if there is a rocket added
		else {
			ImGui::SameLine();
			if (ImGui::Button("Remove rocket")) {
				universe.removeRocket();
				bodyOrbitedByRocketId = std::nullopt;
			}
		}

		updateRocket();
	}

	ImGui::End();
}

void Editor::grabBody()
{
	// Mouse position has to be mapped, because if the window's view is not the default view,
	// the mouse position would be incorrect
	const sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
	const sf::Vector2f mousePos = window.mapPixelToCoords(mousePixelPos);

	for (auto& celestialBody : universe.getCelestialBodies()) {
		// Check if mouse is clicked and hovered over the body's shape
		const auto& bodyShape = celestialBody.getBodyShape();
		if (bodyShape.getGlobalBounds().contains(mousePos)
			&& sf::Mouse::isButtonPressed(sf::Mouse::Left)
			&& !grabbedBody // Prevents switching to another body when the currently grabbed body is dragged over it
			&& (centralBodyId.has_value() ? centralBodyId.value() != celestialBody.getId() : true)) { // Prevents grabbing a central body
			grabbedBody = &celestialBody;
			grabbedArrowHead = false;
			mousePosOnSelect = mousePos;
		}

		// Check if mouse is clicked and hovered over the body's velocity arrow shape
		const auto& arrowShape = celestialBody.getVelocityArrowShape();
		if (arrowShape.contains(mousePos)
			&& sf::Mouse::isButtonPressed(sf::Mouse::Left)
			&& !grabbedBody) { // Prevents switching to another body when the currently grabbed body is dragged over it
			grabbedBody = &celestialBody;
			grabbedArrowHead = true;
			mousePosOnSelect = mousePos;
		}
	}

	// Check if rocket is grabbed
	// TODO: remove code duplication; code is almost the same as the one for checking if a celestial body is grabbed
	if (auto* rocket = universe.getRocket(); rocket) {
		// Check if mouse is clicked and hovered over the body's shape
		const auto& bodyShape = rocket->getSprite();
		if (bodyShape.getGlobalBounds().contains(mousePos)
			&& sf::Mouse::isButtonPressed(sf::Mouse::Left)
			&& !grabbedBody) { // Prevents switching to another body when the currently grabbed body is dragged over it
			grabbedBody = rocket;
			grabbedArrowHead = false;
			mousePosOnSelect = mousePos;
		}

		// Check if mouse is clicked and hovered over the body's velocity arrow shape
		const auto& arrowShape = rocket->getVelocityArrowShape();
		if (arrowShape.contains(mousePos)
			&& sf::Mouse::isButtonPressed(sf::Mouse::Left)
			&& !grabbedBody // Prevents switching to another body when the currently grabbed body is dragged over it
			&& !bodyOrbitedByRocketId.has_value()) { // Prevents changing the rocket's velocity if it is set to orbit a celestial body
			grabbedBody = rocket;
			grabbedArrowHead = true;
			mousePosOnSelect = mousePos;
		}
	}

	// Check whether the body is no longer grabbed
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		grabbedBody = nullptr;
	}

	// Drag the selected body or the body's velocity arrow head with the mouse cursor
	if (grabbedBody) {
		// Update the position based on the difference between current mouse position and
		// the mouse position when the body was grabbed
		const sf::Vector2f posDiff = mousePos - mousePosOnSelect;
		mousePosOnSelect = mousePos;

		// If the grabbed shape was the body's velocity arrow, update the body's initial velocity, 
		// otherwise update the body's initial position
		if (grabbedArrowHead) {
			grabbedBody->setInitialVelocity(grabbedBody->getInitialVelocity() + posDiff);
		}
		else {
			grabbedBody->setInitialPosition(grabbedBody->getInitialPosition() + posDiff);
		}

		// If position was changed and the body is the celestial body orbited by the rocket, update the rocket's initial orbit
		if (auto* rocket = universe.getRocket(); rocket
			&& posDiff != sf::Vector2f(0.f, 0.f)
			&& dynamic_cast<CelestialBody*>(grabbedBody)
			&& bodyOrbitedByRocketId.value_or(-1) == dynamic_cast<CelestialBody*>(grabbedBody)->getId()) {
			rocket->updateInitialOrbit();
		}
	}
}

void Editor::updateSimulationState()
{
	// Text showing the current simulation state
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

	// Buttons to modify the simulation state
	if (ImGui::Button("Run")) {
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

void Editor::updateCentralBody()
{
	ImGui::Text("Center view around a body:");
	// If central body is set, the preview text of the combo box will display the body's id, and if not it will display "None"
	const std::string previewText = centralBodyId.has_value() ? "Body " + std::to_string(centralBodyId.value()) : "None";

	if (ImGui::BeginCombo("", previewText.c_str())) {
		// Option to not center the view around any body
		if (ImGui::Selectable("None", !centralBodyId.has_value())) {
			centralBodyId = std::nullopt;
			universe.setCentralBody(nullptr);
		}

		for (auto& celestialBody : universe.getCelestialBodies()) {
			// Check whether this body is the body set to be the central body
			bool isSelected = centralBodyId.has_value() ? centralBodyId.value() == celestialBody.getId() : false;

			const std::string bodyStr = "Body " + std::to_string(celestialBody.getId());
			// Option to select this body as the central body
			if (ImGui::Selectable(bodyStr.c_str(), isSelected)) {
				centralBodyId.emplace(celestialBody.getId());
				universe.setCentralBody(&celestialBody);
			}
		}
		ImGui::EndCombo();
	}
}

void Editor::updateDrawTrailsOption()
{
	bool drawTrails = universe.getDrawTrails();
	ImGui::Checkbox("Trails", &drawTrails);
	universe.setDrawTrails(drawTrails);

	ImGui::Separator();
}

void Editor::updateCelestialBodiesProperties()
{
	int imguiId{ 0 };
	for (auto& celestialBody : universe.getCelestialBodies()) {
		ImGui::PushID(imguiId);
		++imguiId;

		const int bodyId = celestialBody.getId();
		// Display body id
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
		const float oldMass = celestialBody.getMass();
		float massInput = celestialBody.getMass();
		ImGui::InputFloat("Mass", &massInput);
		celestialBody.setMass(massInput);
		bool massChanged = oldMass != celestialBody.getMass();

		// Update radius
		const float oldRadius = celestialBody.getRadius();
		float radiusInput = celestialBody.getRadius();
		ImGui::InputFloat("Radius", &radiusInput);
		celestialBody.setRadius(radiusInput);
		bool radiusChanged = oldRadius != celestialBody.getRadius();

		// Update initial position
		const sf::Vector2f oldPosition = celestialBody.getCurrentPosition();
		float positionInput[2] = { celestialBody.getCurrentPosition().x, celestialBody.getCurrentPosition().y };
		ImGui::InputFloat2("Position", positionInput);
		celestialBody.setInitialPosition({ positionInput[0], positionInput[1] });
		bool positionChanged = oldPosition != celestialBody.getInitialPosition();

		// Update initial velocity
		const sf::Vector2f oldVelocity = celestialBody.getCurrentVelocity();
		float velocityInput[2] = { celestialBody.getCurrentVelocity().x, celestialBody.getCurrentVelocity().y };
		ImGui::InputFloat2("Velocity", velocityInput);
		celestialBody.setInitialVelocity({ velocityInput[0], velocityInput[1] });
		bool velocityChanged = oldVelocity != celestialBody.getInitialVelocity();

		// Update shape color
		const auto color = celestialBody.getColor();
		float colorInput[3] = { color.r, color.g, color.b };
		ImGui::ColorEdit3("Color", colorInput);
		celestialBody.setColor(Color{ colorInput[0], colorInput[1], colorInput[2] });

		ImGui::Separator();

		bool anyPropertyChanged = massChanged || radiusChanged || positionChanged || velocityChanged;
		// If there is a rocket, this body is the body initially orbited by the rocket and
		// the body's properties were changed, update the rocket initial orbit
		if (auto* rocket = universe.getRocket(); rocket 
			&& bodyOrbitedByRocketId.has_value() 
			&& bodyOrbitedByRocketId.value() == celestialBody.getId()
			&& anyPropertyChanged) {
			rocket->updateInitialOrbit();
		}

		ImGui::PopID();
	}
}

void Editor::updateRocket()
{
	if (auto* rocket = universe.getRocket(); rocket) {
		ImGui::PushID(-1);

		ImGui::Text("Orbit a body:");
		// If rocket is set to orbit a body, the preview text of the combo box will display the body's id and if not it will display "None"
		const std::string previewText = bodyOrbitedByRocketId.has_value() ? "Body " + std::to_string(bodyOrbitedByRocketId.value()) : "None";

		if (ImGui::BeginCombo("", previewText.c_str())) {
			// Option to not orbit any celestial body
			if (ImGui::Selectable("None", !bodyOrbitedByRocketId.has_value())) {
				bodyOrbitedByRocketId = std::nullopt;
				rocket->setInitialBodyToOrbit(nullptr);
			}

			for (auto& celestialBody : universe.getCelestialBodies()) {
				// Check whether this body is the body set to be orbited by the rocket
				bool isSelected = bodyOrbitedByRocketId.has_value() ? bodyOrbitedByRocketId.value() == celestialBody.getId() : false;
				const std::string bodyStr = "Body " + std::to_string(celestialBody.getId());
				// Option to orbit this body
				if (ImGui::Selectable(bodyStr.c_str(), isSelected)) {
					bodyOrbitedByRocketId.emplace(celestialBody.getId());
					rocket->setInitialBodyToOrbit(&celestialBody);
				}
			}

			ImGui::EndCombo();
		}
		
		if (bodyOrbitedByRocketId.has_value()) {
			// Update initial orbit height
			float initialOrbitHeightInput = rocket->getInitialOrbitHeight();
			ImGui::InputFloat("Orbit Height", &initialOrbitHeightInput);
			rocket->setInitialOrbitHeight(initialOrbitHeightInput);
		}
		// Position and velocity can only be changed if the rocket is set to not orbit any celestial body
		else {
			// Update position
			float positionInput[2] = { rocket->getInitialPosition().x, rocket->getInitialPosition().y };
			ImGui::InputFloat2("Position", positionInput);
			rocket->setInitialPosition({ positionInput[0], positionInput[1] });

			// Update velocity
			float velocityInput[2] = { rocket->getInitialVelocity().x, rocket->getInitialVelocity().y };
			ImGui::InputFloat2("Velocity", velocityInput);
			rocket->setInitialVelocity({ velocityInput[0], velocityInput[1] });
		}
		
		ImGui::PopID();
	}
}
