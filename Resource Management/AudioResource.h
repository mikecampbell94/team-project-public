#pragma once
#include "ResourceClass.h"

class AudioResource : public Resourceclass
{
public:
	AudioResource() {}
	~AudioResource() {}
	GLuint GetVolume() { return volume; }
	string GetSoundName() { return soundName; }
	inline void SetVolume(GLuint Volume){ volume = Volume;}
	inline void SetMusicName(string name) { soundName = name; }
protected:
	GLuint volume;
	string soundName;

};
