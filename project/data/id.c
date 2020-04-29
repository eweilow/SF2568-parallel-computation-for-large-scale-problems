
long processLocalCounter = 0;

// Deterministic-ish IDs
// Each baseId can generate at least 2^30 IDs before colliding with another process
long getNextId(long baseId) {
  long processPart = (long)((int32_t)(baseId)) << 32;
  long localPart = (long)((int32_t)(processLocalCounter));

  ++processLocalCounter;

  return processPart + localPart;
}