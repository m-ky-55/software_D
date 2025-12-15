#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#define MAX_ROWS 99  // 縦
#define MAX_COLS 35  // 横

void createSaveFolder();
void createSaveGameFolder(const char* path);
void makeSaveFilename(char* buf, const char* path, struct tm* tm);
void saveGame(char turn, const char* path, struct tm* tm);
int loadGame(const char* filename, char* turn);
void initBoard();
void displayBoard();
int dropPiece(int col, char player);
char checkWin(int ren);
int isFull();
