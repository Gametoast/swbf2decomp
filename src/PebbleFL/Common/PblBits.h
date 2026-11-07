#pragma once

// Shifts with a name. Kinds of bit that belong to a class stay with it (Team::Bit, CollisionMask::BodyBit, ...).

// a power of two
inline int PowerOf2(int iLog2) { return 1 << iLog2; }
inline unsigned int PowerOf2U(unsigned int uiLog2) { return 1u << uiLog2; }

// a mask of the lowest bits
inline int LowBitsMask(int iBits) { return (1 << iBits) - 1; }
inline unsigned int LowBitsMaskU(unsigned int uiBits) { return (1u << uiBits) - 1; }

// a bit within a byte of a bit array
inline int ByteBit(unsigned int uiBit) { return 1 << uiBit; }

// a view's (or camera's) bit in a per-view mask
inline int ViewBit(unsigned int uiView) { return 1 << uiView; }
