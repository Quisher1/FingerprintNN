#include <iostream>
#include "BMP.h"


int main()
{
	
	BMP bmp(10000, 20000, ColorPalette::RED, "default", BMP16);

	bmp.save();



	return 0;
}