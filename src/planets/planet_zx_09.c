/*
 * planet_zx_09.c
 *
 *  Created on: Dec 06, 2019
 *      Author: diegomtassis
 */

#include <genesis.h>

#include "../../inc/elements.h"
#include "../../inc/enemies.h"
#include "../../inc/fwk/commons.h"
#include "../../inc/planet.h"
#include "../../inc/spaceship.h"
#include "../../inc/planet.h"
#include "../../res/sprite.h"

Planet* createPlanetZX09() {

	Planet* planet = allocPlanet();

	createDefaultPlatforms(planet);
	defineEnemiesPopulation(planet, meteoriteDefinition, 5);
	defineSpaceshipInDefaultPlanet(planet, u3Definition, UNASSEMBLED);

	planet->def.jetman_init_pos = 0;
	planet->def.ammo = 50;

	planet->def.mind_bottom = FALSE;

	return planet;
}