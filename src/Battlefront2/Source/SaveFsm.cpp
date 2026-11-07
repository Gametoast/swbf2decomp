#include <stdarg.h>

#include "SaveFsm.h"
#include "PebbleFL/Common/PblJournal.h"
#include "PebbleFL/Common/PblUnicode.h"
#include "RedEngineFL/Interface/RedInterfaceElement.h"
#include "RedEngineFL/Interface/RedTextElement.h"
#include "LuaHelper.h"
#include "PebbleFL/Common/PblHashTable.h"

extern unsigned int CalcCRC(void const* pData, unsigned int uiSize, unsigned int uiSeed);
namespace RedLocalizeDB
{
const unsigned short* Find(unsigned int uiHash, bool bNullIfMissing);
}

// FUNCTION: SWBF2 0x00631550
static int SafeLocalizeSPrintf(unsigned short* dst, unsigned int aDstLen, const char* fmt, ...)
{
    const unsigned short* lfmt = RedLocalizeDB::Find(PblHash(fmt), false);
    if (!lfmt)
    {
        PblUnicode::ustrncnv(dst, fmt, aDstLen);
        return 0;
    }

    va_list args;
    va_start(args, fmt);
    int nwritten = PblUnicode::uvsnprintf(dst, unsigned int(-1), lfmt, args);
    va_end(args);
    return nwritten;
}
unsigned short* SafeLocalizeStrcpy(unsigned short* pusDest, char const* pcSrc, unsigned int uiSize);

namespace RedLocalizeDB
{
const unsigned short* Find(unsigned int uiHash, bool bNullIfMissing);
}

// FUNCTION: SWBF2 0x00631600
unsigned int Hash16(unsigned short const* pusString)
{
    if (!pusString || !*pusString)
    {
        return 0;
    }
    unsigned int uHash = 0x811C9DC5u;
    while (*pusString)
    {
        uHash ^= *pusString++;
        uHash *= 0x01000193u;
    }
    return uHash;
}

// FUNCTION: SWBF2 0x00631630
static SaveFile* GetNextSaveFile(SaveDevice& device)
{
    return device.GetNextFile();
}

// FUNCTION: SWBF2 0x00631640
static void JournalSaveFile(SaveFile*& saveFile)
{
}

// FUNCTION: SWBF2 0x00631650
SaveFsm::SaveFsm()
{
    mCurState = STATE_START;
    mNextState = STATE_Idle;
    mCurSaveFile = 0;
    mLoadQueueCnt = 0;
    mXBOXTotalFreeNeeded = -1;
    mXBOXAdditionalFreeNeeded = -1;
}

// SYNTHETIC: SWBF2 0x006316f0
// SaveFsm::`scalar deleting destructor'

// FUNCTION: SWBF2 0x00631720
bool SaveFsm::IsDone() const
{
    int iState = mCurState;
    return iState == STATE_START || iState == STATE_GLOBDONE || iState == STATE_Idle || iState == STATE_Success || iState == STATE_Failure;
}

// FUNCTION: SWBF2 0x00631760
void SaveFsm::StartInitialMemcardCheck()
{
    SetNextState(STATE_InitialMemcardCheck);
}

// FUNCTION: SWBF2 0x00631770
void SaveFsm::StartPreOp(bool bAllowFormat, unsigned int uiCheckFreeBytes,
                          unsigned int uiCheckFreeFiles, unsigned short const* pusFailOpStr,
                          bool bSilentFail, bool bNoFileList, bool bUseIMCMsg, bool bIsPreLoad)
{
    m_preop_AllowFormat = bAllowFormat;
    m_preop_CheckFreeBytes = uiCheckFreeBytes;
    m_preop_CheckFreeFiles = uiCheckFreeFiles;
    m_preop_SilentFail = bSilentFail;
    m_preop_NoFileList = bNoFileList;
    m_preop_UseIMCMsg = bUseIMCMsg;
    m_preop_bIsPreLoad = bIsPreLoad;
    PblUnicode::ustrlcpy((unsigned short*)mFailOpStr, pusFailOpStr, 64);
    SetNextState(STATE_PreOp);
}

// FUNCTION: SWBF2 0x00631820
void SaveFsm::EmptyQueue()
{
    mLoadQueueCnt = 0;
}

// FUNCTION: SWBF2 0x00631830
void SaveFsm::QueueFile(unsigned short const* pusFilename, void* pDest, unsigned int uiSize,
                         int iType, SaveFsmQueueOp eOp)
{
    if (mLoadQueueCnt < MAX_LOAD_QUEUE)
    {
        PblUnicode::ustrlcpy((unsigned short*)mLoadFilename[mLoadQueueCnt], pusFilename, 66);
        mLoadDest[mLoadQueueCnt] = pDest;
        mLoadSize[mLoadQueueCnt] = uiSize;
        mLoadType[mLoadQueueCnt] = iType;
        mLoadOp[mLoadQueueCnt] = eOp;
        ++mLoadQueueCnt;
    }
}

// FUNCTION: SWBF2 0x006318a0
void SaveFsm::ProcessQueuedFiles()
{
    SetNextState(STATE_ProcessLastQueuedFile);
}

// FUNCTION: SWBF2 0x006318b0
bool SaveFsm::InputAccept(int iAnswer)
{
    mLoadSavePopupAnswer = iAnswer;
    return true;
}

// FUNCTION: SWBF2 0x00631810
SaveFile* SaveFsm::GetNextFile()
{
    return GetNextSaveFile(mSaveDevice);
}

void SaveFsm::MarkMemoryCard()
{
    mSaveDevice.MarkMemoryCard();
}

// FUNCTION: SWBF2 0x006317d0
bool SaveFsm::CheckMemoryCardMark()
{
    return mSaveDevice.CheckMemoryCardMark();
}

// FUNCTION: SWBF2 0x006317e0
bool SaveFsm::IsMemoryCardUnformatted()
{
    if (mSaveDevice.GetStatus() == -4)
    {
        return true;
    }
    return false;
}

// FUNCTION: SWBF2 0x006317f0
bool SaveFsm::IsDeviceInsertted()
{
    if (mSaveDevice.GetStatus() == -2)
    {
        return false;
    }
    return true;
}

// FUNCTION: SWBF2 0x00631800
void SaveFsm::ResetFileIterator()
{
    mSaveDevice.ResetFileIterator();
}

// FUNCTION: SWBF2 0x006318d0
void SaveFsm::EnterState()
{
    switch (mCurState)
    {
    case STATE_InitialMemcardCheck: Enter_InitialMemcardCheck(); return;
    case STATE_InitialMemcardCheck_CheckProfile: Enter_InitialMemcardCheck_CheckProfile(); return;
    case STATE_InitialMemcardCheck_Error: Enter_InitialMemcardCheck_Error(); return;
    case STATE_PreOp: Enter_PreOp(); return;
    case STATE_PreOp_Error: Enter_PreOp_Error(); return;
    case STATE_PreOp_ConfirmDashboard: Enter_PreOp_ConfirmDashboard(); return;
    case STATE_PreOp_Format_Prompt: Enter_PreOp_Format_Prompt(); return;
    case STATE_PreOp_Format: Enter_PreOp_Format(); return;
    case STATE_Save_CheckExist: Enter_Save_CheckExist(); return;
    case STATE_Save_Overwrite: Enter_Save_Overwrite(); return;
    case STATE_Save_Create: Enter_Save_Create(); return;
    case STATE_Save_Open: Enter_Save_Open(); return;
    case STATE_Save_WriteChecksum: Enter_Save_WriteChecksum(); return;
    case STATE_Save_WriteData: Enter_Save_WriteData(); return;
    case STATE_Save_Close: Enter_Save_Close(); return;
    case STATE_Save_Error: Enter_Save_Error(); return;
    case STATE_Save_TooManyFiles: Enter_Save_TooManyFiles(); return;
    case STATE_Load_Start: Enter_Load_Start(); return;
    case STATE_Load_Open: Enter_Load_Open(); return;
    case STATE_Load_ReadChecksum: Enter_Load_ReadChecksum(); return;
    case STATE_Load_ReadData: Enter_Load_ReadData(); return;
    case STATE_Load_Close: Enter_Load_Close(); return;
    case STATE_Load_Error: Enter_Load_Error(); return;
    case STATE_Delete: Enter_Delete(); return;
    case STATE_Delete_Error: Enter_Delete_Error(); return;
    case STATE_ProcessLastQueuedFile: Enter_ProcessLastQueuedFile(); return;
    default: return;
    }
}

// FUNCTION: SWBF2 0x006319e0
void SaveFsm::ExitState()
{
}

// FUNCTION: SWBF2 0x006319f0
void SaveFsm::UpdateState(float fDeltaTime)
{
    switch (mCurState)
    {
    case STATE_InitialMemcardCheck:    break;
    case STATE_InitialMemcardCheck_CheckProfile:    Update_InitialMemcardCheck_CheckProfile(); break;
    case STATE_InitialMemcardCheck_Error:    Update_InitialMemcardCheck_Error();        break;
    case STATE_PreOp_Error:    Update_PreOp_Error();                      break;
    case STATE_PreOp_ConfirmDashboard:  Update_PreOp_ConfirmDashboard();           break;
    case STATE_PreOp_Filelist:  Update_PreOp_Filelist();                   break;
    case STATE_PreOp_Format_Prompt:  Update_PreOp_Format_Prompt();              break;
    case STATE_PreOp_Format:  Update_PreOp_Format();                     break;
    case STATE_Save_Overwrite:  Update_Save_Overwrite();                   break;
    case STATE_Save_Create: Update_Save_Create();                      break;
    case STATE_Save_Open: Update_Save_Open();                        break;
    case STATE_Save_WriteChecksum: Update_Save_WriteChecksum();                break;
    case STATE_Save_WriteData: Update_Save_WriteData();                    break;
    case STATE_Save_Close: Update_Save_Close();                        break;
    case STATE_Save_Error: Update_Save_Error();                        break;
    case STATE_Save_TooManyFiles: Update_Save_TooManyFiles();                 break;
    case STATE_Load_Open: Update_Load_Open();                          break;
    case STATE_Load_ReadChecksum: Update_Load_ReadChecksum();                  break;
    case STATE_Load_ReadData: Update_Load_ReadData();                      break;
    case STATE_Load_Close: Update_Load_Close();                        break;
    case STATE_Load_Error: Update_Load_Error();                         break;
    case STATE_Delete: Update_Delete();                             break;
    case STATE_Delete_Error: Update_Delete_Error();                       break;
    default:   return;
    }
}

// FUNCTION: SWBF2 0x00631b20
void SaveFsm::Enter_InitialMemcardCheck()
{
    mErrorSuccess = false;
    int iResult = mSaveDevice.GetStatus();
    if (iResult == -2)
    {
        SetNextState(STATE_Success);
        return;
    }
    if (iResult == -4)
    {
        SetNextState(STATE_Success);
        return;
    }
    int iNeeded = (int)SaveDevice::BytesToBlocks(0x1800);
    unsigned int uiNeeded = 35;
    if (iNeeded > 35)
    {
        uiNeeded = iNeeded;
    }
    unsigned int uiFree = mSaveDevice.GetFreeSpace();
    if (uiFree < uiNeeded)
    {
        mXBOXAdditionalFreeNeeded = uiNeeded - uiFree;
        mXBOXTotalFreeNeeded = uiNeeded >> 10;
        SetNextState(STATE_InitialMemcardCheck_CheckProfile);
        return;
    }
    SetNextState(STATE_Success);
}

// FUNCTION: SWBF2 0x00631b90
void SaveFsm::Enter_InitialMemcardCheck_CheckProfile()
{
    if (mSaveDevice.StartRefreshFileList())
    {
        SafeLocalizeStrcpy(mMsgStr1, "ifs.loadsave_pc.save08", 400);
        SafeLocalizeStrcpy(mBtnB, "common.ok", 100);
        mBtns = 2;
        mBtnDefault = 2;
        SetNextState(STATE_InitialMemcardCheck_Error);
    }
    else
    {
        SafeLocalizeStrcpy(mMsgStr1, "ifs.loadsave_pc.save19", 400);
        mBtns = 0;
        ShowLoadSavePopup(true);
    }
}

// FUNCTION: SWBF2 0x00631d60
void SaveFsm::Enter_InitialMemcardCheck_Error()
{
    mLoadSavePopupAnswer = -1;
    ShowLoadSavePopup(true);
}

// FUNCTION: SWBF2 0x00631fc0
void SaveFsm::Enter_PreOp_Error()
{
    if (m_preop_SilentFail)
    {
        SetNextState(STATE_Failure);
    }
    else
    {
        mLoadSavePopupAnswer = -1;
        ShowLoadSavePopup(true);
    }
}

// FUNCTION: SWBF2 0x00632020
void SaveFsm::Enter_PreOp_ConfirmDashboard()
{
    mLoadSavePopupAnswer = -1;
    ShowLoadSavePopup(true);
}

// FUNCTION: SWBF2 0x00632170
void SaveFsm::Enter_PreOp_Format_Prompt()
{
    SafeLocalizeSPrintf(mMsgStr1, 400, "ifs.loadsave_pc.save10", mFailOpStr);
    SafeLocalizeStrcpy(mBtnB, "common.no", 100);
    SafeLocalizeStrcpy(mBtnC, "common.yes", 100);
    mBtns = 6;
    mBtnDefault = 2;
    mLoadSavePopupAnswer = -1;
    ShowLoadSavePopup(true);
}

// FUNCTION: SWBF2 0x00632230
void SaveFsm::Enter_PreOp_Format()
{
    SafeLocalizeStrcpy(mMsgStr1, "ifs.loadsave_pc.save11", 400);
    mBtns = 0;
    ShowLoadSavePopup(true);
    int iResult = mSaveDevice.GetStatus();
    if (iResult != -4)
    {
        if (iResult == -2)
        {
            SafeLocalizeSPrintf(mMsgStr1, 400, "ifs.loadsave_pc.save04", RedLocalizeDB::Find(0x7CF9A572u /* "ifs.loadsave_ps2.errPrefixFormat" */, false));
        }
        else
        {
            SafeLocalizeSPrintf(mMsgStr1, 400, "ifs.loadsave_pc.save13", RedLocalizeDB::Find(0x7CF9A572u /* "ifs.loadsave_ps2.errPrefixFormat" */, false));
        }
        SafeLocalizeStrcpy(mBtnB, "common.ok", 100);
    }
    else
    {
        int iFormat = mSaveDevice.StartFormat();
        if (iFormat == 1)
        {
            return;
        }
        if (iFormat == 0)
        {
            return;
        }
        SafeLocalizeSPrintf(mMsgStr1, 400, "ifs.loadsave_pc.save13", RedLocalizeDB::Find(0x7CF9A572u /* "ifs.loadsave_ps2.errPrefixFormat" */, false));
        SafeLocalizeStrcpy(mBtnB, "common.ok", 100);
    }
    mBtns = 2;
    mBtnDefault = 2;
    SetNextState(STATE_PreOp_Error);
}

// FUNCTION: SWBF2 0x00632380
void SaveFsm::Enter_Save_CheckExist()
{
    mSaveErrorAfterClose = false;
    int iType = mLoadType[mLoadQueueCnt - 1];
    if (!iType)
    {
        SetNextState(0x11);
        return;
    }
    mCurSaveFile = mSaveDevice.FindFileInList(mLoadFilename[mLoadQueueCnt - 1], iType);
    JournalSaveFile(mCurSaveFile);
    if (mCurSaveFile)
    {
        SetNextState(0x11);
    }
    else
    {
        SetNextState(0x10);
    }
}

// FUNCTION: SWBF2 0x006323e0
void SaveFsm::Enter_Save_Overwrite()
{
    SafeLocalizeStrcpy(mMsgStr1, "ifs.loadsave_pc.save14", 400);
    SafeLocalizeStrcpy(mBtnA, "ifs.loadsave_pc.yes", 100);
    SafeLocalizeStrcpy(mBtnB, "ifs.loadsave_pc.no", 100);
    mBtns = 3;
    mBtnDefault = 2;
    mLoadSavePopupAnswer = -1;
    ShowLoadSavePopup(true);
}

// FUNCTION: SWBF2 0x00632440
void SaveFsm::Update_Save_Overwrite()
{
    if (mLoadSavePopupAnswer == -1)
    {
        return;
    }
    ShowLoadSavePopup(false);
    if (mLoadSavePopupAnswer == 1)
    {
        SetNextState(0x11);
    }
    else
    {
        SetNextState(STATE_Failure);
        mLoadQueueCnt = 0;
    }
}

// FUNCTION: SWBF2 0x00632480
void SaveFsm::Enter_Save_Create()
{
    SafeLocalizeStrcpy(mMsgStr1, "ifs.loadsave_pc.save15", 400);
    mBtns = 0;
    mLoadSavePopupAnswer = -1;
    ShowLoadSavePopup(true);
    int iResult = mSaveDevice.StartCreateFile(mLoadFilename[mLoadQueueCnt - 1], mLoadType[mLoadQueueCnt - 1]);
    if (iResult != 0 && iResult != 1)
    {
        if (iResult == -6)
        {
            SetNextState(STATE_Save_TooManyFiles);
        }
        else
        {
            SetNextState(STATE_Save_Error);
        }
    }
}

// FUNCTION: SWBF2 0x00632500
void SaveFsm::Update_Save_Create()
{
    int iResult = mSaveDevice.CycleCreateFile(mLoadFilename[mLoadQueueCnt - 1], mLoadType[mLoadQueueCnt - 1]);
    if (iResult == 1)
    {
        return;
    }
    if (iResult)
    {
        if (iResult == -6)
        {
            SetNextState(STATE_Save_TooManyFiles);
        }
        else
        {
            SetNextState(STATE_Save_Error);
        }
    }
    else
    {
        SetNextState(0x11);
    }
}

// FUNCTION: SWBF2 0x00632530
void SaveFsm::Enter_Save_Open()
{
    SafeLocalizeStrcpy(mMsgStr1, "ifs.loadsave_pc.save15", 400);
    mBtns = 0;
    ShowLoadSavePopup(true);
    mCurSaveFile = mSaveDevice.FindFileInList(mLoadFilename[mLoadQueueCnt - 1], mLoadType[mLoadQueueCnt - 1]);
    JournalSaveFile(mCurSaveFile);
    if (mCurSaveFile)
    {
        int iResult = mSaveDevice.StartOpenFile(*mCurSaveFile, false);
        if (iResult == 0 || iResult == 1)
        {
            return;
        }
    }
    SetNextState(STATE_Save_Error);
}

// FUNCTION: SWBF2 0x006325c0
void SaveFsm::Update_Save_Open()
{
    int iResult = mSaveDevice.CycleOpenFile(*mCurSaveFile, false);
    if (iResult == 1)
    {
        return;
    }
    if (iResult)
    {
        mSaveErrorAfterClose = true;
        SetNextState(0x14);
    }
    else
    {
        SetNextState(0x12);
    }
}

// FUNCTION: SWBF2 0x006325f0
void SaveFsm::Enter_Save_WriteChecksum()
{
    int iIdx = mLoadQueueCnt - 1;
    if (!mLoadType[iIdx])
    {
        SetNextState(0x13);
        return;
    }
    mChecksum = CalcCRC(mLoadDest[iIdx], mLoadSize[iIdx], 0);
    int iResult = mSaveDevice.StartWriteFile(*mCurSaveFile, &mChecksum, CRC_SIZE);
    if (iResult != 1 && iResult != 0)
    {
        mSaveErrorAfterClose = true;
        SetNextState(0x14);
    }
}

// FUNCTION: SWBF2 0x00632660
void SaveFsm::Update_Save_WriteChecksum()
{
    int iResult = mSaveDevice.CycleWriteFile(*mCurSaveFile, &mChecksum, CRC_SIZE);
    if (iResult == 1)
    {
        return;
    }
    if (iResult)
    {
        mSaveErrorAfterClose = true;
        SetNextState(0x14);
    }
    else
    {
        SetNextState(0x13);
    }
}

// FUNCTION: SWBF2 0x00632690
void SaveFsm::Enter_Save_WriteData()
{
    int iIdx = mLoadQueueCnt - 1;
    int iResult = mSaveDevice.StartWriteFile(*mCurSaveFile, mLoadDest[iIdx], mLoadSize[iIdx]);
    if (iResult == 1 || iResult == 0)
    {
        return;
    }
    mSaveErrorAfterClose = true;
    SetNextState(0x14);
}

// FUNCTION: SWBF2 0x006326d0
void SaveFsm::Update_Save_WriteData()
{
    int iResult = mSaveDevice.CycleWriteFile(*mCurSaveFile, mLoadDest[mLoadQueueCnt - 1], mLoadSize[mLoadQueueCnt - 1]);
    if (iResult == 1)
    {
        return;
    }
    if (iResult == 0)
    {
        SetNextState(0x14);
        return;
    }
    mSaveErrorAfterClose = true;
    SetNextState(0x14);
}

// FUNCTION: SWBF2 0x006326f0
void SaveFsm::Enter_Save_Close()
{
    int iResult = mSaveDevice.StartCloseFile(*mCurSaveFile, false);
    if (iResult != 0 && iResult != 1)
    {
        SetNextState(STATE_Save_Error);
    }
}

// FUNCTION: SWBF2 0x00632720
void SaveFsm::Update_Save_Close()
{
    int iResult = mSaveDevice.CycleCloseFile(*mCurSaveFile, false);
    if (iResult == 1)
    {
        return;
    }
    if (iResult)
    {
        SetNextState(STATE_Save_Error);
    }
    else if (mSaveErrorAfterClose == true)
    {
        SetNextState(STATE_Save_Error);
    }
    else
    {
        ShowLoadSavePopup(false);
        PblUnicode::ustrncnv(mLoadFilename[mLoadQueueCnt - 1], "", 66);
        mLoadDest[mLoadQueueCnt - 1] = 0;
        --mLoadQueueCnt;
        SetNextState(0x1F);
    }
}

// FUNCTION: SWBF2 0x00632790
void SaveFsm::Enter_Save_Error()
{
    SafeLocalizeStrcpy(mMsgStr1, "ifs.loadsave_pc.save16", 400);
    SafeLocalizeStrcpy(mBtnB, "ifs.loadsave_pc.ok", 100);
    mBtns = 2;
    mBtnDefault = 2;
    mLoadSavePopupAnswer = -1;
    ShowLoadSavePopup(true);
}

// FUNCTION: SWBF2 0x006327e0
void SaveFsm::Update_Save_Error()
{
    if (mLoadSavePopupAnswer == -1)
    {
        return;
    }
    ShowLoadSavePopup(false);
    if (mLoadSavePopupAnswer == 1)
    {
        SetNextState(0xE);
    }
    else
    {
        SetNextState(STATE_Failure);
        mLoadQueueCnt = 0;
    }
}

// FUNCTION: SWBF2 0x00632820
void SaveFsm::Enter_Save_TooManyFiles()
{
    SafeLocalizeStrcpy(mMsgStr1, "ifs.loadsave_pc.save03", 400);
    SafeLocalizeStrcpy(mBtnB, "common.ok", 100);
    mBtns = 2;
    mBtnDefault = 2;
    mLoadSavePopupAnswer = -1;
    ShowLoadSavePopup(true);
}

// FUNCTION: SWBF2 0x00632870
void SaveFsm::Update_Save_TooManyFiles()
{
    if (mLoadSavePopupAnswer == -1)
    {
        return;
    }
    ShowLoadSavePopup(false);
    if (mLoadSavePopupAnswer == 1)
    {
        SetNextState(STATE_Failure);
    }
    else
    {
        SetNextState(STATE_Failure);
        mLoadQueueCnt = 0;
    }
}

// FUNCTION: SWBF2 0x006328b0
void SaveFsm::Enter_Load_Start()
{
    mLoadErrorAfterClose = false;
    SafeLocalizeStrcpy(mMsgStr1, "ifs.loadsave_pc.save18", 400);
    mBtns = 0;
    ShowLoadSavePopup(true);
    mCurSaveFile = mSaveDevice.FindFileInList(mLoadFilename[mLoadQueueCnt - 1], mLoadType[mLoadQueueCnt - 1]);
    JournalSaveFile(mCurSaveFile);
    if (mCurSaveFile)
    {
        SetNextState(STATE_Load_Open);
    }
    else
    {
        SetNextState(STATE_Load_Error);
    }
}

// FUNCTION: SWBF2 0x00632930
void SaveFsm::Enter_Load_Open()
{
    int iResult = mSaveDevice.StartOpenFile(*mCurSaveFile, false);
    if (iResult != 0 && iResult != 1)
    {
        SetNextState(STATE_Load_Error);
    }
}

// FUNCTION: SWBF2 0x00632980
void SaveFsm::Enter_Load_ReadChecksum()
{
    int iResult = mSaveDevice.StartReadFile(*mCurSaveFile, &mChecksum, CRC_SIZE);
    if (iResult == 1 || iResult == 0)
    {
        return;
    }
    mLoadErrorAfterClose = true;
    SetNextState(STATE_Load_Close);
}

// FUNCTION: SWBF2 0x006329f0
void SaveFsm::Enter_Load_ReadData()
{
    int iIdx = mLoadQueueCnt - 1;
    int iResult = mSaveDevice.StartReadFile(*mCurSaveFile, mLoadDest[iIdx], mLoadSize[iIdx]);
    if (iResult == 1 || iResult == 0)
    {
        return;
    }
    mLoadErrorAfterClose = true;
    SetNextState(STATE_Load_Close);
}

// FUNCTION: SWBF2 0x00632a90
void SaveFsm::Enter_Load_Close()
{
    int iResult = mSaveDevice.StartCloseFile(*mCurSaveFile, false);
    if (iResult != 0 && iResult != 1)
    {
        SetNextState(STATE_Load_Error);
    }
}

// FUNCTION: SWBF2 0x00632b30
void SaveFsm::Enter_Load_Error()
{
    SafeLocalizeStrcpy(mMsgStr1, "ifs.loadsave_pc.save17", 400);
    SafeLocalizeStrcpy(mBtnA, "ifs.loadsave_pc.ok", 100);
    mBtns = 1;
    mBtnDefault = 1;
    mLoadSavePopupAnswer = -1;
    ShowLoadSavePopup(true);
}

// FUNCTION: SWBF2 0x00632bb0
void SaveFsm::Enter_Delete()
{
    mLoadErrorAfterClose = false;
    if (mLoadOp[mLoadQueueCnt - 1] == QOP_SILENTDELETE)
    {
        SafeLocalizeStrcpy(mMsgStr1, "ifs.loadsave_pc.save15", 400);
    }
    else
    {
        SafeLocalizeStrcpy(mMsgStr1, "ifs.loadsave_pc.save22", 400);
    }
    mBtns = 0;
    ShowLoadSavePopup(true);
    mCurSaveFile = mSaveDevice.FindFileInList(mLoadFilename[mLoadQueueCnt - 1], mLoadType[mLoadQueueCnt - 1]);
    JournalSaveFile(mCurSaveFile);
    if (mCurSaveFile)
    {
        int iResult = mSaveDevice.StartDeleteFile(mLoadFilename[mLoadQueueCnt - 1], mLoadType[mLoadQueueCnt - 1], 0);
        if (iResult == 0 || iResult == 1)
        {
            return;
        }
    }
    SetNextState(STATE_Delete_Error);
}

// FUNCTION: SWBF2 0x00632ce0
void SaveFsm::Enter_Delete_Error()
{
    SafeLocalizeStrcpy(mMsgStr1, "ifs.loadsave_pc.save23", 400);
    SafeLocalizeStrcpy(mBtnA, "ifs.loadsave_pc.ok", 100);
    mBtns = 1;
    mBtnDefault = 1;
    mLoadSavePopupAnswer = -1;
    ShowLoadSavePopup(true);
}

// FUNCTION: SWBF2 0x00632d60
bool SaveFsm::Enter_ProcessLastQueuedFile()
{
    int iCnt = mLoadQueueCnt;
    if (!iCnt)
    {
        SetNextState(STATE_Success);
        return true;
    }
    switch (mLoadOp[iCnt - 1])
    {
    case 0: SetNextState(STATE_Load_Start); break;
    case 1: SetNextState(STATE_Save_CheckExist); break;
    case 2:
    case 3: SetNextState(STATE_Delete); break;
    default: break;
    }
    return true;
}

// FUNCTION: SWBF2 0x00631dd0
void SaveFsm::Enter_PreOp()
{
    SafeLocalizeStrcpy(mMsgStr1, "ifs.loadsave_pc.save19", 400);
    mBtns = 0;
    ShowLoadSavePopup(true);

    int iResult = mSaveDevice.GetStatus();
    if (iResult == -2)
    {
        SafeLocalizeSPrintf(mMsgStr1, 400, "ifs.loadsave_pc.save04", mFailOpStr);
        SafeLocalizeStrcpy(mBtnB, "common.ok", 100);
        mBtns = 2;
        mBtnDefault = 2;
        SetNextState(STATE_PreOp_Error);
        return;
    }
    if (iResult == -4)
    {
        if (m_preop_AllowFormat)
        {
            SetNextState(0xC);
        }
        else
        {
            SafeLocalizeSPrintf(mMsgStr1, 400, "ifs.loadsave_pc.save05", mFailOpStr);
            SafeLocalizeStrcpy(mBtnB, "common.ok", 100);
            mBtns = 2;
            mBtnDefault = 2;
            SetNextState(STATE_PreOp_Error);
        }
        return;
    }

    if (m_preop_CheckFreeBytes)
    {
        int iNeeded = (int)SaveDevice::BytesToBlocks(m_preop_CheckFreeBytes);
        unsigned int uiNeeded = 3;
        if (iNeeded > 3)
        {
            uiNeeded = iNeeded;
        }
        unsigned int uiFree = mSaveDevice.GetFreeSpace();
        if (uiFree < uiNeeded)
        {
            mXBOXAdditionalFreeNeeded = uiNeeded - uiFree;
            mXBOXTotalFreeNeeded = 3 * (uiNeeded >> 10);
            if (!m_preop_UseIMCMsg)
            {

                SafeLocalizeSPrintf(mMsgStr1, 400, "ifs.loadsave_pc.save06", mFailOpStr, mXBOXAdditionalFreeNeeded);
                SafeLocalizeStrcpy(mBtnB, "common.ok", 100);
                mBtns = 2;
                mBtnDefault = 2;
            }
            else
            {
                SafeLocalizeSPrintf(mMsgStr1, 400, "ifs.loadsave_pc.save02", uiNeeded - uiFree);
                SafeLocalizeStrcpy(mBtnA, "ifs.loadsave_pc.freemoreblocks", 100);
                SafeLocalizeStrcpy(mBtnB, "ifs.loadsave_pc.continue", 100);
                mBtns = 3;
                mBtnDefault = 2;
            }
            SetNextState(STATE_PreOp_Error);
            return;
        }
    }

    if (m_preop_NoFileList)
    {
        SetNextState(STATE_Success);
    }
    else if (mSaveDevice.StartRefreshFileList())
    {
        SafeLocalizeStrcpy(mMsgStr1, "ifs.loadsave_pc.save08", 400);
        SafeLocalizeStrcpy(mBtnB, "common.ok", 100);
        mBtns = 2;
        mBtnDefault = 2;
        SetNextState(STATE_PreOp_Error);
    }
    else
    {
        SetNextState(0xB);
    }
}

// FUNCTION: SWBF2 0x00631c00
void SaveFsm::Update_InitialMemcardCheck_CheckProfile()
{
    int iResult = mSaveDevice.CycleRefreshFileList();
    if (iResult == 1)
    {
        return;
    }
    if (iResult)
    {
        SafeLocalizeStrcpy(mMsgStr1, "ifs.loadsave_pc.save09", 400);
        SafeLocalizeStrcpy(mBtnB, "common.ok", 100);
        mBtns = 2;
        mBtnDefault = 2;
        SetNextState(STATE_InitialMemcardCheck_Error);
        return;
    }

    mSaveDevice.ResetFileIterator();
    SaveFile* pSlot = GetNextSaveFile(mSaveDevice);
    bool bFlag1 = false, bFlag2 = false, bFlag3 = false;
    while (pSlot)
    {
        if (pSlot->GetType() == 1)
        {
            bFlag1 = true;
        }
        if (pSlot->GetType() == 2)
        {
            bFlag2 = true;
        }
        if (pSlot->GetType() == 3)
        {
            bFlag3 = true;
        }
        pSlot = GetNextSaveFile(mSaveDevice);
    }

    if (bFlag1 && bFlag2 && bFlag3)
    {
        SetNextState(STATE_Success);
    }
    else
    {
        SafeLocalizeSPrintf(mMsgStr1, 400, "ifs.loadsave_pc.save02", mXBOXAdditionalFreeNeeded);
        SafeLocalizeStrcpy(mBtnA, "ifs.loadsave_pc.freemoreblocks", 100);
        SafeLocalizeStrcpy(mBtnB, "ifs.loadsave_pc.continue", 100);
        mBtns = 3;
        mBtnDefault = 1;
        mErrorSuccess = true;
        SetNextState(STATE_InitialMemcardCheck_Error);
    }
}

// FUNCTION: SWBF2 0x00631d80
void SaveFsm::Update_InitialMemcardCheck_Error()
{
    if (mLoadSavePopupAnswer == -1)
    {
        return;
    }
    ShowLoadSavePopup(false);
    if (mLoadSavePopupAnswer == 1)
    {
        SetNextState(STATE_InitialMemcardCheck);
    }
    else if (mErrorSuccess)
    {
        SetNextState(STATE_Success);
    }
    else
    {
        SetNextState(STATE_Failure);
    }
}

// FUNCTION: SWBF2 0x00631ff0
void SaveFsm::Update_PreOp_Error()
{
    if (mLoadSavePopupAnswer == -1)
    {
        return;
    }
    ShowLoadSavePopup(false);
    if (mLoadSavePopupAnswer == 1)
    {
        SetNextState(STATE_PreOp);
    }
    else
    {
        SetNextState(STATE_Failure);
    }
}

// FUNCTION: SWBF2 0x00632040
void SaveFsm::Update_PreOp_ConfirmDashboard()
{
    if (mLoadSavePopupAnswer == -1)
    {
        return;
    }
    ShowLoadSavePopup(false);
    if (!m_preop_UseIMCMsg)
    {
        SafeLocalizeSPrintf(mMsgStr1, 400, "ifs.loadsave_pc.save06", mFailOpStr, mXBOXAdditionalFreeNeeded);
        SafeLocalizeStrcpy(mBtnB, "ifs.loadsave_pc.cancel", 100);
    }
    else
    {
        SafeLocalizeSPrintf(mMsgStr1, 400, "ifs.loadsave_pc.save02", mXBOXAdditionalFreeNeeded);
        SafeLocalizeStrcpy(mBtnB, "ifs.loadsave_pc.continue", 100);
    }
    SafeLocalizeStrcpy(mBtnA, "ifs.loadsave_pc.freemoreblocks", 100);
    mBtns = 3;
    mBtnDefault = 2;
    SetNextState(STATE_PreOp_Error);
}

// FUNCTION: SWBF2 0x006320f0
void SaveFsm::Update_PreOp_Filelist()
{
    int iResult = mSaveDevice.CycleRefreshFileList();
    if (iResult == 1)
    {
        return;
    }
    if (iResult)
    {
        SafeLocalizeStrcpy(mMsgStr1, "ifs.loadsave_pc.save09", 400);
        SafeLocalizeStrcpy(mBtnB, "common.ok", 100);
        mBtns = 2;
        mBtnDefault = 2;
        SetNextState(STATE_PreOp_Error);
        return;
    }

    unsigned int uiFreeFiles = mSaveDevice.GetFreeFiles();
    unsigned int uiCheckFiles = m_preop_CheckFreeFiles;
    if (uiCheckFiles && uiFreeFiles < uiCheckFiles)
    {
        mXBOXTotalFreeNeeded = uiCheckFiles - uiFreeFiles;
        SafeLocalizeStrcpy(mMsgStr1, "ifs.loadsave_pc.save07", 400);
        SafeLocalizeStrcpy(mBtnB, "common.ok", 100);
        mBtns = 2;
        mBtnDefault = 2;
        SetNextState(STATE_PreOp_Error);
        return;
    }

    SetNextState(STATE_Success);
}

// FUNCTION: SWBF2 0x006321e0
void SaveFsm::Update_PreOp_Format_Prompt()
{
    if (mLoadSavePopupAnswer == -1)
    {
        return;
    }
    ShowLoadSavePopup(false);
    if (mLoadSavePopupAnswer == 1)
    {
        SetNextState(STATE_PreOp);
    }
    else if (mLoadSavePopupAnswer == 2)
    {
        SetNextState(STATE_Failure);
    }
    else
    {
        SetNextState(0xD);
    }
}

// FUNCTION: SWBF2 0x00632310
void SaveFsm::Update_PreOp_Format()
{
    int iResult = mSaveDevice.CycleFormat();
    if (iResult == 1)
    {
        return;
    }
    if (iResult)
    {

        const unsigned short* pJoy = RedLocalizeDB::Find(0x7CF9A572u /* "ifs.loadsave_ps2.errPrefixFormat" */, false);
        SafeLocalizeSPrintf(mMsgStr1, 400, "ifs.loadsave_pc.save13", pJoy);
        SafeLocalizeStrcpy(mBtnB, "common.ok", 100);
        mBtns = 2;
        mBtnDefault = 2;
        SetNextState(STATE_PreOp_Error);
    }
    else
    {
        SetNextState(STATE_PreOp);
    }
}

// FUNCTION: SWBF2 0x00632960
void SaveFsm::Update_Load_Open()
{
    int iResult = mSaveDevice.CycleOpenFile(*mCurSaveFile, false);
    if (iResult == 1)
    {
        return;
    }
    if (iResult)
    {
        SetNextState(STATE_Load_Error);
    }
    else
    {
        SetNextState(STATE_Load_ReadChecksum);
    }
}

// FUNCTION: SWBF2 0x006329c0
void SaveFsm::Update_Load_ReadChecksum()
{
    int iResult = mSaveDevice.CycleReadFile(*mCurSaveFile, &mChecksum, CRC_SIZE);
    if (iResult == 1)
    {
        return;
    }
    if (iResult)
    {
        mLoadErrorAfterClose = true;
        SetNextState(STATE_Load_Close);
    }
    else
    {
        SetNextState(STATE_Load_ReadData);
    }
}

// FUNCTION: SWBF2 0x00632a30
void SaveFsm::Update_Load_ReadData()
{
    int iResult = mSaveDevice.CycleReadFile(*mCurSaveFile, mLoadDest[mLoadQueueCnt - 1], mLoadSize[mLoadQueueCnt - 1]);
    if (iResult == 1)
    {
        return;
    }
    if (iResult)
    {
        mLoadErrorAfterClose = true;
        SetNextState(STATE_Load_Close);
    }
    else
    {
        PblJournal::Get((PblJournalType)0)->RecordLow(mLoadDest[mLoadQueueCnt - 1], (int)mLoadSize[mLoadQueueCnt - 1]);
        SetNextState(STATE_Load_Close);
    }
}

// FUNCTION: SWBF2 0x00632ac0
void SaveFsm::Update_Load_Close()
{
    int iResult = mSaveDevice.CycleCloseFile(*mCurSaveFile, false);
    if (iResult == 1)
    {
        return;
    }
    if (iResult)
    {
        SetNextState(STATE_Load_Error);
    }
    else if (mLoadErrorAfterClose == true)
    {
        SetNextState(STATE_Load_Error);
    }
    else
    {
        ShowLoadSavePopup(false);
        PblUnicode::ustrncnv(mLoadFilename[mLoadQueueCnt - 1], "", 66);
        mLoadDest[mLoadQueueCnt - 1] = 0;
        --mLoadQueueCnt;
        SetNextState(0x1F);
    }
}

// FUNCTION: SWBF2 0x00632b80
void SaveFsm::Update_Load_Error()
{
    if (mLoadSavePopupAnswer == -1)
    {
        return;
    }
    ShowLoadSavePopup(false);
    SetNextState(STATE_Failure);
    mLoadQueueCnt = 0;
}

// FUNCTION: SWBF2 0x00632c70
void SaveFsm::Update_Delete()
{
    int iResult = mSaveDevice.CycleDeleteFile(mLoadFilename[mLoadQueueCnt - 1], mLoadType[mLoadQueueCnt - 1]);
    if (iResult == 1)
    {
        return;
    }
    if (iResult)
    {
        SetNextState(STATE_Delete_Error);
    }
    else
    {
        ShowLoadSavePopup(false);
        PblUnicode::ustrncnv(mLoadFilename[mLoadQueueCnt - 1], "", 66);
        mLoadDest[mLoadQueueCnt - 1] = 0;
        --mLoadQueueCnt;
        SetNextState(0x1F);
    }
}

// FUNCTION: SWBF2 0x00632d30
void SaveFsm::Update_Delete_Error()
{
    if (mLoadSavePopupAnswer == -1)
    {
        return;
    }
    ShowLoadSavePopup(false);
    SetNextState(STATE_Failure);
    mLoadQueueCnt = 0;
}

// FUNCTION: SWBF2 0x00632dc0
void SaveFsm::ShowLoadSavePopup(bool bSetup)
{

    RedInterfaceElement* pTitle = RedInterfaceElement::Find(0x9E227C3Fu /* "Popup_LoadSave2.title" */);
    RedInterfaceElement* pBtnA = RedInterfaceElement::Find(0x4DB5F949u /* "Popup_LoadSave2.buttons.A.label" */);
    RedInterfaceElement* pBtnB = RedInterfaceElement::Find(0x597FBABCu /* "Popup_LoadSave2.buttons.B.label" */);
    RedInterfaceElement* pBtnC = RedInterfaceElement::Find(0x3C76CD7Bu /* "Popup_LoadSave2.buttons.C.label" */);

    if (bSetup)
    {
        ((RedTextElement*)pBtnA)->SetText((unsigned short const*)mBtnA, true);
        ((RedTextElement*)pBtnB)->SetText((unsigned short const*)mBtnB, true);
        ((RedTextElement*)pBtnC)->SetText((unsigned short const*)mBtnC, true);
        pTitle->Enable(true);
        pBtnA->Enable((mBtns & 1) != 0);
        pBtnB->Enable((mBtns >> 1 & 1) != 0);
        pBtnC->Enable((mBtns >> 2 & 1) != 0);
    }

    LuaHelper::CallGlobalProc("Popup_LoadSave2_ResizeButtons");
    LuaHelper::CallGlobalProc("Popup_LoadSave2_SelectButton", (float)mBtnDefault);
    RedInterfaceElement* pPopup = RedInterfaceElement::Find(0xE65C8183u /* "Popup_LoadSave2" */);
    pPopup->Enable(bSetup);
    LuaHelper::CallGlobalProc("gPopup_Loadsave2_fnSetTitleStr", mMsgStr1);
}

// FUNCTION: SWBF2 0x006315a0
unsigned short* SafeLocalizeStrcpy(unsigned short* pusDest, const char* pcSrc, unsigned int uiSize)
{
    const unsigned short* pusText = RedLocalizeDB::Find(PblHash(pcSrc), false);
    if (!pusText)
    {
        PblUnicode::ustrncnv(pusDest, pcSrc, uiSize);
        return 0;
    }
    wcscpy((wchar_t*)pusDest, (const wchar_t*)pusText);
    return pusDest;
}

// FUNCTION: SWBF2 0x00631750
bool SaveFsm::GetSuccess() const
{
    return mCurState == STATE_Success;
}

