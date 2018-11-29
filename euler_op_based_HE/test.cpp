#include "HalfEdge.h"
#include <iostream>
#include <stdio.h>
#include <map>
using namespace B_rep;
using namespace std;
Solid * s;

LoopIter drawlp;

FaceIter drawf;



//a function for init some feature in gl
void init()
{
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(400, 400);
	glutCreateWindow("render my solid");

	glEnable(GL_DEPTH);
	glClearColor(1.0, 1.0, 1.0, 0);
	glShadeModel(GL_FLAT);

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glColor3f(0.0, 0.0, 1.0);  
	glLoadIdentity();  
	gluLookAt(5, 5, 5, 0, 0, 0, 0, 1, 0);
	glTranslatef(0, 0, 0);
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);    
	glMatrixMode(GL_PROJECTION);  
	glLoadIdentity();
	gluPerspective(60, 1, 0, 20);  
	//glFrustum(-1,1,-1,1,1.5,20.0); 
	glMatrixMode(GL_MODELVIEW);
}

void display()
{
	glColor3f(0, 0, 1);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1000);
	glEnd();

	glColor3f(0, 1, 0);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1000, 0);
	glEnd();

	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(1000, 0, 0);
	glEnd();
	//glScalef(0.5, 0.5, 0.5);
	//s->RenderWireFrame();
	//drawf->RenderWireFrame();
	drawf->RenderFace();
	//drawf->GetNormal();
	glFlush();
}

int main(int argc, char** argv)
{
	Vector3f pos1(0, 0, 0);
	LoopIter l1;
	FaceIter f1;
	VertexIter v1;
	s = mvfs(pos1, v1, f1, l1);
	Vector3f pos2(0, 0, 3);
	VertexIter v2 = mev(pos2, l1, v1);
	Vector3f pos3(0, 4, 0);
	VertexIter v3 = mev(pos3, l1, v2);
	
	printf("f1 loop is l1 : %d\n", f1->out_loop() == l1);
	FaceIter f2 = mef(v3, v1, l1);
	printf("--mef---\n");
	HalfEdgeIter _h = f2->out_loop()->half_edge();
	do
	{
		Vector3f pos = _h->vertex()->pos;
		printf("v at : (%f, %f, %f)\n", pos.x, pos.y, pos.z);
		_h = _h->next();
	} while (_h != f2->out_loop()->half_edge());
	printf("-------------------\n");
	
	Vector3f pos4(0, 1, 1);
	Vector3f pos5(0, 2, 1);
	Vector3f pos6(0, 1, 2);
	
	VertexIter v4 = mev(pos4, f2->out_loop(), v1);
	printf("--mev---\n");
	_h = f2->out_loop()->half_edge();
	do
	{
		Vector3f pos = _h->vertex()->pos;
		printf("v at : (%f, %f, %f)\n", pos.x, pos.y, pos.z);
		_h = _h->next();
	} while (_h != f2->out_loop()->half_edge());
	printf("-------------------\n");
	VertexIter v5 = mev(pos5, f2->out_loop(), v4);
	printf("--mev---\n");
	VertexIter v6 = mev(pos6, f2->out_loop(), v5);
	printf("--mev---\n");
	FaceIter f3 = mef(v6, v4, f2->out_loop());
	printf("--mef---\n");
	_h = f2->out_loop()->half_edge();
	do
	{
		Vector3f pos = _h->vertex()->pos;
		printf("v at : (%f, %f, %f)\n", pos.x, pos.y, pos.z);
		_h = _h->next();
	} while (_h != f2->out_loop()->half_edge());
	printf("-------------------\n");

	_h = f3->out_loop()->half_edge();
	do
	{
		Vector3f pos = _h->vertex()->pos;
		printf("v at : (%f, %f, %f)\n", pos.x, pos.y, pos.z);
		_h = _h->next();
	} while (_h != f3->out_loop()->half_edge());
	printf("-------------------\n");
	kemr(v1, v4, f2->out_loop());
	printf("---kemr---\n");
	getchar();
	drawlp = f2->out_loop();
	drawf = f2;
	glutInit(&argc, argv);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}