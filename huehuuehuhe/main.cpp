#include <string>
#include <iostream>
#include <vector>
#include "bloc.h"
#include "Fonctions.h"

using namespace std;

int main() {

	string texte = "";


	cout << "entrez un mot criss de cul : ";
	cin >> texte;

	creerBlocs(texte);

	cout << "\n";
	system("PAUSE");
	return 0;
}