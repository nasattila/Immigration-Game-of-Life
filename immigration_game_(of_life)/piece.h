#pragma once

class Piece
{
protected:
	int color;
public:
	Piece() : color(-1) {}
	Piece(int color) : color(color) {}
	int getColor() const
	{
		return color;
	}
	virtual void setColor(int) = 0;
};
