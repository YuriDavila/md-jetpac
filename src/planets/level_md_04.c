/*
 * level_md_04.c
 *
 *  Created on: May 24, 2019
 *      Author: diegomtassis
 */

#include "../../inc/planets.h"

#include <genesis.h>

#include "../../inc/fwk/commons.h"
#include "../../inc/level.h"
#include "../../inc/spaceship.h"

static void createPlatforms(Level* level);
static void defineEnemies(Enemies* enemies);
static void defineJetman(Level* level);
static void defineSpaceship(Level* level);

Level* createLevelMD04() {

	Level* level = allocLevel();

	createPlatforms(level);
	defineEnemies(&level->enemies);
	defineSpaceship(level);
	defineJetman(level);

	level->def.mind_bottom = TRUE;

	return level;
}

static void createPlatforms(Level* level) {

	level->floor = createPlatform(0, 25, 32);

	level->num_platforms = 18;
	level->platforms = MEM_alloc(level->num_platforms * sizeof(Platform*));

	level->platforms[0] = createPlatform(2, 22, 4);
	level->platforms[1] = createPlatform(2, 19, 4);
	level->platforms[2] = createPlatform(2, 16, 4);
	level->platforms[3] = createPlatform(2, 13, 4);

	level->platforms[4] = createPlatform(10, 22, 4);
	level->platforms[5] = createPlatform(10, 19, 4);
	level->platforms[6] = createPlatform(10, 16, 4);
	level->platforms[7] = createPlatform(10, 13, 4);
	level->platforms[16] = createPlatform(10, 10, 4);

	level->platforms[8] = createPlatform(18, 22, 4);
	level->platforms[9] = createPlatform(18, 19, 4);
	level->platforms[10] = createPlatform(18, 16, 4);
	level->platforms[11] = createPlatform(18, 13, 4);
	level->platforms[17] = createPlatform(18, 10, 4);

	level->platforms[12] = createPlatform(26, 22, 4);
	level->platforms[13] = createPlatform(26, 19, 4);
	level->platforms[14] = createPlatform(26, 16, 4);
	level->platforms[15] = createPlatform(26, 13, 4);
}

static void defineSpaceship(Level* level) {

	level->def.spaceship_def.type = U1;
	level->def.spaceship_def.init_step = UNASSEMBLED;
	setV2s16(&level->def.spaceship_def.base_pos, 120, 184);
	setV2s16(&level->def.spaceship_def.middle_pos, 56, 184);
	setV2s16(&level->def.spaceship_def.top_pos, 184, 184);
}

static void defineJetman(Level* level) {

	level->def.jetman_init_pos = MEM_alloc(sizeof *level->def.jetman_init_pos);
	setV2s16(level->def.jetman_init_pos, 92, 30);
}

static void defineEnemies(Enemies* enemies) {

	enemies->current_num_enemies = 0;
	enemies->max_num_enemies = 12;
}
