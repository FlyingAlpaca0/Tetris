#ifndef GAME_INCLUDED
#define GAME_INCLUDED

#include "Well.h"
#include "UserInterface.h"
#include <string>
// [Add other #include directives as necessary.]

class Game
{
  public:
    Game(int width, int height);
	~Game();
    void play();
    bool playOneLevel();
    void displayPrompt(std::string s);
    void displayStatus();

	bool createPiece(int x, int y, PieceType pieceType);
	PieceType nextPiece(int posX, int posY, int titleX, int titleY, PieceType pieceType);
	
	void drop();

    // [Add other members as necessary.]

  private:
    Screen  m_screen;
    int     m_level;
	Well    m_well;
	int     m_score;
	int     m_rowsLeft;
	Piece*  m_piece;
	Piece*  m_nextpiece;
    // [Add other members as necessary.]

	PieceType newPiece(int x, int y, Piece* &piece, PieceType pieceType);
};

#endif // GAME_INCLUDED
