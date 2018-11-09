#include "HalfEdge.h"

namespace B_rep
{
	Solid * mvfs(Vector3f pos, VertexIter &new_vi, FaceIter &new_fi, LoopIter &new_li)
	{
		Solid * solid = new Solid();
		VertexIter vi = solid->newVertex();
		vi->pos = pos;
		FaceIter fi = solid->newFace();
		fi->solid = solid;
		solid->AddFace(fi);
		LoopIter li = solid->newLoop();
		li->face() = fi;
		fi->AddLoop(li);

		new_vi = vi;
		new_fi = fi;
		new_li = li;
		return solid;
	}
	VertexIter  mev(Vector3f pos, LoopIter loop, VertexIter start_v)
	{
		Solid * solid = loop->face()->solid;
		VertexIter end_v = solid->newVertex();
		end_v->pos = pos;
		EdgeIter ei = solid->newEdge();
		HalfEdgeIter l_he = solid->newHalfedge();
		HalfEdgeIter r_he = solid->newHalfedge();

		l_he->edge() = ei;
		r_he->edge() = ei;
		r_he->vertex() = start_v;
		l_he->vertex() = end_v;
		l_he->twin() = r_he;
		r_he->twin() = l_he;

		ei->left_he() = l_he;
		ei->right_he() = r_he;

		loop->AddHalfEdge(r_he);
		return end_v;
	}
	void Loop::AddHalfEdge(HalfEdgeIter new_he)
	{
		if (_half_edge_num == 0)
		{
			_half_edge = new_he;
			_half_edge->next() = _half_edge->twin();
			_half_edge->twin()->next() = _half_edge;
		}
		else
		{
			HalfEdgeIter he = _half_edge;
			while (he->vertex() != new_he->vertex()) he = he->next();
			/*{
				printf("enter new half edge\n");
				Vector3f pos = new_he->vertex()->pos;
				printf("v at : (%f, %f, %f)\n", pos.x, pos.y, pos.z);
				pos = new_he->twin()->vertex()->pos;
				printf("v at : (%f, %f, %f)\n", pos.x, pos.y, pos.z);
			}*/
			new_he->next() = new_he->twin();
			new_he->twin()->next() = he;
			he->twin()->next() = new_he;

			/*{
				printf("old half edge\n");
				Vector3f pos = he->next()->vertex()->pos;
				printf("v at : (%f, %f, %f)\n", pos.x, pos.y, pos.z);
				pos = he->next()->next()->twin()->vertex()->pos;
				printf("halfedge equal is %d\n", (new_he->twin() == he->next()->next()->twin()));
				printf("v at : (%f, %f, %f)\n", pos.x, pos.y, pos.z);
				pos = he->next()->next()->next()->vertex()->pos;
				printf("v at : (%f, %f, %f)\n", pos.x, pos.y, pos.z);
			}*/
		}
		_half_edge_num++;
	}
}


