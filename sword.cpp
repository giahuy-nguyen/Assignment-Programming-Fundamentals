#ifndef _definition_h_
#include "defs.h"
#define _definition_h_
#endif

int MaxHP;
bool battleResult;

struct weapon{
  bool Shield = false;
  bool Spear = false;
  bool Hair = false;
  bool Excalibur = false;
  int Lionheart = 5;
};

struct item {
    bool Antidote = false;
    bool DWings = false;
    bool FakeAmor = false;
    bool Mythril_armor = false;
};

struct effect {
    int Curse = 5;
    int Poison = 6;
};

//Calculate level Opponent
int levelO(int countEvent) {
    int b = countEvent % 10;                            //b from formula in the guild file
    int levelO = countEvent > 6 ? (b > 5 ? b : 5) : b;
    return levelO;
}

//Check limit gil, negative HP to call Phoenix
void check(effect& knightEffected, knight& theKnight, report*& pReturn) {
    if (battleResult == true) {
        pReturn->nWin += 1;
        if (theKnight.gil > 999) {
            theKnight.gil = 999;
        }
    }
    else {
        if (theKnight.HP <= 0) {
            pReturn->nLose += callPhoenix(theKnight, MaxHP);
            knightEffected.Poison = 0;
        }
    }
}

// Obtaining Paladin shield
void event95(weapon& knightWeapon)
{
  knightWeapon.Shield = true;
}

// Obtaining Lancelot’s spear
void event96(weapon& knightWeapon)
{
  knightWeapon.Spear = true;
}

// Obtaining Guinevere’s hair
void event97(weapon& knightWeapon)
{
  knightWeapon.Hair = true;
}

// Obtaining Excalibur sword
void event98(weapon& knightWeapon)
{
  if(knightWeapon.Hair == true && 
    knightWeapon.Shield == true && 
    knightWeapon.Spear == true)
    knightWeapon.Excalibur = true;
}

// Encountering Ultimecia
void event99(knight& theKnight,weapon& knightWeapon, bool& bFlag, report*& pReturn)
{
  if(knightWeapon.Excalibur == true)
  {  
    bFlag = true;
    pReturn->nWin += 1;
  }
  else
  {
    if(theKnight.HP < 3)
    {
      theKnight.HP = 1;
    }
    else
    {
      theKnight.HP /= 3;
    }
    pReturn->nLose += 1;
  }
}

//Meeting MadBear
bool event1(int countEvent, knight& theKnight){
    int OpponentLV = levelO(countEvent);
    if (theKnight.level >= OpponentLV) {
        theKnight.gil += 100;
        battleResult = true;
    }
    else {
        int dmg =int(1 * OpponentLV * 10);
        theKnight.HP -= dmg;
        battleResult = false;
    }
    return battleResult;
}

//Meeting Amazon female warrior MoonBringer
bool event2(int countEvent, knight& theKnight) {
    int OpponentLV = levelO(countEvent);
    if (theKnight.level >= OpponentLV) {
        theKnight.gil += 150;
        battleResult = true;
    }
    else {
        int dmg = int (1.5 * OpponentLV * 10);
        theKnight.HP -= dmg;
        battleResult = false;
    }
    return battleResult;
}

//Meeting Elf – item: Antidote
bool event3(int countEvent, item& knightItem, knight& theKnight) {
    int OpponentLV = levelO(countEvent);
    if (theKnight.level >= OpponentLV) {
        theKnight.gil += 450;
        knightItem.DWings = true;
    }
    else {
        int dmg = int(4.5 * OpponentLV * 10);
        theKnight.HP -= dmg;
        battleResult = false;
    }
    return battleResult;
}

//Meeting Saxon warrior
bool event4(int countEvent, knight& theKnight) {
    int OpponentLV = levelO(countEvent);
    if (theKnight.level >= OpponentLV) {
        theKnight.gil += 650;
        battleResult = true;
    }
    else {
        int dmg = int (6.5 * OpponentLV * 10);
        theKnight.HP -= dmg;
        battleResult = false;
    }
    return battleResult;
}

//Meeting Troll: item: Devil’s wings
bool event5(int countEvent, effect& knightEffected, item& knightItem, knight& theKnight) {
    int OpponentLV = levelO(countEvent);
    if (theKnight.level >= OpponentLV) {
        theKnight.gil += 850;
        if(knightItem.DWings == false && knightEffected.Curse == 0){
           knightItem.DWings = true;
           knightEffected.Curse = 5;
           if (knightItem.Antidote == true) {
               knightEffected.Curse = 0;
               knightItem.Antidote = false;
           }
        }
        else if(knightItem.DWings == true && knightEffected.Curse == false){
            knightEffected.Curse = false;
        }
        battleResult = true;
    }
    else {
        int dmg = int(8.5 * OpponentLV * 10);
        theKnight.HP -= dmg;
        battleResult = false;
    }
    return battleResult;
}

void processEvent(int countEvent, int event, effect& knightEffected, item& knightItem, knight& theKnight, weapon& knightWeapon, int& nPetal,
                  bool& bFlag, report*&pReturn, bool& haveFight)
{
  switch (event)
  {
  case 1:
     event1(countEvent, theKnight);
     haveFight = true;
     check(knightEffected, theKnight, pReturn);
     break;
  case 2:
      event2(countEvent, theKnight);
      haveFight = true;
      check(knightEffected, theKnight, pReturn);
      break;
  case 3:
      event3(countEvent, knightItem, theKnight);
      haveFight = true;
      check(knightEffected, theKnight, pReturn);
      break;
  case 4:
      event4(countEvent, theKnight);
      haveFight = true;
      check(knightEffected, theKnight, pReturn);
      break;
  case 5:
      event5(countEvent, knightEffected, knightItem, theKnight);
      haveFight = true;
      check(knightEffected, theKnight, pReturn);
      break;
  case 95:
    event95(knightWeapon);
    break;
  case 96:
    event96(knightWeapon);
    break;
  case 97:
    event97(knightWeapon);
    break;
  case 98:
    event98(knightWeapon);
    break;
  case 99:
    event99(theKnight, knightWeapon, bFlag, pReturn);
    haveFight = true;
    break;
  default:
    break;
  }
}

report* walkthrough(knight& theKnight, castle arrCastle[], int nCastle, int mode, int nPetal)
{
    MaxHP = theKnight.HP;
    report* pReturn = new report();
    weapon knightWeapon;
    item knightItem;
    effect knightEffected;
    bool bFlag = false,
        haveFight = false;
    int countEvent = 1;

    //fighting for the existence of mankind here
    int iCastle = 0;
    while (true)
    {
        castle cas = arrCastle[iCastle];
        haveFight = false;
        for (int j = 0; j < cas.nEvent; j++)
        {
            int e = cas.arrEvent[j];

            processEvent(countEvent, e, knightEffected , knightItem, theKnight, knightWeapon, nPetal, bFlag, pReturn, haveFight);
            countEvent++;
            nPetal -= 1;
            if (bFlag == true)
                break;
            if (nPetal == 0)
            {
                bFlag = false;
                break;
            }
            if (knightEffected.Curse ) {
                knightEffected.Curse -= 1;
            }
        }
        if (bFlag == true || (bFlag == false && nPetal == 0))
            break;
        iCastle += 1;

        // // Have fight -> increase level, health, gil
        if (haveFight == true){
            theKnight.level += 1;
            if (theKnight.level > 10) {
                theKnight.level = 10;
                MaxHP += 100;
            }
            if (MaxHP > 999) {
                MaxHP = 999;
            }
        }

        if (iCastle >= nCastle)
            iCastle = 0;
    }
    pReturn->nPetal = nPetal;
    // success or failure?
    pReturn = (bFlag) ? pReturn : NULL;
    return pReturn;
}
