#include <System\Assert.h>
#include <System\StdLibraries.h>
#include <System\Resource.h>
#include <Sound\AudioResource.h>
#include <Sound\SoundSystem.h>
#include <SDL_mixer.h>

char *gSoundExtentions[] = { ".mp3", ".wav", ".midi", ".ogg" };

using namespace GameSystem;
using namespace Sound;

/// <summary>
/// Creates the handle to a SoundFxResource. Handles are the one that really consume memory
/// </summary>
/// <param name="filename">The filename.</param>
/// <param name="pResCache">The p resource cache.</param>
/// <returns></returns>
ResHandle * SoundFxResource::CreateHandle(const std::string &filename, ResCache *pResCache)
{
	return GCC_NEW SoundFxResHandle(*this, filename, pResCache);

}
/// <summary>
/// Initializes a new instance of the <see cref="SoundFxResHandle"/> class.
/// </summary>
/// <param name="r">The r.</param>
/// <param name="filename">The filename.</param>
/// <param name="pResCache">The p resource cache.</param>
SoundFxResHandle::SoundFxResHandle(Resource &r, const std::string &filename, ResCache *pResCache) :ResHandle(r,filename,pResCache)
{
	m_SoundType = SoundSystem::FindSoundTypeFromFile(filename);
}
/// <summary>
/// Finalizes an instance of the <see cref="SoundFxResHandle"/> class.
/// </summary>
SoundFxResHandle::~SoundFxResHandle()
{
	Mix_FreeChunk(m_Phaser);
	m_Phaser = nullptr;

}

/// <summary>
/// Finalizes an instance of the <see cref="SoundFxResHandle"/> class.
/// </summary>
void SoundFxResHandle::Load()
{
	m_Phaser = Mix_LoadWAV(m_Filename.c_str());
	ASSERT_DESCRIPTION(m_Phaser != nullptr, "Mix_LoadWAV failed: " << Mix_GetError());

}
/// <summary>
/// Creates the handle. that is the one that actually contains the music memory
/// </summary>
/// <param name="filename">The filename.</param>
/// <param name="pResCache">The p resource cache.</param>
/// <returns></returns>
ResHandle *MusicResource::CreateHandle(const std::string &filename, ResCache *pResCache)
{
	return GCC_NEW MusicResourceResHandle(*this, filename, pResCache);
}
/// <summary>
/// Initializes a new instance of the <see cref="MusicResourceResHandle"/> class.
/// </summary>
/// <param name="r">The r.</param>
/// <param name="filename">The filename.</param>
/// <param name="pResCache">The p resource cache.</param>
MusicResourceResHandle::MusicResourceResHandle(Resource &r, const std::string &filename, ResCache *pResCache) :ResHandle(r,filename,pResCache)
{
	m_SoundType = SoundSystem::FindSoundTypeFromFile(filename);
}
/// <summary>
/// Finalizes an instance of the <see cref="MusicResourceResHandle"/> class.
/// </summary>
MusicResourceResHandle::~MusicResourceResHandle()
{
	Mix_FreeMusic(m_Music);
	m_Music = nullptr;
}

/// <summary>
/// Loads the data for a MusicResource
/// </summary>
void MusicResourceResHandle::Load()
{
	m_Music = Mix_LoadMUS(m_Filename.c_str());
	ASSERT_DESCRIPTION(m_Music != nullptr, "Mix_LoadMUS failed: " << Mix_GetError());

}


