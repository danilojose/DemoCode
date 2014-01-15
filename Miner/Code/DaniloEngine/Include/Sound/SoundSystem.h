#pragma once
#include <System\EventManager.h>
#include <Sound\AudioResource.h>

///namespaces
using namespace GameSystem;


namespace Sound
{
/// <summary>
/// The sound system is the one that knows how to handle Sound Resources and is able to actually play them and initialize the Sound System
/// </summary>
class SoundSystem
{

protected:

	bool m_RenderDiagnostics;						// Are we rendering diagnostics?
	EventListenerPtr m_pSoundEventListener;			// Actors event listener to create additional Actors to be added to the system


public:
	/// <summary>
	/// Initializes a new instance of the <see cref="SoundSystem"/> class.
	/// </summary>
	SoundSystem();
	/// <summary>
	/// Finalizes an instance of the <see cref="SoundSystem"/> class.
	/// </summary>
	virtual ~SoundSystem();
	/// <summary>
	/// Initializes the instance
	/// </summary>
	void Init();

	/// <summary>
	/// Plays the sound.
	/// </summary>
	/// <param name="filepath">The filepath.</param>
	void PlaySound(std::string filepath);
	/// <summary>
	/// Finds the sound type from file.
	/// </summary>
	/// <param name="fileName">Name of the file.</param>
	/// <returns></returns>
	static SoundType FindSoundTypeFromFile(const std::string &fileName);
};


/// <summary>
/// Listener class that implements the events that affect the sound system. (mainly play sound, play music)
/// </summary>
class SoundListener : public IEventListener
{
	SoundSystem *m_SoundSystem;
public:
	/// <summary>
	/// Initializes a new instance of the <see cref="SoundListener"/> class.
	/// </summary>
	/// <param name="soundSystem">The sound system.</param>
	explicit SoundListener(SoundSystem * soundSystem) : IEventListener() { m_SoundSystem = soundSystem; }
	/// <summary>
	/// Gets the name.
	/// </summary>
	/// <returns></returns>
	virtual char const* GetName(void) const override { return "SoundListener"; }
	/// <summary>
	/// Handles the event. Mainly play a sound right now
	/// </summary>
	/// <param name="event">The event.</param>
	/// <returns></returns>
	virtual bool HandleEvent(IEventData const & event) const override;
};

}