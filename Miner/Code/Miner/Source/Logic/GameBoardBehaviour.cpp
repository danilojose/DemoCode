#include <System\Assert.h>
#include <System\StdLibraries.h>
#include <System\Entity.h>
#include <Logic\GameBoardBehaviour.h>
#include <Logic\SimBinVadersLogic.h>
#include <System\World.h>

using namespace AI;

extern SimBinGameLogic * g_pGameLogic;

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
		m_StonesVector.push_back(HashedString(stoneName));
	}
}

/// <summary>
/// Clones the current Component
/// </summary>
/// <param name="descriptor">The descriptor.</param>
std::shared_ptr<IComponent> GameBoardBehaviour::Clone()
{
	std::shared_ptr<GameBoardBehaviour> cloned = std::shared_ptr<GameBoardBehaviour>(GCC_NEW GameBoardBehaviour());

	cloned->m_StonesVector = this->m_StonesVector;
	cloned->m_Alive = this->m_Alive;
	cloned->m_Points = this->m_Points;
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

	bool bEvaluateHit1 = EvaluateHit(positionAtWorldOne.first, positionAtWorldOne.second, cellTwo->GetEntity()->GetEntityName());
	bool bEvaluateHit2 = EvaluateHit(positionAtWorldTwo.first, positionAtWorldTwo.second, cellOne->GetEntity()->GetEntityName());

	if (bEvaluateHit1||bEvaluateHit2)
	{

		IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_StoneMovementRequested(entity->GetID(), m_ChosenEntityOne->GetEntity()->GetID())));
		IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_StoneMovementRequested(entity->GetID(), m_ChosenEntityOne->GetEntity()->GetID())));
	}
	else
	{
		IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_StoneMovementRequested(entity->GetID(), m_ChosenEntityOne->GetEntity()->GetID())));
		IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_StoneMovementRequested(entity->GetID(), m_ChosenEntityOne->GetEntity()->GetID())));
	}


	std::shared_ptr<Entity> temp = cellTwo->GetEntity();
	cellTwo->SetEntity(cellOne->GetEntity());
	cellOne->SetEntity(temp);

}
bool GameBoardBehaviour::EvaluateHit(uint16_t positionX, uint16_t positionY, const HashedString & entityName)
{
	//Left hit
	if (positionX > 1)
	{
		std::shared_ptr<Cell> cellLeftMinus1 = g_pGameLogic->GetWorld()->GetCellAt(positionX - 1, positionY);
		std::shared_ptr<Cell> cellLeftMinus2 = g_pGameLogic->GetWorld()->GetCellAt(positionX - 2, positionY);
		if (
			(cellLeftMinus1->IsBusy()) && (cellLeftMinus2->IsBusy())
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