#pragma once
#include <graphics.h>
//�޸�
const int SIDE_LENGTH = 15;//������ÿһ��ֱ��������λ�����εĳ���
const int SHOW_XBLOCK = 48;//���ؿ����������
const int SHOW_YBLOCK = 48;//���ظߵ���������
const int BOUNDARY = 2;//�߽���
const COLORREF SCORE_COLOR = RGB(225, 1231, 186);
const int INFO_HEIGHT_LINE = 30;//ÿ����Ϣ�߶�
const int INFO_LINE_NUM = 4;//��Ϣ����
const COLORREF BK_COLOR = RGB(225, 255, 255);//�˵�����ɫ
const int PAUSE_SIDE_LENGTH = INFO_HEIGHT_LINE * 3 / 4;//��ͣ���߳�
const int PAUSE_SIDE_LENGTH_GETBIG = PAUSE_SIDE_LENGTH * 5 / 4;//��ͣ�����߳�
const int QUIT_LENGTH = PAUSE_SIDE_LENGTH * 3;//�˳������
const int QUIT_HEIGHT = PAUSE_SIDE_LENGTH * 3 / 2;//�˳����߶�
const int QUIT_LENGTH_GETBIG = QUIT_LENGTH * 5 / 4;//�˳��������
const int QUIT_HEIGHT_GETBIG = QUIT_HEIGHT * 4 / 3;//�˳������߶�


//��ͼ��Сѡ����
const char ORDI_MAP = 0;
const char BIG_MAP = 1;

//�ߵ���ɫ��Ƥ����ѡ����
const char BLUE_S = 1;
const char YELLOW_S = 2;
const char GREEN_S = 3;
const char MAX_NAME_LENGTH = 16;//��β0�������

//����ģʽ�Ѷ�ѡ�񷵻�ֵ
const char LEVEL1 = 1;
const char LEVEL2 = 2;
const char LEVEL3 = 3;

//���˵�����ֵ
const char ONE_PLAYER = 0;
const char AI_PLAYER = 4;
const char TWO_PLAYER = 5;
const char RANK = 6;
const char HELP = 7;

//���޸�
const int SHOW_WIDTH = SIDE_LENGTH * SHOW_XBLOCK + 2 * BOUNDARY + 1;//���ؿ�����//easyx����ʾһ������
const int SHOW_HEIGHT = SIDE_LENGTH * SHOW_YBLOCK + 2 * BOUNDARY + 1;//���ظ�����//easyx����ʾһ������
const int WINDOW_HEIGHT = SHOW_HEIGHT + INFO_HEIGHT_LINE * INFO_LINE_NUM;//�ܴ��ڸ߶�
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

