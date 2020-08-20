#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string.h>

struct knight
{
   int HP;
   int level;
   int gil;
};

struct castle
{
	int arrEvent[20];
	int nEvent;
};

struct report
{
	int nPetal; 
	int nWin;
	int nLose;
};

int callPhoenix(knight& woundedKnight, int maxHP);
int hash(int eventCode);
report* walkthrough(knight& theKnight, castle arrCastle[], int nCastle, int mode, int nPetal);