#pragma once
#include <System\StdLibraries.h>
#include <System\Entity.h>
#include <System\Assert.h>
namespace GameSystem
{
	struct Cell
	{
		uint16_t								m_PosX;
		uint16_t								m_PosY;
		Entity*									m_Entity;
		bool									m_Locked;
	public :
		Cell(uint16_t posX, uint16_t posY):m_PosX(posX),m_PosY(posY),m_Locked(false){}
		Cell(uint16_t posX, uint16_t posY,Entity* entity) :m_PosX(posX), m_PosY(posY),m_Entity(entity){}
		inline void Clear() { m_Entity = nullptr; }
		inline uint16_t GetPosX() const { return m_PosX; }  
		inline uint16_t GetPosY()const { return m_PosY; } 
		inline const Entity* GetEntity() const { return m_Entity; } 
		inline Entity* GetUpdatableEntity() { return m_Entity; }
		inline void SetEntity(Entity* entity){
			ASSERT_DESCRIPTION(entity, "Entity must not be nullptr");
			m_Entity = entity;
			m_Entity->SetPosX(m_PosX);
			m_Entity->SetPosY(m_PosY);
		}
		inline bool IsBusy(){ return m_Entity!=nullptr; }

		inline bool IsLocked() const { return m_Locked; }
		inline const void Lock() { m_Locked = true; }
		inline const void UnLock() { m_Locked = false; }
	private:
		Cell();
	};
	/// <summary>
	/// Entity class that stores the different subcomponents. (pending to make them inherit a single interface component)
	/// </summary>
	class World
	{

	protected:
		uint16_t										m_PosX;
		uint16_t									    m_PosY;
		uint16_t										m_SquareSize;
		uint16_t										m_NumberOfStonesByRow;
		uint16_t										m_NumberOfRows;
		std::vector<std::shared_ptr<Cell>>				m_CellVector;
		std::vector<std::shared_ptr<Cell>>				m_VerticalCellVector;
	public:

		World() {}
		virtual ~World(){}
		void Build(JSONNode *descriptor);
		bool IsFull() const ;
		bool IsEmpty() const;
		inline uint16_t GetNumberOfStonesByRow() const { return m_NumberOfStonesByRow; }
		inline uint16_t GetNumberOfRows() const { return m_NumberOfRows; }
		inline uint16_t GetSquareSize() const { return m_SquareSize; }
		void AddEntity(uint16_t positionX, uint16_t positionY, Entity* entity);

		std::shared_ptr<Cell> GetCellAt(uint16_t positionX, uint16_t positionY);
		std::shared_ptr<Cell> GetCellAtWorldPosition(uint16_t positionX, uint16_t positionY);
		std::shared_ptr<Cell> GetCellWhereEntityIsFound(const uint32_t &entityId);

		const Entity* GetEntityAt(uint16_t positionX, uint16_t positionY);
		const Entity* GetEntityAtWorldPosition(uint16_t positionX, uint16_t positionY);
		std::pair<uint16_t,uint16_t> GetBoardPositionFromCell(std::shared_ptr<Cell> cell);

		std::vector<std::shared_ptr<Cell>> & GetCellVector() 
		{
			return m_CellVector;
		}

		std::vector<std::shared_ptr<Cell>> & GetVerticalCellVector()
		{
			return m_VerticalCellVector;
		}


	};
}
