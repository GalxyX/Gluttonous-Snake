#pragma once
#include <unordered_map>
#include <iostream>
#include "Parameter.h"
using namespace std;
//蛇头朝向
const char UP = 0;
const char RIGHT = 1;
const char LEFT = 3;
const char DOWN = 2;
//地图区域标记
const char SOLID = -1;//边界
const char GROUND = 0;
const char FAST_GROUND = 1;
const char SLOW_GROUND = 2;

const char GROUND_NUM = max(max(GROUND, FAST_GROUND), SLOW_GROUND);

const char SOFT_WALL = 3;
const char HARD_WALL = 4;
const char SNAKE_ORDI = 5;
const char SNAKE_FAST = 6;
const char SNAKE_SLOW = 7;
//果子编号
const char NO_FOOD = -1;
const char BAD = 0;//坏果，编号为0，生命-1
const char SIMPLE = 1;//普通果，编号为1，体力+10
const char GOOD = 2;//好果，编号为2，体力+20
const char EXCELLENT = 3;//极好果，编号为3，体力+30，生命+1
//速度时间间隔
const char FAST = 0;
const char NORMAL = 1;
const char SLOW = 2;
const char WALL = 80;
//Snake相关定义
const char SOFT_HURT = 2;//撞软墙伤害
const char SPIRIT_DECREASE = 1;//体能减少速率
const char SOFT_SCORE = -2;//软墙减分
const char HARD_SCORE = -3;//硬墙减分
const char BOUNDARY_SCORE = -3;//边界减分
//Snake::JudgeBody函数返回值
const char NO_SNAKE = 0;
const char IS_HEADBODY = 1;
const char IS_TAIL = 2;

class BasicImg {
private:
	IMAGE info_bk;					//信息部分背景，自制
	IMAGE PAUSE_SMALL;
	IMAGE PAUSE_BIG;
public:
	BasicImg();
	void DisplayInfoBK(int num, TCHAR name[][MAX_NAME_LENGTH]);
	void DisplayPause(bool is_big);
};

class Snake;
class Food;
class Map {//整个大地图
private:
	const COLORREF BOUNDARY_COLOR = RGB(205, 170, 125);
	const int CANVAS_WIDTH;			//总场地（画布）宽像素（含边界）
	const int CANVAS_HEIGHT;		//总场地（画布）高像素（含边界）
	IMAGE MAP_IMG[5];				//地图五种区块颜色，按上定义顺序，对应数组下标
	void AsearchList(const coordinate& start, const coordinate& end, unordered_map<int, coordinate>& this_parent, unordered_map<int, int>& this_G, Food& f);
public:
	char level;						//本次游戏版本
	clock_t runtime;				//本部分游戏运行时间
	const int CANVAS_XBLOCK;		//总场地（画布）宽像素块个数（不含边界）
	const int CANVAS_YBLOCK;		//总场地（画布）高像素块个数（不含边界）
	IMAGE* canvas;					//整个地图区域的画布
	char* TERRITORY;				//char TERRITORY[CANVAS_YBLOCK][CANVAS_XBLOCK] = { GROUND };//存储每一块的情况，0为空，1为块地，2为慢地，3为软墙，4为硬墙，5为蛇在空地上，6为蛇在快地上，7为蛇在慢地上
	bool SnakeToWall(coordinate coor);
	void Refresh(coordinate Block, IMAGE* cover = NULL);
	void Display(const Snake& s, int showwidth = SHOW_WIDTH, int startx = 0, int starty = 0);
	void RandomRegion();			//随机初始化地图区块（初始游戏时使用）
	bool GetNextStep(char& direction, int& step, const coordinate& start, const coordinate& end, Food& f);
	~Map();
	Map(int this_level, int xblock, int yblock);	//地图宽与高的块数
};
class Food {
private:
	const int FOOD_MAX[4] = { 2,4,3,1 };	//四种食物最大数量
	int foodnum[4];							//四种食物当前数量
	IMAGE FOOD_IMG[4];
	unordered_map <int, int> allfood;		//键值为块坐标（一维数组），值为食物类型（按前述声明顺序作为编号）
public:
	const int spiritadd[4] = { 0,50,80,100 };//记录每个果体力影响，按前述声明顺序
	const int lifeadd[4] = { -1,0,0,1 };	//记录每个果生命影响，按前述声明顺序
	const int scoreadd[4] = { -4,2,4,6 };		//记录每个果分数影响，按前述声明顺序
	Food(Map& m);
	bool RandomGenerateFood(Map& tofood);
	char JudgeFood(coordinate tojudge, int canvas_width);
	void EraseFood(coordinate tojudge, Map& m);
	void DestroyAllFood(Map& tofood);
	int GetGoodFoodNum();
	void CreateFood(coordinate tojudge, Map& m);
	char WhichDirection(Map& m, const coordinate& start);
};

typedef struct QNode {	//元素结点
	coordinate place;			//坐标
	struct QNode* next;
}QNode, * Qptr;
typedef struct {		//特殊节点
	Qptr head;			//蛇头指针（队列尾）
	Qptr tail;			//蛇尾指针（队列头）
}LinkQ;
class Snake {
private:
	const int MAX_SPIRIT = 400;		//初始最大体能
	const char INI_LENGTH = 2;		//蛇的初始长度
	char orientation;				//蛇头朝向
	char this_orientation;			//本次键盘改变的蛇头朝向
	LinkQ body;						//存储整条蛇（包括蛇头）的坐标
	bool operate;					//操作方式，缺省（1）为箭头，否则（0）为wasd
	int life;						//蛇的生命值
	int length;						//蛇的长度，等于不包括蛇头的蛇身长
	int spirit;						//体能
	int speed;						//速度，普通为1，加速为0，减速为2，撞墙为20
	int count;						//计数经过多少个循环，达到speed值可以移动
	void InitSnake(Map& m, coordinate start);
	void DestroySnake(Map& m, Food& f);
	bool RandomGenerateSnake(Map& m, Food& f);
	char LookForward(const Map& m);
	coordinate MoveHead(Map& m);
	coordinate MoveTail(Map& m);
	char JudgeBody(coordinate tojudge);
	friend bool Map::SnakeToWall(coordinate coor);//对于Snake中元素全部重置，因此定义为友元以便利操作
	friend void Map::Display(const Snake& s, int showwidth, int startx, int starty);
public:
	TCHAR name[MAX_NAME_LENGTH];	//玩家名字
	int score;						//当前分数
	IMAGE head_img[4];				//蛇头图片
	IMAGE body_img;					//蛇身图片
	void ChangeOrient(int key);
	bool ClearNum(Map& m, Food& f);
	bool MoveSnake(Map& m, Food& f);
	void DisplayInfo(int num, int total);
	coordinate GetHead();
	Snake(int key, Map& m, int startx, int starty, int no, int life, TCHAR s_name[]);//操作方式，地图，蛇头初始块x，蛇头初始块y
	~Snake();
};