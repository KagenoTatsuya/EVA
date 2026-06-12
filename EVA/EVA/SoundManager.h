// SoundManager.h
#pragma once
#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include <memory>

class SoundManager {
    std::map<std::string, sf::SoundBuffer> buffers;
    std::map<std::string, std::unique_ptr<sf::Sound>> sounds;  //    unique_ptr
    sf::Music music;

public:
    void Load(const std::string& name, const std::string& filepath);
    void Play(const std::string& name);
    void PlayMusic(const std::string& filepath, bool loop = true);
    void StopMusic();
    void SetVolume(const std::string& name, float volume);
    void SetMusicVolume(float volume);
    bool IsStopped(const std::string& name);
    bool IsMusicPlaying() const;
};