#pragma once


class Bloc
{
public:
	Bloc();
	Bloc(char c[16]);
	~Bloc();

	char m[4][4]; //tableau représentant le bloc de 16 bytes.
	//Toutes les opérations seront éxécutée sur ce tableau.
};