#include <graphics.h>
#include <conio.h>
#include "snake_main.h"

int main() {
	TCHAR name[1][MAX_NAME_LENGTH] = { L"\0" };
	char color;
	char mapsize;
	srand((unsigned int)(time(0)));
	initgraph(SHOW_WIDTH, WINDOW_HEIGHT);  // 初始化窗口
	while (true) {
		switch (main_menu()) {
			case ONE_PLAYER:
				single_game(name, color, mapsize, single_menu(name[0], color, mapsize));
				break;
			case TWO_PLAYER:
				double_game(level_menu());
				break;
			case AI_PLAYER:
				GetName(name[0]);
				AI_game(name, level_menu());
				break;
			case HELP:
				help_menu();
				continue;
			case RANK:
				rank_menu();
				continue;

		}
		SetWorkingImage(NULL);
		const LOGFONT chara = { INFO_HEIGHT_LINE,0,0,0,FW_BLACK,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,DEFAULT_PITCH, _T("Comic Sans MS") };//字体高度与位置不随长宽比例变化
		settextstyle(&chara);
		settextcolor(RGB(205, 85, 85));
		TCHAR text1[] = _T("Game Over!");
		outtextxy((SHOW_WIDTH - textwidth(text1)) / 2, (SHOW_WIDTH - textheight(text1)) / 2, text1);
		Sleep(3000);
	}
	closegraph();

	return 0;
}