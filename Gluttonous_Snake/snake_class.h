#pragma once
#include <unordered_map>
#include <iostream>
#include "Parameter.h"
using namespace std;
//��ͷ����
const char UP = 0;
const char RIGHT = 1;
const char LEFT = 3;
const char DOWN = 2;
//��ͼ������
const char SOLID = -1;//�߽�
const char GROUND = 0;
const char FAST_GROUND = 1;
const char SLOW_GROUND = 2;

const char GROUND_NUM = max(max(GROUND, FAST_GROUND), SLOW_GROUND);

const char SOFT_WALL = 3;
const char HARD_WALL = 4;
const char SNAKE_ORDI = 5;
const char SNAKE_FAST = 6;
const char SNAKE_SLOW = 7;
//���ӱ��
const char NO_FOOD = -1;
const char BAD = 0;//���������Ϊ0������-1
const char SIMPLE = 1;//��ͨ�������Ϊ1������+10
const char GOOD = 2;//�ù������Ϊ2������+20
const char EXCELLENT = 3;//���ù������Ϊ3������+30������+1
//�ٶ�ʱ����
const char FAST = 0;
const char NORMAL = 1;
const char SLOW = 2;
const char WALL = 80;
//Snake��ض���
const char SOFT_HURT = 2;//ײ��ǽ�˺�
const char SPIRIT_DECREASE = 1;//���ܼ�������
const char SOFT_SCORE = -2;//��ǽ����
const char HARD_SCORE = -3;//Ӳǽ����
const char BOUNDARY_SCORE = -3;//�߽����
//Snake::JudgeBody��������ֵ
const char NO_SNAKE = 0;
const char IS_HEADBODY = 1;
const char IS_TAIL = 2;

class BasicImg {
private:
	IMAGE info_bk;					//��Ϣ���ֱ���������
	IMAGE PAUSE_SMALL;
	IMAGE PAUSE_BIG;
public:
	BasicImg();
	void DisplayInfoBK(int num, TCHAR name[][MAX_NAME_LENGTH]);
	void DisplayPause(bool is_big);
};

class Snake;
class Food;
class Map {//�������ͼ
private:
	const COLORREF BOUNDARY_COLOR = RGB(205, 170, 125);
	const int CANVAS_WIDTH;			//�ܳ��أ������������أ����߽磩
	const int CANVAS_HEIGHT;		//�ܳ��أ������������أ����߽磩
	IMAGE MAP_IMG[5];				//��ͼ����������ɫ�����϶���˳�򣬶�Ӧ�����±�
	void AsearchList(const coordinate& start, const coordinate& end, unordered_map<int, coordinate>& this_parent, unordered_map<int, int>& this_G, Food& f);
public:
	char level;						//������Ϸ�汾
	clock_t runtime;				//��������Ϸ����ʱ��
	const int CANVAS_XBLOCK;		//�ܳ��أ������������ؿ�����������߽磩
	const int CANVAS_YBLOCK;		//�ܳ��أ������������ؿ�����������߽磩
	IMAGE* canvas;					//������ͼ����Ļ���
	char* TERRITORY;				//char TERRITORY[CANVAS_YBLOCK][CANVAS_XBLOCK] = { GROUND };//�洢ÿһ��������0Ϊ�գ�1Ϊ��أ�2Ϊ���أ�3Ϊ��ǽ��4ΪӲǽ��5Ϊ���ڿյ��ϣ�6Ϊ���ڿ���ϣ�7Ϊ����������
	bool SnakeToWall(coordinate coor);
	void Refresh(coordinate Block, IMAGE* cover = NULL);
	void Display(const Snake& s, int showwidth = SHOW_WIDTH, int startx = 0, int starty = 0);
	void RandomRegion();			//�����ʼ����ͼ���飨��ʼ��Ϸʱʹ�ã�
	bool GetNextStep(char& direction, int& step, const coordinate& start, const coordinate& end, Food& f);
	~Map();
	Map(int this_level, int xblock, int yblock);	//��ͼ����ߵĿ���
};
class Food {
private:
	const int FOOD_MAX[4] = { 2,4,3,1 };	//����ʳ���������
	int foodnum[4];							//����ʳ�ﵱǰ����
	IMAGE FOOD_IMG[4];
	unordered_map <int, int> allfood;		//��ֵΪ�����꣨һά���飩��ֵΪʳ�����ͣ���ǰ������˳����Ϊ��ţ�
public:
	const int spiritadd[4] = { 0,50,80,100 };//��¼ÿ��������Ӱ�죬��ǰ������˳��
	const int lifeadd[4] = { -1,0,0,1 };	//��¼ÿ��������Ӱ�죬��ǰ������˳��
	const int scoreadd[4] = { -4,2,4,6 };		//��¼ÿ��������Ӱ�죬��ǰ������˳��
	Food(Map& m);
	bool RandomGenerateFood(Map& tofood);
	char JudgeFood(coordinate tojudge, int canvas_width);
	void EraseFood(coordinate tojudge, Map& m);
	void DestroyAllFood(Map& tofood);
	int GetGoodFoodNum();
	void CreateFood(coordinate tojudge, Map& m);
	char WhichDirection(Map& m, const coordinate& start);
};

typedef struct QNode {	//Ԫ�ؽ��
	coordinate place;			//����
	struct QNode* next;
}QNode, * Qptr;
typedef struct {		//����ڵ�
	Qptr head;			//��ͷָ�루����β��
	Qptr tail;			//��βָ�루����ͷ��
}LinkQ;
class Snake {
private:
	const int MAX_SPIRIT = 400;		//��ʼ�������
	const char INI_LENGTH = 2;		//�ߵĳ�ʼ����
	char orientation;				//��ͷ����
	char this_orientation;			//���μ��̸ı����ͷ����
	LinkQ body;						//�洢�����ߣ�������ͷ��������
	bool operate;					//������ʽ��ȱʡ��1��Ϊ��ͷ������0��Ϊwasd
	int life;						//�ߵ�����ֵ
	int length;						//�ߵĳ��ȣ����ڲ�������ͷ������
	int spirit;						//����
	int speed;						//�ٶȣ���ͨΪ1������Ϊ0������Ϊ2��ײǽΪ20
	int count;						//�����������ٸ�ѭ�����ﵽspeedֵ�����ƶ�
	void InitSnake(Map& m, coordinate start);
	void DestroySnake(Map& m, Food& f);
	bool RandomGenerateSnake(Map& m, Food& f);
	char LookForward(const Map& m);
	coordinate MoveHead(Map& m);
	coordinate MoveTail(Map& m);
	char JudgeBody(coordinate tojudge);
	friend bool Map::SnakeToWall(coordinate coor);//����Snake��Ԫ��ȫ�����ã���˶���Ϊ��Ԫ�Ա�������
	friend void Map::Display(const Snake& s, int showwidth, int startx, int starty);
public:
	TCHAR name[MAX_NAME_LENGTH];	//�������
	int score;						//��ǰ����
	IMAGE head_img[4];				//��ͷͼƬ
	IMAGE body_img;					//����ͼƬ
	void ChangeOrient(int key);
	bool ClearNum(Map& m, Food& f);
	bool MoveSnake(Map& m, Food& f);
	void DisplayInfo(int num, int total);
	coordinate GetHead();
	Snake(int key, Map& m, int startx, int starty, int no, int life, TCHAR s_name[]);//������ʽ����ͼ����ͷ��ʼ��x����ͷ��ʼ��y
	~Snake();
};