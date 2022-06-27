// VisualController.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#include <Windows.h>
#include <WinUser.h>
using namespace std;

void ShowDesktop();
void ReadSerial();

int main()
{
    cout << "Hello World!\n";
    string str;
    cin >> str;
    while (1) {
        if (str == "do") {
            ShowDesktop();
        }
        else if (str == "s") {
            ReadSerial();
        }
        cin >> str;
    }
}

void ReadSerial() {
    // Open serial port
    HANDLE serialHandle;

    serialHandle = CreateFile(L"COM3", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    // Do some basic settings
    DCB serialParams = { 0 };
    serialParams.DCBlength = sizeof(serialParams);

    if (!GetCommState(serialHandle, &serialParams)) {
        cout << "Error GetCommState" << endl;
    }
    serialParams.BaudRate = 9600;
    serialParams.ByteSize = 8;
    serialParams.StopBits = TWOSTOPBITS;
    serialParams.Parity = NOPARITY;
    if (!SetCommState(serialHandle, &serialParams)) {
        cout << "Error SetCommState" << endl;
    }

    // Set timeouts
    COMMTIMEOUTS timeout = { 0 };
    timeout.ReadIntervalTimeout = 50;
    timeout.ReadTotalTimeoutConstant = 50;
    timeout.ReadTotalTimeoutMultiplier = 50;
    timeout.WriteTotalTimeoutConstant = 50;
    timeout.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(serialHandle, &timeout)) {
        cout << "Error SetCommTimeouts" << endl;
    }

#define number 4
    char szBuff[number + 1] = { 0 };
    DWORD dwBytesRead = 0;

    bool prevState[4] = {false};
    char numtochar[4] = { 0 };
    numtochar[0] = 'D';
    numtochar[1] = 'F';
    numtochar[2] = 'J';
    numtochar[3] = 'K';

    while (1) {
        if (!ReadFile(serialHandle, szBuff, number, &dwBytesRead, NULL)) {
            //error occurred. Report to user.
            cout << "Error reading serial!";
        }

//        cout << szBuff << endl;
 //       cout << "READ_END" << endl;
        
        for (int i = 0; i < 4; i++) {
            if (!prevState[i] && szBuff[i] == '1') {
                cout << "KeyDown" << endl;
                INPUT inputs[1] = {};
                ZeroMemory(inputs, sizeof(inputs));

                inputs[0].type = INPUT_KEYBOARD;
                inputs[0].ki.wVk = numtochar[i];

                UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
                if (uSent != ARRAYSIZE(inputs))
                {
                    printf("SendInput failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError()));
                }
            }
            if (prevState[i] && szBuff[i] == '0') {
                cout << "KeyUp" << endl;
                INPUT inputs[1] = {};
                ZeroMemory(inputs, sizeof(inputs));

                inputs[0].type = INPUT_KEYBOARD;
                inputs[0].ki.wVk = numtochar[i];
                inputs[0].ki.dwFlags = KEYEVENTF_KEYUP;

                UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
                if (uSent != ARRAYSIZE(inputs))
                {
                    printf("SendInput failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError()));
                }
            }
            if (szBuff[i] == '0') {
                prevState[i] = false;
            }
            if (szBuff[i] == '1') {
                prevState[i] = true;
            }
        }
        


        Sleep(20);
    }
    CloseHandle(serialHandle);


}


void ShowDesktop()
{
    printf("Sending 'Win-D'\r\n");
    INPUT inputs[4] = {};
    ZeroMemory(inputs, sizeof(inputs));

    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = VK_LWIN;

    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = 'D';

    inputs[2].type = INPUT_KEYBOARD;
    inputs[2].ki.wVk = 'D';
    inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;

    inputs[3].type = INPUT_KEYBOARD;
    inputs[3].ki.wVk = VK_LWIN;
    inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;

    UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
    if (uSent != ARRAYSIZE(inputs))
    {
        printf("SendInput failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError()));
    }
}