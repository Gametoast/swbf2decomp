#pragma once

// The one place where an index becomes its bit. Callers name the set by their argument: Bit(iTeam), Bit(iAward), ...
// The int, unsigned and 64-bit forms all exist because the game's code mixes them, and the compiled code differs where the
// result is widened or compared.
inline int Bit(int iIndex) { return 1 << iIndex; }
inline unsigned int BitU(unsigned int uiIndex) { return 1u << uiIndex; }
inline unsigned __int64 Bit64(int iIndex) { return (unsigned __int64)1 << iIndex; }

// a power of two (a size, not a bit)
inline int PowerOf2(int iLog2) { return Bit(iLog2); }
inline unsigned int PowerOf2U(unsigned int uiLog2) { return BitU(uiLog2); }

// a mask of the lowest bits
inline int LowBitsMask(int iBits) { return Bit(iBits) - 1; }
inline unsigned int LowBitsMaskU(unsigned int uiBits) { return BitU(uiBits) - 1; }
