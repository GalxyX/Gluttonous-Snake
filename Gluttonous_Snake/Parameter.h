#pragma once
#include <graphics.h>
//修改
const int SIDE_LENGTH = 15;//蛇身体每一节直径，即单位正方形的长宽
const int SHOW_XBLOCK = 48;//场地宽的正方形数
const int SHOW_YBLOCK = 48;//场地高的正方形数
const int BOUNDARY = 2;//边界宽度
const COLORREF SCORE_COLOR = RGB(225, 1231, 186);
const int INFO_HEIGHT_LINE = 30;//每行信息高度
const int INFO_LINE_NUM = 4;//信息行数
const COLORREF BK_COLOR = RGB(225, 255, 255);//菜单背景色
const int PAUSE_SIDE_LENGTH = INFO_HEIGHT_LINE * 3 / 4;//暂停键边长
const int PAUSE_SIDE_LENGTH_GETBIG = PAUSE_SIDE_LENGTH * 5 / 4;//暂停键变大边长
const int QUIT_LENGTH = PAUSE_SIDE_LENGTH * 3;//退出键宽度
const int QUIT_HEIGHT = PAUSE_SIDE_LENGTH * 3 / 2;//退出键高度
const int QUIT_LENGTH_GETBIG = QUIT_LENGTH * 5 / 4;//退出键变大宽度
const int QUIT_HEIGHT_GETBIG = QUIT_HEIGHT * 4 / 3;//退出键变大高度


//地图大小选择标记
const char ORDI_MAP = 0;
const char BIG_MAP = 1;

//蛇的颜色（皮肤）选择标记
const char BLUE_S = 1;
const char YELLOW_S = 2;
const char GREEN_S = 3;
const char MAX_NAME_LENGTH = 16;//含尾0，最长名字

//单人模式难度选择返回值
const char LEVEL1 = 1;
const char LEVEL2 = 2;
const char LEVEL3 = 3;

//主菜单返回值
const char ONE_PLAYER = 0;
const char AI_PLAYER = 4;
const char TWO_PLAYER = 5;
const char RANK = 6;
const char HELP = 7;

//不修改
const int SHOW_WIDTH = SIDE_LENGTH * SHOW_XBLOCK + 2 * BOUNDARY + 1;//场地宽像素//easyx少显示一排像素
const int SHOW_HEIGHT = SIDE_LENGTH * SHOW_YBLOCK + 2 * BOUNDARY + 1;//场地高像素//easyx少显示一排像素
const int WINDOW_HEIGHT = SHOW_HEIGHT + INFO_HEIGHT_LINE * INFO_LINE_NUM;//总窗口高度
class coordinate {
public:
	int x;
	int y;
	bool operator==(const coordinate& t)
	{
		if (x == t.x && y == t.y)
			return true;
		else
			return false;
	}
	bool operator!=(const coordinate& t)
	{
		if (x != t.x || y != t.y)
			return true;
		else
			return false;
	}
};
const coordinate SMALL_QUIT = { SHOW_WIDTH - PAUSE_SIDE_LENGTH_GETBIG - 10 - (QUIT_LENGTH_GETBIG + QUIT_LENGTH) / 2,0 };
const coordinate BIG_QUIT = { SHOW_WIDTH - PAUSE_SIDE_LENGTH_GETBIG - 10 - QUIT_LENGTH_GETBIG,0 };

