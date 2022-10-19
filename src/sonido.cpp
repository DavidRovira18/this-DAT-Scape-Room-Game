#include "sonido.h"
#include "includes.h"
#include "framework.h"

#include <cassert>

static std::map<std::string, HSAMPLE> sLoadedAudios; //para nuestro manager
std::map<std::string, HSAMPLE> Audio::sLoadedAudios;

Audio::Audio()
{
	//sample = NULL;
}

HSAMPLE Audio::LoadSample(const char* fileName, bool loop)
{
	//El handler para un sample
	//Audio* hSample = new Audio();
	HSAMPLE hSample;

	//Cargamos un sample del disco duro (memoria, filename, offset, length, max, flags)
	//use BASS_SAMPLE_LOOP in the last param to have a looped sound

	std::map<std::string, HSAMPLE>::iterator it = sLoadedAudios.find(fileName);
	if (it != sLoadedAudios.end())
	{
		std::cout << " + AUDIO found " << fileName << std::endl;
		return it->second;
	}
	else
	{
		std::cout << " + AUDIO load " << fileName << std::endl;
		if (loop)
			hSample = BASS_SampleLoad(false, fileName, 0, 0, 3, BASS_SAMPLE_LOOP);
		else
			hSample = BASS_SampleLoad(false, fileName, 0, 0, 3, 0);
		sLoadedAudios[fileName] = hSample;
		return hSample;
	}

	if (hSample == 0)
	{
		std::cout << " ERROR load " << fileName << std::endl;
	}
	return hSample;
}

HSAMPLE Audio::Find(const char* filename)
{
	assert(filename);
	auto it = sLoadedAudios.find(filename);
	if (it != sLoadedAudios.end())
		return it->second;
	return NULL;
}


void Audio::setVolume(const char* fileName, float vol)
{

	HSAMPLE hSample = Find(fileName);
	HCHANNEL hSampleChannel;

	//Creamos un canal para el sample
	hSampleChannel = BASS_SampleGetChannel(hSample, false);

	hSampleChannel = BASS_SetVolume(vol);
	BASS_ChannelPlay(hSampleChannel, true);
}

void Audio::PlaySample(const char* fileName, bool loop)
{
	//El handler para un canal
	HCHANNEL hSampleChannel;

	HSAMPLE hSample = LoadSample(fileName, loop);

	//Creamos un canal para el sample
	hSampleChannel = BASS_SampleGetChannel(hSample, false);

	// BASS_ChannelSetAttribute(hSampleChannel, BASS_ATTRIB_VOL, float [0,1]);

	//Lanzamos un sample
	BASS_ChannelPlay(hSampleChannel, true);
}

void Audio::Stop(const char* fileName)
{
	int j;
	if (!fileName)
	{
		for (int i = 0; i < sLoadedAudios.size(); ++i)
		{
			j = 0;
		}
	}


	HSAMPLE hSample = Find(fileName);
	bool lmao = false;
	int i = 0;
	if (hSample)
		lmao = BASS_SampleStop(hSample);
	if (!lmao)
		i = BASS_ErrorGetCode();
}
