#include <string>
#include "Bloc.h"

using namespace std;

//Les commentaires sur les m�thodes sont dans le fichier Bloc.h
//Certaines sp�cifications seront tout de m�me directement dans le code des m�thodes.

//constructeur par d�faut, instantie un bloc vide.
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

//Constructeur surcharg�, re�oit un tableau de charact�res non-sign�s pour se remplir.
Bloc::Bloc(unsigned char c[16])
{
	for (int i = 0; i < 16; i++)
	{
		state[0][i] = c[i];
	}
}

//destructeur
Bloc::~Bloc()
{
	//... lol xD ...
}

//�tape de substitution des bytes � l'aide de la s_box.
void Bloc::subBytes()
{
	unsigned int temp[2]; //valeur temporaire servant d'index pour la s_box
	string tempChar; //valeur temporaire du charact�re.

	for (int i = 0; i < 16; i++)
	{
		try
		{
			tempChar = convHex(state[0][i]); //Convertion du charact�re en valeur h�xad�cimale.
			
			//v�rification servant � s'assurer qu'un byte trop petit de fausse pas le r�sultat.
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

			//Le nouveau byte est trouv� dans la s_box gr�ce aux deux chiffres dans la valeur h�xad�cimale du byte de d�part.
			state[0][i] = s[temp[0]][temp[1]];
		}
		catch (exception e)
		{
			//lol �a a plant�
		}	
	}
}

void Bloc::shiftRows()
{
	unsigned char temp;
	//La ligne 0 ne change pas.
	//La ligne 1 d�cale de 1 emplacement vers la gauche.
	temp = state[1][0];
	state[1][0] = state[1][1];
	state[1][1] = state[1][2];
	state[1][2] = state[1][3];
	state[1][3] = temp;
	//La ligne 2 d�cale de 2 emplacement vers la gauche.
	for (int i = 0; i < 2; i++)
	{
		temp = state[2][0];
		state[2][0] = state[2][1];
		state[2][1] = state[2][2];
		state[2][2] = state[2][3];
		state[2][3] = temp;
	}
	//La ligne 3 d�cale de 3 emplacement vers la gauche.
	for (int i = 0; i < 3; i++)
	{
		temp = state[3][0];
		state[3][0] = state[3][1];
		state[3][1] = state[3][2];
		state[3][2] = state[3][3];
		state[3][3] = temp;
	}
}

//Multiplication dans GF(2^8).
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

//La m�thode mixColumns.
//L'op�ration est en fait une multiplication matricielle.
//Cependant, � cause de la nature des bytes et du champ GF(2^8),
//on ne peut utiliser d'addition et multiplication r�guli�re:
//on utilise la multiplication dans GF(2^8) et l'op�ration Xor.
void Bloc::mixColumns()
{
	unsigned char temp[4];	//colonne temporaire.
	unsigned char result[4]; //r�sultat temporaire.

	for (int i = 0; i < 4; i++)
	{
		//assignation d'une colonne au tableau temp
		for (int j = 0; j < 4; j++)
		{
			temp[j] = state[j][i];
		}		

		//Calcul du produit matriciel
		/*
		|s'1| = |2	3	1	1| |s1|
		|s'2|   |1	2	3	1| |s2|
		|s'3|   |1	1	2	3| |s3|
		|s'4|   |3	1	1	2| |s4|
		*/
		//La calcul n'est pas effectu� � l'aide de boucle par souci de pr�cision.

		result[0] = gmul(temp[0], 0x02) ^ gmul(temp[1], 0x03) ^ temp[2] ^ temp[3];
		result[1] = temp[0] ^ gmul(temp[1], 0x02) ^ gmul(temp[2], 0x03) ^ temp[3];
		result[2] = temp[0] ^ temp[1] ^ gmul(temp[2], 0x02) ^ gmul(temp[3], 0x03);
		result[3] = gmul(temp[0], 0x03) ^ temp[1] ^ temp[2] ^ gmul(temp[3], 0x02);

		//R�assignation de la colonne dans le tableau principal.
		for (int j = 0; j < 4; j++)
		{
			state[j][i] = result[j];
		}		
	}
}

void Bloc::addRoundKey(Bloc* key)
{
	for (int i = 0; i < 16; i++)
	{
		state[0][i] = state[0][i] ^ key->state[0][i];
	}
}

//m�thodes de d�codage.
//Inverse de subBytes, fonctionne exactement de la m�me fa�on � 
//l'exception de l'utilisation de la table inv_s au lieu de s.
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
			//lol �a a plant�
		}
	}
}

void Bloc::invShiftRows()
{
	unsigned char temp;
	//La ligne 0 ne change pas.
	//La ligne 1 d�cale de 1 emplacement vers la droite.
	temp = state[1][3];
	state[1][3] = state[1][2];
	state[1][2] = state[1][1];
	state[1][1] = state[1][0];
	state[1][0] = temp;
	//La ligne 2 d�cale de 2 emplacement vers la gauche.
	for (int i = 0; i < 2; i++)
	{
		temp = state[2][3];
		state[2][3] = state[2][2];
		state[2][2] = state[2][1];
		state[2][1] = state[2][0];
		state[2][0] = temp;
	}
	//La ligne 3 d�cale de 3 emplacement vers la gauche.
	for (int i = 0; i < 3; i++)
	{
		temp = state[3][3];
		state[3][3] = state[3][2];
		state[3][2] = state[3][1];
		state[3][1] = state[3][0];
		state[3][0] = temp;
	}
}

//La m�thode inverse de mixColumns.
//L'op�ration est en fait une multiplication matricielle.
//Cependant, � cause de la nature des bytes et du champ GF(2^8),
//on ne peut utiliser d'addition et multiplication r�guli�re:
//on utilise la multiplication dans GF(2^8) et l'op�ration Xor.
//La matrice constante est diff�rente dans cette m�thode.
void Bloc::invMixColumns()
{
	unsigned char temp[4];	//colonne temporaire.
	unsigned char result[4]; //r�sultat temporaire.

	for (int i = 0; i < 4; i++)
	{
		//assignation d'une colonne au tableau temp.
		for (int j = 0; j < 4; j++)
		{
			temp[j] = state[j][i];
		}

		//Calcul du produit matriciel
		/*
		|s'1| = |0e	0b	0d	09| |s1|
		|s'2|   |09	0e	0b	0d| |s2|
		|s'3|   |0d	09	0e	0b| |s3|
		|s'4|   |0b	0d	09	0e| |s4|
		*/
		//La calcul n'est pas effectu� � l'aide de boucle par souci de pr�cision.

		result[0] = gmul(temp[0], 0x0e) ^ gmul(temp[1], 0x0b) ^ gmul(temp[2], 0x0d) ^ gmul(temp[3], 0x09);
		result[1] = gmul(temp[0], 0x09) ^ gmul(temp[1], 0x0e) ^ gmul(temp[2], 0x0b) ^ gmul(temp[3], 0x0d);
		result[2] = gmul(temp[0], 0x0d) ^ gmul(temp[1], 0x09) ^ gmul(temp[2], 0x0e) ^ gmul(temp[3], 0x0b);
		result[3] = gmul(temp[0], 0x0b) ^ gmul(temp[1], 0x0d) ^ gmul(temp[2], 0x09) ^ gmul(temp[3], 0x0e);

		//R�assignation de la colonne dans le tableau principal.
		for (int j = 0; j < 4; j++)
		{
			state[j][i] = result[j];
		}
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
				//lol �a a plant�
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

