#ifndef WELL_INCLUDED
#define WELL_INCLUDED
#include "Piece.h"
#include "UserInterface.h"
#include <vector>
class Screen;

class Well
{
public:
	Well(int width, int height);
    void display(Screen& screen, int x, int y);
	int getHeight();
	int getCoord(int x, int y);
	void setCoord(int x, int y, int val);
	int vaporize(Screen& screen);
	void clearBricks(Screen& screen);
private:
	int m_width;
	int m_height;
	std::vector< std::vector <int> > matrix;

};

#endif // WELL_INCLUDED
