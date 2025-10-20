#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <conio.h>
#include "Parameter.h"
using namespace std;
const int small_BUTTON_WIDTH = SHOW_HEIGHT / 4;//较大按钮原始宽度
const int small_BUTTON_HEIGHT = WINDOW_HEIGHT / 7;//较大按钮原始高度
const int small_MINI_BUTTON_WIDTH = small_BUTTON_WIDTH * 4 / 9;//较小按钮原始宽度
const int small_MINI_BUTTON_HEIGHT = small_MINI_BUTTON_WIDTH * 5 / 7;//较小按钮原始高度
const int BUTTON_SIDE = WINDOW_HEIGHT / 10;//按钮到上下边界距离
const int BUTTON_GAP = (WINDOW_HEIGHT - small_BUTTON_HEIGHT * 4 - BUTTON_SIDE * 2) / 3;//按钮之间距离
const coordinate small_BUTTON[5] = { {SHOW_WIDTH * 11 / 12 - small_BUTTON_WIDTH, BUTTON_SIDE},
	{SHOW_WIDTH * 11 / 12 - small_BUTTON_WIDTH, BUTTON_SIDE + (small_BUTTON_HEIGHT + BUTTON_GAP)},
	{SHOW_WIDTH * 11 / 12 - small_BUTTON_WIDTH, BUTTON_SIDE + 2 * (small_BUTTON_HEIGHT + BUTTON_GAP)},
	{SHOW_WIDTH * 11 / 12 - small_BUTTON_WIDTH, BUTTON_SIDE + 3 * (small_BUTTON_HEIGHT + BUTTON_GAP)},
	{SHOW_WIDTH * 11 / 12 - small_MINI_BUTTON_WIDTH, BUTTON_SIDE + 3 * (small_BUTTON_HEIGHT + BUTTON_GAP)},
};
const int big_BUTTON_WIDTH = small_BUTTON_WIDTH + BUTTON_GAP * 2 / 3;
const int big_BUTTON_HEIGHT = small_BUTTON_HEIGHT + BUTTON_GAP * 2 / 3;
const int big_MINI_BUTTON_WIDTH = small_MINI_BUTTON_WIDTH + BUTTON_GAP / 3;
const int big_MINI_BUTTON_HEIGHT = small_MINI_BUTTON_HEIGHT + BUTTON_GAP / 3;
const coordinate big_BUTTON[5] = { {small_BUTTON[0].x - BUTTON_GAP / 3, small_BUTTON[0].y - BUTTON_GAP / 3},
	{small_BUTTON[1].x - BUTTON_GAP / 3, small_BUTTON[1].y - BUTTON_GAP / 3},
	{small_BUTTON[2].x - BUTTON_GAP / 3, small_BUTTON[2].y - BUTTON_GAP / 3},
	{small_BUTTON[3].x - BUTTON_GAP / 6, small_BUTTON[3].y - BUTTON_GAP / 6},
	{small_BUTTON[4].x - BUTTON_GAP / 6, small_BUTTON[4].y - BUTTON_GAP / 6}
};
const COLORREF BUTTON_COLOR[5] = { RGB(255, 231, 186) ,RGB(255, 231, 186) ,RGB(255, 231, 186) ,RGB(245, 222, 179) ,RGB(245, 222, 179) };
void DrawRoundrect(coordinate XL, int width, int height, COLORREF color, const TCHAR text[])
{//左上角位置坐标，总宽度，总高度，圆角半径，颜色
	setbkmode(TRANSPARENT);
	setfillcolor(color);
	setlinestyle(PS_SOLID, 2);
	fillroundrect(XL.x, XL.y, XL.x + width, XL.y + height, width / 10, height / 10);//圆角矩形左部 x 坐标;圆角矩形顶部 y 坐标; 圆角矩形右部 x 坐标; 圆角矩形底部 y 坐标;构成圆角矩形的圆角的椭圆的宽度; 构成圆角矩形的圆角的椭圆的高度。
	const LOGFONT chara = { height / 3,0,0,0,FW_BLACK,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,DEFAULT_PITCH, _T("幼圆") };//字体高度与位置不随长宽比例变化
	settextstyle(&chara);
	settextcolor(WHITE);
	int tx = XL.x + (width - textwidth(text)) / 2;
	int ty = XL.y + (height - textheight(text)) / 2;
	outtextxy(tx, ty, text);
	const LOGFONT chara1 = { INFO_HEIGHT_LINE * 2 / 3,0,0,0,FW_BLACK,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,DEFAULT_PITCH, _T("Comic Sans MS") };//字体高度与位置不随长宽比例变化
	settextstyle(&chara1);
	settextcolor(BLACK);
	setfillcolor(BK_COLOR);
}
int main_menu()
{//返回值1代表单人模式，2代表人机对战，3代表双人对战，4代表多人联机对战，5代表
	setbkcolor(BK_COLOR);
	cleardevice();//背景填充
	IMAGE bkimg;
	loadimage(&bkimg, _T("./image/menu_bk.png"));
	putimage(0, 0, &bkimg);

	//打印所有基础按钮
	DrawRoundrect(small_BUTTON[0], small_BUTTON_WIDTH, small_BUTTON_HEIGHT, BUTTON_COLOR[0], _T("单人模式"));
	DrawRoundrect(small_BUTTON[1], small_BUTTON_WIDTH, small_BUTTON_HEIGHT, BUTTON_COLOR[1], _T("人机对战"));
	DrawRoundrect(small_BUTTON[2], small_BUTTON_WIDTH, small_BUTTON_HEIGHT, BUTTON_COLOR[2], _T("双人对战"));
	DrawRoundrect(small_BUTTON[3], small_MINI_BUTTON_WIDTH, small_MINI_BUTTON_HEIGHT, BUTTON_COLOR[3], _T("总分榜"));
	DrawRoundrect(small_BUTTON[4], small_MINI_BUTTON_WIDTH, small_MINI_BUTTON_HEIGHT, BUTTON_COLOR[4], _T("小提示"));
	//获取鼠标信息
	MOUSEMSG m{ 0 };           // 鼠标信息结构体
	BOOL flag = TRUE;
	while (flag)
	{
		if (MouseHit())
			m = GetMouseMsg();   // 获取鼠标信息
		if (m.uMsg == WM_MOUSEMOVE)// 鼠标移动
			if (m.x >= small_BUTTON[0].x && m.x <= small_BUTTON[0].x + small_BUTTON_WIDTH && m.y >= small_BUTTON[0].y && m.y <= small_BUTTON[0].y + small_BUTTON_HEIGHT) {
				DrawRoundrect(big_BUTTON[0], big_BUTTON_WIDTH, big_BUTTON_HEIGHT, BUTTON_COLOR[0], _T("单人模式"));
				while (true) {
					if (MouseHit())
						m = GetMouseMsg();   // 获取鼠标信息
					if (m.uMsg == WM_LBUTTONDOWN)
						return ONE_PLAYER;
					else if (m.uMsg == WM_MOUSEMOVE)// 鼠标移动
						if (m.x <= big_BUTTON[0].x || m.x >= big_BUTTON[0].x + big_BUTTON_WIDTH || m.y <= big_BUTTON[0].y || m.y >= big_BUTTON[0].y + big_BUTTON_HEIGHT) {
							setfillcolor(BK_COLOR);
							solidrectangle(big_BUTTON[0].x - 2, big_BUTTON[0].y - 2, big_BUTTON[0].x + big_BUTTON_WIDTH + 2, big_BUTTON[0].y + big_BUTTON_HEIGHT + 2);
							DrawRoundrect(small_BUTTON[0], small_BUTTON_WIDTH, small_BUTTON_HEIGHT, BUTTON_COLOR[0], _T("单人模式"));
							break;
						}
				}
			}
			else if (m.x >= small_BUTTON[1].x && m.x <= small_BUTTON[1].x + small_BUTTON_WIDTH && m.y >= small_BUTTON[1].y && m.y <= small_BUTTON[1].y + small_BUTTON_HEIGHT) {
				DrawRoundrect(big_BUTTON[1], big_BUTTON_WIDTH, big_BUTTON_HEIGHT, BUTTON_COLOR[1], _T("人机对战"));
				while (true) {
					if (MouseHit())
						m = GetMouseMsg();   // 获取鼠标信息
					if (m.uMsg == WM_LBUTTONDOWN)
						return AI_PLAYER;
					else if (m.uMsg == WM_MOUSEMOVE)// 鼠标移动
						if (m.x <= big_BUTTON[1].x || m.x >= big_BUTTON[1].x + big_BUTTON_WIDTH || m.y <= big_BUTTON[1].y || m.y >= big_BUTTON[1].y + big_BUTTON_HEIGHT) {
							setfillcolor(BK_COLOR);
							solidrectangle(big_BUTTON[1].x - 2, big_BUTTON[1].y - 2, big_BUTTON[1].x + big_BUTTON_WIDTH + 2, big_BUTTON[1].y + big_BUTTON_HEIGHT + 2);
							DrawRoundrect(small_BUTTON[1], small_BUTTON_WIDTH, small_BUTTON_HEIGHT, BUTTON_COLOR[1], _T("人机对战"));
							break;
						}
				}
			}
			else if (m.x >= small_BUTTON[2].x && m.x <= small_BUTTON[2].x + small_BUTTON_WIDTH && m.y >= small_BUTTON[2].y && m.y <= small_BUTTON[2].y + small_BUTTON_HEIGHT) {
				DrawRoundrect(big_BUTTON[2], big_BUTTON_WIDTH, big_BUTTON_HEIGHT, BUTTON_COLOR[2], _T("双人对战"));
				while (true) {
					if (MouseHit())
						m = GetMouseMsg();   // 获取鼠标信息
					if (m.uMsg == WM_LBUTTONDOWN)
						return TWO_PLAYER;
					else if (m.uMsg == WM_MOUSEMOVE)// 鼠标移动
						if (m.x <= big_BUTTON[2].x || m.x >= big_BUTTON[2].x + big_BUTTON_WIDTH || m.y <= big_BUTTON[2].y || m.y >= big_BUTTON[2].y + big_BUTTON_HEIGHT) {
							setfillcolor(BK_COLOR);
							solidrectangle(big_BUTTON[2].x - 2, big_BUTTON[2].y - 2, big_BUTTON[2].x + big_BUTTON_WIDTH + 2, big_BUTTON[2].y + big_BUTTON_HEIGHT + 2);
							DrawRoundrect(small_BUTTON[2], small_BUTTON_WIDTH, small_BUTTON_HEIGHT, BUTTON_COLOR[2], _T("双人对战"));
							break;
						}
				}
			}
			else if (m.x >= small_BUTTON[3].x && m.x <= small_BUTTON[3].x + small_MINI_BUTTON_WIDTH && m.y >= small_BUTTON[3].y && m.y <= small_BUTTON[3].y + small_MINI_BUTTON_HEIGHT) {
				DrawRoundrect(big_BUTTON[3], big_MINI_BUTTON_WIDTH, big_MINI_BUTTON_HEIGHT, BUTTON_COLOR[3], _T("总分榜"));
				while (true) {
					if (MouseHit())
						m = GetMouseMsg();   // 获取鼠标信息
					if (m.uMsg == WM_LBUTTONDOWN)
						return RANK;
					else if (m.uMsg == WM_MOUSEMOVE)// 鼠标移动
						if (m.x <= big_BUTTON[3].x || m.x >= big_BUTTON[3].x + big_MINI_BUTTON_WIDTH || m.y <= big_BUTTON[3].y || m.y >= big_BUTTON[3].y + big_MINI_BUTTON_HEIGHT) {
							setfillcolor(BK_COLOR);
							solidrectangle(big_BUTTON[3].x - 2, big_BUTTON[3].y - 2, big_BUTTON[3].x + big_MINI_BUTTON_WIDTH + 2, big_BUTTON[3].y + big_MINI_BUTTON_HEIGHT + 2);
							DrawRoundrect(small_BUTTON[3], small_MINI_BUTTON_WIDTH, small_MINI_BUTTON_HEIGHT, BUTTON_COLOR[3], _T("总分榜"));
							break;
						}
				}
			}
			else if (m.x >= small_BUTTON[4].x && m.x <= small_BUTTON[4].x + small_MINI_BUTTON_WIDTH && m.y >= small_BUTTON[4].y && m.y <= small_BUTTON[4].y + small_MINI_BUTTON_HEIGHT) {
				DrawRoundrect(big_BUTTON[4], big_MINI_BUTTON_WIDTH, big_MINI_BUTTON_HEIGHT, BUTTON_COLOR[4], _T("小提示"));
				while (true) {
					if (MouseHit())
						m = GetMouseMsg();   // 获取鼠标信息
					if (m.uMsg == WM_LBUTTONDOWN)
						return HELP;
					else if (m.uMsg == WM_MOUSEMOVE)// 鼠标移动
						if (m.x <= big_BUTTON[4].x || m.x >= big_BUTTON[4].x + big_MINI_BUTTON_WIDTH || m.y <= big_BUTTON[4].y || m.y >= big_BUTTON[4].y + big_MINI_BUTTON_HEIGHT) {
							setfillcolor(BK_COLOR);
							solidrectangle(big_BUTTON[4].x - 2, big_BUTTON[4].y - 2, big_BUTTON[4].x + big_MINI_BUTTON_WIDTH + 2, big_BUTTON[4].y + big_MINI_BUTTON_HEIGHT + 2);
							DrawRoundrect(small_BUTTON[4], small_MINI_BUTTON_WIDTH, small_MINI_BUTTON_HEIGHT, BUTTON_COLOR[4], _T("小提示"));
							break;
						}
				}
			}
	}
	return 0;
}
void GetName(TCHAR name[])
{
	TCHAR inputn[MAX_NAME_LENGTH];
	InputBox(inputn, MAX_NAME_LENGTH, _T("请输入你的名字…\n不输入名字无法记录你的成绩哦[注意名字不要被空格分开了]"), _T("NAME_INPUT"));
	for (int i = 0; i < MAX_NAME_LENGTH; ++i)
		name[i] = inputn[i];
}
int level_menu()
{
	setbkcolor(BK_COLOR);
	cleardevice();//背景填充
	IMAGE bkimg;
	loadimage(&bkimg, _T("./image/pure_bk.png"), 400, 400);
	putimage(0, (WINDOW_HEIGHT - bkimg.getheight()) / 2, &bkimg);
	//打印所有基础按钮
	DrawRoundrect(small_BUTTON[0], small_BUTTON_WIDTH, small_BUTTON_HEIGHT, BUTTON_COLOR[0], _T("入门版"));
	DrawRoundrect(small_BUTTON[1], small_BUTTON_WIDTH, small_BUTTON_HEIGHT, BUTTON_COLOR[1], _T("进阶版"));
	DrawRoundrect(small_BUTTON[2], small_BUTTON_WIDTH, small_BUTTON_HEIGHT, BUTTON_COLOR[2], _T("高级版"));
	//获取鼠标信息
	MOUSEMSG m{ 0 };           // 鼠标信息结构体
	BOOL flag = TRUE;
	while (flag)
	{
		if (MouseHit())
			m = GetMouseMsg();   // 获取鼠标信息
		if (m.uMsg == WM_MOUSEMOVE)// 鼠标移动
			if (m.x >= small_BUTTON[0].x && m.x <= small_BUTTON[0].x + small_BUTTON_WIDTH && m.y >= small_BUTTON[0].y && m.y <= small_BUTTON[0].y + small_BUTTON_HEIGHT) {
				DrawRoundrect(big_BUTTON[0], big_BUTTON_WIDTH, big_BUTTON_HEIGHT, BUTTON_COLOR[0], _T("入门版"));
				while (true) {
					if (MouseHit())
						m = GetMouseMsg();   // 获取鼠标信息
					if (m.uMsg == WM_LBUTTONDOWN)
						return LEVEL1;
					else if (m.uMsg == WM_MOUSEMOVE)// 鼠标移动
						if (m.x <= big_BUTTON[0].x || m.x >= big_BUTTON[0].x + big_BUTTON_WIDTH || m.y <= big_BUTTON[0].y || m.y >= big_BUTTON[0].y + big_BUTTON_HEIGHT) {
							setfillcolor(BK_COLOR);
							solidrectangle(big_BUTTON[0].x - 2, big_BUTTON[0].y - 2, big_BUTTON[0].x + big_BUTTON_WIDTH + 2, big_BUTTON[0].y + big_BUTTON_HEIGHT + 2);
							DrawRoundrect(small_BUTTON[0], small_BUTTON_WIDTH, small_BUTTON_HEIGHT, BUTTON_COLOR[0], _T("入门版"));
							break;
						}
				}
			}
			else if (m.x >= small_BUTTON[1].x && m.x <= small_BUTTON[1].x + small_BUTTON_WIDTH && m.y >= small_BUTTON[1].y && m.y <= small_BUTTON[1].y + small_BUTTON_HEIGHT) {
				DrawRoundrect(big_BUTTON[1], big_BUTTON_WIDTH, big_BUTTON_HEIGHT, BUTTON_COLOR[1], _T("进阶版"));
				while (true) {
					if (MouseHit())
						m = GetMouseMsg();   // 获取鼠标信息
					if (m.uMsg == WM_LBUTTONDOWN)
						return LEVEL2;
					else if (m.uMsg == WM_MOUSEMOVE)// 鼠标移动
						if (m.x <= big_BUTTON[1].x || m.x >= big_BUTTON[1].x + big_BUTTON_WIDTH || m.y <= big_BUTTON[1].y || m.y >= big_BUTTON[1].y + big_BUTTON_HEIGHT) {
							setfillcolor(BK_COLOR);
							solidrectangle(big_BUTTON[1].x - 2, big_BUTTON[1].y - 2, big_BUTTON[1].x + big_BUTTON_WIDTH + 2, big_BUTTON[1].y + big_BUTTON_HEIGHT + 2);
							DrawRoundrect(small_BUTTON[1], small_BUTTON_WIDTH, small_BUTTON_HEIGHT, BUTTON_COLOR[1], _T("进阶版"));
							break;
						}
				}
			}
			else if (m.x >= small_BUTTON[2].x && m.x <= small_BUTTON[2].x + small_BUTTON_WIDTH && m.y >= small_BUTTON[2].y && m.y <= small_BUTTON[2].y + small_BUTTON_HEIGHT) {
				DrawRoundrect(big_BUTTON[2], big_BUTTON_WIDTH, big_BUTTON_HEIGHT, BUTTON_COLOR[2], _T("高级版"));
				while (true) {
					if (MouseHit())
						m = GetMouseMsg();   // 获取鼠标信息
					if (m.uMsg == WM_LBUTTONDOWN)
						return LEVEL3;
					else if (m.uMsg == WM_MOUSEMOVE)// 鼠标移动
						if (m.x <= big_BUTTON[2].x || m.x >= big_BUTTON[2].x + big_BUTTON_WIDTH || m.y <= big_BUTTON[2].y || m.y >= big_BUTTON[2].y + big_BUTTON_HEIGHT) {
							setfillcolor(BK_COLOR);
							solidrectangle(big_BUTTON[2].x - 2, big_BUTTON[2].y - 2, big_BUTTON[2].x + big_BUTTON_WIDTH + 2, big_BUTTON[2].y + big_BUTTON_HEIGHT + 2);
							DrawRoundrect(small_BUTTON[2], small_BUTTON_WIDTH, small_BUTTON_HEIGHT, BUTTON_COLOR[2], _T("高级版"));
							break;
						}
				}
			}
	}
	return -1;

}
int single_menu(TCHAR name[], char& color, char& mapsize)
{
	GetName(name);
	color = BLUE_S;//初始化颜色

	wchar_t inputm[7];
	InputBox(inputm, 7, _T("请选择地图大小\n请输入：small或big"), _T("MAP_SIZE"));
	if ((inputm[0] == 'b' || inputm[0] == 'B') && (inputm[1] == 'i' || inputm[1] == 'I') && (inputm[2] == 'g' || inputm[0] == 'G'))
		mapsize = BIG_MAP;//初始化地图大小
	else
		mapsize = ORDI_MAP;//初始化地图大小

	wchar_t inputc[3];
	InputBox(inputc, 3, _T("请选择你的颜色\n蓝色:1；黄色:2；绿色:3"), _T("COLOR_INPUT"));
	color = inputc[0] - '0';
	if (color< BLUE_S || color>GREEN_S)
		color = BLUE_S;

	return level_menu();
}
void help_menu()
{
	IMAGE HELP;
	loadimage(&HELP, _T("./image/help.png"), SHOW_WIDTH, SHOW_WIDTH * 1920 / 3072);
	//setbkcolor(BK_COLOR);
	cleardevice();
	putimage(0, (WINDOW_HEIGHT - HELP.getheight()) / 2, &HELP);
	ExMessage operation;//操作类型
	while (true) {
		peekmessage(&operation, EX_MOUSE);
		if (operation.message == WM_LBUTTONDOWN)
			break;
	}
}
void rank_menu()
{
	setbkcolor(BK_COLOR);
	cleardevice();
	const LOGFONT chara = { INFO_HEIGHT_LINE / 2,0,0,0,FW_BLACK,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,DEFAULT_PITCH, _T("幼圆") };//字体高度与位置不随长宽比例变化
	settextstyle(&chara);
	settextcolor(BLACK);
	ifstream infile;
	infile.open("./SCORE.txt", ios::binary);
	if (!infile) {
		cerr << "File Error!" << endl;
		exit(-1);
	}
	int max1, max2, max3, readnum, num;
	string name1 = "", name2 = "", name3 = "", readname_c[MAX_NAME_LENGTH];
	string readlevel = "", readname;
	max1 = max2 = max3 = -1;
	num = 0;
	while (!infile.eof()) {
		infile >> readlevel >> readname >> readnum;
		if (readlevel == "入门版" && readnum > max1) {
			max1 = readnum;
			name1 = readname;
		}

		else if (readlevel == "进阶版" && readnum > max2) {
			max2 = readnum;
			name2 = readname;
		}

		else if (readlevel == "高级版" && readnum > max3) {
			max3 = readnum;
			name3 = readname;
		}
		WCHAR text[70], widelevel[20], widename[20];

		if (readname != "") {
			MultiByteToWideChar(CP_ACP, 0, readlevel.c_str(), -1, widelevel, 20);
			MultiByteToWideChar(CP_ACP, 0, readname.c_str(), -1, widename, 20);
			_stprintf(text, _T("版本：%s 用户名：%s 得分：%d\0"), widelevel, widename, readnum);
			outtextxy(0, INFO_HEIGHT_LINE * 3 / 2 + INFO_HEIGHT_LINE * num++ / 2, text);
		}
	}
	infile.close();

	WCHAR text[70], widename[20];
	if (name1 != "") {
		MultiByteToWideChar(CP_ACP, 0, name1.c_str(), -1, widename, 20);
		_stprintf(text, _T("最高分：入门版 用户名：%s 得分：%d\0"), widename, max1);
		outtextxy(0, 0, text);
	}
	else
		outtextxy(0, 0, L"没有入门版最高分记录");
	if (name2 != "") {
		MultiByteToWideChar(CP_ACP, 0, name2.c_str(), -1, widename, 20);
		_stprintf(text, _T("最高分：进阶版 用户名：%s 得分：%d\0"), widename, max2);
		outtextxy(0, INFO_HEIGHT_LINE / 2, text);
	}
	else
		outtextxy(0, INFO_HEIGHT_LINE / 2, L"没有进阶版最高分记录");
	if (name3 != "") {
		MultiByteToWideChar(CP_ACP, 0, name3.c_str(), -1, widename, 20);
		_stprintf(text, _T("最高分：高级版 用户名：%s 得分：%d\0"), widename, max3);
		outtextxy(0, INFO_HEIGHT_LINE, text);
	}
	else
		outtextxy(0, INFO_HEIGHT_LINE, L"没有高级版最高分记录");

	outtextxy(0, WINDOW_HEIGHT - INFO_HEIGHT_LINE / 2, L"按下回车以继续...");
	while (getchar() != '\n')
		;
}