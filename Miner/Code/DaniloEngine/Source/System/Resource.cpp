#include <System\Resource.h>

using namespace GameSystem;

/// <summary>
/// Vs the create handle.
/// </summary>
/// <param name="filename">The filename.</param>
/// <param name="pResCache">The p resource cache.</param>
/// <returns></returns>
ResHandle *Resource::CreateHandle(const std::string &filename, ResCache *pResCache)
{
	return GCC_NEW ResHandle(*this, filename, pResCache);
}


ResCache::~ResCache()
{
	Flush();
}




/// <summary>
/// Gets the handle.
/// </summary>
/// <param name="r">The r.</param>
/// <returns></returns>
std::shared_ptr<ResHandle> ResCache::GetHandle(Resource * r)
{
	std::shared_ptr<ResHandle> handle(Find(r));
	if (handle == NULL)
	{
		handle = Load(r);
	}
	else
	{
		Update(handle);
	}
	return handle;
}

/// <summary>
/// Loads the specified r.
/// </summary>
/// <param name="r">The r.</param>
/// <returns></returns>
std::shared_ptr<ResHandle> ResCache::Load(Resource *r)
{
	// Create a new resource and add it to the lru list and map
	std::shared_ptr<ResHandle> handle(r->CreateHandle(r->m_Filename, this));
	handle->Load();
	m_lru.push_front(handle);
	m_resources[r->m_Filename] = handle;
	return handle;
}


/// <summary>
/// Finds the specified r.
/// </summary>
/// <param name="r">The r.</param>
/// <returns></returns>
std::shared_ptr<ResHandle> ResCache::Find(Resource * r)
{
	ResHandleMap::iterator i = m_resources.find(r->m_Filename);
	if (i == m_resources.end())
	{
		return std::shared_ptr<ResHandle>();
	}

	return (*i).second;
}

/// <summary>
/// Updates the specified handle. It basically updates the lru list
/// </summary>
/// <param name="handle">The handle.</param>
void ResCache::Update(std::shared_ptr<ResHandle> handle)
{
	m_lru.remove(handle);
	m_lru.push_front(handle);
}

/// <summary>
/// Flushes this instance.
/// </summary>
void ResCache::Flush()
{
	while (!m_lru.empty())
	{
		std::shared_ptr<ResHandle> handle = *(m_lru.begin());
		Free(handle);
	}
}



/// <summary>
/// Frees the specified gonner.
/// </summary>
/// <param name="gonner">The gonner.</param>
void ResCache::Free(std::shared_ptr<ResHandle> gonner)
{
	m_lru.remove(gonner);
	m_resources.erase(gonner->m_Resource.m_Filename);
	// Note - the resource might still be in use by something,
	// so the cache can't actually count the memory freed until the
	// ResHandle pointing to it is destroyed.

	//m_allocated -= gonner->m_resource.m_size;
	//delete gonner;
}

/// <summary>
/// Memories the has been freed.
/// </summary>
/// <param name="size">The size.</param>
void ResCache::MemoryHasBeenFreed(unsigned int size)
{
	// Here we should release memory but the memory that we are managing with SDL (the only game library I am working with at this moment looks like it
	// handles the memory in a dark way... so I do not handle memory in my resource cache.
}
