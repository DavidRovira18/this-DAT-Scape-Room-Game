#include "sonido.h"

HSAMPLE Sonidos::LoadSample(const char* fileName)
{
	//El handler para un sample
	HSAMPLE hSample;

	//Cargamos un sample del disco duro (memoria, filename, offset, length, max, flags)
	//use BASS_SAMPLE_LOOP in the last param to have a looped sound
	hSample = BASS_SampleLoad(false, fileName, 0, 0, 3, BASS_SAMPLE_LOOP);
	if (hSample == 0)
	{
		//file not found
	}
	std::cout << "+ AUDIO load " << fileName << std::endl;
	return hSample;
}

void Sonidos::PlaySample(const char* fileName)
{
	//El handler para un sample
	HSAMPLE hSample;

	//El handler para un canal
	HCHANNEL hSampleChannel;

	//Creamos un canal para el sample
	hSampleChannel = BASS_SampleGetChannel(hSample, false);

	//Lanzamos un sample
	BASS_ChannelPlay(hSampleChannel, true);
}
