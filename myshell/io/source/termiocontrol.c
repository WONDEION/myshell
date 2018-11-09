#include "termiocontrol.h"

struct termios nts, ots;

int intimode()
{
	int pid;

	if (tcgetattr(0, &ots) < 0) // 得到当前终端(0表示标准输入)的设置
	{
		return -1;
	}

	nts = ots;
	cfmakeraw(&nts); // 设置终端为Raw原始模式，该模式下所有的输入数据以字节为单位被处理
	//set bit
	//nts.c_lflag |= ISIG;
	nts.c_oflag |= ONLCR;
	nts.c_oflag |= OPOST;
	//************
	if (tcsetattr(0, TCSANOW, &nts) < 0) // 设置上更改之后的设置
	{
		return -1;
	}

	return 0;
}

int setmode()
{
	struct termios orgmode_st;
	cfmakeraw(&orgmode_st); // 设置终端为Raw原始模式，该模式下所有的输入数据以字节为单位被处理
	//set bit
	//orgmode_st.c_lflag |= ISIG;
	orgmode_st.c_oflag |= ONLCR;
	orgmode_st.c_oflag |= OPOST;
	//************
	if (tcsetattr(0, TCSANOW, &orgmode_st) < 0) // 设置上更改之后的设置
	{
		return -1;
	}

	return 0;
}

int back_mode()
{
	if (tcsetattr(0, TCSANOW, &ots) < 0) // 设置还原成老的模式
		return -1;
	return 0;
}

int set_run_termode()
{
	return back_mode();
}