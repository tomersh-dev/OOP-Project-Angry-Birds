#include "AudioManager.h"

AudioManager& AudioManager::GetInstance() {
    static AudioManager instance;
    return instance;
}
