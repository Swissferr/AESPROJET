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
	bytes.push_back('\0');

	nbrBlocs = int(bytes.size() / 16) + 1;

	do {
		nbrBlocs--;

		for (int i = 0; i < 16; i++) {
			if (bytes[0] != NULL) {
				temp[i] = bytes[0];
				bytes.erase(bytes.begin());
			}
			else {
				temp[i] = 0x00;
				bytes.erase(bytes.begin());
			}
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
	string texte = "";

	cout << "entrez un mot criss de cul : ";
	getline(cin, texte);

	creerBlocs(texte);
	subKey();

	blocs[0]->print(0, 0);

	blocs[0]->subBytes();
	blocs[0]->print(0, 1);
	blocs[0]->shiftRows();
	blocs[0]->print(0, 2);
	blocs[0]->print(0, 3);

	/*blocs[0]->invSubBytes();
	blocs[0]->print(0, 5);
	blocs[0]->invShiftRows();
	blocs[0]->print(0, 6);*/

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