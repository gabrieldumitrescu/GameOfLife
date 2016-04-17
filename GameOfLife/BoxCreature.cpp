/* BoxCreature class impl */


#include "BoxCreature.h"

BoxCreature::BoxCreature(bool alive):
	m_alive(alive),
	m_willDie(false),
	m_willLive(false)
{
}

BoxCreature::~BoxCreature()
{
}

BoxCreature::BoxCreature(const BoxCreature& other):
m_alive(other.m_alive),
m_willDie(other.m_willDie),
m_willLive(other.m_willLive),
m_neighbors(other.m_neighbors)
{

}

BoxCreature& BoxCreature::operator=(BoxCreature right)
{
	swap(*this,right);
	return (*this);
}

bool BoxCreature::isAlive() const
{
	return m_alive;
}

void BoxCreature::setAlive()
{
	m_alive=true;
}

void BoxCreature::transition()
{
	if(m_willDie) 
	{
		m_alive=false;
		m_willDie=false;
	}
	if(m_willLive) 
	{
		m_alive=true;
		m_willLive=false;
	}
}

void BoxCreature::live()
{
	size_t liveNeighbors=0;
	for(size_t i=0;i<m_neighbors.size();++i)
	{
		if(m_neighbors[i]->isAlive()) liveNeighbors++;
	}
	if(liveNeighbors<2 || liveNeighbors>3) m_willDie=true;
	else{
		if(liveNeighbors==3 && !m_alive) m_willLive=true;
		//if(liveNeighbors==2 && !m_alive) m_willLive=true;
	}

}

void BoxCreature::setPos(RECT& pos)
{
	m_pos=pos;
}

void BoxCreature::setGridPos(POINT& gridPos)
{
	m_gridPos=gridPos;
}

RECT BoxCreature::getPos() const
{
	return m_pos;
}

POINT BoxCreature::getGridPos() const
{
	return m_gridPos;
}

void BoxCreature::addNeighbor(BoxCreature* bp)
{
	m_neighbors.push_back(bp);
}



void swap(BoxCreature& first, BoxCreature& second)
{
	using std::swap;
	swap(first.m_alive,second.m_alive);
	swap(first.m_neighbors,second.m_neighbors);
}


