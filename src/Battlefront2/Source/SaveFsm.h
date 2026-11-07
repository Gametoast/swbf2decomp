#pragma once

#include "Fsm.h"
#include "Battlefront2/Platform/Pc/pcSaveDevice.h"
#include "Battlefront2/Platform/Pc/pcSaveFile.h"

enum SaveFsmQueueOp
{
    QOP_LOAD = 0,
    QOP_SAVE = 1,
    QOP_DELETE = 2,
    QOP_SILENTDELETE = 3
};

// VTABLE: SWBF2 0x007ae3a8
class SaveFsm : public Fsm
{
    // 5 and up are named after the Enter_ / Update_ functions EnterState and UpdateState call for them; 1-4 after their use
    // (1 first, 2 where it rests, 3 success as GetSuccess reads it, 4 failure).
    enum
    {
        STATE_START = 1,
        STATE_Idle = 2,
        STATE_Success = 3,
        STATE_Failure = 4,
        STATE_InitialMemcardCheck = 5,
        STATE_InitialMemcardCheck_CheckProfile = 6,
        STATE_InitialMemcardCheck_Error = 7,
        STATE_PreOp = 8,
        STATE_PreOp_Error = 9,
        STATE_PreOp_ConfirmDashboard = 10,
        STATE_PreOp_Filelist = 11,
        STATE_PreOp_Format_Prompt = 12,
        STATE_PreOp_Format = 13,
        STATE_Save_CheckExist = 14,
        STATE_Save_Overwrite = 15,
        STATE_Save_Create = 16,
        STATE_Save_Open = 17,
        STATE_Save_WriteChecksum = 18,
        STATE_Save_WriteData = 19,
        STATE_Save_Close = 20,
        STATE_Save_Error = 21,
        STATE_Save_TooManyFiles = 22,
        STATE_Load_Start = 23,
        STATE_Load_Open = 24,
        STATE_Load_ReadChecksum = 25,
        STATE_Load_ReadData = 26,
        STATE_Load_Close = 27,
        STATE_Load_Error = 28,
        STATE_Delete = 29,
        STATE_Delete_Error = 30,
        STATE_ProcessLastQueuedFile = 31
    };

public:

    SaveFsm();

    void StartInitialMemcardCheck();

    void StartPreOp(bool bAllowFormat, unsigned int uiCheckFreeBytes,
                     unsigned int uiCheckFreeFiles, unsigned short const* pusFailOpStr,
                     bool bSilentFail, bool bNoFileList, bool bUseIMCMsg, bool bIsPreLoad);

    void EmptyQueue();

    void QueueFile(unsigned short const* pusFilename, void* pDest, unsigned int uiSize,
                    int iType, SaveFsmQueueOp eOp);

    bool InputAccept(int iAnswer);

    void ProcessQueuedFiles();

    SaveFile* GetNextFile();

protected:

    virtual void EnterState();

    virtual void ExitState();
    virtual void UpdateState(float fDeltaTime);

public:
    virtual bool IsDone() const;

protected:

    void Enter_InitialMemcardCheck();
    void Enter_InitialMemcardCheck_CheckProfile();
    void Enter_InitialMemcardCheck_Error();
    void Enter_PreOp_Error();
    void Enter_PreOp_ConfirmDashboard();
    void Enter_PreOp_Format_Prompt();
    void Enter_PreOp_Format();
    void Enter_Save_CheckExist();
    void Enter_Save_Overwrite();
    void Enter_Save_Create();
    void Enter_Save_Open();
    void Enter_Save_WriteChecksum();
    void Enter_Save_WriteData();
    void Enter_Save_Close();
    void Enter_Save_Error();
    void Enter_Save_TooManyFiles();
    void Enter_Load_Start();
    void Enter_Load_Open();
    void Enter_Load_ReadChecksum();
    void Enter_Load_ReadData();
    void Enter_Load_Close();
    void Enter_Load_Error();
    void Enter_Delete();
    void Enter_Delete_Error();
    bool Enter_ProcessLastQueuedFile();
    void Enter_PreOp();

    void ShowLoadSavePopup(bool bSetup);

    void Update_InitialMemcardCheck_CheckProfile();
    void Update_InitialMemcardCheck_Error();
    void Update_PreOp_Error();
    void Update_PreOp_ConfirmDashboard();
    void Update_PreOp_Filelist();
    void Update_PreOp_Format_Prompt();
    void Update_PreOp_Format();
    void Update_Save_Overwrite();
    void Update_Save_Create();
    void Update_Save_Open();
    void Update_Save_WriteChecksum();
    void Update_Save_WriteData();
    void Update_Save_Close();
    void Update_Save_Error();
    void Update_Save_TooManyFiles();

    void Update_Load_Open();
    void Update_Load_ReadChecksum();
    void Update_Load_ReadData();

    void Update_Load_Close();
    void Update_Load_Error();
    void Update_Delete();
    void Update_Delete_Error();

public:

    bool GetSuccess() const;

    void MarkMemoryCard();
    bool CheckMemoryCardMark();
    bool IsMemoryCardUnformatted();
    bool IsDeviceInsertted();
    void ResetFileIterator();

public:
    SaveDevice mSaveDevice;
    unsigned int mChecksum;
    SaveFile* mCurSaveFile;
    int mLoadQueueCnt;
    unsigned short mLoadFilename[5][66];

    unsigned int mLoadSize[5];
    void* mLoadDest[5];
    int mLoadType[5];
    SaveFsmQueueOp mLoadOp[5];
    unsigned short mMsgStr1[400];
    unsigned short mBtnA[100];
    unsigned short mBtnB[100];
    unsigned short mBtnC[100];
    unsigned char mBtns;

    unsigned char mBtnDefault;
    int mLoadSavePopupAnswer;
    int mXBOXTotalFreeNeeded;
    int mXBOXAdditionalFreeNeeded;
    bool mErrorSuccess;
    bool m_preop_AllowFormat;
    unsigned int m_preop_CheckFreeBytes;
    unsigned int m_preop_CheckFreeFiles;
    unsigned short mFailOpStr[64];
    bool m_preop_SilentFail;
    bool m_preop_NoFileList;
    bool m_preop_UseIMCMsg;
    bool m_preop_bIsPreLoad;
    bool mSaveErrorAfterClose;
    bool mLoadErrorAfterClose;

    static const int CRC_SIZE = 4;
    static const int MAX_LOAD_QUEUE = 5;
};
