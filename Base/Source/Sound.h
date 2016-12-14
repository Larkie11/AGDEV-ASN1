/******************************************************************************/
/*!
\file	Sound.h
\author Mok Wei Min
\par	email: 155208U\@mymail.nyp.edu.sg
\brief
Handles music and sound effect for the game using irrklang
*/
/******************************************************************************/
#ifndef SOUND_H
#define SOUND_H

#include "irrKlang.h"
#include <string>
using std::string;

#pragma comment(lib, "irrKlang.lib")
/******************************************************************************/
/*!
Class Sound:
\brief	Creates sound engine and allows music to be played
*/
/******************************************************************************/
class Sound
{
public:
	
	Sound();
	~Sound();
	void playMusic(string Music);
	void stopMusic(string Music);
	void playSE(string Music);
	
private:
	irrklang::ISoundEngine* musicEngine;
	irrklang::ISound* Song;
	irrklang::ISound* SoundEffect;
};
#endif