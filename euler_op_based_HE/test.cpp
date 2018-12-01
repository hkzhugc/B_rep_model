#include "HalfEdge.h"
#include <iostream>
#include <stdio.h>
#include <map>
#include "OpReader.h"
using namespace B_rep;
using namespace std;
Solid * s;

LoopIter drawlp;

FaceIter drawf;

float angley = 2.1;
float anglex = 2.1;

//a function for init some feature in gl
void init()
{
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(400, 400);
	glutCreateWindow("render my solid");

	glClearColor(1.0, 1.0, 1.0, 0.0);
	glShadeModel(GL_SMOOTH);

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);

	
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);

	GLfloat AmbientLight[4] = { 1, 1, 1, 1 };
	GLfloat light_position[] = { 5, 50, 50, 1 };
	GLfloat mat_diffuse[] = { 0.85f, 0.65f, 0.2f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientLight);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	
}

void init2()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	glEnable(GL_DEPTH_TEST);  //¿ªÆôÉî¶È²âÊÔ
	glEnable(GL_LIGHTING); //¿ªÆô¹âÕÕÄ£Ê½
	GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 }; // ¶¨ÒåÑÕÉ«
	GLfloat light_pos[] = { 0, 50, 30, 1 };  //¶¨Òå¹âÔ´Î»ÖÃ

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos); //ÉèÖÃµÚ0ºÅ¹âÔ´µÄ¹âÕÕÎ»ÖÃ
	glLightfv(GL_LIGHT0, GL_AMBIENT, white); //ÉèÖÃµÚ0ºÅ¹âÔ´¶à´Î·´ÉäºóµÄ¹âÕÕÑÕÉ«£¨»·¾³¹âÑÕÉ«£©
	glEnable(GL_LIGHT0); //¿ªÆôµÚ0ºÅ¹âÔ´

	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	GLfloat mat_diffuse0[] = { 0.85f, 0.65f, 0.2f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse0);

}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);    
	glMatrixMode(GL_PROJECTION);  
	glLoadIdentity();
	gluPerspective(60, 1, 0, 20);  
	glClearColor(1.0, 1.0, 1.0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void reshape2(GLsizei w, GLsizei h)
{
	GLfloat fovy = 45;
	GLfloat aspect = 1;
	if (0 == h) { h = 1; }
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	fovy = 45;
	aspect = (GLfloat)w / h;
	if (w < h)
	{
		fovy = fovy * (GLfloat)h / w;
	}
	gluPerspective(fovy, aspect, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void draw_scene()
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
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	//gluLookAt(-2, -2, -10, 0, 0, 0, 0, 1, 0);
	glTranslatef(0, 0, -10);
	glRotatef(angley, 0, 1, 0);
	glRotatef(anglex, 1, 0, 0);
	draw_scene();
	
	
	glEnable(GL_LIGHT0);
	
	glEnable(GL_LIGHTING);
	//s->RenderWireFrame();
	s->RenderFace();
	//glDisable(GL_LIGHTING);
	glPopMatrix();
	glFlush();
}

void keyboard(GLubyte key, GLint x, GLint y)
{
	switch (key) {

	case 'a':
		angley -= 1.7;
		break;
	case 'd':
		angley += 1.7;
		break;
	case 'w':
		anglex -= 1.7;
		break;
	case 's':
		anglex += 1.7;
		break;
	case 27:
		exit(0);
		break;
	default:
		break;
	}

	glutPostRedisplay();
}


int main(int argc, char** argv)
{
	OpReader op_read("D:/CAD/B_rep_model/Release/euler_op.txt");
	op_read.start_build();
	getchar();
	Vector3f pos1(0, 0, 0);
	LoopIter l1;
	FaceIter f1;
	VertexIter v1;
	s = mvfs(pos1, v1, f1, l1);
	printf("face size is = %d\n", s->face_list().size());
	Vector3f pos2(0, 3, 0);
	VertexIter v2 = mev(pos2, l1, v1);
	Vector3f pos3(4, 0, 0);
	VertexIter v3 = mev(pos3, l1, v2);
	
	printf("f1 loop is l1 : %d\n", f1->out_loop() == l1);
	FaceIter f2 = mef(v3, v1, l1);
	printf("face size is = %d\n", s->face_list().size());
	printf("--mef---\n");
	HalfEdgeIter _h = f2->out_loop()->half_edge();
	do
	{
		Vector3f pos = _h->vertex()->pos;
		printf("v at : (%f, %f, %f)\n", pos.x, pos.y, pos.z);
		_h = _h->next();
	} while (_h != f2->out_loop()->half_edge());
	printf("-------------------\n");
	
	Vector3f pos4(1, 1, 0);
	Vector3f pos5(2, 1, 0);
	Vector3f pos6(1, 2, 0);
	
	
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
	sweep(drawf, Vector3f(0, 0, 1), 1);
	kfmrh(f1, f3);
	glutInit(&argc, argv);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape2);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}