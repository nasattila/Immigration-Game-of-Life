#pragma once

#include <stdlib.h>
#include <ctype.h>
#include <iostream>

#include "pixel.h"

using namespace std;

typedef class Board
{
private:
	int dimension;
	int colorOne;
	int colorTwo;
	Pixel** state;

	int relocatePosition(int) const;
public:
	Board() : dimension(25)
	{
		colorOne = 0x000000;

		state = new Pixel*[dimension];

		for (int i = 0; i < dimension; i = i + 1)
		{
			state[i] = new Pixel[dimension];

			for (int j = 0; j < dimension; j = j + 1)
			{
				state[i][j].setLocation(i, j);
			}
		}
		colorTwo = 0xFFFFFF;
	}
	Board(int dimension) : dimension(dimension)
	{
		if (dimension <= 0)
		{
			cerr << "the dimension is invalid; board cannot be allocated" << endl;
		}
		else
		{
			state = new Pixel*[dimension];

			for (int i = 0; i < dimension; i = i + 1)
			{
				state[i] = new Pixel[dimension];

				for (int j = 0; j < dimension; j = j + 1)
				{
					state[i][j].setLocation(i, j);
				}
			}

			colorOne = 0x000000;
			colorTwo = 0xFFFFFF;
		}
	}
	int getColorOne() const;
	int getColorTwo() const;
	int getDimension() const;
	void setColorOne(int);
	void setColorTwo(int);
	int getColor(int, int) const;
	int neighbors(int, int) const;
	int majorityColor() const;
	int majorityColor(int, int) const;
	void advance() const;
	void advance(int) const;
	void changePixel(int, int, int);
	bool gameOver() const;
	Pixel* Board::operator [](unsigned int position);
	const Pixel* Board::operator [](unsigned int position) const;
	void saveBoard(std::ostream&, int, int, unsigned int, unsigned int) const;
	void openBoard(std::istream&, int&, int&, unsigned int&, unsigned int&);
	void print() const;
	~Board()
	{
		for (int i = 0; i < dimension; i = i + 1)
		{
			delete[] state[i];
		}

		delete[] state;
	}
}
board;
