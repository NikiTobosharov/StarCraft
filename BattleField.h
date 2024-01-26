#ifndef BATTLEFIELD_H_
#define BATTLEFIELD_H_

#include <stdbool.h>

#include "Vector.h"
#include "Defines.h"

struct TerranArmy;   
struct ProtossArmy;  

typedef struct 
{
  Vector terranFleet;
  Vector protossFleet;
} BattleField;

typedef struct 
{
  enum AirShipDefines type;
  int health;
  int damage;
} Airship;

typedef struct TerranArmy
{
  Airship terranShip;
  void (*attack)(struct TerranArmy *,struct ProtossArmy *);
} TerranArmy;

typedef struct ProtossArmy
{
  Airship protossShip;
  int shield;
  int shieldRegenRate;
  void (*attack)(struct ProtossArmy *,struct TerranArmy *);
} ProtossArmy;


void generateTerranFleet(BattleField *battleField, const char *terranFleetStr);
void generateProtossFleet(BattleField *battleField, const char *protossFleetStr);

void startBattle(BattleField *battleField);
bool isDead(Airship *airship);

void deinit(BattleField *battleField);

bool processTerranTurn(BattleField *battleField);
bool processProtossTurn(BattleField *battleField);

void terranAttack(TerranArmy *attacker, ProtossArmy *target);
void protossAttack(ProtossArmy *attacker, TerranArmy *target);

#endif /* BATTLEFIELD_H_ */
