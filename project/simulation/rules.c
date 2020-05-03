
///////////////// VEGETATION /////////////////////

double vegetationAtEndOfDayRule(double vegetationAtStartOfDay, long nRabbitsAtStartOfDay) {
  double vegetationAtEndOfDay = 1.1*vegetationAtStartOfDay - (0.1 / 100.0)*nRabbitsAtStartOfDay;
  vegetationAtEndOfDay = vegetationAtEndOfDay > 1.0 ? 1.0 : vegetationAtEndOfDay;
  vegetationAtEndOfDay = vegetationAtEndOfDay < 0.1 ? 0.1 : vegetationAtEndOfDay;
  return vegetationAtEndOfDay;
}


///////////////// RABBITS ///////////////////////
bool isRabbitBirthingDay(long timestep) {
  return timestep % (7*9) == 0;
}

long numberOfRabbitsToMigrateAtEndOfDayRule(long nRabbitsAtEndOfDay){
  return nRabbitsAtEndOfDay / 5;
}

/*
Returns expected life span of rabbit in DAYS
*/

#define VEG_L1 1.0
#define VEG_L2 0.3
#define VEG_L3 0.25
#define VEG_L4 0.15
#define VEG_MO 28

long rabbitExpectedLifeSpanRule(double vegetationLevel) {
  if (vegetationLevel < VEG_L4)
    return 3*VEG_MO;
  if (vegetationLevel < VEG_L3)
    return 6*VEG_MO;
  if (vegetationLevel < VEG_L2)
    return 12*VEG_MO;
  if (vegetationLevel < VEG_L1)
    return 18*VEG_MO;
  return 18*VEG_MO;
}

long rabbitLitterSizeRule(double vegetationLevel, long nRabbitsAtStartOfDay){
  if (nRabbitsAtStartOfDay < 2)
    return 0;

  if (vegetationLevel < 0.2) {
    return nRabbitsAtStartOfDay <= 700 ? 3 : 2;
  }
  if (vegetationLevel >= 0.2 && vegetationLevel < 0.5) {
    return nRabbitsAtStartOfDay <= 700 ? 4 : 3;
  }
  if (vegetationLevel >= 0.5 && vegetationLevel < 0.8) {
    if (nRabbitsAtStartOfDay <= 200)
      return 6;
    if (nRabbitsAtStartOfDay <= 700)
      return 5;
    // Case nRabbitsAtStartOfDay > 700
    return 4;
  }
  if (vegetationLevel >= 0.8) {
    if (nRabbitsAtStartOfDay <= 200)
      return 9;
    if (nRabbitsAtStartOfDay <= 700)
      return 8;
    if (nRabbitsAtStartOfDay <= 5000)
      return 7;
    // Case nRabbitsAtStartOfDay > 5000
    return 5;
  }
}


///////////////// FOXES /////////////////////////
bool isFoxBirthingDay(long timestep) {
  return timestep % (28*6) == 0;
}

long foxExpectedLifeSpanRule() {
  return 4*12*28;
}

long foxLitterSizeRule(long nFoxesAtStartOfDay, long nRabbitsAtStartOfDay){
  if (nFoxesAtStartOfDay < 2)
    return 0;

  double rabbitsPerFox = (double)nRabbitsAtStartOfDay / (double)nFoxesAtStartOfDay;

  long litterSize = 0;
  if (rabbitsPerFox < 3) {
    if (nFoxesAtStartOfDay <= 50)
      return 2;
    if (nFoxesAtStartOfDay <= 100)
      return 1;
    if (nFoxesAtStartOfDay > 100)
      return 0;
  }
  if (rabbitsPerFox >= 3 && rabbitsPerFox < 10) {
    if (nFoxesAtStartOfDay <= 50)
      return 3;
    if (nFoxesAtStartOfDay <= 100)
      return 2;
    if (nFoxesAtStartOfDay > 100)
      return 1;
  }
  if (rabbitsPerFox >= 10 && rabbitsPerFox < 40) {
    if (nFoxesAtStartOfDay <= 10)
      return 4;
    if (nFoxesAtStartOfDay <= 100)
      return 3;
    if (nFoxesAtStartOfDay > 100)
      return 2;
  }
  if (rabbitsPerFox >= 40) {
    if (nFoxesAtStartOfDay <= 10)
      return 5;
    if (nFoxesAtStartOfDay <= 50)
      return 4;
    if (nFoxesAtStartOfDay > 50)
      return 3;
  }
  return 0;
}
