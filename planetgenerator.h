#ifndef PLANETGENERATOR_H
#define PLANETGENERATOR_H

#include <cstdint>
#include <vector>

std::pair<std::vector<float>, std::vector<uint32_t>>
generateSubdividedCube(const uint32_t numberOfSubdivisions);

#endif // PLANETGENERATOR_H
