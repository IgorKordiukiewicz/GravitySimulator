#include "../Include/Editor.hpp"
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include "../Include/ArrowShape.hpp"
#include <iostream>
#include "../Include/Utils.hpp"
#include <fstream>
#include <filesystem>

Editor::Editor(sf::RenderWindow& window, Universe& universe, PresetManager& presetManager)
	: window(window)
	, universe(universe)
	, presetManager(presetManager)
{
	// Find screenshot file with the biggest id, so the next available id is one bigger than it,
	// so that screenshots are not overwritten
	auto path = std::filesystem::current_path() / "Screenshots";
	if (std::filesystem::is_directory(path)) {
		int biggestScreenshotId{ 0 };
		for (const auto& it : std::filesystem::directory_iterator(path)) {
			if (it.path().extension() == ".png") {
				const auto fileName = it.path().stem().string();
				const std::string screenshotStr = "screenshot";
				const int currentScreenshotId = std::stoi(fileName.substr(screenshotStr.size()));
				biggestScreenshotId = std::max(biggestScreenshotId, currentScreenshotId);
			}
		}
		screenshotId = biggestScreenshotId + 1;
	}
}

Editor::~Editor()
{
	
}

void Editor::update()
{	
	// Only allow changing bodies positions and velocities when the simulation is not running
	if (universe.getSimulationState() == SimulationState::Reset) {
		grabBody();
	}

	// Set font
	if (font) {
		ImGui::PushFont(font);
	}

	ImGui::Begin("Editor");
	
	updateSimulationState();
	updatePresets();

	// Allow to save screenshots when the simulation is paused
	if (universe.getSimulationState() == SimulationState::Paused) {
		if (ImGui::Button("Save screenshot")) {
			sf::Texture texture;
			texture.create(window.getSize().x, window.getSize().y);
			texture.update(window);

			// Create Screenshots directory if it doesn't exist
			auto screenshotsPath = std::filesystem::current_path() / "Screenshots";
			if (!std::filesystem::is_directory(screenshotsPath)) {
				std::filesystem::create_directory(screenshotsPath);
			}
			// Save texture to file
			if (texture.copyToImage().saveToFile("Screenshots/screenshot" + std::to_string(screenshotId) + ".png")) {
				++screenshotId;
			}
		}
		ImGui::Separator();
	}

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
	}

	if (font) {
		ImGui::PopFont();
	}

	ImGui::End();
}

void Editor::setFont(ImFont* newFont)
{
	font = newFont;
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

void Editor::updatePresets()
{
	if (universe.getSimulationState() == SimulationState::Reset) {
		if (ImGui::CollapsingHeader("Presets")) {
			// Save as preset
			ImGui::InputText("##Preset name", presetNameBuffer, 64);
			ImGui::SameLine();
			if (ImGui::Button("Save as preset")) {
				const std::string presetName = presetNameBuffer;
				// Can't save preset without a name
				if (!presetName.empty()) {
					presetManager.addNewPreset(presetName, universe.getCelestialBodies());
				}
			}

			const std::string previewText = selectedPresetName.value_or("");
			// Select preset
			if (ImGui::BeginCombo("Select preset", previewText.data())) {
				for (const auto& [presetName, preset] : presetManager.getPresets()) {
					bool isSelected = preset.getName() == presetName;
					if (ImGui::Selectable(presetName.data(), isSelected)) {
						selectedPresetName = presetName;
					}
				}

				ImGui::EndCombo();
			}

			// Load from preset
			if (ImGui::Button("Load from preset")) {
				if (selectedPresetName.has_value()) {
					const auto preset = presetManager.getPresets().find(selectedPresetName.value())->second;
					universe.loadFromPreset(preset);
				}
			}

			// Delete the currently selected preset
			ImGui::SameLine();
			if (ImGui::Button("Delete preset")) {
				if (selectedPresetName.has_value()) {
					presetManager.removePreset(selectedPresetName.value());
					selectedPresetName = std::nullopt;
				}
			}
		}	

		ImGui::Separator();
	}
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

		if (ImGui::TreeNode("Properties")) {
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

			ImGui::TreePop();
		}

		ImGui::Separator();

		ImGui::PopID();
	}
}
