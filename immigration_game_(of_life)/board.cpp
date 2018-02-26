#include <stdlib.h>
#include <ctype.h>
#include <iostream>

#include "board.h"
#include "pixel.h"

using namespace std;

int Board::relocatePosition(int position) const
{
	if (position < 0)
	{
		return position + dimension;
	}
	else if (position > dimension - 1)
	{
		return position - dimension;
	}
	else
	{
		return position;
	}
}

int Board::getColorOne() const
{
    return colorOne;
}

int Board::getColorTwo() const
{
    return colorTwo;
}

void Board::setColorOne(int color)
{
	for (int i = 0; i < dimension; i = i + 1)
	{
		for (int j = 0; j < dimension; j = j + 1)
		{
			if (state[i][j].getColor() == getColorOne())
			{
				state[i][j] = color;
			}
		}
	}

	colorOne = color;
}

void Board::setColorTwo(int color)
{
	for (int i = 0; i < dimension; i = i + 1)
	{
		for (int j = 0; j < dimension; j = j + 1)
		{
			if (state[i][j].getColor() == getColorTwo())
			{
				state[i][j] = color;
			}
		}
	}

	colorTwo = color;
}

int Board::getColor(int row, int column) const
{
    if(row < 0 || row > dimension - 1 || column < 0 || column > dimension - 1)
    {
		cerr << "invalid location; color cannot be retrieved" << endl;

		return -1;
    }
    else
    {
		return state[row][column].getColor();
    }
}

int Board::getDimension() const
{
	return dimension;
}
int Board::neighbors(int row, int column) const
{
	int neighbors = 0;
	int rowPlus = relocatePosition(row + 1);
	int rowMinus = relocatePosition(row - 1);
	int columnPlus = relocatePosition(column + 1);
	int columnMinus = relocatePosition(column - 1);

	if (state[relocatePosition(row - 1)][relocatePosition(column - 1)].getColor() != -1)
	{
		neighbors = neighbors + 1;
	}
	if (state[row][relocatePosition(column - 1)].getColor() != -1)
	{
		neighbors = neighbors + 1;
	}
	if (state[relocatePosition(row + 1)][relocatePosition(column - 1)].getColor() != -1)
	{
		neighbors = neighbors + 1;
	}
	if (state[relocatePosition(row + 1)][column].getColor() != -1)
	{
		neighbors = neighbors + 1;
	}
	if (state[relocatePosition(row + 1)][relocatePosition(column + 1)].getColor() != -1)
	{
		neighbors = neighbors + 1;
	}
	if (state[row][relocatePosition(column + 1)].getColor() != -1)
	{
		neighbors = neighbors + 1;
	}
	if (state[relocatePosition(row - 1)][relocatePosition(column + 1)].getColor() != -1)
	{
		neighbors = neighbors + 1;
	}
	if (state[relocatePosition(row - 1)][column].getColor() != -1)
	{
		neighbors = neighbors + 1;
	}

	return neighbors;
}

int Board::majorityColor() const
{
    int colorOneCount = 0;
    int colorTwoCount = 0;

    for(int i = 0; i < dimension; i = i + 1)
    {
        for(int j = 0; j < dimension; j = j + 1)
        {
            int color = state[i][j].getColor();

            if(color == colorOne)
            {
                colorOneCount = colorOneCount + 1;
            }
            else if(color == colorTwo)
            {
                colorTwoCount = colorTwoCount + 1;
            }
        }
    }

    if(colorOneCount > colorTwoCount)
    {
        return colorOne;
    }
    else if(colorOneCount < colorTwoCount)
    {
        return colorTwo;
    }
	else
	{
		return -1;
	}
}

int Board::majorityColor(int row, int column) const
{
	int colorOneCount = 0;
	int colorTwoCount = 0;
	int rowPlus = relocatePosition(row + 1);
	int rowMinus = relocatePosition(row - 1);
	int columnPlus = relocatePosition(column + 1);
	int columnMinus = relocatePosition(column - 1);

	if (state[rowMinus][columnMinus].getColor() != -1)
	{
		if (state[rowMinus][columnMinus].getColor() == colorOne)
		{
			colorOneCount = colorOneCount + 1;
		}
		else
		{
			colorTwoCount = colorTwoCount + 1;
		}
	}
	if (state[row][columnMinus].getColor() != -1)
	{
		if (state[row][columnMinus].getColor() == colorOne)
		{
			colorOneCount = colorOneCount + 1;
		}
		else
		{
			colorTwoCount = colorTwoCount + 1;
		}
	}
	if (state[rowPlus][columnMinus].getColor() != -1)
	{
		if (state[rowPlus][columnMinus].getColor() == colorOne)
		{
			colorOneCount = colorOneCount + 1;
		}
		else
		{
			colorTwoCount = colorTwoCount + 1;
		}
	}
	if (state[rowPlus][column].getColor() != -1)
	{
		if (state[rowPlus][column].getColor() == colorOne)
		{
			colorOneCount = colorOneCount + 1;
		}
		else
		{
			colorTwoCount = colorTwoCount + 1;
		}
	}
	if (state[rowPlus][columnPlus].getColor() != -1)
	{
		if (state[rowPlus][columnPlus].getColor() == colorOne)
		{
			colorOneCount = colorOneCount + 1;
		}
		else
		{
			colorTwoCount = colorTwoCount + 1;
		}
	}
	if (state[row][columnPlus].getColor() != -1)
	{
		if (state[row][columnPlus].getColor() == colorOne)
		{
			colorOneCount = colorOneCount + 1;
		}
		else
		{
			colorTwoCount = colorTwoCount + 1;
		}
	}
	if (state[rowMinus][columnPlus].getColor() != -1)
	{
		if (state[rowMinus][columnPlus].getColor() == colorOne)
		{
			colorOneCount = colorOneCount + 1;
		}
		else
		{
			colorTwoCount = colorTwoCount + 1;
		}
	}
	if (state[rowMinus][column].getColor() != -1)
	{
		if (state[rowMinus][column].getColor() == colorOne)
		{
			colorOneCount = colorOneCount + 1;
		}
		else
		{
			colorTwoCount = colorTwoCount + 1;
		}
	}

	if (colorOneCount > colorTwoCount)
	{
		return colorOne;
	}
	else if (colorOneCount < colorTwoCount)
	{
		return colorTwo;
	}
	else
	{
		return -1;
	}
}

void Board::advance() const
{
	int** newState = new int*[dimension];

	for (int i = 0; i < dimension; i = i + 1)
	{
		newState[i] = new int[dimension];

		for (int j = 0; j < dimension; j = j + 1)
		{
			if (neighbors(i, j) <= 1 || neighbors(i, j) >= 4)
			{
				newState[i][j] = -1;
			}
			else if (neighbors(i, j) == 3)
			{
				if (state[i][j].getColor() == -1)
				{
					newState[i][j] = majorityColor(i, j);
				}
				else
				{
					newState[i][j] = state[i][j].getColor();
				}
			}
			else
			{
				newState[i][j] = state[i][j].getColor();
			}
		}
	}

	for (int i = 0; i < dimension; i = i + 1)
	{
		for (int j = 0; j < dimension; j = j + 1)
		{
			state[i][j] = newState[i][j];
		}

		delete[] newState[i];
	}

	delete[] newState;
}

void Board::advance(int times) const
{
	if (times > 0)
	{
		for (int i = 0; i < times; i = i + 1)
		{
			advance();
		}
	}
	else
	{
		cerr << "invalid repetitions; board cannot be advanced" << endl;
	}
}

void Board::changePixel(int row, int column, int color)
{
	if (row < 0 || row > dimension - 1 || column < 0 || column > dimension - 1)
	{
		cerr << "invalid location; pixel color cannot be changed" << endl;
	}
	else if (color != colorOne && color != colorTwo && color != -1)
	{
		cerr << "invalid color; pixel color cannot be changed" << endl;
	}
	else
	{
		state[row][column].setColor(color);
	}
}

bool Board::gameOver() const
{
	int majority = -2;

	for (int i = 0; i < dimension; i = i + 1)
	{
		for (int j = 0; j < dimension; j = j + 1)
		{
			if (state[i][j].getColor() != -1)
			{
				if (majority == -2)
				{
					majority = state[i][j].getColor();
				}
				else
				{
					if (majority != state[i][j].getColor())
					{
						return false;
					}
				}
			}
		}
	}

	return true;
}

Pixel* Board::operator [](unsigned int position)
{
	if (position < (unsigned int)0 && position >= (unsigned int)dimension)
	{
		cerr << "index out of bounds" << endl;
	}

	return state[position];
}

const Pixel* Board::operator [](unsigned int position) const
{
	if (position < (unsigned int)0 && position >= (unsigned int)dimension)
	{
		cerr << "index out of bounds" << endl;
	}
	
	return state[position];
}

void Board::saveBoard(std::ostream& os, int background, int turn, unsigned int pOnePieces, unsigned int pTwoPieces) const
{
	os << dimension << ';' << colorOne << ';' << colorTwo << ';' << background << ';' << turn << ";" << pOnePieces << ";" << pTwoPieces << ";";

	for (int i = 0; i < dimension; i = i + 1)
	{
		for (int j = 0; j < dimension; j = j + 1)
		{
			os << getColor(i, j) << ';';
		}
	}

	os << endl;
}

void Board::openBoard(std::istream& is, int& background, int& turn, unsigned int& pOnePieces, unsigned int& pTwoPieces)
{
	int dimension;
	int colorOne;
	int colorTwo;
	int newBackground;
	int newTurn;
	unsigned int newPOnePieces;
	unsigned int newPTwoPieces;
	int** state;
	char next;

	is >> dimension;
	is >> next;

	if (next != ';')
	{
		is.clear(ios::failbit);
	}

	is >> colorOne;
	is >> next;

	if (next != ';')
	{
		is.clear(ios::failbit);
	}

	is >> colorTwo;
	is >> next;

	if (next != ';')
	{
		is.clear(ios::failbit);
	}

	is >> newBackground;
	is >> next;

	if (next != ';')
	{
		is.clear(ios::failbit);
	}

	is >> newTurn;
	is >> next;

	if (next != ';')
	{
		is.clear(ios::failbit);
	}

	is >> newPOnePieces;
	is >> next;

	if (next != ';')
	{
		is.clear(ios::failbit);
	}

	is >> newPTwoPieces;
	is >> next;

	state = new int*[dimension];

	for (int i = 0; i < dimension; i = i + 1)
	{
		state[i] = new int[dimension];

		for (int j = 0; j < dimension; j = j + 1)
		{
			if (next != ';')
			{
				is.clear(ios::failbit);
			}

			is >> state[i][j];
			is >> next;
		}
	}

	if (is)
	{
		this->dimension = dimension;
		this->colorOne = colorOne;
		this->colorTwo = colorTwo;

		for (int i = 0; i < this->dimension; i = i + 1)
		{
			for (int j = 0; j < this->dimension; j = j + 1)
			{
				this->state[i][j] = state[i][j];
			}
		}

		background = newBackground;
		turn = newTurn;
		pOnePieces = newPOnePieces;
		pTwoPieces = newPTwoPieces;
	}
	else
	{
		cerr << "Error in input format." << endl;
	}
}

void Board::print() const
{
	for (int i = 0; i < dimension; i = i + 1)
	{
		for (int j = 0; j < dimension; j = j + 1)
		{
			if (state[i][j].getColor() == -1)
			{
				cout << "0 ";
			}
			else if(state[i][j].getColor() == colorOne)
			{
				cout << "1 ";
			}
			else
			{
				cout << "2 ";
			}
		}

		cout << endl;
	}
}
