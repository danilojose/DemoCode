#pragma once
#include <System\FileSystemReader.h>
#include <System\EntitySystem.h>
#include <System\World.h>
/// <summary>
/// Entity class that stores the different subcomponents. (pending to make them inherit a single interface component)
/// </summary>
class StoneSpawner
{

protected:
	std::shared_ptr<World> m_World;
	std::shared_ptr<EntitySystem> m_EntitySystem;
public:

	StoneSpawner() {}
	virtual ~StoneSpawner(){}

	void Build(JSONNode *descriptor);

	void SpawnEntities();
};