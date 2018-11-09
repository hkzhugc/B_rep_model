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
		fi->out_loop() = li;

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
		l_he->loop() = loop;
		r_he->loop() = loop;
		r_he->vertex() = start_v;
		l_he->vertex() = end_v;
		l_he->twin() = r_he;
		r_he->twin() = l_he;

		ei->left_he() = l_he;
		ei->right_he() = r_he;

		loop->AddHalfEdge(r_he);
		return end_v;
	}

	FaceIter mef(VertexIter start_v, VertexIter end_v, LoopIter loop)
	{
		Solid * solid = loop->face()->solid;
		EdgeIter ei = solid->newEdge();
		HalfEdgeIter l_he = solid->newHalfedge();
		HalfEdgeIter r_he = solid->newHalfedge();
		FaceIter new_face = solid->newFace();
		LoopIter new_loop = solid->newLoop();

		new_face->out_loop() = new_loop;
		new_loop->face() = new_face;

		l_he->edge() = ei;
		r_he->edge() = ei;
		r_he->vertex() = start_v;
		l_he->vertex() = end_v;
		l_he->twin() = r_he;
		r_he->twin() = l_he;

		divide_loop(loop, r_he);

		//reset loop->halfedge and new_loop->halfedge, make sure loop point to the truely inner one
		loop->SetHalfEdge(r_he, loop);
		new_loop->SetHalfEdge(l_he, new_loop);
		return new_face;
	}


	HalfEdgeIter pre(HalfEdgeIter h)
	{
		HalfEdgeIter _h = h;
		while (_h->next() != h) _h = _h->next();
		return _h;
	}

	//return the new_loop, the new_loop point to the twin of the add_he
	LoopIter divide_loop(LoopIter big_loop, HalfEdgeIter add_he)
	{
		HalfEdgeIter add_he_twin = add_he->twin();
		VertexIter begin_v = add_he->vertex();
		VertexIter end_v = add_he_twin->vertex();

		HalfEdgeIter inner_begin = big_loop->half_edge();
		HalfEdgeIter inner_begin_pre = pre(big_loop->half_edge());
		while (inner_begin->vertex() != end_v) { inner_begin_pre = inner_begin; inner_begin = inner_begin->next(); assert(inner_begin_pre->next() == inner_begin); }
		//assert(inner_begin_pre->next() == inner_begin);

		HalfEdgeIter inner_end = big_loop->half_edge();
		HalfEdgeIter inner_end_pre = pre(big_loop->half_edge());
		while (inner_end->vertex() != begin_v) { inner_end_pre = inner_end; inner_end = inner_end->next(); assert(inner_end_pre->next() == inner_end); }
		//assert(inner_end_pre->next() == inner_end);
		add_he->next() = inner_begin;
		inner_begin_pre->next() = add_he_twin;
		
		add_he_twin->next() = inner_end;
		inner_end_pre->next() = add_he;
		printf("-------------------\n");
		return big_loop;
	}


	void Loop::AddHalfEdge(HalfEdgeIter new_he)
	{
		if (isEmpty)
		{
			_half_edge = new_he;
			_half_edge->next() = _half_edge->twin();
			_half_edge->twin()->next() = _half_edge;
		}
		else
		{
			HalfEdgeIter he = _half_edge;
			while (he->vertex() != new_he->vertex()) he = he->next();
			
			HalfEdgeIter he_pre = pre(he);

			new_he->next() = new_he->twin();
			new_he->twin()->next() = he;
			he_pre->next() = new_he;
		}
		isEmpty = false;
	}

	void Loop::SetHalfEdge(HalfEdgeIter he, LoopIter _this)
	{ 
		isEmpty = false; 
		_half_edge = he; 
		HalfEdgeIter _h = _half_edge;
		do
		{
			_h->loop() = _this;
			_h = _h->next();
		} while (_h != _half_edge);
	}
}


