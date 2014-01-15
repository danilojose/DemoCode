#pragma once
#include <System\StdLibraries.h>
#include <System\Entity.h>

namespace GameSystem
{
	struct Cell
	{
		uint16_t m_PosX;
		uint16_t m_PosY;
		std::shared_ptr<Entity> m_Entity;
	public :
		Cell(uint16_t posX, uint16_t posY):m_PosX(posX),m_PosY(posY){}
		Cell(uint16_t posX, uint16_t posY,Entity* entity) :m_PosX(posX), m_PosY(posY),m_Entity(entity){}
		inline uint16_t GetPosX() const { return m_PosX; }  
		inline uint16_t GetPosY()const { return m_PosY; } 
		inline const std::shared_ptr<Entity> GetEntity() const { return m_Entity; } 
		inline std::shared_ptr<Entity> GetUpdatableEntity() { return m_Entity; }
		inline void SetEntity(std::shared_ptr<Entity> entity){
			m_Entity = entity;
			m_Entity->SetPosX(m_PosX);
			m_Entity->SetPosY(m_PosY);
		}
		inline bool IsBusy(){ return m_Entity!=nullptr; }
	private:
		Cell();
	};
	/// <summary>
	/// Entity class that stores the different subcomponents. (pending to make them inherit a single interface component)
	/// </summary>
	class World
	{

	protected:
		uint16_t m_PosX;
		uint16_t m_PosY;
		uint16_t m_SquareSize;
		uint16_t m_NumberOfStonesByRow;
		uint16_t m_NumberOfRows;
		std::vector<std::shared_ptr<Cell>> m_CellVector;
	public:

		World() {}
		virtual ~World(){}
		void Build(JSONNode *descriptor);
		bool IsFull() const ;
		bool IsEmpty() const;
		uint16_t GetNumberOfStonesByRow() const { return m_NumberOfStonesByRow; }
		uint16_t GetNumberOfRows() const { return m_NumberOfRows; }
		void AddEntity(uint16_t positionX, uint16_t positionY, std::shared_ptr<Entity> entity);
		std::shared_ptr<Cell> GetCellAt(uint16_t positionX, uint16_t positionY);
		std::shared_ptr<Cell> GetCellAtWorldPosition(uint16_t positionX, uint16_t positionY);
		std::shared_ptr<Cell> GetCellWhereEntityIsFound(const uint32_t &entityId);
		const std::shared_ptr<Entity> GetEntityAt(uint16_t positionX, uint16_t positionY);
		const std::shared_ptr<Entity> GetEntityAtWorldPosition(uint16_t positionX, uint16_t positionY);

		std::pair<uint16_t,uint16_t> GetBoardPositionFromCell(std::shared_ptr<Cell> cell);

	};
}
