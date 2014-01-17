#include <System\Assert.h>
#include <System\StdLibraries.h>
#include <System\Entity.h>
#include <System\EntitySystem.h>
#include <System\MinerEntityEvents.h>
#include <Logic\GameBoardBehaviour.h>
#include <Logic\StoneBehaviour.h>
#include <Logic\GameLogic.h>
#include <System\World.h>
#include <random>

using namespace AI;

extern GameLogic * g_pGameLogic;
extern EntitySystem * g_pEntitySystem;
const std::string AI::GameBoardBehaviour::COMPONENT_NAME = "GameBoardBehaviour";

/// <summary>
/// Initializes a new instance of the <see cref="GameBoardBehaviour"/> class.
/// </summary>
GameBoardBehaviour::GameBoardBehaviour() :IBehaviourComponent(GameBoardBehaviour::COMPONENT_NAME) 
{
	m_pEntityListener = EventListenerPtr(GCC_NEW GameBoardBehaviourListener(this));
}
/// <summary>
/// Initializes a new instance of the <see cref="GameBoardBehaviour"/> class.
/// </summary>
/// <param name="owner">The entity owner.</param>
GameBoardBehaviour::GameBoardBehaviour(Entity *owner): IBehaviourComponent(GameBoardBehaviour::COMPONENT_NAME, owner, 0)
{
	m_pEntityListener = EventListenerPtr(GCC_NEW GameBoardBehaviourListener(this));
}

/// <summary>
/// Behaves when the specified delta milliseconds have passed. It checks the output from the Input and depending on the fire rate decides if it can fire or not.
/// </summary>
/// <param name="deltaMilliseconds">The delta milliseconds.</param>
void GameBoardBehaviour::OnUpdate(uint32_t deltaMilliseconds)
{

	if (m_AIState == AISTATE::INITIALIZING)
	{
		InitializeStep(deltaMilliseconds);
	}
	else
	{
		RunningStep(deltaMilliseconds);
	}
}
/// <summary>
/// Initializes the step.
/// </summary>
/// <param name="deltaMilliseconds">The delta milliseconds.</param>
void GameBoardBehaviour::InitializeStep(uint32_t deltaMilliseconds)
{
	for (int i = 0; i < g_pGameLogic->GetWorld()->GetNumberOfStonesByRow(); ++i)
	{
		for (int j = 0; j <g_pGameLogic->GetWorld()->GetNumberOfRows(); ++j)
		{
			bool bSpawnStone = false;
			while (!bSpawnStone)
			{
				bSpawnStone = true;
				std::uniform_int_distribution<uint16_t> dis(0, m_StonesVector.size() - 1);
				uint16_t chosenStone = dis(g_pGameLogic->GetRandomEngine());
				HashedString stoneName = HashedString(m_StonesVector[chosenStone].c_str());
				//vertical check
				if (j >= 2)
				{
					std::shared_ptr<Cell> cellMinusOne = g_pGameLogic->GetWorld()->GetCellAt(i, j - 1);
					std::shared_ptr<Cell> cellMinusTwo = g_pGameLogic->GetWorld()->GetCellAt(i, j - 2);
					if (
						cellMinusOne->GetEntity()->GetEntityName().getHashValue()==cellMinusTwo->GetEntity()->GetEntityName().getHashValue()&&
						cellMinusOne->GetEntity()->GetEntityName().getHashValue() == stoneName.getHashValue()
						)
					{
						bSpawnStone = false;
					}
				}

				if (i >= 2)
				{
					std::shared_ptr<Cell> cellMinusOne = g_pGameLogic->GetWorld()->GetCellAt(i-1, j);
					std::shared_ptr<Cell> cellMinusTwo = g_pGameLogic->GetWorld()->GetCellAt(i-2, j);
					if (
						cellMinusOne->GetEntity()->GetEntityName().getHashValue() == cellMinusTwo->GetEntity()->GetEntityName().getHashValue() &&
						cellMinusOne->GetEntity()->GetEntityName().getHashValue() == stoneName.getHashValue()
						)
					{
						bSpawnStone = false;
					}
				}
				if (bSpawnStone)
				{
					std::shared_ptr<Entity> stone = g_pEntitySystem->CreateEntity(m_StonesVector[chosenStone].c_str());
					g_pGameLogic->GetWorld()->AddEntity(i, j, stone.get());

				}

			}
		}
	}
	m_AIState = AISTATE::RUNNING;
}

/// <summary>
/// Initializes the step.
/// </summary>
/// <param name="deltaMilliseconds">The delta milliseconds.</param>
void GameBoardBehaviour::RunningStep(uint32_t deltaMilliseconds)
{
	const std::vector<std::shared_ptr<Cell>> cellVector = g_pGameLogic->GetWorld()->GetCellVector();

	for (uint16_t i = 0; i < g_pGameLogic->GetWorld()->GetNumberOfStonesByRow(); ++i)
	{
		if (!cellVector[i]->IsBusy())
		{

			std::uniform_int_distribution<uint16_t> dis(0, m_StonesVector.size() - 1);
			uint16_t chosenStone = dis(g_pGameLogic->GetRandomEngine());

			std::shared_ptr<Entity> stone = g_pEntitySystem->CreateEntity(m_StonesVector[chosenStone].c_str());
			g_pGameLogic->GetWorld()->AddEntity(i, 0, stone.get());
			//we start with the cell locked... in that way the cell is not taken into account until next step
			g_pGameLogic->GetWorld()->GetCellAt(i, 0)->Lock();
		}
	}


	uint16_t currentIterator = 0;
	uint16_t currentEntity = 0;
	uint16_t vectorSize = cellVector.size();

	while (currentIterator <= vectorSize)
	{
		if
			(
			(currentIterator % g_pGameLogic->GetWorld()->GetNumberOfStonesByRow()) == 0
			)
		{
			if ((currentIterator - currentEntity)>2)
			{
				uint16_t multiplier = (currentIterator - currentEntity) - 2;
				for (uint16_t i = currentEntity; i < currentIterator; i++)
				{
					DestroyCell(cellVector[i].get(), multiplier);
				}
			}

			currentEntity = currentIterator;
			if (currentIterator == vectorSize)
			{
				break;
			}

		}

		if (
			!(cellVector[currentIterator]->IsBusy())
			||
			!(cellVector[currentEntity]->IsBusy())
			||
			(cellVector[currentIterator]->IsLocked())
			||
			(cellVector[currentEntity]->IsLocked())
			||
			(
			cellVector[currentIterator]->IsBusy() && cellVector[currentEntity]->IsBusy() &&
			(cellVector[currentIterator]->GetEntity()->GetEntityName().getHashValue() != cellVector[currentEntity]->GetEntity()->GetEntityName().getHashValue())

			)
			)
		{
			if ((currentIterator - currentEntity)>2)
			{
				uint16_t multiplier = (currentIterator - currentEntity) - 2;
				for (uint16_t i = currentEntity; i < currentIterator; i++)
				{
					DestroyCell(cellVector[i].get(), multiplier);
				}
			}
			currentEntity = currentIterator;
		}
		++currentIterator;
	}

	const std::vector<std::shared_ptr<Cell>> verticalCellVector = g_pGameLogic->GetWorld()->GetVerticalCellVector();

	currentIterator = 0;
	currentEntity = 0;
	vectorSize = verticalCellVector.size();

	while (currentIterator <= vectorSize)
	{
		if
			(
			(currentIterator % g_pGameLogic->GetWorld()->GetNumberOfRows()) == 0
			)
		{
			if ((currentIterator - currentEntity)>2)
			{
				uint16_t multiplier = (currentIterator - currentEntity) - 2;
				for (uint16_t i = currentEntity; i < currentIterator; i++)
				{
					DestroyCell(verticalCellVector[i].get(), multiplier);
				}
			}

			currentEntity = currentIterator;
			if (currentIterator == vectorSize)
			{
				break;
			}
		}

		if (
			!(verticalCellVector[currentIterator]->IsBusy())
			||
			!(verticalCellVector[currentEntity]->IsBusy())
			||
			(verticalCellVector[currentIterator]->IsLocked())
			||
			(verticalCellVector[currentEntity]->IsLocked())
			||
			(
			verticalCellVector[currentIterator]->IsBusy() && verticalCellVector[currentEntity]->IsBusy() &&
			(verticalCellVector[currentIterator]->GetEntity()->GetEntityName().getHashValue() != verticalCellVector[currentEntity]->GetEntity()->GetEntityName().getHashValue())

			)
			)
		{
			if ((currentIterator - currentEntity)>2)
			{
				uint16_t multiplier = (currentIterator - currentEntity) - 2;
				for (uint16_t i = currentEntity; i < currentIterator; i++)
				{
					DestroyCell(verticalCellVector[i].get(), multiplier);
				}
			}
			currentEntity = currentIterator;
		}
		++currentIterator;
	}

}
/// <summary>
/// DestroyCell
/// </summary>
/// <param name="positionX">The position x.</param>
/// <param name="positionY">The position y.</param>
/// <param name="multiplier">The Multiplier of the score and points.</param>
void GameBoardBehaviour::DestroyCell(Cell *cell, uint16_t multiplier)
{
	uint32_t points = dynamic_cast<StoneBehaviour*>(cell->GetUpdatableEntity()->GetComponents()[1].get())->GetPoints();
	g_pGameLogic->AddScore(points*multiplier);
	IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_DestroyActor(cell->GetEntity()->GetID())));
	std::shared_ptr<Entity> explosion = g_pEntitySystem->CreateEntity("Explosion");
	cell->SetEntity(explosion.get());
	cell->Lock();


}
/// <summary>
/// Builds the specified Component using the specified descriptor.
/// </summary>
/// <param name="descriptor">The descriptor.</param>
void GameBoardBehaviour::Build(JSONNode *descriptor)
{
	ASSERT_DESCRIPTION(descriptor, "Not a valid descriptor");

	std::vector<JSONNode *> stonesDescriptorVector = descriptor->GetChild("Stones")->GetArrayValues();

	for (const auto & stoneDescriptor : stonesDescriptorVector)
	{
		const char * stoneName = stoneDescriptor->GetString("Name");
		ASSERT_DESCRIPTION(stoneName && strlen(stoneName)>0, "Empty Stone Name");
		m_StonesVector.push_back(stoneName);
	}
}

/// <summary>
/// Clones the current Component
/// </summary>
/// <param name="descriptor">The descriptor.</param>
std::shared_ptr<IComponent> GameBoardBehaviour::Clone(Entity *entity)
{
	std::shared_ptr<GameBoardBehaviour> cloned = std::shared_ptr<GameBoardBehaviour>(GCC_NEW GameBoardBehaviour());

	cloned->m_StonesVector = this->m_StonesVector;
	cloned->m_Alive = this->m_Alive;
	cloned->m_Points = this->m_Points;
	cloned->m_Entity = entity;
	cloned->m_AIState = AISTATE::INITIALIZING;
	g_pGameLogic->AddBehaviour(cloned);
	IEventManager::Get()->VAddListener(cloned->m_pEntityListener, EvtData_SwapEntitiesRequested::sk_EventType);

	return cloned;
}

/// <summary>
/// OnEntityEvent: This method is in charge of handling the events that are raised inside the entity domain to communicate one component with another.
/// </summary>
/// <param name="descriptor">The descriptor.</param>
void GameBoardBehaviour::OnEntityEvent(IEventData const & event)
{

}


/// <summary>
/// OnEntityEvent: This method is in charge of handling the events that are raised inside the entity domain to communicate one component with another.
/// </summary>
/// <param name="descriptor">The descriptor.</param>
void GameBoardBehaviour::SwapCellEntities(const uint32_t &entityOne, const uint32_t &entityTwo)
{

	std::shared_ptr<Cell> cellOne=g_pGameLogic->GetWorld()->GetCellWhereEntityIsFound(entityOne);
	std::shared_ptr<Cell> cellTwo= g_pGameLogic->GetWorld()->GetCellWhereEntityIsFound(entityTwo);

	std::pair<uint16_t, uint16_t> positionAtWorldOne = g_pGameLogic->GetWorld()->GetBoardPositionFromCell(cellOne);
	std::pair<uint16_t, uint16_t> positionAtWorldTwo = g_pGameLogic->GetWorld()->GetBoardPositionFromCell(cellTwo);

	bool bEvaluateHit1 = EvaluateHit(positionAtWorldOne.first, positionAtWorldOne.second, cellTwo->GetEntity()->GetEntityName(),cellTwo->GetEntity()->GetID());
	bool bEvaluateHit2 = EvaluateHit(positionAtWorldTwo.first, positionAtWorldTwo.second, cellOne->GetEntity()->GetEntityName(), cellOne->GetEntity()->GetID());

	std::pair<uint16_t, uint16_t> middlePoint(
		(cellOne->GetEntity()->GetPosX() + cellTwo->GetEntity()->GetPosX()) / 2,
		(cellOne->GetEntity()->GetPosY() + cellTwo->GetEntity()->GetPosY()) / 2);

	if (bEvaluateHit1||bEvaluateHit2)
	{
		IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_StoneMovementRequested(cellOne->GetEntity()->GetID(),
			std::pair<uint16_t, uint16_t>(cellTwo->GetEntity()->GetPosX(), cellTwo->GetEntity()->GetPosY()), middlePoint)));

		IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_StoneMovementRequested(cellTwo->GetEntity()->GetID(),
			std::pair<uint16_t, uint16_t>(cellOne->GetEntity()->GetPosX(), cellOne->GetEntity()->GetPosY()), middlePoint)));

	}
	else
	{
		//IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_StoneMovementRequested(cellOne->GetEntity()->GetID(),
		//	std::pair<uint16_t, uint16_t>(cellTwo->GetEntity()->GetPosX(), cellTwo->GetEntity()->GetPosY()), middlePoint)));

		//IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_StoneMovementRequested(cellTwo->GetEntity()->GetID(),
		//	std::pair<uint16_t, uint16_t>(cellOne->GetEntity()->GetPosX(), cellOne->GetEntity()->GetPosY()), middlePoint)));

		IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_StoneMovementRequested(cellOne->GetEntity()->GetID(),
			std::pair<uint16_t, uint16_t>(cellOne->GetEntity()->GetPosX(), cellOne->GetEntity()->GetPosY()), middlePoint)));

		IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_StoneMovementRequested(cellTwo->GetEntity()->GetID(),
			std::pair<uint16_t, uint16_t>(cellTwo->GetEntity()->GetPosX(), cellTwo->GetEntity()->GetPosY()), middlePoint)));
	}


	//std::shared_ptr<Entity> temp = cellTwo->GetEntity();
	//cellTwo->SetEntity(cellOne->GetEntity());
	//cellOne->SetEntity(temp);

}
bool GameBoardBehaviour::EvaluateHit(uint16_t positionX, uint16_t positionY, const HashedString & entityName, const uint32_t &entityId) const
{
	//Left hit
	if (positionX > 1)
	{
		std::shared_ptr<Cell> cellLeftMinus1 = g_pGameLogic->GetWorld()->GetCellAt(positionX - 1, positionY);
		std::shared_ptr<Cell> cellLeftMinus2 = g_pGameLogic->GetWorld()->GetCellAt(positionX - 2, positionY);
		if (
			(cellLeftMinus1->IsBusy()) && (cellLeftMinus2->IsBusy())
			&&
			(cellLeftMinus1->GetEntity()->GetID()!=entityId) && (cellLeftMinus2->GetEntity()->GetID()!=entityId)
			&&
			(cellLeftMinus1->GetEntity()->GetEntityName() == entityName) && (cellLeftMinus2->GetEntity()->GetEntityName() == entityName)
			)
		{
			return true;
		}
	}
	//Right hit
	if (positionX< (g_pGameLogic->GetWorld()->GetNumberOfStonesByRow()-2))
	{
		std::shared_ptr<Cell> cellLeftMinus1 = g_pGameLogic->GetWorld()->GetCellAt(positionX + 1, positionY);
		std::shared_ptr<Cell> cellLeftMinus2 = g_pGameLogic->GetWorld()->GetCellAt(positionX + 2, positionY);
		if (
			(cellLeftMinus1->IsBusy()) && (cellLeftMinus2->IsBusy())
			&&
			(cellLeftMinus1->GetEntity()->GetID() != entityId) && (cellLeftMinus2->GetEntity()->GetID() != entityId)
			&&
			(cellLeftMinus1->GetEntity()->GetEntityName() == entityName) && (cellLeftMinus2->GetEntity()->GetEntityName() == entityName)
			)
		{
			return true;
		}
	}
	//Up hit

	if (positionY> 1)
	{
		std::shared_ptr<Cell> cellLeftMinus1 = g_pGameLogic->GetWorld()->GetCellAt(positionX, positionY-1);
		std::shared_ptr<Cell> cellLeftMinus2 = g_pGameLogic->GetWorld()->GetCellAt(positionX, positionY-2);
		if (
			(cellLeftMinus1->IsBusy()) && (cellLeftMinus2->IsBusy())
			&&
			(cellLeftMinus1->GetEntity()->GetID() != entityId) && (cellLeftMinus2->GetEntity()->GetID() != entityId)
			&&
			(cellLeftMinus1->GetEntity()->GetEntityName() == entityName) && (cellLeftMinus2->GetEntity()->GetEntityName() == entityName)
			)
		{
			return true;
		}
	}
	//Down hit
	if (positionY< (g_pGameLogic->GetWorld()->GetNumberOfRows()-2))
	{
		std::shared_ptr<Cell> cellLeftMinus1 = g_pGameLogic->GetWorld()->GetCellAt(positionX, positionY + 1);
		std::shared_ptr<Cell> cellLeftMinus2 = g_pGameLogic->GetWorld()->GetCellAt(positionX, positionY + 2);
		if (
			(cellLeftMinus1->IsBusy()) && (cellLeftMinus2->IsBusy())
			&&
			(cellLeftMinus1->GetEntity()->GetID() != entityId) && (cellLeftMinus2->GetEntity()->GetID() != entityId)
			&&
			(cellLeftMinus1->GetEntity()->GetEntityName() == entityName) && (cellLeftMinus2->GetEntity()->GetEntityName() == entityName)
			)
		{
			return true;
		}
	}


	//Vertical Middle Hit
	if (
		(positionY>0)&&(positionY< (g_pGameLogic->GetWorld()->GetNumberOfRows() - 1))
		)
	{
		std::shared_ptr<Cell> cellLeftMinus1 = g_pGameLogic->GetWorld()->GetCellAt(positionX, positionY - 1);
		std::shared_ptr<Cell> cellLeftMinus2 = g_pGameLogic->GetWorld()->GetCellAt(positionX, positionY + 1);
		if (
			(cellLeftMinus1->IsBusy()) && (cellLeftMinus2->IsBusy())
			&&
			(cellLeftMinus1->GetEntity()->GetID() != entityId) && (cellLeftMinus2->GetEntity()->GetID() != entityId)
			&&
			(cellLeftMinus1->GetEntity()->GetEntityName() == entityName) && (cellLeftMinus2->GetEntity()->GetEntityName() == entityName)
			)
		{
			return true;
		}
	}

	//Horizontal Middle Hit
	if (
		(positionX>0) && (positionX<(g_pGameLogic->GetWorld()->GetNumberOfStonesByRow() - 1))
		)
	{
		std::shared_ptr<Cell> cellLeftMinus1 = g_pGameLogic->GetWorld()->GetCellAt(positionX-1, positionY);
		std::shared_ptr<Cell> cellLeftMinus2 = g_pGameLogic->GetWorld()->GetCellAt(positionX+1, positionY);
		if (
			(cellLeftMinus1->IsBusy()) && (cellLeftMinus2->IsBusy())
			&&
			(cellLeftMinus1->GetEntity()->GetID() != entityId) && (cellLeftMinus2->GetEntity()->GetID() != entityId)
			&&
			(cellLeftMinus1->GetEntity()->GetEntityName() == entityName) && (cellLeftMinus2->GetEntity()->GetEntityName() == entityName)
			)
		{
			return true;
		}
	}


	return false;
}
/// <summary>
/// Handles the event. Entity Events right now... By now I am only creating fires, explosions and destroying actors (sic)
/// </summary>
/// <param name="event">The event.</param>
/// <returns></returns>
bool GameBoardBehaviourListener::HandleEvent(IEventData const & event) const
{
	EventType eventType = event.GetEventType();

	if (eventType == EvtData_SwapEntitiesRequested::sk_EventType)
	{
		EvtData_SwapEntitiesRequested const & ed = static_cast< const EvtData_SwapEntitiesRequested & >(event);
		m_GameBoard->SwapCellEntities(ed.m_EntityIdOne, ed.m_EntityIdTwo);
	}

	return true;
}