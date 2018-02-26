#include <stdlib.h>
#include <ctype.h>
#include <iostream>

#include "pixel.h"

using namespace std;

void Pixel::setLocation(int row, int column)
{
	if (row < 0 || column < 0)
	{
		cerr << "invalid location; location cannot be set" << endl;
	}
	else
	{
		this->row = row;
		this->column = column;
	}
}

int Pixel::getRow() const
{
	return row;
}

int Pixel::getColumn() const
{
	return column;
}

void Pixel::setColor(int color)
{
	if (color > 0xFFFFFF || color < -1)  // largest RGB hexadecimal number (FFFFFF)
	{
		cerr << "invalid color; pixel color cannot be set" << endl;
	}
	else
	{
		this->color = color;
	}
}

Pixel& Pixel::operator =(const int color)
{
	if (color > 0xFFFFFF || color < -1)  // largest RGB hexadecimal number (FFFFFF)
	{
		cerr << "invalid color; pixel color cannot be set" << endl;
	}
	else
	{
		setColor(color);
	}

	return *this;
}