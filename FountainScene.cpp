#include "FountainScene.h"

FountainParticle *fountain = NULL;
FountainParticle *fountain2 = NULL;
FountainParticle *fountain3 = NULL;


float pos1[] = {0.0, 0.0, 2.0};
float pos2[] = {1.5, 1.0, 2.0};
float pos3[] = {-2.0, -2.0, 2.0};
float pos4[] = {0.0, 0.0, 2.0};

float p[] = {0.0, 0.0, 2.0};
float p2[] = {-2.0, 1.0, 2.0};
float p3[] = {2.5, 2.0, 2.0};

static float angle = -90.0;

bool FountainSceneInit()
{
	fountain = new FountainParticle("data/t1.jpg");
	fountain2 = new FountainParticle("data/Galaxy-256.jpg");
	fountain3 = new FountainParticle("data/star6-128.jpg");

	if(!fountain)
		return false;
	if(!fountain2)
		return false;
	if(!fountain3)
		return false;

	fountain->setSize(900);
	fountain->setRadius(1.5);
	fountain->setHeight(3.0);
	fountain->setGravity(1.0);

	fountain2->setSize(900);
	fountain2->setRadius(0.1);
	fountain2->setHeight(3.0);
	fountain2->setGravity(5.5);

	fountain3->setSize(1800);
	fountain3->setRadius(0.2);
	fountain3->setHeight(3.0);
	fountain3->setGravity(6.0);


	if(!fountain->InitializeParticles())
	{
		return false;
	}

	if(!fountain2->InitializeParticles())
	{
		return false;
	}

	if(!fountain3->InitializeParticles())
	{
		return false;
	}

	return true;
}


int DrawFountain(float t)
{
	float seconds = fabs((t-tStart))*1000.0;

	if((seconds >= 136500.001) && (seconds <= 137000.001))
	{
		glClearColor(1.0,1.0,1.0,1.0);
	}
	else if((seconds >= 137000.001) && (seconds <= 138000.001))
	{
		glClearColor(0.0,0.0,0.0,0.0);
	}

	if(seconds >= 136500.001)
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

		glPushMatrix();
		glTranslatef(pos1[0], pos1[1], pos1[2]);
		glRotatef(angle, 1.0, 0.0, 0.0);
			
		fountain->Draw(t);
		fountain2->Draw(t);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(p[0], p[1], p[2]);
		glRotatef(angle, 1.0, 0.0, 0.0);

		fountain3->Draw(t);
		glPopMatrix();
	}

	if( (seconds >= 139000.001 && seconds < 142000.001) || (seconds >= 148000.001 && seconds < 151000.001) )
	{
		interpolatePos(pos1, pos2, (t*1000.0)/3000.0);
		interpolatePos(p, p2, (t*1000.0)/3000.0);
	}
	else if( (seconds >= 142000.001 && seconds < 145000.001) || (seconds >= 151000.001 && seconds < 154000.001) )
	{
		interpolatePos(pos1, pos3, (t*1000.0)/3000.0);
		interpolatePos(p, p3, (t*1000.0)/3000.0);
	}
	else if( (seconds >= 145000.001 && seconds < 148000.001) || (seconds >= 154000.001 && seconds < 157000.001) )
	{
		interpolatePos(pos1, pos4, (t*1000.0)/3000.0);
		interpolatePos(p, pos4, (t*1000.0)/3000.0);
	}
	else if(seconds >= 157000.001 && seconds < 161000.001)
	{
		interpolatePos(pos1, pos4, (t*1000.0)/1000.0);
		interpolatePos(p, pos4, (t*1000.0)/1000.0);
	}
	else if(seconds >= 164000.001 && seconds < 169000.001)
	{
		fountain->disableLooping();
		fountain2->disableLooping();
		fountain3->disableLooping();

		angle += (-90.0*(t*1000.00))/3000.00;
	}

	return 1;
}
