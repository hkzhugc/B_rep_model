#include "HalfEdge.h"
#include <iostream>
#include <stdio.h>
using namespace B_rep;
int main()
{
	Vector3f pos1(0, 0, 1);
	LoopIter l1;
	FaceIter f1;
	VertexIter v1;
	Solid * s = mvfs(pos1, v1, f1, l1);
	Vector3f pos2(0, 0, 2);
	VertexIter v2 = mev(pos2, l1, v1);
	Vector3f pos3(0, 0, 3);
	VertexIter v3 = mev(pos3, l1, v2);
	HalfEdgeIter _h = l1->half_edge();
	int i = 0;
	do
	{
		Vector3f pos = _h->vertex()->pos;
		printf("v at : (%f, %f, %f)\n", pos.x, pos.y, pos.z);
		_h = _h->next();
	} while (i++ != 3);
	getchar();
}