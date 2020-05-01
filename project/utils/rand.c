// Returns in the interval [0, upperBound)
long getRandomInt(long upperBound) {
  return rand() * upperBound / RAND_MAX;
}

// Returns in the interval [0, 1]
double getRandomDouble() {
  return (double)rand() / (double)RAND_MAX;
}

// Returns in the interval (0, 1)
double getSigmoidProbability(double currentValue, double center, double width) {
  return exp((currentValue-center)/width) / (exp((currentValue-center)/width) + 1.0);
}