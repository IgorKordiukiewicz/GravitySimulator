#include "..\Include\PresetManager.hpp"
#include <filesystem>
#include <fstream>

void PresetManager::loadPresets()
{
	presets.clear();
	std::ifstream file("presets.txt");
	std::string line;
	while (std::getline(file, line)) {
		Preset preset(line);
		preset.loadFromFile();
		presets.insert({ line, std::move(preset) });
	}
	file.close();
}

void PresetManager::savePresets()
{
	// Delete already existing presets files
	auto path = std::filesystem::current_path() / "Presets";
	for (const auto& it : std::filesystem::directory_iterator(path)) {
		if (it.path().extension() == ".preset") {
			std::filesystem::remove(it.path());
		}
	}

	// Save presets
	std::ofstream file("presets.txt");
	for (const auto& [presetName, preset] : presets) {
		preset.saveToFile();
		file << preset.getName() << '\n';
	}
	file.close();
}

void PresetManager::addNewPreset(const std::string& name, const std::vector<CelestialBody>& celestialBodies)
{
	Preset newPreset{ name, celestialBodies };
	presets.insert({ name, std::move(newPreset) });
}

void PresetManager::removePreset(const std::string& name)
{
	presets.erase(name);
}
