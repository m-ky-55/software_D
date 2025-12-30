#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#define MAX_TATE 99  // 縦
#define MAX_YOKO 35  // 横

int TATE, YOKO;
int ren;
char board[MAX_TATE][MAX_YOKO];  // '.' 空, 'O' Player1, 'X' Player2

void createSaveFolder();
void createSaveGameFolder(const char* path);
void makeSaveFilename(char* buf, const char* path, struct tm* tm);
void saveGame(char turn, const char* path, struct tm* tm);
int loadGame(const char* filename, char* turn);
void initBoard();
void displayBoard();
int dropPiece(int yoko, char player);
char checkWin(int ren);
int isFull();
