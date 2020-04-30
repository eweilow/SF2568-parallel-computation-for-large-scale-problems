///////////////// VEGETATION /////////////////////

double vegetationAtEndOfDayRule(double vegetationAtStartOfDay, long nRabbitsAtStartOfDay) {
  double vegetationAtEndOfDay = 1.1*(vegetationAtStartOfDay - 0.001*nRabbitsAtStartOfDay);
  vegetationAtEndOfDay = vegetationAtEndOfDay > 1.0 ? 1.0 : vegetationAtEndOfDay;
  vegetationAtEndOfDay = vegetationAtEndOfDay < 0.1 ? 0.1 : vegetationAtEndOfDay;
  return vegetationAtEndOfDay;
}


///////////////// RABBITS ///////////////////////
bool isRabbitBirthingDay(long timestep) {
  return timestep % (7*7) == 0;
}

long numberOfRabbitsToMigrateAtEndOfDayRule(long nRabbitsAtEndOfDay){
  return nRabbitsAtEndOfDay / 5;
}

long rabbitExpectedLifeSpanRule(double vegetationLevel) {
  if (vegetationLevel >= 0.35)
    return 18;
  if (vegetationLevel < 0.15)
    return 3;
  if (vegetationLevel < 0.25 & vegetationLevel >= 0.15 )
    return 6;
  if (vegetationLevel < 0.35 & vegetationLevel >= 0.25 )
    return 12;

  return 18; // Default return, shouldn't happen though
}

long rabbitLitterSizeRule(double vegetationLevel, long nRabbitsAtStartOfDay){
  if (nRabbitsAtStartOfDay < 2)
    return 0;

  if (vegetationLevel < 0.2) {
    return nRabbitsAtStartOfDay <= 700 ? 3 : 2;
  }
  if (vegetationLevel >= 0.2 & vegetationLevel < 0.5) {
    return nRabbitsAtStartOfDay <= 700 ? 4 : 3;
  }
  if (vegetationLevel >= 0.5 & vegetationLevel < 0.8) {
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

long foxLitterSizeRule(long nFoxesAtStartOfDay, long nRabbitsAtStartOfDay){
  if (nFoxesAtStartOfDay < 2)
    return 0;

  long litterSize = 0;
  if (nRabbitsAtStartOfDay < 3) {
    if (nFoxesAtStartOfDay <= 50)
      return 2;
    if (nFoxesAtStartOfDay <= 100)
      return 1;
    if (nFoxesAtStartOfDay > 100)
      return 0;
  }
  if (nRabbitsAtStartOfDay >= 3 & nRabbitsAtStartOfDay < 10) {
    if (nFoxesAtStartOfDay <= 50)
      return 3;
    if (nFoxesAtStartOfDay <= 100)
      return 2;
    if (nFoxesAtStartOfDay > 100)
      return 1;
  }
  if (nRabbitsAtStartOfDay >= 10 & nRabbitsAtStartOfDay < 40) {
    if (nFoxesAtStartOfDay <= 10)
      return 4;
    if (nFoxesAtStartOfDay <= 100)
      return 3;
    if (nFoxesAtStartOfDay > 100)
      return 2;
  }
  if (nRabbitsAtStartOfDay >= 40) {
    if (nFoxesAtStartOfDay <= 10)
      return 5;
    if (nFoxesAtStartOfDay <= 50)
      return 4;
    if (nFoxesAtStartOfDay > 50)
      return 3;
  }
}

double foxHuntingSuccessChanceRule(double vegetationLevel, long nFoxesAtStartOfDay, long nRabbitsAtStartOfDay) {
  if (nRabbitsAtStartOfDay < nFoxesAtStartOfDay || vegetationLevel >= 0.6)
    return 2/7;

  return 4/7;
}

bool foxesRiskStarvationRule(long nFoxesAtStartOfDay, long nRabbitsAtStartOfDay){
  if (nRabbitsAtStartOfDay < nFoxesAtStartOfDay)
    return true;

  return false;
}
