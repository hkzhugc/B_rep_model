#pragma once
#include <gl/glut.h>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <vector>
#include <list>

using namespace std;

namespace B_rep
{
	class Vertex;
	class HalfEdge;
	class Edge;
	class Loop;
	class Face;
	class Solid;

	typedef glm::vec3 Vector3f;
	typedef   list<Vertex>::iterator   VertexIter;
	typedef     list<Edge>::iterator     EdgeIter;
	typedef     list<Face>::iterator     FaceIter;
	typedef     list<Loop>::iterator     LoopIter;
	typedef list<HalfEdge>::iterator HalfEdgeIter;

	inline bool operator<(const HalfEdgeIter& i, const HalfEdgeIter& j) {
		return &*i < &*j;
	}

	inline bool operator<(const VertexIter& i, const VertexIter& j) {
		return &*i < &*j;
	}

	inline bool operator<(const EdgeIter& i, const EdgeIter& j) {
		return &*i < &*j;
	}

	inline bool operator<(const FaceIter& i, const FaceIter& j) {
		return &*i < &*j;
	}

	inline bool operator<(const LoopIter& i, const LoopIter& j) {
		return &*i < &*j;
	}


	class Face
	{
	public:
		Face() { _loop_list.clear(); };
		~Face() {};
		void AddLoop(LoopIter li) { _loop_list.push_back(li); }
		LoopIter & out_loop() { return _out_loop; }
		Solid * solid;
		void RenderWireFrame();
		void RenderFace();
		Vector3f GetNormal();
	private:
		vector<LoopIter> _loop_list;
		LoopIter _out_loop;
	};

	class Loop
	{
	public:
		Loop() { _half_edge_num = 0; isEmpty = true; };
		~Loop() {};

		FaceIter & face() { return _face; }
		HalfEdgeIter & half_edge() { return _half_edge; }
		void AddHalfEdge(HalfEdgeIter new_he);
		void SetHalfEdge(HalfEdgeIter he, LoopIter _this);
		void RenderWireFrame();
	private:
		FaceIter _face;
		HalfEdgeIter _half_edge;
		int _half_edge_num;
		bool isEmpty;
	};

	class Edge
	{
	public:
		Edge() {};
		~Edge() {};

		HalfEdgeIter & left_he() { return _left_he; };
		HalfEdgeIter & right_he() { return _right_he; };
	private:
		HalfEdgeIter _left_he;
		HalfEdgeIter _right_he;//define the normal dir
	};

	class HalfEdge
	{
	public:
		HalfEdge() {};
		~HalfEdge() {};

		VertexIter & vertex() { return _vertex; }
		HalfEdgeIter &twin() { return _twin; }
		HalfEdgeIter &next() { return _next; }
		EdgeIter &edge() { return _edge; }
		LoopIter &loop() { return _loop; }
	private:
		HalfEdgeIter _twin; 
		HalfEdgeIter _next; 
		VertexIter _vertex; 
		EdgeIter _edge;
		LoopIter _loop;
	};

	class Vertex
	{
	public:
		Vertex() {};
		Vertex(Vector3f _pos) : pos(_pos){ for (int i = 0; i < 3; i++) gl_pos[i] = pos[i]; };
		~Vertex() {};
		Vector3f pos;
		GLdouble gl_pos[3];
	private:

		HalfEdgeIter _half_edge;
	};

	class Solid
	{
	public:
		Solid()
		{
			_faces.clear();
			_loops.clear();
			_edges.clear();
			_halfedges.clear();
			_vertices.clear();
			_face_list.clear();
		}
		~Solid() {};
		FaceIter & face() { return _faces.begin(); }
		HalfEdgeIter newHalfedge() { return  _halfedges.insert(_halfedges.end(), HalfEdge()); }
		VertexIter   newVertex() { return   _vertices.insert(_vertices.end(), Vertex()); }
		VertexIter   newVertex(Vector3f _pos) { return   _vertices.insert(_vertices.end(), Vertex(_pos)); }
		EdgeIter     newEdge() { return      _edges.insert(_edges.end(), Edge()); }
		LoopIter     newLoop() { return      _loops.insert(_loops.end(), Loop()); }
		FaceIter     newFace() { return      _faces.insert(_faces.end(), Face()); }
		void deleteHalfedge(HalfEdgeIter h) { _halfedges.erase(h); }
		void deleteFace(FaceIter f) { _faces.erase(f); }
		void deleteLoop(LoopIter lp) { _loops.erase(lp); }
		void deleteVertex(VertexIter v) { _vertices.erase(v); }
		void deleteEdge(EdgeIter e) { _edges.erase(e); }

		void AddFace(FaceIter fi) { _face_list.push_back(fi); }
		void RenderWireFrame();
	private:
		list<Face> _faces;
		list<Loop> _loops;
		list<Edge> _edges;
		list<HalfEdge> _halfedges;
		list<Vertex> _vertices;
		vector<FaceIter> _face_list;
	};

	LoopIter divide_loop(LoopIter big_loop, HalfEdgeIter add_he);//a helper func for mef
	HalfEdgeIter pre(HalfEdgeIter h);

	Solid * mvfs(Vector3f pos, VertexIter &new_vi, FaceIter &new_fi, LoopIter &new_li);
	VertexIter  mev(Vector3f pos, LoopIter loop, VertexIter start_v);
	FaceIter mef(VertexIter start_v, VertexIter end_v, LoopIter loop);
	LoopIter kemr(HalfEdgeIter l_he, HalfEdgeIter r_he, LoopIter lp);
	LoopIter kemr(EdgeIter e, LoopIter lp);
	LoopIter kemr(VertexIter v1, VertexIter v2, LoopIter lp);
	LoopIter kfmrh(FaceIter loop_f, FaceIter deleted_f);
}


