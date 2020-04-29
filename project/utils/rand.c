long getRandomInt(long upperBound) {
  return rand() * upperBound / RAND_MAX;
}

double getRandomDouble() {
  return (double)rand() / (double)RAND_MAX;
}
