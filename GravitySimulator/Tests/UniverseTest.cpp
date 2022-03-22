#include "../3rdParty/doctest.h"
#include "../Include/Universe.hpp"

TEST_CASE("Universe") {
	Universe universe;

	SUBCASE("Adding new bodies") {
		const int bodiesCount{ static_cast<int>(universe.getCelestialBodies().size()) };
		universe.createNewBody();
		CHECK(universe.getCelestialBodies().size() == bodiesCount + 1);
	}

	SUBCASE("Simulation state") {
		// Simulation state has to be initially set to reset
		CHECK(universe.getSimulationState() == SimulationState::Reset);
		universe.runSimulation();
		CHECK(universe.getSimulationState() == SimulationState::Running);
		universe.pauseSimulation();
		CHECK(universe.getSimulationState() == SimulationState::Paused);
		universe.resetSimulation();
		CHECK(universe.getSimulationState() == SimulationState::Reset);
	}

	SUBCASE("Load from preset") {
		CelestialBody celestialBody{ 0.f, 0.f, {0.f, 0.f}, {0.f, 0.f} };
		std::vector<CelestialBody> bodies{ { std::move(celestialBody) } };
		Preset preset{ "preset", bodies };
		universe.loadFromPreset(preset);
		CHECK(universe.getCelestialBodies().size() == 1);
	}
}