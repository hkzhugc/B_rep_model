#include "OpReader.h"

CMD OpReader::read_cmd()
{
	char cmd[10];
	fscanf_s(fp, "%s", cmd);
	if (strcmp(cmd, "mev")) return MEV;
	else if (strcmp(cmd, "mef")) return MEF;
	else if (strcmp(cmd, "mvfs")) return MVFS;
	else if (strcmp(cmd, "kemr")) return KEMR;
	else if (strcmp(cmd, "kfmrh")) return KFMRH;
	else
	{
		printf("unKown CMD %s , exit\n", cmd);
		exit(-1);
	}
}

B_rep::Vector3f OpReader::read_pos()
{
	B_rep::Vector3f pos;
	fscanf_s(fp, "(%f,%f,%f)", &pos.x, &pos.y, &pos.z);
	return pos;
}

void OpReader::read_name(char * name_buffer)
{
	fscanf_s(fp, "%s)", name_buffer);
}

void OpReader::do_cmd(CMD cmd)
{
	switch (cmd)
	{
	case MVFS:
		char f[255], l[255], v[255];
		read_name(f); read_name(l); read_name(v);
		break;
	case MEV:
		break;
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
