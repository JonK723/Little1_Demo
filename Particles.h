#ifndef _PARTICLES_H
#define _PARTICLES_H

#include "stdafx.h"

typedef float tVector3[3];
typedef float tColor[4];

// simple point structure
struct sParticle
{
	tVector3 pos;	// the position of the particle
	tVector3 prevpos;	// the previous position of the particle
	tVector3 direction;	// which direction the particle is moving
	tVector3 velocity;	// the speed at which the particles move
	float life;	// the life of this particle
	float size;
	float decay;
	tColor color;	// the color of the particle
	tColor prevcolor;	// the previous color of the particle
	tColor deltaColor;	// the rate at which the color changes
};


// initial particle class 
class BaseParticle 
{
public:
	BaseParticle();
	~BaseParticle();
	virtual void setSize(int sz);
	int getSize();
	virtual bool InitializeParticles() { return true; }	// initialize the list of particles
	virtual bool initParticle(int n) { return true; }	// initialize particle n
	virtual void Update(float time) { }
	virtual void Draw(float time) { }
	virtual void setGravity(float gv){ gravity = gv; }
	virtual float getGravity(){ return gravity; }
	sParticle* getParticles()
	{
		return pList;
	}
	void enableLooping();
	void disableLooping();
	void enablePaused();
	void disablePaused();

protected:
	float gravity;	// the rate at which the particles fall
	float height;
	sParticle *pList;
	int maxParticles;
	int totDead;
	bool looping;	//should this particle loop??
	bool paused;	// should this particle update??
	unsigned int texId;
	char *fileName;
	corona::Image *image;
};


// Particle system that looks like a water fountain
class FountainParticle : public BaseParticle
{
public:
	FountainParticle();
	FountainParticle(const char* file);
	~FountainParticle();
	bool InitializeParticles();
	bool initParticle(int n);
	virtual void Update(float time);
	virtual void Draw(float time);
	void setGravity(float gv);
	float getGravity();
	void setRadius(float r);
	float getRadius();
	void setHeight(float h);
	float getHeight();
private:
	float radius;	// the radius of the fountain spread
};



// Particle system that looks like snow falling
class SnowParticle : public BaseParticle
{
public:
	SnowParticle();
	~SnowParticle();
	bool InitializeParticles();
	bool initParticle(int n);
	virtual void Update(float time);
	virtual void Draw(float time);
	void setGravity(float gv);
	float getGravity();
	void setHeight(float h);
	float getHeight();
	void setArea(int a);
	int getArea();
private:
	int area;
};


// The lorenz particle system generates particles which create the lorenz pattern
// as they move....this is also called the butterfly pattern
class LorenzParticle : public BaseParticle
{
public:
	LorenzParticle();
	LorenzParticle(const char* file);
	~LorenzParticle(){}
	bool InitializeParticles();
	bool initParticle(int n);
	virtual void Update(float time);
	virtual void Draw(float time);
	void setArea(int a);
	void setDepth(float d);

	void setPhi(float ph)
	{
		phi = ph;
	}
	void setOmg(float om)
	{
		omega = om;
	}
	void setBeta(float bt)
	{
	    beta = bt;
	}

	float getPhi()
	{
		return phi;
	}
	float getOmg()
	{
		return omega;
	}
	float getBeta()
	{
		return beta;
	}

private:
	int area;
	float depth;
	float phi;
	float omega;
	float beta;
};



class LogarithmicParticle : public BaseParticle
{
public:
	LogarithmicParticle();
	LogarithmicParticle(const char* file);
	~LogarithmicParticle(){}
	bool InitializeParticles();
	bool initParticle(int n);
	virtual void Update(float time);
	virtual void Draw(float time);
	void setAParam(float a);
	void setBParam(float b);
	void setDepth(float d);
private:
	float depth;
	float A;
	float B;
};

#endif
