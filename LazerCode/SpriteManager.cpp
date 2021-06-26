#include "pch.h"
#include "SpriteManager.h"

sf::Sprite& SpriteManager::CreateSprite(std::string _name, std::string _filepath) {
	// Create a texturefrom the given filepath
	if (textures.find(_filepath) == textures.end()) {
		sf::Texture texture;
		if (!texture.loadFromFile(_filepath)) {
			MessageBox(NULL, L"Texture Load Failure", L"ERROR", MB_SETFOREGROUND);
			exit(EXIT_FAILURE);
		}

		// Adds the texture to the hash table if it does not already exist
		textures[_filepath] = texture;
	}

	// create a sprite and give it the texture
	sf::Sprite sprite;
	sprite.setTexture(textures[_filepath]);

	// Add sprite to the hash table
	int sA = sprites.size();
	sprites[_name] = sprite;

	// return the sprite object
	return sprites[_name];
}
sf::Sprite& SpriteManager::GetSprite(std::string _name) {
	// Return the given sprite
	return sprites[_name];
}
std::unordered_map<std::string, sf::Sprite>* SpriteManager::GetSprites() {
	// return all of the sprites
	return &sprites;
}
void SpriteManager::RemoveSprite(std::string _name) {
	// remove the given sprite
	sprites.erase(_name);
}
void SpriteManager::TranslateSprites(sf::Vector2<float> _translationAmount) {
	// translate all the sprites
	for (std::pair<std::string, sf::Sprite> pair : sprites) {
		sf::Sprite& sprite = pair.second;
		sprite.setPosition(sf::Vector2<float>(sprite.getPosition() + _translationAmount));
	}
}
void SpriteManager::TranslateSprites(float _translationX, float _translationY) {
	// translate all the sprites
	for (std::pair<std::string, sf::Sprite> pair : sprites) {
		sf::Sprite& sprite = pair.second;
		sprite.setPosition(sf::Vector2<float>(sprite.getPosition().x + _translationX, sprite.getPosition().y + _translationY));
	}
}
void SpriteManager::DrawSprites(sf::RenderWindow& _window) {
	// Draws all the sprites
	for (std::pair<std::string, sf::Sprite> pair : sprites) {
		_window.draw(pair.second);
	}
}