#pragma once

#include "Preset.hpp"
#include <map>
#include <string>

class PresetManager
{
public:
	void loadPresets();
	void savePresets();

	void addNewPreset(const std::string& name, const std::vector<CelestialBody>& celestialBodies);
	void removePreset(const std::string& name);

	const std::map<std::string, Preset>& getPresets() const { return presets; }

private:
	std::map<std::string, Preset> presets;
};