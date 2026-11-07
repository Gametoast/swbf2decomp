#include <string.h>
#include "PebbleFL/Common/PblMath.h"

#include "pcRedInputManager.h"
#include "RedEngineFL/Application/pc/pcFLRender.h"
#include "RedEngineFL/Interface/RedInterfaceScreen.h"

// GLOBAL: SWBF2 0x00787484
// GUID_SysMouse
// GLOBAL: SWBF2 0x00787bc4
// c_dfDIMouse2

extern HWND hGlobalWnd;

// FUNCTION: SWBF2 0x006da310
bool RedMouse::Exists()
{
    return pc_device != 0;
}

// FUNCTION: SWBF2 0x006da4b0
void RedMouse::Remove()
{
    pc_device->Unacquire();
    pc_device->SetCooperativeLevel(hGlobalWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
    m_connected = false;
}

// FUNCTION: SWBF2 0x006da320
void RedMouse::Init(unsigned char ucPortNum)
{
    pc_device = 0;
    m_portNum = ucPortNum;
    m_connected = false;
    pCursorImage = 0;

    POINT point;
    GetCursorPos(&point);
    m_X = point.x;
    m_Y = point.y;
    m_Z = 0;
    m_dZ = 0;
    m_dY = 0;
    m_dX = 0;
    m_uiDownButtons = 0;
    m_uiPressedButtons = 0;
    m_uiReleasedButtons = 0;

    IDirectInput8A* pDI = RedInputManager::pcGetDIInterface();

    if (pc_device)
    {
        return;
    }

    if (pDI->CreateDevice(GUID_SysMouse, &pc_device, 0) != DI_OK)
    {
        return;
    }

    HRESULT hr = pc_device->SetDataFormat(&c_dfDIMouse2);

    DIPROPRANGE diprg;
    diprg.diph.dwHow = DIPH_DEVICE;
    diprg.diph.dwObj = 0;
    diprg.diph.dwSize = sizeof(DIPROPRANGE);
    diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    diprg.lMin = (LONG)0x80000000;
    diprg.lMax = 0x7FFFFFFF;
    pc_device->GetProperty(DIPROP_RANGE, &diprg.diph);
    pc_device->SetProperty(DIPROP_RANGE, &diprg.diph);

    if (SUCCEEDED(hr))
    {
        hr = pc_device->SetCooperativeLevel(hGlobalWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
    }
    if (FAILED(hr))
    {
        pc_device->Release();
        pc_device = 0;
    }
}

// FUNCTION: SWBF2 0x006da450
void RedMouse::Insert()
{
    if (m_connected)
    {
        return;
    }

    memset(&pc_state, 0, sizeof(pc_state));

    pc_device->Unacquire();
    pc_device->SetCooperativeLevel(hGlobalWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);

    HRESULT hr = pc_device->Acquire();
    while (hr == DIERR_INPUTLOST)
    {
        hr = pc_device->Acquire();
    }

    m_connected = (hr == DI_OK);
}

// FUNCTION: SWBF2 0x006da290
RedMouse::RedMouse()
{
}

// FUNCTION: SWBF2 0x006da2a0
RedMouse::~RedMouse()
{
}

// FUNCTION: SWBF2 0x006da4e0
void RedMouse::SetCursorPos(int iX, int iY)
{
    m_X = iX;
    m_Y = iY;

    m_X = Clamp(iX, 0, (int)RedInterfaceScreen::GetScreenWidth());
    m_Y = Clamp(iY, 0, (int)RedInterfaceScreen::GetScreenHeight());
    FLRenderer::SetCursorPos((float)m_X, (float)m_Y);

    ::SetCursorPos(iX, iY);
}

// FUNCTION: SWBF2 0x006da2e0
void RedMouse::Cleanup()
{
    if (pc_device)
    {
        pc_device->Unacquire();
        pc_device->Release();
        pc_device = 0;
    }
    m_connected = false;
}

// FUNCTION: SWBF2 0x006da560
void RedMouse::Update(float fDeltaTime)
{
    if (!pc_device)
    {
        return;
    }

    HRESULT hr = pc_device->Poll();
    if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
    {
        Remove();
        Insert();
        if (FAILED(pc_device->Poll()))
        {
            return;
        }
    }

    if (FAILED(pc_device->GetDeviceState(sizeof(DIMOUSESTATE2), &pc_state)))
    {
        return;
    }

    m_dX = pc_state.lX;
    m_dY = pc_state.lY;
    m_dZ = pc_state.lZ;
    if (abs(m_dX) >= 3 || abs(m_dY) >= 3)
    {
        pCursorImage = 0;
    }

    m_X += m_dX;
    m_Y += m_dY;
    m_Z += m_dZ;

    m_X = Clamp(m_X, 0, (int)RedInterfaceScreen::GetScreenWidth());
    m_Y = Clamp(m_Y, 0, (int)RedInterfaceScreen::GetScreenHeight());
    FLRenderer::SetCursorPos((float)m_X, (float)m_Y);

    const unsigned int uiPrevDown = m_uiDownButtons;
    m_uiDownButtons = 0;
    for (int i = 0; i < 8; i++)
    {
        if (pc_state.rgbButtons[i] & 0x80)
        {
            m_uiDownButtons |= Bit(i);
        }
    }
    m_uiPressedButtons = m_uiDownButtons & (m_uiDownButtons ^ uiPrevDown);
    m_uiReleasedButtons = uiPrevDown & (m_uiDownButtons ^ uiPrevDown);
}
