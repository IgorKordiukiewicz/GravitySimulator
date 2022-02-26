#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <vector>
#include "Universe.hpp"
#include "PresetManager.hpp"
#include <optional>
#include <imgui.h>
#include <map>

class Editor
{
public:
	Editor(sf::RenderWindow& window, Universe& universe, PresetManager& presetManager);

	void update();

	void setFont(ImFont* newFont);

private:
	void grabBody();
	void updateSimulationState();
	void updatePresets();
	void updateCentralBody();
	void updateDrawTrailsOption();
	void updateCelestialBodiesProperties();

private:
	sf::RenderWindow& window;
	Universe& universe;
	PresetManager& presetManager;

	// Variables for changing the bodies positions or velocities by grabbing their shape or
	// their velocity arrow shape and moving it using mouse
	CelestialBody* grabbedBody{ nullptr };
	// Whether the grabbed shape was the body's shape or its velocity arrow
	bool grabbedArrowHead{ false };
	sf::Vector2f mousePosOnSelect;

	std::optional<int> centralBodyId;

	ImFont* font{ nullptr };

	int screenshotId{ 0 };

	char presetNameBuffer[64]{ "" };
	std::optional<std::string> selectedPresetName;
};
