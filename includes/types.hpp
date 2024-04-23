#pragma once

#include <utility>

using Position = std::pair<const int, const int>;
using PositionMutable = std::pair<int, int>;

// Renvoie une position invalide
Position emptyPosition();
