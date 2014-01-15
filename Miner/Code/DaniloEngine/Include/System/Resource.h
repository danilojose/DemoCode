#pragma once

#include <System\StdLibraries.h>



namespace GameSystem
{
//typedefs and forward declarations
class ResHandle;
class ResCache;
typedef std::list< std::shared_ptr <ResHandle > > ResHandleList;			// lru list
typedef std::unordered_map<std::string, std::shared_ptr < ResHandle  > > ResHandleMap;		// maps indentifiers to resource data

/// <summary>
/// Filename oriented resource, it contains a resource that will later be used as key and is able to create a handle that is the one that really consumes the memory
/// </summary>
class Resource
{
public:
	std::string m_Filename;
	/// <summary>
	/// Initializes a new instance of the <see cref="Resource"/> class.
	/// </summary>
	/// <param name="name">The name.</param>
	Resource(std::string name) :m_Filename(name){}
	/// <summary>
	/// Vs the create handle.
	/// </summary>
	/// <param name="filename">The filename.</param>
	/// <param name="pResCache">The p resource cache.</param>
	/// <returns></returns>
	virtual ResHandle *CreateHandle(const std::string &filename, ResCache *pResCache);
};

/// <summary>
/// Handle to a physical resource that is stored in the resources cache in a way that only one instance of the resource is consuming memory
/// </summary>
class ResHandle
{
	friend class ResCache;

protected:
	Resource m_Resource;
	const std::string m_Filename;

	ResCache *m_pResCache;

public:
	/// <summary>
	/// Initializes a new instance of the <see cref="ResHandle"/> class.
	/// </summary>
	/// <param name="resource">The resource.</param>
	/// <param name="filename">The filename.</param>
	/// <param name="pResCache">The p resource cache.</param>
	ResHandle(Resource & resource, const std::string &filename, ResCache *pResCache) : m_Resource(resource), m_Filename(filename), m_pResCache(pResCache)
	{
	}
	/// <summary>
	/// Finalizes an instance of the <see cref="ResHandle"/> class.
	/// </summary>
	virtual ~ResHandle()
	{
		//TODO Call ResourceCache to actually release the space of the resource.
	}
	
	/// <summary>
	/// Gets the name of the file.
	/// </summary>
	/// <returns></returns>
	const std::string & GetFileName() { return m_Filename; }

	/// <summary>
	/// Loads the required data from the resource
	/// </summary>
	/// <returns></returns>
	virtual void Load() {} 

};

/// <summary>
/// Resources cache, is the system that stores the singleton resources so that no more that one instance is consuming memory
/// </summary>
class ResCache
{
	friend class ResHandle;

	ResHandleList m_lru;								// lru list
	ResHandleMap m_resources;

protected:
	/// <summary>
	/// Frees the specified gonner.
	/// </summary>
	/// <param name="gonner">The gonner.</param>
	void Free(std::shared_ptr<ResHandle> gonner);
	/// <summary>
	/// Loads the specified r.
	/// </summary>
	/// <param name="r">The r.</param>
	/// <returns></returns>
	std::shared_ptr<ResHandle> Load(Resource * r);
	/// <summary>
	/// Finds the specified r.
	/// </summary>
	/// <param name="r">The r.</param>
	/// <returns></returns>
	std::shared_ptr<ResHandle> Find(Resource * r);
	/// <summary>
	/// Updates the specified handle. It basically updates the lru list
	/// </summary>
	/// <param name="handle">The handle.</param>
	void Update(std::shared_ptr<ResHandle> handle);

	void MemoryHasBeenFreed(unsigned int size);

private:
	/// <summary>
	/// Prevents a default instance of the <see cref="ResCache"/> class from being created.
	/// </summary>
	/// <param name="">The .</param>
	ResCache(const ResCache &);
	/// <summary>
	/// Operator=s the specified .
	/// </summary>
	/// <param name="">The .</param>
	/// <returns></returns>
	ResCache& operator=(const ResCache&);

public:

	/// <summary>
	/// Initializes a new instance of the <see cref="ResCache"/> class.
	/// </summary>
	ResCache(){}

	/// <summary>
	/// Finalizes an instance of the <see cref="ResCache"/> class.
	/// </summary>
	virtual ~ResCache();
	/// <summary>
	/// Gets the handle.
	/// </summary>
	/// <param name="r">The r.</param>
	/// <returns></returns>
	std::shared_ptr<ResHandle> GetHandle(Resource * r);
	/// <summary>
	/// Flushes this instance.
	/// </summary>
	void Flush(void);

};

}