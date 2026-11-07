#pragma once

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif
#include <dinput.h>
#include <stddef.h>

// VTABLE: SWBF2 0x00772b94
class RedJoystick
{
public:
    // a button's bit in m_uiDownButtons (32 and up are the POV hats)
    static unsigned __int64 ButtonBit(int iButton) { return (unsigned __int64)1 << iButton; }
    // the same, as the game sets the plain buttons: a 32-bit shift, so button 31 also sets all the POV bits
    static int LowButtonBit(int iButton) { return 1 << iButton; }


    RedJoystick();

    virtual ~RedJoystick();

    // SYNTHETIC: SWBF2 0x006c5340
    // RedJoystick::`scalar deleting destructor'

    void Init(unsigned char ucPortNum);
    bool pcCreateDevice(DIDEVICEINSTANCEA const* pInstance);

    void SetDeadZoneScale(float fScale);

    unsigned __int64 GetDownButtons() const { return m_uiDownButtons; }
    float GetAxis(int iAxis) const { return m_fAxis[iAxis]; }

    void Insert();
    void Remove();
    void Cleanup();
    void Update(float fDeltaTime);
    void SetHeavyEngine(float f);
    void SetLightEngine(float f);

    unsigned char m_portNum;
    bool m_connected;
    float m_fAxis[8];
    float m_fAnalogButtons[48];
    int m_iHeavyEngine;
    int m_iLightEngine;
    unsigned __int64 m_uiDownButtons;
    unsigned __int64 m_uiPressedButtons;
    unsigned __int64 m_uiReleasedButtons;
    IDirectInputDevice8A* pc_device;
    DIJOYSTATE2 pc_state;

    float mPreviousAxisX;
    float mPreviousAxisY;

    float mDeadZoneScale;
};
 
 
 
 
 
