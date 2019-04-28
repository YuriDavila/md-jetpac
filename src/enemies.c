/*
 * enemies.c
 *
 *  Created on: Apr 24, 2019
 *      Author: diegomtassis
 */

#include "../inc/enemies.h"

#include <genesis.h>

#include "../inc/commons.h"
#include "../inc/physics.h"
#include "../res/sprite.h"

#include "../inc/enemy_01.h"

#define SPEED_ZERO		FIX16(0)
#define SPEED_H_NORMAL	FIX16(1)
#define SPEED_V_NORMAL	FIX16(0.3)

#define MIN_POS_H_PX_F16	LEFT_POS_H_PX_F16 - FIX16(-8)
#define MAX_POS_H_PX_F16	RIGHT_POS_H_PX_F16 - FIX16(8)
#define MIN_POS_V_PX_F16	TOP_POS_V_PX_F16
#define MAX_POS_V_PX_F16	BOTTOM_POS_V_PX_F16 - FIX16(16)

#define LAST_ENEMY_CREATION_TIMER 0

static void addEnemy(Level*, u8 pos);
static Enemy* createEnemy();
static void releaseEnemy(Enemy*);
static void enemiesJoin(Level* level);

static void moveEnemy(Enemy*, const Level*);
static void calculateNextMovement(Enemy*);
static void updatePosition(Enemy*, const Level*);
static fix16 crashedHIntoPlatform(Box_f16 subject_box, const Level* level);
static fix16 crashedVIntoPlatform(Box_f16 subject_box, const Level* level);
static Sprite* createSprite(Enemy* enemy);

static void updateSprite(Enemy* enemy);

static void detectNuclearBomb();

u8 nuclear_bomb;

void startEnemies(Level* level) {

	if (!level->enemies->objects) {
		// First time
		level->enemies->objects = MEM_alloc(sizeof(Enemy*) * level->enemies->max_num_enemies);
		setRandomSeed(getTick());
	}

	u8 num_enemies = level->enemies->max_num_enemies / 2; // start with half the maximum enemies
	while (num_enemies--) {
		addEnemy(level, num_enemies);
	}
}

void enemiesAct(Level* level) {

	detectNuclearBomb();

	u8 num_enemies = level->enemies->max_num_enemies;

	while (num_enemies--) {

		Enemy* enemy = level->enemies->objects[num_enemies];
		if (enemy) {

			if (nuclear_bomb) {
				enemy->alive = FALSE;
			} else {
				moveEnemy(enemy, level);
			}

			updateSprite(enemy);
		}
	}

	// New enemies joining the party?
	enemiesJoin(level);

	nuclear_bomb = FALSE;
}

void releaseAllEnemies(Level* level) {

	u8 num_enemies = level->enemies->max_num_enemies;
	while (num_enemies--) {

		Enemy* enemy = level->enemies->objects[num_enemies];
		if (enemy) {

			releaseEnemy(enemy);
			level->enemies->objects[num_enemies] = NULL;
			level->enemies->current_num_enemies--;
		}
	}
}

void releaseDeadEnemies(Level* level) {

	u8 num_enemies = level->enemies->max_num_enemies;
	while (num_enemies--) {

		Enemy* enemy = level->enemies->objects[num_enemies];
		if (enemy && !enemy->alive) {

			releaseEnemy(enemy);
			level->enemies->objects[num_enemies] = NULL;
			level->enemies->current_num_enemies--;
		}
	}
}

static void releaseEnemy(Enemy* enemy) {

	SPR_releaseSprite(enemy->sprite);
	MEM_free(enemy->sprite);
	MEM_free(enemy->object.box);
	MEM_free(enemy);
}

static void addEnemy(Level* level, u8 pos) {

	Enemy* enemy = createEnemy();
	level->enemies->objects[pos] = enemy;
	enemy->sprite = createSprite(enemy);
	level->enemies->current_num_enemies++;
}

static Enemy* createEnemy() {

	Enemy* enemy = MEM_alloc(sizeof(Enemy));
	enemy->alive = TRUE;

	// position & direction
	if (random() % 2) {
		enemy->object.pos.x = MIN_POS_H_PX_F16;
		enemy->object.mov.x = SPEED_H_NORMAL;
	} else {
		enemy->object.pos.x = MAX_POS_H_PX_F16;
		enemy->object.mov.x = -SPEED_H_NORMAL;
	}
	enemy->object.pos.y = randomInRangeFix16(MIN_POS_V_PX_F16, MAX_POS_V_PX_F16);

	// V speed
	if (random() % 2) {
		enemy->object.mov.y = SPEED_ZERO;
	} else {
		enemy->object.mov.y = SPEED_V_NORMAL;
	}

	// box
	enemy->object.box = MEM_alloc(sizeof(Box_f16));
	enemy->object.box->w = ENEMY_01_WIDTH;
	enemy->object.box->h = ENEMY_01_HEIGHT;
	enemy->object.box->x = enemy->object.pos.x;
	enemy->object.box->y = enemy->object.pos.y;

	startTimer(LAST_ENEMY_CREATION_TIMER);
	return enemy;
}

static void enemiesJoin(Level* level) {

	if (level->enemies->current_num_enemies < level->enemies->max_num_enemies
			&& getTimer(LAST_ENEMY_CREATION_TIMER, FALSE) > SUBTICKPERSECOND * 2) {

		u8 num_enemies = level->enemies->max_num_enemies;
		u8 idx;
		while (num_enemies--) {
			// find the first empty slot
			Enemy* enemy = level->enemies->objects[num_enemies];
			if (!enemy) {
				idx = num_enemies;
				break;
			}
		}

		addEnemy(level, idx);
	}
}

static void moveEnemy(Enemy* enemy, const Level* level) {

	calculateNextMovement(enemy);
	updatePosition(enemy, level);
}

static void calculateNextMovement(Enemy* enemy) {
	// nothing for the moment
}

static void updatePosition(Enemy* enemy, const Level* level) {

	// horizontal position
	Box_f16 target_h = targetHBox(&enemy->object, ENEMY_01_WIDTH, ENEMY_01_HEIGHT);
	if (target_h.x > MAX_POS_H_PX_F16) {
		enemy->object.pos.x = MIN_POS_H_PX_F16;

	} else if (target_h.x < MIN_POS_H_PX_F16) {
		enemy->object.pos.x = MAX_POS_H_PX_F16;

	} else if (crashedHIntoPlatform(target_h, level)) {
		enemy->alive = FALSE;
	} else {
		enemy->object.pos.x = target_h.x;
	}

	// vertical position
	Box_f16 target_v = targetVBox(&enemy->object, ENEMY_01_WIDTH, ENEMY_01_HEIGHT);
	if (crashedVIntoPlatform(target_v, level)) {
		enemy->alive = FALSE;
	} else {
		enemy->object.pos.y = target_v.y;
	}

	// update box
	enemy->object.box->x = enemy->object.pos.x;
	enemy->object.box->y = enemy->object.pos.y;
}

static fix16 crashedHIntoPlatform(Box_f16 subject_box, const Level* level) {

	fix16 crashed;
	for (u8 i = 0; i < level->num_platforms; i++) {
		Box_f16 platform_box = *level->platforms[i]->object.box;
		crashed = hitLeft(subject_box, platform_box) //
		|| hitRight(subject_box, platform_box);
		if (crashed) {
			return crashed;
		}
	}

	return 0;
}

static fix16 crashedVIntoPlatform(Box_f16 subject_box, const Level* level) {

	fix16 crashed = hitAbove(subject_box, *level->floor->object.box);
	if (crashed) {
		return crashed;
	}

	for (u8 i = 0; i < level->num_platforms; i++) {
		Box_f16 platform_box = *level->platforms[i]->object.box;
		crashed = hitAbove(subject_box, platform_box) //
		|| hitUnder(subject_box, platform_box);
		if (crashed) {
			return crashed;
		}
	}

	return 0;
}

static Sprite* createSprite(Enemy* enemy) {

	Sprite* enemySprite = SPR_addSprite(&enemy_01_sprite, fix16ToInt(enemy->object.pos.x),
			fix16ToInt(enemy->object.pos.y), TILE_ATTR(PAL0, TRUE, FALSE, FALSE));
	SPR_setAnim(enemySprite, (abs(random())) % 4);
	SPR_setFrame(enemySprite, (abs(random())) % 2);

	if (enemy->object.mov.x > 0) {
		SPR_setHFlip(enemySprite, TRUE);
	}

	return enemySprite;
}

static void updateSprite(Enemy* enemy) {

	if (enemy->alive) {
		SPR_setPosition(enemy->sprite, fix16ToInt(enemy->object.pos.x), fix16ToInt(enemy->object.pos.y));
	}
}

static void detectNuclearBomb() {

	if (JOY_readJoypad(JOY_1) & BUTTON_A) {
		nuclear_bomb = TRUE;
	}
}
