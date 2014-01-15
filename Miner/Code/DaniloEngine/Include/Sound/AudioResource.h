#pragma once

#include <System\Resource.h>
#include <SDL_mixer.h>

// Typedef declarations and forward declarations.
enum SoundType
{
	SOUND_TYPE_FIRST,
	SOUND_TYPE_MP3 = SOUND_TYPE_FIRST,
	SOUND_TYPE_WAVE,
	SOUND_TYPE_MIDI,
	SOUND_TYPE_OGG,

	// This needs to be the last sound type
	SOUND_TYPE_COUNT,
	SOUND_TYPE_UNKNOWN,
};
extern char *gSoundExtentions[];
using namespace GameSystem;

namespace Sound
{


/// <summary>
/// SoundFxResource that contains the name of the resource and has the ability of creating a handle to the resource
/// </summary>
class SoundFxResource : public Resource
{
public:
	/// <summary>
	/// Initializes a new instance of the <see cref="SoundFxResource"/> class.
	/// </summary>
	/// <param name="filename">The filename.</param>
	SoundFxResource(const std::string &filename) : Resource(filename) { }
	/// <summary>
	/// Creates the handle to a SoundFxResource. Handles are the one that really consume memory
	/// </summary>
	/// <param name="filename">The filename.</param>
	/// <param name="pResCache">The p resource cache.</param>
	/// <returns></returns>
	virtual ResHandle *CreateHandle(const std::string &filename, ResCache *pResCache) override;
};
/// <summary>
/// Class that contains the memory for a given sound FX. 
/// </summary>
class SoundFxResHandle: public ResHandle
{
public:
	/// <summary>
	/// Initializes a new instance of the <see cref="SoundFxResHandle"/> class.
	/// </summary>
	/// <param name="r">The r.</param>
	/// <param name="filename">The filename.</param>
	/// <param name="pResCache">The p resource cache.</param>
	SoundFxResHandle(Resource &r, const std::string &filename, ResCache *pResCache);
	/// <summary>
	/// Finalizes an instance of the <see cref="SoundFxResHandle"/> class.
	/// </summary>
	virtual ~SoundFxResHandle();

	/// <summary>
	/// Gets the Mix chunk. This is specific to SDL (sic)
	/// </summary>
	/// <returns></returns>
	Mix_Chunk *GetChunk(){ return m_Phaser; }


	/// <summary>
	/// Gets the type of the sound.
	/// </summary>
	/// <returns></returns>
	SoundType GetSoundType() { return m_SoundType; }

	/// <summary>
	/// Loads the data for the soundfxresourcehandle
	/// </summary>
	/// <returns></returns>
	virtual void Load() override;
private:
	enum SoundType m_SoundType;			// is this an Ogg, WAV, etc.?
	Mix_Chunk *m_Phaser = nullptr;
};


/// <summary>
/// Class that contains the filename of the music file and is able to create a Handle that contains the actual memory
/// </summary>
class MusicResource : public Resource
{
public:
	/// <summary>
	/// Initializes a new instance of the <see cref="MusicResource"/> class.
	/// </summary>
	/// <param name="name">The name.</param>
	MusicResource(const std::string &filename) : Resource(filename) { }
	/// <summary>
	/// Creates the handle. that is the one that actually contains the music memory
	/// </summary>
	/// <param name="filename">The filename.</param>
	/// <param name="pResCache">The p resource cache.</param>
	/// <returns></returns>
	virtual ResHandle *CreateHandle(const std::string &filename, ResCache *pResCache) override;
};
/// <summary>
/// Class that contains the memory chunk where the music is stored. It is specific to SDL.
/// </summary>
class MusicResourceResHandle : public ResHandle
{
public:
	/// <summary>
	/// Initializes a new instance of the <see cref="MusicResourceResHandle"/> class.
	/// </summary>
	/// <param name="r">The r.</param>
	/// <param name="filename">The filename.</param>
	/// <param name="pResCache">The p resource cache.</param>
	MusicResourceResHandle(Resource &r, const std::string &filename, ResCache *pResCache);
	/// <summary>
	/// Finalizes an instance of the <see cref="MusicResourceResHandle"/> class.
	/// </summary>
	virtual ~MusicResourceResHandle();

	/// <summary>
	/// Gets the type of the sound.
	/// </summary>
	/// <returns></returns>
	SoundType GetSoundType() { return m_SoundType; }
	/// <summary>
	/// Gets the music.
	/// </summary>
	/// <returns></returns>
	const Mix_Music *GetMusic(){ return m_Music; }

	/// <summary>
	/// Loads the data for the music
	/// </summary>
	/// <returns></returns>
	virtual void Load();


private:
	enum SoundType m_SoundType;			// is this an Ogg, WAV, etc.?
	Mix_Music *m_Music = nullptr;

};


}
