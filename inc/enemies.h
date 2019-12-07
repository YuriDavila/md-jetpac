/*
 * enemies.h
 *
 *  Created on: Apr 24, 2019
 *      Author: diegomtassis
 */

#ifndef INC_ENEMIES_H_
#define INC_ENEMIES_H_

#include "elements.h"

#define METEORITE	1
#define ALIEN		2
#define BUBBLE		3
#define FIGHTER		4
#define SAUCER		5
#define CROSS		6
#define FALCON		7
#define OWL			8

void startEnemies(Planet planet[static 1]);
void releaseEnemies(Planet planet[static 1]);

void enemiesAct(Planet planet[static 1]);
void killEnemy(Enemy* enemy, Planet planet[static 1], u8 exploding);

extern const EnemyDefinition meteoriteDefinition;
extern const EnemyDefinition alienDefinition;
extern const EnemyDefinition bubbleDefinition;
extern const EnemyDefinition fighterDefinition;
extern const EnemyDefinition saucerDefinition;
extern const EnemyDefinition falconDefinition;
extern const EnemyDefinition crossDefinition;
extern const EnemyDefinition owlDefinition;

#endif /* INC_ENEMIES_H_ */
