#pragma once
#include <gl/glut.h>
#include <glm/vec3.hpp>
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
	class Face
	{
	public:
		Face() { _loop_list.clear(); };
		~Face() {};
		void AddLoop(LoopIter li) { _loop_list.push_back(li); }
		Solid * solid;
	private:
		vector<LoopIter> _loop_list;
	};

	class Loop
	{
	public:
		Loop() { _half_edge_num = 0; };
		~Loop() {};

		FaceIter & face() { return _face; }
		HalfEdgeIter & half_edge() { return _half_edge; }
		void AddHalfEdge(HalfEdgeIter new_he);
	private:
		FaceIter _face;
		HalfEdgeIter _half_edge;
		int _half_edge_num;
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
	private:
		HalfEdgeIter _twin; 
		HalfEdgeIter _next; 
		VertexIter _vertex; 
		EdgeIter _edge; 
	};

	class Vertex
	{
	public:
		Vertex() {};
		Vertex(Vector3f _pos) : pos(_pos){};
		~Vertex() {};
		Vector3f pos;
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
		EdgeIter     newEdge() { return      _edges.insert(_edges.end(), Edge()); }
		LoopIter     newLoop() { return      _loops.insert(_loops.end(), Loop()); }
		FaceIter     newFace() { return      _faces.insert(_faces.end(), Face()); }
		void AddFace(FaceIter fi) { _face_list.push_back(fi); }
	private:
		list<Face> _faces;
		list<Loop> _loops;
		list<Edge> _edges;
		list<HalfEdge> _halfedges;
		list<Vertex> _vertices;
		vector<FaceIter> _face_list;
	};

	Solid * mvfs(Vector3f pos, VertexIter &new_vi, FaceIter &new_fi, LoopIter &new_li);
	VertexIter  mev(Vector3f pos, LoopIter loop, VertexIter start_v);
}


