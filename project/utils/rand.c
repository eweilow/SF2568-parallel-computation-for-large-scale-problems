long getRandomInt(long upperBound) {
  return rand() * upperBound / RAND_MAX;
}

double getRandomDouble() {
  return (double)rand() / (double)RAND_MAX;
}

double getSigmoidProbability(double currentValue, double center, double width) {
  return exp((currentValue-center)/width) / (exp((currentValue-center)/width) + 1.0);
}