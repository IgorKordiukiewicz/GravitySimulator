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

	void setInitialBodyToOrbit(CelestialBody* newInitialBodyToOrbit);
	void updateInitialOrbit();

	void setInitialOrbitHeight(const float newInitialOrbitHeight);

	const sf::Sprite& getSprite() const { return sprite; }
	float getInitialOrbitHeight() const { return initialOrbitHeight; }

private:
	virtual void updateDrawablesPosition() override;

private:
	sf::Sprite sprite;
	sf::Texture texture;

	const float gravitationalForce;

	CelestialBody* initialBodyToOrbit{ nullptr };
	float initialOrbitHeight{ 30.f };
};
