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
			
			if (tempChar.length() == 2)
			{
				temp[0] = tempChar[0];
				temp[1] = tempChar[1];
			}
			else if (tempChar.length() == 1)
			{
				temp[0] = 0;
				temp[1] = tempChar[0];
			}

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

unsigned char Bloc::gmul(unsigned char a, unsigned char b) 
{
	unsigned char p = 0;
	unsigned char counter;
	unsigned char hi_bit_set;
	for (counter = 0; counter < 8; counter++) {
		if ((b & 1) == 1)
			p ^= a;
		hi_bit_set = (a & 0x80);
		a <<= 1;
		if (hi_bit_set == 0x80)
			a ^= 0x1b;
		b >>= 1;
	}
	return p;
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

		/*for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				result[j] ^= gmul(temp[k], a[j][k]);
			}
		}*/

		result[0] = gmul(temp[0], 0x02) ^ gmul(temp[1], 0x03) ^ temp[2] ^ temp[3];
		result[1] = temp[0] ^ gmul(temp[1], 0x02) ^ gmul(temp[2], 0x03) ^ temp[3];
		result[2] = temp[0] ^ temp[1] ^ gmul(temp[2], 0x02) ^ gmul(temp[3], 0x03);
		result[3] = gmul(temp[0], 0x03) ^ temp[1] ^ temp[2] ^ gmul(temp[3], 0x02);


		state[0][i] = result[0];
		state[1][i] = result[1];
		state[2][i] = result[2];
		state[3][i] = result[3];
	}

}

void Bloc::addRoundKey(Bloc* key)
{
	for (int i = 0; i < 16; i++)
	{
		state[0][i] = state[0][i] ^ key->state[0][i];
	}
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

		/*for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				result[j] ^= gmul(temp[k], inv_a[j][k]);
			}
		}*/

		result[0] = gmul(temp[0], 0x0e) ^ gmul(temp[1], 0x0b) ^ gmul(temp[2], 0x0d) ^ gmul(temp[3], 0x09);
		result[1] = gmul(temp[0], 0x09) ^ gmul(temp[1], 0x0e) ^ gmul(temp[2], 0x0b) ^ gmul(temp[3], 0x0d);
		result[2] = gmul(temp[0], 0x0d) ^ gmul(temp[1], 0x09) ^ gmul(temp[2], 0x0e) ^ gmul(temp[3], 0x0b);
		result[3] = gmul(temp[0], 0x0b) ^ gmul(temp[1], 0x0d) ^ gmul(temp[2], 0x09) ^ gmul(temp[3], 0x0e);

		state[0][i] = result[0];
		state[1][i] = result[1];
		state[2][i] = result[2];
		state[3][i] = result[3];
	}


}

string Bloc::subKey(int rconCtr)
{
	int temp[2];	//Valeurs temporaire pour la substitution.
	string tempChar;

	unsigned char col4PrevKey[4];
	unsigned char newKey[4][4];


	col4PrevKey[0] = state[3][1];//rotation dirrecte de la colone
	col4PrevKey[1] = state[3][2];
	col4PrevKey[2] = state[3][3];
	col4PrevKey[3] = state[3][0];


	for (int i = 0; i < 4; i++)	//subByte de la colone
	{
		try
		{
			tempChar = convHex(col4PrevKey[i]);

			if (tempChar.length() == 2)
			{
				temp[0] = tempChar[0];
				temp[1] = tempChar[1];
			}
			else if (tempChar.length() == 1)
			{
				temp[0] = 0;
				temp[1] = tempChar[0];
			}

			col4PrevKey[i] = s[temp[0]][temp[1]];
		}
			catch (exception e)
			{
				//lol ça a planté
			}
		}

	for (int i = 0; i < 4; i++)
	{
		for (int u = 0; u < 4; u++)
		{
			if (i == 0)
			{
				col4PrevKey[u] = col4PrevKey[u] ^ rcon[u][rconCtr - 1];
				col4PrevKey[u] = state[i][u] ^ col4PrevKey[u];
			}
			else
			{
				col4PrevKey[u] = state[i][u] ^ col4PrevKey[u];
			}
			newKey[i][u] = col4PrevKey[u];
		}
	}

	string newKeyBloc = "";

	for (int i = 0; i < 16; i++)
	{
		newKeyBloc += newKey[0][i];
	}

	return newKeyBloc;
}

