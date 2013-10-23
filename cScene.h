#pragma once
#include "utils.h"
#include "cTexture.h"
#include "cEnemy.h"
#include "cBoss.h"
#include "cObject.h"
#include "cData.h"
#include "cPlayer.h"

#define STR_DUNGEON	"dungeon"
#define DUNGEON_PROP		0//if its true indicates that the entry in this level its a dungeon; otherwise it indicates that it's a shop


#define OK						0
#define COLLIDES				1
#define COLLIDES_LOCKED_DOOR	2
#define OUTLIMITS				3
#define DUNGEON					4
#define MARKET					5
#define HURT					6

#define WALKABLE	0
#define DOOR		1
#define LOCKED_DOOR	2
#define WATER		3
#define STAIRS		4
#define OTHERS		-1

#define DOWN	0
#define UP		1
#define RIGHT	2
#define LEFT	3
#define DOWN_LEFT	4
#define UP_LEFT		5
#define UP_RIGHT	6
#define DOWN_RIGHT	7

#define TILE_DOOR	22

#define N_MAX_ENEMIES 42


class cScene
{
public:
	cScene(void);
	virtual ~cScene(void);

	bool LoadLevel(char* level,bool overridable, cData *data);
	void LoadLevelAnimation(char *oldLevel, char *newLevel, int dir,int obj_id,int tex_id, cPlayer *Link, int link_id);
	bool PrintMainMenu(int idMM);
	void Draw(int tex_id, int obj_id, bool mainMenu, char* text[], int currentText,int state);
	void newGameAnimation(int texID,int currentAnimation);
	int *GetMap();
	char *getId();
	void setId(char Nid[]);

	void drawPauseMenu(char *t1, char* t2, char *t3, int select);
	int Process(cRect *BoxOrg,String unlockedDoors[], String triforcesCollected[],cData *data, cPlayer *link);
	int whatsThere(int x,int y);
	void unlock();
	void drawEnemies();

	void freeEnemies();
	void freeObjects();
	bool isMarket();

	bool processAttacks(cRect swordBox);
	void processObjects(cPlayer *Link,int *n, int *vector);
	char* getxDooryDoor();
	void setxDooryDoor(int xDoor,int yDoor);
	bool getBossAlive();

private:
	int findTextureId(String str, cData *data);
	int map[SCENE_WIDTH * SCENE_HEIGHT];	//scene
	int id_DL;								//actual level display list
	char id[3];
	bool prop[NUM_PROPS];
	char *names[NUM_PROPS];
	void generateCallLevel();
	int xDoor,yDoor;//indicates the position of the door previusly gone by, if theres one; otherwise this value it's -1
	bool dungeon;//indicates if the current level is a dungeon or a market
	bool LoadDungeon(bool dungeon);
	bool exitingDoor;

	int whatsThereMonsterVersion(int x,int y);

	bool bossAlive;

	cObject *objects[NUM_MAX_OBJECTS];
	int nObjects;
	cEnemy *enemies[N_MAX_ENEMIES];
	int nEnemies;
	char *enemyTypes[NUM_ENEMIES];
	cBoss boss;
};
