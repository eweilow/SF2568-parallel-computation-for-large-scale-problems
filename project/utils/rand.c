long getRandomInt(long upperBound) {
  return rand() * upperBound / RAND_MAX;
}

double drawU01() {
  return (double) rand() / (double) RAND_MAX;
}

void getRandomSigmoidDraw(double uDraw, double sigmoidDraw, long center, long width) {
  uDraw = rand();
  sigmoidDraw = exp((uDraw-center)/width) / (exp((uDraw-center)/width) + 1.0);
}
