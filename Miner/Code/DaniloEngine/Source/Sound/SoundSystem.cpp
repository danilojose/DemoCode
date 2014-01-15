#include <Sound\SoundSystem.h>
#include <SDL_mixer.h>
#include <System\Assert.h>
#include <System\Application.h>
#include <System\Resource.h>

extern GameCodeApp * g_pApp;
using namespace Sound;
/// <summary>
/// Initializes a new instance of the <see cref="SoundSystem"/> class.
/// </summary>
SoundSystem::SoundSystem()
{

	m_pSoundEventListener = EventListenerPtr(GCC_NEW SoundListener(this));
	/* Same setup as before */
	int audio_rate = 44100;
	Uint16 audio_format = AUDIO_S16;
	int audio_channels = 16;
	int audio_buffers = 4096;

	const int mixerInitResult = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	ASSERT_DESCRIPTION(mixerInitResult == 0, "Mix_OpenAudio failed: " << Mix_GetError());

}

/// <summary>
/// Finalizes an instance of the <see cref="SoundSystem"/> class.
/// </summary>
SoundSystem::~SoundSystem()
{
	Mix_HaltMusic();
	Mix_HaltChannel(-1);

	Mix_CloseAudio();

}

/// <summary>
/// Initializes the instance
/// </summary>
void SoundSystem::Init()
{
	IEventManager::Get()->VAddListener(m_pSoundEventListener, EvtData_PlaySound::sk_EventType);
}

/// <summary>
/// Plays the sound.
/// </summary>
/// <param name="filepath">The filepath.</param>
void SoundSystem::PlaySound(std::string filepath)
{

	SoundFxResource r(filepath);
	std::shared_ptr<SoundFxResHandle> sfxResHandle = std::static_pointer_cast<SoundFxResHandle>(g_pApp->GetCache()->GetHandle(&r));
	int result=Mix_PlayChannel(-1, sfxResHandle->GetChunk(), 0);
	ASSERT_DESCRIPTION(result != -1, "Mix_OpenAudio failed: " << Mix_GetError());
}


/// <summary>
/// Finds the sound type from file.
/// </summary>
/// <param name="fileName">Name of the file.</param>
/// <returns></returns>
SoundType SoundSystem::FindSoundTypeFromFile(const std::string &filename)
{
	int type = SOUND_TYPE_FIRST;

	while (type != SOUND_TYPE_COUNT)
	{
		if (strstr(filename.c_str(), gSoundExtentions[type]))
			return SoundType(type);
		type++;
	}
	return SOUND_TYPE_UNKNOWN;
}

/// <summary>
/// Handles the event. Mainly play a sound right now
/// </summary>
/// <param name="event">The event.</param>
/// <returns></returns>
bool SoundListener::HandleEvent(IEventData const & event) const
{
	EventType eventType = event.GetEventType();

	if (eventType == EvtData_PlaySound::sk_EventType)
	{
		EvtData_PlaySound const & ed = static_cast< const EvtData_PlaySound & >(event);

		m_SoundSystem->PlaySound(ed.m_Filename);
	}
	return true;
}


