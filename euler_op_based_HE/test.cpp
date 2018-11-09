#include "HalfEdge.h"
#include <iostream>
#include <stdio.h>
using namespace B_rep;
int main()
{
	Vector3f pos1(0, 0, 0);
	LoopIter l1;
	FaceIter f1;
	VertexIter v1;
	Solid * s = mvfs(pos1, v1, f1, l1);
	Vector3f pos2(0, 0, 3);
	VertexIter v2 = mev(pos2, l1, v1);
	Vector3f pos3(0, 4, 0);
	VertexIter v3 = mev(pos3, l1, v2);
	
	printf("f1 loop is l1 : %d\n", f1->out_loop() == l1);
	FaceIter f2 = mef(v3, v1, l1);
	
	HalfEdgeIter _h = f1->out_loop()->half_edge();
	do
	{
		Vector3f pos = _h->vertex()->pos;
		printf("v at : (%f, %f, %f)\n", pos.x, pos.y, pos.z);
		_h = _h->next();
	} while (_h != f1->out_loop()->half_edge());
	printf("-------------------\n");

	Vector3f pos4(0, 1, 1);
	Vector3f pos5(0, 2, 1);
	Vector3f pos6(0, 1, 2);

	VertexIter v4 = mev(pos4, l1, v1);

	_h = f1->out_loop()->half_edge();
	do
	{
		Vector3f pos = _h->vertex()->pos;
		printf("v at : (%f, %f, %f)\n", pos.x, pos.y, pos.z);
		_h = _h->next();
	} while (_h != f1->out_loop()->half_edge());
	printf("-------------------\n");

	VertexIter v5 = mev(pos5, l1, v4);
	VertexIter v6 = mev(pos6, l1, v5);

	FaceIter f3 = mef(v6, v4, l1);

	_h = l1->half_edge();
	do
	{
		Vector3f pos = _h->vertex()->pos;
		printf("v at : (%f, %f, %f)\n", pos.x, pos.y, pos.z);
		_h = _h->next();
	} while (_h != l1->half_edge());
	printf("-------------------\n");

	_h = f3->out_loop()->half_edge();
	do
	{
		Vector3f pos = _h->vertex()->pos;
		printf("v at : (%f, %f, %f)\n", pos.x, pos.y, pos.z);
		_h = _h->next();
	} while (_h != f3->out_loop()->half_edge());
	printf("-------------------\n");
	getchar();
}