#define _CRT_SECURE_NO_WARNINGS
#include <math.h>
#include "snake_class.h"

BasicImg::BasicImg()
{
	loadimage(&info_bk, _T("./image/info_bk.png"), SHOW_WIDTH / 6, INFO_HEIGHT_LINE * INFO_LINE_NUM);
	loadimage(&PAUSE_SMALL, _T("./image/PAUSE.png"), PAUSE_SIDE_LENGTH, PAUSE_SIDE_LENGTH);
	loadimage(&PAUSE_BIG, _T("./image/PAUSE.png"), PAUSE_SIDE_LENGTH_GETBIG, PAUSE_SIDE_LENGTH_GETBIG);
}
void BasicImg::DisplayInfoBK(int num, TCHAR name[][MAX_NAME_LENGTH])
{//传参在屏幕一共几部分背景图（即多少玩家）
	//设置字体颜色
	const LOGFONT chara = { INFO_HEIGHT_LINE * 2 / 3,0,0,0,FW_BLACK,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,DEFAULT_PITCH, _T("Comic Sans MS") };//字体高度与位置不随长宽比例变化
	settextstyle(&chara);
	settextcolor(BLACK);
	SetWorkingImage(NULL);//直接显示在屏幕上
	int i;//当前第几部分背景图
	for (i = 0; i < num; ++i) {
		putimage((SHOW_WIDTH / num) * i, 0, &info_bk);
		outtextxy((SHOW_WIDTH / num) * (i + 1) - textwidth(name[i]) - 10, INFO_HEIGHT_LINE * INFO_LINE_NUM - textheight(name[i]), (name[i]));
		//打印名字
	}
	DisplayPause(false);
}
void BasicImg::DisplayPause(bool is_big)
{//显示暂停键，传参true为变大（鼠标悬停）
	solidrectangle(SHOW_WIDTH - PAUSE_SIDE_LENGTH_GETBIG, 0, SHOW_WIDTH, PAUSE_SIDE_LENGTH_GETBIG);//首先用背景覆盖
	if (is_big) {
		putimage(SHOW_WIDTH - PAUSE_SIDE_LENGTH_GETBIG, 0, &PAUSE_BIG);
	}
	else
		putimage(SHOW_WIDTH - PAUSE_SIDE_LENGTH, 0, &PAUSE_SMALL);
}

Map::Map(int this_level, int xblock, int yblock) :level(this_level), CANVAS_XBLOCK(xblock), CANVAS_YBLOCK(yblock), CANVAS_WIDTH(xblock* SIDE_LENGTH + 2 * BOUNDARY), CANVAS_HEIGHT(yblock* SIDE_LENGTH + 2 * BOUNDARY), runtime(0)
{
	//MAP_IMG+ GROUND图像加载
	SetWorkingImage(MAP_IMG + GROUND);
	Resize(MAP_IMG + GROUND, SIDE_LENGTH, SIDE_LENGTH);
	setbkcolor(BK_COLOR);
	cleardevice();//背景填充

	loadimage(MAP_IMG + SOFT_WALL, _T("./image/soft_wall.png"), SIDE_LENGTH, SIDE_LENGTH);
	loadimage(MAP_IMG + HARD_WALL, _T("./image/hard_wall.png"), SIDE_LENGTH, SIDE_LENGTH);
	loadimage(MAP_IMG + FAST_GROUND, _T("./image/fast_region.png"), SIDE_LENGTH, SIDE_LENGTH);
	loadimage(MAP_IMG + SLOW_GROUND, _T("./image/slow_region.png"), SIDE_LENGTH, SIDE_LENGTH);
	canvas = new(nothrow) IMAGE(CANVAS_WIDTH, CANVAS_HEIGHT);
	if (!canvas) {
		cerr << "Space Error!" << endl;
		exit(-1986);
	}
	TERRITORY = new(nothrow)char[CANVAS_XBLOCK * CANVAS_YBLOCK]();
	if (!TERRITORY) {
		cerr << "Space Error!" << endl;
		exit(-1986);
	}

	//初始化canvas背景
	SetWorkingImage(canvas);
	setbkcolor(BK_COLOR);
	cleardevice();//背景填充
	setlinecolor(BOUNDARY_COLOR);//边界颜色
	setlinestyle(PS_SOLID, BOUNDARY);
	line(0 + BOUNDARY / 2, 0, 0 + BOUNDARY / 2, CANVAS_HEIGHT);//最左侧线
	line(0, CANVAS_HEIGHT - BOUNDARY / 2, CANVAS_WIDTH, CANVAS_HEIGHT - BOUNDARY / 2);//最下侧线
	line(CANVAS_WIDTH - BOUNDARY / 2, 0, CANVAS_WIDTH - BOUNDARY / 2, CANVAS_HEIGHT);//最右侧线
	line(0, 0 + BOUNDARY / 2, CANVAS_WIDTH, 0 + BOUNDARY / 2);//最上侧线
}
Map::~Map()
{
	delete[]TERRITORY;
	delete canvas;
}
void Map::Refresh(coordinate Block, IMAGE* cover)//使用前应先设置绘图设备
{//缺省为恢复为背景，否则用传入图像覆盖。BLOCK为块坐标
	putimage(Block.x * SIDE_LENGTH + BOUNDARY, Block.y * SIDE_LENGTH + BOUNDARY, &MAP_IMG[TERRITORY[Block.x + Block.y * CANVAS_XBLOCK]]);
	//首先用背景覆盖一次保证消除
	if (cover)
		putimage(Block.x * SIDE_LENGTH + BOUNDARY, Block.y * SIDE_LENGTH + BOUNDARY, cover);
	//将指定元素（正方形为单位）加入图像缓冲区
}
void Map::Display(const Snake& s, int showwidth, int startx, int starty)
{//蛇头在canvas中坐标（即画面中心点）（块坐标），显示宽度（像素宽度），显示区域左上角x坐标（像素坐标），显示区域左上角y坐标（像素坐标）。缺省为单人模式
	/******************显示图像部分（canvas）******************/
	coordinate point = { s.body.head->place.x * SIDE_LENGTH + SIDE_LENGTH / 2 + SIDE_LENGTH % 2/*中心点坐标*/ - showwidth / 2 + (showwidth % 2 == 0)
		,s.body.head->place.y * SIDE_LENGTH + SIDE_LENGTH / 2 + SIDE_LENGTH % 2/*中心点坐标*/ - SHOW_HEIGHT / 2 + (SHOW_HEIGHT % 2 == 0) };//绘制内容在canvas全画布中的左上角坐标（像素坐标）
	if (point.x < 0)
		point.x = 0;
	else if (point.x > CANVAS_WIDTH - (showwidth - 1))
		point.x = CANVAS_WIDTH - (showwidth - 1);
	if (point.y < 0)
		point.y = 0;
	else if (point.y > CANVAS_HEIGHT - (SHOW_HEIGHT - 1))
		point.y = CANVAS_HEIGHT - (SHOW_HEIGHT - 1);
	SetWorkingImage(NULL);
	putimage(startx, INFO_HEIGHT_LINE * INFO_LINE_NUM + starty, showwidth, SHOW_HEIGHT, canvas, point.x, point.y);
	SetWorkingImage(canvas);//每次只在display使用默认

}
void Map::RandomRegion()
{//随机初始化地图区块（初始游戏时使用）
	int size = CANVAS_XBLOCK * CANVAS_YBLOCK;
	int width;//区块宽度
	int height;//区块高度
	int number;//区块数量
	int i, j, k, startx, starty, place;//生成区域的左上角为第几块
	//优先加减速区，区块数量根据地图大小动态调整上限，后生成可以覆盖先生成//不采用循环，每一部分可以随意调整游戏参数（生成概率，大小，覆盖关系等）
	for (i = 0, number = rand() % (4 * (CANVAS_XBLOCK / SHOW_XBLOCK) * (CANVAS_XBLOCK / SHOW_XBLOCK)) + 1; i < number; ++i) {//减速区
		width = rand() % 6 + 1;
		height = rand() % 5 + 1;
		place = rand() % size;
		startx = place % CANVAS_YBLOCK;
		starty = place / CANVAS_YBLOCK;
		for (j = startx; j < startx + width && j < CANVAS_XBLOCK; ++j) {//从区块左上角开始第[几]行
			for (k = starty; k < starty + height && k < CANVAS_YBLOCK; ++k) {//从区块左上角开始第[几]列
				TERRITORY[k * CANVAS_XBLOCK + j] = SLOW_GROUND;
				coordinate p = { j,k };
				Refresh(p, MAP_IMG + SLOW_GROUND);
			}
		}
	}
	for (i = 0, number = rand() % (5 * (CANVAS_XBLOCK / SHOW_XBLOCK) * (CANVAS_XBLOCK / SHOW_XBLOCK)) + 1; i < number; ++i) {//加速区
		width = rand() % 8 + 1;
		height = rand() % 5 + 1;
		place = rand() % size;
		startx = place % CANVAS_YBLOCK;
		starty = place / CANVAS_YBLOCK;
		for (j = startx; j < startx + width && j < CANVAS_XBLOCK; ++j) {//从区块左上角开始第[几]行
			for (k = starty; k < starty + height && k < CANVAS_YBLOCK; ++k) {//从区块左上角开始第[几]列
				TERRITORY[k * CANVAS_XBLOCK + j] = FAST_GROUND;
				coordinate p = { j,k };
				Refresh(p, MAP_IMG + FAST_GROUND);
			}
		}
	}
	for (i = 0, number = rand() % (4 * (CANVAS_XBLOCK / SHOW_XBLOCK) * (CANVAS_XBLOCK / SHOW_XBLOCK)) + 1; i < number; ++i) {//软墙
		int probibility = rand() % 5;//此处为提高一格墙的概率，即横墙：竖墙：方格墙=2：2：1
		if (probibility == 0) {
			width = rand() % 5 + 1;
			height = rand() % 5 + 1;
		}
		else if (probibility <= 2) {
			width = 1;
			height = rand() % 6 + 1;
		}
		else if (probibility <= 4) {
			width = rand() % 6 + 1;
			height = 1;
		}
		place = rand() % size;
		startx = place % CANVAS_YBLOCK;
		starty = place / CANVAS_YBLOCK;
		for (j = startx; j < startx + width && j < CANVAS_XBLOCK; ++j) {//从区块左上角开始第[几]行
			for (k = starty; k < starty + height && k < CANVAS_YBLOCK; ++k) {//从区块左上角开始第[几]列
				TERRITORY[k * CANVAS_XBLOCK + j] = SOFT_WALL;
				coordinate p = { j,k };
				Refresh(p, MAP_IMG + SOFT_WALL);
			}
		}
	}
	for (i = 0, number = rand() % (5 * (CANVAS_XBLOCK / SHOW_XBLOCK) * (CANVAS_XBLOCK / SHOW_XBLOCK)) + 1; i < number; ++i) {//硬墙
		int probibility = rand() % 5;//此处为提高一格宽墙的概率，即横墙：竖墙：方格墙=2：2：1
		if (probibility == 0) {
			width = rand() % 5 + 1;
			height = rand() % 5 + 1;
		}
		else if (probibility <= 2) {
			width = 1;
			height = rand() % 6 + 1;
		}
		else if (probibility <= 4) {
			width = rand() % 6 + 1;
			height = 1;
		}
		place = rand() % size;
		startx = place % CANVAS_YBLOCK;
		starty = place / CANVAS_YBLOCK;
		for (j = startx; j < startx + width && j < CANVAS_XBLOCK; ++j) {//从区块左上角开始第[几]行
			for (k = starty; k < starty + height && k < CANVAS_YBLOCK; ++k) {//从区块左上角开始第[几]列
				TERRITORY[k * CANVAS_XBLOCK + j] = HARD_WALL;
				coordinate p = { j,k };
				Refresh(p, MAP_IMG + HARD_WALL);
			}
		}
	}
}

Food::Food(Map& m) :foodnum{ 0,0,0 }
{
	loadimage(FOOD_IMG + BAD, _T("./image/bad_food.png"), SIDE_LENGTH, SIDE_LENGTH);
	loadimage(FOOD_IMG + SIMPLE, _T("./image/simple_food.png"), SIDE_LENGTH, SIDE_LENGTH);
	loadimage(FOOD_IMG + GOOD, _T("./image/good_food.png"), SIDE_LENGTH, SIDE_LENGTH);
	loadimage(FOOD_IMG + EXCELLENT, _T("./image/excellent_food.png"), SIDE_LENGTH, SIDE_LENGTH);
	RandomGenerateFood(m);//初始情况空间足够
}
bool Food::RandomGenerateFood(Map& tofood)
{
	coordinate add;//食物生成位置块坐标
	int j, time = 0;//搜索尝试次数
	//const int badnum = rand() % BAD_MAX + 1;//期望生成的坏果数量
	//const int simnum = rand() % SIMPLE_MAX + 1;//期望生成的普通果数量
	//const int goodnum = rand() % GOOD_MAX + 1;//期望生成的好果数量
	//const int excenum = rand() % EXCELLENT_MAX + 1;//期望生成的极好果数量
	int expect;//期望果实数量
	for (j = 0; j < 4; ++j) {//j代表此次循环增添食物的编号
		expect = rand() % FOOD_MAX[j] + 1;//期望j号果的最大数量
		while (foodnum[j] < expect) {
			add.x = rand() % tofood.CANVAS_XBLOCK;
			add.y = rand() % tofood.CANVAS_YBLOCK;
			int coor = add.y * tofood.CANVAS_XBLOCK + add.x;//一维数组下标

			//if (tofood.TERRITORY[coor] == SOFT_WALL || tofood.TERRITORY[coor] == HARD_WALL)
			//	continue;//此处为墙
			//if (JudgeFruit(add, tofood.CANVAS_XBLOCK) != NO_FOOD)
			//	continue;//此处为食物
			//for (i = 0; i < snakenum; ++i)
			//	if (s[i].JudgeBody(add))
			//		break;
			//if (i < snakenum)
			//	continue;//此处有蛇
			if (tofood.TERRITORY[coor] < SOFT_WALL && JudgeFood(add, tofood.CANVAS_XBLOCK) == NO_FOOD) {//此处可以添加食物
				allfood[coor] = j;//添加的食物加至哈希表
				tofood.Refresh(add, &FOOD_IMG[j]);//在画布上更新食物
				++foodnum[j];
				//				--tofood.space;//剩余空间减一
			}
			else
				++time;//失败便利次数加一
			if (time > 100000)//认为画面已满
				return false;
		}
	}
	return true;
}
char Food::JudgeFood(coordinate tojudge, int canvas_width)//判断食物种类有无
{
	if (allfood.find(tojudge.y * canvas_width/*CANVAS_XBLOCK*/ + tojudge.x) != allfood.end())//此处找到食物
		return allfood.find(tojudge.y * canvas_width/*CANVAS_XBLOCK*/ + tojudge.x)->second;//返回食物种类编号
	else//此处无食物
		return NO_FOOD;//返回-1
}
void Food::EraseFood(coordinate tojudge, Map& m)
{
	--foodnum[allfood.find(tojudge.y * m.CANVAS_XBLOCK + tojudge.x)->second];
	allfood.erase(tojudge.y * m.CANVAS_XBLOCK + tojudge.x);
	return;
}
void Food::DestroyAllFood(Map& tofood)
{
	//	tofood.space += allfood.size();
	coordinate erase;//消除食物位置块坐标
	for (unordered_map<int, int>::iterator p = allfood.begin(); p != allfood.end(); ++p) {
		erase.x = p->first % tofood.CANVAS_XBLOCK;
		erase.y = p->first / tofood.CANVAS_XBLOCK;
		tofood.Refresh(erase);//在画布上更新食物
	}
	allfood.clear();
	foodnum[0] = foodnum[1] = foodnum[2] = foodnum[3] = 0;
}
void Food::CreateFood(coordinate tofood, Map& m)
{
	int foodtype = rand() % 4;
	allfood[tofood.y * m.CANVAS_XBLOCK + tofood.x] = foodtype;//添加的食物加至哈希表
	m.Refresh(tofood, &FOOD_IMG[foodtype]);//在画布上更新食物
	++foodnum[foodtype];
}
int Food::GetGoodFoodNum()
{
	return foodnum[SIMPLE] + foodnum[GOOD] + foodnum[EXCELLENT];
}

Snake::Snake(int key, Map& m, int startx, int starty, int no, int ini_life, TCHAR s_name[]) :/*length(INI_LENGTH)*/orientation(UP), this_orientation(UP), operate(key), length(INI_LENGTH), spirit(MAX_SPIRIT), score(0), speed(1), life(ini_life), count(0)
{//操作方式，地图，蛇头初始块坐标x，蛇头初始块坐标y，蛇种类编号
	for (int i = 0; i < MAX_NAME_LENGTH; ++i)
		name[i] = s_name[i];
	TCHAR  str[35];
	_stprintf(str, _T("./image/snake_head%1d_up.png"), no);
	loadimage(&head_img[UP], str, SIDE_LENGTH, SIDE_LENGTH);
	_stprintf(str, _T("./image/snake_head%1d_right.png"), no);
	loadimage(&head_img[RIGHT], str, SIDE_LENGTH, SIDE_LENGTH);
	_stprintf(str, _T("./image/snake_head%1d_left.png"), no);
	loadimage(&head_img[LEFT], str, SIDE_LENGTH, SIDE_LENGTH);
	_stprintf(str, _T("./image/snake_head%1d_down.png"), no);
	loadimage(&head_img[DOWN], str, SIDE_LENGTH, SIDE_LENGTH);
	_stprintf(str, _T("./image/snake_body%1d.png"), no);
	loadimage(&body_img, str, SIDE_LENGTH, SIDE_LENGTH);
	coordinate p = { startx,starty };
	InitSnake(m, p);
}
Snake::~Snake()
{
	while (body.tail) {
		body.head = body.tail->next;
		delete body.tail;
		body.tail = body.head;
	}
}
void Snake::InitSnake(Map& m, coordinate start)	//构造一个3长蛇的链队列，块坐标
{
	body.head = body.tail = new(nothrow)QNode;
	if (!body.tail) {
		cerr << "Space Error!" << endl;
		exit(-1986);
	}//存储分配失败
	body.head->place = start;
	body.head->next = NULL;
	m.Refresh(start, head_img + orientation);
	if (m.TERRITORY[body.head->place.y * m.CANVAS_XBLOCK + body.head->place.x] <= GROUND_NUM)//该处为各种类型地
		m.TERRITORY[body.head->place.y * m.CANVAS_XBLOCK + body.head->place.x] += 5;
	else//该处为墙
		m.TERRITORY[body.head->place.y * m.CANVAS_XBLOCK + body.head->place.x] = SNAKE_ORDI;


	for (int i = 0; i < length; ++i) {
		Qptr p = new(nothrow)QNode;
		if (!p) {
			cerr << "Space Error!" << endl;
			exit(-1986);
		}//存储分配失败
		p->place = body.tail->place;


		switch (orientation) {
			case LEFT:
				++p->place.x;
				break;
			case RIGHT:
				--p->place.x;
				break;
			case UP:
				++p->place.y;
				break;
			case DOWN:
				--p->place.y;
				break;
			default://希望不可能执行到这一句
				cout << "怎么搞的，方向怎么还出非法值了";
				exit('?');
		}


		p->next = body.tail;
		body.tail = p;
		m.Refresh(p->place, &body_img);
		if (m.TERRITORY[p->place.y * m.CANVAS_XBLOCK + p->place.x] <= GROUND_NUM) {//该处为各种类型地
			m.TERRITORY[p->place.y * m.CANVAS_XBLOCK + p->place.x] += 5;
			//			--m.space;
		}
		else//该处为墙
			m.TERRITORY[p->place.y * m.CANVAS_XBLOCK + p->place.x] = SNAKE_ORDI;

	}
	for (int k = 0; k < m.CANVAS_YBLOCK; ++k) {//从区块左上角开始第[几]列
		for (int j = 0; j < m.CANVAS_XBLOCK; ++j) {//从区块左上角开始第[几]行
			cout << int(m.TERRITORY[k * m.CANVAS_XBLOCK + j]) << ' ';
		}
		cout << endl;
	}
	cout << endl;
}
void Snake::ChangeOrient(int key)
{
	if (operate) {
		if (key == VK_UP && orientation != DOWN)//UP ARROW 键
			this_orientation = UP;
		else if (key == VK_DOWN && orientation != UP)//DOWN ARROW 键
			this_orientation = DOWN;
		else if (key == VK_LEFT && orientation != RIGHT)//LEFT ARROW 键
			this_orientation = LEFT;
		else if (key == VK_RIGHT && orientation != LEFT)//RIGHT ARROW 键
			this_orientation = RIGHT;
	}
	else
		if (key == 0x57 && orientation != DOWN)
			this_orientation = UP;
		else if (key == 0x53 && orientation != UP)
			this_orientation = DOWN;
		else if (key == 0x41 && orientation != RIGHT)
			this_orientation = LEFT;
		else if (key == 0x44 && orientation != LEFT)
			this_orientation = RIGHT;
}
bool Snake::ClearNum(Map& m, Food& f)//返回值为游戏是否结束
{//处理数值可能小于0
	coordinate nowhead = body.head->place;
	if (score < 0)
		score &= 0;//处理得分
	if (spirit <= 0 || length <= 0) {
		spirit = MAX_SPIRIT;
		length = INI_LENGTH;
		--life;
		DestroySnake(m, f);
		if (life > 0)
			if (!RandomGenerateSnake(m, f)) {//空间不足
				body.head = body.tail = new(nothrow)QNode;
				if (!body.head) {
					cerr << "Space Error!" << endl;
					exit(-1986);
				}
				body.head->place = nowhead;
				body.head->next = NULL;
				return false;
			}
	}
	if (life <= 0) {
		DestroySnake(m, f);
		body.head = body.tail = new(nothrow)QNode;
		if (!body.head) {
			cerr << "Space Error!" << endl;
			exit(-1986);
		}
		body.head->place = nowhead;
		body.head->next = NULL;
		return false;
	}
	else
		return true;
}
bool Snake::MoveSnake(Map& m, Food& f)//返回值为游戏是否继续
{
	SetWorkingImage(m.canvas);
	coordinate nowhead = body.head->place;
	char fronttype;//前方地形
	char foodtype;//前方食物
	int i;

	if (speed == WALL && this_orientation != orientation) {
		speed = FAST;//撞墙后若有方向改变立刻移动
	}

	if (count >= speed) {
		orientation = this_orientation;//改变方向
		fronttype = LookForward(m);//要移动到区块类型
		if (fronttype == SOLID) {//边界
			score += BOUNDARY_SCORE;
			length = -1;//标记已死
		}
		else if (fronttype == SOFT_WALL) {//软墙
			score += SOFT_SCORE;
			length -= SOFT_HURT;
			if (length > 0)//本命仍活着
				for (i = 0; i < SOFT_HURT; ++i) {
					m.Refresh(MoveTail(m), NULL);//删并画蛇尾
				}
		}
		else if (fronttype == HARD_WALL) {//硬墙
			score += HARD_SCORE;
			i = length;//记录原始长度
			length /= 2;
			if (length > 0)//长度够长，活
				for (; i > length; --i) {
					m.Refresh(MoveTail(m), NULL);//画蛇尾
				}
		}
		else if (fronttype <= GROUND_NUM || JudgeBody(nowhead) == IS_TAIL) {//前方为安全地形
			m.Refresh(nowhead, &body_img);//覆盖蛇头
			nowhead = MoveHead(m);//更新蛇头链表，不画蛇头
			foodtype = f.JudgeFood(nowhead, m.CANVAS_XBLOCK);
			if (foodtype != NO_FOOD) {//前方有食物，此时只需要改蛇精神状态参数，画蛇头（直接覆盖食物），无需画蛇尾
				f.EraseFood(nowhead, m);
				spirit += f.spiritadd[foodtype];
				life += f.lifeadd[foodtype];
				score += f.scoreadd[foodtype];
				m.Refresh(nowhead, head_img + orientation);//画蛇头（直接覆盖食物）
				if (foodtype == BAD) {
					life &= 0;
					m.Refresh(MoveTail(m), NULL);//画蛇尾
					return false;//恶果吃到直接结束游戏
				}
				++length;
			}
			else if (fronttype <= GROUND_NUM) {//前方为地面，即不是蛇尾
				m.Refresh(nowhead, head_img + orientation);//画蛇头
				m.Refresh(MoveTail(m), NULL);//画蛇尾
			}
			else {//前方为本蛇蛇尾
				MoveTail(m);//移蛇尾
				m.Refresh(nowhead, head_img + orientation);//画蛇头
			}
		}
		else//前方为自己蛇身，前方为其它蛇蛇尾
			length = -1;//标记死亡
		count &= 0;
		//更新速度
		if (fronttype == GROUND)
			speed = NORMAL;
		else if (fronttype == FAST_GROUND)
			speed = FAST;
		else if (fronttype == SLOW_GROUND)
			speed = SLOW;
		else if (fronttype == SOFT_WALL || fronttype == HARD_WALL)
			speed = WALL;
	}
	else
		++count;
	spirit -= SPIRIT_DECREASE;
	return true;
}
char Snake::LookForward(const Map& m)
{//判断下一步要走的地方是什么
	coordinate next = body.head->place;
	switch (orientation) {
		case LEFT:
			--next.x;
			break;
		case RIGHT:
			++next.x;
			break;
		case UP:
			--next.y;
			break;
		case DOWN:
			++next.y;
			break;
		default://希望不可能执行到这一句
			cout << "怎么搞的，方向怎么还出非法值了";
			exit('?');
	}
	if (next.x < 0 || next.x >= m.CANVAS_XBLOCK || next.y < 0 || next.y >= m.CANVAS_YBLOCK)//撞边界了
		return SOLID;
	else
		return m.TERRITORY[next.y * m.CANVAS_XBLOCK + next.x];
}
coordinate Snake::MoveHead(Map& m)//蛇头向前移动一格，返回该格坐标（块坐标）
{
	Qptr p = new(nothrow)QNode;
	if (!p) {
		cerr << "Space Error!" << endl;
		exit(0);
	}//存储分配失败
	p->place = body.head->place;
	switch (orientation) {
		case LEFT:
			--p->place.x;
			break;
		case RIGHT:
			++p->place.x;
			break;
		case UP:
			--p->place.y;
			break;
		case DOWN:
			++p->place.y;
			break;
	}
	p->next = NULL;
	body.head->next = p;
	body.head = p;
	m.TERRITORY[p->place.y * m.CANVAS_XBLOCK + p->place.x] += (SNAKE_ORDI - GROUND);
	return p->place;
}
coordinate Snake::MoveTail(Map& m)//蛇尾向前移动一格，返回该尾坐标（块坐标）
{
	coordinate coor = body.tail->place;//待删除蛇尾坐标
	Qptr p = body.tail;//保存待删除蛇尾地址
	body.tail = body.tail->next;
	delete p;
	m.TERRITORY[coor.y * m.CANVAS_XBLOCK + coor.x] -= (SNAKE_ORDI - GROUND);
	return coor;
}
void Snake::DisplayInfo(int num, int total)
{//第[几]条蛇的信息，总共几条蛇
	const LOGFONT chara = { INFO_HEIGHT_LINE * 2 / 3,0,0,0,FW_BLACK,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,DEFAULT_PITCH, _T("Comic Sans MS") };//字体高度与位置不随长宽比例变化
	settextstyle(&chara);
	settextcolor(BLACK);
	SetWorkingImage(NULL);//直接显示在屏幕上
	//覆盖上一次得分
	solidrectangle(SHOW_WIDTH / 6 + SHOW_WIDTH * num / total, 0, SHOW_WIDTH * (num + 1) / total - SHOW_WIDTH / total / 3, INFO_HEIGHT_LINE * INFO_LINE_NUM - 1);
	TCHAR str[20];
	//打印得分
	_stprintf(str, _T("%d\0"), score);
	outtextxy(SHOW_WIDTH / 6 + SHOW_WIDTH * num / total, (INFO_HEIGHT_LINE - textheight(str)) / 2, str);
	//打印长度
	_stprintf(str, _T("%d\0"), length + 1);
	outtextxy(SHOW_WIDTH / 6 + SHOW_WIDTH * num / total, INFO_HEIGHT_LINE + (INFO_HEIGHT_LINE - textheight(str)) / 2, str);
	//打印生命
	_stprintf(str, _T("%d\0"), life);
	outtextxy(SHOW_WIDTH / 6 + SHOW_WIDTH * num / total, INFO_HEIGHT_LINE * 2 + (INFO_HEIGHT_LINE - textheight(str)) / 2, str);
	//打印体力
	_stprintf(str, _T("%d\0"), spirit);
	outtextxy(SHOW_WIDTH / 6 + SHOW_WIDTH * num / total, INFO_HEIGHT_LINE * 3 + (INFO_HEIGHT_LINE - textheight(str)) / 2, str);

}
coordinate Snake::GetHead()
{
	return body.head->place;
}
void Snake::DestroySnake(Map& m, Food& f)	//销毁队列
{
	while (body.tail) {
		body.head = body.tail->next;
		m.TERRITORY[body.tail->place.y * m.CANVAS_XBLOCK + body.tail->place.x] -= (SNAKE_ORDI - GROUND);
		if (m.level == LEVEL1)
			m.Refresh(body.tail->place);
		else if (m.level == LEVEL2) {
			m.TERRITORY[body.tail->place.y * m.CANVAS_XBLOCK + body.tail->place.x] = HARD_WALL;
			m.Refresh(body.tail->place);
		}
		else if (m.level == LEVEL3) {
			f.CreateFood(body.tail->place, m);
		}
		delete body.tail;
		body.tail = body.head;
	}
}
bool Snake::RandomGenerateSnake(Map& m, Food& f)	//销毁队列
{
	length = INI_LENGTH;
	int i = 0;
	while (i++ < 1000) {
		coordinate coor = { rand() % m.CANVAS_XBLOCK ,rand() % m.CANVAS_YBLOCK };
		coordinate now;
		for (now = coor; now.y - coor.y < 3 && now.y < m.CANVAS_YBLOCK; ++now.y)//头方向为UP
			if (m.TERRITORY[now.y * m.CANVAS_XBLOCK + now.x] >= SOFT_WALL || f.JudgeFood(now, m.CANVAS_XBLOCK) != NO_FOOD) {
				break;
			}
		if (now.y - coor.y >= 3) {
			this_orientation = orientation = UP;
			speed = FAST;//死亡后立刻能走
			InitSnake(m, coor);
			return true;
		}
		for (now = coor; now.y - coor.y > -3 && now.y >= 0; --now.y)//头方向为DOWN
			if (m.TERRITORY[now.y * m.CANVAS_XBLOCK + now.x] >= SOFT_WALL || f.JudgeFood(now, m.CANVAS_XBLOCK) != NO_FOOD) {
				break;
			}
		if (now.y - coor.y <= -3) {
			this_orientation = orientation = DOWN;
			InitSnake(m, coor);
			return true;
		}
		for (now = coor; now.x - coor.x < 3 && now.x < m.CANVAS_XBLOCK; ++now.x)//头方向为LEFT
			if (m.TERRITORY[now.y * m.CANVAS_XBLOCK + now.x] >= SOFT_WALL || f.JudgeFood(now, m.CANVAS_XBLOCK) != NO_FOOD) {
				break;
			}
		if (now.x - coor.x >= 3) {
			this_orientation = orientation = LEFT;
			InitSnake(m, coor);
			return true;
		}
		for (now = coor; now.x - coor.x > -3 && now.x >= 0; --now.x)//头方向为RIGHT
			if (m.TERRITORY[now.y * m.CANVAS_XBLOCK + now.x] >= SOFT_WALL || f.JudgeFood(now, m.CANVAS_XBLOCK) != NO_FOOD) {
				break;
			}
		if (now.x - coor.x <= -3) {
			this_orientation = orientation = RIGHT;
			InitSnake(m, coor);
			return true;
		}
	}
	return false;
}
char Snake::JudgeBody(coordinate tojudge)//判断tojudge处是否为蛇身
{//0代表无蛇，1代表蛇头与身，2代表蛇尾
	if (body.tail->place == tojudge)
		return IS_TAIL;
	else {
		Qptr p = body.tail->next;
		while (p != NULL) {
			if (p->place == tojudge)
				return IS_HEADBODY;
			else
				p = p->next;
		}
		return NO_SNAKE;
	}
}