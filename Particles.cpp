#include "Particles.h"


void billboardCheatCylindricalBegin() {
	
	float modelview[16];

	memset(modelview, 0, sizeof(modelview));
	
	// save the current modelview matrix
	glPushMatrix();
	modelview[0] = modelview[5] = modelview[10] = modelview[15] = 1.0;

    glMultMatrixf(modelview);
}

void billboardEnd() {

	// restore the previously 
	// stored modelview matrix
	glPopMatrix();
}


BaseParticle::BaseParticle()
{
	maxParticles = 100;
	pList = NULL;
	totDead = 0;
	looping = true;
	paused = false;
	gravity = (float)9.8;	// the standard rate for gravity in m/s^2
	height = 1.0;
	image = NULL;
	texId = 0;
	fileName = NULL;
}

BaseParticle::~BaseParticle()
{
	if(pList)
		pList = NULL;
}

void BaseParticle::setSize(int sz)
{
	maxParticles = sz;
}

int BaseParticle::getSize()
{
	return maxParticles;
}

void BaseParticle::enableLooping()
{
	looping = true;
}

void BaseParticle::disableLooping()
{
	looping = false;
}

void BaseParticle::enablePaused()
{
	paused = true;
}

void BaseParticle::disablePaused()
{
	paused = false;
}


//*********************************************************************************************
// FOUNTAIN PARTICLE FUNCTIONS
FountainParticle::FountainParticle() : BaseParticle()
{
	radius = 1.0;
	gravity = (float)9.8;	// the standard rate for gravity in m/s^2
	height = 1.0;
}

FountainParticle::FountainParticle(const char* file) : BaseParticle()
{
	if(file != NULL)
	{
		fileName = new char[strlen(file)];
		strcpy(fileName, file);
	}
}

FountainParticle::~FountainParticle()
{
}

void FountainParticle::setGravity(float g)
{
	gravity = g;
}

float FountainParticle::getGravity()
{
	return gravity;
}

void FountainParticle::setRadius(float r)
{
	radius = r;
}

float FountainParticle::getRadius()
{
	return radius;
}

void FountainParticle::setHeight(float h)
{
	height = h;
}

float FountainParticle::getHeight()
{
	return height;
}

bool FountainParticle::InitializeParticles()
{
	if(!pList)	
		pList = new sParticle[maxParticles];

	if(fileName)
	{
		image = corona::OpenImage(fileName);
		if(image)
		{	
			glGenTextures(1, &texId);
			glBindTexture(GL_TEXTURE_2D, texId);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			glTexEnvf(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image->getWidth(), image->getHeight(), GL_UNSIGNED_BYTE, GL_RGB, image->getPixels());
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->getWidth(), image->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, image->getPixels());
		}
	}

	// draw the stars as a sphere
	for(int n = 0; n < maxParticles; n++)
	{
		float theta = (float)rand()/(RAND_MAX%360);
		
		pList[n].pos[0] = 0.0;
		pList[n].pos[1] = 0.0;
		pList[n].pos[2] = 0.0;	// particles start at 0

		pList[n].prevpos[0] = 0.0;
		pList[n].prevpos[1] = 0.0;
		pList[n].prevpos[2] = 0.0;

		pList[n].size = 0.05;
		pList[n].life = 1.0;
		pList[n].decay = (float)rand()/RAND_MAX;
		pList[n].deltaColor[0] = pList[n].deltaColor[1] = pList[n].deltaColor[2] = pList[n].deltaColor[3] = 0.0;  // not changing the color over time
		
		pList[n].color[0] = (float)rand()/RAND_MAX;
		pList[n].color[1] = (float)rand()/RAND_MAX;
		pList[n].color[2] = (float)rand()/RAND_MAX;
		pList[n].color[3] = pList[n].life;	// set the alpha parameter based on the life
		
		pList[n].prevcolor[0] = (float)rand()/RAND_MAX;
		pList[n].prevcolor[1] = (float)rand()/RAND_MAX;
		pList[n].prevcolor[2] = (float)rand()/RAND_MAX;
		pList[n].prevcolor[3] = 1.0;

		pList[n].direction[0] = radius * cos(theta);
		pList[n].direction[1] = 1.0;
		pList[n].direction[2] = radius * sin(theta);

		pList[n].velocity[0] = (float)rand()/(RAND_MAX+1.0);
		pList[n].velocity[1] = height;
		pList[n].velocity[2] = (float)rand()/(RAND_MAX+1.0);
	}

	return true;
}

bool FountainParticle::initParticle(int n)
{
	if(!pList)
		return false;

	float theta = (float)rand()/(RAND_MAX%360);

	pList[n].pos[0] = 0.0;
	pList[n].pos[1] = 0.0;
	pList[n].pos[2] = 0.0;	// particles start at 0

	pList[n].prevpos[0] = 0.0;
	pList[n].prevpos[1] = 0.0;
	pList[n].prevpos[2] = 0.0;

	pList[n].size = 0.05;
	pList[n].life = 1.0;
	pList[n].decay = (float)rand()/RAND_MAX;

	pList[n].deltaColor[0] = pList[n].deltaColor[1] = pList[n].deltaColor[2] = pList[n].deltaColor[3] = 0.0;  // not changing the color over time

/*
	pList[n].color[0] = (float)rand()/RAND_MAX;
	pList[n].color[1] = (float)rand()/RAND_MAX;
	pList[n].color[2] = (float)rand()/RAND_MAX;
	pList[n].color[3] = pList[n].life;
	pList[n].prevcolor[0] = (float)rand()/RAND_MAX;
	pList[n].prevcolor[1] = (float)rand()/RAND_MAX;
	pList[n].prevcolor[2] = (float)rand()/RAND_MAX;
	pList[n].prevcolor[3] = 1.0;
*/

	pList[n].color[0] = 1.0;
	pList[n].color[1] = 1.0;
	pList[n].color[2] = 1.0;
	pList[n].color[3] = pList[n].life;
	pList[n].prevcolor[0] = 1.0;
	pList[n].prevcolor[1] = 1.0;
	pList[n].prevcolor[2] = 1.0;
	pList[n].prevcolor[3] = 1.0;

	pList[n].direction[0] = radius * cos(theta);
	pList[n].direction[1] = 1.0;
	pList[n].direction[2] = radius * sin(theta);

	pList[n].velocity[0] = (float)rand()/(RAND_MAX+1.0);
	pList[n].velocity[1] = height;
	pList[n].velocity[2] = (float)rand()/(RAND_MAX+1.0);

	return true;
}

void FountainParticle::Update(float time)
{
	if(!paused)
	{
		if(!pList)
			return;

		for(int n = 0; n < maxParticles; n++)
		{
			pList[n].prevpos[0] = pList[n].pos[0];
			pList[n].prevpos[1] = pList[n].pos[1];
			pList[n].prevpos[2] = pList[n].pos[2];

			pList[n].velocity[0] += gravity*(time);
			pList[n].velocity[1] -= gravity*(time);
			pList[n].velocity[2] += gravity*(time);

			pList[n].pos[0] += pList[n].direction[0]*(pList[n].velocity[0]*time);
			pList[n].pos[1] += pList[n].direction[1]*(pList[n].velocity[1]*time);
			pList[n].pos[2] += pList[n].direction[2]*(pList[n].velocity[2]*time);


			pList[n].life -= pList[n].decay;
			pList[n].color[3] = pList[n].life;
			pList[n].prevcolor[3] = pList[n].life;
			
			if(pList[n].life <= 0.0)
			{
				if(looping)
				{
					initParticle(n);
				}
			}
		}
	}
}

void FountainParticle::Draw(float time)
{
	if(!pList)
		return;

	float mat[16];
	float a[3],b[3],c[3],d[3],right[3],up[3],pos[3];

	glGetFloatv(GL_MODELVIEW_MATRIX, mat);
	
	right[0] = mat[0];	
	right[1] = mat[4];
	right[2] = mat[8];	
	up[0] = mat[1];
	up[1] = mat[5];
	up[2] = mat[9];
	pos[0] = mat[12];
	pos[1] = mat[13];
	pos[2] = mat[14];


	if(image)
	{
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		billboardCheatCylindricalBegin();
		glBindTexture(GL_TEXTURE_2D, texId);
	}
	else
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBegin(GL_LINES);
	}
	for(int n = 0; n < maxParticles; n++)
	{
		a[0] = pList[n].pos[0] - (right[0] + up[0]) * pList[n].size;
		a[1] = pList[n].pos[1] - (right[1] + up[1]) * pList[n].size;
		a[2] = pList[n].pos[2] - (right[2] + up[2]) * pList[n].size;

		b[0] = pList[n].pos[0] + (right[0] - up[0]) * pList[n].size;
		b[1] = pList[n].pos[1] + (right[1] - up[1]) * pList[n].size;
		b[2] = pList[n].pos[2] + (right[2] - up[2]) * pList[n].size;

		c[0] = pList[n].pos[0] + (right[0] + up[0]) * pList[n].size;
		c[1] = pList[n].pos[1] + (right[1] + up[1]) * pList[n].size;
		c[2] = pList[n].pos[2] + (right[2] + up[2]) * pList[n].size;

		d[0] = pList[n].pos[0] - (right[0] - up[0]) * pList[n].size;
		d[1] = pList[n].pos[1] - (right[1] - up[1]) * pList[n].size;
		d[2] = pList[n].pos[2] - (right[2] - up[2]) * pList[n].size;

		if(image)
		{	
			glBegin(GL_TRIANGLE_FAN);
			glColor4f(pList[n].color[0],pList[n].color[1],pList[n].color[2],pList[n].life);
			glTexCoord2f(0.0, 0.0);	glVertex3f(b[0],b[1],b[2]);
			glTexCoord2f(1.0, 0.0);	glVertex3f(a[0],a[1],a[2]);
			glTexCoord2f(1.0, 1.0);	glVertex3f(d[0],d[1],d[2]);
			glTexCoord2f(0.0, 1.0); glVertex3f(c[0],c[1],c[2]);
			glEnd();
		}
		else
		{
			glColor4fv(pList[n].prevcolor);
			glVertex3f(pList[n].prevpos[0], pList[n].prevpos[1], pList[n].prevpos[2]);
			glColor4fv(pList[n].color);
			glVertex3f(pList[n].pos[0], pList[n].pos[1], pList[n].pos[2]);
			glEnd();
		}
	}

	if(image)
	{
		billboardEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
	}

	Update(time);

	glDisable(GL_BLEND);
}


//*********************************************************************************************
// Snow PARTICLE FUNCTIONS

SnowParticle::SnowParticle() : BaseParticle()
{
	height = 1;
	area = 1;
	gravity = (float)9.8;	// the standard rate for gravity in m/s^2
}

SnowParticle::~SnowParticle()
{
}

void SnowParticle::setGravity(float gv)
{
	gravity = gv;
}

float SnowParticle::getGravity()
{
	return gravity;
}

void SnowParticle::setHeight(float h)
{
	height = h;
}

float SnowParticle::getHeight()
{
	return height;
}

void SnowParticle::setArea(int a)
{
	area = a;
}

int SnowParticle::getArea()
{
	return area;
}


bool SnowParticle::InitializeParticles()
{
	if(!pList)	
		pList = new sParticle[maxParticles];


	for(int n = 0; n < maxParticles; n++)
	{
		float theta = rand()%360;
		
		pList[n].pos[0] = area * rand() / (RAND_MAX + 1.0);
		pList[n].pos[1] = height;
		pList[n].pos[2] = area * rand() / (RAND_MAX + 1.0);	// particles start at 0

		pList[n].prevpos[0] = pList[n].pos[0];
		pList[n].prevpos[1] = height;
		pList[n].prevpos[2] = pList[n].pos[2];

		pList[n].size = 0.05;
		pList[n].life = 1.0;
		pList[n].decay = (float)rand()/RAND_MAX;
		pList[n].deltaColor[0] = pList[n].deltaColor[1] = pList[n].deltaColor[2] = pList[n].deltaColor[3] = 0.0;  // not changing the color over time
		
		pList[n].color[0] = (float)rand()/RAND_MAX;
		pList[n].color[1] = (float)rand()/RAND_MAX;
		pList[n].color[2] = (float)rand()/RAND_MAX;
		pList[n].color[3] = pList[n].life;
		
		pList[n].prevcolor[0] = (float)rand()/RAND_MAX;
		pList[n].prevcolor[1] = (float)rand()/RAND_MAX;
		pList[n].prevcolor[2] = (float)rand()/RAND_MAX;
		pList[n].prevcolor[3] = 1.0;

		pList[n].direction[0] = 0.0;
		pList[n].direction[1] = 1.0;
		pList[n].direction[2] = 0.0;

		pList[n].velocity[0] = ((float)rand()/(RAND_MAX+1.0));
		pList[n].velocity[1] = -((float)rand()/(RAND_MAX+1.0));
		pList[n].velocity[2] = ((float)rand()/(RAND_MAX+1.0));
	}

	return true;
}


bool SnowParticle::initParticle(int n)
{
	if(!pList)
		return false;

	float theta = (float)rand()/(RAND_MAX%360);

	pList[n].pos[0] = area * rand() / (RAND_MAX + 1.0);
	pList[n].pos[1] = height;
	pList[n].pos[2] = area * rand() / (RAND_MAX + 1.0);	// particles start at 0

	pList[n].prevpos[0] = pList[n].pos[0];
	pList[n].prevpos[1] = height;
	pList[n].prevpos[2] = pList[n].pos[2];

	pList[n].life = 1.0;
	pList[n].decay = (float)rand()/RAND_MAX;

	pList[n].deltaColor[0] = pList[n].deltaColor[1] = pList[n].deltaColor[2] = pList[n].deltaColor[3] = 0.0;  // not changing the color over time
	
	pList[n].color[0] = (float)rand()/RAND_MAX;
	pList[n].color[1] = (float)rand()/RAND_MAX;
	pList[n].color[2] = (float)rand()/RAND_MAX;
	pList[n].color[3] = pList[n].life;
	pList[n].prevcolor[0] = (float)rand()/RAND_MAX;
	pList[n].prevcolor[1] = (float)rand()/RAND_MAX;
	pList[n].prevcolor[2] = (float)rand()/RAND_MAX;
	pList[n].prevcolor[3] = 1.0;

	pList[n].direction[0] = 0.0;
	pList[n].direction[1] = 1.0;
	pList[n].direction[2] = 0.0;

	pList[n].velocity[0] = ((float)rand()/(RAND_MAX+1.0));
	pList[n].velocity[1] = -((float)rand()/(RAND_MAX+1.0));
	pList[n].velocity[2] = ((float)rand()/(RAND_MAX+1.0));

	return true;
}


void SnowParticle::Update(float time)
{
	if(!paused)
	{
		if(!pList)
			return;

		for(int n = 0; n < maxParticles; n++)
		{
			pList[n].prevpos[0] = pList[n].pos[0];
			pList[n].prevpos[1] = pList[n].pos[1];
			pList[n].prevpos[2] = pList[n].pos[2];

			pList[n].pos[0] += pList[n].direction[0]*(pList[n].velocity[0]*time);
			pList[n].pos[1] += pList[n].direction[1]*(pList[n].velocity[1]*time);
			pList[n].pos[2] += pList[n].direction[2]*(pList[n].velocity[2]*time);

			pList[n].life -= pList[n].decay;
			pList[n].color[3] = pList[n].life;
			pList[n].prevcolor[3] = pList[n].life;
			
			if(pList[n].life <= 0.0)
			{
				if(looping)
				{
					initParticle(n);
				}
			}
		}
	}
}


void SnowParticle::Draw(float time)
{
	if(!pList)
		return;

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glPushMatrix();
	glBegin(GL_QUADS);
	for(int n = 0; n < maxParticles; n++)
	{
		glColor4fv(pList[n].prevcolor);
		glVertex3f(pList[n].prevpos[0], pList[n].prevpos[1], pList[n].prevpos[2]);
		glVertex3f(pList[n].prevpos[0]+pList[n].size, pList[n].prevpos[1], pList[n].prevpos[2]);

		glColor4fv(pList[n].color);
		glVertex3f(pList[n].pos[0]+pList[n].size, pList[n].pos[1], pList[n].pos[2]);
		glVertex3f(pList[n].pos[0], pList[n].pos[1], pList[n].pos[2]);
		
	}
	glEnd();
	Update(time);
	glPopMatrix();
	glDisable(GL_BLEND);
}



LorenzParticle::LorenzParticle()
{
	depth = 1.0;
	area =  1;
	image = NULL;
	texId = 0;
	fileName = NULL;
	beta = 2.66;
	phi = 52.50;
	omega = 10.0;
}

LorenzParticle::LorenzParticle(const char *file)
{
	if(file != NULL)
	{
		fileName = new char[strlen(file)];
		strcpy(fileName, file);
	}

	depth = 1.0;
	area =  1;
	image = NULL;
	texId = 0;
	beta = 2.66;
	phi = 52.50;
	omega = 10.0;
}

void LorenzParticle::setArea(int a)
{
	area = a;
}

void LorenzParticle::setDepth(float d)
{
	depth = d;
}

bool LorenzParticle::InitializeParticles()
{
	if(!pList)	
		pList = new sParticle[maxParticles];

	if(fileName)
	{
		image = corona::OpenImage(fileName);
		if(image)
		{	
			glGenTextures(1, &texId);
			glBindTexture(GL_TEXTURE_2D, texId);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			glTexEnvf(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image->getWidth(), image->getHeight(), GL_UNSIGNED_BYTE, GL_RGB, image->getPixels());
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->getWidth(), image->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, image->getPixels());
		}
	}

	for(int n = 0; n < maxParticles; n++)
	{
		pList[n].color[0] = 0.7;
		pList[n].color[1] = 0.7;
		pList[n].color[2] = 0.7;

		float theta;
		float angle;
		theta = (float)rand()/(2*pi);
		angle = (float)rand()/pi;

		pList[n].pos[0] = area * cos(theta) * sin(angle);
		pList[n].pos[1] = area * sin(theta) * sin(angle);
		pList[n].pos[2] = depth * cos(angle);

		pList[n].prevpos[0] = pList[n].pos[0];
		pList[n].prevpos[1] = pList[n].pos[1];
		pList[n].prevpos[2] = pList[n].pos[2];

		pList[n].life = 1.0;

		int num;
		num = rand()%2;
		if(num)
			pList[n].pos[0] = -pList[n].pos[0];
		num = rand()%2;
		if(num)
			pList[n].pos[1] = -pList[n].pos[1];

		pList[n].size = ((float)rand()/RAND_MAX);
	}

	return true;
}

bool LorenzParticle::initParticle(int n)
{
	return true;
}

void LorenzParticle::Update(float time)
{
	// The update function for this particle uses the lorenze attractor function
	if(!paused)
	{
		for(int n = 0; n < maxParticles; n++)
		{
			pList[n].pos[0] += (omega*(pList[n].pos[1]-pList[n].pos[0]))*time;
			pList[n].pos[1] += (pList[n].pos[0]*(phi+pList[n].pos[2])-pList[n].pos[1])*time;
			pList[n].pos[2] -= ((pList[n].pos[0]*pList[n].pos[1])+(beta*pList[n].pos[2]))*time;
		}
	}
	return;
}


void LorenzParticle::Draw(float time)
{
	if(!pList)
		return;

	float mat[16];
	float a[3],b[3],c[3],d[3],right[3],up[3],pos[3];

	glGetFloatv(GL_MODELVIEW_MATRIX, mat);
	
	right[0] = mat[0];	
	right[1] = mat[4];
	right[2] = mat[8];	
	up[0] = mat[1];
	up[1] = mat[5];
	up[2] = mat[9];
	pos[0] = mat[12];
	pos[1] = mat[13];
	pos[2] = mat[14];


	if(image)
	{
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		billboardCheatCylindricalBegin();
		glBindTexture(GL_TEXTURE_2D, texId);
	}
	else
	{
		glBegin(GL_POINTS);	
		glPointSize(2.0);
	}

	for(int n = 0; n < maxParticles; n++)
	{
		a[0] = pList[n].pos[0] - (right[0] + up[0]) * pList[n].size;
		a[1] = pList[n].pos[1] - (right[1] + up[1]) * pList[n].size;
		a[2] = pList[n].pos[2] - (right[2] + up[2]) * pList[n].size;

		b[0] = pList[n].pos[0] + (right[0] - up[0]) * pList[n].size;
		b[1] = pList[n].pos[1] + (right[1] - up[1]) * pList[n].size;
		b[2] = pList[n].pos[2] + (right[2] - up[2]) * pList[n].size;

		c[0] = pList[n].pos[0] + (right[0] + up[0]) * pList[n].size;
		c[1] = pList[n].pos[1] + (right[1] + up[1]) * pList[n].size;
		c[2] = pList[n].pos[2] + (right[2] + up[2]) * pList[n].size;

		d[0] = pList[n].pos[0] - (right[0] - up[0]) * pList[n].size;
		d[1] = pList[n].pos[1] - (right[1] - up[1]) * pList[n].size;
		d[2] = pList[n].pos[2] - (right[2] - up[2]) * pList[n].size;

		if(image)
		{	
			glBegin(GL_TRIANGLE_FAN);
			glColor4f(pList[n].color[0],pList[n].color[1],pList[n].color[2],pList[n].life);
			glTexCoord2f(0.0, 0.0);	glVertex3f(b[0],b[1],b[2]);
			glTexCoord2f(1.0, 0.0);	glVertex3f(a[0],a[1],a[2]);
			glTexCoord2f(1.0, 1.0);	glVertex3f(d[0],d[1],d[2]);
			glTexCoord2f(0.0, 1.0); glVertex3f(c[0],c[1],c[2]);
			glEnd();
		}
		else
		{
			glColor4f(pList[n].color[0],pList[n].color[1],pList[n].color[2],pList[n].life);
			glVertex3f(pList[n].pos[0],pList[n].pos[1],pList[n].pos[2]);
		}	
	}

	if(image)
	{
		billboardEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
	}

	Update(time);
}

//***************************************************************************************
//***************************************************************************************


LogarithmicParticle::LogarithmicParticle()
{
	depth = 1.0;
	image = NULL;
	texId = 0;
	fileName = NULL;
	A = 1.0;
	B = 1.0;
}

LogarithmicParticle::LogarithmicParticle(const char *file)
{
	if(file != NULL)
	{
		fileName = new char[strlen(file)];
		strcpy(fileName, file);
	}

	depth = 1.0;
	image = NULL;
	texId = 0;
	A = 1.0;
	B = 1.0;
}

void LogarithmicParticle::setAParam(float a)
{
	A = a;
}

void LogarithmicParticle::setBParam(float b)
{
	B = b;
}

void LogarithmicParticle::setDepth(float d)
{
	depth = d;
}

bool LogarithmicParticle::InitializeParticles()
{
	if(!pList)	
		pList = new sParticle[maxParticles];

	if(fileName)
	{
		image = corona::OpenImage(fileName);
		if(image)
		{	
			glGenTextures(1, &texId);
			glBindTexture(GL_TEXTURE_2D, texId);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			glTexEnvf(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image->getWidth(), image->getHeight(), GL_UNSIGNED_BYTE, GL_RGB, image->getPixels());
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->getWidth(), image->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, image->getPixels());
		}
	}

	for(int n = 0; n < maxParticles; n++)
	{
		pList[n].color[0] = 0.7;
		pList[n].color[1] = 0.7;
		pList[n].color[2] = 0.7;

		float theta;
		theta = (float)rand()/(2*pi);

		pList[n].pos[0] = A+B*cos(theta);
		pList[n].pos[1] = A+B*sin(theta);
		pList[n].pos[2] = depth*cos(theta);

		pList[n].prevpos[0] = pList[n].pos[0];
		pList[n].prevpos[1] = pList[n].pos[1];
		pList[n].prevpos[2] = pList[n].pos[2];

		pList[n].life = 1.0;

		int num;
		num = rand()%2;
		if(num)
		{
			pList[n].pos[0] = -pList[n].pos[0];
			pList[n].pos[1] = -pList[n].pos[1];
			pList[n].pos[2] = -pList[n].pos[2];
		}

		pList[n].size = ((float)rand()/RAND_MAX);
	}

	return true;
}

bool LogarithmicParticle::initParticle(int n)
{
	return true;
}

void LogarithmicParticle::Update(float time)
{
	return;
}


void LogarithmicParticle::Draw(float time)
{
	if(!pList)
		return;

	float mat[16];
	float a[3],b[3],c[3],d[3],right[3],up[3],pos[3];

	glGetFloatv(GL_MODELVIEW_MATRIX, mat);
	
	right[0] = mat[0];	
	right[1] = mat[4];
	right[2] = mat[8];	
	up[0] = mat[1];
	up[1] = mat[5];
	up[2] = mat[9];
	pos[0] = mat[12];
	pos[1] = mat[13];
	pos[2] = mat[14];


	if(image)
	{
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		billboardCheatCylindricalBegin();
		glBindTexture(GL_TEXTURE_2D, texId);
	}
	else
	{
		glBegin(GL_POINTS);	
		glPointSize(2.0);
	}

	for(int n = 0; n < maxParticles; n++)
	{
		a[0] = pList[n].pos[0] - (right[0] + up[0]) * pList[n].size;
		a[1] = pList[n].pos[1] - (right[1] + up[1]) * pList[n].size;
		a[2] = pList[n].pos[2] - (right[2] + up[2]) * pList[n].size;

		b[0] = pList[n].pos[0] + (right[0] - up[0]) * pList[n].size;
		b[1] = pList[n].pos[1] + (right[1] - up[1]) * pList[n].size;
		b[2] = pList[n].pos[2] + (right[2] - up[2]) * pList[n].size;

		c[0] = pList[n].pos[0] + (right[0] + up[0]) * pList[n].size;
		c[1] = pList[n].pos[1] + (right[1] + up[1]) * pList[n].size;
		c[2] = pList[n].pos[2] + (right[2] + up[2]) * pList[n].size;

		d[0] = pList[n].pos[0] - (right[0] - up[0]) * pList[n].size;
		d[1] = pList[n].pos[1] - (right[1] - up[1]) * pList[n].size;
		d[2] = pList[n].pos[2] - (right[2] - up[2]) * pList[n].size;

		if(image)
		{	
			glBegin(GL_TRIANGLE_FAN);
			glColor4f(pList[n].color[0],pList[n].color[1],pList[n].color[2],pList[n].life);
			glTexCoord2f(0.0, 0.0);	glVertex3f(b[0],b[1],b[2]);
			glTexCoord2f(1.0, 0.0);	glVertex3f(a[0],a[1],a[2]);
			glTexCoord2f(1.0, 1.0);	glVertex3f(d[0],d[1],d[2]);
			glTexCoord2f(0.0, 1.0); glVertex3f(c[0],c[1],c[2]);
			glEnd();
		}
		else
		{
			glColor4f(pList[n].color[0],pList[n].color[1],pList[n].color[2],pList[n].life);
			glVertex3f(pList[n].pos[0],pList[n].pos[1],pList[n].pos[2]);
		}	
	}

	if(image)
	{
		billboardEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
	}

	Update(time);
}
