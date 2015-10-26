                                                                     
                                                                     
                                                                     
                                             
// GraphicsDemo2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Particles.h"

#define DIST 3.5

double tStart, tFinish, tFTime;	// the start time for the timer, finish time, and frame rate
__int64 startclock;  // we want a start time
double rate_inv;


bool done = false;
unsigned int testpic;
unsigned int pics[5];
corona::Image *pmyimage;
StaticParticle *ptl1 = NULL;
StaticParticle *ptl2 = NULL;
StaticParticle *ptl3 = NULL;
float lightpos[4] = {0.0, 6.0, 0.0, 0.0};
float lightspos[3] = {0.0,0.0,0.0};
float ambient[4] = {1.0,1.0,1.0,1.0};
bool forward = true;
int count = 0;
ifstream fd;
float colors[3][4] = { 1.0, 0.00, 0.00, 0.65, 
					   0.0, 1.00, 0.00, 0.65,
					   0.00, 0.00, 1.0, 0.65 };
float fogcolor[4] = { 0.2, 0.2, 0.2, 0.2};
float fogpos[4] = { 0.0, 0.0, 19.0 }; 
float angle[3] = {0.0, 0.0, 0.0};

BOOL initftime(void) {
  __int64 rate;

  // we need the accuracy
  if(!QueryPerformanceFrequency((LARGE_INTEGER*)&rate)) {
    return FALSE; // win errors
  }

  // usually the rate will be 1193180
  if(!rate) {
    return FALSE;
  }

  rate_inv=1.0/(double)rate;

  if(!QueryPerformanceCounter((LARGE_INTEGER*)&startclock)) {
    return FALSE; // win errors
  }

  return TRUE; // there is a clock
}

 
double getFTime(void) {
  // by dividing by its rate you get accurate seconds

  __int64 endclock;

  QueryPerformanceCounter((LARGE_INTEGER*)&endclock);

  return (double)(endclock-startclock)*rate_inv;

  // note: I recommend that you multiply but the inverse of a constant.
  // (speed reasons)
}


bool Initialize()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glEnable(GL_TEXTURE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightspos);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 115.0);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 75.0);
	glEnable(GL_LIGHT0);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);


	if(!initftime())
		return false;
	tStart = getFTime();

	pmyimage = corona::OpenImage("data/galaxybackground.png");

	// load the test picture
	glGenTextures(1, &testpic);
	glBindTexture(GL_TEXTURE_2D, testpic);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pmyimage->getWidth(), pmyimage->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, pmyimage->getPixels());

	ptl1 = new StaticParticle("data/t1.png");
	if(!ptl1)
		return false;
	ptl1->setSize(125);
	ptl1->setDepth(20.0);
	ptl1->setArea(10);
	ptl1->InitializeParticles();

	ptl2 = new StaticParticle("data/starfield1-256.png");
	if(!ptl2)
		return false;
	ptl2->setSize(125);
	ptl2->setDepth(15.0);
	ptl2->setArea(7);
	ptl2->InitializeParticles();

	ptl3 = new StaticParticle("data/star6-128.png");
	if(!ptl3)
		return false;
	ptl3->setSize(100);
	ptl3->setDepth(12.0);
	ptl3->setArea(4);
	ptl3->InitializeParticles();

	// fog settings
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogfv(GL_FOG_COLOR, fogcolor);
	glFogf(GL_FOG_DENSITY, 0.12);
	glHint(GL_FOG_HINT, GL_NICEST);
	glFogf(GL_FOG_START, 19.5);
	glFogf(GL_FOG_END, 60.0);

	return true;
}

void ReshapeWindow(int w, int h)
{
	if(h==0)
		h = 1;

	float ratio = 1.0* w / h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(45.0, ratio, 1.0, 1050.0);
}


// This scene is used for the final credits for all the people involved
// One side of the door will have the 3d model of the names and the other side
// will have the shadow of the name
void DoorwayScene(float tFTime)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(-lightpos[0]+(2*(0.30*tFTime)), lightpos[1]+4.0, -9.0, 0.0, 2.0, 0.0, 0.0, 1.0, 0.0);

	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindTexture(GL_TEXTURE_2D, testpic);
	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glColor4f(1.0,1.0,1.0,0.65);
	glTexCoord2f(0.0, 1.0); glVertex3f(0.001, 0.0, -2.5);
	glTexCoord2f(1.0, 1.0); glVertex3f(0.001, 0.0, 2.5);
	glTexCoord2f(1.0, 0.0); glVertex3f(0.001, 4.0, 2.5);
	glTexCoord2f(0.0, 0.0); glVertex3f(0.001, 4.0, -2.5);
	glEnd();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	
	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	
	if((lightpos[0] >= -0.001) && (lightpos[0] <= 0.001))
	{
      count++;
	  if(count > 2)
		  count = 0;
	}

	glColor4fv(colors[count]);
	glVertex3f(0.0,0.0,-2.5);
	glVertex3f(0.0,0.0, 2.5);
	glVertex3f(-2.40*(lightpos[0]), 0.0, 2.5);
	glVertex3f(-2.40*(lightpos[0]), 0.0, -2.5);
	glEnd();
	glPopMatrix();
	glDisable(GL_BLEND);
	

	glPushMatrix();
	glTranslatef(lightpos[0], lightpos[1], lightpos[2]);
	glColor3f(1.0, 1.0, 1.0);
	glutWireSphere(0.05, 10, 10);
	glLineWidth(0.2);
	glBegin(GL_LINES);
	glColor3f(1.0,1.0,1.0);
	glVertex3f(0.0,0.0,0.0);
	glVertex3f(-lightpos[0], lightpos[1], lightpos[2]);
	glEnd();
	glPopMatrix();


	if(forward)
	{
		lightpos[0] = lightpos[0]+(0.30*tFTime);
		lightpos[1] += sin(lightpos[0]/180.0);
		if(lightpos[0] > DIST)
		{
			forward = false;
		}
	}
	else
	{
		lightpos[0] = lightpos[0]-(0.30*tFTime);
		lightpos[1] -= sin(lightpos[0]/180.0);
		if(lightpos[0] < -DIST)
		{
			forward = true;
		}
	}
}


void DrawStarField(float t)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, -10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glDisable(GL_LIGHTING);	

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0,0.001);

	glEnable(GL_TEXTURE_2D);


	glEnable(GL_FOG);
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPushMatrix();
	glRotatef(-angle[1], 1.0, 1.0, 0.0);
	ptl1->Draw(t);
	glPopMatrix();	

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.1);
	glPushMatrix();
	glRotatef(-angle[0], 1.0, 1.0, 0.0);
	ptl2->Draw(t);
	glPopMatrix();

	glPushMatrix();
	glRotatef(-angle[0], 1.0, 1.0, 0.0);
	ptl3->Draw(t);
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);
	
	glDisable(GL_POLYGON_OFFSET_FILL);
	glDisable(GL_FOG);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	angle[0] += 0.4 * tFTime;
	angle[1] += 0.3 * tFTime;
    angle[1] += 0.2 * tFTime;	

	if(angle[0] > 360.0)
		angle[0] = 1.0;
	if(angle[1] > 360.0)
		angle[1] = 1.0;	
	if(angle[3] > 360.0)
		angle[3] = 1.0;	
}

// simple draw routine
void Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(done)
	{
		exit(1);	// shutdown code should go here
	}


	DrawStarField(tFTime);
	
	//DoorwayScene(tFTime);

	glutSwapBuffers();
}


void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 27: 
		done = true;
		break;
	default:
		break;	
	}
}

void specialKeys(int key, int x, int y)
{
	switch(key)
	{
	default:
		break;
	}

}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Funky Demo");
	
	int isFull;
	if((isFull = MessageBoxA(NULL, "Fullscreen?", "GraphicsDemo2", MB_YESNO)) == 6)
	{
		glutFullScreen();
	}

	glutDisplayFunc(Draw);
	glutIdleFunc(Draw);
	glutReshapeFunc(ReshapeWindow);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeys);

	if(!Initialize())
	{
		return -1;
	}

	do{
		tFinish = getFTime();
	}while(tFinish == tStart);

	tFTime = tFinish-tStart;
	tStart = tFinish;
	
//	FSOUND_Update();
	glutMainLoop();

	return 0;
}

