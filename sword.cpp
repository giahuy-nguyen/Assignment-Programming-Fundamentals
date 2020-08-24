#ifndef _definition_h_
#include "defs.h"
#define _definition_h_
#endif

int MaxHP;
bool battleResult = true;

struct weapon{
  bool Shield = false;
  bool Spear = false;
  bool Hair = false;
  bool Excalibur = false;
};

struct item {
    bool Antidote = false;
    bool DWings = false;
};

struct effect {
    bool Curse = false;
};

//Calculate level Opponent
int levelO(int countEvent) {
    int b = countEvent % 10;
    int levelO = countEvent > 6 ? (b > 5 ? b : 5) : b;
    return levelO;
}


// Obtaining Paladin shield
void event95(weapon& knightWeapon, int &nPetal)
{
  knightWeapon.Shield = true;
}

// Obtaining Lancelot’s spear
void event96(weapon& knightWeapon, int &nPetal)
{
  knightWeapon.Spear = true;
}

// Obtaining Guinevere’s hair
void event97(weapon& knightWeapon, int& nPetal)
{
  knightWeapon.Hair = true;
}

// Obtaining Excalibur sword
void event98(weapon& knightWeapon, int& nPetal)
{
  if(knightWeapon.Hair == true && 
    knightWeapon.Shield == true && 
    knightWeapon.Spear == true)
    knightWeapon.Excalibur = true;
}

// Encountering Ultimecia
void event99(knight& theKnight,weapon& knightWeapon, int& nPetal, bool& bFlag, report*& pReturn)
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
void event1(int countEvent, knight& theKnight, int& nPetal){
    int OpponentLV = levelO(countEvent);
    if (theKnight.level >= OpponentLV) {
        theKnight.gil += 100;
        battleResult = true;
    }
    else {
        int dmg = 1 * OpponentLV * 10;
        theKnight.HP -= dmg;
        battleResult = false;
    }
}

//Meeting Amazon female warrior MoonBringer
void event2(int countEvent, knight& theKnight, int& nPetal) {
    int OpponentLV = levelO(countEvent);
    if (theKnight.level >= OpponentLV) {
        theKnight.gil += 150;
        battleResult = true;
    }
    else {
        double dmg = 1.5 * OpponentLV * 10;
        theKnight.HP -= dmg;
        battleResult = false;
    }
}

//Meeting Elf – item: Antidote
void event3(int countEvent, item& knightItem, knight& theKnight, int& nPetal) {
    int OpponentLV = levelO(countEvent);
    if (theKnight.level >= OpponentLV) {
        theKnight.gil += 450;
        knightItem.DWings = true;
    }
    else {
        int dmg = 4.5 * OpponentLV * 10;
        theKnight.HP -= dmg;
        battleResult = false;
    }
}

//Meeting Saxon warrior
void event4(int countEvent, knight& theKnight, int& nPetal) {
    int OpponentLV = levelO(countEvent);
    if (theKnight.level >= OpponentLV) {
        theKnight.gil += 650;
        battleResult = true;
    }
    else {
        int dmg = 6.5 * OpponentLV * 10;
        theKnight.HP -= dmg;
        battleResult = false;
    }
}

//Meeting Troll: item: Devil’s wings
void event5(int countEvent, effect& knightEffected, item& knightItem, knight& theKnight, int& nPetal) {
    int OpponentLV = levelO(countEvent);
    if (theKnight.level >= OpponentLV) {
        theKnight.gil += 850;
        if(knightItem.DWings == false && knightEffected.Curse == false){
           knightItem.DWings = true;
           knightEffected.Curse = true;
           if (knightItem.Antidote == true) {
               knightEffected.Curse = false;
           }
        }
        else if(knightItem.DWings == true && knightEffected.Curse == false){
            knightEffected.Curse = false;
        }
        battleResult = true;
    }
    else {
        int dmg = 8.5 * OpponentLV * 10;
        theKnight.HP -= dmg;
        battleResult = false;
    }
}

void processEvent(int countEvent, int event,item& knightItem, knight& theKnight, weapon& knightWeapon, int& nPetal,
                  bool& bFlag, report*&pReturn, bool& haveFight)
{
  switch (event)
  {
  case 1:
     event1(countEvent, theKnight, nPetal);
    
     break;
  case 2:
      event2(countEvent, theKnight, nPetal);
      break;
  case 3:
      event3(countEvent, knightItem, theKnight, nPetal);

      break;
  case 4:
   
      event4(countEvent, theKnight, nPetal);
      
      break;
  case 95:
  
    event95(knightWeapon, nPetal);
 
    break;
  case 96:
   
    event96(knightWeapon, nPetal);
    break;
  case 97:
     
    event97(knightWeapon, nPetal);
    
    break;
  case 98:
      
    event98(knightWeapon, nPetal);
  
    break;
  case 99:
    event99(theKnight, knightWeapon, nPetal, bFlag, pReturn);
    haveFight = true;
    break;
  default:
    break;
  }
}

report*  walkthrough (knight& theKnight, castle arrCastle[], int nCastle, int mode, int nPetal)
{
  MaxHP = theKnight.HP;
  report* pReturn = new report();
  weapon knightWeapon;
  item knightItem;
  bool bFlag = false,
      haveFight = false;
  int countEvent = 1;
  //fighting for the existence of mankind here
  int iCastle = 0;
  while(true)
  {
    castle cas = arrCastle[iCastle];
    haveFight = false;
    for(int j = 0;j < cas.nEvent;j++)
    {
      int e = cas.arrEvent[j];
      processEvent(countEvent,  e, knightItem, theKnight, knightWeapon, nPetal, bFlag, pReturn, haveFight);
      countEvent++;
      nPetal-=1;
      if(bFlag == true)
        break;
      if(nPetal == 0)
      {
        bFlag = false;
        break;
      }
    }
    if(bFlag == true || (bFlag == false && nPetal == 0))
      break;
    iCastle += 1;

    // // Have fight -> increase level, health, gil
    if(haveFight == true)
    {
      theKnight.level += 1;
      if(theKnight.level > 10) 
        theKnight.level = 10;
      MaxHP+=100;
      if (MaxHP > 999)
        MaxHP = 999;
      if (battleResult == true) {
          if (theKnight.gil > 999) {
              theKnight.gil = 999;
          }
      }
      else if (battleResult == false){
          if (theKnight.HP <= 0){
              pReturn->nLose += callPhoenix(theKnight, MaxHP);
          }
      }
    }

    if(iCastle >= nCastle)
      iCastle = 0;
  }
  pReturn->nPetal = nPetal;
  // success or failure?
  pReturn = (bFlag)? pReturn : NULL;
  return pReturn;
}
