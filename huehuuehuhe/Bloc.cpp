#include <string>
#include "Bloc.h"

using namespace std;

Bloc::Bloc()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; i < 4; i++)
		{
			m[i][j] = '\0';
		}
	}
}

Bloc::Bloc(char c[16])
{
	for (int i = 0; i < 16; i++)
	{
		m[0][i] = c[i];
	}
}

Bloc::~Bloc()
{
	//... lol xD ...
}