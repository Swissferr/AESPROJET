#include <string>
#include "Bloc.h"

using namespace std;

Bloc::Bloc()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; i < 4; i++)
		{
			m[i][j] = '\0';
		}
	}
}

Bloc::Bloc(unsigned char c[16])
{
	for (int i = 0; i < 16; i++)
	{
		m[0][i] = c[i];
	}
}

Bloc::~Bloc()
{
	//... lol xD ...
}

void Bloc::subBytes()
{
	int temp[2];
	string tempChar;

	for (int i = 0; i < 16; i++)
	{
		try
		{
			tempChar = convHex(m[0][i]);
			temp[0] = tempChar[0];
			temp[1] = tempChar[1];

			m[0][i] = s[temp[0]][temp[1]];
		}
		catch (exception e)
		{
			//lol ça a planté
		}	
	}
}

void Bloc::shiftRows()
{

}

void Bloc::mixColumns()
{

}

void Bloc::addRoundKey()
{

}

void Bloc::invSubBytes()
{

}

void Bloc::invShiftRows()
{

}

void Bloc::invMixColumns()
{

}

void Bloc::invAddRoundKey()
{

}