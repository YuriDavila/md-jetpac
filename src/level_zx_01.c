/*
 * level_zx_01.c
 *
 *  Created on: May 2, 2019
 *      Author: diegomtassis
 */

#include "../inc/level_zx_01.h"

#include <genesis.h>

#include "../inc/elements.h"
#include "../inc/levels.h"

static void createPlatforms(Level* level);
static void defineEnemies(Enemies* enemies);
static void defineSpaceship(Level* level);

Level* createLevelZX01() {

	Level* level = allocLevel();

	createPlatforms(level);
	defineEnemies(&level->enemies);
	defineSpaceship(level);

	level->def.check_bottom = FALSE;

	return level;
}

static void createPlatforms(Level* level) {

	level->floor = createPlatform(0, 25, 32);

	level->num_platforms = 3;
	level->platforms = MEM_alloc(level->num_platforms * sizeof(Platform*));

	level->platforms[0] = createPlatform(4, 11, 6);
	level->platforms[1] = createPlatform(15, 14, 4);
	level->platforms[2] = createPlatform(24, 8, 6);
}

static void defineEnemies(Enemies* enemies) {

	enemies->current_num_enemies = 0;
	enemies->max_num_enemies = 10;
}

static void defineSpaceship(Level* level) {

	level->def.spaceshipDef = MEM_alloc(sizeof(SpaceshipDefinition));
	level->def.spaceshipDef->type = U1;
	level->def.spaceshipDef->init_step = 0;
}