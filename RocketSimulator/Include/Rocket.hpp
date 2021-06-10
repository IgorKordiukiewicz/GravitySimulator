#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "Body.hpp"

class CelestialBody;

class Rocket : public Body
{
public:
	Rocket(const float gravitationalForce);

	void update(const std::vector<CelestialBody>& celestialBodies, float deltaTime);

	void draw(sf::RenderWindow& window, bool drawArrowShape);

	void orbitCelestialBody(const CelestialBody& celestialBody);

	const sf::Sprite& getSprite() const { return sprite; }

private:
	virtual void updateDrawablesPosition() override;

private:
	sf::Sprite sprite;
	sf::Texture texture;

	const float gravitationalForce;
};
