/*
 * bubbles.c
 *
 *  Created on: May 30, 2019
 *      Author: diegomtassis
 */

#include <genesis.h>

#include "../../inc/constants.h"
#include "../../inc/elements.h"
#include "../../inc/enemies.h"
#include "../../inc/enemy.h"
#include "../../inc/fwk/commons.h"
#include "../../res/sprite.h"

#define BUBBLE_WIDTH    	16
#define BUBBLE_HEIGHT    	14

#define BUBBLE_DEFAULT_SPEED_H	FIX16(0.8)
#define BUBBLE_DEFAULT_SPEED_V	FIX16(0.8)

#define WAIT_BETWEEN_DIRECTION_CHANGE    	125

static Enemy* createBubble();
static void actBubble(Enemy enemy[static 1], Planet planet[static 1]);
static void releaseBubble(Enemy enemy[static 1]);

const EnemyDefinition bubbleDefinition = { //
		.type = BUBBLE, //
				.size_t.x = BUBBLE_WIDTH, //
				.size_t.y = BUBBLE_HEIGHT, //
				.createFunc = &createBubble, //
				.actFunc = &actBubble, //
				.releaseFunc = &releaseBubble };

typedef struct {
	u16 mov_counter;
} Bubble;

static f16 randomVSpeed();

static Enemy* createBubble() {

	Enemy* enemy = createEnemy(&bubbleDefinition);

	Bubble* bubble = MEM_calloc(sizeof *bubble);
	bubble->mov_counter = WAIT_BETWEEN_DIRECTION_CHANGE;
	enemy->extension = bubble;

	// position & movement
	initPosAndMov(enemy, BUBBLE_DEFAULT_SPEED_H, randomVSpeed());

	// box
	initBox(enemy);

	// sprite
	Sprite* enemySprite = SPR_addSprite(&bubble_sprite, fix16ToInt(enemy->object.pos.x),
			fix16ToInt(enemy->object.pos.y), TILE_ATTR(PAL0, TRUE, FALSE, FALSE));
	SPR_setAnim(enemySprite, (abs(random())) % 8); // 8 animations
	enemy->sprite = enemySprite;

	return enemy;
}

static void actBubble(Enemy enemy[static 1], Planet planet[static 1]) {

	Bubble* bubble = enemy->extension;

	bubble->mov_counter--;
	if (!bubble->mov_counter) {
		enemy->object.mov.y = randomVSpeed();
		bubble->mov_counter = WAIT_BETWEEN_DIRECTION_CHANGE;
	}

	Box_s16 target = targetBox(&enemy->object);

	if (target.min.y <= MIN_POS_V_PX_S16 || target.min.y >= MAX_POS_V_PX_S16) {
		enemy->object.mov.y = -enemy->object.mov.y;
		target = targetBox(&enemy->object);

	} else if (crashedIntoPlatform(target, planet)) {

		// THIS MUST BE OPTIMIZED

		// change horizontal direction
		enemy->object.mov.x = -enemy->object.mov.x;
		target = targetBox(&enemy->object);

		if (crashedIntoPlatform(target, planet)) {

			enemy->object.mov.x = -enemy->object.mov.x;
			enemy->object.mov.y = -enemy->object.mov.y;
			target = targetBox(&enemy->object);
		}
	}

	updatePosition(enemy, target);
}

static void releaseBubble(Enemy enemy[static 1]) {

	if (enemy->extension) {
		MEM_free(enemy->extension);
		enemy->extension = 0;
	}

	SPR_releaseSprite(enemy->sprite);
	releaseEnemy(enemy);
}

static f16 randomVSpeed() {

	int i = random() % 3;
	if (i == 2) {
		return BUBBLE_DEFAULT_SPEED_V;
	}

	if (i) {
		return -BUBBLE_DEFAULT_SPEED_V;
	}

	return SPEED_ZERO;
}
