#include "ObjModel.h"

sVertex3d::sVertex3d()
{
	memset(vertex, 0, 3);
}

sTexCoord::sTexCoord()
{
	memset(coord,0,2);
}

sNormal3d::sNormal3d()
{
	memset(normal, 0, 3);
}

sMaterial::sMaterial()
{
	matname = "";
	memset(ambient, 0, 3);
	memset(diffuse, 0, 3);
	memset(specular, 0, 3);
	ambient[3] = diffuse[3] = specular[3] = 1.0;
	alpha = 1.0;	// completely opaque
	shine = 0.0;
	illum = 0;
	texture = "";
	image = NULL;
	texId = 0;
}

sFace::sFace()
{
	vertlist.clear();
	normlist.clear();
	texcoordlist.clear();

	combined.clear();
	material = "";
}


ObjModel::ObjModel()
{
	filename = "";	// the name of the obj model file
    numVerts = 0;
	numTexCoords = 0;
	numNormals = 0;
	numFaces = 0;

	pvertices = NULL;
	pnormals = NULL;
	ptexcoords = NULL;
	pfaces = NULL;	
}

ObjModel::~ObjModel()
{
	if(pvertices)
		delete [] pvertices;
	if(pnormals)
		delete [] pnormals;
	if(ptexcoords)
		delete [] ptexcoords;
	if(pfaces)
		delete [] pfaces;

	mats.clear();
}

bool ObjModel::LoadModel(char *file)
{
	if(!file)
		return false;

	filename = file;	// store the name of the model file

	ifstream modelfile;

	modelfile.open(file);

	if(!modelfile.is_open())
		return false;

	char line[512];	// array to hold a line of the file
	
	while(!modelfile.eof())
	{
		modelfile.getline(line, 512, '\n');

		if(line[0] == '#')	// ignore comments
		{
			continue;
		}
		else if(strstr(line, "v ") != NULL)
		{
			numVerts++;	// increase the number of vertices
		}
		else if(strstr(line, "vn ") != NULL)
		{
			numNormals++;
		}
		else if(strstr(line, "vt ") != NULL)
		{
			numTexCoords++;
		}
		else if(strstr(line, "f ") != NULL)
		{
			numFaces++;
		}
	}

	// allocate the space for the arrays
	if(numVerts)
		pvertices = new sVertex3d[numVerts];
	if(numNormals)
		pnormals = new sNormal3d[numNormals];
	if(numTexCoords) 
		ptexcoords = new sTexCoord[numTexCoords];
	if(numFaces)
		pfaces = new sFace[numFaces];	
	
	modelfile.close();
	modelfile.clear();
	modelfile.open(file);	// reopen the file

	int currv = 0;	// the curent vertex
	int currvn = 0;	// the current normal
	int currvt = 0;	// the current tex coord
	int currf = 0;	// the current face
	char facemtl[20];	// the material that this face uses
	memset(facemtl, 0, 20);

	while(!modelfile.eof())
	{
		modelfile.getline(line, 512, '\n');

		if(line[0] == '#')	// ignore comments
		{
			continue;
		}
		else if(strstr(line, "v ") != NULL)
		{
			float v1, v2, v3;

			sscanf(line, "v %f %f %f", &v1, &v2, &v3);
			pvertices[currv].vertex[0] = v1;
			pvertices[currv].vertex[1] = v2;
			pvertices[currv].vertex[2] = v3;

			currv++;
		}
		else if(strstr(line, "vn ") != NULL)
		{
			float vn1, vn2, vn3;

			sscanf(line, "vn %f %f %f", &vn1, &vn2, &vn3);
			pnormals[currvn].normal[0] = vn1;
			pnormals[currvn].normal[1] = vn2;
			pnormals[currvn].normal[2] = vn3;

			currvn++;
		}
		else if(strstr(line, "vt ") != NULL)
		{
			float vt1, vt2;

			sscanf(line, "vt %f %f", &vt1, &vt2);
			ptexcoords[currvt].coord[0] = vt1;
			ptexcoords[currvt].coord[1] = vt2;

			currvt++;
		}
		else if(strstr(line, "mtllib ") != NULL)
		{
			char temp[32];
			sscanf(line, "mtllib %s", temp);

			LoadMtl(temp);
		}
		else if(strstr(line, "f ") != NULL)
		{
			if(strstr(line, "//") != NULL)	// this face only has vertices and normals
			{
				int vert, norm;
				char *tok = strtok(line, " "); // go past the 'f'
				tok = strtok(NULL, " ");
				sscanf(tok,"%d//%d", &vert, &norm);
				pfaces[currf].vertlist.push_back(vert);
				pfaces[currf].normlist.push_back(norm);

				while(tok != NULL)
				{
					tok = strtok(NULL, " ");
					if(!tok)
						break;
					sscanf(tok,"%d//%d", &vert, &norm);
					pfaces[currf].vertlist.push_back(vert);
					pfaces[currf].normlist.push_back(norm);	
				}
				
				pfaces[currf].material = facemtl;

				currf++;	// go to the next face in the list
			}
			else	// we probably have a 'x/x/x' or x/x
			{
				int vert, norm, coord;
				char *tok = strtok(line, " "); // go past the 'f'
				tok = strtok(NULL, " ");

				int scount = 0;	// the number of slashes found it the section
				for(int x = 0; x < strlen(tok); x++)
				{
					if(tok[x] == '/')
						scount++;
				}
				
				if(scount == 1)
				{
					sscanf(tok,"%d/%d", &vert, &coord);
					pfaces[currf].vertlist.push_back(vert);
					pfaces[currf].texcoordlist.push_back(coord);

					while(tok != NULL)
					{
						tok = strtok(NULL, " ");
						if(!tok)
							break;

						sscanf(tok,"%d/%d", &vert, &coord);
						pfaces[currf].vertlist.push_back(vert);
						pfaces[currf].texcoordlist.push_back(coord);
					}
				}
				else if(scount == 2)
				{
					sscanf(tok,"%d/%d/%d", &vert, &coord,&norm);
					pfaces[currf].vertlist.push_back(vert);
					pfaces[currf].normlist.push_back(norm);
					pfaces[currf].texcoordlist.push_back(coord);

					while(tok != NULL)
					{
						tok = strtok(NULL, " ");
						if(!tok)
							break;

						sscanf(tok,"%d/%d/%d", &vert, &coord,&norm);
						pfaces[currf].vertlist.push_back(vert);
						pfaces[currf].normlist.push_back(norm);
						pfaces[currf].texcoordlist.push_back(coord);
					}
				}
				else // vertex information
				{
					sscanf(tok,"%d", &vert);
					pfaces[currf].vertlist.push_back(vert);
					
					while(tok != NULL)
					{
						tok = strtok(NULL, " ");
						if(!tok)
							break;

						sscanf(tok,"%d", &vert);
						pfaces[currf].vertlist.push_back(vert);
					}					
				}
				pfaces[currf].material = facemtl;

				currf++;
			}
		}
		else if(strstr(line, "usemtl ") != NULL)
		{
			sscanf(line, "usemtl %s", facemtl);

			pfaces[currf].material = facemtl;
		}
	}

	return true;
}


bool ObjModel::LoadMtl(char* mtlfilename)
{
	if(mtlfilename == "")
		return false;

	ifstream mtlfile;

	mtlfile.open(mtlfilename);
	if(!mtlfile.is_open())
		return false;


	sMaterial *pMat = NULL;
	char line[512];
    char texmap[20];	// the material that this face uses

	while(!mtlfile.eof())
	{
		mtlfile.getline(line,  512, '\n');
		memset(texmap, 0, 20);

		if(line[0] == '#')
		{
			continue;
		}
		else if(strstr(line, "newmtl ") != NULL)
		{
			if(pMat)	// if we've already allocated a material save it
			{
				mats[pMat->matname] = *pMat;   // use a hash instead of a vector for faster searching
			}
			pMat = new sMaterial();

			char t[32];
			sscanf(line, "%*s %s", t);
			pMat->matname = t;
		}
		else if(strstr(line, "map_") != NULL)
		{
			sscanf(line, "map_Ka %s", texmap);
			pMat->texture = texmap;	
			
			if((pMat->image = corona::OpenImage(pMat->texture.c_str())) != NULL)
			{
				glGenTextures(1, &pMat->texId);
				glBindTexture(GL_TEXTURE_2D, pMat->texId);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pMat->image->getWidth(), pMat->image->getHeight(), 0, GL_RGBA8, GL_UNSIGNED_INT, pMat->image->getPixels());
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}
		else if(strstr(line, "Ka ") != NULL)
		{
			if(pMat)
			{
				float kr, kg, kb;
				sscanf(line, "Ka %f %f %f", &kr, &kg, &kb);
				pMat->ambient[0] = kr;
				pMat->ambient[1] = kg;
				pMat->ambient[2] = kb;
			}
		}
		else if(strstr(line, "Kd ") != NULL)
		{
			if(pMat)
			{
				float kr, kg, kb;
				sscanf(line, "Kd %f %f %f", &kr, &kg, &kb);
				pMat->diffuse[0] = kr;
				pMat->diffuse[1] = kg;
				pMat->diffuse[2] = kb;
			}
		}
		else if(strstr(line, "Ks ") != NULL)
		{
			if(pMat)
			{
				float kr, kg, kb;
				sscanf(line, "Ks %f %f %f", &kr, &kg, &kb);
				pMat->specular[0] = kr;
				pMat->specular[1] = kg;
				pMat->specular[2] = kb;
			}
		}
		else if(strstr(line, "Ns ") != NULL)
		{
			if(pMat)
			{
				float ns;
				sscanf(line, "Ns %f", &ns);
				pMat->shine = ns;
			}
		}
		else if(strstr(line, "illum ") != NULL)
		{
			if(pMat)
			{
				int il;
				sscanf(line, "illum %d", &il);
				pMat->illum = il;
			}
		}
		else if(strstr(line, "Tr ") != NULL || strstr(line, "d ") != NULL)
		{
			if(pMat)
			{
				float a;
				sscanf(line, "%*s %f", &a);
				pMat->alpha = a;
				pMat->ambient[3] = pMat->diffuse[3] = pMat->specular[3] = a;
			}
		}
	}

	// store the last material found before the end of the file
	if(pMat)	// if we've already allocated a material save it
	{
		mats[pMat->matname] = *pMat;   // use a hash instead of a vector for faster searching
	}

	return true;
}


sMaterial* ObjModel::FindMatByName(string matname)
{
	if(matname == "" || mats.empty()) 
		return NULL;

	return &mats[matname];
}


void ObjModel::Draw()
{
	int i;

	for(i = 0; i < numFaces; i++)
	{
		if(pfaces[i].vertlist.size() == 3)
		{
			glBegin(GL_TRIANGLE_STRIP);
		}
		else
		{
			glBegin(GL_POLYGON);
		}

		// first get the material for the model before rendering it
		if(!mats.empty())
		{
			sMaterial *pMat;
			if((pMat = FindMatByName(pfaces[i].material)) != NULL)
			{
				if(pMat->alpha < 1.0)
				{
				  glEnable(GL_BLEND);
				  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
				}

				glMaterialfv(GL_FRONT, GL_AMBIENT, pMat->ambient);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, pMat->diffuse);
				if(pMat->illum != 1)
					glMaterialfv(GL_FRONT, GL_SPECULAR, pMat->specular);
				glMaterialf(GL_FRONT, GL_SHININESS, pMat->shine);
				if(pMat->texId)
				{
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, pMat->texId);
				}
			}
		}

		// the reason the texture array isnt checked here is because not all models will have a texture applied
		// also, there will never be a model with more texture coordinates than the number of vertices in the model
		// 
		if(pvertices || pnormals)
		{
			for(int x = 0; x < pfaces[i].vertlist.size(); x++)
			{
				// this is a redundant check since there cant be more vertex normals than vertices in the model
				if(pnormals)
					glNormal3fv(pnormals[pfaces[i].normlist[x]-1].normal);

				if(ptexcoords && (x <= pfaces[i].texcoordlist.size()))
					glTexCoord2fv(ptexcoords[pfaces[i].texcoordlist[x]-1].coord);
				
				glVertex3fv(pvertices[pfaces[i].vertlist[x]-1].vertex);
			}

			glEnd();
		}

		glDisable(GL_TEXTURE_2D);	
		glDisable(GL_BLEND);
	}
}
