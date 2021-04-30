#include "CppUnitTest.h"
#include "../RocketSimulator/Include/CelestialBody.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(CelestialBodyTest)
	{
	public:
		TEST_METHOD(BodyOriginTest)
		{
			CelestialBody body{ 1.f, 20.f, sf::Vector2f(), sf::Vector2f() };

			Assert::IsTrue(body.getBodyShape().getOrigin() == sf::Vector2f{ 20.f, 20.f });
			body.setRadius(12.f);
			Assert::IsTrue(body.getBodyShape().getOrigin() == sf::Vector2f{ 12.f, 12.f });
		}

		TEST_METHOD(SetPositionShapesTest)
		{
			CelestialBody body{ 1.f, 1.f, sf::Vector2f(10.f, 15.f), sf::Vector2f() };

			Assert::IsTrue(body.getBodyShape().getPosition() == sf::Vector2f{ 10.f, 15.f });
			Assert::IsTrue(body.getVelocityArrowShape().getStartPos() == sf::Vector2f{ 10.f, 15.f });

			body.setInitialPosition({ 25.f, 30.f });
			Assert::IsTrue(body.getBodyShape().getPosition() == sf::Vector2f{ 25.f, 30.f });
			Assert::IsTrue(body.getVelocityArrowShape().getStartPos() == sf::Vector2f{ 25.f, 30.f });
		}

		TEST_METHOD(SetVelocityArrowShapeTest)
		{
			CelestialBody body{ 1.f, 1.f, sf::Vector2f(), sf::Vector2f(20.f, 120.f) };

			Assert::IsTrue(body.getVelocityArrowShape().getDirection() == sf::Vector2f{ 20.f, 120.f });

			body.setInitialVelocity({ -30.f, -80.f });
			Assert::IsTrue(body.getVelocityArrowShape().getDirection() == sf::Vector2f{ -30.f, -80.f });
		}
	};
}