#include <stdio.h>
#include <stdlib.h>

#include "BattleField.h"

void generateTerranFleet(BattleField *battleField, const char *terranFleetStr) 
{
  int terranFleetStrLength = 0;
  const char *timer = terranFleetStr;

  while ('\0' != *timer) 
  {
    terranFleetStrLength++;
    timer++;
  }

vectorInit(&battleField->terranFleet, terranFleetStrLength);
   
 while ('\0' != *terranFleetStr)
 {
    TerranArmy *newTerranShip = malloc(sizeof(TerranArmy));
    if (NULL == newTerranShip) 
    {
      printf("Memory allocation failed. Exiting the program.\n");
      exit(1);
    }

    if ('b' == *terranFleetStr)
    {
      newTerranShip->attack = &terranAttack;
      newTerranShip->terranShip.type = BATTLE_CRUSER;
      newTerranShip->terranShip.health = BATTLE_CRUSER_HEALTH;
      newTerranShip->terranShip.damage = BATTLE_CRUSER_DAMAGE;
      vectorPush(&battleField->terranFleet, newTerranShip);
    }
    else if('v' == *terranFleetStr)
    {
      newTerranShip->attack = &terranAttack;
      newTerranShip->terranShip.type = VIKING;
      newTerranShip->terranShip.health = VIKING_HEALTH;
      newTerranShip->terranShip.damage = VIKING_DAMAGE;
      vectorPush(&battleField->terranFleet, newTerranShip);
    }
    else
    {
      printf("You entered an unexisting Terran ally. Exiting the program.\n");
      exit(1);
    }

    terranFleetStr++;
 }
}


void generateProtossFleet(BattleField *battleField, const char *protossFleetStr) 
{
  int protossFleetStrLength = 0;
  const char *timer = protossFleetStr;

  while ('\0' != *timer) 
  {
    protossFleetStrLength++;
    timer++;
  }

  vectorInit(&battleField->protossFleet, protossFleetStrLength);

  while ('\0' != *protossFleetStr)
  {
    ProtossArmy *newProtossShip = malloc(sizeof(ProtossArmy));
    
    if (NULL == newProtossShip) 
    {
      printf("Memory allocation failed for Protoss ship. Exiting the program.\n");
      exit(1);
    }

    if ('c' == *protossFleetStr)
    {
      newProtossShip->attack = &protossAttack;
      newProtossShip->protossShip.type = CARRIER;
      newProtossShip->protossShip.health = CARRIER_HEALTH;
      newProtossShip->protossShip.damage = CARRIER_DAMAGE;
      newProtossShip->shield = CARRIER_SHIELD;
      newProtossShip->shieldRegenRate = CARRIER_SHIELD_REGENERATE_RATE;
      vectorPush(&battleField->protossFleet, newProtossShip);
    }
    else if ('p' == *protossFleetStr)
    {
      newProtossShip->attack = &protossAttack;
      newProtossShip->protossShip.type = PHOENIX;
      newProtossShip->protossShip.health = PHOENIX_HEALTH;
      newProtossShip->protossShip.damage = PHOENIX_DAMAGE;
      newProtossShip->shield = PHOENIX_SHIELD;
      newProtossShip->shieldRegenRate = PHOENIX_SHIELD_REGENERATE_RATE;
      vectorPush(&battleField->protossFleet, newProtossShip);
    }
    else
    {
      printf("You entered an unexisting Protoss ally. Exiting the program.\n");
      exit(1);
    }

    protossFleetStr++;
  }
}


void startBattle(BattleField *battleField) 
{
  while (true) 
  {
    if (processTerranTurn(battleField)) 
    {
      printf("TERRAN has won!\n");
      break;
    }

    if (processProtossTurn(battleField)) 
    {
      printf("PROTOSS has won!\n");
      break;
    }
  }
}


bool isDead(Airship *airship)
{
  return airship->health <= 0;
}


void deinit(BattleField *battleField) 
{
  
  for (int i = 0; i < vectorGetSize(&battleField->protossFleet); i++)
  {
    ProtossArmy *protossShip = vectorGet(&battleField->protossFleet, i);
    free(protossShip);
  }

  for (int i = 0; i < vectorGetSize(&battleField->terranFleet); i++)
  { 
    TerranArmy *terranShip = vectorGet(&battleField->terranFleet, i);
    free(terranShip);
  }

  vectorFree(&battleField->protossFleet);
  vectorFree(&battleField->terranFleet);
}


void terranAttack(struct TerranArmy *attacker,struct ProtossArmy *target)
{
  
  target->shield -= attacker->terranShip.damage;

  if (0 > target->shield)
  {
    target->protossShip.health += target->shield;
    target->shield = 0;
  }
}


void protossAttack(struct ProtossArmy *attacker, struct TerranArmy *target)
{
  target->terranShip.health -= attacker->protossShip.damage;
}


bool processTerranTurn(BattleField *battleField) 
{
  int defenderCount = vectorGetSize(&battleField->protossFleet);
  int lastIndex = defenderCount - 1;
  static int turnCounter = 1;
  
  for (int i = 0; i < vectorGetSize(&battleField->terranFleet); i++)
  {
    TerranArmy *terranship = vectorGet(&battleField->terranFleet, i);
    
    if (!vectorIsEmpty(&battleField->protossFleet))
    {
      ProtossArmy *protossShip = vectorGet(&battleField->protossFleet, lastIndex);

       if (turnCounter % 5 == 0 && BATTLE_CRUSER == terranship->terranShip.type)
      {
        terranship->terranShip.damage *= 5;
      }

      if (VIKING == terranship->terranShip.type && PHOENIX == protossShip->protossShip.type)
      {
        terranship->terranShip.damage *= 2;
      }
  
      terranship->attack(terranship, protossShip);
      
      if (BATTLE_CRUSER == terranship->terranShip.type)
      {
        terranship->terranShip.damage= BATTLE_CRUSER_DAMAGE;
      }
        else if (VIKING == terranship->terranShip.type)
      {
        terranship->terranShip.damage = VIKING_DAMAGE;
      }
      

      if (isDead(&protossShip->protossShip))
      {
        enum AirShipType currentUnitType = terranship->terranShip.type;

        if (BATTLE_CRUSER == currentUnitType)
        {
          printf("BattleCruser with ID: %d killed enemy airship with ID: %d\n",
          i,
          lastIndex);
        }
        else if (VIKING == currentUnitType)
        {
          printf("Viking with ID: %d killed enemy airship with ID: %d\n",
          i,
          lastIndex);
        }

        free(protossShip);
        vectorPop(&battleField->protossFleet);       
        lastIndex--;
      }
    }
  }


  if (vectorIsEmpty(&battleField->protossFleet))
  {
    return true;
  }

  ProtossArmy *lastProtossShip = vectorGet(&battleField->protossFleet, lastIndex);
  printf("Last Protoss AirShip with ID: %d has %d health and %d shield left\n",
  lastIndex,
  lastProtossShip->protossShip.health,
  lastProtossShip->shield);

  turnCounter++;
  return false;
}


bool processProtossTurn(BattleField *battleField) 
{
  int defenderCount = vectorGetSize(&battleField->terranFleet);
  int lastIndex = defenderCount - 1;

  for (int i = 0; i < vectorGetSize(&battleField->protossFleet); i++)
  {
    ProtossArmy *protossShip = vectorGet(&battleField->protossFleet, i);

    if (!vectorIsEmpty(&battleField->protossFleet))
    {
      TerranArmy *terranShip = vectorGet(&battleField->terranFleet, lastIndex);
      
      if (CARRIER == protossShip->protossShip.type)
      {
        unsigned char numInterceptors = 0;

        if (CARRIER_HEALTH == protossShip->protossShip.health)
        {
          numInterceptors = 8;
        }
        else if (CARRIER_HEALTH > protossShip->protossShip.health)
        {
          numInterceptors = 4;
        }
        
        while (0 < numInterceptors)
        {
          protossShip->attack(protossShip, terranShip);

          if (isDead(&terranShip->terranShip))
          {
            printf("Carrier with ID: %d killed enemy airship with ID: %d\n",
            i,
            lastIndex);
            
            free(terranShip);
            vectorPop(&battleField->terranFleet);
            lastIndex--;

            if (0 <= lastIndex)
            {
              terranShip = vectorGet(&battleField->terranFleet, lastIndex);
            }
            else
            {
              break;
            }
          }
          
          numInterceptors--;
        }

        if (CARRIER_SHIELD < protossShip->shield + protossShip->shieldRegenRate)
        {
          protossShip->shield = CARRIER_SHIELD;
        }
        else
        {
          protossShip->shield += protossShip->shieldRegenRate;
        }
      }
 
      if (PHOENIX == protossShip->protossShip.type)
      {
        protossShip->attack(protossShip, terranShip);

        if (isDead(&terranShip->terranShip))
        {
          printf("Phoenix with ID: %d killed enemy airship with ID: %d\n",
          i,
          lastIndex);
          
          free(terranShip);
          vectorPop(&battleField->terranFleet);
          lastIndex--;
        }

        if (PHOENIX_SHIELD < protossShip->shield + protossShip->shieldRegenRate)
        {
          protossShip->shield = PHOENIX_SHIELD;
        }
        else
        {
          protossShip->shield += protossShip->shieldRegenRate;
        }
      }
            
    }
  }
  

  if (vectorIsEmpty(&battleField->terranFleet))
  {
    return true;
  }
  
  TerranArmy *lastTerranShip = vectorGet(&battleField->terranFleet, lastIndex);
  printf("Last Terran AirShip with ID: %d has %d health left\n",
         lastIndex,
         lastTerranShip->terranShip.health);

  return false;
}