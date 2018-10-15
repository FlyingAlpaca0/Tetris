#ifndef PIECE_INCLUDED
#define PIECE_INCLUDED
#include <vector>

enum PieceType {
    PIECE_I, PIECE_L, PIECE_J, PIECE_T, PIECE_O, PIECE_S, PIECE_Z,
    PIECE_VAPOR, PIECE_FOAM, PIECE_CRAZY, NUM_PIECE_TYPES
};

PieceType chooseRandomPieceType();
class Screen;
class Well;

class Piece
{
public:
	Piece(int x, int y, Well* WellPtr);
	virtual ~Piece() {}
	bool checkOverlap() const;
	bool virtual shift(char ch);
	void brickify() const;
	void display(Screen& screen) const;
	void clear(Screen& screen) const;
	void updateWell() const;
	virtual void special() = 0;

private:
	int m_orientation;
protected:
	int m_x;
	int m_y;
	Well* m_WellPtr;
	bool box[4][4];
	void move(char arrow);
	void rotate(int steps);
	virtual void setOrientation(int orientation) = 0;

};

class I: public Piece
{
public:
	I(int x, int y, Well* WellPtr);
	virtual void setOrientation(int orientation);
	virtual ~I() {}
	virtual void special() {}
};

class L : public Piece
{
public:
	L(int x, int y, Well* WellPtr);
	virtual void setOrientation(int orientation);
	virtual ~L() {}
	virtual void special() {}

};

class J : public Piece
{
public:
	J(int x, int y, Well* WellPtr);
	virtual void setOrientation(int orientation);
	virtual ~J() {}
	virtual void special() {}

};

class T : public Piece
{
public:
	T(int x, int y, Well* WellPtr);
	virtual void setOrientation(int orientation);
	virtual ~T() {}
	virtual void special() {}

};

class O : public Piece
{
public:
	O(int x, int y, Well* WellPtr);
	virtual void setOrientation(int orientation);
	virtual ~O() {}
	virtual void special() {}

};

class S : public Piece
{
public:
	S(int x, int y, Well* WellPtr);
	virtual void setOrientation(int orientation);
	virtual ~S() {}
	virtual void special() {}

};

class Z : public Piece
{
public:
	Z(int x, int y, Well* WellPtr);
	virtual void setOrientation(int orientation);
	virtual ~Z() {}
	virtual void special() {}

};

class Crazy : public Piece
{
public:
	Crazy(int x, int y, Well* WellPtr);
	virtual void setOrientation(int orientation);
	virtual bool shift(char ch);
	virtual ~Crazy() {}
	virtual void special() {}

};

class Vapor : public Piece
{
public:
	Vapor(int x, int y, Well* WellPtr);
	virtual void setOrientation(int orientation);
	virtual void special();
	virtual ~Vapor() {}
};

class Foam : public Piece
{
public:
	Foam(int x, int y, Well* WellPtr);
	virtual void setOrientation(int orientation);
	virtual void special();
	void explode(int x, int y);
	virtual ~Foam() {}
};
#endif // PIECE_INCLUDED
