#include "Well.h"
#include "UserInterface.h"
#include <vector>


Well::Well(int width, int height) :
	m_width(width + 1), m_height(height), matrix(m_width + 1, std::vector<int>(m_height + 1))
{
	for (int j = 0; j <= m_height; j++)
	{
		matrix[0][j] = 1;
		matrix[m_width][j] = 1;
	}
	for (int i = 0; i <= m_width; i++)
	{
		matrix[i][m_height] = 1;
	}
}



int Well::getHeight()
{
	return m_height;
}



int Well::getCoord(int x, int y)
{
	return matrix[x][y];
}

void Well::setCoord(int x, int y, int val)
{
	matrix[x][y] = val;
}


void Well::display(Screen& screen, int x, int y)
{
	for (int j = 0; j <= m_height; j++)
	{
		for (int i = 0; i <= m_width; i++)
		{
			if (matrix[i][j] == 0)
				continue;
			screen.gotoXY(x + i, y + j);
			if (matrix[i][j] == 1)
				screen.printChar('@');
			else if (matrix[i][j] == 2)
			{
				screen.printChar(' ');
				matrix[i][j] = 0;
			}
			else if (matrix[i][j] == 3)
			{
				screen.printChar('#');
				matrix[i][j] = 2;
			}
			else if (matrix[i][j] == 4)
				screen.printChar('$');
			else if (matrix[i][j] == 5)
				screen.printChar('*');
		}
	}
	screen.refresh();
}

int Well::vaporize(Screen& screen)
{
	for (int j = 0; j < m_height; j++)
	{
		for (int i = 1; i < m_width; i++)
		{
			if (matrix[i][j] == 4 || matrix[i][j] == 5)
			{
				screen.gotoXY(i, j);
				screen.printChar(' ');
			}
		}
	}
	int count = 0;
	for (int j = m_height - 1; j >= 0; j--)
	{
		int i;
		for (i = 1; i < m_width; i++)
		{
			if (matrix[i][j + count] != 4 && matrix[i][j + count] != 5)
				break;
		}
		if (i == m_width)
		{
			for (i = 1; i < m_width; i++)
			{
				matrix[i].erase(matrix[i].begin() + j + count);
				matrix[i].insert(matrix[i].begin(), 0);
			}
			count++;
		}
	}
	return count;
}

void Well::clearBricks(Screen& screen)
{
	for (int j = 0; j < m_height; j++)
	{
		for (int i = 1; i < m_width; i++)
		{
			if (matrix[i][j] == 4 || matrix[i][j] == 5)
			{
				matrix[i][j] = 0;
				screen.gotoXY(i, j);
				screen.printChar(' ');
			}
		}
	}
}

