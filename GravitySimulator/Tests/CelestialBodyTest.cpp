#include "../3rdParty/doctest.h"
#include "../Include/CelestialBody.hpp"

TEST_CASE("Celestial Body") {
	CelestialBody celestialBody{ 0.f, 0.f, {0.f, 0.f}, {0.f, 0.f} };

	SUBCASE("Properties affect body shape") {
		const sf::CircleShape& bodyShape{ celestialBody.getBodyShape() };

		CHECK(bodyShape.getRadius() == 0.f);
		const float radius{ 10.f };
		celestialBody.setRadius(radius);
		CHECK(bodyShape.getRadius() == radius);

		CHECK(bodyShape.getPosition() == sf::Vector2f{0.f, 0.f});
		const sf::Vector2f initialPosition{ 10.f, 10.f };
		celestialBody.setInitialPosition(initialPosition);
		CHECK(bodyShape.getPosition() == initialPosition);

		const Color color{ 0.5f, 0.5f, 0.5f };
		const sf::Color sfmlColor{ color.toSFMLColor() };
		celestialBody.setColor(color);
		CHECK(bodyShape.getFillColor() == sfmlColor);
	}

	SUBCASE("Properties affect velocity arrow shape") {
		const ArrowShape& arrowShape{ celestialBody.getVelocityArrowShape() };
		CHECK(arrowShape.getStartPos() == sf::Vector2f{ 0.f, 0.f });
		CHECK(arrowShape.getDirection() == sf::Vector2f{ 0.f, 0.f });

		const sf::Vector2f initialPosition{ 10.f, 10.f };
		celestialBody.setInitialPosition(initialPosition);
		CHECK(arrowShape.getStartPos() == initialPosition);

		const sf::Vector2f initialVelocity{ 10.f, 10.f };
		celestialBody.setInitialVelocity(initialVelocity);
		CHECK(arrowShape.getDirection() == initialVelocity);
	}

	SUBCASE("Mark to delete") {
		celestialBody.markToDelete();
		CHECK(celestialBody.getShouldBeDeleted());
	}

	SUBCASE("Destroy") {
		celestialBody.destroy();
		CHECK(celestialBody.isDestroyed());
	}

	SUBCASE("Different id's") {
		CelestialBody celestialBody2{ 0.f, 0.f, {0.f, 0.f}, {0.f, 0.f} };
		CHECK_FALSE(celestialBody.getId() == celestialBody2.getId());

		// Id shouldn't be copied when copying object using copy ctor/assignment
		CelestialBody celestialBody3{ celestialBody2 };
		CHECK_FALSE(celestialBody2.getId() == celestialBody3.getId());
	}

	SUBCASE("Simulation") {
		// Run one frame of simple simulation
		celestialBody.setMass(10.f);
		celestialBody.setInitialVelocity({ 10.f, 10.f });
		CelestialBody celestialBody2{ celestialBody };
		celestialBody2.setInitialPosition({ 100.f, 100.f });
		std::vector<CelestialBody> bodies{ {celestialBody, celestialBody2} };
		CelestialBody& body{ bodies.front() };
		body.updateVelocity(bodies, 100.f, 1.f);
		body.updatePosition(1.f);

		// Body's position and velocity should change after the simulation
		CHECK_FALSE(body.getCurrentPosition() == body.getInitialPosition());
		CHECK_FALSE(body.getCurrentVelocity() == body.getInitialVelocity());

		// Body's bodyShape's position should also be updated
		CHECK(celestialBody.getBodyShape().getPosition() == celestialBody.getCurrentPosition());

		// Ensure reset reverts 'current' properties to 'initial' properties
		body.reset();
		CHECK(body.getCurrentPosition() == body.getInitialPosition());
		CHECK(body.getCurrentVelocity() == body.getInitialVelocity());
		CHECK_FALSE(body.isDestroyed());
	}
}