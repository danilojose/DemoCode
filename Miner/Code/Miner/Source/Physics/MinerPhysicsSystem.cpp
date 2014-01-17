#include <Physics\MinerPhysicsSystem.h>
#include <System\World.h>
#include <System\MinerEntityEvents.h>

/// <summary>
/// This function for this game will be in charge of calculating if a stone should start falling or not
/// </summary>
/// <param name="deltaMilliseconds">The delta milliseconds.</param>
/// <returns></returns>
bool MinerPhysicsSystem::OnUpdate(uint32_t deltaMilliseconds)
{

	for (int column = 0; column < m_pWorld->GetNumberOfStonesByRow(); ++column)
	{
		bool bEmptySpaceFound=false;
		uint16_t drops = 0;
		for (int row = m_pWorld->GetNumberOfRows() - 1; row >= 0; --row)
		{

			std::shared_ptr<Cell> cell = m_pWorld->GetCellAt(column,row);
			if (!cell->IsBusy())
			{
				bEmptySpaceFound = true;
				++drops;
			}
			else
			{
				if (bEmptySpaceFound)
				{
					std::pair<uint16_t, uint16_t> cellPosition = m_pWorld->GetBoardPositionFromCell(cell);

					std::shared_ptr<Cell> cellTarget = m_pWorld->GetCellAt(cellPosition.first, cellPosition.second+drops);
					
					IEventManager::Get()->VQueueEvent(IEventDataPtr(GCC_NEW EvtData_StoneMovementRequested(cell->GetEntity()->GetID(),
						std::pair<uint16_t, uint16_t>(cellTarget->GetPosX(), cellTarget->GetPosY()), std::pair<uint16_t, uint16_t>(0,0))));
				}
			}

		}
	}
	return true;
}