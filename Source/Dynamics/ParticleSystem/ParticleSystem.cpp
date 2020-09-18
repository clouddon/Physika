#include "ParticleSystem.h"
#include "PositionBasedFluidModel.h"

#include "Framework/Topology/PointSet.h"
#include "Core/Utility.h"


namespace PhysIKA
{
	IMPLEMENT_CLASS_1(ParticleSystem, TDataType)

	template<typename TDataType>
	ParticleSystem<TDataType>::ParticleSystem(std::string name)
		: Node(name)
	{
		attachField(&m_position, MechanicalState::position(), "Storing the particle positions!", false);
		attachField(&m_velocity, MechanicalState::velocity(), "Storing the particle velocities!", false);
		attachField(&m_force, MechanicalState::force(), "Storing the force densities!", false);

		m_pSet = std::make_shared<PointSet<TDataType>>();
		this->setTopologyModule(m_pSet);

// 		m_pointsRender = std::make_shared<PointRenderModule>();
// 		this->addVisualModule(m_pointsRender);
	}

	template<typename TDataType>
	ParticleSystem<TDataType>::~ParticleSystem()
	{
		
	}


	template<typename TDataType>
	void ParticleSystem<TDataType>::loadParticles(std::string filename)
	{
		m_pSet->loadObjFile(filename);
	}

	template<typename TDataType>
	void ParticleSystem<TDataType>::loadParticles(Coord center, Real r, Real distance)
	{
		std::vector<Coord> vertList;
		std::vector<Coord> normalList;

		Coord lo = center - r;
		Coord hi = center + r;

		for (Real x = lo[0]; x <= hi[0]; x +=  distance)
		{
			for (Real y = lo[1]; y <= hi[1]; y += distance)
			{
				for (Real z = lo[2]; z <= hi[2]; z += distance)
				{
					Coord p = Coord(x, y, z);
					if ((p-center).norm() < r)
					{
						vertList.push_back(Coord(x, y, z));
					}
				}
			}
		}
		normalList.resize(vertList.size());

		m_pSet->setPoints(vertList);
		m_pSet->setNormals(normalList);

		vertList.clear();
		normalList.clear();
	}

	template<typename TDataType>
	void ParticleSystem<TDataType>::loadParticles(Coord lo, Coord hi, Real distance)
	{
		std::vector<Coord> vertList;
		std::vector<Coord> normalList;

		for (Real x = lo[0]; x <= hi[0]; x += distance)
		{
			for (Real y = lo[1]; y <= hi[1]; y += distance)
			{
				for (Real z = lo[2]; z <= hi[2]; z += distance)
				{
					Coord p = Coord(x, y, z);
					vertList.push_back(Coord(x, y, z));
				}
			}
		}
		normalList.resize(vertList.size());

		m_pSet->setPoints(vertList);
		m_pSet->setNormals(normalList);

		std::cout << "particle number: " << vertList.size() << std::endl;

		vertList.clear();
		normalList.clear();
	}

	template<typename TDataType>
	void ParticleSystem<TDataType>::loadHeightFieldParticles(Coord lo, Coord hi, Real distance, Real slope)
	{
		std::vector<Coord> vertList;
		std::vector<Coord> normalList;
		
		float height,e = 2.71828;
		float xcenter = (hi[0] - lo[0]) / 2, zcenter = (hi[2] - lo[2]) / 2;
		for (Real x = lo[0]; x <= hi[0]; x += distance)
		{
			for (Real z = lo[2]; z <= hi[2]; z += distance)
			{
				//if (pow(x - xcenter, 2) + pow(z - zcenter, 2) > 400*distance*distance)
					//height = slope * pow(e, -400*distance*distance*100);
				//	height = slope * pow(e, -400 * distance*distance * 10);
				//else
					height = 0.3+slope * pow(e, -(pow(x - xcenter, 2) + pow(z - zcenter, 2))*100);
				//height = slope * pow(e, -(pow(x, 2) + pow(z, 2)) * 5);
				//height = 3*slope*(x + z - lo[0] - lo[2]);
				Coord p = Coord(x, 0, z);
				vertList.push_back(Coord(x, height + lo[1], z));
				normalList.push_back(Coord(0, 1, 0));
			}
		}
		m_pSet->setPoints(vertList);
		m_pSet->setNormals(normalList);

		m_position.setElementCount(vertList.size());
		Function1Pt::copy(m_position.getValue(), vertList);

		m_velocity.setElementCount(vertList.size());

		vertList.clear();
		normalList.clear();
	}
	template<typename TDataType>
	bool ParticleSystem<TDataType>::translate(Coord t)
	{
		m_pSet->translate(t);

		return true;
	}


	template<typename TDataType>
	bool ParticleSystem<TDataType>::scale(Real s)
	{
		m_pSet->scale(s);

		return true;
	}

	template<typename TDataType>
	bool ParticleSystem<TDataType>::initialize()
	{
		return Node::initialize();
	}

// 	template<typename TDataType>
// 	void ParticleSystem<TDataType>::setVisible(bool visible)
// 	{
// 		if (m_pointsRender == nullptr)
// 		{
// 			m_pointsRender = std::make_shared<PointRenderModule>();
// 			this->addVisualModule(m_pointsRender);
// 		}
// 
// 		Node::setVisible(visible);
// 	}

	template<typename TDataType>
	void ParticleSystem<TDataType>::updateTopology()
	{
		auto pts = m_pSet->getPoints();
		Function1Pt::copy(pts, getPosition()->getValue());
	}


	template<typename TDataType>
	bool ParticleSystem<TDataType>::resetStatus()
	{
		auto pts = m_pSet->getPoints();

		m_position.setElementCount(pts.size());
		m_velocity.setElementCount(pts.size());
		m_force.setElementCount(pts.size());

		Function1Pt::copy(m_position.getValue(), pts);
		m_velocity.getReference()->reset();

		return Node::resetStatus();
	}

// 	template<typename TDataType>
// 	std::shared_ptr<PointRenderModule> ParticleSystem<TDataType>::getRenderModule()
// 	{
// // 		if (m_pointsRender == nullptr)
// // 		{
// // 			m_pointsRender = std::make_shared<PointRenderModule>();
// // 			this->addVisualModule(m_pointsRender);
// // 		}
// 
// 		return m_pointsRender;
// 	}
}