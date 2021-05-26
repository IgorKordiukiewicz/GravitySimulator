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
	Rocket();
	~Rocket() = default;

	void update(const std::vector<CelestialBody>& celestialBodies, const float gravitationalForce, float deltaTime);

	void draw(sf::RenderWindow& window, bool drawArrowShape);

	const sf::Sprite& getSprite() const { return sprite; }

private:
	virtual void onInitialPositionUpdated() override;

private:
	sf::Sprite sprite;
	sf::Texture texture;
};
