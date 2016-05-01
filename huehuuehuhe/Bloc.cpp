#include <string>
#include "Bloc.h"

using namespace std;

Bloc::Bloc()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; i < 4; i++)
		{
			state[i][j] = '\0';
		}
	}
}

Bloc::Bloc(unsigned char c[16])
{
	for (int i = 0; i < 16; i++)
	{
		state[0][i] = c[i];
	}
}

Bloc::~Bloc()
{
	//... lol xD ...
}

void Bloc::subBytes()
{
	unsigned int temp[2];
	string tempChar;

	for (int i = 0; i < 16; i++)
	{
		try
		{
			tempChar = convHex(state[0][i]);
			temp[0] = tempChar[0];
			temp[1] = tempChar[1];

			state[0][i] = s[temp[0]][temp[1]];
		}
		catch (exception e)
		{
			//lol ça a planté
		}	
	}
}

void Bloc::shiftRows()
{
	unsigned char temp;
	//La ligne 0 ne change pas.
	//La ligne 1 décale de 1 emplacement vers la gauche.
	temp = state[1][0];
	state[1][0] = state[1][1];
	state[1][1] = state[1][2];
	state[1][2] = state[1][3];
	state[1][3] = temp;
	//La ligne 2 décale de 2 emplacement vers la gauche.
	for (int i = 0; i < 2; i++)
	{
		temp = state[2][0];
		state[2][0] = state[2][1];
		state[2][1] = state[2][2];
		state[2][2] = state[2][3];
		state[2][3] = temp;
	}
	//La ligne 3 décale de 3 emplacement vers la gauche.
	for (int i = 0; i < 3; i++)
	{
		temp = state[3][0];
		state[3][0] = state[3][1];
		state[3][1] = state[3][2];
		state[3][2] = state[3][3];
		state[3][3] = temp;
	}
}

void Bloc::mixColumns()
{
	unsigned char temp[4];
	unsigned char result[4];

	for (int i = 0; i < 4; i++)
	{
		temp[0] = state[0][i];
		temp[1] = state[1][i];
		temp[2] = state[2][i];
		temp[3] = state[3][i];

		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				result[j] += a[j][k] * temp[k];
			}
		}

		state[0][i] = result[0];
		state[1][i] = result[1];
		state[2][i] = result[2];
		state[3][i] = result[3];
	}
}

void Bloc::addRoundKey()
{

}

//méthodes de décodage.
void Bloc::invSubBytes()
{
	unsigned int invtemp[2];
	string invtempChar;

	for (int i = 0; i < 16; i++)
	{
		try
		{
			invtempChar = convHex(state[0][i]);
			
			if (invtempChar.length() == 2)
			{
				invtemp[0] = invtempChar[0];
				invtemp[1] = invtempChar[1];
			}
			else if (invtempChar.length() == 1)
			{
				invtemp[0] = 0;
				invtemp[1] = invtempChar[0];
			}

			state[0][i] = inv_s[invtemp[0]][invtemp[1]];
		}
		catch (exception e)
		{
			//lol ça a planté
		}
	}
}

void Bloc::invShiftRows()
{
	unsigned char temp;
	//La ligne 0 ne change pas.
	//La ligne 1 décale de 1 emplacement vers la droite.
	temp = state[1][3];
	state[1][3] = state[1][2];
	state[1][2] = state[1][1];
	state[1][1] = state[1][0];
	state[1][0] = temp;
	//La ligne 2 décale de 2 emplacement vers la gauche.
	for (int i = 0; i < 2; i++)
	{
		temp = state[2][3];
		state[2][3] = state[2][2];
		state[2][2] = state[2][1];
		state[2][1] = state[2][0];
		state[2][0] = temp;
	}
	//La ligne 3 décale de 3 emplacement vers la gauche.
	for (int i = 0; i < 3; i++)
	{
		temp = state[3][3];
		state[3][3] = state[3][2];
		state[3][2] = state[3][1];
		state[3][1] = state[3][0];
		state[3][0] = temp;
	}
}

void Bloc::invMixColumns()
{
	unsigned char temp[4];
	unsigned char result[4];

	for (int i = 0; i < 4; i++)
	{
		temp[0] = state[0][i];
		temp[1] = state[1][i];
		temp[2] = state[2][i];
		temp[3] = state[3][i];

		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				result[j] += inv_a[j][k] * temp[k];
			}
		}

		state[0][i] = result[0];
		state[1][i] = result[1];
		state[2][i] = result[2];
		state[3][i] = result[3];
	}
}

void Bloc::invAddRoundKey()
{

}