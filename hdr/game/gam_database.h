#pragma once

#include "system/sys_main.h"

typedef struct {
	int         maxHealth;
	float       maxSpeed;
	float       accelerate;
	int         score;
	int         bounceDamage;
	bool        canShoot;
	int         bulletType;
	int         chanceToShoot;
	int         bulletDamage;
	bool        disrupterImmune;
	float       rechargeTime;
	int         tokenCount;
	int         imageIndex;
	std::string dbImageFileName;
	std::string height;
	std::string weight;

	std::string description;
	std::string className;
	std::string drive;
	std::string brain;
	std::string weapon;
	std::string sensor1;
	std::string sensor2;
	std::string sensor3;
	std::string notes;
} _dataBaseEntry;

extern std::vector<_dataBaseEntry> dataBaseEntry;

// Get and build up information for droid database files
bool gam_getDBInformation ();
