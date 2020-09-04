#ifndef _definition_h_
#include "defs.h"
#define _definition_h_
#endif

#include <vector>
#include <utility>
#include <cmath>

int MaxHP;
std::vector<bool> checkLucky(1001, true);
std::vector<bool> checkPrime(1001, true);
std::vector<bool> checkSmith(1001, false);

struct weapon {
	bool shield = false;
	bool spear = false;
	bool hair = false;
	bool excalibur = false;
};

struct item {
	int antidote = 0;
	bool devil_wings = false;
	int lionHeart = 0;
	bool mythrillArmor = false;
	bool goldCoin = false;
	bool scarletKatana = false;
};

struct infor {
	int cutAllDurian = 0;
	bool defeatOmega = false;
	int revivalEventIdx, revivialICaslte = -1;
	bool revivalGateDisappear = false;
	std::string typeKnight = "normal";
	int poison = 0;
	int curse = 0;
	int odinHelp = 0;
	int fakeArmor = 0;
	int meetFakeOdin = -1;
	int old_event_index, old_icaslte;
	std::vector<std::pair<int, int>> pos_curse;
};

// Custom call phoenix for revival position
void Revival(knight& theKnight, infor& knightInfor, int& iCaslte, int& eventIndex)
{
	callPhoenix(theKnight, MaxHP);
	if (!knightInfor.revivalGateDisappear && knightInfor.revivialICaslte != -1) {
		iCaslte = knightInfor.revivialICaslte;
		eventIndex = knightInfor.revivalEventIdx;
		knightInfor.revivalGateDisappear = true;
	}
	knightInfor.poison = 0;
	knightInfor.curse = 0;
}


/* 1. Meeting MadBear
2. Meeting Amazon female warrior MoonBringer
3. Meeting Elf – item: Antidote
4. Meeting Saxon warrior
5. Meeting Troll: item: Devil’s wings
*/
void Event_1_5(int eventCode, weapon& knightWeapon, item& knightItem, knight& theKnight, infor& knightInfor,
	int& nPetal, report*& pReturn, int& indexEvent, int& iCaslte)
{
	int gil[6] = { 0, 100, 150, 450, 650, 850 };
	double baseDame[6] = { 0, 1, 1.5, 4.5, 6.5, 8.5 };

	int b = indexEvent % 10;
	int levelO = indexEvent > 6 ? (b > 5 ? b : 5) : b;

	int tmpHP = theKnight.HP;

	double baseDamage = baseDame[eventCode];
	int damage = int(baseDamage) * levelO * 10;

	if (((knightInfor.meetFakeOdin < 0 && ((theKnight.level >= levelO && !knightInfor.fakeArmor) || knightItem.lionHeart || knightInfor.odinHelp)) ||
		knightInfor.typeKnight == "arthur" && knightWeapon.excalibur) || knightInfor.typeKnight == "lancelot" || knightInfor.typeKnight == "paladin") {
		theKnight.gil += gil[eventCode];
		pReturn->nWin += 1;
		if (theKnight.gil > 999)
			theKnight.gil = 999;

		if (eventCode == 3)
			knightItem.antidote += 1;
		if (eventCode == 5 && knightInfor.odinHelp == false)
		{
			bool met = false;
			for (int i = 0; i < knightInfor.pos_curse.size(); i++)
			{
				int icaslte = knightInfor.pos_curse[i].first;
				int eventindex = knightInfor.pos_curse[i].second;
				if (icaslte == iCaslte && eventindex == indexEvent)
					met = true;
			}
			if (!met && !knightInfor.curse && !knightItem.devil_wings) {
				knightItem.devil_wings = true;
				knightInfor.old_event_index = indexEvent;
				knightInfor.old_icaslte = iCaslte;
				knightInfor.curse = 6;
				knightInfor.pos_curse.push_back(std::pair<int, int>{iCaslte, indexEvent});
			}
		}

		if (knightInfor.poison) {
			theKnight.HP -= damage;
			if (theKnight.HP <= 0) {
				Revival(theKnight, knightInfor, iCaslte, indexEvent);
			}
		}
	}
	else {
		// The knight does not take dame if have mythrill armor or knight is King Arthur.
		if (!knightItem.mythrillArmor && knightInfor.typeKnight != "arthur")
		{
			if (eventCode == 2 && knightInfor.typeKnight == "guinevere")
			{
				theKnight.HP /= 4;
			}
			else
			{
				theKnight.HP -= damage;
			}
			if (theKnight.HP <= 0) {
				Revival(theKnight, knightInfor, iCaslte, indexEvent);
			}
		}
		if (knightInfor.typeKnight != "arthur") {
			pReturn->nLose += 1;
		}
		if (knightInfor.poison)
		{
			theKnight.HP /= 2;
		}
	}
	if (knightInfor.curse && knightItem.antidote)
	{
		knightItem.antidote -= 1;
		indexEvent = knightInfor.old_event_index;
		iCaslte = knightInfor.old_icaslte;
		knightInfor.curse = 0;
		knightInfor.poison = 0;
	}
}

// 6. Meeting Tornbery
void Event_6(knight& theKnight, weapon& knightWeapon, item& knightItem, infor& knightInfor, int& nPetal, report*& pReturn, int& indexEvent)
{
	if (knightInfor.poison)
		return;
	int b = indexEvent % 10;
	int levelO = indexEvent > 6 ? (b > 5 ? b : 5) : b;

	if ((knightInfor.meetFakeOdin < 0 && ((theKnight.level >= levelO && !knightInfor.fakeArmor) || knightItem.lionHeart || knightInfor.odinHelp)) ||
		knightInfor.typeKnight == "arthur" && knightWeapon.excalibur)
	{
		theKnight.level += 1;
		if (theKnight.level > 10)
			theKnight.level = 10;
		pReturn->nWin += 1;
	}
	else
	{
		if (knightInfor.typeKnight != "arthur")
		{
			if (knightItem.antidote == 0)
				if (knightInfor.typeKnight != "paladin" && knightInfor.typeKnight != "dragonknight")
					knightInfor.poison = 6;
				else knightItem.antidote -= 1;
			pReturn->nLose += 1;
		}
	}
}

// 7a. Check Fibonancy
bool isPerfectSquare(int x)
{
	int s = sqrt(x);
	return (s * s == x);
}

// Returns true if n is a Fibinacci Number, else false 
bool isFibonacci(int n)
{
	// n is Fibinacci if one of 5*n*n + 4 or 5*n*n - 4 or both 
	// is a perferct square 
	return isPerfectSquare(5 * n * n + 4) ||
		isPerfectSquare(5 * n * n - 4);
}

// 7. Meeting Queen of Cards
void Event_7(knight& theKnight, weapon& knightWeapon, item& knightItem, infor& knightInfor, report* pReturn, int& nPetal)
{
	if ((knightInfor.meetFakeOdin < 0 && (isFibonacci(theKnight.HP) || knightInfor.odinHelp)) ||
		knightInfor.typeKnight == "arthur" && knightWeapon.excalibur)
	{
		theKnight.gil *= 2;
		if (!knightItem.mythrillArmor && !knightItem.scarletKatana)
		{
			knightInfor.fakeArmor = 6;
		}
		pReturn->nWin += 1;
	}
	else
	{
		if (knightInfor.typeKnight != "arthur") {
			theKnight.HP /= 2;
			pReturn->nLose += 1;
		}
		if (!knightItem.scarletKatana)
		{
			theKnight.gil /= 2;
			knightItem.mythrillArmor = 0;
		}
	}
}

// 8a. Init boolean lucky array number event_8 meeting Nina de Rings
void InitCheckLucky()
{
	// HP <= 999
	// checkLucky: boolean array -> true (lucky number), false (not a lucky number).
	for (int i = 0; i < checkLucky.size(); i++)
		if (i % 2 == 0)
			checkLucky[i] = false;

	int n_times = 2, cnt_idx = 0, idx = 2;
	while (true) {
		cnt_idx = 0;
		// find first index
		for (int i = 1; i < checkLucky.size(); i++)
		{
			if (checkLucky[i])
			{
				cnt_idx += 1;
				if (cnt_idx == n_times)
				{
					idx = i;
				}
			}
		}

		// break when the number of true in the lucky array less than idx.
		if (cnt_idx < idx)
			break;

		// from first index, delete number have index % idx == 0.
		int index_number = 0;
		for (int i = 1; i < checkLucky.size(); i++)
		{
			if (checkLucky[i])
				index_number += 1;
			if (index_number % idx == 0)
				checkLucky[i] = false;
		}

		n_times += 1;
	}
}

// 8b. Check friendly number
bool IsFriend(int n, int m)
{
	int sum1 = 0, sum2 = 0;
	for (int i = 1; i <= n; i++)
		if (n % i == 0)
			sum1 += i;
	for (int i = 1; i <= m; i++)
	{
		if (m % i == 0)
			sum2 += i;
	}
	return (sum1 / n) == (sum2 / m);
}


// 8. Meeting the merry merchant Nina de Rings
void Event_8(knight& theKnight, item& knightItem, infor& knightInfor, int& nPetal)
{
	// Nina de Rings will chase away if HP is not lucky number
	if (!checkLucky[theKnight.HP])
	{
		return;
	}
	bool isFriend = IsFriend(theKnight.HP, theKnight.gil);
	// The Knight will continue if gil less than 50 and HP
	if (theKnight.gil < 50 && !isFriend && knightInfor.typeKnight != "guinevere")
		return;

	if (knightInfor.fakeArmor || knightInfor.poison)
	{
		if (!isFriend && knightInfor.typeKnight != "guinevere")
		{
			theKnight.gil -= 50;
		}
		knightInfor.fakeArmor = 0;
		knightInfor.poison = 0;
	}
	if (isFriend)
	{
		theKnight.HP = MaxHP;
		knightItem.lionHeart = 5;
	}
	else if (knightInfor.typeKnight == "guinevere")
	{
		int diffHP = MaxHP - theKnight.HP;
		int recover = diffHP;
		if (diffHP > theKnight.gil)
			recover = theKnight.gil;
		theKnight.HP += recover;
		theKnight.gil += 60;
		if (theKnight.gil > 999)
			theKnight.gil = 999;
	}
	else {
		int diffHP = MaxHP - theKnight.HP;
		int recover = diffHP;
		if (diffHP > theKnight.gil)
			recover = theKnight.gil;
		theKnight.HP += recover;
		theKnight.gil -= recover;
	}
}

//
// 9.Lost into the Durian Garden 
void Event_9(knight& theKnight, item& knightItem, infor& knightInfor, int& nPetal)
{
	// If go to durian tree when have scarlet katana -> cut all tree
	if (knightItem.scarletKatana || knightInfor.cutAllDurian)
		knightInfor.cutAllDurian += 1;

	// If all durian tree haved cut -> return
	if (knightInfor.cutAllDurian)
		return;

	knightInfor.odinHelp = 0;

	knightInfor.poison = 0;
	theKnight.HP = MaxHP;
	nPetal += 20;
	if (nPetal > 99)
		nPetal = 99;
}

//
// 10.Obtaining Poisonous mushroom
void Event_10(knight& theKnight, infor& knightInfor, int& nPetal, int& iCaslte, int& indexEvent)
{
	if (knightInfor.odinHelp)
		return;
	if (knightInfor.poison)
	{
		Revival(theKnight, knightInfor, iCaslte, indexEvent);
	}
	else
	{
		knightInfor.poison = 6;
		theKnight.HP /= 2;
	}
}

//
// 11. Meeting Odin
void Event_11(knight& theKnight, infor& knightInfor, int& nPetal, std::vector<std::pair<int, int>> road, castle arrCastle[])
{
	// If meet fake Odin 3 times, the knight realise fake odin and ignore help.
	if (knightInfor.meetFakeOdin == -2)
		return;

	// If have fake Odin -> increase times meet fake Odin.
	if (knightInfor.meetFakeOdin >= 0)
	{
		knightInfor.meetFakeOdin += 1;
		if (knightInfor.meetFakeOdin == 3)
			knightInfor.meetFakeOdin = -2;
	}
	std::vector<std::pair<int, int>> tmp = road;
	// Trace back to 3 event, if one is Durian garden (Event 9), Odin will refure to help.
	bool lostInDurian = false;
	int times = 3;
	if (times > tmp.size())
		times = tmp.size();
	for (int i = 0; i < times; i++)
	{
		std::pair<int, int> last = tmp[tmp.size() - 1];
		// The past 3 event have Durian Garden.
		if (arrCastle[last.first].arrEvent[last.second - 1] == 9 && knightInfor.cutAllDurian <= 1)
		{
			lostInDurian = true;
			break;
		}
		tmp.pop_back();
	}
	if (!lostInDurian && knightInfor.meetFakeOdin >= 0)
		knightInfor.odinHelp = 6;
}

//
// 12. Meeting Merlin the Wizard
void Event_12(knight& theKnight, item& knightItem, infor& knightInfor, int& nPetal)
{
	knightInfor.fakeArmor = 0;
	knightInfor.poison = 0;
	theKnight.HP = MaxHP;
	// Give a gold coin
	knightItem.goldCoin = true;
}

//
// 13. Meeting Omega Weapon
void Event_13(knight& theKnight, weapon& knightWeapon, item& knightItem,
	infor& knightInfor, int& nPetal, report* pReturn, int& iCaslte, int& indexEvent)
{
	// If Omega has defeated -> return
	if (knightInfor.defeatOmega == true)
		return;

	//Only knight at level 10 who has Excalibur or Dragon Knight at any level with Lionheart can defeat Omega Weapon
	if ((theKnight.level == 10 && knightWeapon.excalibur))
	{
		theKnight.gil = 999;
		theKnight.level = 10;
		MaxHP = 999;
		knightInfor.defeatOmega = true;
		pReturn->nWin += 1;
	}
	else {
		if (!knightItem.mythrillArmor)
		{
			Revival(theKnight, knightInfor, iCaslte, indexEvent);
		}
		pReturn->nLose += 1;
	}
}

//
// 14a. Init prime number array for HP.
void InitCheckPrime()
{
	checkPrime[0] = checkPrime[1] = false;
	for (int i = 2; i * i < checkPrime.size(); i++)
	{
		if (checkPrime[i] == true)
		{
			for (int j = i * i; j < checkPrime.size(); j += i)
				checkPrime[j] = false;
		}
	}
}

// 14. Meeting Hades
void Event_14(knight& theKnight, weapon& knightWeapon, item& knightItem, infor& knightInfor, int indexEvent, int& nPetal, report* pReturn)
{
	// Kill Odin and make fake Odin if the Knight have help from Odin.
	if (knightInfor.odinHelp)
	{
		if (knightInfor.typeKnight != "arthur" && knightInfor.typeKnight != "dragonknight")
		{
			knightInfor.odinHelp = 0;
			// Make fake Odin, default = -1, if = -2 -> meet greater than 3 -> kill fake Odin.
			knightInfor.meetFakeOdin = 0;
		}
		else {
			// win if the knight is Athur and have Odin 
			// or the Knight is Dragon Knight and have Odin
			knightItem.mythrillArmor = true;
			pReturn->nWin += 1;
			return;
		}
	}

	if (knightInfor.typeKnight != "paladin")
		theKnight.gil = 0;

	// Fight :D
	// Power of love + HP is prime
	bool win = false;
	if (checkPrime[theKnight.HP] == true)
	{
		if (knightWeapon.hair && knightWeapon.spear && !knightWeapon.excalibur)
			win = true;
		if (knightInfor.typeKnight == "arthur" && knightWeapon.hair)
			win = true;
		if (knightInfor.typeKnight == "lancelot" && knightWeapon.hair)
			win = true;
		if (knightInfor.typeKnight == "guinevere" && knightWeapon.spear)
			win = true;
	}

	if (knightItem.scarletKatana)
	{
		win = true;
		knightItem.scarletKatana = false;
	}

	if (knightItem.lionHeart)
		win = true;

	// Fight scene
	int b = indexEvent % 10;
	int levelO = indexEvent > 6 ? (b > 5 ? b : 5) : b;

	if (theKnight.level >= levelO || win)
	{
		knightItem.mythrillArmor = true;
		pReturn->nWin += 1;
	}
	else
	{
		if (knightItem.goldCoin)
		{
			knightItem.goldCoin = false;
			return;
		}

		if (!knightItem.mythrillArmor)
		{
			callPhoenix(theKnight, MaxHP);
			knightInfor.poison = 0;
		}
		pReturn->nLose += 1;
	}
}

//
// 15. Obtaining Scarlet Katana 
void Event_15(item& knightItem, int& nPetal)
{
	knightItem.scarletKatana = true;
}

//
//16. Meeting LockedDoor
void Event_16(knight& theKnight, infor& knightInfor, int& nPetal, int& indexEvent, report* pReturn)
{
	int idx = indexEvent % 10;
	if (theKnight.level <= idx && knightInfor.typeKnight != "lancelot" && knightInfor.typeKnight != "dragonknight")
	{
		indexEvent = 22; // Max event 20.
	}
}
//
// 17. Meeting RevivalDoor
void Event_17(knight& theKnight, infor& knightInfor, int& nPetal, int iEvent, int iCastle)
{
	// First see revivial gate
	if (knightInfor.revivialICaslte == -1)
	{
		knightInfor.revivialICaslte = iCastle;
		knightInfor.revivalEventIdx = iEvent;
	}
}

// Check can pick up treasure if mode == 1;
bool canPickUp(weapon& knightWeapon, int eventCode)
{
	int code[3] = { 95, 96, 97 };
	bool codePick[3] = { knightWeapon.shield, knightWeapon.spear, knightWeapon.hair };
	bool canPick = true;

	int priority = hash(eventCode);
	for (int i = 0; i < 3; i++)
	{
		if (priority > hash(code[i]) && codePick[i] == false)
		{
			canPick = false;
		}
	}
	return canPick;
}


// Obtaining Paladin shield
void Event_95(int mode, weapon& knightWeapon, int& nPetal)
{
	if (mode == 1)
		if (!canPickUp(knightWeapon, 95))
			return;
	knightWeapon.shield = true;
}

// Obtaining Lancelot’s spear
void Event_96(int mode, weapon& knightWeapon, int& nPetal)
{
	if (mode == 1)
		if (!canPickUp(knightWeapon, 96))
			return;
	knightWeapon.spear = true;
}

// Obtaining Guinevere’s hair
void Event_97(int mode, weapon& knightWeapon, int& nPetal)
{
	if (mode == 1)
		if (!canPickUp(knightWeapon, 97))
			return;
	knightWeapon.hair = true;
}

// Obtaining Excalibur sword
void Event_98(infor& knightInfor, weapon& knightWeapon, int& nPetal)
{
	if ((knightWeapon.hair == true && knightWeapon.shield == true && knightWeapon.spear == true)
		|| knightInfor.typeKnight == "arthur")
		knightWeapon.excalibur = true;
}

// Encountering Ultimecia
void Event_99(knight& theKnight, weapon& knightWeapon, item& knightItem, infor& knightInfor, int& nPetal, bool& bFlag, report*& pReturn)
{
	if ((knightWeapon.excalibur == true && !knightInfor.fakeArmor) || knightItem.lionHeart)
	{
		bFlag = true;
		pReturn->nWin += 1;
		if (knightInfor.poison)
		{
			if (knightInfor.typeKnight == "guinevere")
			{
				theKnight.HP /= 4;
			}
			else {
				if (theKnight.HP < 3)
				{
					theKnight.HP = 1;
				}
				else
				{
					theKnight.HP /= 3;
				}
			}
		}
	}
	else
	{
		if (knightInfor.typeKnight == "guinevere")
		{
			theKnight.HP /= 4;
			if (knightInfor.poison)
			{
				theKnight.HP /= 4;
			}
			pReturn->nLose += 1;
		}
		else {
			if (theKnight.HP < 3)
			{
				theKnight.HP = 1;
			}
			else
			{
				theKnight.HP /= 3;
			}
			pReturn->nLose += 1;

			if (knightInfor.poison)
			{
				theKnight.HP /= 2;
			}
		}
	}
}

// Get sum of all digits of number
int getSumPart(int n)
{
	int tmp, sum = 0;
	while (n != 0)
	{
		tmp = n % 10;
		n /= 10;
		sum += tmp;
	}
	return sum;
}

// Check Smith number
// 
bool IsSmith(int n)
{
	int sum1 = 0;
	int sum2 = getSumPart(n);
	for (int i = 2; i <= n;)
	{
		if (n % i == 0 && checkPrime[i]) {
			sum1 += getSumPart(i);
			n /= i;
		}
		else { i++; }
	}
	if (sum1 == sum2)
		return true;
	return false;
}

// Init Array Smith for Knight HP
void InitCheckSmith()
{
	for (int i = 0; i < checkSmith.size(); i++)
	{
		if (IsSmith(i))
		{
			checkSmith[i] = true;
		}
	}
}

// Check sum of Pythagorean Triplet
bool isSumPythagoreanTriplet(int n)
{
	long a, b, c;
	for (a = 1; a < n; a++)
	{
		b = (n * n - 2.0 * n * a) / (2 * n - 2 * a);
		c = n - b - a;
		if (b == int((n * n - 2 * n * a) / (2 * n - 2 * a)) && (a * a + b * b == c * c))
			return true;
	}
	return false;
}

void InitKnight(knight theKnight, weapon& knightWeapon, infor& knightInfor)
{
	if (theKnight.HP == 999)
	{
		knightInfor.typeKnight = "arthur";
	}
	else if (theKnight.HP == 888)
	{
		knightInfor.typeKnight = "lancelot";
		knightWeapon.spear = true;
	}
	else if (theKnight.HP == 777)
	{
		knightInfor.typeKnight = "guinevere";
		knightWeapon.hair = true;
	}
	else if (checkSmith[theKnight.HP])
	{
		knightInfor.typeKnight = "paladin";
		knightWeapon.shield = true;
	}
	else if (isSumPythagoreanTriplet(theKnight.HP))
	{
		knightInfor.typeKnight = "dragonknight";
	}
	else
	{
		knightInfor.typeKnight = "normal";
	}
}

void processEvent(int eventCode, knight& theKnight, weapon& knightWeapon, item& knightItem, infor& knightInfor, int& nPetal,
	bool& bFlag, report*& pReturn, bool& haveFight, int& indexEvent, int& iCaslte, std::vector<std::pair<int, int>> road, castle arrCastle[], int mode)
{
	switch (eventCode)
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
		Event_1_5(eventCode, knightWeapon, knightItem, theKnight, knightInfor, nPetal, pReturn, indexEvent, iCaslte);
		haveFight = true;
		break;
	case 6:
		Event_6(theKnight, knightWeapon, knightItem, knightInfor, nPetal, pReturn, indexEvent);
		haveFight = true;
		break;
	case 7:
		Event_7(theKnight, knightWeapon, knightItem, knightInfor, pReturn, nPetal);
		break;
	case 8:
		Event_8(theKnight, knightItem, knightInfor, nPetal);
		break;
	case 9:
		Event_9(theKnight, knightItem, knightInfor, nPetal);
		break;
	case 10:
		Event_10(theKnight, knightInfor, nPetal, iCaslte, indexEvent);
		break;
	case 11:
		Event_11(theKnight, knightInfor, nPetal, road, arrCastle);
		break;
	case 12:
		Event_12(theKnight, knightItem, knightInfor, nPetal);
		break;
	case 13:
		Event_13(theKnight, knightWeapon, knightItem, knightInfor, nPetal, pReturn, iCaslte, indexEvent);
		haveFight = true;
		break;
	case 14:
		Event_14(theKnight, knightWeapon, knightItem, knightInfor, indexEvent, nPetal, pReturn);
		haveFight = true;
		break;
	case 15:
		Event_15(knightItem, nPetal);
		break;
	case 16:
		Event_16(theKnight, knightInfor, nPetal, indexEvent, pReturn);
		break;
	case 17:
		Event_17(theKnight, knightInfor, nPetal, indexEvent, iCaslte);
		break;
	case 95:
		Event_95(mode, knightWeapon, nPetal);
		break;
	case 96:
		Event_96(mode, knightWeapon, nPetal);
		break;
	case 97:
		Event_97(mode, knightWeapon, nPetal);
		break;
	case 98:
		Event_98(knightInfor, knightWeapon, nPetal);
		break;
	case 99:
		Event_99(theKnight, knightWeapon, knightItem, knightInfor, nPetal, bFlag, pReturn);
		haveFight = true;
		break;
	default:
		break;
	}
}

report* walkthrough(knight& theKnight, castle arrCastle[], int nCastle, int mode, int nPetal)
{
	std::vector<std::pair<int, int>> eventIndex;
	MaxHP = theKnight.HP;
	report* pReturn = new report();
	weapon knightWeapon;
	item knightItem;
	infor knightInfor;
	bool bFlag = false,
		haveFight = false;
	//fighting for the existence of mankind here
	// Init check lucky std::vector for event Nina de Rings
	InitCheckPrime();
	InitCheckLucky();
	InitKnight(theKnight, knightWeapon, knightInfor);

	int iCastle = 0;
	while (true)
	{
		castle cas = arrCastle[iCastle];
		haveFight = false;
		for (int j = 0; j < cas.nEvent; j++)
		{

			int e = cas.arrEvent[j];
			int index = j + 1;
			processEvent(e, theKnight, knightWeapon, knightItem, knightInfor, nPetal, bFlag,
				pReturn, haveFight, index, iCastle, eventIndex, arrCastle, mode);
			nPetal -= 1;
			if (bFlag == true)
				break;
			if (nPetal == 0)
			{
				bFlag = false;
				break;
			}
			j = index - 1;
			cas = arrCastle[iCastle];
			std::pair<int, int> id = { iCastle, j };
			eventIndex.push_back(id);

			knightInfor.poison -= 1;
			if (knightInfor.poison < 0)
				knightInfor.poison = 0;

			knightInfor.fakeArmor -= 1;
			if (knightInfor.fakeArmor < 0)
				knightInfor.fakeArmor = 0;

			if (knightInfor.typeKnight != "paladin")
				knightItem.lionHeart -= 1;

			if (knightItem.lionHeart < 0)
				knightItem.lionHeart = 0;

			knightInfor.odinHelp -= 1;
			if (knightInfor.odinHelp < 0)
				knightInfor.odinHelp = 0;


			if (knightInfor.curse)
			{
				std::vector<std::pair<int, int>> cursePos;

				int times = 5;
				if (times > eventIndex.size() - 1)
					times = eventIndex.size() - 1;

				for (int i = 0; i < times; i++)
				{
					int lst = eventIndex.size() - 2 - i;
					cursePos.push_back(eventIndex[lst]);
				}

				haveFight = false;

				for (int i = 0; i < times; i++)
				{
					if (knightInfor.curse)
					{
						int e = arrCastle[cursePos[i].first].arrEvent[cursePos[i].second];
						processEvent(e, theKnight, knightWeapon, knightItem, knightInfor, nPetal, bFlag,
							pReturn, haveFight, index, iCastle, eventIndex, arrCastle, mode);

						nPetal -= 1;

						if (bFlag == true)
							break;
						if (nPetal == 0)
						{
							bFlag = false;
							break;
						}

						knightInfor.poison -= 1;
						if (knightInfor.poison < 0)
							knightInfor.poison = 0;

						knightInfor.fakeArmor -= 1;
						if (knightInfor.fakeArmor < 0)
							knightInfor.fakeArmor = 0;

						if (knightInfor.typeKnight != "paladin")
							knightItem.lionHeart -= 1;

						if (knightItem.lionHeart < 0)
							knightItem.lionHeart = 0;

						knightInfor.odinHelp -= 1;
						if (knightInfor.odinHelp < 0)
							knightInfor.odinHelp = 0;
					}
				}

				//iCastle = knightInfor.old_icaslte;
				//j = knightInfor.old_event_index + 1;
				//cas = arrCastle[iCastle];
			}
		}
		if (bFlag == true || (bFlag == false && nPetal == 0 && knightInfor.typeKnight != "arthur"))
			break;
		iCastle += 1;

		// Have fight -> increase level, health
		if (haveFight == true)
		{
			theKnight.level += 1;
			if (theKnight.level > 10)
				theKnight.level = 10;
			MaxHP += 100;
			if (MaxHP > 999)
				MaxHP = 999;
		}

		if (iCastle >= nCastle)
			iCastle = 0;
	}
	pReturn->nPetal = nPetal;
	if (pReturn->nPetal < 0)
		pReturn->nPetal = 0;
	// success or failure?
	pReturn = (bFlag) ? pReturn : NULL;
	return pReturn;
}