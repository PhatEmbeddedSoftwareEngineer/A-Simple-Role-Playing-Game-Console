//a simple role-playing game

#include <iostream>
#include <ctime>

class CCreature
{
public:
	CCreature() : muiAttack(0), muiDefense(0), muiHitPoints(3){
		for(unsigned int uiIndex = 0; uiIndex < 3; ++uiIndex) {
			muiAttack = muiAttack + (rand() % 34);  // muiAttack + 0->33 
			muiDefense = muiDefense + (rand() % 34); // muiDefense + 0 -> 33 
			muiHitPoints = muiHitPoints + (rand() % 10); // 3 + 0->9  
		}
	}

	void Attack(CCreature& qrDefender)
	{
		// generate a number between 1 and 100 
		unsigned int uiAttack1 = (rand() % 100) + 1;
		unsigned int uiDefense1 = (rand() % 100) + 1;
		unsigned int uiAttack2 = (rand() % 100) + 1;
		unsigned int uiDefense2 = (rand() % 100) + 1;
		// Did the attacker (hero) hit?
		if(uiAttack1 < muiAttack && uiDefense1 > qrDefender.muiDefense) {
			--qrDefender.muiHitPoints;
			std::cout << "Monster hit! " << std::endl;
		}
		else {
			std::cout << "Monster missed!" << std::endl;
		}
		// did the monster hit
		if(uiAttack2 < qrDefender.muiAttack && uiDefense2 > muiDefense){
			--muiHitPoints;
			std::cout << "Hero hit!" << std::endl;
		}
		else {
			std::cout << "Hero Missed!" << std::endl;
		}
		std::cout << "Hero HitPoints!" << muiHitPoints << std::endl;
		std::cout << "Monster HitPoints!" << qrDefender.muiHitPoints << std::endl;
	}

	bool IsDead(){
		return (muiHitPoints == 0);
	}

private:
	unsigned int muiAttack;
	unsigned int muiDefense;
	unsigned int muiHitPoints;
};

class CDungeon{
public:
	CDungeon() {
		char caaMaze[10][11] = {
			"**********", // ky tu /0 
			"* **  ** *",
			"* *  **  *",
			"*        *",
			"* *  *** *",
			"* ** * ***",
			"* *  *   *",
			"* **     *",
			"*  * *** *",
			"**********"
		};

		for(unsigned int uiRow = 0; uiRow < 10; ++uiRow) {
			for(unsigned int uiCol = 0; uiCol < 10; ++uiCol) {
				mcaaMaze[uiRow][uiCol] = caaMaze[uiRow][uiCol];
			}
		}
	}

	char GetMazeSquare(unsigned int uiRow, unsigned int uiCol){
		return mcaaMaze[uiRow][uiCol];
	}
	
private:
	char mcaaMaze[10][10];
};

class CRolePlayingGame {
public:
	CRolePlayingGame(){
		// initialize the random number generator 
		time_t qTime; // int64_t 8 bytes
		time(&qTime); // get time 
		srand((unsigned int)qTime); 
		
		// Initialize the dungeon to the empty
		for(unsigned int uiRow = 0; uiRow < 10; ++uiRow){
			for(unsigned int uiCol = 0; uiCol < 10; ++uiCol){
				mqpaaCreatures[uiRow][uiCol] = 0; // assign nullptr for pointer 
			}
		}
		
		// create a hero 
		bool bFoundSpot = false;
		while(!bFoundSpot){
			unsigned int uiRow = 1 + (rand() % 8); // 1->8
			unsigned int uiCol = 1 + (rand() % 8); // 1->8 
			if (QueryLocation(uiRow, uiCol) == ' ') { 
				bFoundSpot = true;
				mqpaaCreatures[uiRow][uiCol] = &mqHero; 
									
			}
		}
		// create 10 monsters
		bFoundSpot = false;
		unsigned int uiMonster = 0;
		while(!bFoundSpot){
			unsigned int uiRow = 1 + (rand() % 8); // 1->8
			unsigned int uiCol = 1 + (rand() % 8);  // 1->8 
			if(QueryLocation(uiRow, uiCol) == ' ') { 
				mqpaaCreatures[uiRow][uiCol] = &mqaMonsters[uiMonster];
				++uiMonster;
				if(uiMonster == 10){ // tao 10 con quai vat 
					bFoundSpot = true;
				}
			}
		}
	}
	
	char QueryLocation(unsigned int uiRow, unsigned int uiCol){
		for(unsigned int uIndex = 0; uIndex < 10; ++uIndex) {
			if(mqpaaCreatures[uiRow][uiCol] == &(mqaMonsters[uIndex])){
				return (char)('0' + uIndex);
			}
		}
		if (mqpaaCreatures[uiRow][uiCol] == &mqHero) {
			return 'H';
		}
		else { // case  ' '
			return mqDungeon.GetMazeSquare(uiRow, uiCol);
		}
	}

	bool MoveHero(const char kcDirection) {
		unsigned int uiHeroRow;
		unsigned int uiHeroCol;
		LocateCreature(uiHeroRow, uiHeroCol, &mqHero);
		unsigned int uiNextRow = uiHeroRow;
		unsigned int uiNextCol = uiHeroCol;
		switch(kcDirection){
			case 'w':
			case 'W':
			{
				--uiNextRow;
				break;
			}
			case 's':
			case 'S':
			{
				++uiNextCol;
				break;
				
			}
			case 'z':
			case 'Z':
			{
				++uiNextRow;
				break;
			}
			case 'a':
			case 'A':
			{
				--uiNextCol;
				break;
			}
			default:	
			{
				return false;
			}
		}

		char cNextLoc = QueryLocation(uiNextRow, uiNextCol);
		if (cNextLoc == ' '){
			mqpaaCreatures[uiNextRow][uiNextCol] = &mqHero;
			mqpaaCreatures[uiHeroRow][uiHeroCol] = 0;
			return true;
		}
		else if(cNextLoc >= '0' && cNextLoc <= '9')
		{
			mqHero.Attack(mqaMonsters[(int)(cNextLoc - '0')]);
			return true;
		}
		else 
		{
			return false;
		}
	}
	
	void PrintBoard() {
		for(unsigned int uiRow = 0; uiRow < 10; ++uiRow){
			for(unsigned int uiCol = 0; uiCol < 10; ++uiCol){
				std::cout << QueryLocation(uiRow, uiCol);
			}
			std::cout << std::endl;
		}
	}

	bool HeroIsDead() {
		return mqHero.IsDead();
	}

	void RemoveDeadMonsters() {
		for(unsigned int uiIndex = 0; uiIndex < 10; ++uiIndex) {
			if(mqaMonsters[uiIndex].IsDead()) {
				unsigned int uiRow;
				unsigned int uiCol;
				if(LocateCreature(uiRow, uiCol, &(mqaMonsters[uiIndex]))) {
					mqpaaCreatures[uiRow][uiCol] = 0;
					std::cout << "Monster Killed!" << std::endl;
				}
			}
		}
	}

	bool AllMonstersDead() {
		bool bAllDead = true;
		for (unsigned int uiIndex = 0; uiIndex < 10; ++uiIndex) {
			if(!mqaMonsters[uiIndex].IsDead())
			{
				bAllDead = false;
			}
		}
		return bAllDead;
	}

private:
	bool LocateCreature(unsigned int& uirRow, unsigned int& uirCol, CCreature* qpCreature){
		for(unsigned int uiRow = 0; uiRow < 10; ++uiRow){
			for(unsigned int uiCol = 0; uiCol < 10; ++uiCol){
				if(mqpaaCreatures[uiRow][uiCol] == qpCreature){
					uirRow = uiRow;
					uirCol = uiCol;
					return true;
				}
			}
		}
		return false;
	}

	CDungeon mqDungeon;
	CCreature mqHero;
	CCreature mqaMonsters[10];
	CCreature* mqpaaCreatures[10][10];

};

int main()
{
	CRolePlayingGame qGame;
	// cleared the dungeon
	bool bGameOver = false;
	do {
		qGame.PrintBoard();
		// get the next move
		char cMove;
		std::cout << "Enter w, a, s, or z  to move:" << std::endl;
		std::cin >> cMove;
		// check the move was valid 
		if(qGame.MoveHero(cMove)){
			// if the hero is dead 
			if(qGame.HeroIsDead()){
				std::cout << "You have died!" << std::endl;
				bGameOver = true;
			}else {
				// remove all dead monsters from game
				qGame.RemoveDeadMonsters();
				// if all of the monsters are dead 
				if(qGame.AllMonstersDead()){
					std::cout << "Dungeon clearned!" << std::endl;
					bGameOver = true;
				}
			}
		}
	}while(!bGameOver);
	return 0;
}
