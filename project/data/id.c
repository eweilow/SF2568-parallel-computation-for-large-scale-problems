
u_int64_t processLocalCounter = 0;

const u_int64_t baseIDLength = 24; // 2^baseIDLength baseIds are allowed
const u_int64_t NUMLEN = 64;
// With baseIDLength = 24, we can have 4096*4096 total tiles before ID collisions are a problem
// This means that we can have 2^(64 - 24) = 2^40 ~= 1000 billion IDs per tile...

// Deterministic-ish IDs
// Each baseId can generate at least 2^31 IDs before colliding with another process
// The idea is that baseId is given a global index, shared by all processes. This is for example the id of each tile.
// Since tiles never change process ownership, the local counter makes sure that we never generate two equal IDs for a given baseId.
// This all holds as long as we generate less than 2^31 IDs.
u_int64_t getNextId(u_int64_t baseId) {

  u_int64_t processPart = baseId << (NUMLEN - baseIDLength);
  u_int64_t localPart = (processLocalCounter << (NUMLEN - baseIDLength)) >> (NUMLEN - baseIDLength);

  ++processLocalCounter;

  return processPart | localPart; //>>shift + localPart;
}

void formatGlobalId(
  u_int64_t id,
  char* buffer
) {
  u_int64_t processPart = id >> (NUMLEN - baseIDLength);
  u_int64_t localPart = (id << (NUMLEN - baseIDLength)) >> (NUMLEN - baseIDLength);

  sprintf(buffer, "t%llu:%llu", processPart, localPart);
}