#pragma once
#include <bass.h>


class Sonidos
{
public:
	HSAMPLE LoadSample(const char* fileName);
	void PlaySample(const char* fileName);
};