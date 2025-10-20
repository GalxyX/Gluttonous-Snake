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
{//��������Ļһ�������ֱ���ͼ����������ң�
	//����������ɫ
	const LOGFONT chara = { INFO_HEIGHT_LINE * 2 / 3,0,0,0,FW_BLACK,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,DEFAULT_PITCH, _T("Comic Sans MS") };//����߶���λ�ò��泤������仯
	settextstyle(&chara);
	settextcolor(BLACK);
	SetWorkingImage(NULL);//ֱ����ʾ����Ļ��
	int i;//��ǰ�ڼ����ֱ���ͼ
	for (i = 0; i < num; ++i) {
		putimage((SHOW_WIDTH / num) * i, 0, &info_bk);
		outtextxy((SHOW_WIDTH / num) * (i + 1) - textwidth(name[i]) - 10, INFO_HEIGHT_LINE * INFO_LINE_NUM - textheight(name[i]), (name[i]));
		//��ӡ����
	}
	DisplayPause(false);
}
void BasicImg::DisplayPause(bool is_big)
{//��ʾ��ͣ��������trueΪ��������ͣ��
	solidrectangle(SHOW_WIDTH - PAUSE_SIDE_LENGTH_GETBIG, 0, SHOW_WIDTH, PAUSE_SIDE_LENGTH_GETBIG);//�����ñ�������
	if (is_big) {
		putimage(SHOW_WIDTH - PAUSE_SIDE_LENGTH_GETBIG, 0, &PAUSE_BIG);
	}
	else
		putimage(SHOW_WIDTH - PAUSE_SIDE_LENGTH, 0, &PAUSE_SMALL);
}

Map::Map(int this_level, int xblock, int yblock) :level(this_level), CANVAS_XBLOCK(xblock), CANVAS_YBLOCK(yblock), CANVAS_WIDTH(xblock* SIDE_LENGTH + 2 * BOUNDARY), CANVAS_HEIGHT(yblock* SIDE_LENGTH + 2 * BOUNDARY), runtime(0)
{
	//MAP_IMG+ GROUNDͼ�����
	SetWorkingImage(MAP_IMG + GROUND);
	Resize(MAP_IMG + GROUND, SIDE_LENGTH, SIDE_LENGTH);
	setbkcolor(BK_COLOR);
	cleardevice();//�������

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

	//��ʼ��canvas����
	SetWorkingImage(canvas);
	setbkcolor(BK_COLOR);
	cleardevice();//�������
	setlinecolor(BOUNDARY_COLOR);//�߽���ɫ
	setlinestyle(PS_SOLID, BOUNDARY);
	line(0 + BOUNDARY / 2, 0, 0 + BOUNDARY / 2, CANVAS_HEIGHT);//�������
	line(0, CANVAS_HEIGHT - BOUNDARY / 2, CANVAS_WIDTH, CANVAS_HEIGHT - BOUNDARY / 2);//���²���
	line(CANVAS_WIDTH - BOUNDARY / 2, 0, CANVAS_WIDTH - BOUNDARY / 2, CANVAS_HEIGHT);//���Ҳ���
	line(0, 0 + BOUNDARY / 2, CANVAS_WIDTH, 0 + BOUNDARY / 2);//���ϲ���
}
Map::~Map()
{
	delete[]TERRITORY;
	delete canvas;
}
void Map::Refresh(coordinate Block, IMAGE* cover)//ʹ��ǰӦ�����û�ͼ�豸
{//ȱʡΪ�ָ�Ϊ�����������ô���ͼ�񸲸ǡ�BLOCKΪ������
	putimage(Block.x * SIDE_LENGTH + BOUNDARY, Block.y * SIDE_LENGTH + BOUNDARY, &MAP_IMG[TERRITORY[Block.x + Block.y * CANVAS_XBLOCK]]);
	//�����ñ�������һ�α�֤����
	if (cover)
		putimage(Block.x * SIDE_LENGTH + BOUNDARY, Block.y * SIDE_LENGTH + BOUNDARY, cover);
	//��ָ��Ԫ�أ�������Ϊ��λ������ͼ�񻺳���
}
void Map::Display(const Snake& s, int showwidth, int startx, int starty)
{//��ͷ��canvas�����꣨���������ĵ㣩�������꣩����ʾ��ȣ����ؿ�ȣ�����ʾ�������Ͻ�x���꣨�������꣩����ʾ�������Ͻ�y���꣨�������꣩��ȱʡΪ����ģʽ
	/******************��ʾͼ�񲿷֣�canvas��******************/
	coordinate point = { s.body.head->place.x * SIDE_LENGTH + SIDE_LENGTH / 2 + SIDE_LENGTH % 2/*���ĵ�����*/ - showwidth / 2 + (showwidth % 2 == 0)
		,s.body.head->place.y * SIDE_LENGTH + SIDE_LENGTH / 2 + SIDE_LENGTH % 2/*���ĵ�����*/ - SHOW_HEIGHT / 2 + (SHOW_HEIGHT % 2 == 0) };//����������canvasȫ�����е����Ͻ����꣨�������꣩
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
	SetWorkingImage(canvas);//ÿ��ֻ��displayʹ��Ĭ��

}
void Map::RandomRegion()
{//�����ʼ����ͼ���飨��ʼ��Ϸʱʹ�ã�
	int size = CANVAS_XBLOCK * CANVAS_YBLOCK;
	int width;//������
	int height;//����߶�
	int number;//��������
	int i, j, k, startx, starty, place;//������������Ͻ�Ϊ�ڼ���
	//���ȼӼ������������������ݵ�ͼ��С��̬�������ޣ������ɿ��Ը���������//������ѭ����ÿһ���ֿ������������Ϸ���������ɸ��ʣ���С�����ǹ�ϵ�ȣ�
	for (i = 0, number = rand() % (4 * (CANVAS_XBLOCK / SHOW_XBLOCK) * (CANVAS_XBLOCK / SHOW_XBLOCK)) + 1; i < number; ++i) {//������
		width = rand() % 6 + 1;
		height = rand() % 5 + 1;
		place = rand() % size;
		startx = place % CANVAS_YBLOCK;
		starty = place / CANVAS_YBLOCK;
		for (j = startx; j < startx + width && j < CANVAS_XBLOCK; ++j) {//���������Ͻǿ�ʼ��[��]��
			for (k = starty; k < starty + height && k < CANVAS_YBLOCK; ++k) {//���������Ͻǿ�ʼ��[��]��
				TERRITORY[k * CANVAS_XBLOCK + j] = SLOW_GROUND;
				coordinate p = { j,k };
				Refresh(p, MAP_IMG + SLOW_GROUND);
			}
		}
	}
	for (i = 0, number = rand() % (5 * (CANVAS_XBLOCK / SHOW_XBLOCK) * (CANVAS_XBLOCK / SHOW_XBLOCK)) + 1; i < number; ++i) {//������
		width = rand() % 8 + 1;
		height = rand() % 5 + 1;
		place = rand() % size;
		startx = place % CANVAS_YBLOCK;
		starty = place / CANVAS_YBLOCK;
		for (j = startx; j < startx + width && j < CANVAS_XBLOCK; ++j) {//���������Ͻǿ�ʼ��[��]��
			for (k = starty; k < starty + height && k < CANVAS_YBLOCK; ++k) {//���������Ͻǿ�ʼ��[��]��
				TERRITORY[k * CANVAS_XBLOCK + j] = FAST_GROUND;
				coordinate p = { j,k };
				Refresh(p, MAP_IMG + FAST_GROUND);
			}
		}
	}
	for (i = 0, number = rand() % (4 * (CANVAS_XBLOCK / SHOW_XBLOCK) * (CANVAS_XBLOCK / SHOW_XBLOCK)) + 1; i < number; ++i) {//��ǽ
		int probibility = rand() % 5;//�˴�Ϊ���һ��ǽ�ĸ��ʣ�����ǽ����ǽ������ǽ=2��2��1
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
		for (j = startx; j < startx + width && j < CANVAS_XBLOCK; ++j) {//���������Ͻǿ�ʼ��[��]��
			for (k = starty; k < starty + height && k < CANVAS_YBLOCK; ++k) {//���������Ͻǿ�ʼ��[��]��
				TERRITORY[k * CANVAS_XBLOCK + j] = SOFT_WALL;
				coordinate p = { j,k };
				Refresh(p, MAP_IMG + SOFT_WALL);
			}
		}
	}
	for (i = 0, number = rand() % (5 * (CANVAS_XBLOCK / SHOW_XBLOCK) * (CANVAS_XBLOCK / SHOW_XBLOCK)) + 1; i < number; ++i) {//Ӳǽ
		int probibility = rand() % 5;//�˴�Ϊ���һ���ǽ�ĸ��ʣ�����ǽ����ǽ������ǽ=2��2��1
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
		for (j = startx; j < startx + width && j < CANVAS_XBLOCK; ++j) {//���������Ͻǿ�ʼ��[��]��
			for (k = starty; k < starty + height && k < CANVAS_YBLOCK; ++k) {//���������Ͻǿ�ʼ��[��]��
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
	RandomGenerateFood(m);//��ʼ����ռ��㹻
}
bool Food::RandomGenerateFood(Map& tofood)
{
	coordinate add;//ʳ������λ�ÿ�����
	int j, time = 0;//�������Դ���
	//const int badnum = rand() % BAD_MAX + 1;//�������ɵĻ�������
	//const int simnum = rand() % SIMPLE_MAX + 1;//�������ɵ���ͨ������
	//const int goodnum = rand() % GOOD_MAX + 1;//�������ɵĺù�����
	//const int excenum = rand() % EXCELLENT_MAX + 1;//�������ɵļ��ù�����
	int expect;//������ʵ����
	for (j = 0; j < 4; ++j) {//j����˴�ѭ������ʳ��ı��
		expect = rand() % FOOD_MAX[j] + 1;//����j�Ź����������
		while (foodnum[j] < expect) {
			add.x = rand() % tofood.CANVAS_XBLOCK;
			add.y = rand() % tofood.CANVAS_YBLOCK;
			int coor = add.y * tofood.CANVAS_XBLOCK + add.x;//һά�����±�

			//if (tofood.TERRITORY[coor] == SOFT_WALL || tofood.TERRITORY[coor] == HARD_WALL)
			//	continue;//�˴�Ϊǽ
			//if (JudgeFruit(add, tofood.CANVAS_XBLOCK) != NO_FOOD)
			//	continue;//�˴�Ϊʳ��
			//for (i = 0; i < snakenum; ++i)
			//	if (s[i].JudgeBody(add))
			//		break;
			//if (i < snakenum)
			//	continue;//�˴�����
			if (tofood.TERRITORY[coor] < SOFT_WALL && JudgeFood(add, tofood.CANVAS_XBLOCK) == NO_FOOD) {//�˴��������ʳ��
				allfood[coor] = j;//��ӵ�ʳ�������ϣ��
				tofood.Refresh(add, &FOOD_IMG[j]);//�ڻ����ϸ���ʳ��
				++foodnum[j];
				//				--tofood.space;//ʣ��ռ��һ
			}
			else
				++time;//ʧ�ܱ���������һ
			if (time > 100000)//��Ϊ��������
				return false;
		}
	}
	return true;
}
char Food::JudgeFood(coordinate tojudge, int canvas_width)//�ж�ʳ����������
{
	if (allfood.find(tojudge.y * canvas_width/*CANVAS_XBLOCK*/ + tojudge.x) != allfood.end())//�˴��ҵ�ʳ��
		return allfood.find(tojudge.y * canvas_width/*CANVAS_XBLOCK*/ + tojudge.x)->second;//����ʳ��������
	else//�˴���ʳ��
		return NO_FOOD;//����-1
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
	coordinate erase;//����ʳ��λ�ÿ�����
	for (unordered_map<int, int>::iterator p = allfood.begin(); p != allfood.end(); ++p) {
		erase.x = p->first % tofood.CANVAS_XBLOCK;
		erase.y = p->first / tofood.CANVAS_XBLOCK;
		tofood.Refresh(erase);//�ڻ����ϸ���ʳ��
	}
	allfood.clear();
	foodnum[0] = foodnum[1] = foodnum[2] = foodnum[3] = 0;
}
void Food::CreateFood(coordinate tofood, Map& m)
{
	int foodtype = rand() % 4;
	allfood[tofood.y * m.CANVAS_XBLOCK + tofood.x] = foodtype;//��ӵ�ʳ�������ϣ��
	m.Refresh(tofood, &FOOD_IMG[foodtype]);//�ڻ����ϸ���ʳ��
	++foodnum[foodtype];
}
int Food::GetGoodFoodNum()
{
	return foodnum[SIMPLE] + foodnum[GOOD] + foodnum[EXCELLENT];
}

Snake::Snake(int key, Map& m, int startx, int starty, int no, int ini_life, TCHAR s_name[]) :/*length(INI_LENGTH)*/orientation(UP), this_orientation(UP), operate(key), length(INI_LENGTH), spirit(MAX_SPIRIT), score(0), speed(1), life(ini_life), count(0)
{//������ʽ����ͼ����ͷ��ʼ������x����ͷ��ʼ������y����������
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
void Snake::InitSnake(Map& m, coordinate start)	//����һ��3���ߵ������У�������
{
	body.head = body.tail = new(nothrow)QNode;
	if (!body.tail) {
		cerr << "Space Error!" << endl;
		exit(-1986);
	}//�洢����ʧ��
	body.head->place = start;
	body.head->next = NULL;
	m.Refresh(start, head_img + orientation);
	if (m.TERRITORY[body.head->place.y * m.CANVAS_XBLOCK + body.head->place.x] <= GROUND_NUM)//�ô�Ϊ�������͵�
		m.TERRITORY[body.head->place.y * m.CANVAS_XBLOCK + body.head->place.x] += 5;
	else//�ô�Ϊǽ
		m.TERRITORY[body.head->place.y * m.CANVAS_XBLOCK + body.head->place.x] = SNAKE_ORDI;


	for (int i = 0; i < length; ++i) {
		Qptr p = new(nothrow)QNode;
		if (!p) {
			cerr << "Space Error!" << endl;
			exit(-1986);
		}//�洢����ʧ��
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
			default://ϣ��������ִ�е���һ��
				cout << "��ô��ģ�������ô�����Ƿ�ֵ��";
				exit('?');
		}


		p->next = body.tail;
		body.tail = p;
		m.Refresh(p->place, &body_img);
		if (m.TERRITORY[p->place.y * m.CANVAS_XBLOCK + p->place.x] <= GROUND_NUM) {//�ô�Ϊ�������͵�
			m.TERRITORY[p->place.y * m.CANVAS_XBLOCK + p->place.x] += 5;
			//			--m.space;
		}
		else//�ô�Ϊǽ
			m.TERRITORY[p->place.y * m.CANVAS_XBLOCK + p->place.x] = SNAKE_ORDI;

	}
	for (int k = 0; k < m.CANVAS_YBLOCK; ++k) {//���������Ͻǿ�ʼ��[��]��
		for (int j = 0; j < m.CANVAS_XBLOCK; ++j) {//���������Ͻǿ�ʼ��[��]��
			cout << int(m.TERRITORY[k * m.CANVAS_XBLOCK + j]) << ' ';
		}
		cout << endl;
	}
	cout << endl;
}
void Snake::ChangeOrient(int key)
{
	if (operate) {
		if (key == VK_UP && orientation != DOWN)//UP ARROW ��
			this_orientation = UP;
		else if (key == VK_DOWN && orientation != UP)//DOWN ARROW ��
			this_orientation = DOWN;
		else if (key == VK_LEFT && orientation != RIGHT)//LEFT ARROW ��
			this_orientation = LEFT;
		else if (key == VK_RIGHT && orientation != LEFT)//RIGHT ARROW ��
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
bool Snake::ClearNum(Map& m, Food& f)//����ֵΪ��Ϸ�Ƿ����
{//������ֵ����С��0
	coordinate nowhead = body.head->place;
	if (score < 0)
		score &= 0;//����÷�
	if (spirit <= 0 || length <= 0) {
		spirit = MAX_SPIRIT;
		length = INI_LENGTH;
		--life;
		DestroySnake(m, f);
		if (life > 0)
			if (!RandomGenerateSnake(m, f)) {//�ռ䲻��
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
bool Snake::MoveSnake(Map& m, Food& f)//����ֵΪ��Ϸ�Ƿ����
{
	SetWorkingImage(m.canvas);
	coordinate nowhead = body.head->place;
	char fronttype;//ǰ������
	char foodtype;//ǰ��ʳ��
	int i;

	if (speed == WALL && this_orientation != orientation) {
		speed = FAST;//ײǽ�����з���ı������ƶ�
	}

	if (count >= speed) {
		orientation = this_orientation;//�ı䷽��
		fronttype = LookForward(m);//Ҫ�ƶ�����������
		if (fronttype == SOLID) {//�߽�
			score += BOUNDARY_SCORE;
			length = -1;//�������
		}
		else if (fronttype == SOFT_WALL) {//��ǽ
			score += SOFT_SCORE;
			length -= SOFT_HURT;
			if (length > 0)//�����Ի���
				for (i = 0; i < SOFT_HURT; ++i) {
					m.Refresh(MoveTail(m), NULL);//ɾ������β
				}
		}
		else if (fronttype == HARD_WALL) {//Ӳǽ
			score += HARD_SCORE;
			i = length;//��¼ԭʼ����
			length /= 2;
			if (length > 0)//���ȹ�������
				for (; i > length; --i) {
					m.Refresh(MoveTail(m), NULL);//����β
				}
		}
		else if (fronttype <= GROUND_NUM || JudgeBody(nowhead) == IS_TAIL) {//ǰ��Ϊ��ȫ����
			m.Refresh(nowhead, &body_img);//������ͷ
			nowhead = MoveHead(m);//������ͷ����������ͷ
			foodtype = f.JudgeFood(nowhead, m.CANVAS_XBLOCK);
			if (foodtype != NO_FOOD) {//ǰ����ʳ���ʱֻ��Ҫ���߾���״̬����������ͷ��ֱ�Ӹ���ʳ������軭��β
				f.EraseFood(nowhead, m);
				spirit += f.spiritadd[foodtype];
				life += f.lifeadd[foodtype];
				score += f.scoreadd[foodtype];
				m.Refresh(nowhead, head_img + orientation);//����ͷ��ֱ�Ӹ���ʳ�
				if (foodtype == BAD) {
					life &= 0;
					m.Refresh(MoveTail(m), NULL);//����β
					return false;//����Ե�ֱ�ӽ�����Ϸ
				}
				++length;
			}
			else if (fronttype <= GROUND_NUM) {//ǰ��Ϊ���棬��������β
				m.Refresh(nowhead, head_img + orientation);//����ͷ
				m.Refresh(MoveTail(m), NULL);//����β
			}
			else {//ǰ��Ϊ������β
				MoveTail(m);//����β
				m.Refresh(nowhead, head_img + orientation);//����ͷ
			}
		}
		else//ǰ��Ϊ�Լ�����ǰ��Ϊ��������β
			length = -1;//�������
		count &= 0;
		//�����ٶ�
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
{//�ж���һ��Ҫ�ߵĵط���ʲô
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
		default://ϣ��������ִ�е���һ��
			cout << "��ô��ģ�������ô�����Ƿ�ֵ��";
			exit('?');
	}
	if (next.x < 0 || next.x >= m.CANVAS_XBLOCK || next.y < 0 || next.y >= m.CANVAS_YBLOCK)//ײ�߽���
		return SOLID;
	else
		return m.TERRITORY[next.y * m.CANVAS_XBLOCK + next.x];
}
coordinate Snake::MoveHead(Map& m)//��ͷ��ǰ�ƶ�һ�񣬷��ظø����꣨�����꣩
{
	Qptr p = new(nothrow)QNode;
	if (!p) {
		cerr << "Space Error!" << endl;
		exit(0);
	}//�洢����ʧ��
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
coordinate Snake::MoveTail(Map& m)//��β��ǰ�ƶ�һ�񣬷��ظ�β���꣨�����꣩
{
	coordinate coor = body.tail->place;//��ɾ����β����
	Qptr p = body.tail;//�����ɾ����β��ַ
	body.tail = body.tail->next;
	delete p;
	m.TERRITORY[coor.y * m.CANVAS_XBLOCK + coor.x] -= (SNAKE_ORDI - GROUND);
	return coor;
}
void Snake::DisplayInfo(int num, int total)
{//��[��]���ߵ���Ϣ���ܹ�������
	const LOGFONT chara = { INFO_HEIGHT_LINE * 2 / 3,0,0,0,FW_BLACK,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,DEFAULT_PITCH, _T("Comic Sans MS") };//����߶���λ�ò��泤������仯
	settextstyle(&chara);
	settextcolor(BLACK);
	SetWorkingImage(NULL);//ֱ����ʾ����Ļ��
	//������һ�ε÷�
	solidrectangle(SHOW_WIDTH / 6 + SHOW_WIDTH * num / total, 0, SHOW_WIDTH * (num + 1) / total - SHOW_WIDTH / total / 3, INFO_HEIGHT_LINE * INFO_LINE_NUM - 1);
	TCHAR str[20];
	//��ӡ�÷�
	_stprintf(str, _T("%d\0"), score);
	outtextxy(SHOW_WIDTH / 6 + SHOW_WIDTH * num / total, (INFO_HEIGHT_LINE - textheight(str)) / 2, str);
	//��ӡ����
	_stprintf(str, _T("%d\0"), length + 1);
	outtextxy(SHOW_WIDTH / 6 + SHOW_WIDTH * num / total, INFO_HEIGHT_LINE + (INFO_HEIGHT_LINE - textheight(str)) / 2, str);
	//��ӡ����
	_stprintf(str, _T("%d\0"), life);
	outtextxy(SHOW_WIDTH / 6 + SHOW_WIDTH * num / total, INFO_HEIGHT_LINE * 2 + (INFO_HEIGHT_LINE - textheight(str)) / 2, str);
	//��ӡ����
	_stprintf(str, _T("%d\0"), spirit);
	outtextxy(SHOW_WIDTH / 6 + SHOW_WIDTH * num / total, INFO_HEIGHT_LINE * 3 + (INFO_HEIGHT_LINE - textheight(str)) / 2, str);

}
coordinate Snake::GetHead()
{
	return body.head->place;
}
void Snake::DestroySnake(Map& m, Food& f)	//���ٶ���
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
bool Snake::RandomGenerateSnake(Map& m, Food& f)	//���ٶ���
{
	length = INI_LENGTH;
	int i = 0;
	while (i++ < 1000) {
		coordinate coor = { rand() % m.CANVAS_XBLOCK ,rand() % m.CANVAS_YBLOCK };
		coordinate now;
		for (now = coor; now.y - coor.y < 3 && now.y < m.CANVAS_YBLOCK; ++now.y)//ͷ����ΪUP
			if (m.TERRITORY[now.y * m.CANVAS_XBLOCK + now.x] >= SOFT_WALL || f.JudgeFood(now, m.CANVAS_XBLOCK) != NO_FOOD) {
				break;
			}
		if (now.y - coor.y >= 3) {
			this_orientation = orientation = UP;
			speed = FAST;//��������������
			InitSnake(m, coor);
			return true;
		}
		for (now = coor; now.y - coor.y > -3 && now.y >= 0; --now.y)//ͷ����ΪDOWN
			if (m.TERRITORY[now.y * m.CANVAS_XBLOCK + now.x] >= SOFT_WALL || f.JudgeFood(now, m.CANVAS_XBLOCK) != NO_FOOD) {
				break;
			}
		if (now.y - coor.y <= -3) {
			this_orientation = orientation = DOWN;
			InitSnake(m, coor);
			return true;
		}
		for (now = coor; now.x - coor.x < 3 && now.x < m.CANVAS_XBLOCK; ++now.x)//ͷ����ΪLEFT
			if (m.TERRITORY[now.y * m.CANVAS_XBLOCK + now.x] >= SOFT_WALL || f.JudgeFood(now, m.CANVAS_XBLOCK) != NO_FOOD) {
				break;
			}
		if (now.x - coor.x >= 3) {
			this_orientation = orientation = LEFT;
			InitSnake(m, coor);
			return true;
		}
		for (now = coor; now.x - coor.x > -3 && now.x >= 0; --now.x)//ͷ����ΪRIGHT
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
char Snake::JudgeBody(coordinate tojudge)//�ж�tojudge���Ƿ�Ϊ����
{//0�������ߣ�1������ͷ����2������β
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