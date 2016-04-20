#include "Fonctions.h"
#include "Bloc.h"
#include <vector>

vector<Bloc*> blocs;

void creerBlocs(string texte) {
	int nbrBlocs = 0;
	char test[16];
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