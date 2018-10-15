#include "Piece.h"
#include "UserInterface.h"
#include "Well.h"

Piece::Piece(int x, int y, Well* WellPtr) : m_orientation(0), m_x(x), m_y(y), m_WellPtr(WellPtr)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			box[i][j] = 0;
	}
}


void Piece::move(char direction)
{
	switch (direction)
	{
	case 'u':
		m_y--;
		break;
	case 'd':
		m_y++;
		break;
	case 'l':
		m_x--;
		break;
	case 'r':
		m_x++;
		break;
	default:
		break;
	}
}

void Piece::rotate(int steps)
{
	m_orientation = ((m_orientation + steps) % 4);
	setOrientation(m_orientation);
}

void Piece::updateWell() const
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (box[i][j])
				m_WellPtr->setCoord(m_x + i, m_y + j, 3);
		}
	}
}


void Piece::clear(Screen& screen) const
{
	if (this == nullptr) return;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (box[i][j])
			{
				screen.gotoXY(m_x + i, m_y + j);
				screen.printChar(' ');
			}
		}
	}
}

void Piece::display(Screen& screen) const
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (box[i][j])
			{
				screen.gotoXY(m_x + i, m_y + j);
				screen.printChar('#');
			}
		}
	}
	screen.refresh();
}

bool Piece::checkOverlap() const
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (box[i][j] &&
				(m_WellPtr->getCoord(m_x + i, m_y + j) == 1 ||
					m_WellPtr->getCoord(m_x + i, m_y + j) == 4 || 
						m_WellPtr->getCoord(m_x + i, m_y + j) == 5))
				return true;
		}
	}
	return false;
}

bool Piece::shift(char ch)
{
	switch (ch)
	{
	case ARROW_DOWN:
		move('d');
		if (checkOverlap())
		{
			move('u');
			return false;
		}
		break;
	case ARROW_LEFT:
		move('l');
		if (checkOverlap())
		{
			move('r');
			return false;
		}
		break;
	case ARROW_RIGHT:
		move('r');
		if (checkOverlap())
		{
			move('l');
			return false;
		}
		break;
	case ARROW_UP:
		rotate(1);
		if (checkOverlap())
		{
			rotate(3);
			return false;
		}
		break;
	default:
		return false;
	}
	updateWell();

	return true;
}

void Piece::brickify() const
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (box[i][j])
				m_WellPtr->setCoord(m_x + i, m_y + j, 4);
		}
	}
}

I::I(int x, int y, Well* WellPtr) :Piece(x, y, WellPtr)
{
	setOrientation(0);
}

void I::setOrientation( int orientation)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (box[i][j])
				box[i][j] = 0;
		}
	}
	switch (orientation)
	{
	case 0:
	case 2:
		box[0][1] = 1;
		box[1][1] = 1;
		box[2][1] = 1;
		box[3][1] = 1;
		break;
	case 1:
	case 3:
		box[1][0] = 1;
		box[1][1] = 1;
		box[1][2] = 1;
		box[1][3] = 1;
	default:
		break;
	}
}

L::L(int x, int y, Well* WellPtr) :Piece(x, y, WellPtr) 
{
	setOrientation(0);
}

void L::setOrientation(int orientation)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (box[i][j])
				box[i][j] = 0;
		}
	}
	switch (orientation)
	{
	case 0:
		box[0][1] = 1;
		box[1][1] = 1;
		box[2][1] = 1;
		box[0][2] = 1;
		break;
	case 1:
		box[1][0] = 1;
		box[2][0] = 1;
		box[2][1] = 1;
		box[2][2] = 1;
		break;
	case 2:
		box[0][2] = 1;
		box[1][2] = 1;
		box[2][1] = 1;
		box[2][2] = 1;
		break;
	case 3:
		box[1][1] = 1;
		box[1][2] = 1;
		box[1][3] = 1;
		box[2][3] = 1;
	default:
		break;
	}
}

J::J(int x, int y, Well* WellPtr) :Piece(x, y, WellPtr)
{
	setOrientation(0);
}

void J::setOrientation(int orientation)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (box[i][j])
				box[i][j] = 0;
		}
	}
	switch (orientation)
	{
	case 0:
		box[1][1] = 1;
		box[2][1] = 1;
		box[3][1] = 1;
		box[3][2] = 1;
		break;
	case 1:
		box[2][1] = 1;
		box[2][2] = 1;
		box[2][3] = 1;
		box[1][3] = 1;
		break;
	case 2:
		box[1][1] = 1;
		box[1][2] = 1;
		box[2][2] = 1;
		box[3][2] = 1;
		break;
	case 3:
		box[1][0] = 1;
		box[2][0] = 1;
		box[1][1] = 1;
		box[1][2] = 1;
	default:
		break;
	}
}

T::T(int x, int y, Well* WellPtr) :Piece(x, y, WellPtr)
{
	setOrientation(0);
}

void T::setOrientation(int orientation)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (box[i][j])
				box[i][j] = 0;
		}
	}
	switch (orientation)
	{
	case 0:
		box[0][1] = 1;
		box[1][0] = 1;
		box[2][1] = 1;
		box[1][1] = 1;
		break;
	case 1:
		box[1][0] = 1;
		box[1][1] = 1;
		box[2][1] = 1;
		box[1][2] = 1;
		break;
	case 2:
		box[0][1] = 1;
		box[1][1] = 1;
		box[2][1] = 1;
		box[1][2] = 1;
		break;
	case 3:
		box[0][1] = 1;
		box[1][1] = 1;
		box[1][0] = 1;
		box[1][2] = 1;
	default:
		break;
	}
}

O::O(int x, int y, Well* WellPtr) :Piece(x, y, WellPtr)
{
	setOrientation(0);
}

void O::setOrientation(int orientation)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (box[i][j])
				box[i][j] = 0;
		}
	}
	switch (orientation)
	{
	case 0:
	case 1:
	case 2:
	case 3:
		box[0][0] = 1;
		box[1][0] = 1;
		box[0][1] = 1;
		box[1][1] = 1;
		break;
	}
}

S::S(int x, int y, Well* WellPtr) :Piece(x, y, WellPtr)
{
	setOrientation(0);
}

void S::setOrientation(int orientation)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (box[i][j])
				box[i][j] = 0;
		}
	}
	switch (orientation)
	{
	case 0:
	case 2:
		box[0][2] = 1;
		box[1][1] = 1;
		box[1][2] = 1;
		box[2][1] = 1;
		break;
	case 1:
	case 3:
		box[1][0] = 1;
		box[1][1] = 1;
		box[2][1] = 1;
		box[2][2] = 1;
		break;
	default:
		break;
	}
}

Z::Z(int x, int y, Well* WellPtr) :Piece(x, y, WellPtr)
{
	setOrientation(0);
}

void Z::setOrientation(int orientation)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (box[i][j])
				box[i][j] = 0;
		}
	}
	switch (orientation)
	{
	case 0:
	case 2:
		box[0][1] = 1;
		box[1][1] = 1;
		box[1][2] = 1;
		box[2][2] = 1;
		break;
	case 1:
	case 3:
		box[1][1] = 1;
		box[2][0] = 1;
		box[2][1] = 1;
		box[1][2] = 1;
		break;
	default:
		break;
	}
}

Crazy::Crazy(int x, int y, Well* WellPtr) :Piece(x, y, WellPtr)
{
	setOrientation(0);
}

void Crazy::setOrientation(int orientation)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (box[i][j])
				box[i][j] = 0;
		}
	}
	switch (orientation)
	{
	case 0:
		box[0][0] = 1;
		box[1][1] = 1;
		box[2][1] = 1;
		box[3][0] = 1;
		break;
	case 1:
		box[3][0] = 1;
		box[2][1] = 1;
		box[2][2] = 1;
		box[3][3] = 1;
		break;
	case 2:
		box[0][3] = 1;
		box[1][2] = 1;
		box[2][2] = 1;
		box[3][3] = 1;
		break;
	case 3:
		box[0][0] = 1;
		box[1][1] = 1;
		box[1][2] = 1;
		box[0][3] = 1;
	default:
		break;
	}
}

bool Crazy::shift(char ch)
{
	switch (ch)
	{
	case ARROW_DOWN:
		move('d');
		if (checkOverlap())
		{
			move('u');
			return false;
		}
		break;
	case ARROW_LEFT:
		move('r');
		if (checkOverlap())
		{
			move('l');
			return false;
		}
		break;
	case ARROW_RIGHT:
		move('l');
		if (checkOverlap())
		{
			move('r');
			return false;
		}
		break;
	case ARROW_UP:
		rotate(1);
		if (checkOverlap())
		{
			rotate(3);
			return false;
		}
		break;
	default:
		return false;
	}
	updateWell();

	return true;
}

Vapor::Vapor(int x, int y, Well* WellPtr) :Piece(x, y, WellPtr)
{
	setOrientation(0);
}

void Vapor::setOrientation(int orientation)
{
	switch (orientation)
	{
	case 0:
	case 1:
	case 2:
	case 3:
		box[1][0] = 1;
		box[2][0] = 1;
		break;
	default:
		break;
	}
}

void Vapor::special()
{
	for (int j = -2; j <= 2; j++)
	{
		if ((m_y + j > m_WellPtr->getHeight() - 1) || (m_y + j < 0))
			continue;
		for (int i = 1; i <= 2; i++)
			m_WellPtr->setCoord(m_x + i, m_y + j, 2);
	}
}

Foam::Foam(int x, int y, Well* WellPtr) :Piece(x, y, WellPtr)
{
	setOrientation(0);
}

void Foam::setOrientation(int orientation)
{
	switch (orientation)
	{
	case 0:
	case 1:
	case 2:
	case 3:
		box[1][1] = 1;
		break;
	default:
		break;
	}
}

void Foam::special()
{
	m_WellPtr->setCoord(m_x + 1, m_y + 1, 2);
	explode(0, 0);
}

void Foam::explode(int x, int y)
{
	if (
		x > 2 || x < -2 || y > 2 || y < -2 ||
		(m_WellPtr->getCoord(m_x + x + 1, m_y + y + 1) != 0 && m_WellPtr->getCoord(m_x + x + 1, m_y + y + 1) != 2)
		)
		return;
	m_WellPtr->setCoord(m_x + x + 1, m_y + y + 1, 5);
	explode(x + 1, y);
	explode(x - 1, y);
	explode(x, y + 1);
	if(m_y + y + 1 != 0)
		explode(x, y - 1);
}



