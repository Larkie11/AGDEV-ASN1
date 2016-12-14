/******************************************************************************/
/*!
\file	Sound.cpp
\author Mok Wei Min
\par	email: 155208U\@mymail.nyp.edu.sg
\brief
Handles music and sound effect for the game using irrklang
*/
/******************************************************************************/
#include "Sound.h"

Sound::Sound()
{
	musicEngine = irrklang::createIrrKlangDevice();
}
Sound::~Sound()
{
	musicEngine->drop();
}
/******************************************************************************/
/*!
\brief
Plays the background music looped

\param Music
Takes in path of background music to play

\exception None
\return None
*/
/******************************************************************************/
void Sound::playMusic(string Music)
{
	Song = musicEngine->play2D(Music.c_str(), true, false, true);
}
/******************************************************************************/
/*!
\brief
Plays the sound effect no loop

\param Music
Takes in path of sound effect to play

\exception None
\return None
*/
/******************************************************************************/
void Sound::playSE(string Music)
{
	SoundEffect = musicEngine->play2D(Music.c_str(), false, false, false);
}
/******************************************************************************/
/*!
\brief
Stop certain music when called

\param Music
Takes in path of background music/sound effect to stop

\exception None
\return None
*/
/******************************************************************************/
void Sound::stopMusic(string Music)
{
	Song->stop();
}