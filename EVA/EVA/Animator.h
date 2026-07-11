#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <map>
#include <optional>

struct Animation {
    std::vector<sf::IntRect> frames;
    float frameDuration;
    bool loop;
};

class Animator {
    sf::Texture texture;
    std::map<std::string, Animation> animations;
    std::string currentAnim;
    int currentFrame;
    float timer;
    bool finished;

public:
    std::optional<sf::Sprite> sprite;

    Animator();
    bool LoadTexture(const std::string& path);
    void AddAnimation(const std::string& name, std::vector<sf::IntRect> frames, float frameDuration, bool loop = true);
    void Play(const std::string& name, bool force = false);
    void Update(float dt);
    bool IsFinished() const { return finished; }
    std::string GetCurrent() const { return currentAnim; }
};