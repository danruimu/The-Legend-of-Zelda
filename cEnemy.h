#pragma once
#include "cbicho.h"
#define	ENEMY_DOWN	0
#define	ENEMY_RIGHT	1
#define	ENEMY_UP	2
#define	ENEMY_LEFT	3

#define	ENEMY_STATE_IDLE	0
#define	ENEMY_STATE_MOVE	1

#define FILE_PROP_OCTOROK_B	"enemies/octorok-b.txt"
#define FILE_PROP_FAT_DOG_O "enemies/fat_dog-o.txt"

#define OCTOROK_B	"octorok-b"
#define FAT_DOG_O	"fat_dog-o"

#define ROCK	0

#define ENEMY_SIZE	16

class cEnemy :
	public cBicho
{
public:
	cEnemy(void);
	cEnemy(int x, int y, char *type);
	~cEnemy(void);

	void draw();

private:
	int life;
	int maxLife;
	char *type;
	int damage;
	bool proyectile;
	char *typeProyectile;
	float probShoot;
	float probDropHeart;
	float probDropRuppe;
	int x;
	int y;
	int state;
	int direction;
	int tex_id;
};

