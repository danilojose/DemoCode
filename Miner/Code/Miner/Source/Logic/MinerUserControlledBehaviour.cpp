#include <System\Assert.h>
#include <System\StdLibraries.h>
#include <System\Entity.h>
#include <Logic\MinerUserControlledBehaviour.h>
#include <Logic\GameLogic.h>
#include <System\World.h>
#include <System\MinerEntityEvents.h>

const int PlayerFireDirection = 1;
const uint32_t PLAYER_SPEED = 20;
const uint32_t PLAYER_FIRERATE = 500;
using namespace AI;

const std::string AI::MinerUserControlledBehaviour::COMPONENT_NAME = "MinerUserControlledBehaviour";

extern GameLogic * g_pGameLogic;
/// <summary>
/// Initializes a new instance of the <see cref="UserControlledBehaviour"/> class.
/// </summary>
/// <param name="owner">The Entity owner identifier.</param>
/// <param name="selectedCellSound">The sound.</param>
MinerUserControlledBehaviour::MinerUserControlledBehaviour(Entity *owner,const std::string &selectedCellSound) 
						:IBehaviourComponent(MinerUserControlledBehaviour::COMPONENT_NAME,
						owner, 0), m_SelectedCellSound(selectedCellSound), m_ChosenEntityOne(0)
{
}

/// <summary>
/// Behaves when the specified delta milliseconds have passed. It checks the output from the Input and depending on the fire rate decides if it can fire or not.
/// </summary>
/// <param name="deltaMilliseconds">The delta milliseconds.</param>
void MinerUserControlledBehaviour::OnUpdate(uint32_t deltaMilliseconds)
{
	bool updatedPosition=false; 

	if (Input::mouseDown())
	{
		uint16_t mouseX = Input::mousePositionX();
		uint16_t mouseY = Input::mousePositionY();
		std::shared_ptr<Cell> cell=g_pGameLogic->GetWorld()->GetCellAtWorldPosition(mouseX, mouseY);
		if (cell!=nullptr&&cell->IsBusy())
		{
			Entity* entity = cell->GetUpdatableEntity();
			if (m_ChosenEntityOne == nullptr)
			{
				m_ChosenEntityOne = cell.get();
				cell->GetUpdatableEntity()->TriggerInternalEvent(IEventDataPtr(GCC_NEW EvtData_OnEntitySelected(cell->GetEntity()->GetID())));
			}
			else
			{
				if (entity->GetID() == m_ChosenEntityOne->GetEntity()->GetID())
				{
					cell->GetUpdatableEntity()->TriggerInternalEvent(IEventDataPtr(GCC_NEW EvtData_OnEntityUnSelected(cell->GetEntity()->GetID())));
					m_ChosenEntityOne = nullptr;
				}
				else
				{

					uint16_t distance = abs(m_ChosenEntityOne->GetPosX() - cell->GetPosX()) + abs(m_ChosenEntityOne->GetPosY() - cell->GetPosY());
					if (distance == g_pGameLogic->GetWorld()->GetSquareSize())
					{
						IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_SwapEntitiesRequested(entity->GetID(), m_ChosenEntityOne->GetEntity()->GetID())));
					}

					m_ChosenEntityOne->GetUpdatableEntity()->TriggerInternalEvent(IEventDataPtr(GCC_NEW EvtData_OnEntityUnSelected(m_ChosenEntityOne->GetEntity()->GetID())));
					cell->GetUpdatableEntity()->TriggerInternalEvent(IEventDataPtr(GCC_NEW EvtData_OnEntityUnSelected(cell->GetEntity()->GetID())));
					m_ChosenEntityOne = nullptr;
				}
			}
		}
	}
}

/// <summary>
/// Builds the specified Component using the specified descriptor.
/// </summary>
/// <param name="descriptor">The descriptor.</param>
void MinerUserControlledBehaviour::Build(JSONNode *descriptor)
{
	m_SelectedCellSound = descriptor->GetString("SelectedCellSound");
	ASSERT_DESCRIPTION(!m_SelectedCellSound.empty(), "Movement Speed must be greater than zero");
}

/// <summary>
/// Clones the current Component
/// </summary>
/// <param name="descriptor">The descriptor.</param>
std::shared_ptr<IComponent> MinerUserControlledBehaviour::Clone(Entity *entity)
{
	std::shared_ptr<MinerUserControlledBehaviour> cloned = std::shared_ptr<MinerUserControlledBehaviour>(GCC_NEW MinerUserControlledBehaviour());
	cloned->m_SelectedCellSound = this->m_SelectedCellSound;
	cloned->m_Entity = entity;
	g_pGameLogic->AddBehaviour(cloned);
	return cloned;
}

/// <summary>
/// OnEntityEvent: This method is in charge of handling the events that are raised inside the entity domain to communicate one component with another.
/// </summary>
/// <param name="descriptor">The descriptor.</param>
void MinerUserControlledBehaviour::OnEntityEvent(IEventData const & event)
{

}