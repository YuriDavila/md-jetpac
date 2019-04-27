/*
 * game.c
 *
 *  Created on: Apr 20, 2019
 *      Author: diegomtassis
 */

#include "../inc/game.h"

#include <genesis.h>

#include "../inc/elements.h"
#include "../inc/enemies.h"
#include "../inc/jetman.h"
#include "../inc/levels.h"

static Level* current_level;

static void handleInput();

void startGame() {

	current_level = createLevel();
	startLevel(current_level);

	startJetman(current_level);
	startEnemies(current_level);

	while (1) {

		handleInput();

		handleJetman(current_level);
		handleEnemies(current_level);

		SPR_update();
		VDP_waitVSync();
	}

	return;
}

static void handleInput() {

}