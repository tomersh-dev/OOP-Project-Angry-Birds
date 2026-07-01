#pragma once

// Audio stub — full implementation deferred to AudioManager roadmap phase.
// Members and SFML/Audio includes will be added when the audio feature is built.
class AudioManager {
public:
    static AudioManager& GetInstance();

    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

private:
    AudioManager() = default;
    ~AudioManager() = default;
};
