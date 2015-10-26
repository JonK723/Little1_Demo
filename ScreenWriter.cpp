#include "ScreenWriter.h"



FontWriter3d *testFont[10];


bool WriterInit()// initialize the writer scene code
{
	for(int n = 0; n < 10; n++)
	{
		testFont[n] = new FontWriter3d("data/elf.ttf");//testFont[n] = new FontWriter("Arial Black");
		testFont[n]->setHeight(21);
		testFont[n]->setPos(0.0, 0.0, -30.0);
		testFont[n]->setDepth(0.2);
		testFont[n]->SetupFont();
	}


	testFont[1]->setRaster(0.0, 0.0);		// "A"
	testFont[1]->setPos(-1.0, 5.0, -30.0);

	testFont[2]->setRaster(0.0, 0.0);	// " small intro for your pleasure"
	testFont[2]->setPos(-1.0, 5.0, -30.0);

	testFont[3]->setRaster(0.0, 0.0);	// "by an old school Finnish scener"
	testFont[3]->setPos(-2.0, 1.0, -30.0);

	testFont[4]->setRaster(0.0, 0.0);   // And a new school American  _cener"
	testFont[4]->setPos(-2.0, -5.0, -30.0);

	testFont[5]->setRaster(0.0, 0.0);	// "S"
	testFont[5]->setPos(4.0, -5.0, -30.0);

	testFont[6]->setRaster(0.0, 0.0);	// "_is back and enjoy!!!"
	testFont[6]->setPos(2.0, 0.0, -30.0);



	return true;
}


// render the different fonts onto the screen
int DrawFontStrings(float t)
{ 
	float seconds = fabs((t-tStart))*1000.0;		// convert the milliseconds to seconds

	float tr1[3] = {-3.5, 1.0, -30.0};
	float tr2[3] = {0.0, 0.0, 0.0};

	if((seconds >= 3000.001) && (seconds < 8500.001))
	{
		testFont[1]->PrintBitmapString("A ");
		testFont[2]->PrintBitmapString("  small intro for your pleasure");
	}

	if((seconds >= 8500.001) && (seconds < 23000.001))
	{
		interpolatePos(testFont[1]->getPos(), tr1, 0.001*(seconds/2500));
		testFont[1]->PrintBitmapString("A ");
	}

	if((seconds >= 19000.001) && (seconds < 27000.001))
	{
		testFont[1]->PrintBitmapString("A ");
		testFont[3]->PrintBitmapString("by   n oldsckool Finnish scener");
	}

	if((seconds >= 24000.001) && (seconds < 30000.001))
	{
		testFont[4]->PrintBitmapString("And a 'new'sckool American   cener");
		testFont[5]->PrintBitmapString("S");
	}

	if((seconds >= 30000.001) && (seconds < 37000.001))
	{
		interpolatePos(testFont[5]->getPos(), testFont[6]->getPos(), 0.001*(seconds/2500));
		testFont[5]->PrintBitmapString("S");
	}

	if((seconds >= 34000.001) && (seconds < 40000.001))
	{
		testFont[5]->PrintBitmapString("S");
		testFont[6]->PrintBitmapString("  it back and enjoy!!!");
	}


	return 0;
}