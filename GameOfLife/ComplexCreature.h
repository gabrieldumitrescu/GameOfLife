/* Class to implement different multi-cell creature */


#ifndef _COMPLEXCREATURE_H_
#define _COMPLEXCREATURE_H_

#include <windows.h>
#include "BoxCreature.h"
#include <vector>
#include <string>
#include <fstream>

enum CreatureType {Glider,GliderGun,Pulsar,Dragon,Puffer,Enterprise,
									Buckaroo,ExtremelyImpressive,BigGlider,MWSS,Factory,Snail,Eater};

class ComplexCreature
{
public:
	ComplexCreature(CreatureType type);
	void setPos(POINT &startPos);
	void loadShape(const std::string &filename);
	void reverse(size_t worldX, size_t worldY);
	void live(std::vector<BoxCreature*> &creatures, size_t worldX, size_t worldY) const;


private:
	std::vector<POINT> m_alivePos;
	size_t m_numAlive;
	CreatureType m_type;

};


#endif /* _COMPLEXCREATURE_H_*/