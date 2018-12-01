#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <map>
#include "HalfEdge.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
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
	OpReader(const char * fn) : file(fn){
		fp = NULL;
		fopen_s(&fp, fn, "r");
		if (!fp)
		{
			printf("cannot open file %s\n", fn);
		}
	}
	~OpReader(){};
	void start_build();
private:
	FILE * fp;
	ifstream file;
	CMD read_cmd(string s, int &pos);
	void read_name(char * name_buffer);
	B_rep::Vector3f read_pos(string line);
	void do_cmd(CMD cmd, string line, int pos);

	map<string, B_rep::Solid *> solids;
	map<string, B_rep::FaceIter> faces;
	map<string, B_rep::EdgeIter> edges;
	map<string, B_rep::VertexIter> vertices;
	map<string, B_rep::LoopIter> loops;

	void mvfs();
};

