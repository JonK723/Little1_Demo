#include "LogoScene.h"

#define MAX_CUBES 200

struct simpleCube {
	float pos[2];	// cube x, y position	(scaled between -0.5 and 0.5)
	float color[3];	// cube color (blue/gray tint
	float rotation[3];	// the normalized rotation for the cube
	float size;	// the size of the cube between 0 and 1
	float angle;
};


unsigned int logopic = -1;
unsigned int names = -1;
corona::Image *logoImg = NULL;
static float logoalpha = 0.0;
static float speed = 0.002;
static float angle = 0.0;

float texCoords[][4] = {{0.0, 1.0,0.0}, {0.0, 0.0,0.0}, {1.0, 0.0,0.0}, {1.0, 1.0,0.0}};
bool chgDir = false;
simpleCube *cubes = NULL;
float cubeXPos = -70.0;


PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2DEXT = NULL;
PFNGLBINDRENDERBUFFEREXTPROC glBindRenderbufferEXT = NULL;
PFNGLRENDERBUFFERSTORAGEEXTPROC glRenderbufferStorageEXT = NULL;
PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC glFramebufferRenderbufferEXT = NULL;
PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebufferEXT = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatusEXT = NULL;
PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffersEXT = NULL;
PFNGLGENFRAMEBUFFERSEXTPROC glGenRenderbuffersEXT = NULL;

GLuint fb, rb, texid;	// framebuffer object, renderbuffer object, and texture id



bool InitLogoScene()
{
	logoImg = corona::OpenImage("data/accfilm.jpg");
	if(!logoImg)
		return false;

	glGenTextures(1, &logopic);
	glBindTexture(GL_TEXTURE_2D, logopic);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexEnvf(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, logoImg->getWidth(), logoImg->getHeight(), GL_RGB, GL_UNSIGNED_BYTE, logoImg->getPixels());
	
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	
	delete logoImg;


	cubes = new simpleCube[MAX_CUBES];
	if(!cubes)
		return false;
	
	//initialize all the cubes
	for(int n = 0; n < MAX_CUBES; n++)
	{
		cubes[n].size = (float)rand()/RAND_MAX;	// give the cube an arbitrary size
		cubes[n].rotation[0] = (float)rand()/RAND_MAX;
		cubes[n].rotation[1] = (float)rand()/RAND_MAX;
		cubes[n].rotation[2] = (float)rand()/RAND_MAX;
		
		float len = sqrt((cubes[n].rotation[0]*cubes[n].rotation[0])+(cubes[n].rotation[1]*cubes[n].rotation[1])+(cubes[n].rotation[2]*cubes[n].rotation[2]));
		cubes[n].rotation[0] /= len;	// normalize the vector
		cubes[n].rotation[1] /= len;
		cubes[n].rotation[2] /= len;

		cubes[n].color[0] = 0.89;
		cubes[n].color[1] = 0.89;
		cubes[n].color[2] = 1.0;

		cubes[n].pos[0] = ((rand()%100)+1)-50.0;
		cubes[n].pos[1] = (float)rand()/RAND_MAX;
		cubes[n].pos[2] = 0.0;

		cubes[n].angle = ((float)rand()/RAND_MAX)+45.0;
	}


	// frame buffer stuff
	// get function pointers to the framebuffer functions
	glFramebufferTexture2DEXT = (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC) wglGetProcAddress("glFramebufferTexture2DEXT");
	glBindRenderbufferEXT = (PFNGLBINDRENDERBUFFEREXTPROC) wglGetProcAddress("glBindRenderbufferEXT");
	glRenderbufferStorageEXT = (PFNGLRENDERBUFFERSTORAGEEXTPROC) wglGetProcAddress("glRenderbufferStorageEXT");
	glFramebufferRenderbufferEXT = (PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC) wglGetProcAddress("glFramebufferRenderbufferEXT");
	glBindFramebufferEXT = (PFNGLBINDFRAMEBUFFEREXTPROC) wglGetProcAddress("glBindFramebufferEXT");
	glCheckFramebufferStatusEXT = (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC) wglGetProcAddress("glCheckFramebufferStatusEXT");
	glGenFramebuffersEXT = (PFNGLGENFRAMEBUFFERSEXTPROC) wglGetProcAddress("glGenFramebuffersEXT");
	glGenRenderbuffersEXT = (PFNGLGENFRAMEBUFFERSEXTPROC) wglGetProcAddress("glGenRenderbuffersEXT");

	if(!(glFramebufferTexture2DEXT && glBindRenderbufferEXT && glRenderbufferStorageEXT && glFramebufferRenderbufferEXT && glBindFramebufferEXT &&
		 glCheckFramebufferStatusEXT && glGenFramebuffersEXT && glGenRenderbuffersEXT))
	{
		return false;
	}

	glGenFramebuffersEXT(1, &fb);
	glGenRenderbuffersEXT(1, &rb);
	glGenTextures(1, &texid);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);
	
	glBindTexture(GL_TEXTURE_2D, texid);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 128, 0, GL_RGBA, GL_INT, NULL);
	
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, texid, 0);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, rb);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, 1024, 128);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, rb);


	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);	// unbind the framebuffer after setting everything up

	GLenum status;
	status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);

	switch(status)
	{
	case GL_FRAMEBUFFER_COMPLETE_EXT:	cout << "Framebuffer setup complete" << endl;
		break;
	case GL_FRAMEBUFFER_UNSUPPORTED_EXT:	cout << "Frambuffer format unsupported" << endl;
		break;
	default:
		break;
	}

	return true;
}


void RenderCubes()
{
	for(int n = 0; n < MAX_CUBES; n++)
	{
		glPushMatrix();
		glTranslatef(cubes[n].pos[0], cubes[n].pos[1], cubes[n].pos[2]);
		glRotatef(cubes[n].angle, cubes[n].rotation[0], cubes[n].rotation[1], cubes[n].rotation[2]);

		glEnable(GL_LIGHTING);
		glColor3fv(cubes[n].color);
		glBegin(GL_QUADS);			// Start Drawing Quads
		// Front Face
		glNormal3f( 0.0f, 0.0f, 1.0f);		// Normal Facing Forward
		glVertex3f(-cubes[n].size, -cubes[n].size,  cubes[n].size);	// Bottom Left Of The Texture and Quad
		glVertex3f( cubes[n].size, -cubes[n].size,  cubes[n].size);	// Bottom Right Of The Texture and Quad
		glVertex3f( cubes[n].size,  cubes[n].size,  cubes[n].size);	// Top Right Of The Texture and Quad
		glVertex3f(-cubes[n].size,  cubes[n].size,  cubes[n].size);	// Top Left Of The Texture and Quad
		// Back Face
		glNormal3f( 0.0f, 0.0f,-1.0f);		// Normal Facing Away
		glVertex3f(-cubes[n].size, -cubes[n].size, -cubes[n].size);	// Bottom Right Of The Texture and Quad
		glVertex3f(-cubes[n].size,  cubes[n].size, -cubes[n].size);	// Top Right Of The Texture and Quad
		glVertex3f( cubes[n].size,  cubes[n].size, -cubes[n].size);	// Top Left Of The Texture and Quad
		glVertex3f( cubes[n].size, -cubes[n].size, -cubes[n].size);	// Bottom Left Of The Texture and Quad
		// Top Face
		glNormal3f( 0.0f, 1.0f, 0.0f);		// Normal Facing Up
		glVertex3f(-cubes[n].size,  cubes[n].size, -cubes[n].size);	// Top Left Of The Texture and Quad
		glVertex3f(-cubes[n].size,  cubes[n].size,  cubes[n].size);	// Bottom Left Of The Texture and Quad
		glVertex3f(cubes[n].size,  cubes[n].size,  cubes[n].size);	// Bottom Right Of The Texture and Quad
		glVertex3f(cubes[n].size,  cubes[n].size, -cubes[n].size);	// Top Right Of The Texture and Quad
		// Bottom Face
		glNormal3f( 0.0f,-1.0f, 0.0f);		// Normal Facing Down
		glVertex3f(-cubes[n].size, -cubes[n].size, -cubes[n].size);	// Top Right Of The Texture and Quad
		glVertex3f( cubes[n].size, -cubes[n].size, -cubes[n].size);	// Top Left Of The Texture and Quad
		glVertex3f( cubes[n].size, -cubes[n].size,  cubes[n].size);	// Bottom Left Of The Texture and Quad
		glVertex3f(-cubes[n].size, -cubes[n].size,  cubes[n].size);	// Bottom Right Of The Texture and Quad
		// Right face
		glNormal3f( 1.0f, 0.0f, 0.0f);		// Normal Facing Right
		glVertex3f( cubes[n].size, -cubes[n].size, -cubes[n].size);	// Bottom Right Of The Texture and Quad
		glVertex3f( cubes[n].size,  cubes[n].size, -cubes[n].size);	// Top Right Of The Texture and Quad
		glVertex3f( cubes[n].size,  cubes[n].size,  cubes[n].size);	// Top Left Of The Texture and Quad
		glVertex3f( cubes[n].size, -cubes[n].size,  cubes[n].size);	// Bottom Left Of The Texture and Quad
		// Left Face
		glNormal3f(-1.0f, 0.0f, 0.0f);		// Normal Facing Left
		glVertex3f(-cubes[n].size, -cubes[n].size, -cubes[n].size);	// Bottom Left Of The Texture and Quad
		glVertex3f(-cubes[n].size, -cubes[n].size,  cubes[n].size);	// Bottom Right Of The Texture and Quad
		glVertex3f(-cubes[n].size,  cubes[n].size,  cubes[n].size);	// Top Right Of The Texture and Quad
		glVertex3f(-cubes[n].size,  cubes[n].size, -cubes[n].size);	// Top Left Of The Texture and Quad
		glEnd();
		
		glDisable(GL_LIGHTING);

		//Draw the box outlines
		glPushAttrib(GL_HINT_BIT);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		glLineWidth(0.2);
		glEnable(GL_LINE_SMOOTH);

		glColor3f(0.3, 0.3, 0.3);
		glBegin(GL_LINES);			// Start Drawing Quads
		// Front Face
		
		glVertex3f(-cubes[n].size, -cubes[n].size,  cubes[n].size);	// Bottom Left Of The Texture and Quad
		glVertex3f( cubes[n].size, -cubes[n].size,  cubes[n].size);	// Bottom Right Of The Texture and Quad
		glVertex3f( cubes[n].size,  cubes[n].size,  cubes[n].size);	// Top Right Of The Texture and Quad
		glVertex3f(-cubes[n].size,  cubes[n].size,  cubes[n].size);	// Top Left Of The Texture and Quad
		// Back Face
		
		glVertex3f(-cubes[n].size, -cubes[n].size, -cubes[n].size);	// Bottom Right Of The Texture and Quad
		glVertex3f(-cubes[n].size,  cubes[n].size, -cubes[n].size);	// Top Right Of The Texture and Quad
		glVertex3f( cubes[n].size,  cubes[n].size, -cubes[n].size);	// Top Left Of The Texture and Quad
		glVertex3f( cubes[n].size, -cubes[n].size, -cubes[n].size);	// Bottom Left Of The Texture and Quad
		// Top Face
		
		glVertex3f(-cubes[n].size,  cubes[n].size, -cubes[n].size);	// Top Left Of The Texture and Quad
		glVertex3f(-cubes[n].size,  cubes[n].size,  cubes[n].size);	// Bottom Left Of The Texture and Quad
		glVertex3f(cubes[n].size,  cubes[n].size,  cubes[n].size);	// Bottom Right Of The Texture and Quad
		glVertex3f(cubes[n].size,  cubes[n].size, -cubes[n].size);	// Top Right Of The Texture and Quad
		// Bottom Face
		
		glVertex3f(-cubes[n].size, -cubes[n].size, -cubes[n].size);	// Top Right Of The Texture and Quad
		glVertex3f( cubes[n].size, -cubes[n].size, -cubes[n].size);	// Top Left Of The Texture and Quad
		glVertex3f( cubes[n].size, -cubes[n].size,  cubes[n].size);	// Bottom Left Of The Texture and Quad
		glVertex3f(-cubes[n].size, -cubes[n].size,  cubes[n].size);	// Bottom Right Of The Texture and Quad
		// Right face
		
		glVertex3f( cubes[n].size, -cubes[n].size, -cubes[n].size);	// Bottom Right Of The Texture and Quad
		glVertex3f( cubes[n].size,  cubes[n].size, -cubes[n].size);	// Top Right Of The Texture and Quad
		glVertex3f( cubes[n].size,  cubes[n].size,  cubes[n].size);	// Top Left Of The Texture and Quad
		glVertex3f( cubes[n].size, -cubes[n].size,  cubes[n].size);	// Bottom Left Of The Texture and Quad
		// Left Face
		
		glVertex3f(-cubes[n].size, -cubes[n].size, -cubes[n].size);	// Bottom Left Of The Texture and Quad
		glVertex3f(-cubes[n].size, -cubes[n].size,  cubes[n].size);	// Bottom Right Of The Texture and Quad
		glVertex3f(-cubes[n].size,  cubes[n].size,  cubes[n].size);	// Top Right Of The Texture and Quad
		glVertex3f(-cubes[n].size,  cubes[n].size, -cubes[n].size);	// Top Left Of The Texture and Quad
		glEnd();

		glPopMatrix();
		glPopAttrib();

		glDisable(GL_LINE_SMOOTH);
		cubes[n].angle += 0.1;
		if(cubes[n].angle > 360.0)
			cubes[n].angle = 0.0;
	}
}



int DrawLogoScene(float t)
{
	float seconds = fabs((t-tStart))*1000.0;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	
	if((seconds >= 167000.001)) //&& (seconds < 230000.001))
	{
		texCoords[0][0] += speed;
		texCoords[1][0] += speed;
		texCoords[2][0] += speed;
		texCoords[3][0] += speed;

		if((seconds > 169000.001))
		{
			glEnable(GL_BLEND);
			glEnable(GL_TEXTURE);
			glEnable(GL_TEXTURE_2D);

			beginOrtho(800, 600);

			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
			glBindTexture(GL_TEXTURE_2D, logopic);
		
			glPushMatrix();
			glTranslatef(0.0,0.0,0.0);

			glBegin(GL_TRIANGLE_FAN);
			glColor4f(0.5,0.5,0.5,logoalpha);	
			glTexCoord3fv(texCoords[0]); glVertex3f(0.0,210.0,0.0);
			glTexCoord3fv(texCoords[1]); glVertex3f(0.0,330.0,0.0);
			glTexCoord3fv(texCoords[2]); glVertex3f(805.0,330.0,0.0);
			glTexCoord3fv(texCoords[3]); glVertex3f(805.0,210.0,0.0);
			glEnd();

			glPopMatrix();

			if((seconds >= 169000.0001) && (seconds < 175000.0001))
			{
				logoalpha += (t*1000)/5000.00;
			}

			endOrtho();

			glDisable(GL_BLEND);
			glDisable(GL_TEXTURE);
			glDisable(GL_TEXTURE_2D);
		}

		if(seconds >= 171000.0001)
		{
			glPushAttrib(GL_VIEWPORT_BIT);
			glViewport(0, 0, 800, 228);	// change the viewport to the size of the texture


			// enable the frame buffer object
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);	

			glClearColor(0.0, 0.0, 0.0, 0.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				
			glPushMatrix();
			glTranslatef(cubeXPos, -6.0, -18.0);
			
				RenderCubes();	// render the cubes  

			glPopMatrix();


			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);	// disable the frame buffer object so the texture can be enabled
			glBindTexture(GL_TEXTURE_2D, texid);			// bind the texture that the frame buffer rendered
			
			glPopAttrib();


			glClearColor(0.0,0.0,0.0,0.0);

			beginOrtho(800, 600);	// begin orthographic projecetion

			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_TEXTURE);
			glEnable(GL_TEXTURE_2D);

			glPushMatrix();			// render the texture
			glTranslatef(0.0, 0.0, 0.8);
			glBegin(GL_TRIANGLE_FAN);
			glColor4f(1.0, 1.0, 1.0, 1.0);
			glTexCoord2d(0.0, 1.0); glVertex3f(0.0,210.0,0.0);
			glTexCoord2d(0.0, 0.0); glVertex3f(0.0,330.0,0.0);
			glTexCoord2d(1.0, 0.0); glVertex3f(1290.0,330.0,0.0);
			glTexCoord2d(1.0, 1.0); glVertex3f(1290.0,210.0,0.0);
			glEnd();
			glPopMatrix();

			
			glDisable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);
			glEnable(GL_DEPTH_TEST);
		
			endOrtho();	// end orthographic projecetion
			

			cubeXPos += (t*1000)/1100.00;
			angle += (360.0*(t*1000))/(10000);
		}
	}

	return 1;
}


/*
GREETINGS LIST
---------------

mups
Truck
Damones
Byterapers
MFX
Kewelers
Paradox
FLT
TRSI
Farbrausch
Perkele
Anansmurska

Sceners from around the world!!

Fuckings to Lamers

*/