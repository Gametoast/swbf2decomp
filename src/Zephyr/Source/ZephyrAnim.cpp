#include "ZephyrAnim.h"
#include "ZephyrAnimJoint.h"
#include "PebbleFL/Common/PblMath.h"

// a joint's bit in u64TransFlags: a 32-bit shift, as the game has it
static inline int JointBit(int iJoint) { return 1 << iJoint; }

// FUNCTION: SWBF2 0x0072f220
bool ZephyrAnim::Open(const ZephyrAnimD* pkAnimD, const ZephyrAnimJointD* pkJointD, ZephyrAnimJoint* pkJoints, char* pcData)
{
    m_kCrcName = pkAnimD->u32CrcName;
    m_u16NumFrames = pkAnimD->u16NumFrames;
    m_u16NumJoints = pkAnimD->u16NumJoints;
    m_pkJoints = pkJoints;

    for (int i = 0; i < m_u16NumJoints; i++)
    {
        if (!m_pkJoints[i].Open(this, pkJointD, pcData, (pkAnimD->u64TransFlags & JointBit(i)) != 0))
        {
            return false;
        }
        ++pkJointD;
    }
    return true;
}
