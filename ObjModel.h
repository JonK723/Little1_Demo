#ifndef OBJMODEL_H
#define OBJMODEL_H

#include "stdafx.h"


// defines a single vertex for the model
struct sVertex3d 
{
	sVertex3d();
	float vertex[3];
};

// a single texture coordinate
struct sTexCoord
{
	sTexCoord();
	float coord[2];
};

// a single triangle normal
struct sNormal3d
{
	sNormal3d();
	float normal[3];
};

struct sMaterial
{
	sMaterial();
	string matname;	// the name of the material
	float ambient[4];	// the ambient color of this material
	float diffuse[4];	// the diffuse color of the material
	float specular[4];	// the specular color of the material
	float alpha;	// the alpha value of the material
	float shine;	// how much does this material shine
	int illum;		// the illumination model for this material
	string texture;	// the texture this material uses
	corona::Image* image;
	unsigned int texId;
};

struct sFace
{
	sFace();
	vector<int> vertlist;	// the vertices that make up this face
	vector<int> normlist;	// the list of normals that make up this face
	vector<int> texcoordlist;	// the list of texture coordinates that make up this face
	vector<int> combined;	// the combined indices for this face

	string material;	// which material does this face use
};



class ObjModel {
public:
	ObjModel();
	~ObjModel();
	bool LoadModel(char *fname);
	bool LoadMtl(char *mtlfilename);	// loads a material library file
	sMaterial *FindMatByName(string name);	// searches the array of materials by name and returns the matching material
	void Draw();	// draw the geometry for the model

protected:
	string filename;	// the name of the obj model file
	int numVerts;
	int numTexCoords;
	int numNormals;
	int numFaces;

	sVertex3d *pvertices;	// array of vertices in the model
	sNormal3d *pnormals;	// array of normals in the model		
	sTexCoord *ptexcoords;  // array of texture coordinates in the model
	sFace *pfaces;	// array of faces in the model 

	map<string, sMaterial> mats;	// the list of materials from all of the material libraries
};


#endif
