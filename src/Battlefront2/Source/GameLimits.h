#pragma once

// Battlefront2: limits shared by classes whose headers do not see each other.

namespace GameLimits
{
// the most characters (players and AI units) a mission has: SpawnManager clamps the count to it, and the per-character
// arrays and bit sets (spotters, active regions, bonuses, dispenser tracking, scoreboard sorting) have this many entries
const int kMaxCharacters = 600;
} // namespace GameLimits
