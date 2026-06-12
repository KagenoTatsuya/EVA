#include "Animator.h"
#include <iostream>

Animator::Animator() : currentFrame(0), timer(0.f), finished(false) {}

void Animator::AddAnimation(const std::string& name, std::vector<sf::IntRect> frames, float frameDuration, bool loop) {
    animations[name] = { frames, frameDuration, loop };
}
bool Animator::LoadTexture(const std::string& path) {
    texture = sf::Texture();
    if (!texture.loadFromFile(path)) {
        std::cerr << "Erreur chargement spritesheet: " << path << std::endl;
        return false;
    }
    sprite.emplace(texture);
    return true;
}

void Animator::Play(const std::string& name) {
    if (currentAnim == name) {
        // Si l'animation est terminee, la relancer
        if (finished) {
            currentFrame = 0;
            timer = 0.f;
            finished = false;
            if (sprite && animations.count(name))
                sprite->setTextureRect(animations[name].frames[0]);
        }
        return;
    }
    currentAnim = name;
    currentFrame = 0;
    timer = 0.f;
    finished = false;
    if (sprite && animations.count(name)) {
        sprite->setTextureRect(animations[name].frames[0]);
    }
}

void Animator::Update(float dt) {
    if (currentAnim.empty() || !animations.count(currentAnim)) return;
    Animation& anim = animations[currentAnim];
    if (finished && !anim.loop) return;

    timer += dt;
    if (timer >= anim.frameDuration) {
        timer = 0.f;
        currentFrame++;
        if (currentFrame >= (int)anim.frames.size()) {
            if (anim.loop) {
                currentFrame = 0;
            }
            else {
                currentFrame = (int)anim.frames.size() - 1;
                finished = true;
                return;
            }
        }
        if (sprite) sprite->setTextureRect(anim.frames[currentFrame]);
    }
}