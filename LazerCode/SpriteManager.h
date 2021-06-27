#pragma once
#include "pch.h"

// A class that simplifies the creation of sprites and stores a group of sprites that can then be manipulated together

class SpriteManager
{
private:
	std::unordered_map<std::string, sf::Sprite> sprites;
	std::unordered_map<std::string, sf::Texture> textures;

public:
	sf::Sprite& CreateSprite(std::string _name, std::string _filepath);
	sf::Sprite& GetSprite(std::string _name);
	std::unordered_map<std::string, sf::Sprite>* GetSprites();
	void RemoveSprite(std::string _name);

	void TranslateSprites(sf::Vector2<float> _translationAmount);
	void TranslateSprites(float _translationX, float _translationY);
	void DrawSprites(sf::RenderWindow& _window);
};

