#include "../3rdParty/doctest.h"
#include "../Include/PresetManager.hpp"

TEST_CASE("Preset Manager") {
	PresetManager presetManager;
	CelestialBody celestialBody{ 0.f, 0.f, {0.f, 0.f}, {0.f, 0.f} };
	std::vector<CelestialBody> bodies{ { std::move(celestialBody) } };

	CHECK(presetManager.getPresets().size() == 0);
	presetManager.addNewPreset("Preset1", bodies);
	CHECK(presetManager.getPresets().size() == 1);
	presetManager.addNewPreset("Preset1", bodies);
	CHECK(presetManager.getPresets().size() == 1);
	presetManager.addNewPreset("Preset2", bodies);
	CHECK(presetManager.getPresets().size() == 2);
	presetManager.removePreset("Preset3");
	CHECK(presetManager.getPresets().size() == 2);
	presetManager.removePreset("Preset1");
	CHECK(presetManager.getPresets().size() == 1);

	presetManager.savePresets();
	presetManager.loadPresets();
	CHECK(presetManager.getPresets().size() == 1);
}