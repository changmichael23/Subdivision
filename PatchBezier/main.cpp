#include <math.h>
#include <vector>
#include <iostream>
#include "Camera.hpp"
#include "UtilEnums.hpp"
#include "PatchesController.hpp"
#include "Controller.hpp"
#include "Colore.hpp"
#ifdef _MSC_VER
#pragma comment(lib, "opengl32.lib")
#include <windows.h>
#endif

#ifdef _MSC_VER
#pragma comment(lib, "glew32.lib")
#endif

#include "../common/EsgiShader.h"
#include "../common/mat4.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../stb/stb_image.h"

// format des vertices : X, Y, Z, ?, ?, ?, ?, ? = 8 floats
//#include "../data/DragonData.h"

int sizetab, sizeind;

//--------------------------------------------------------------------------------------------------------------------------------------------------------
EsgiShader g_BasicShader;

std::vector<Point> pointsControles3D;           // Tous les points en 3D
std::vector<Point> gridPoints3D;
float* tabPoints, *tmpPoints;         //Tous les points en 3D
GLushort* createInd(int);
GLushort* createIndForGridPoints();
GLushort* createIndForGridPoints(std::vector<int> tmpCptGP);
GLushort* indi;			// Tab indice
GLushort* indTmp;			// Tab indice

std::vector<Point> allGridPoints = std::vector<Point>();
std::vector<Point> allControlPoints= std::vector<Point>();
int cptCtrlPts = 0;
int sumStatic = 0;

GLuint VAO;
GLuint VBO0;	// identifiant du Vertex Buffer Object 0
GLuint VBO1;	// identifiant du Vertex Buffer Object 1
GLuint IBO,IBO1;	// identifiant du Index Buffer Object
GLuint TexObj; // identifiant du Texture Object

float colore[4];
float sizeCube = 0.1f;

std::vector<Colore> col;

//PatchController variables
int nbPatches = 0;
std::vector<Patch> patches = std::vector<Patch>();
Patch tmpPatch;
int nbPoints;
int pointIdx, patchIdx;
int precision = 4;

//Controller variables
float TimeInSeconds;
int TimeSinceAppStartedInMS;
int OldTime = 0;
float DeltaTime;
float heightStep = .5f;
float rotateStep = 5.f;
float translateStep = .5f;
float resizeStep = .05f;

State gs;
EditMode em;
RotateMode rm = matrix;
int menu_Main, menu_sub1, menu_sub2;
void CreateGlutMenu();
bool Initialize1();
bool Initialize2();

bool initialized1 = false;
bool initialized2 = false;

void structToTabColor(std::vector<Point> newPoints, std::vector<Colore> c,float * tabP)
{
	int j = 0;
	for (int i = 0; i < newPoints.size() * 9; i += 9)
	{
		tabP[i] = newPoints[j].x;
		tabP[i + 1] = newPoints[j].y;
		tabP[i + 2] = newPoints[j].z;

		tabP[i + 3] = -newPoints[j].n1;
		tabP[i + 4] = -newPoints[j].n2;
		//tabP[i + 4] = newPoints[j].n2;
		tabP[i + 5] = -newPoints[j].n3;

		

		
			/*tabP[i + 6] = RandomFloat(0, 1);
			tabP[i + 7] = RandomFloat(0, 1);
			tabP[i + 8] = RandomFloat(0, 1);*/
		
		tabP[i + 6] = newPoints[j].c1;
		tabP[i + 7] = newPoints[j].c2;
		tabP[i + 8] = newPoints[j].c3;

		j++;
	}

}

void structToTabTmp(std::vector<Point> newPoints, std::vector<Colore> c,float * tabP)
{
	
	int j = 0;
	for (int i = 0; i < newPoints.size() * 3; i += 3)
	{
		tabP[i] = newPoints[j].x;
		tabP[i + 1] = newPoints[j].y;
		tabP[i + 2] = newPoints[j].z;
		j++;
	}
}
void MenuFunction(int i)
{
	switch (i)
	{
	case 0: StartNewPatch();
		Initialize1(); break;
	case 1: CancelPatch(); break;
	case 2: 
		ConfirmPatch();
		Initialize2();
	break;
	case 30: RotatePatch(0); break;
	case 31: RotatePatch(1); break;
	case 32: RotatePatch(2); break;
	case 40: TranslatePatch(0); break;
	case 41: TranslatePatch(1); break;
	case 42: TranslatePatch(2); break;
	case 5: ResizePatch(); break;
	case 6: CancelEdit(); break;
	case 7: ChangeRotationMethod(); break;
	case 8: DeletePatch(); break;
	case 9: QuitDelete(); break;
	case 10: ConfirmDelete(); break;
	case 11: SubdivisePatch(); break;
	case 12: ConfirmSubdivise(); break;
	case 13: CancelSubdivise(); break;
	default:
		break;
	}
	glutDestroyMenu(menu_Main);
	CreateGlutMenu();
}

void CreateGlutMenu()
{
	if (rm == quaternion)
	{
		menu_sub1 = glutCreateMenu(MenuFunction);
		glutAddMenuEntry("X (1, 0, 0)", 30);
		glutAddMenuEntry("Y (0, 1, 0)", 31);
		glutAddMenuEntry("Z (0, 0, 1)", 32);
	}
	else if (rm == matrix)
	{
		menu_sub1 = glutCreateMenu(MenuFunction);
		glutAddMenuEntry("X", 30);
		glutAddMenuEntry("Y", 31);
		glutAddMenuEntry("Z", 32);
	}

	menu_sub2 = glutCreateMenu(MenuFunction);
	glutAddMenuEntry("X", 40);
	glutAddMenuEntry("Y", 41);
	glutAddMenuEntry("Z", 42);

	menu_Main = glutCreateMenu(MenuFunction);
	if (gs == first)
	{
		glutAddMenuEntry("Tracer une surface de Bezier", 0);
	}
	else if (gs == idle)
	{
		glutAddMenuEntry("Tracer une surface de Bezier", 0);
		glutAddSubMenu("Rotation...", menu_sub1);
		glutAddSubMenu("Translation...", menu_sub2);
		glutAddMenuEntry("Redimensionnement", 5);
		glutAddMenuEntry("Changer la méthode de rotation", 7);
		glutAddMenuEntry("Supprimer une surface", 8);
		glutAddMenuEntry("Subdiviser une surface", 11);
	}
	else if (gs == editing)
	{
		glutAddSubMenu("Rotation...", menu_sub1);
		glutAddSubMenu("Translation...", menu_sub2);
		glutAddMenuEntry("Redimensionnement", 5);
		glutAddMenuEntry("Changer la méthode de rotation", 7);
		glutAddMenuEntry("Supprimer une surface", 8);
		glutAddMenuEntry("Quitter le mode édition", 6);
	}
	else if (gs == drawing)
	{
		glutAddMenuEntry("Annuler le tracé", 1);
		glutAddMenuEntry("Terminer le tracé", 2);
	}
	else if (gs == deleting)
	{
		glutAddMenuEntry("Annuler la suppression", 9);
		glutAddMenuEntry("Supprimer la surface", 10);
	}
	else if (gs == subdivising)
	{
		glutAddMenuEntry("Annuler la subdivision", 13);
		glutAddMenuEntry("Subdiviser la surface", 12);
	}

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

bool Initialize1()
{
	initialized1 = true;
	colore[0] = 0.0;
	colore[1] = 1.0;
	colore[2] = 1.0;
	colore[3] = 1.0;

	std::vector<Point> centerPoints3D = tmpPatch.controlPoints;

	std::vector<Colore> tmpColore;
	
	pointsControles3D = transformPointsToCube(centerPoints3D);

	for (int i = 0; i < pointsControles3D.size(); i++)
	{
		tmpColore.push_back(Colore(red));
	}

	tabPoints = new float[pointsControles3D.size()*9];

	structToTabColor(pointsControles3D,tmpColore,tabPoints);

	indi = createInd(centerPoints3D.size()*24);


	glewInit();
	g_BasicShader.LoadVertexShader("basic.vs");
	g_BasicShader.LoadFragmentShader("basic.fs");
	g_BasicShader.CreateProgram();

	//glGenTextures(1, &TexObj);
	//glBindTexture(GL_TEXTURE_2D, TexObj);
	//int w, h, c; //largeur, hauteur et # de composantes du fichier
	//uint8_t* bitmapRGBA = stbi_load("../data/dragon.png",
	//	&w, &h, &c, STBI_rgb_alpha);

	////glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); //GL_NEAREST)
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, // Destination
	//	GL_RGBA, GL_UNSIGNED_BYTE, bitmapRGBA);		// Source

	//glGenerateMipmap(GL_TEXTURE_2D);

	//stbi_image_free(bitmapRGBA);

	//glBindTexture(GL_TEXTURE_2D, 0);
	
	// Points controles VBO0
	glGenVertexArrays(1, &VBO0); // Créer le VAO
	glBindVertexArray(VBO0); // Lier le VAO pour l'utiliser
	glEnableVertexAttribArray(0);


	//glGenBuffers(1, &VBO0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO0);
	glBufferData(GL_ARRAY_BUFFER, pointsControles3D.size() * 9 * sizeof(float), tabPoints, GL_STATIC_DRAW);
	//---

	// rendu indexe
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, pointsControles3D.size() * sizeof(GLushort), indi, GL_STATIC_DRAW);

	// le fait de specifier 0 comme BO desactive l'usage des BOs
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	ChangeCam(CamType);
	return true;
}

bool Initialize2()
{
	sumStatic = 0;
	initialized2 = true;
	//gridPoints3D = patches[0].gridPoints;
	//tmpPoints = new float[gridPoints3D.size() * 9];
	gridPoints3D.clear();
	std::vector<int> cptGridPoints;
	for (int i = 0; i < patches.size(); i++)
	{
		for (int j = 0; j < patches[i].gridPoints.size(); j++)
		{
			gridPoints3D.push_back(patches[i].gridPoints[j]);
		}

		cptGridPoints.push_back(sqrt(patches[i].gridPoints.size()) - 1);
		sumStatic += (sqrt(patches[i].gridPoints.size()) - 1)*(sqrt(patches[i].gridPoints.size()) - 1) * 4;
	}


	tmpPoints = new float[gridPoints3D.size() * 9];

	glewInit();
	g_BasicShader.LoadVertexShader("basic.vs");
	g_BasicShader.LoadFragmentShader("basic.fs");
	g_BasicShader.CreateProgram();
	

	indTmp = createIndForGridPoints(cptGridPoints);

	structToTabColor(gridPoints3D, col,tmpPoints);

	// Points controles VBO0
	glGenVertexArrays(1, &VBO0); // Créer le VAO
	glBindVertexArray(VBO0); // Lier le VAO pour l'utiliser
	glEnableVertexAttribArray(0);

	//glGenBuffers(1, &VBO0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO0);
	glBufferData(GL_ARRAY_BUFFER, pointsControles3D.size() * 9 * sizeof(float), tabPoints, GL_STATIC_DRAW);
	//---


	//---
	glGenVertexArrays(1, &VBO1); // Créer le VAO
	glBindVertexArray(VBO1); // Lier le VAO pour l'utiliser
	glEnableVertexAttribArray(1);

	
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, gridPoints3D.size() * 9 * sizeof(float), tmpPoints, GL_STATIC_DRAW);


	// rendu indexe
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, pointsControles3D.size() * sizeof(GLushort), indi, GL_STATIC_DRAW);

	glGenBuffers(1, &IBO1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sumStatic * sizeof(GLushort), indTmp, GL_STATIC_DRAW);

	// le fait de specifier 0 comme BO desactive l'usage des BOs
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return true;
}

void Terminate()
{
	glDeleteTextures(1, &TexObj);
	glDeleteBuffers(1, &IBO);
	glDeleteBuffers(1, &VBO0);
	glDeleteBuffers(1, &IBO1);
	glDeleteBuffers(1, &VBO1);
	g_BasicShader.DestroyProgram();
}

void update()
{
	
	
	if (initialized1)
	{
		
			pointsControles3D = transformPointsToCube(tmpPatch.controlPoints);
			structToTabColor(pointsControles3D, col, tabPoints);

			glBindBuffer(GL_ARRAY_BUFFER, VBO0);
			glBufferSubData(GL_ARRAY_BUFFER,0, pointsControles3D.size() * 9 * sizeof(float), tabPoints);
		
	}
	if (initialized2)
	{
		sumStatic = 0;
		std::vector<int> cptGridPoints;

		gridPoints3D.clear();
		for (int i = 0; i < patches.size(); i++)
		{
			for (int j = 0; j < patches[i].gridPoints.size(); j++)
			{
				gridPoints3D.push_back(patches[i].gridPoints[j]);
			}
			cptGridPoints.push_back(sqrt(patches[i].gridPoints.size()) - 1);
			sumStatic += (sqrt(patches[i].gridPoints.size()) - 1)*(sqrt(patches[i].gridPoints.size()) - 1)*4;
		}
		
		delete(tmpPoints);
		tmpPoints = new float[gridPoints3D.size()*9];


		indTmp = createIndForGridPoints(cptGridPoints);

		structToTabColor(gridPoints3D, col,tmpPoints);

		glBindBuffer(GL_ARRAY_BUFFER, VBO1);
		glBufferSubData(GL_ARRAY_BUFFER, 0, gridPoints3D.size() * 9 * sizeof(float), tmpPoints);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO0);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sumStatic * sizeof(GLushort), indTmp);
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glutPostRedisplay();
}

void animate()
{
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		// afin d'obtenir le deltatime actuel
		TimeSinceAppStartedInMS = glutGet(GLUT_ELAPSED_TIME);
		TimeInSeconds = TimeSinceAppStartedInMS / 1000.0f;
		DeltaTime = (TimeSinceAppStartedInMS - OldTime) / 1000.0f;
		OldTime = TimeSinceAppStartedInMS;

		glViewport(0, 0, width, height);
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
		//glColorMask(GL_TRUE, GL_FALSE, GL_TRUE, GL_TRUE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		auto program = g_BasicShader.GetProgram();
		glUseProgram(program);

		/*	uint32_t texUnit = 0;
		glActiveTexture(GL_TEXTURE0 + texUnit);
		glBindTexture(GL_TEXTURE_2D, TexObj);
		auto texture_location = glGetUniformLocation(program, "u_Texture");
		glUniform1i(texture_location, texUnit);
		*/
		// UNIFORMS
		Esgi::Mat4 worldMatrix;
		worldMatrix.MakeScale(1.0f, 1.0f, 1.0f);

		//  Camera Matrix
		Esgi::Mat4 cameraMatrix;
		switch (CamType)
		{
		case 0:	//FPS
			cameraMatrix = FPSCamera(posX, posY, posZ, rotX, rotY);
			break;
		case 1:	//Orbit
			cameraMatrix = OrbitCamera(posX, posY, posZ, distance, rotX, rotY);
			break;
		}

		//

		auto world_location = glGetUniformLocation(program, "u_WorldMatrix");
		glUniformMatrix4fv(world_location, 1, GL_FALSE, worldMatrix.m);

		Esgi::Mat4 projectionMatrix;
		float w = glutGet(GLUT_WINDOW_WIDTH), h = glutGet(GLUT_WINDOW_HEIGHT);
		// ProjectionMatrix
		float aspectRatio = w / h;			// facteur d'aspect
		float fovy = 45.0f;					// degree d'ouverture
		float nearZ = 0.1f;
		float farZ = 10000.0f;
		projectionMatrix.Perspective(fovy, aspectRatio, nearZ, farZ);

		//projectionMatrix.MakeScale(1.0f / (0.5f*w), 1.0f / (0.5f*h), 1.0f);

		auto projection_location = glGetUniformLocation(program, "u_ProjectionMatrix");
		glUniformMatrix4fv(projection_location, 1, GL_FALSE, projectionMatrix.m);

		auto camera_location = glGetUniformLocation(program, "u_CameraMatrix");
		glUniformMatrix4fv(camera_location, 1, GL_FALSE, cameraMatrix.m);

		auto time_location = glGetUniformLocation(program, "u_Time");
		glUniform1f(time_location, TimeInSeconds);

		/*auto c_location = glGetUniformLocation(program, "color");
		glUniform4fv(c_location, 1, colore);*/

		// ATTRIBUTES
		auto normal_location = glGetAttribLocation(program, "a_Normal");
		auto position_location = glGetAttribLocation(program, "a_Position");
		auto color_location = glGetAttribLocation(program, "a_Color");
		//auto texcoords_location = glGetAttribLocation(program, "a_TexCoords");
		//glVertexAttrib3f(color_location, 0.0f, 1.0f, 0.0f);

		// Le fait de specifier la ligne suivante va modifier le fonctionnement interne de glVertexAttribPointer
		// lorsque GL_ARRAY_BUFFER != 0 cela indique que les donnees sont stockees sur le GPU
		glBindBuffer(GL_ARRAY_BUFFER, VBO0);

		//glBindVertexArray(VAO);

		glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), reinterpret_cast<const void *>(0 * sizeof(float)));
		glVertexAttribPointer(normal_location, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), reinterpret_cast<const void *>(3 * sizeof(float)));
		glVertexAttribPointer(color_location, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), reinterpret_cast<const void *>(6 * sizeof(float)));
		// on interprete les 3 valeurs inconnues comme RGB alors que ce sont les normales
		//glVertexAttribPointer(texcoords_location, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<const void *>(6 * sizeof(float)));

		//glEnableVertexAttribArray(texcoords_location);
		glEnableVertexAttribArray(position_location);
		glEnableVertexAttribArray(normal_location);
		glEnableVertexAttribArray(color_location);
		//glEnableVertexAttribArray(texcoords_location);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glDrawElements(GL_QUADS, pointsControles3D.size(), GL_UNSIGNED_SHORT, nullptr);

		//-----------

		glBindBuffer(GL_ARRAY_BUFFER, VBO1);

		int sizeP = 9;
		glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, sizeP * sizeof(float), reinterpret_cast<const void *>(0 * sizeof(float)));
		glVertexAttribPointer(normal_location, 3, GL_FLOAT, GL_FALSE, sizeP * sizeof(float), reinterpret_cast<const void *>(3 * sizeof(float)));
		glVertexAttribPointer(color_location, 3, GL_FLOAT, GL_FALSE, sizeP * sizeof(float), reinterpret_cast<const void *>(6 * sizeof(float)));


		glEnableVertexAttribArray(position_location);
		glEnableVertexAttribArray(normal_location);
		glEnableVertexAttribArray(color_location);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO1);
		glDrawElements(GL_QUADS, sumStatic, GL_UNSIGNED_SHORT, nullptr);

		//----------------
		glDisableVertexAttribArray(position_location);
		glDisableVertexAttribArray(normal_location);
		glDisableVertexAttribArray(color_location);
		glUseProgram(0);


		//Repositionnement du curseur 
		//glutWarpPointer(width*0.5f, height*0.5f);
		glEnd();


		glutSwapBuffers();

}

int main(int argc, const char* argv[])
{


	// passe les parametres de la ligne de commande a glut
	glutInit(&argc, (char**)argv);
	// defini deux color buffers (un visible, un cache) RGBA
	// GLUT_DEPTH alloue egalement une zone mémoire pour le depth buffer
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	// positionne et dimensionne la fenetre
	glutInitWindowPosition(100, 10);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Transformation");
	gluOrtho2D(-400, 400, -400, 400);						// Repère 2D délimitant les abscisses et les ordonnées
															// creation de la fenetre ainsi que du contexte de rendu

	glClearColor(1.0, 1.0, 1.0, 0.5);
	glColor3f(1.0, 1.0, 1.0);			     	 // couleur: blanc
	glPointSize(2.0);
#ifdef FREEGLUT
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
#endif

#ifdef NO_GLEW
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)
		wglGetProcAddress("glVertexAttribPointer");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)
		wglGetProcAddress("glEnableVertexAttribArray");
	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)
		wglGetProcAddress("glDisableVertexAttribArray");
#else
	glewInit();
#endif
	
	glutIdleFunc(update);
	glutDisplayFunc(animate);

	glutPassiveMotionFunc(mouse);
	glutSpecialFunc(SpecialInput);
	glutKeyboardFunc(keyboard);
	CreateGlutMenu();

	glutMainLoop();

	Terminate();

	return 1;
}

GLushort* createInd(int n)
{
	GLushort* tmp = new GLushort[n];
	for (int i = 0; i < n ; i++)
	{
		tmp[i] = i;
	}
	return tmp;
}

GLushort* createIndForGridPoints()
{
	// precision * precision
	int cpt = 0;
	GLushort* tmp = new GLushort[patches.size()*(sqrt(gridPoints3D.size()) - 1)*(sqrt(gridPoints3D.size()) - 1) * 4];
	//int tmps = precision*precision * 4;
	int prec = (sqrt(gridPoints3D.size()) - 1);
	for (int l = 0; l < patches.size(); l++)
	{
		for (int i = 0; i < prec; i++)
		{
			int k = 0;
			for (int j = 0; j < prec; j++)
			{
				tmp[cpt] = l*(prec + 1)*(prec + 1)+i*(prec + 1) + j;
				tmp[cpt + 1] = l*(prec + 1)*(prec + 1) + i*(prec + 1) + j + 1;
				tmp[cpt + 2] = l*(prec + 1)*(prec + 1) + (i + 1)*(prec + 1) + j + 1;
				tmp[cpt + 3] = l*(prec + 1)*(prec + 1) + (i + 1)*(prec + 1) + j;
				cpt += 4;
			}

		}
	}
	return tmp;
}

GLushort* createIndForGridPoints(std::vector<int> tmpCptGP)
{
	// precision * precision
	int cpt = 0;
	GLushort* tmp = new GLushort[sumStatic];
	//int tmps = precision*precision * 4;
	//int prec = (sqrt(gridPoints3D.size()) - 1);
	int prec;
	for (int l = 0; l < patches.size(); l++)
	{
		prec = tmpCptGP[l];
		for (int i = 0; i < prec; i++)
		{
			int k = 0;
			for (int j = 0; j < prec; j++)
			{
				tmp[cpt] = l*(prec + 1)*(prec + 1) + i*(prec + 1) + j;
				tmp[cpt + 1] = l*(prec + 1)*(prec + 1) + i*(prec + 1) + j + 1;
				tmp[cpt + 2] = l*(prec + 1)*(prec + 1) + (i + 1)*(prec + 1) + j + 1;
				tmp[cpt + 3] = l*(prec + 1)*(prec + 1) + (i + 1)*(prec + 1) + j;
				cpt += 4;
			}

		}
	}
	return tmp;
}

