#include "OpReader.h"

using namespace std;
CMD OpReader::read_cmd(string s, int &pos)
{
	pos = s.find_first_of(' ');
	cout << "read cmd:" << s.substr(pos) << endl;
	if (s.substr(0, pos) == "mev") return MEV;
	else if (s.substr(0, pos) == "mef") return MEF;
	else if (s.substr(0, pos) == "mvfs") return MVFS;
	else if (s.substr(0, pos) == "kemr") return KEMR;
	else if (s.substr(0, pos) == "kfmrh") return KFMRH;
	else
	{
		cout << "unKown CMD " << s.substr(0, pos) << "exit -1"<< endl;
		getchar();
		exit(-1);
	}
}

B_rep::Vector3f OpReader::read_pos(string line)
{
	int left_pos = line.find_first_of('(');
	int space_pos = line.find_first_of(' ', left_pos + 1);
	B_rep::Vector3f pos;
	//cout << "read pos.x : " << line.substr(left_pos + 1, space_pos) << endl;
	istringstream(line.substr(left_pos + 1, space_pos)) >> pos.x;
	//cout << "read pos.x : " << pos.x << endl;
	left_pos = space_pos;
	space_pos = line.find_first_of(' ', left_pos + 1);
	//cout << "read pos.y : " << line.substr(left_pos + 1, space_pos) << endl;
	istringstream(line.substr(left_pos + 1, space_pos)) >> pos.y;
	//cout << "read pos.y : " << pos.y << endl;
	left_pos = space_pos;
	space_pos = line.find_first_of(' ', left_pos + 1);
	//cout << "read pos.z : " << line.substr(left_pos + 1, space_pos) << endl;
	istringstream(line.substr(left_pos + 1, space_pos)) >> pos.z;
	//cout << "read pos.z : " << pos.z << endl;
	return pos;
}

void OpReader::read_name(char * name_buffer)
{
	fscanf_s(fp, "%s", name_buffer);
	printf("read name %s\n", name_buffer);
	getchar();
}

void OpReader::start_build()
{
	if (!file.is_open())
	{
		cout << "cannot openfile" << endl;
	}
	string line;
	while (getline(file, line))
	{
		cout << line << endl;
		int pos1;
		CMD cmd = read_cmd(line, pos1);
		B_rep::Vector3f pos = read_pos(line);
		printf("pos %f %f %f\n", pos.x, pos.y, pos.z);
		//do_cmd(cmd, line);
	}
	return;
	while (!feof(fp))
	{
		//CMD cmd = read_cmd();
		//printf("read cmd %d\n", cmd);
		//do_cmd(cmd);
	}
}

void OpReader::do_cmd(CMD cmd, string line, int str_pos)
{
	switch (cmd)
	{
	case MVFS:
	{
		printf("mvfs:\n");
		char f[255], l[255], v[255], s[255];
		read_name(s);  
		read_name(f); 
		read_name(l); 
		read_name(v);
		B_rep::Solid * sp;
		B_rep::FaceIter fi;
		B_rep::LoopIter li;
		B_rep::VertexIter vi;
		//B_rep::Vector3f pos = read_pos();
		//printf("MVFS %s %s %s %s (%f, %f, %f)\n", s, f, l, v, pos.x, pos.y, pos.z);
		//sp = B_rep::mvfs(pos, vi, fi, li);
		solids[string(s)] = sp;
		vertices[string(v)] = vi;
		faces[string(f)] = fi;
		loops[string(l)] = li;
		break;
	}
	case MEV:
	{
		
		char start_v[255], end_v[255], added_loop[255];
		//B_rep::Vector3f pos = read_pos();
		read_name(end_v);
		read_name(start_v); read_name(added_loop);
		//printf("MEV (%f %f %f) %s %s %s\n", pos.x, pos.y, pos.z, end_v, start_v, added_loop);
		B_rep::VertexIter end_vi, start_vi = vertices[string(start_v)];
		B_rep::LoopIter added_loopi = loops[string(added_loop)];
		//end_vi = B_rep::mev(pos, added_loopi, start_vi);
		vertices[string(end_v)] = end_vi;
		break;
	}
	case MEF:
		break;
	case KEMR:
		break;
	case KFMRH:
		break;
	default:
		break;
	}
}
