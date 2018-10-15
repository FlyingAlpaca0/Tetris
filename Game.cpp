#include "Game.h"
#include "Well.h"
#include "UserInterface.h"
#include "Piece.h"
#include <string>
#include <cmath>
using namespace std;

const int SCREEN_WIDTH = 80;
const int SCREEN_HEIGHT = 25;

const int WELL_X = 0;
const int WELL_Y = 0;

const int PROMPT_Y = 20;
const int PROMPT_X = 0;

const int SCORE_X = 16;
const int SCORE_Y = 8;

const int ROWS_LEFT_X = 16;
const int ROWS_LEFT_Y = 9;

const int LEVEL_X = 16;
const int LEVEL_Y = 10;

const int NEXT_PIECE_TITLE_X = 16;
const int NEXT_PIECE_TITLE_Y = 3;

const int NEXT_PIECE_X = 16;
const int NEXT_PIECE_Y = 4;

const int PIECE_INITIAL_X = 3;
const int PIECE_INITIAL_Y = 0;

Game::Game(int width, int height)
 : m_screen(SCREEN_WIDTH, SCREEN_HEIGHT), m_level(1), m_well(width, height), m_score(0), m_rowsLeft(5*m_level),
	m_piece(nullptr), m_nextpiece(nullptr)
{
}

Game::~Game()
{
	delete m_piece;
	delete m_nextpiece;
}

void Game::play()
{
	m_well.display(m_screen, WELL_X, WELL_Y);
    displayStatus();  //  score, rows left, level
    displayPrompt("Press the Enter key to begin playing Chetyris!");
    waitForEnter();  // [in UserInterface.h]

	for(;;)
    {
			if (!playOneLevel())
				break;
			displayPrompt("Good job!  Press the Enter key to start next level!");
			waitForEnter();
			m_level++;
			displayStatus();
	    }
    displayPrompt("Game Over!  Press the Enter key to exit!");
    waitForEnter();
}

void Game::displayPrompt(std::string s)     
{
    m_screen.gotoXY(PROMPT_X, PROMPT_Y);
    m_screen.printStringClearLine(s);   // overwrites previous text
    m_screen.refresh();
}

void Game::displayStatus()
{
	m_screen.gotoXY(SCORE_X, SCORE_Y);
	m_screen.printStringClearLine("Score:");   // overwrites previous text
	m_screen.gotoXY(SCORE_X + 18 - to_string(m_score).size(), SCORE_Y);
	m_screen.printString(to_string(m_score));
	m_screen.gotoXY(ROWS_LEFT_X, ROWS_LEFT_Y);
	m_screen.printStringClearLine("Rows left:");   // overwrites previous text
	m_screen.gotoXY(ROWS_LEFT_X + 18 - to_string(m_rowsLeft).size(), ROWS_LEFT_Y);
	m_screen.printString(to_string(m_rowsLeft));
	m_screen.gotoXY(LEVEL_X, LEVEL_Y);
	m_screen.printStringClearLine("Level:");   // overwrites previous text
	m_screen.gotoXY(LEVEL_X + 18 - to_string(m_level).size(), LEVEL_Y);
	m_screen.printString(to_string(m_level));
	m_screen.refresh();
    // [Replace this with code to display the status.]
}

bool Game::playOneLevel()
{
	m_rowsLeft = 5 * m_level;
	m_well.clearBricks(m_screen);
	displayStatus();
	displayPrompt("Press the Enter key to begin playing Chetyris!");
	int T = (1000 - (100 * (m_level - 1)) > 100) ? 1000 - (100 * (m_level - 1)) : 100;
	PieceType nextPieceType = NUM_PIECE_TYPES; //something that doesn't affect us
	Timer timer;
	for (;;)
	{
		if (!createPiece(PIECE_INITIAL_X, PIECE_INITIAL_Y, nextPieceType))
		{
			m_well.display(m_screen, WELL_X, WELL_Y);
			break;
		}
		nextPieceType = nextPiece(NEXT_PIECE_X, NEXT_PIECE_Y, NEXT_PIECE_TITLE_X, NEXT_PIECE_TITLE_Y, chooseRandomPieceType());
		
		m_well.display(m_screen, WELL_X, WELL_Y);
		for (;;)
		{

			timer.start();
			char ch;
			while (timer.elapsed() <= T) 
			{
				if (getCharIfAny(ch))
				{
					discardPendingKeys();
					if (ch == 'q' || ch == 'Q')
						return false;
					if (ch == 32)
					{
						drop();
						break;
					}
					bool shiftSuccessful = m_piece->shift(ch);
					if (!shiftSuccessful && ch == ARROW_DOWN)
						break;
					else if (!shiftSuccessful)
						continue;
					if (ch == ARROW_DOWN) timer.start();
					m_well.display(m_screen, WELL_X, WELL_Y);
				}
			}
			if(!m_piece->shift(ARROW_DOWN))
				break;
			m_well.display(m_screen, WELL_X, WELL_Y);
		}
		m_piece->brickify();
		m_piece->special();
		m_well.display(m_screen, WELL_X, WELL_Y);
		int vaporizedCount = m_well.vaporize(m_screen);
		m_score += static_cast<int>(pow(2, vaporizedCount - 1)) * 100;
		m_rowsLeft = ((m_rowsLeft - vaporizedCount) > 0) ? (m_rowsLeft - vaporizedCount) : 0;
		m_well.display(m_screen, WELL_X, WELL_Y);
		displayStatus();
		if (m_rowsLeft == 0) 
			return true;
	}
    return false;  // [Replace this with the code to play a level.]
}

bool Game::createPiece(int x, int y, PieceType pieceType)
{
	delete m_piece;
	if(pieceType == NUM_PIECE_TYPES)
		newPiece(x + 1, y, m_piece, chooseRandomPieceType());
	else
		newPiece(x + 1, y, m_piece, pieceType);
	if (m_piece->checkOverlap())
	{

		m_piece->updateWell();
		return false;
	}
	m_piece->updateWell();
	return true;
}

PieceType Game::nextPiece(int posX, int posY, int titleX, int titleY, PieceType pieceType)
{
	if (m_nextpiece != nullptr)
	{
		m_nextpiece->clear(m_screen);
		delete m_nextpiece;
	}
	PieceType temp = newPiece(posX, posY, m_nextpiece, pieceType);
	m_screen.gotoXY(titleX, titleY);
	m_screen.printStringClearLine("Next Piece:");
	m_nextpiece->display(m_screen);
	return temp;

}

PieceType Game::newPiece(int x, int y, Piece* &piece, PieceType pieceType)
{
	switch (pieceType)
	{
	case PIECE_I:
		piece = new I(x, y, &m_well);
		return PIECE_I;
	case PIECE_L:
		piece = new L(x, y, &m_well);
		return PIECE_L;
	case PIECE_J:
		piece = new J(x, y, &m_well);
		return PIECE_J;
	case PIECE_T:
		piece = new T(x, y, &m_well);
		return PIECE_T;
	case PIECE_O:
		piece = new O(x, y, &m_well);
		return PIECE_O;
	case PIECE_S:
		piece = new S(x, y, &m_well);
		return PIECE_S;
	case PIECE_Z:
		piece = new Z(x, y, &m_well);
		return PIECE_Z;
	case PIECE_CRAZY:
		piece = new Crazy(x, y, &m_well);
		return PIECE_CRAZY;
	case PIECE_VAPOR:
		piece = new Vapor(x, y, &m_well);
		return PIECE_VAPOR;
	case PIECE_FOAM:
		piece = new Foam(x, y, &m_well);
		return PIECE_FOAM;
	default:
		piece = new Foam(x, y, &m_well);
		return PIECE_FOAM;
	}
}

void Game::drop()
{
	while (m_piece->shift(ARROW_DOWN)) {}
	m_piece->updateWell();
	m_well.display(m_screen, WELL_X, WELL_Y);
}

