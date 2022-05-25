#pragma once

class CKeyInput
{
    std::map<int, int> m_mapRegisteredKey;      // (virt, id)
    std::map<int, short> m_mapLastKeyState;
    std::map<int, DWORD> m_mapKeyPressTime;
    bool m_bCapsLockEnabled;
    bool m_bShiftPressed;
    bool m_bEnterPressed;
    DWORD m_dwFirstRepeatTick;
    DWORD m_dwRepeatTick;

public:
    CKeyInput(void);

    void Clear(void);
    void Register(int nVirtKey, int nID);
    void Query(std::list<ST_KEYSTATE>& outState);

    bool IsEnabledCapsLock(void);
    bool IsEnabledShift(void);
    bool IsEnterPressed(void);

    void SetRepeatTick(DWORD dwRepeatTick = 200);

private:
    void GenerateRepeatKey(std::list<ST_KEYSTATE>& inState, std::list<ST_KEYSTATE>& outRepeatState);
};