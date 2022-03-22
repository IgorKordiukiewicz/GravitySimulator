#include "../3rdParty/doctest.h"
#include "../Include/Preset.hpp"

TEST_CASE("Preset") {
	CelestialBody celestialBody{ 10.f, 30.f, {50.f, 50.f}, {20.f, 20.f} };
	std::vector<CelestialBody> bodies{ { std::move(celestialBody) } };
	Preset preset{ "preset", bodies };
	const CelestialBodyProperties bodyProperties{ preset.getCelestialBodiesProperties().front() };

	CHECK(preset.getCelestialBodiesProperties().size() == 1);
	preset.saveToFile();
	preset.loadFromFile();
	CHECK(preset.getCelestialBodiesProperties().size() == 1);
	const CelestialBodyProperties bodyPropertiesAfterSaveAndLoad{ preset.getCelestialBodiesProperties().front() };
	CHECK(bodyProperties.mass == bodyPropertiesAfterSaveAndLoad.mass);
	CHECK(bodyProperties.radius == bodyPropertiesAfterSaveAndLoad.radius);
	CHECK(bodyProperties.initialPosition == bodyPropertiesAfterSaveAndLoad.initialPosition);
	CHECK(bodyProperties.initialVelocity == bodyPropertiesAfterSaveAndLoad.initialVelocity);
}