#pragma once

#include <ctype.h>
#include <stdlib.h>
#include <iostream>

#include "piece.h"

using namespace std;

typedef class Pixel: public Piece
{
private:
	// location:
	int row;
	int column;
public:
	Pixel() : Piece(), row(0), column(0) {}
	Pixel(int row, int column, int color) : Piece(color), row(row), column(column)
	{
		if (color > 0xFFFFFF || color < -1)  // largest RGB hexadecimal number is FFFFFF
		{
			cerr << "invalid color; pixel color cannot be set" << endl;
		}

		if (row < 0 || column < 0)
		{
			cerr << "invalid location; pixel cannot be placed" << endl;
		}
		else
		{
			this->row = row;
			this->column = column;
			this->color = color;
		}
	}
	void setLocation(int, int);
	int getRow() const;
	int getColumn() const;
	virtual void setColor(int);
	Pixel& operator =(const int color);
}
pixel;
