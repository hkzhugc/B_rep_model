#include "HalfEdge.h"

namespace B_rep
{
	Solid * mvfs(Vector3f pos, VertexIter &new_vi, FaceIter &new_fi, LoopIter &new_li)
	{
		Solid * solid = new Solid();
		VertexIter vi = solid->newVertex(pos);
		//vi->pos = pos;
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
		VertexIter end_v = solid->newVertex(pos);
		//end_v->pos = pos;
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

		new_face->solid = solid;
		new_face->out_loop() = new_loop;
		new_loop->face() = new_face;

		l_he->edge() = ei;
		r_he->edge() = ei;
		ei->left_he() = l_he;
		ei->right_he() = r_he;
		r_he->vertex() = start_v;
		l_he->vertex() = end_v;
		l_he->twin() = r_he;
		r_he->twin() = l_he;
		printf("---pre_div---\n");
		divide_loop(loop, r_he);
		printf("---divide---\n");
		//reset loop->halfedge and new_loop->halfedge, make sure loop point to the bound half edges
		loop->SetHalfEdge(l_he, loop);
		new_loop->SetHalfEdge(r_he, new_loop);
		return new_face;
	}

	LoopIter kemr(HalfEdgeIter l_he, HalfEdgeIter r_he, LoopIter lp)
	{
		Solid * solid = lp->face()->solid;
		
		EdgeIter e = r_he->edge();

		FaceIter f = lp->face();
		LoopIter new_lp = solid->newLoop();
		f->AddLoop(new_lp);

		if (r_he->next() == l_he)
		{
			printf("only one edge, just delete\n");
			solid->deleteHalfedge(r_he);
			solid->deleteHalfedge(l_he);
		}
		else
		{
			printf("connect the halfedge\n");
			HalfEdgeIter l_he_pre = pre(l_he);
			HalfEdgeIter r_he_next = r_he->next();
			HalfEdgeIter r_he_pre = pre(r_he);
			HalfEdgeIter l_he_next = l_he->next();

			r_he_pre->next() = l_he_next;
			l_he_pre->next() = r_he_next;
			new_lp->SetHalfEdge(r_he_next, new_lp);
			lp->SetHalfEdge(r_he_pre, lp);
			
		}
		solid->deleteEdge(e);
		return new_lp;
	}

	LoopIter kemr(EdgeIter e, LoopIter lp)
	{
		return kemr(e->left_he(), e->right_he(), lp);
	}

	LoopIter kemr(VertexIter v1, VertexIter v2, LoopIter lp)
	{
		HalfEdgeIter he = lp->half_edge();
		bool isFound = false;
		do
		{
			if (he->vertex() == v1)
			{
				if (he->twin()->vertex() == v2)
				{
					isFound = true;
					break;
				}
			}
			he = he->next();
		} while (he != lp->half_edge());
		if (isFound) return kemr(he->twin(), he, lp);
		else 
		{
			printf("can't not found edge (v1, v2) in loop lp\n");
			return lp;
		}
	}

	LoopIter kfmrh(FaceIter loop_f, FaceIter deleted_f)
	{
		return loop_f->out_loop();
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
		printf("---find inner begin---\n");
		//assert(inner_begin_pre->next() == inner_begin);

		HalfEdgeIter inner_end = big_loop->half_edge();
		HalfEdgeIter inner_end_pre = pre(big_loop->half_edge());
		while (inner_end->vertex() != begin_v) { inner_end_pre = inner_end; inner_end = inner_end->next(); assert(inner_end_pre->next() == inner_end); }
		printf("---find inner end---\n");
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

	void Solid::RenderWireFrame()
	{
		glColor3f(0.0f, 1.0f, 1.0f);
		glEnable(GL_BLEND);
		glLineWidth(5.0f);
		printf("render wireframe, %d edges\n", _edges.size());
		int cnt = 0;
		for (auto e : _edges)
		{
			printf("%dth edge\n", ++cnt);
			glBegin(GL_LINES);
			VertexIter start_v, end_v;
			start_v = e.right_he()->vertex();
			end_v = e.left_he()->vertex();
			glVertex3f(start_v->pos.x, start_v->pos.y, start_v->pos.z);
			glVertex3f(end_v->pos.x, end_v->pos.y, end_v->pos.z);
			glEnd();
		}
		glFlush();
	}

	void Loop::RenderWireFrame()
	{
		glColor3f(0.0f, 1.0f, 1.0f);
		glEnable(GL_BLEND);
		glLineWidth(5.0f);
		glBegin(GL_LINE_LOOP);
		HalfEdgeIter _h = _half_edge;
		int cnt = 0;
		do
		{
			printf("%dth Points\n", ++cnt);
			Vector3f pos = _h->vertex()->pos;
			printf("v at : (%f, %f, %f)\n", pos.x, pos.y, pos.z);
			glVertex3f(pos.x, pos.y, pos.z);
			_h = _h->next();
		} while (_h != _half_edge);
		//glVertex3f(start_v->pos.x, start_v->pos.y, start_v->pos.z);
		glEnd();
		glFlush();
	}

	void Face::RenderWireFrame()
	{
		
		_out_loop->RenderWireFrame();
		for (auto lp : _loop_list)
		{
			lp->RenderWireFrame();
		}
	}

	Vector3f Face::GetNormal()
	{
		bool isFind[3] = {false, false, false};
		HalfEdgeIter h_max2 = out_loop()->half_edge();
		HalfEdgeIter idxFind[3] = { h_max2, h_max2, h_max2 };
		for (int i = 0; i < 3; ++i)
		{
			HalfEdgeIter h = out_loop()->half_edge();
			HalfEdgeIter max_h = h;
			Vector3f max = h->vertex()->pos;
			h = h->next();
			do
			{
				Vector3f p = h->vertex()->pos;
				if (p[i] > max[i])
				{
					/*
					printf("p[%d] = %f > max[%d] = %f ? %d\n", i, p[i], i, max[i], p[i] > max[i]);
					printf("%d dim max at : (%f, %f, %f)\n", i, max.x, max.y, max.z);
					printf("%d dim p at : (%f, %f, %f)\n", i, p.x, p.y, p.z);
					*/
					max = p;
					max_h = h;
					isFind[i] = true;
				}
				else if (p[i] < max[i])
				{
					isFind[i] = true;
				}
				h = h->next();
			} while (h != out_loop()->half_edge());
			if (isFind[i])
			{
				idxFind[i] = max_h;
			}
		}
		for (int i = 0; i < 3; ++i)
		{
			if (isFind[i])
			{
				h_max2 = idxFind[i];
				Vector3f max = h_max2->vertex()->pos;
				break;
			}
		}
		//max2 was a convex point
		Vector3f max2 = h_max2->vertex()->pos;
		

		Vector3f p0, p1, p2, normal;
		p0 = pre(h_max2)->vertex()->pos;
		p1 = h_max2->vertex()->pos;
		p2 = h_max2->next()->vertex()->pos;

		normal = glm::normalize(glm::cross(p1 - p0, p2 - p1));
		return normal;
	}

	void CALLBACK PolyLine3DBegin(GLenum type)
	{
		glBegin(type);
	}

	void CALLBACK PolyLine3DVertex(GLdouble * vertex)
	{
		const GLdouble *pointer = (GLdouble *)vertex;
		glColor3d(1.0, 0, 0); 
		glVertex3dv(pointer);
	}

	void CALLBACK PolyLine3DEnd()
	{
		glEnd();
	}
	GLUtesselator* tesser()
	{
		GLUtesselator * tess;
		tess = gluNewTess();
		gluTessCallback(tess, GLU_TESS_BEGIN, (void (CALLBACK*)())&PolyLine3DBegin);
		gluTessCallback(tess, GLU_TESS_VERTEX, (void (CALLBACK*)())&PolyLine3DVertex);
		gluTessCallback(tess, GLU_TESS_END, (void (CALLBACK*)())&PolyLine3DEnd);
		return tess;
	}

#define GLUTESSFACE 1
	void Face::RenderFace()
	{
		Vector3f normal = GetNormal();
		GLUtesselator *tobj = tesser();
		if (!tobj) { return; }
#if GLUTESSFACE
		gluTessBeginPolygon(tobj, NULL);
		gluTessBeginContour(tobj);
#else
		glColor3f(0.0f, 1.0f, 1.0f);
		glEnable(GL_BLEND);
		glLineWidth(5.0f);
		glBegin(GL_LINE_LOOP);
#endif
			HalfEdgeIter _h = out_loop()->half_edge();
			do
			{
				Vector3f pos = _h->vertex()->pos;
#if GLUTESSFACE
				gluTessVertex(tobj, _h->vertex()->gl_pos, _h->vertex()->gl_pos);
#else
				glVertex3f(_h->vertex()->gl_pos[0], _h->vertex()->gl_pos[1], _h->vertex()->gl_pos[2]);
#endif
				_h = _h->next();
			} while (_h != out_loop()->half_edge());
#if GLUTESSFACE
		gluTessEndContour(tobj);
#else
		glEnd();
#endif
		int lp_cnt = 0;
		for (auto lp : _loop_list)
		{
#if GLUTESSFACE
			gluTessBeginContour(tobj);
#else
			glColor3f(0.0f, 1.0f, 1.0f);
			glEnable(GL_BLEND);
			glLineWidth(5.0f);
			glBegin(GL_LINE_LOOP);
#endif
			HalfEdgeIter _h = lp->half_edge();
			do
			{
				Vector3f pos = _h->vertex()->pos;
#if GLUTESSFACE
				gluTessVertex(tobj, _h->vertex()->gl_pos, _h->vertex()->gl_pos);
#else
				glVertex3f(_h->vertex()->gl_pos[0], _h->vertex()->gl_pos[1], _h->vertex()->gl_pos[2]);
#endif
				_h = _h->next();
			} while (_h != lp->half_edge());
#if GLUTESSFACE
			gluTessEndContour(tobj);
#else
			glEnd();
#endif
		}
#if GLUTESSFACE
		gluTessEndPolygon(tobj);
		gluDeleteTess(tobj);
#endif
		glFlush();
	}
}


