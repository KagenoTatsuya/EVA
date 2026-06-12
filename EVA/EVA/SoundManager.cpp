// SoundManager.cpp
#include "SoundManager.h"
#include <iostream>

void SoundManager::Load(const std::string& name, const std::string& filepath) {
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filepath)) {
        std::cerr << "Erreur chargement son: " << filepath << std::endl;
        return;
    }
    buffers[name] = std::move(buffer);  // Convertit les Lvalue en Rvalue ( convertit un objet possedant un nom, en une reference), permettant d?appeler les constructeurs/operateurs de deplacement
    sounds[name] = std::make_unique<sf::Sound>(buffers[name]);
}

void SoundManager::Play(const std::string& name) {
    if (sounds.count(name)) {
        sounds[name]->play();
    }
}


void SoundManager::PlayMusic(const std::string& filepath, bool loop) {
    if (!music.openFromFile(filepath)) {
        std::cerr << "Erreur chargement musique: " << filepath << std::endl;
        return;
    }
    music.setLooping(loop);
    music.play();
}

void SoundManager::StopMusic() { music.stop(); }

bool SoundManager::IsMusicPlaying() const {
    return music.getStatus() == sf::Music::Status::Playing;
}

void SoundManager::SetVolume(const std::string& name, float volume) {
    if (sounds.count(name)) sounds[name]->setVolume(volume);
}

bool SoundManager::IsStopped(const std::string& name) {
    if (sounds.count(name))
        return sounds[name]->getStatus() == sf::Sound::Status::Stopped;
    return true;
}

void SoundManager::SetMusicVolume(float volume) {
    music.setVolume(volume);
}