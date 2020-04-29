///////////////// VEGETATION /////////////////////

// vegetationAtEndOfDayRule()
// - in: double vegetationLevelAtStartOfDayOfTile
//       long number of rabbits at startOfNewDay
// - out: long vegetationAtEndOfDay
double vegetationAtEndOfDayRule(double vegetationAtStartOfDay, long nRabbitsAtStartOfDay) {
  return 1.10*vegetationAtStartOfDay - 0.001*(double)nRabbitsAtStartOfDay;
}


///////////////// RABBITS ///////////////////////




///////////////// FOXES /////////////////////////
