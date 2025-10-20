#define _CRT_SECURE_NO_WARNINGS
#include <conio.h>
#include <fstream>
#include "snake_class.h"
const int TIME_GAP = 80;
const int FOOD_REGEN = 16000;
void DrawRoundrect(coordinate XL, int width, int height, COLORREF color, const TCHAR text[]);

bool PAUSE(Map& m, BasicImg& img)
{//�����Ƿ������Ϸ
	SetWorkingImage(NULL);
	DrawRoundrect(SMALL_QUIT, QUIT_LENGTH, QUIT_HEIGHT, RGB(255, 127, 0), L"������Ϸ");
	setfillcolor(BK_COLOR);
	ExMessage operation;//��������
	char buttonstatus = 0;//��ť�ޱ仯false����ǰ��ͣ��ť���Ϊ1��������ť���Ϊ2
	while (true) {
		if (peekmessage(&operation, EX_MOUSE | EX_KEY)) {
			switch (operation.message) {
				case WM_KEYUP://��������
					if (operation.vkcode == VK_SPACE) {
						solidrectangle(BIG_QUIT.x - 2, BIG_QUIT.y - 2, BIG_QUIT.x + QUIT_LENGTH_GETBIG + 2, BIG_QUIT.y + QUIT_HEIGHT_GETBIG + 2);//�����ñ�������
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
						//solidrectangle(SMALL_QUIT.x, SMALL_QUIT.y, SMALL_QUIT.x + QUIT_LENGTH+2, SMALL_QUIT.y + QUIT_HEIGHT+2);//�����ñ�������
						DrawRoundrect(BIG_QUIT, QUIT_LENGTH_GETBIG, QUIT_HEIGHT_GETBIG, RGB(255, 127, 0), L"������Ϸ");
						buttonstatus = 2;
					}
					else if (buttonstatus == 2 && (operation.x < BIG_QUIT.x || operation.x > BIG_QUIT.x + QUIT_LENGTH || operation.y < BIG_QUIT.y || operation.y > BIG_QUIT.y + QUIT_HEIGHT)) {
						solidrectangle(BIG_QUIT.x - 2, BIG_QUIT.y - 2, BIG_QUIT.x + QUIT_LENGTH_GETBIG + 2, BIG_QUIT.y + QUIT_HEIGHT_GETBIG + 2);//�����ñ�������
						DrawRoundrect(SMALL_QUIT, QUIT_LENGTH, QUIT_HEIGHT, RGB(255, 127, 0), L"������Ϸ");
						buttonstatus = 0;
					}
					break;
				case WM_LBUTTONUP:
					if (buttonstatus == 1 && operation.x >= SHOW_WIDTH - PAUSE_SIDE_LENGTH_GETBIG && operation.x <= SHOW_WIDTH && operation.y >= 0 && operation.y <= PAUSE_SIDE_LENGTH_GETBIG) {
						solidrectangle(BIG_QUIT.x - 2, BIG_QUIT.y - 2, BIG_QUIT.x + QUIT_LENGTH_GETBIG + 2, BIG_QUIT.y + QUIT_HEIGHT_GETBIG + 2);//�����ñ�������
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
{//�����Ƿ��˳���Ϸ
	bool is_continue = true;//������Ϸ��־
	ExMessage operation;//��������
	if (peekmessage(&operation, EX_MOUSE | EX_KEY)) {
		int i;
		switch (operation.message) {
			case WM_KEYUP://��������
				if (operation.vkcode == VK_SPACE)//��ͣ///////////////////////////////////
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
	cleardevice();//�������
	int block_width = SHOW_XBLOCK + mapsize * SHOW_XBLOCK;
	int block_height = SHOW_YBLOCK + mapsize * SHOW_YBLOCK;

	BasicImg imgstd;//���ر�׼����ͼ
	imgstd.DisplayInfoBK(1, name);//��ʾ����ͼ
	//�������ɵ�ͼ�ռ�
	Map single(level, block_width, block_height);
	//SetWorkingImage(single.canvas);
	single.RandomRegion();//�ٳ�ʼ����ͼ����
	//single.DisplayInfoBK(2);
	//��ȷ���ߵ�λ�ã��ɸ������飬����ǽ
	int snake_life;
	if (level == LEVEL1)
		snake_life = 1;
	else if (level == LEVEL2)
		snake_life = 100000000;
	else
		snake_life = 5;
	Snake s1(true, single, block_width / 2 - 1, block_height / 2 - 1, color, snake_life, name[0]);/*���泤����Ϊ�������룬���泤ż������0�ࣨ�����ᣩ*/
	//�۳�ʼ��ʳ��
	Food mapfood(single);//����ʳ��
	/*************************************���Կ�ʼѭ����*************************************/
	bool mouse_on_pause = false;//����Ƿ���ͣ����ͣ��
	bool continuegame = true;//��Ϸ�Ƿ����

	clock_t pasttime = single.runtime;//�ϴγ�������ʱ��
	clock_t starttime = clock();//��Ϸ��ʼʱ��
	clock_t lasttime = starttime;//�ϴ�����ʱ��
	clock_t nowtime;//����ʱ��
	clock_t lastfoodgene = starttime;//�ϴ�����ʳ��ʱ��

	while (1) {
		nowtime = clock();//��ȡ����ѭ����ʱ��
		single.runtime = pasttime + (nowtime - starttime);//������Ϸ����ʱ��

		if (!JUDGE_MOUSE_KEY(single, imgstd, 1, &s1, mouse_on_pause))
			break;

		if (nowtime - lasttime >= TIME_GAP) {//�Ѿ���ȥʱ����
			continuegame = s1.MoveSnake(single, mapfood);//�ƶ���
			if (!s1.ClearNum(single, mapfood))//��������
				continuegame = false;
			if (nowtime - lastfoodgene >= FOOD_REGEN || !mapfood.GetGoodFoodNum()) {//�̶�ʱ�����������µ�ʳ��
				mapfood.DestroyAllFood(single);
				mapfood.RandomGenerateFood(single);
				lastfoodgene = nowtime;
			}
			single.Display(s1);//��ʾͼ��
			s1.DisplayInfo(0, 1);//��ʾ��Ϣ

			if (!continuegame)//�ж��Ƿ����
				break;

			lasttime = nowtime;
			//��ӡʱ��
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
		strcpy(level_name, "���Ű�");
	else if (level == LEVEL2)
		strcpy(level_name, "���װ�");
	else
		strcpy(level_name, "�߼���");

	WideCharToMultiByte(CP_ACP, 0, s1.name, -1, player, MAX_NAME_LENGTH, NULL, NULL);
	if (player[0] != '\0')
		outfile << ' ' << level_name << ' ' << player << ' ' << s1.score;
	outfile.close();
}
void double_game(char level)
{
	setbkcolor(BK_COLOR);
	cleardevice();//�������
	TCHAR player[2][MAX_NAME_LENGTH] = { L"Player1",L"Player2" };

	//��ɫ��λ���
	char color1 = rand() % GREEN_S + 1;
	char color2 = (color1 + rand() % (GREEN_S - 1)) % GREEN_S + 1;//��Ч��ֹ��ɫ����������

	BasicImg imgstd;//���ر�׼����ͼ
	imgstd.DisplayInfoBK(2, player);//��ʾ����ͼ

	//�������ɵ�ͼ�ռ�
	Map twoP(level, SHOW_XBLOCK, SHOW_YBLOCK);
	twoP.RandomRegion();//�ٳ�ʼ����ͼ����

	//��ȷ���ߵ�λ�ã��ɸ������飬����ǽ
	Snake s[2]{ {false, twoP, SHOW_XBLOCK / 4 - 1, SHOW_YBLOCK / 2 - 1, color1, 5, player[0]},{true, twoP, SHOW_XBLOCK * 3 / 4 - 1, SHOW_YBLOCK / 2 - 1, color2, 5, player[1]} };/*һ�����������룬��һ�����Ұ������*/
	//�۳�ʼ��ʳ��
	Food mapfood(twoP);//����ʳ��
	/*************************************���Կ�ʼѭ����*************************************/
	bool mouse_on_pause = false;//����Ƿ���ͣ����ͣ��
	bool stillalive[2] = { true,true };//��[i]�����Ƿ񻹻���

	clock_t pasttime = twoP.runtime;//�ϴγ�������ʱ��
	clock_t starttime = clock();//��Ϸ��ʼʱ��
	clock_t lasttime = starttime;//�ϴ�����ʱ��
	clock_t nowtime;//����ʱ��
	clock_t lastfoodgene = starttime;//�ϴ�����ʳ��ʱ��

	int i;
	while (1) {
		nowtime = clock();//��ȡ����ѭ����ʱ��
		twoP.runtime = pasttime + (nowtime - starttime);//������Ϸ����ʱ��

		if (!JUDGE_MOUSE_KEY(twoP, imgstd, 2, s, mouse_on_pause))
			break;

		if (nowtime - lasttime >= TIME_GAP) {//�Ѿ���ȥʱ����
			for (i = 0; i < 2; ++i)
				if (stillalive[i]) {
					stillalive[i] = s[i].MoveSnake(twoP, mapfood);//�ƶ���
					if (!s[i].ClearNum(twoP, mapfood))//��������
						stillalive[i] = false;
					s[i].DisplayInfo(i, 2);//��ʾ��Ϣ
					SetWorkingImage(twoP.canvas);
				}
			if (nowtime - lastfoodgene >= FOOD_REGEN || !mapfood.GetGoodFoodNum()) {//�̶�ʱ�����������µ�ʳ��
				mapfood.DestroyAllFood(twoP);
				mapfood.RandomGenerateFood(twoP);
				lastfoodgene = nowtime;
			}
			twoP.Display(s[0]);//��ʾͼ��

			if (!stillalive[0] && !stillalive[1])//�ж��Ƿ����
				break;

			SetWorkingImage(NULL);
			lasttime = nowtime;
			//��ӡʱ��
			TCHAR str[10];
			_stprintf(str, _T("%d : %d\0"), twoP.runtime / 1000 / 60, twoP.runtime / 1000 % 60);
			solidrectangle(SHOW_WIDTH / 2 - textwidth(str) - 20, 0, SHOW_WIDTH / 2, INFO_HEIGHT_LINE);//ȷ������ԭʱ��
			outtextxy(SHOW_WIDTH / 2 - textwidth(str) - 10, 0, str);
		}
	}
}
void AI_game(TCHAR name[][MAX_NAME_LENGTH], char level)
{
	setbkcolor(BK_COLOR);
	cleardevice();//�������
	TCHAR player[2][MAX_NAME_LENGTH] = { L"Player",L"AI Player" };
	if (name[0][0] != L'\0')
		for (int i = 0; i < MAX_NAME_LENGTH; ++i)
			player[0][i] = name[0][i];
	//��ɫ��λ���
	char color1 = rand() % GREEN_S + 1;
	char color2 = (color1 + rand() % (GREEN_S - 1)) % GREEN_S + 1;//��Ч��ֹ��ɫ����������

	BasicImg imgstd;//���ر�׼����ͼ
	imgstd.DisplayInfoBK(2, player);//��ʾ����ͼ

	//�������ɵ�ͼ�ռ�
	Map twoP(level, SHOW_XBLOCK, SHOW_YBLOCK);
	twoP.RandomRegion();//�ٳ�ʼ����ͼ����

	//��ȷ���ߵ�λ�ã��ɸ������飬����ǽ
	Snake s[2]{ {true, twoP, SHOW_XBLOCK / 4 - 1, SHOW_YBLOCK / 2 - 1, color1, 5, player[0]},{false, twoP, SHOW_XBLOCK * 3 / 4 - 1, SHOW_YBLOCK / 2 - 1, color2, 5, player[1]} };/*һ�����������룬��һ�����Ұ������*/
	//�۳�ʼ��ʳ��
	Food mapfood(twoP);//����ʳ��
	/*************************************���Կ�ʼѭ����*************************************/
	bool mouse_on_pause = false;//����Ƿ���ͣ����ͣ��
	bool stillalive[2] = { true,true };//��[i]�����Ƿ񻹻���

	clock_t pasttime = twoP.runtime;//�ϴγ�������ʱ��
	clock_t starttime = clock();//��Ϸ��ʼʱ��
	clock_t lasttime = starttime;//�ϴ�����ʱ��
	clock_t nowtime;//����ʱ��
	clock_t lastfoodgene = starttime;//�ϴ�����ʳ��ʱ��

	int i;
	while (1) {
		nowtime = clock();//��ȡ����ѭ����ʱ��
		twoP.runtime = pasttime + (nowtime - starttime);//������Ϸ����ʱ��

		if (!JUDGE_MOUSE_KEY(twoP, imgstd, 1, s, mouse_on_pause))
			break;

		if (nowtime - lasttime >= TIME_GAP) {//�Ѿ���ȥʱ����
			s[1].ChangeOrient(mapfood.WhichDirection(twoP, s[1].GetHead()));
			for (i = 0; i < 2; ++i)
				if (stillalive[i]) {
					stillalive[i] = s[i].MoveSnake(twoP, mapfood);//�ƶ���
					if (!s[i].ClearNum(twoP, mapfood))//��������
						stillalive[i] = false;
					s[i].DisplayInfo(i, 2);//��ʾ��Ϣ
					SetWorkingImage(twoP.canvas);
				}
			if (nowtime - lastfoodgene >= FOOD_REGEN || !mapfood.GetGoodFoodNum()) {//�̶�ʱ�����������µ�ʳ��
				mapfood.DestroyAllFood(twoP);
				mapfood.RandomGenerateFood(twoP);
				lastfoodgene = nowtime;
			}
			twoP.Display(s[0]);//��ʾͼ��

			if (!stillalive[0] && !stillalive[1])//�ж��Ƿ����
				break;

			SetWorkingImage(NULL);
			lasttime = nowtime;
			//��ӡʱ��
			TCHAR str[10];
			_stprintf(str, _T("%d : %d\0"), twoP.runtime / 1000 / 60, twoP.runtime / 1000 % 60);
			solidrectangle(SHOW_WIDTH / 2 - textwidth(str) - 20, 0, SHOW_WIDTH / 2, INFO_HEIGHT_LINE);//ȷ������ԭʱ��
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
		outfile << ' ' << "AIģʽ" << ' ' << playername << ' ' << s[0].score;
	outfile.close();
}