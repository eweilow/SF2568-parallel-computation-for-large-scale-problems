///////////////// VEGETATION /////////////////////

// vegetationAtEndOfDayRule()
// - in: double vegetationLevelAtStartOfDayOfTile
//       long number of rabbits at startOfNewDay
// - out: long vegetationAtEndOfDay
double vegetationAtEndOfDayRule(double vegetationAtStartOfDay, long nRabbitsAtStartOfDay) {
  double vegetationAtEndOfDay = 1.1*(vegetationStart - 0.001*rabbitCountStart);
  vegetationAtEndOfDay = vegetationAtEndOfDay > 1.0 ? 1.0 : vegetationAtEndOfDay;
  vegetationAtEndOfDay = vegetationAtEndOfDay < 0.1 ? 0.1 : vegetationAtEndOfDay;
  return vegetationAtEndOfDay;
}

///////////////// RABBITS ///////////////////////
long numberOfRabbitsToMigrateAtEndOfDayRule(long numberOfRabbitsAtEndOfDay){
  return numberOfRabbitsToMigrate / 5;
}



///////////////// FOXES /////////////////////////
