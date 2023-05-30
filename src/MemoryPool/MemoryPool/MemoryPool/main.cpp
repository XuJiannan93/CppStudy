#include "Alloctor.h"

int main(int _Argcm, char* argv[])
{
	char* data = new char[112];
	delete[] data;

	return 0;
}