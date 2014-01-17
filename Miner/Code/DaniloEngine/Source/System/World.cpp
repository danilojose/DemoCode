#include <System\World.h>
#include <System\Assert.h>

void World::Build(JSONNode *descriptor)
{
	m_PosX = descriptor->GetUInteger("PosX");
	m_PosY = descriptor->GetUInteger("PosY");

	m_SquareSize = descriptor->GetUInteger("SquareSize");
	ASSERT_DESCRIPTION(m_SquareSize > 0, "Square Size must be greater than zero");
	m_NumberOfStonesByRow = descriptor->GetUInteger("NumberOfStonesByRow");
	ASSERT_DESCRIPTION(m_NumberOfStonesByRow > 0, "numberOfStonesByRow must be greater than zero");
	m_NumberOfRows = descriptor->GetUInteger("NumberOfRows");
	ASSERT_DESCRIPTION(m_NumberOfRows > 0, "numberOfRows must be greater than zero");
	m_CellVector.reserve(m_NumberOfRows*m_NumberOfStonesByRow);

	for (uint16_t i = 0; i < (m_NumberOfRows*m_NumberOfStonesByRow); i++)
	{
		uint16_t posX = m_PosX+((i%m_NumberOfStonesByRow) * m_SquareSize);


		uint16_t posY = m_PosY+((i / m_NumberOfStonesByRow) * m_SquareSize);

		m_CellVector.push_back(std::shared_ptr<Cell>(GCC_NEW Cell(posX, posY)));
	}
	for (uint16_t i = 0; i < m_NumberOfStonesByRow; i++)
	{
		for (uint16_t j = 0; j < m_NumberOfRows; j++)
		{
			m_VerticalCellVector.push_back(GetCellAt(i,j));
		}
	}



}
bool World::IsFull() const 
{

	const auto &cellIt = std::find_if(m_CellVector.begin(), m_CellVector.end(), [](std::shared_ptr<Cell> aCell)
	{ return aCell->GetEntity() ? true : false; });

	return cellIt != m_CellVector.end();
}
bool World::IsEmpty() const 
{
	const auto &cellIt = std::find_if(m_CellVector.begin(), m_CellVector.end(), [](std::shared_ptr<Cell> aCell)
	{ return aCell->GetEntity() ? true : false; });

	return cellIt == m_CellVector.end();
}
void World::AddEntity(uint16_t positionX, uint16_t positionY, Entity* entity)
{

	uint16_t positionAtVector = (positionY*m_NumberOfStonesByRow) + positionX;
	ASSERT_DESCRIPTION(positionAtVector < m_CellVector.size(), "Position requested does not exist in current gameboard");
	m_CellVector[positionAtVector]->SetEntity(entity);
	
}

std::shared_ptr<Cell> World::GetCellAt(uint16_t positionX, uint16_t positionY)
{
	uint16_t positionAtVector = (positionY*m_NumberOfStonesByRow) + positionX;
	ASSERT_DESCRIPTION(positionAtVector < m_CellVector.size(), "Position requested does not exist in current gameboard");
	return m_CellVector[positionAtVector];

}
std::shared_ptr<Cell> World::GetCellAtWorldPosition(uint16_t positionX, uint16_t positionY)
{

	uint16_t boardPositionX = (positionX - m_PosX) / m_SquareSize;
	uint16_t boardPositionY = (positionY - m_PosY) / m_SquareSize;
	uint16_t positionAtVector = (boardPositionY*m_NumberOfStonesByRow) + boardPositionX;
	if (positionAtVector >=0 && positionAtVector < m_CellVector.size())
	{
		return m_CellVector[positionAtVector];
	}
	else
	{
		return nullptr;
	}

}

std::shared_ptr<Cell> World::GetCellWhereEntityIsFound(const uint32_t &entityId)
{
	auto iter = std::find_if(m_CellVector.begin(), m_CellVector.end(), [&](std::shared_ptr<Cell> cell) {
		if (cell->GetEntity() != nullptr)
		{
			return cell->GetEntity()->GetID() == entityId;

		}
		else
		{
			return false;
		}
	}
	);
	if (iter != m_CellVector.end())
	{
		return *iter;
	}
	else
	{
		return nullptr;
	}

}

const Entity* World::GetEntityAt(uint16_t positionX, uint16_t positionY)
{
	return GetCellAt(positionX,positionY)->GetEntity();
}
const Entity* World::GetEntityAtWorldPosition(uint16_t positionX, uint16_t positionY)
{
	return GetCellAtWorldPosition(positionX,positionY)->GetEntity();

}


std::pair<uint16_t, uint16_t> World::GetBoardPositionFromCell(std::shared_ptr<Cell> cell)
{
	const auto &iter=std::find(m_CellVector.begin(), m_CellVector.end(), cell);
	ASSERT_DESCRIPTION(iter != m_CellVector.end(), "Cell not found in vector so we cannot find its position");

	uint16_t positionY = (iter - m_CellVector.begin()) / m_NumberOfStonesByRow;
	uint16_t positionX = (iter - m_CellVector.begin()) % m_NumberOfStonesByRow;
	return std::pair<uint16_t, uint16_t>(positionX, positionY);
}

