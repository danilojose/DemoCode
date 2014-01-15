#pragma once
#include <System\StdLibraries.h>
#include <System\IComponent.h>

using namespace GameSystem;
namespace AI
{
/// <summary>
/// IBehaviourComponent base class for implementing AI Behaviours
/// </summary>

	//TODO: Añadir componentes y hacer que esos componentes hereden de IComponent. Los IComponents van a tener una cadena del tipo parecido a los eventos el entity system te dice si acepta o no ese componente.
	// Entidad farola puede recibir tres tipos de componentes. y el EntitySystem es capaz de saber para una entidad "Farola" puede recibir componentes del tipo Logica, graficos.
class IBehaviourComponent:public IComponent
{
protected:
	
	bool m_Alive;
	uint16_t m_Points;
public:

	/// <summary>
	/// Initializes a new instance of the <see cref="IBehaviourComponent"/> class.
	/// </summary>
	/// <param name="componentId">The component identifier.</param>
	IBehaviourComponent(const std::string &componentId) :IComponent(componentId,nullptr),
		m_Alive(false), m_Points(0){}

	/// <summary>
	/// Initializes a new instance of the <see cref="IBehaviourComponent"/> class.
	/// </summary>
	/// <param name="ownerId">The ownerId identifier.</param>
	/// <param name="posX">The position x.</param>
	/// <param name="posY">The position y.</param>
	IBehaviourComponent(const std::string &componentId, Entity* owner, uint16_t points) :IComponent(componentId,owner),
						m_Alive(true),m_Points(points){}
	/// <summary>
	/// Finalizes an instance of the <see cref="IBehaviourComponent"/> class.
	/// </summary>
	virtual ~IBehaviourComponent(){
	};
	/// <summary>
	/// Implements the behaviour of the enemy. Expected behaviour is to move towards current direction until a barrier is found. At that moment change the direction and move towards the opposite.
	/// When the last fire was done longer than the current fire rate the enemy fires again
	/// </summary>
	/// <param name="deltaMilliseconds">The delta milliseconds.</param>
	virtual void OnUpdate(uint32_t deltaMilliseconds) = 0;

	/// <summary>
	/// Gets if the certain component is alive or not.
	/// </summary>
	/// <returns></returns>
	const bool GetAlive() const { return m_Alive; }

	/// <summary>
	/// Sets the Alive property
	/// </summary>
	/// <param name="alive">The alive.</param>
	virtual void SetAlive(bool alive) { m_Alive = alive; }

	/// <summary>
	/// Gets the Points property
	/// </summary>
	/// <param name="alive">The alive.</param>
	virtual const uint16_t GetPoints() const { return m_Points; }

};
}