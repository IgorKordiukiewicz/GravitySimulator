#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Texture.hpp>

class CelestialBody;

class Rocket
{
public:
	Rocket();
	~Rocket() = default;

	void update(const std::vector<CelestialBody>& celestialBodies, const float gravitationalForce, float deltaTime);

	void draw(sf::RenderWindow& window);

	const sf::Vector2f& getInitialPosition() const { return initialPosition; }
	const sf::Vector2f& getInitialVelocity() const { return initialVelocity; }
	const sf::Vector2f& getCurrentPosition() const { return currentPosition; }
	const sf::Vector2f& getCurrentVelocity() const { return currentVelocity; }

private:
	sf::Sprite sprite;
	sf::Texture texture;

	sf::Vector2f initialPosition;
	sf::Vector2f initialVelocity;
	sf::Vector2f currentPosition;
	sf::Vector2f currentVelocity;
};
