#pragma once
#include <bass.h>
#include <map>

#include <iostream> //to output

class Audio
{
public:

	static std::map<std::string, HSAMPLE> sLoadedAudios; //para nuestro manager
	//HSAMPLE sample; //aqui guardamos el handler del sample que retorna BASS_SampleLoad

	Audio(); //importante poner sample a cero aqui
	//~Audio(); //aqui deberiamos liberar el sample con BASS_SampleFree

	//HCHANNEL play(float volume); //lanza el audio y retorna el channel donde suena

	//static void Stop(HCHANNEL channel); //para parar un audio necesitamos su channel
	//static Audio* Get(const char* filename); //manager de audios 

	HSAMPLE LoadSample(const char* fileName, bool loop);
	HSAMPLE Find(const char* filename);
	void setVolume(const char* filename, float vol);
	void PlaySample(const char* fileName, bool loop);

	void Stop(const char* fileName);

	
};