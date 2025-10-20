#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <conio.h>
#include "Parameter.h"
using namespace std;
const int small_BUTTON_WIDTH = SHOW_HEIGHT / 4;//�ϴ�ťԭʼ���
const int small_BUTTON_HEIGHT = WINDOW_HEIGHT / 7;//�ϴ�ťԭʼ�߶�
const int small_MINI_BUTTON_WIDTH = small_BUTTON_WIDTH * 4 / 9;//��С��ťԭʼ���
const int small_MINI_BUTTON_HEIGHT = small_MINI_BUTTON_WIDTH * 5 / 7;//��С��ťԭʼ�߶�
const int BUTTON_SIDE = WINDOW_HEIGHT / 10;//��ť�����±߽����
const int BUTTON_GAP = (WINDOW_HEIGHT - small_BUTTON_HEIGHT * 4 - BUTTON_SIDE * 2) / 3;//��ť֮�����
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
{//���Ͻ�λ�����꣬�ܿ�ȣ��ܸ߶ȣ�Բ�ǰ뾶����ɫ
	setbkmode(TRANSPARENT);
	setfillcolor(color);
	setlinestyle(PS_SOLID, 2);
	fillroundrect(XL.x, XL.y, XL.x + width, XL.y + height, width / 10, height / 10);//Բ�Ǿ����� x ����;Բ�Ǿ��ζ��� y ����; Բ�Ǿ����Ҳ� x ����; Բ�Ǿ��εײ� y ����;����Բ�Ǿ��ε�Բ�ǵ���Բ�Ŀ��; ����Բ�Ǿ��ε�Բ�ǵ���Բ�ĸ߶ȡ�
	const LOGFONT chara = { height / 3,0,0,0,FW_BLACK,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,DEFAULT_PITCH, _T("��Բ") };//����߶���λ�ò��泤������仯
	settextstyle(&chara);
	settextcolor(WHITE);
	int tx = XL.x + (width - textwidth(text)) / 2;
	int ty = XL.y + (height - textheight(text)) / 2;
	outtextxy(tx, ty, text);
	const LOGFONT chara1 = { INFO_HEIGHT_LINE * 2 / 3,0,0,0,FW_BLACK,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,DEFAULT_PITCH, _T("Comic Sans MS") };//����߶���λ�ò��泤������仯
	settextstyle(&chara1);
	settextcolor(BLACK);
	setfillcolor(BK_COLOR);
}
int main_menu()
{//����ֵ1������ģʽ��2�����˻���ս��3����˫�˶�ս��4�������������ս��5����
	setbkcolor(BK_COLOR);
	cleardevice();//�������
	IMAGE bkimg;
	loadimage(&bkimg, _T("./image/menu_bk.png"));
	putimage(0, 0, &bkimg);

	//��ӡ���л�����ť
	DrawRoundrect(small_BUTTON[0], small_BUTTON_WIDTH, small_BUTTON_HEIGHT, BUTTON_COLOR[0], _T("����ģʽ"));
	DrawRoundrect(small_BUTTON[1], small_BUTTON_WIDTH, small_BUTTON_HEIGHT, BUTTON_COLOR[1], _T("�˻���ս"));
	DrawRoundrect(small_BUTTON[2], small_BUTTON_WIDTH, small_BUTTON_HEIGHT, BUTTON_COLOR[2], _T("˫�˶�ս"));
	DrawRoundrect(small_BUTTON[3], small_MINI_BUTTON_WIDTH, small_MINI_BUTTON_HEIGHT, BUTTON_COLOR[3], _T("�ְܷ�"));
	DrawRoundrect(small_BUTTON[4], small_MINI_BUTTON_WIDTH, small_MINI_BUTTON_HEIGHT, BUTTON_COLOR[4], _T("С��ʾ"));
	//��ȡ�����Ϣ
	MOUSEMSG m{ 0 };           // �����Ϣ�ṹ��
	BOOL flag = TRUE;
	while (flag)
	{
		if (MouseHit())
			m = GetMouseMsg();   // ��ȡ�����Ϣ
		if (m.uMsg == WM_MOUSEMOVE)// ����ƶ�
			if (m.x >= small_BUTTON[0].x && m.x <= small_BUTTON[0].x + small_BUTTON_WIDTH && m.y >= small_BUTTON[0].y && m.y <= small_BUTTON[0].y + small_BUTTON_HEIGHT) {
				DrawRoundrect(big_BUTTON[0], big_BUTTON_WIDTH, big_BUTTON_HEIGHT, BUTTON_COLOR[0], _T("����ģʽ"));
				while (true) {
					if (MouseHit())
						m = GetMouseMsg();   // ��ȡ�����Ϣ
					if (m.uMsg == WM_LBUTTONDOWN)
						return ONE_PLAYER;
					else if (m.uMsg == WM_MOUSEMOVE)// ����ƶ�
						if (m.x <= big_BUTTON[0].x || m.x >= big_BUTTON[0].x + big_BUTTON_WIDTH || m.y <= big_BUTTON[0].y || m.y >= big_BUTTON[0].y + big_BUTTON_HEIGHT) {
							setfillcolor(BK_COLOR);
							solidrectangle(big_BUTTON[0].x - 2, big_BUTTON[0].y - 2, big_BUTTON[0].x + big_BUTTON_WIDTH + 2, big_BUTTON[0].y + big_BUTTON_HEIGHT + 2);
							DrawRoundrect(small_BUTTON[0], small_BUTTON_WIDTH, small_BUTTON_HEIGHT, BUTTON_COLOR[0], _T("����ģʽ"));
							break;
						}
				}
			}
			else if (m.x >= small_BUTTON[1].x && m.x <= small_BUTTON[1].x + small_BUTTON_WIDTH && m.y >= small_BUTTON[1].y && m.y <= small_BUTTON[1].y + small_BUTTON_HEIGHT) {
				DrawRoundrect(big_BUTTON[1], big_BUTTON_WIDTH, big_BUTTON_HEIGHT, BUTTON_COLOR[1], _T("�˻���ս"));
				while (true) {
					if (MouseHit())
						m = GetMouseMsg();   // ��ȡ�����Ϣ
					if (m.uMsg == WM_LBUTTONDOWN)
						return AI_PLAYER;
					else if (m.uMsg == WM_MOUSEMOVE)// ����ƶ�
						if (m.x <= big_BUTTON[1].x || m.x >= big_BUTTON[1].x + big_BUTTON_WIDTH || m.y <= big_BUTTON[1].y || m.y >= big_BUTTON[1].y + big_BUTTON_HEIGHT) {
							setfillcolor(BK_COLOR);
							solidrectangle(big_BUTTON[1].x - 2, big_BUTTON[1].y - 2, big_BUTTON[1].x + big_BUTTON_WIDTH + 2, big_BUTTON[1].y + big_BUTTON_HEIGHT + 2);
							DrawRoundrect(small_BUTTON[1], small_BUTTON_WIDTH, small_BUTTON_HEIGHT, BUTTON_COLOR[1], _T("�˻���ս"));
							break;
						}
				}
			}
			else if (m.x >= small_BUTTON[2].x && m.x <= small_BUTTON[2].x + small_BUTTON_WIDTH && m.y >= small_BUTTON[2].y && m.y <= small_BUTTON[2].y + small_BUTTON_HEIGHT) {
				DrawRoundrect(big_BUTTON[2], big_BUTTON_WIDTH, big_BUTTON_HEIGHT, BUTTON_COLOR[2], _T("˫�˶�ս"));
				while (true) {
					if (MouseHit())
						m = GetMouseMsg();   // ��ȡ�����Ϣ
					if (m.uMsg == WM_LBUTTONDOWN)
						return TWO_PLAYER;
					else if (m.uMsg == WM_MOUSEMOVE)// ����ƶ�
						if (m.x <= big_BUTTON[2].x || m.x >= big_BUTTON[2].x + big_BUTTON_WIDTH || m.y <= big_BUTTON[2].y || m.y >= big_BUTTON[2].y + big_BUTTON_HEIGHT) {
							setfillcolor(BK_COLOR);
							solidrectangle(big_BUTTON[2].x - 2, big_BUTTON[2].y - 2, big_BUTTON[2].x + big_BUTTON_WIDTH + 2, big_BUTTON[2].y + big_BUTTON_HEIGHT + 2);
							DrawRoundrect(small_BUTTON[2], small_BUTTON_WIDTH, small_BUTTON_HEIGHT, BUTTON_COLOR[2], _T("˫�˶�ս"));
							break;
						}
				}
			}
			else if (m.x >= small_BUTTON[3].x && m.x <= small_BUTTON[3].x + small_MINI_BUTTON_WIDTH && m.y >= small_BUTTON[3].y && m.y <= small_BUTTON[3].y + small_MINI_BUTTON_HEIGHT) {
				DrawRoundrect(big_BUTTON[3], big_MINI_BUTTON_WIDTH, big_MINI_BUTTON_HEIGHT, BUTTON_COLOR[3], _T("�ְܷ�"));
				while (true) {
					if (MouseHit())
						m = GetMouseMsg();   // ��ȡ�����Ϣ
					if (m.uMsg == WM_LBUTTONDOWN)
						return RANK;
					else if (m.uMsg == WM_MOUSEMOVE)// ����ƶ�
						if (m.x <= big_BUTTON[3].x || m.x >= big_BUTTON[3].x + big_MINI_BUTTON_WIDTH || m.y <= big_BUTTON[3].y || m.y >= big_BUTTON[3].y + big_MINI_BUTTON_HEIGHT) {
							setfillcolor(BK_COLOR);
							solidrectangle(big_BUTTON[3].x - 2, big_BUTTON[3].y - 2, big_BUTTON[3].x + big_MINI_BUTTON_WIDTH + 2, big_BUTTON[3].y + big_MINI_BUTTON_HEIGHT + 2);
							DrawRoundrect(small_BUTTON[3], small_MINI_BUTTON_WIDTH, small_MINI_BUTTON_HEIGHT, BUTTON_COLOR[3], _T("�ְܷ�"));
							break;
						}
				}
			}
			else if (m.x >= small_BUTTON[4].x && m.x <= small_BUTTON[4].x + small_MINI_BUTTON_WIDTH && m.y >= small_BUTTON[4].y && m.y <= small_BUTTON[4].y + small_MINI_BUTTON_HEIGHT) {
				DrawRoundrect(big_BUTTON[4], big_MINI_BUTTON_WIDTH, big_MINI_BUTTON_HEIGHT, BUTTON_COLOR[4], _T("С��ʾ"));
				while (true) {
					if (MouseHit())
						m = GetMouseMsg();   // ��ȡ�����Ϣ
					if (m.uMsg == WM_LBUTTONDOWN)
						return HELP;
					else if (m.uMsg == WM_MOUSEMOVE)// ����ƶ�
						if (m.x <= big_BUTTON[4].x || m.x >= big_BUTTON[4].x + big_MINI_BUTTON_WIDTH || m.y <= big_BUTTON[4].y || m.y >= big_BUTTON[4].y + big_MINI_BUTTON_HEIGHT) {
							setfillcolor(BK_COLOR);
							solidrectangle(big_BUTTON[4].x - 2, big_BUTTON[4].y - 2, big_BUTTON[4].x + big_MINI_BUTTON_WIDTH + 2, big_BUTTON[4].y + big_MINI_BUTTON_HEIGHT + 2);
							DrawRoundrect(small_BUTTON[4], small_MINI_BUTTON_WIDTH, small_MINI_BUTTON_HEIGHT, BUTTON_COLOR[4], _T("С��ʾ"));
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
	InputBox(inputn, MAX_NAME_LENGTH, _T("������������֡�\n�����������޷���¼��ĳɼ�Ŷ[ע�����ֲ�Ҫ���ո�ֿ���]"), _T("NAME_INPUT"));
	for (int i = 0; i < MAX_NAME_LENGTH; ++i)
		name[i] = inputn[i];
}
int level_menu()
{
	setbkcolor(BK_COLOR);
	cleardevice();//�������
	IMAGE bkimg;
	loadimage(&bkimg, _T("./image/pure_bk.png"), 400, 400);
	putimage(0, (WINDOW_HEIGHT - bkimg.getheight()) / 2, &bkimg);
	//��ӡ���л�����ť
	DrawRoundrect(small_BUTTON[0], small_BUTTON_WIDTH, small_BUTTON_HEIGHT, BUTTON_COLOR[0], _T("���Ű�"));
	DrawRoundrect(small_BUTTON[1], small_BUTTON_WIDTH, small_BUTTON_HEIGHT, BUTTON_COLOR[1], _T("���װ�"));
	DrawRoundrect(small_BUTTON[2], small_BUTTON_WIDTH, small_BUTTON_HEIGHT, BUTTON_COLOR[2], _T("�߼���"));
	//��ȡ�����Ϣ
	MOUSEMSG m{ 0 };           // �����Ϣ�ṹ��
	BOOL flag = TRUE;
	while (flag)
	{
		if (MouseHit())
			m = GetMouseMsg();   // ��ȡ�����Ϣ
		if (m.uMsg == WM_MOUSEMOVE)// ����ƶ�
			if (m.x >= small_BUTTON[0].x && m.x <= small_BUTTON[0].x + small_BUTTON_WIDTH && m.y >= small_BUTTON[0].y && m.y <= small_BUTTON[0].y + small_BUTTON_HEIGHT) {
				DrawRoundrect(big_BUTTON[0], big_BUTTON_WIDTH, big_BUTTON_HEIGHT, BUTTON_COLOR[0], _T("���Ű�"));
				while (true) {
					if (MouseHit())
						m = GetMouseMsg();   // ��ȡ�����Ϣ
					if (m.uMsg == WM_LBUTTONDOWN)
						return LEVEL1;
					else if (m.uMsg == WM_MOUSEMOVE)// ����ƶ�
						if (m.x <= big_BUTTON[0].x || m.x >= big_BUTTON[0].x + big_BUTTON_WIDTH || m.y <= big_BUTTON[0].y || m.y >= big_BUTTON[0].y + big_BUTTON_HEIGHT) {
							setfillcolor(BK_COLOR);
							solidrectangle(big_BUTTON[0].x - 2, big_BUTTON[0].y - 2, big_BUTTON[0].x + big_BUTTON_WIDTH + 2, big_BUTTON[0].y + big_BUTTON_HEIGHT + 2);
							DrawRoundrect(small_BUTTON[0], small_BUTTON_WIDTH, small_BUTTON_HEIGHT, BUTTON_COLOR[0], _T("���Ű�"));
							break;
						}
				}
			}
			else if (m.x >= small_BUTTON[1].x && m.x <= small_BUTTON[1].x + small_BUTTON_WIDTH && m.y >= small_BUTTON[1].y && m.y <= small_BUTTON[1].y + small_BUTTON_HEIGHT) {
				DrawRoundrect(big_BUTTON[1], big_BUTTON_WIDTH, big_BUTTON_HEIGHT, BUTTON_COLOR[1], _T("���װ�"));
				while (true) {
					if (MouseHit())
						m = GetMouseMsg();   // ��ȡ�����Ϣ
					if (m.uMsg == WM_LBUTTONDOWN)
						return LEVEL2;
					else if (m.uMsg == WM_MOUSEMOVE)// ����ƶ�
						if (m.x <= big_BUTTON[1].x || m.x >= big_BUTTON[1].x + big_BUTTON_WIDTH || m.y <= big_BUTTON[1].y || m.y >= big_BUTTON[1].y + big_BUTTON_HEIGHT) {
							setfillcolor(BK_COLOR);
							solidrectangle(big_BUTTON[1].x - 2, big_BUTTON[1].y - 2, big_BUTTON[1].x + big_BUTTON_WIDTH + 2, big_BUTTON[1].y + big_BUTTON_HEIGHT + 2);
							DrawRoundrect(small_BUTTON[1], small_BUTTON_WIDTH, small_BUTTON_HEIGHT, BUTTON_COLOR[1], _T("���װ�"));
							break;
						}
				}
			}
			else if (m.x >= small_BUTTON[2].x && m.x <= small_BUTTON[2].x + small_BUTTON_WIDTH && m.y >= small_BUTTON[2].y && m.y <= small_BUTTON[2].y + small_BUTTON_HEIGHT) {
				DrawRoundrect(big_BUTTON[2], big_BUTTON_WIDTH, big_BUTTON_HEIGHT, BUTTON_COLOR[2], _T("�߼���"));
				while (true) {
					if (MouseHit())
						m = GetMouseMsg();   // ��ȡ�����Ϣ
					if (m.uMsg == WM_LBUTTONDOWN)
						return LEVEL3;
					else if (m.uMsg == WM_MOUSEMOVE)// ����ƶ�
						if (m.x <= big_BUTTON[2].x || m.x >= big_BUTTON[2].x + big_BUTTON_WIDTH || m.y <= big_BUTTON[2].y || m.y >= big_BUTTON[2].y + big_BUTTON_HEIGHT) {
							setfillcolor(BK_COLOR);
							solidrectangle(big_BUTTON[2].x - 2, big_BUTTON[2].y - 2, big_BUTTON[2].x + big_BUTTON_WIDTH + 2, big_BUTTON[2].y + big_BUTTON_HEIGHT + 2);
							DrawRoundrect(small_BUTTON[2], small_BUTTON_WIDTH, small_BUTTON_HEIGHT, BUTTON_COLOR[2], _T("�߼���"));
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
	color = BLUE_S;//��ʼ����ɫ

	wchar_t inputm[7];
	InputBox(inputm, 7, _T("��ѡ���ͼ��С\n�����룺small��big"), _T("MAP_SIZE"));
	if ((inputm[0] == 'b' || inputm[0] == 'B') && (inputm[1] == 'i' || inputm[1] == 'I') && (inputm[2] == 'g' || inputm[0] == 'G'))
		mapsize = BIG_MAP;//��ʼ����ͼ��С
	else
		mapsize = ORDI_MAP;//��ʼ����ͼ��С

	wchar_t inputc[3];
	InputBox(inputc, 3, _T("��ѡ�������ɫ\n��ɫ:1����ɫ:2����ɫ:3"), _T("COLOR_INPUT"));
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
	ExMessage operation;//��������
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
	const LOGFONT chara = { INFO_HEIGHT_LINE / 2,0,0,0,FW_BLACK,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,DEFAULT_PITCH, _T("��Բ") };//����߶���λ�ò��泤������仯
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
		if (readlevel == "���Ű�" && readnum > max1) {
			max1 = readnum;
			name1 = readname;
		}

		else if (readlevel == "���װ�" && readnum > max2) {
			max2 = readnum;
			name2 = readname;
		}

		else if (readlevel == "�߼���" && readnum > max3) {
			max3 = readnum;
			name3 = readname;
		}
		WCHAR text[70], widelevel[20], widename[20];

		if (readname != "") {
			MultiByteToWideChar(CP_ACP, 0, readlevel.c_str(), -1, widelevel, 20);
			MultiByteToWideChar(CP_ACP, 0, readname.c_str(), -1, widename, 20);
			_stprintf(text, _T("�汾��%s �û�����%s �÷֣�%d\0"), widelevel, widename, readnum);
			outtextxy(0, INFO_HEIGHT_LINE * 3 / 2 + INFO_HEIGHT_LINE * num++ / 2, text);
		}
	}
	infile.close();

	WCHAR text[70], widename[20];
	if (name1 != "") {
		MultiByteToWideChar(CP_ACP, 0, name1.c_str(), -1, widename, 20);
		_stprintf(text, _T("��߷֣����Ű� �û�����%s �÷֣�%d\0"), widename, max1);
		outtextxy(0, 0, text);
	}
	else
		outtextxy(0, 0, L"û�����Ű���߷ּ�¼");
	if (name2 != "") {
		MultiByteToWideChar(CP_ACP, 0, name2.c_str(), -1, widename, 20);
		_stprintf(text, _T("��߷֣����װ� �û�����%s �÷֣�%d\0"), widename, max2);
		outtextxy(0, INFO_HEIGHT_LINE / 2, text);
	}
	else
		outtextxy(0, INFO_HEIGHT_LINE / 2, L"û�н��װ���߷ּ�¼");
	if (name3 != "") {
		MultiByteToWideChar(CP_ACP, 0, name3.c_str(), -1, widename, 20);
		_stprintf(text, _T("��߷֣��߼��� �û�����%s �÷֣�%d\0"), widename, max3);
		outtextxy(0, INFO_HEIGHT_LINE, text);
	}
	else
		outtextxy(0, INFO_HEIGHT_LINE, L"û�и߼�����߷ּ�¼");

	outtextxy(0, WINDOW_HEIGHT - INFO_HEIGHT_LINE / 2, L"���»س��Լ���...");
	while (getchar() != '\n')
		;
}