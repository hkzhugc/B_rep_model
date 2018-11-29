#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <map>
#include "HalfEdge.h"

using namespace std;
enum CMD
{
	MVFS = 0,
	MEV,
	MEF,
	KEMR,
	KFMRH,
};
class OpReader
{
public:
	OpReader(const char * fn){
		fopen_s(&fp, fn, "r");
	}
	~OpReader(){};
private:
	FILE * fp;
	CMD read_cmd();
	void read_name(char * name_buffer);
	B_rep::Vector3f read_pos();
	void do_cmd(CMD cmd);

	map<string, B_rep::Solid *> solids;
	map<string, B_rep::FaceIter> faces;
	map<string, B_rep::EdgeIter> edges;
	map<string, B_rep::VertexIter> vertices;
	map<string, B_rep::LoopIter> loops;

	void mvfs();
};

