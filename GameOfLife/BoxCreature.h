/* Class to implement a box-like
creature */


#ifndef _BOXCREATURE_H_
#define _BOXCREATURE_H_

#include <vector> 
#include <windows.h>



class BoxCreature{
	bool m_alive;
	bool m_willDie;
	bool m_willLive;
	std::vector<BoxCreature*> m_neighbors;
	RECT m_pos;
	POINT m_gridPos;
public:
	BoxCreature(bool alive=false);
	~BoxCreature();
	friend void swap(BoxCreature& first, BoxCreature& second); 
	BoxCreature(const BoxCreature& other);
	BoxCreature& operator=(BoxCreature right);
	bool isAlive() const;
	void setAlive();



	void setPos(RECT& pos);
	void setGridPos(POINT& gridPos);

	void addNeighbor(BoxCreature* cp);

	RECT getPos() const;
	POINT getGridPos() const;

	void live();
	void transition();

};




#endif // _BOXCREATURE_H_