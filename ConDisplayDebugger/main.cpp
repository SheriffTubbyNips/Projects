#include <iostream>
#include "E:\\Documents\\CodeStuff\\C++\\Headers\\ConDisplay.h"

using namespace std;

HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);

void MouseEventProc(MOUSE_EVENT_RECORD m){ //Handle mouse inputs
    #ifndef MOUSE_HWHEELED
    #define MOUSE_HWHEELED 0x0008
    #endif
    if(m.dwEventFlags == MOUSE_MOVED){
        SETCURSOR(0,30);
        cout << "                          ";
        SETCURSOR(0,30);
        cout << "X:" << m.dwMousePosition.X/2 << " Y:" << m.dwMousePosition.Y;
    }
}

void GetInput(){ //Does what it says on the tin
    INPUT_RECORD inputBuffer[100];
    DWORD nRead;
    ReadConsoleInput(hInput,inputBuffer,100,&nRead);
    for(int a=0;a<(int)nRead;a++){
        switch(inputBuffer[a].EventType){//Call appropriate event functions
                case MOUSE_EVENT:
                    MouseEventProc(inputBuffer[a].Event.MouseEvent);
                break;
                case KEY_EVENT:
                break;
                case WINDOW_BUFFER_SIZE_EVENT:
                break;
                case MENU_EVENT:
                break;
                case FOCUS_EVENT:
                break;
        }
    }
}

int main()
{
    //Buncha BULLSHIT to set up the console window
    CONSOLE_CURSOR_INFO curInfo;
    GetConsoleCursorInfo(hConsole,&curInfo);
    curInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole,&curInfo);
    SetConsoleMode(hConsole,mode);

    //Actual code testing
    frame f;
    plot p;
    p.createLine(15,15,10,15);
    for(int a=0;a<=10;a++)
    {
        p.rotatePlot(-45,15,15);
        clearDisplay();
        p.displayPlot();
    }







    while(true){
        GetInput();
    }
}
