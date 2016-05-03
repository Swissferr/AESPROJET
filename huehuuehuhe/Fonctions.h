#pragma once
#include <string>

using namespace std;

void creerBlocs(string); //Fonction qui crée un vecteur de blocs de 4X4

string convHex(unsigned char c);

void cipher(); //Application de l'algorithme.
void inv_cipher(); //Application de l'algorithme inverse.

void subKey();

void print(int etape, int round);

