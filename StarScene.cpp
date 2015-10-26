#include "StarScene.h"


float angle[3] = {0.0, 0.0, 0.0};
static float texAlphas[4] = {0.0, 0.0, 0.0, 0.0};
static float partSpeed = 0.0;
unsigned int testpic;
unsigned int pics[4];
corona::Image *pmyimage;
LorenzParticle *ptl1 = NULL;
LorenzParticle *ptl2 = NULL;
LorenzParticle *ptl3 = NULL;
LogarithmicParticle *logPart = NULL;
unsigned int bgList = 0;

float particleStart[3] = {0.0, 0.0, -15.05};
float particleEnd[3] = {0.0, 0.0, 20.0};

GLUquadricObj *pSphere = NULL;

bool SceneInit()
{

	pmyimage = corona::OpenImage("data/acc-space1b.jpg");
	if(!pmyimage)
		return false;

	glGenTextures(1, &testpic);
	glBindTexture(GL_TEXTURE_2D, testpic);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pmyimage->getWidth(), pmyimage->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, pmyimage->getPixels());


	ptl1 = new LorenzParticle("data/t1.jpg");
	if(!ptl1)
		return false;
	ptl1->setSize(300);
	ptl1->setDepth(2.0);
	ptl1->setArea(4);
	if(!ptl1->InitializeParticles())
		return false;

	ptl2 = new LorenzParticle("data/Galaxy-256.jpg");
	if(!ptl2)
		return false;
	ptl2->setSize(300);
	ptl2->setDepth(7.5);
	ptl2->setArea(3);
	if(!ptl2->InitializeParticles())
		return false;

	ptl3 = new LorenzParticle("data/star6-128.jpg");
	if(!ptl3)
		return false;
	ptl3->setSize(300);
	ptl3->setDepth(6.0);
	ptl3->setArea(2);
	if(!ptl3->InitializeParticles())
		return false;

	logPart = new LogarithmicParticle("data/nebula.jpg");
	if(!logPart)
		return false;
	logPart->setSize(170);
	logPart->setDepth(-1.0);
	logPart->setAParam(0.5);
	logPart->setBParam(3.0);
	if(!logPart->InitializeParticles())
		return false;


	bgList = glGenLists(1);
	glNewList(bgList, GL_COMPILE);

	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(0.0, 0.0);	glVertex3f(-512.0,512.0,0.0);
	glTexCoord2f(1.0, 0.0);	glVertex3f(-512.0,-512.0,0.0);
	glTexCoord2f(1.0, 1.0);	glVertex3f(512.0,-512.0,0.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(512.0,512.0,0.0);
	glEnd();

	glEndList();

	pSphere = gluNewQuadric();
	gluQuadricTexture(pSphere, GL_TRUE);


	return true;
}


void drawWhiteOut(float time)
{
	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_TEXTURE_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDisable(GL_TEXTURE_2D);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0,800,0,600);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();


	glPushMatrix();
	glTranslatef(0.0,0.0,-0.4);

	glBegin(GL_TRIANGLE_FAN);
	glColor4f(0.0,0.0,0.0,time);
	glVertex3f(-800.0,700.0,0.0);
	glVertex3f(-800.0,-700.0,0.0);
	glVertex3f(800.0,-700.0,0.0);
	glVertex3f(800.0,700.0,0.0);
	glEnd();
	glPopMatrix();

	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);	
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glPopAttrib();
}



int DrawStarField(float t)
{
	float seconds = fabs((t-tStart))*1000.0;		// convert the milliseconds to seconds
	static float rotateAng = 0.0;

	if((seconds <= 136500.001))
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

		/*************************************
		Draw the background
		**************************************/

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		if(seconds >= 24000.0001)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, testpic);

			glPushMatrix();
			glTranslatef(0.0, 0.0, 3.0);
			glRotatef(angle[0], 1.0, 1.0, 1.0);
			glColor4f(1.0, 1.0, 1.0, texAlphas[0]);
			gluSphere(pSphere, 10.0, 30, 30);
			glPopMatrix();

			texAlphas[0] += 0.001*(seconds)/13000.0;	
		}

		if(seconds >= 56000.0001)
		{
			if(seconds < 56900.0001)
			{
				drawWhiteOut(texAlphas[1]);
			}

			glPushMatrix();	
			glTranslatef(0.0,0.0,0.1);
			glRotatef(angle[2], 1.0, 1.0, 1.0);
			glColor4f(1.0,1.0,1.0,texAlphas[1]);
			gluSphere(pSphere, 10.0, 30, 30);
			glPopMatrix();

			glPushMatrix();	
			glTranslatef(0.0, 0.0, 2.0);
			glRotatef(angle[1], 1.0, 1.0, 1.0);
			glColor4f(1.0,1.0,1.0,texAlphas[2]);
			gluSphere(pSphere, 10.0, 30, 30);
			glPopMatrix();

			glPushMatrix();	
			glTranslatef(0.0, 0.0, 2.0);
			glRotatef(angle[1]/3.0, 1.0, 1.0, 1.0);
			glColor4f(1.0,1.0,1.0,texAlphas[2]);
			gluSphere(pSphere, 10.0, 30, 30);
			glPopMatrix();

			texAlphas[2] += 0.001*(seconds)/13000.0;
			texAlphas[1] += 0.001*(seconds)/13000.0;
		}

		/************************************* 
		Draw the rest of the stars
		**************************************/
		
		if((seconds >= 24000.0001) && (seconds < 109005.0001))
		{
			glPushMatrix();
			glTranslatef(particleStart[0], particleStart[1], particleStart[2]);
			glRotatef(-angle[0], 1.0, 1.0, 1.0);
			logPart->Draw(t);	/* intro particles */
			glPopMatrix();

			if(seconds > 82000.001)
			{
				interpolatePos(particleStart, particleEnd, ((t*1000.0)/9000.00));
			}
		}

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);

		if(seconds >= 75005.0001)
		{
			glPushMatrix();
			glTranslatef(0.0,0.0,6.0);

			if(seconds >= 85000.0001)
			{
				glRotatef(rotateAng, 0.0, 0.0, 1.0);
			
				if(rotateAng < 120.0)
				{
					rotateAng += (120.0*(t*1000.00))/20000.00;
				}
			}

			if(seconds >= 110005.001)
			{
				glPushMatrix();
				ptl1->Draw(t/8.0);
				glPopMatrix();	

				glPushMatrix();
				ptl2->Draw(t/8.0);
				glPopMatrix();

				glPushMatrix();
				ptl3->Draw(t/8.0);
				glPopMatrix();
			}
			else
			{
				glPushMatrix();
				ptl1->Draw(t/40.0);
				glPopMatrix();	

				glPushMatrix();
				ptl2->Draw(t/40.0);
				glPopMatrix();

				glPushMatrix();
				ptl3->Draw(t/40.0);
				glPopMatrix();
			}

			glPopMatrix();
		}

		// rotations for the bacground textures
		angle[0] += (360.0*(t*1000))/(95000);
		if((seconds < 109005.001))
		{
			angle[1] += (360.0*(t*1000))/(83000);
		}
		else
		{
			angle[1] += (360.0*(t*1000))/(20000);
		}
		angle[2] += (360.0*(t*1000))/(87000);

		if(texAlphas[0] >= 0.68)
			texAlphas[0] = 0.68;
		if(texAlphas[1] >= 0.38)
			texAlphas[1] = 0.38;
		if(texAlphas[2] >= 0.60)
			texAlphas[2] = 0.60;
		if(texAlphas[3] >= 0.85)
			texAlphas[3] = 0.85;
	}
	
	return 1;
}

