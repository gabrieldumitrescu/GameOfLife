/* ComplexCreature class impl*/

#include "ComplexCreature.h"

ComplexCreature::ComplexCreature(CreatureType type):
	m_type(type)
{
	switch(m_type)
	{
	case Glider:
		loadShape("Shapes/glider.cells");
		break;
	case GliderGun:
		loadShape("Shapes/Gosper_glider_gun.cells");
		break;
	case Puffer:
		loadShape("Shapes/puffer.cells");
		break;
	case Enterprise:
		loadShape("Shapes/Enterprise.cells");
		break;
	case ExtremelyImpressive:
		loadShape("Shapes/extremely_impressive.cells");
		break;
	case Buckaroo:
		loadShape("Shapes/buckaroo.cells");
		break;
	case BigGlider:
		loadShape("Shapes/big_glider.cells");
		break;
	case MWSS:
		loadShape("Shapes/MWSS.cells");
		break;
	case Pulsar:
		loadShape("Shapes/pulsar.cells");
		break;
	case Dragon:
		loadShape("Shapes/dragon.cells");
		break;
	case Factory:
		loadShape("Shapes/factory.cells");
		break;
	case Snail:
		loadShape("Shapes/snail.cells");
		break;
	case Eater:
		loadShape("Shapes/eater.cells");
		break;
	}
	m_numAlive=m_alivePos.size();
}

void ComplexCreature::loadShape(const std::string &filename)
{
	std::ifstream in(filename.c_str());
	if(!in.is_open()) 
	{
		TCHAR mess[256];
		wsprintf(mess,TEXT("\nCould not open cells file %s.\n"),filename.c_str());
		OutputDebugString(mess);
		return;
	}
	std::string line;
	size_t numRow=0;
	POINT current;
	while(getline(in,line))
	{
		// empty line or comment
		if(line=="" || line[0]== '!') continue;
		current.x=numRow;
		for (size_t i=0; i<line.size();++i)
		{
			if(line[i] == 'O')
			{
				current.y=i;
				m_alivePos.push_back(current);
			}
		}
		numRow++;
	}
	in.close();
}


void ComplexCreature::setPos(POINT &startPos)
{
	for(size_t i=0;i<m_numAlive; ++i)
	{
		m_alivePos[i].x+=startPos.x;
		m_alivePos[i].y+=startPos.y;
	}
}

void ComplexCreature::reverse(size_t worldX, size_t worldY)
{
	for(size_t i=0;i<m_numAlive; ++i)
	{
		m_alivePos[i].x=worldX-m_alivePos[i].x;
		m_alivePos[i].y=worldY-m_alivePos[i].y;
	}
}

void ComplexCreature::live(std::vector<BoxCreature*> &creatures, size_t worldX, size_t worldY) const
{
	for(size_t i=0;i<m_numAlive; ++i)
	{
		creatures[m_alivePos[i].x * worldY + m_alivePos[i].y]->setAlive();
	}
}