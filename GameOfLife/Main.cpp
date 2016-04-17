
#include <windows.h>
#include <time.h>
#include <Math.h>
#include <vector>

#include "BoxCreature.h"
#include "WindowProgram.h"
#include "ComplexCreature.h"

COLORREF RandomColor(){
	int r=rand() % 256;
	int g=rand() % 256;
	int b=rand() % 256;
	return (RGB(r,g,b));
}

#define DEFAULT_FPS 15
#define CREATURE_SIZE 10

enum {IDS_CONTROL=50,IDB_PAUSE};

//#define MY_DEBUG

struct Creatures
{
	std::vector<BoxCreature*> m_creatures;
	HBRUSH liveBrush,deadBrush;
	POINT creatureGrid;
	size_t noCreatures;
	Creatures()
	{
		COLORREF aliveColor=RGB(255,255,0);
		COLORREF deadColor=RGB(128,128,128);
		liveBrush=CreateSolidBrush(aliveColor);
		deadBrush=CreateSolidBrush(deadColor);//CreateHatchBrush(HS_DIAGCROSS,deadColor);
	}

	void addCreatures(RECT &rBounds)
	{
		
		creatureGrid.x=rBounds.bottom/CREATURE_SIZE;
		creatureGrid.y=rBounds.right/CREATURE_SIZE;
		noCreatures=creatureGrid.x * creatureGrid.y;
		RECT currentPos;
		SetRect(&currentPos,rBounds.left,rBounds.top,CREATURE_SIZE,CREATURE_SIZE);
		POINT gridPos;
		gridPos.x=0;
		gridPos.y=0;
		for(size_t i=0; i<noCreatures;++i)
		{
			//bool alive= (rand() % 7) == 0;//gridPos.x>2 && gridPos.x<32  && 
			BoxCreature* current=new BoxCreature(false);
			current->setPos(currentPos);
			current->setGridPos(gridPos);
			m_creatures.push_back(current);
			if(++gridPos.y == creatureGrid.y){
				gridPos.y=0;
				gridPos.x++;
				SetRect(&currentPos,0,currentPos.top+CREATURE_SIZE,CREATURE_SIZE,currentPos.bottom+CREATURE_SIZE);
			}
			else
			{
				currentPos.left+=CREATURE_SIZE;
				currentPos.right+=CREATURE_SIZE;
			}
		}
		
		POINT pos;
		pos.x=5;
		pos.y=4;
		ComplexCreature glGun(GliderGun);
		glGun.setPos(pos);
		glGun.live(m_creatures,creatureGrid.x,creatureGrid.y);
		

		pos.x=creatureGrid.x-16;
		pos.y=creatureGrid.y-36;
		ComplexCreature eater(Eater);
		eater.setPos(pos);
		eater.live(m_creatures,creatureGrid.x,creatureGrid.y);

		
		for(size_t i=0; i<m_creatures.size();++i)
		{
			POINT pos=m_creatures[i]->getGridPos();

#ifdef MY_DEBUG
			//DEBUG
			TCHAR mess[256];
			wsprintf(mess,TEXT("\nNeighbors of (%d,%d):"),pos.x,pos.y);
			OutputDebugString(mess);
			//end DEBUG
#endif
			std::vector<POINT> neighborsPos;
			getNeighbors(pos,neighborsPos);
			for(size_t j=0; j<neighborsPos.size();++j)
			{
				m_creatures[i]->addNeighbor(getCreatureAt(neighborsPos[j]));
			}
		}

	}

	void getNeighbors(POINT& pos, std::vector<POINT>& neighborPos)
	{
		for(long i=pos.x-1;i<=pos.x+1;++i)
		{
			for(long j=pos.y-1;j<=pos.y+1;++j)
			{
				if(!(i==pos.x && j==pos.y))
				{
					POINT neigh;
					neigh.x=i;
					neigh.y=j;
					if(isIn(neigh))
						neighborPos.push_back(neigh);
				}
			}
		}
	}

	bool isIn(POINT &p)
	{
		return (p.x >=0 && p.x<creatureGrid.x && p.y>=0 && p.y<creatureGrid.y);
	}

	BoxCreature* getCreatureAt(POINT& gridPos)
	{
		return m_creatures[gridPos.x*creatureGrid.y + gridPos.y];
	}

	void drawCreature(BoxCreature& c, HDC hdc)
	{
		HBRUSH hBrush;
		RECT pos=c.getPos();
		if(c.isAlive()) hBrush=liveBrush;
		else hBrush=deadBrush;
		
		SelectObject(hdc,hBrush);

		Rectangle(hdc,pos.left,pos.top,pos.right,pos.bottom);
		//FillRect(hdc,&pos,hBrush);
	}

	void live()
	{
		for(size_t i=0; i<m_creatures.size();++i)
		{
			m_creatures[i]->live();
		}
		for(size_t i=0; i<m_creatures.size();++i)
		{
			m_creatures[i]->transition();
		}

	}
	void draw(HDC hdc)
	{
		for(size_t i=0; i<m_creatures.size();++i)
		{
			drawCreature(*m_creatures[i],hdc);
		}
	}

	~Creatures()
	{
		for(size_t i=0; i<m_creatures.size();++i)
		{
			delete m_creatures[i];
		}
		m_creatures.clear();
		DeleteObject(liveBrush);
		DeleteObject(deadBrush);
	}


};


struct AnimationState{
	DWORD m_fps;
	DWORD m_frameTime;
	RECT m_rBounds;
	RECT m_rBtnZone;
	RECT m_rGameZone;
	int m_iBtnWidth,m_iBtnHeight;
	HWND m_hwnd,m_hwndBtnPause;
	BOOL m_animationPaused;
	DWORD m_lastRunTime;
	Creatures m_creatures;
	AnimationState(HWND hwnd,DWORD fps=DEFAULT_FPS):
		m_fps(fps),
		m_animationPaused(FALSE),
		m_lastRunTime(0),
		m_frameTime(1000/fps),
		m_hwnd(hwnd),
		m_iBtnWidth(50),
		m_iBtnHeight(30)
	{
		GetClientRect(hwnd,&m_rBounds);
		SetRect(&m_rBtnZone,0,m_rBounds.bottom-45,m_rBounds.right,m_rBounds.bottom);
		SetRect(&m_rGameZone,0,0,m_rBounds.right,m_rBtnZone.top);
		m_creatures.addCreatures(m_rGameZone);

		HWND m_hwndBtnPause = CreateWindow(  
			TEXT("BUTTON"),   // predefined class 
			TEXT("PAUSE"),       // button text 
			WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // styles 
		    // Size and position values are given explicitly, because 
			// the CW_USEDEFAULT constant gives zero values for buttons. 
			(m_rBtnZone.right+m_iBtnWidth)/2,         // starting x position 
			m_rBtnZone.bottom-m_iBtnHeight-5,         // starting y position 
	        m_iBtnWidth,        // button width 
			m_iBtnHeight,        // button height 
			hwnd,       // parent window 
		    (HMENU) IDB_PAUSE,       // Button Id 
			(HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE), 
			NULL);      // pointer not needed 
	}

		void updateButtons()
		{
			MoveWindow(m_hwndBtnPause,(m_rBtnZone.right+m_iBtnWidth)/2,m_rBtnZone.bottom-m_iBtnWidth-5,m_iBtnWidth,m_iBtnHeight,true);
		}

	~AnimationState(){
		
	}
	void pauseAnimation(){
		m_animationPaused=TRUE;
	}
	void unpauseAnimation(){
		m_animationPaused=FALSE;
	}

	BOOL isPaused(){
		return m_animationPaused;
	}

	void setBounds(int x, int y){
		m_rBounds.right=x;
		m_rBounds.bottom=y;
	}

	BOOL needsRepaint(){
		DWORD currentTime=GetTickCount();
		return currentTime-m_lastRunTime >= m_frameTime;
	}
	void updateRunTime(){
		m_lastRunTime=GetTickCount();
	}

};

static AnimationState* animStat;

LRESULT CALLBACK WinProc(HWND,UINT,WPARAM,LPARAM);
void updateScreen(HWND hwnd);
void PaintGraphics(HDC hdc);



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd){
	WindowProgram wp(hInstance,hPrevInstance,lpCmdLine,nShowCmd,WinProc,TEXT("Game of life"));
	return wp.MessageLoopEx(updateScreen);
}



LRESULT CALLBACK WinProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam){
	switch(msg){
		case WM_CREATE:
			srand(5);
			animStat=new AnimationState(hWnd);
			return 0;
		case WM_DESTROY:
			delete animStat;
			PostQuitMessage(0);
			return 0;
		case WM_PAINT:
			animStat->updateButtons();
			return 0;
		case WM_COMMAND:
			switch (LOWORD(wParam)){
				case IDB_PAUSE:
					if(animStat->isPaused())
						animStat->unpauseAnimation();
					else
						animStat->pauseAnimation();
					break;
			}
			return 0;
		case WM_SIZE:
			animStat->setBounds(LOWORD(lParam),HIWORD(lParam));
			return 0;
	}

	return DefWindowProc(hWnd,msg,wParam,lParam);
}

void updateScreen(HWND hwnd){
	if(!animStat->isPaused() && animStat->needsRepaint()){
		HDC hdc=GetDC(hwnd);
		HDC cHdc=CreateCompatibleDC(hdc);
		RECT clientRect=animStat->m_rGameZone;
		HBITMAP hBitMap=CreateCompatibleBitmap(hdc,clientRect.right,clientRect.bottom);
		SelectObject(cHdc,hBitMap);
		FillRect(cHdc,&clientRect,(HBRUSH)(GetStockObject(GRAY_BRUSH)));
		PaintGraphics(cHdc);
		BitBlt(hdc,clientRect.left,clientRect.top,clientRect.right,clientRect.bottom,cHdc,0,0,SRCCOPY);
		DeleteObject(hBitMap);
		DeleteDC(cHdc);
		ReleaseDC(hwnd,hdc);
		animStat->updateRunTime();
	}
	//animStat->pauseAnimation();
}

void PaintGraphics(HDC hdc)
{
	animStat->m_creatures.draw(hdc);
	animStat->m_creatures.live();
	
}
