#include "Fonctions.h"
#include "Bloc.h"
#include <vector>
#include <iostream>

using namespace std;

vector<Bloc*> blocs;

void creerBlocs(string texte) {
	int nbrBlocs = 0;
	unsigned char test[16];
	bool flag = true;

	vector<char> bytes(texte.begin(), texte.end());
	bytes.push_back('\0');

	nbrBlocs = int(bytes.size() / 16) + 1;

	do {
		nbrBlocs--;

		for (int i = 0; i < 16; i++) {
			if (bytes[0] != NULL) {
				test[i] = bytes[0];
				bytes.erase(bytes.begin());
			}
			else {
				test[i] = '\0';
			}
		}

		blocs.push_back(new Bloc(test));

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

		for (int i = 0; i < mod.size(); i++)
		{
			switch (mod[i])
			{
			case 10:
				mod[i] = 'a'; break;
			case 11:
				mod[i] = 'b'; break;
			case 12:
				mod[i] = 'c'; break;
			case 13:
				mod[i] = 'd'; break;
			case 14:
				mod[i] = 'e'; break;
			case 15:
				mod[i] = 'f'; break;
			default:
				break;
			}//fin du switch
		}

		for (int i = mod.size() - 1; i >= 0; i--)
		{
			out += mod[i];
		}
	}
	else
		out = "00";

	return out;
	
}

void cipher()
{
	string texte = "";
	Bloc temp;

	cout << "entrez un mot criss de cul : ";
	cin >> texte;

	creerBlocs(texte);

	temp = *blocs[0];

	temp.subBytes();

}