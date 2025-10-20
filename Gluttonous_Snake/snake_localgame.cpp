#define _CRT_SECURE_NO_WARNINGS
#include <conio.h>
#include <fstream>
#include "snake_class.h"
const int TIME_GAP = 80;
const int FOOD_REGEN = 16000;
void DrawRoundrect(coordinate XL, int width, int height, COLORREF color, const TCHAR text[]);

bool PAUSE(Map& m, BasicImg& img)
{//返回是否继续游戏
	SetWorkingImage(NULL);
	DrawRoundrect(SMALL_QUIT, QUIT_LENGTH, QUIT_HEIGHT, RGB(255, 127, 0), L"结束游戏");
	setfillcolor(BK_COLOR);
	ExMessage operation;//操作类型
	char buttonstatus = 0;//按钮无变化false，当前暂停按钮变大为1，结束按钮变大为2
	while (true) {
		if (peekmessage(&operation, EX_MOUSE | EX_KEY)) {
			switch (operation.message) {
				case WM_KEYUP://按键弹起
					if (operation.vkcode == VK_SPACE) {
						solidrectangle(BIG_QUIT.x - 2, BIG_QUIT.y - 2, BIG_QUIT.x + QUIT_LENGTH_GETBIG + 2, BIG_QUIT.y + QUIT_HEIGHT_GETBIG + 2);//首先用背景覆盖
						return true;
					}
				case WM_MOUSEMOVE:
					if (buttonstatus != 1 && operation.x >= SHOW_WIDTH - PAUSE_SIDE_LENGTH && operation.x <= SHOW_WIDTH && operation.y >= 0 && operation.y <= PAUSE_SIDE_LENGTH) {
						img.DisplayPause(true);
						buttonstatus = 1;
					}
					else if (buttonstatus == 1 && (operation.x < SHOW_WIDTH - PAUSE_SIDE_LENGTH_GETBIG || operation.x > SHOW_WIDTH || operation.y < 0 || operation.y > PAUSE_SIDE_LENGTH_GETBIG)) {
						img.DisplayPause(false);
						buttonstatus = 0;
					}
					if (buttonstatus != 2 && operation.x >= SMALL_QUIT.x && operation.x <= SMALL_QUIT.x + QUIT_LENGTH && operation.y >= SMALL_QUIT.y && operation.y <= SMALL_QUIT.y + QUIT_HEIGHT) {
						//solidrectangle(SMALL_QUIT.x, SMALL_QUIT.y, SMALL_QUIT.x + QUIT_LENGTH+2, SMALL_QUIT.y + QUIT_HEIGHT+2);//首先用背景覆盖
						DrawRoundrect(BIG_QUIT, QUIT_LENGTH_GETBIG, QUIT_HEIGHT_GETBIG, RGB(255, 127, 0), L"结束游戏");
						buttonstatus = 2;
					}
					else if (buttonstatus == 2 && (operation.x < BIG_QUIT.x || operation.x > BIG_QUIT.x + QUIT_LENGTH || operation.y < BIG_QUIT.y || operation.y > BIG_QUIT.y + QUIT_HEIGHT)) {
						solidrectangle(BIG_QUIT.x - 2, BIG_QUIT.y - 2, BIG_QUIT.x + QUIT_LENGTH_GETBIG + 2, BIG_QUIT.y + QUIT_HEIGHT_GETBIG + 2);//首先用背景覆盖
						DrawRoundrect(SMALL_QUIT, QUIT_LENGTH, QUIT_HEIGHT, RGB(255, 127, 0), L"结束游戏");
						buttonstatus = 0;
					}
					break;
				case WM_LBUTTONUP:
					if (buttonstatus == 1 && operation.x >= SHOW_WIDTH - PAUSE_SIDE_LENGTH_GETBIG && operation.x <= SHOW_WIDTH && operation.y >= 0 && operation.y <= PAUSE_SIDE_LENGTH_GETBIG) {
						solidrectangle(BIG_QUIT.x - 2, BIG_QUIT.y - 2, BIG_QUIT.x + QUIT_LENGTH_GETBIG + 2, BIG_QUIT.y + QUIT_HEIGHT_GETBIG + 2);//首先用背景覆盖
						return true;
					}
					else if (buttonstatus == 2 && operation.x >= BIG_QUIT.x && operation.x <= BIG_QUIT.x + QUIT_LENGTH_GETBIG && operation.y >= BIG_QUIT.y && operation.y <= BIG_QUIT.y + QUIT_HEIGHT_GETBIG)
						return false;
				default:
					break;
			}
		}
	}
	return true;
}
bool JUDGE_MOUSE_KEY(Map& m, BasicImg& img, int snaken, Snake s[], bool& mouse_on_pause)
{//返回是否退出游戏
	bool is_continue = true;//继续游戏标志
	ExMessage operation;//操作类型
	if (peekmessage(&operation, EX_MOUSE | EX_KEY)) {
		int i;
		switch (operation.message) {
			case WM_KEYUP://按键弹起
				if (operation.vkcode == VK_SPACE)//暂停///////////////////////////////////
					is_continue = PAUSE(m, img);
				else
					for (i = 0; i < snaken; ++i)
						s[i].ChangeOrient(operation.vkcode);
				break;
			case WM_MOUSEMOVE:
				if (!mouse_on_pause && operation.x >= SHOW_WIDTH - PAUSE_SIDE_LENGTH && operation.x <= SHOW_WIDTH && operation.y >= 0 && operation.y <= PAUSE_SIDE_LENGTH) {
					img.DisplayPause(true);
					mouse_on_pause = true;
					SetWorkingImage(m.canvas);
				}
				else if (mouse_on_pause && (operation.x < SHOW_WIDTH - PAUSE_SIDE_LENGTH_GETBIG || operation.x > SHOW_WIDTH || operation.y < 0 || operation.y > PAUSE_SIDE_LENGTH_GETBIG)) {
					img.DisplayPause(false);
					mouse_on_pause = false;
					SetWorkingImage(m.canvas);
				}
				break;
			case WM_LBUTTONDOWN:
				if (mouse_on_pause && operation.x >= SHOW_WIDTH - PAUSE_SIDE_LENGTH_GETBIG && operation.x <= SHOW_WIDTH && operation.y >= 0 && operation.y <= PAUSE_SIDE_LENGTH_GETBIG)
					is_continue = PAUSE(m, img);
				break;
			default:
				break;
		}
	}
	return is_continue;
}
void single_game(TCHAR name[][MAX_NAME_LENGTH], char& color, char& mapsize, char level)
{
	setbkcolor(BK_COLOR);
	cleardevice();//背景填充
	int block_width = SHOW_XBLOCK + mapsize * SHOW_XBLOCK;
	int block_height = SHOW_YBLOCK + mapsize * SHOW_YBLOCK;

	BasicImg imgstd;//加载标准背景图
	imgstd.DisplayInfoBK(1, name);//显示背景图
	//首先生成地图空间
	Map single(level, block_width, block_height);
	//SetWorkingImage(single.canvas);
	single.RandomRegion();//①初始化地图区块
	//single.DisplayInfoBK(2);
	//②确定蛇的位置，可覆盖区块，消除墙
	int snake_life;
	if (level == LEVEL1)
		snake_life = 1;
	else if (level == LEVEL2)
		snake_life = 100000000;
	else
		snake_life = 5;
	Snake s1(true, single, block_width / 2 - 1, block_height / 2 - 1, color, snake_life, name[0]);/*画面长奇数为画面中央，画面长偶数靠近0侧（坐标轴）*/
	//③初始化食物
	Food mapfood(single);//所有食物
	/*************************************可以开始循环了*************************************/
	bool mouse_on_pause = false;//鼠标是否悬停在暂停键
	bool continuegame = true;//游戏是否继续

	clock_t pasttime = single.runtime;//上次程序运行时间
	clock_t starttime = clock();//游戏开始时间
	clock_t lasttime = starttime;//上次运行时间
	clock_t nowtime;//运行时间
	clock_t lastfoodgene = starttime;//上次生成食物时间

	while (1) {
		nowtime = clock();//获取本次循环的时间
		single.runtime = pasttime + (nowtime - starttime);//更新游戏运行时间

		if (!JUDGE_MOUSE_KEY(single, imgstd, 1, &s1, mouse_on_pause))
			break;

		if (nowtime - lasttime >= TIME_GAP) {//已经过去时间间隔
			continuegame = s1.MoveSnake(single, mapfood);//移动蛇
			if (!s1.ClearNum(single, mapfood))//负数清零
				continuegame = false;
			if (nowtime - lastfoodgene >= FOOD_REGEN || !mapfood.GetGoodFoodNum()) {//固定时间后随机生成新的食物
				mapfood.DestroyAllFood(single);
				mapfood.RandomGenerateFood(single);
				lastfoodgene = nowtime;
			}
			single.Display(s1);//显示图像
			s1.DisplayInfo(0, 1);//显示信息

			if (!continuegame)//判断是否结束
				break;

			lasttime = nowtime;
			//打印时间
			TCHAR str[10];
			_stprintf(str, _T("%d : %d\0"), single.runtime / 1000 / 60, single.runtime / 1000 % 60);
			outtextxy(SHOW_WIDTH / 2 - textwidth(str) - 10, 0, str);
		}
	}

	ofstream outfile;
	outfile.open("./SCORE.txt", ios::binary | ios::app);
	if (!outfile) {
		cerr << "File Error" << endl;
		exit(-3012);
	}
	char level_name[11] = "\0", player[MAX_NAME_LENGTH];
	if (level == LEVEL1)
		strcpy(level_name, "入门版");
	else if (level == LEVEL2)
		strcpy(level_name, "进阶版");
	else
		strcpy(level_name, "高级版");

	WideCharToMultiByte(CP_ACP, 0, s1.name, -1, player, MAX_NAME_LENGTH, NULL, NULL);
	if (player[0] != '\0')
		outfile << ' ' << level_name << ' ' << player << ' ' << s1.score;
	outfile.close();
}
void double_game(char level)
{
	setbkcolor(BK_COLOR);
	cleardevice();//背景填充
	TCHAR player[2][MAX_NAME_LENGTH] = { L"Player1",L"Player2" };

	//着色两位玩家
	char color1 = rand() % GREEN_S + 1;
	char color2 = (color1 + rand() % (GREEN_S - 1)) % GREEN_S + 1;//有效防止重色，无需讨论

	BasicImg imgstd;//加载标准背景图
	imgstd.DisplayInfoBK(2, player);//显示背景图

	//首先生成地图空间
	Map twoP(level, SHOW_XBLOCK, SHOW_YBLOCK);
	twoP.RandomRegion();//①初始化地图区块

	//②确定蛇的位置，可覆盖区块，消除墙
	Snake s[2]{ {false, twoP, SHOW_XBLOCK / 4 - 1, SHOW_YBLOCK / 2 - 1, color1, 5, player[0]},{true, twoP, SHOW_XBLOCK * 3 / 4 - 1, SHOW_YBLOCK / 2 - 1, color2, 5, player[1]} };/*一个在左半边中央，另一个在右半边中央*/
	//③初始化食物
	Food mapfood(twoP);//所有食物
	/*************************************可以开始循环了*************************************/
	bool mouse_on_pause = false;//鼠标是否悬停在暂停键
	bool stillalive[2] = { true,true };//第[i]条蛇是否还活着

	clock_t pasttime = twoP.runtime;//上次程序运行时间
	clock_t starttime = clock();//游戏开始时间
	clock_t lasttime = starttime;//上次运行时间
	clock_t nowtime;//运行时间
	clock_t lastfoodgene = starttime;//上次生成食物时间

	int i;
	while (1) {
		nowtime = clock();//获取本次循环的时间
		twoP.runtime = pasttime + (nowtime - starttime);//更新游戏运行时间

		if (!JUDGE_MOUSE_KEY(twoP, imgstd, 2, s, mouse_on_pause))
			break;

		if (nowtime - lasttime >= TIME_GAP) {//已经过去时间间隔
			for (i = 0; i < 2; ++i)
				if (stillalive[i]) {
					stillalive[i] = s[i].MoveSnake(twoP, mapfood);//移动蛇
					if (!s[i].ClearNum(twoP, mapfood))//负数清零
						stillalive[i] = false;
					s[i].DisplayInfo(i, 2);//显示信息
					SetWorkingImage(twoP.canvas);
				}
			if (nowtime - lastfoodgene >= FOOD_REGEN || !mapfood.GetGoodFoodNum()) {//固定时间后随机生成新的食物
				mapfood.DestroyAllFood(twoP);
				mapfood.RandomGenerateFood(twoP);
				lastfoodgene = nowtime;
			}
			twoP.Display(s[0]);//显示图像

			if (!stillalive[0] && !stillalive[1])//判断是否结束
				break;

			SetWorkingImage(NULL);
			lasttime = nowtime;
			//打印时间
			TCHAR str[10];
			_stprintf(str, _T("%d : %d\0"), twoP.runtime / 1000 / 60, twoP.runtime / 1000 % 60);
			solidrectangle(SHOW_WIDTH / 2 - textwidth(str) - 20, 0, SHOW_WIDTH / 2, INFO_HEIGHT_LINE);//确保覆盖原时间
			outtextxy(SHOW_WIDTH / 2 - textwidth(str) - 10, 0, str);
		}
	}
}
void AI_game(TCHAR name[][MAX_NAME_LENGTH], char level)
{
	setbkcolor(BK_COLOR);
	cleardevice();//背景填充
	TCHAR player[2][MAX_NAME_LENGTH] = { L"Player",L"AI Player" };
	if (name[0][0] != L'\0')
		for (int i = 0; i < MAX_NAME_LENGTH; ++i)
			player[0][i] = name[0][i];
	//着色两位玩家
	char color1 = rand() % GREEN_S + 1;
	char color2 = (color1 + rand() % (GREEN_S - 1)) % GREEN_S + 1;//有效防止重色，无需讨论

	BasicImg imgstd;//加载标准背景图
	imgstd.DisplayInfoBK(2, player);//显示背景图

	//首先生成地图空间
	Map twoP(level, SHOW_XBLOCK, SHOW_YBLOCK);
	twoP.RandomRegion();//①初始化地图区块

	//②确定蛇的位置，可覆盖区块，消除墙
	Snake s[2]{ {true, twoP, SHOW_XBLOCK / 4 - 1, SHOW_YBLOCK / 2 - 1, color1, 5, player[0]},{false, twoP, SHOW_XBLOCK * 3 / 4 - 1, SHOW_YBLOCK / 2 - 1, color2, 5, player[1]} };/*一个在左半边中央，另一个在右半边中央*/
	//③初始化食物
	Food mapfood(twoP);//所有食物
	/*************************************可以开始循环了*************************************/
	bool mouse_on_pause = false;//鼠标是否悬停在暂停键
	bool stillalive[2] = { true,true };//第[i]条蛇是否还活着

	clock_t pasttime = twoP.runtime;//上次程序运行时间
	clock_t starttime = clock();//游戏开始时间
	clock_t lasttime = starttime;//上次运行时间
	clock_t nowtime;//运行时间
	clock_t lastfoodgene = starttime;//上次生成食物时间

	int i;
	while (1) {
		nowtime = clock();//获取本次循环的时间
		twoP.runtime = pasttime + (nowtime - starttime);//更新游戏运行时间

		if (!JUDGE_MOUSE_KEY(twoP, imgstd, 1, s, mouse_on_pause))
			break;

		if (nowtime - lasttime >= TIME_GAP) {//已经过去时间间隔
			s[1].ChangeOrient(mapfood.WhichDirection(twoP, s[1].GetHead()));
			for (i = 0; i < 2; ++i)
				if (stillalive[i]) {
					stillalive[i] = s[i].MoveSnake(twoP, mapfood);//移动蛇
					if (!s[i].ClearNum(twoP, mapfood))//负数清零
						stillalive[i] = false;
					s[i].DisplayInfo(i, 2);//显示信息
					SetWorkingImage(twoP.canvas);
				}
			if (nowtime - lastfoodgene >= FOOD_REGEN || !mapfood.GetGoodFoodNum()) {//固定时间后随机生成新的食物
				mapfood.DestroyAllFood(twoP);
				mapfood.RandomGenerateFood(twoP);
				lastfoodgene = nowtime;
			}
			twoP.Display(s[0]);//显示图像

			if (!stillalive[0] && !stillalive[1])//判断是否结束
				break;

			SetWorkingImage(NULL);
			lasttime = nowtime;
			//打印时间
			TCHAR str[10];
			_stprintf(str, _T("%d : %d\0"), twoP.runtime / 1000 / 60, twoP.runtime / 1000 % 60);
			solidrectangle(SHOW_WIDTH / 2 - textwidth(str) - 20, 0, SHOW_WIDTH / 2, INFO_HEIGHT_LINE);//确保覆盖原时间
			outtextxy(SHOW_WIDTH / 2 - textwidth(str) - 10, 0, str);
		}
	}


	ofstream outfile;
	outfile.open("./SCORE.txt", ios::binary | ios::app);
	if (!outfile) {
		cerr << "File Error" << endl;
		exit(-3012);
	}
	char level_name[11] = "\0", playername[MAX_NAME_LENGTH];

	WideCharToMultiByte(CP_ACP, 0, s[0].name, -1, playername, MAX_NAME_LENGTH, NULL, NULL);

	if (name[0][0] != L'\0')
		outfile << ' ' << "AI模式" << ' ' << playername << ' ' << s[0].score;
	outfile.close();
}