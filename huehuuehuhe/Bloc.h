#pragma once


class Bloc
{
public:
	Bloc();
	Bloc(char c[16]);
	~Bloc();

	char m[4][4]; //tableau repr�sentant le bloc de 16 bytes.
	//Toutes les op�rations seront �x�cut�e sur ce tableau.
};