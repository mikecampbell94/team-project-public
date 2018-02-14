#pragma once

//#pragma comment(lib, "Resource Management.lib")

#include "OpenAL 1.1 SDK\include\al.h"
#include "..\Resource Management\Resources\Resource.h"

///////////////////////////////////////
#include "OpenAL 1.1 SDK/include/alc.h"
////////////////////////////////////

#include <string>
#include <map>
#include <iostream>
#include <fstream>

using namespace std;

typedef unsigned long DWORD;

struct FMTCHUNK 
{
	short format;
	short channels;
	DWORD srate;
	DWORD bps;
	short balign;
	short samp;
};

class Sound : public Resource
{
public:

	Sound(std::string filePath);
	~Sound();

	char* getData() { return data; }
	int getBitRate() { return bitRate; }
	float getFrequency() { return freqRate; }
	int getChannels() { return channels; }
	int getSize() { return size; }
	ALuint getBuffer() { return buffer; }
	ALenum getOALFormat();
	double getLength();

	void loadFromWAV(string filename);
	void loadWAVChunkInfo(ifstream &file, string &name, unsigned int &size);

private:
	ALuint buffer;
	char *data;
	float freqRate;
	double length;
	unsigned int bitRate;
	unsigned int size;
	unsigned int channels;
};