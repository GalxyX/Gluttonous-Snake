#pragma once
#include <iostream>
#include "Parameter.h"
using namespace std;
int main_menu();
void GetName(TCHAR name[]);
void single_game(TCHAR name[][MAX_NAME_LENGTH], char& color, char& mapsize, char level);
void double_game(char level);
void help_menu();
void AI_game(TCHAR name[][MAX_NAME_LENGTH], char level);
int level_menu();
int single_menu(TCHAR name[], char& color, char& mapsize);
void rank_menu();