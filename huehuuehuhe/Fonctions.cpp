#include "Fonctions.h"
#include "Bloc.h"
#include <vector>
#include <iostream>

using namespace std;

vector<Bloc*> blocs;
vector<Bloc*> key;

void creerBlocs(string texte) {
	int nbrBlocs = 0;
	unsigned char temp[16];
	bool flag = true;

	vector<unsigned char> bytes(texte.begin(), texte.end());

	if (bytes.size() % 16 == 0)
		nbrBlocs = int(bytes.size() / 16);
	else
		nbrBlocs = int(bytes.size() / 16) + 1;

	do {
		nbrBlocs--;

		for (int i = 0; i < 16; i++) {
			if (bytes.size() != 0)
			{
				if (bytes[0] != NULL) {
					temp[i] = bytes[0];
					bytes.erase(bytes.begin());
				}
				else {
					temp[i] = 0x00;
					bytes.erase(bytes.begin());
				}
			}
			else
				temp[i] = 0x00;
		}

		blocs.push_back(new Bloc(temp));

		if (nbrBlocs == 0)
			flag = false;
	} while (flag);
}

string convHex(unsigned char c)
{
	string out = "";
	vector<unsigned char> mod;

	if (c != NULL)
	{
		while (c != 0)
		{
			mod.push_back(c % 16);
			c /= 16;
		}

		for (int i = mod.size() - 1; i >= 0; i--)
		{
			out += mod[i];
		}
	}
	else
	{
		out += '\0';
	}

	return out;	
}

void cipher()
{
	int round = 0;

	string texte = "";

	cout << "entrez un message: ";
	getline(cin, texte);

	creerBlocs(texte);
	subKey();

	print(0, round);

	//Round Input
	for (int i = 0; i < blocs.size(); i++)
	{
		blocs[i]->addRoundKey(key[round]);
	}
	print(4, round);
	round++;
	//Pour chaque bloc dans le message, la clé de départ est appliquée.
	//rounds 1 à 10
	for (round; round <= 10; round++)
	{
		for (int i = 0; i < blocs.size(); i++)
		{
			blocs[i]->subBytes();			
		}
		print(1, round);

		for (int i = 0; i < blocs.size(); i++)
		{
			blocs[i]->shiftRows();
		}
		print(2, round);

		if (round != 10)
		{
			for (int i = 0; i < blocs.size(); i++)
			{
				blocs[i]->mixColumns();
			}
			print(3, round);
		}

		for (int i = 0; i < blocs.size(); i++)
		{
			blocs[i]->addRoundKey(key[round]);
		}
		print(4, round);		
	}
	
	print(8, round);

}

void inv_cipher()
{
	int round = 10;

	print(0, round);

	//Round Input
	for (int i = 0; i < blocs.size(); i++)
	{
		blocs[i]->addRoundKey(key[round]);
	}
	print(4, round);
	round--;
	//Pour chaque bloc dans le message, la clé de départ est appliquée.
	//rounds 1 à 10
	for (round; round >= 0; round--)
	{
		for (int i = 0; i < blocs.size(); i++)
		{
			blocs[i]->invShiftRows();
		}
		print(6, round);

		for (int i = 0; i < blocs.size(); i++)
		{
			blocs[i]->invSubBytes();
		}
		print(5, round);		

		if (round != 0)
		{
			for (int i = 0; i < blocs.size(); i++)
			{
				blocs[i]->invMixColumns();
			}
			print(7, round);
		}

		for (int i = 0; i < blocs.size(); i++)
		{
			blocs[i]->addRoundKey(key[round]);
		}
		print(4, round);
	}

	print(8, round);

}

void print(int etape, int round)
{
	switch (etape)
	{
	case 0:
		cout << "Ronde " << round << ", Entree: " << endl; break;
	case 1:
		cout << "Ronde " << round << ", subBytes(): " << endl; break;
	case 2:
		cout << "Ronde " << round << ", shiftRows(): " << endl; break;
	case 3:
		cout << "Ronde " << round << ", mixColumns(): " << endl; break;
	case 4:
		cout << "Ronde " << round << ", addRoundKey(): " << endl; break;
	case 5:
		cout << "Ronde " << round << ", invSubBytes(): " << endl; break;
	case 6:
		cout << "Ronde " << round << ", invShiftRows(): " << endl; break;
	case 7:
		cout << "Ronde " << round << ", invMixColumns(): " << endl; break;
	case 8:
		cout << "Sortie: " << endl; break;
	}

	for (int i = 0; i < blocs.size(); i++)
	{	
		for (int j = 0; j < 16; j++)
		{
			cout << blocs[i]->state[0][j];
		}
		cout << endl;
	}
	
	cout << endl;
}
	


void subKey()
{
	string txt = "0123456789ABCDEF";

	unsigned char temp[16] =
	{
		0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
		0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
	};

	for (int i = 0; i <= 10; i++)
	{
		if (i != 0)
		{
			txt = key[i - 1]->subKey(i);

			vector<unsigned char> bytes(txt.begin(), txt.end());
			bytes.push_back('\0');

			for (int i = 0; i < 16; i++)
			{
				if (bytes[0] != NULL)
				{
					temp[i] = bytes[0];
					bytes.erase(bytes.begin());
				}
				else
				{
					temp[i] = 0x00;
					bytes.erase(bytes.begin());
				}
			}
		}
				
		key.push_back(new Bloc(temp));
	}
}