#include <System\EventManager.h>
#include <System\Assert.h>
#include <SDL.h>

using namespace GameSystem;
static IEventManager * g_pEventMgr = NULL;

IEventManager * IEventManager::Get()
{
	return g_pEventMgr;
}

/// <summary>
/// Initializes a new instance of the <see cref="IEventManager"/> class.
/// </summary>
/// <param name="setAsGlobal">The set as global.</param>
IEventManager::IEventManager(
	bool setAsGlobal)
{
	if (setAsGlobal)
		g_pEventMgr = this;
}

IEventManager::~IEventManager()
{
	if (g_pEventMgr == this)
		g_pEventMgr = NULL;
}
// EventManager

//
// EventManager::EventManager		 - Chapter 10, page 290
//
EventManager::EventManager(
	bool setAsGlobal)
	: IEventManager(setAsGlobal),
	m_activeQueue(0)
{
}


//
// EventManager::~EventManager		 - Chapter 10, page 290
//
EventManager::~EventManager()
{
	m_activeQueue = 0;
	m_typeList.clear();
}

//
// EventManager::VAddListener		 - Chapter 10, page 290
//
// Register a listener for a specific event type, implicitly the
// event type will be added to the known event types if not
// already known.
//
// The function will return false on failure for any reason. The
// only really anticipated failure reason is if the input event
// type is bad ( e.g.: known-ident number with different
// signature text, or signature text is empty )

bool EventManager::VAddListener(
	EventListenerPtr const & inListener,
	EventType const & inType)
{
	if (!VValidateType(inType))
		return false;

	// check / update type list

	EventTypeSet::iterator evIt = m_typeList.find(inType);

	// find listener map entry, create one if no table already
	// exists for this entry ...

	EventListenerMap::iterator elmIt =
		m_registry.find(inType.getHashValue());

	if (elmIt == m_registry.end())
	{
		EventListenerMapIRes elmIRes = m_registry.insert(
			EventListenerMapEnt(inType.getHashValue(),
			EventListenerTable()));

		// whoops, could not insert into map!?!?
		if (elmIRes.second == false)
			return false;

		// should not be possible, how did we insert and create
		// an empty table!?!?!
		if (elmIRes.first == m_registry.end())
			return false;

		// store it so we can update the mapped list next ...
		elmIt = elmIRes.first;
	}

	// update the mapped list of listeners, walk the existing
	// list (if any entries) to prevent duplicate addition of
	// listeners. This is a bit more costly at registration time
	// but will prevent the hard-to-notice duplicate event
	// propogation sequences that would happen if double-entries
	// were allowed.

	// note: use reference to make following code more simple
	EventListenerTable & evlTable = (*elmIt).second;

	for (EventListenerTable::iterator it = evlTable.begin(),
		itEnd = evlTable.end(); it != itEnd; it++)
	{
		bool bListenerMatch = (*it == inListener);

		if (bListenerMatch)
			return false;
	}

	// okay, event type validated, event listener validated,
	// event listener not already in map, add it

	evlTable.push_back(inListener);

	return true;
}


//
// EventManager::VDelListener		- Chapter 10, page 292
//
// Remove a listener/type pairing from the internal tables
//
// Returns false if the pairing was not found.
//
bool EventManager::VDelListener(
	EventListenerPtr const & inListener, EventType const & inType)
{
	if (!VValidateType(inType))
		return false;

	bool rc = false;

	// brute force method, iterate through all existing mapping
	// entries looking for the matching listener and remove it.

	for (EventListenerMap::iterator it = m_registry.begin(),
		itEnd = m_registry.end(); it != itEnd; it++)
	{
		uint32_t const    kEventId = it->first;
		EventListenerTable & table = it->second;

		for (EventListenerTable::iterator it2 = table.begin(),
			it2End = table.end(); it2 != it2End; it2++)
		{
			if (*it2 == inListener)
			{
				// found match, remove from table,
				table.erase(it2);

				// update return code
				rc = true;

				// and early-quit the inner loop as addListener()
				// code ensures that each listener can only
				// appear in one event's processing list once.
				break;
			}
		}
	}

	return rc;
}


//
// EventManager::VTrigger		- Chapter 10, page 293
//
// Fire off event - synchronous - do it NOW kind of thing -
// analogous to Win32 SendMessage() API.
//
// returns true if the event was consumed, false if not. Note
// that it is acceptable for all event listeners to act on an
// event and not consume it, this return signature exists to
// allow complete propogation of that shred of information from
// the internals of this system to outside uesrs.
//
bool EventManager::VTrigger(
	IEventData const & inEvent) const
{
	if (!VValidateType(inEvent.GetEventType()))
		return false;

	EventListenerMap::const_iterator itWC = m_registry.find(0);

	if (itWC != m_registry.end())
	{
		EventListenerTable const & table = itWC->second;

		bool processed = false;

		for (EventListenerTable::const_iterator it2 = table.begin(),
			it2End = table.end(); it2 != it2End; it2++)
		{
			(*it2)->HandleEvent(inEvent);
		}
	}

	EventListenerMap::const_iterator it =
		m_registry.find(inEvent.GetEventType().getHashValue());

	if (it == m_registry.end())
		return false;

	EventListenerTable const & table = it->second;

	bool processed = false;

	for (EventListenerTable::const_iterator it2 = table.begin(),
		it2End = table.end(); it2 != it2End; it2++)
	{
		EventListenerPtr listener = *it2;
		if (listener->HandleEvent(inEvent))
		{
			// only set to true, if processing eats the messages
			processed = true;
		}
	}

	return processed;
}

//
// EventManager::VQueueEvent			- Chapter 10, page 294
//
// Fire off event - asynchronous - do it WHEN the event system
// tick() method is called, normally at a judicious time during
// game-loop processing.
//
// returns true if the message was added to the processing queue,
// false otherwise.
//
bool EventManager::VQueueEvent(IEventDataPtr const & inEvent)
{
	ASSERT_DESCRIPTION(m_activeQueue >= 0,"No active queues at the moment");
	ASSERT_DESCRIPTION(m_activeQueue < kNumQueues,"Active queues higher than the number of available queues");

	if (!VValidateType(inEvent->GetEventType()))
		return false;

	EventListenerMap::const_iterator it =
		m_registry.find(inEvent->GetEventType().getHashValue());

	if (it == m_registry.end())
	{
		// if global listener is not active, then abort queue add
		EventListenerMap::const_iterator itWC = m_registry.find(0);

		if (itWC == m_registry.end())
		{
			// no listeners for this event, skipit
			return false;
		}
	}

	m_queues[m_activeQueue].push_back(inEvent);

	return true;
}


// EventManager::VThreadSafeQueueEvent			- Chapter 18, page 671

bool EventManager::VThreadSafeQueueEvent(IEventDataPtr const & inEvent)
{
	//m_RealtimeEventQueue.push(inEvent);
	//assert(0 && "Not implemented - the editor won't link with boost::thread");
	return true;
}

//
// EventManager::VAbortEvent			- Chapter 10, page 295
//
// Find the next-available instance of the named event type and
// remove it from the processing queue.
//
// This may be done up to the point that it is actively being
// processed ...  e.g.: is safe to happen during event processing
// itself.
//
// returns true if the event was found and removed, false
// otherwise
//
bool EventManager::VAbortEvent(EventType const & inType,
	bool allOfType)
{
	ASSERT_DESCRIPTION(m_activeQueue >= 0, "No active queues at the moment");
	ASSERT_DESCRIPTION(m_activeQueue < kNumQueues, "Active queues higher than the number of available queues");

	if (!VValidateType(inType))
		return false;

	EventListenerMap::iterator it = m_registry.find(inType.getHashValue());

	if (it == m_registry.end())
		return false; // no listeners for this event, skipit

	bool rc = false;

	// See a good discussion on this code here:
	// http://www.mcshaffry.com/GameCode/thread.php?threadid=517&sid=

	EventQueue &evtQueue = m_queues[m_activeQueue];

	for (EventQueue::iterator it = evtQueue.begin(),
		itEnd = evtQueue.end(); it != itEnd; it++)
	{
		if ((*it)->GetEventType() == inType)
		{
			it = evtQueue.erase(it);
			rc = true;
			if (!allOfType)
				break;
		}
		else
		{
			++it;
		}
	}

	return rc;
}


//
// EventManager::VTick			- Chapter 10, page 296
// EventManager::VTick			- Chapter 18, page 671  (for the threadsafe stuff)								
//
// Allow for processing of any queued messages, optionally
// specify a processing time limit so that the event processing
// does not take too long. Note the danger of using this
// artificial limiter is that all messages may not in fact get
// processed.
//
// returns true if all messages ready for processing were
// completed, false otherwise (e.g. timeout )
//
bool EventManager::VTick(unsigned long maxMillis)
{
	const  Uint32 curMs = SDL_GetTicks(); 
	unsigned long maxMs =
		maxMillis == IEventManager::kINFINITE
		? IEventManager::kINFINITE
		: (curMs + maxMillis);

	EventListenerMap::const_iterator itWC = m_registry.find(0);

	// --------------------------------------------------------

	// swap active queues, make sure new queue is empty after the
	// swap ...

	int queueToProcess = m_activeQueue;

	m_activeQueue = (m_activeQueue + 1) % kNumQueues;

	m_queues[m_activeQueue].clear();

	// now process as many events as we can ( possibly time
	// limited ) ... always do AT LEAST one event, if ANY are
	// available ...

	while (m_queues[queueToProcess].size() > 0)
	{
		IEventDataPtr event = m_queues[queueToProcess].front();

		m_queues[queueToProcess].pop_front();

		EventType const & eventType = event->GetEventType();

		EventListenerMap::const_iterator itListeners =
			m_registry.find(eventType.getHashValue());

		if (itWC != m_registry.end())
		{
			EventListenerTable const & table = itWC->second;

			bool processed = false;

			for (EventListenerTable::const_iterator
				it2 = table.begin(), it2End = table.end();
				it2 != it2End; it2++)
			{
				(*it2)->HandleEvent(*event);
			}
		}

		// no listerners currently for this event type, skipit
		if (itListeners == m_registry.end())
			continue;

		uint32_t const kEventId = itListeners->first;
		EventListenerTable const & table = itListeners->second;

		for (EventListenerTable::const_iterator
			it = table.begin(), end = table.end();
			it != end; it++)
		{
			((*it)->HandleEvent(*event));
		}

		const  Uint32 curMs = SDL_GetTicks();

		if (maxMillis != IEventManager::kINFINITE)
		{

			if (curMs >= maxMs)
			{
				// time ran about, abort processing loop
				break;
			}
		}
	}

	// if any events left to process, push them onto the active
	// queue.
	//
	// Note: to preserver sequencing, go bottom-up on the
	// raminder, inserting them at the head of the active
	// queue...

	bool queueFlushed = (m_queues[queueToProcess].size() == 0);

	if (!queueFlushed)
	{
		while (m_queues[queueToProcess].size() > 0)
		{
			IEventDataPtr event = m_queues[queueToProcess].back();

			m_queues[queueToProcess].pop_back();

			m_queues[m_activeQueue].push_front(event);
		}
	}

	// all done, this pass

	return queueFlushed;
}

// --- information lookup functions ---

//
// EventManager::VValidateType			- Chapter 10, page 299
//
// Validate an event type, this does NOT add it to the internal
// registry, only verifies that it is legal ( e.g. either the
// ident number is not yet assigned, or it is assigned to
// matching signature text, and the signature text is not empty
// ).
//
bool EventManager::VValidateType(
	EventType const & inType) const
{
	if (0 == inType.getStr().length())
		return false;

	if ((inType.getHashValue() == 0))
		return false;

	EventTypeSet::const_iterator evIt =
		m_typeList.find(inType);

	ASSERT_DESCRIPTION(evIt != m_typeList.end(), "Failed validation of an event type; it was probably not registered with the EventManager!");

	return true;
}

//
// EventManager::getListenerList		- Chapter 10, page 300
//
// Get the list of listeners associated with a specific event
// type
//
EventListenerList EventManager::GetListenerList(
	EventType const & eventType) const
{
	// invalid event type, so sad
	if (!VValidateType(eventType))
		return EventListenerList();

	EventListenerMap::const_iterator itListeners =
		m_registry.find(eventType.getHashValue());

	// no listerners currently for this event type, so sad
	if (itListeners == m_registry.end())
		return EventListenerList();

	EventListenerTable const & table = itListeners->second;

	// there was, but is not now, any listerners currently for
	// this event type, so sad
	if (table.size() == 0)
		return EventListenerList();

	EventListenerList result;

	result.reserve(table.size());

	for (EventListenerTable::const_iterator it = table.begin(),
		end = table.end(); it != end; it++)
	{
		result.push_back(*it);
	}

	return result;
}


//
// EventManager::GetTypeList			- Chapter 11
//
// Get the list of known event types
//
EventTypeList EventManager::GetTypeList(void) const
{
	// no entries, so sad
	if (m_typeList.size() == 0)
		return EventTypeList();

	EventTypeList result;

	result.reserve(m_typeList.size());

	for (EventTypeSet::const_iterator it = m_typeList.begin(),
		itEnd = m_typeList.end(); it != itEnd; it++)
	{
		result.push_back(*it);
	}

	return result;
}

void EventManager::RegisterEvent(const EventType & eventType)
{
	if (m_typeList.find(eventType)==m_typeList.end())
	{
		m_typeList.insert(eventType);
	}
}