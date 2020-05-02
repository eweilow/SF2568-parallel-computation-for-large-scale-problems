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
  double W = (double)sqrtf((float)center*width);
  return exp((currentValue-center)/W) / (exp((currentValue-center)/W) + 1.0)/W;
}