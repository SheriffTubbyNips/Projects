#include <iostream>
#include "E:\\Documents\\CodeStuff\\C++\\Headers\\ConDisplayv2.h"
#include "E:\\Documents\\CodeStuff\\C++\\Headers\\TubbyRaster.h"

using namespace std;

HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
int colour = 0b0101;
Frame* activeFrame;
plot* imageBorder;
plot topHeader;

void NewIMG()
{
    string exit;
    string name = "default";
    int x,y;
    while(true)
    {
        ClearDisplay();
        SetConsoleCursorPosition(conOut,{0,0});
        cout << "Enter two numbers for the size of image to be created (X Y)\n";
        cin >> x >> y;
        cout << "\nEnter a name for this image\n";
        cin >> name;
        if(cin.fail())
        {
            cin.clear();
            cin.ignore(100,'\n');
        }
        cout << "\nImage dimensions will be " << x << "x" << y << "\nAccept? (Y/N)";
        cin >> exit;
        if(exit == "y"){break;}
    }
    activeFrame = new Frame(1,5,x,y);
    imageBorder = new plot;
    imageBorder->createBox(0,3,x+1,y+1);
    ClearDisplay();
    imageBorder->displayPlot();
    topHeader.displayPlot();
    SetConsoleCursorPosition(conOut,{0,0});
    SetConsoleTextAttribute(conOut,BGWHITE);
    cout << name << "|Press number keys to change colour";
}

void RefreshDisplay()
{
    activeFrame->Display();
}

void MouseEventProc(MOUSE_EVENT_RECORD m)
{
    #ifndef MOUSE_HWHEELED
    #define MOUSE_HWHEELED 0x0008
    #endif
    if(m.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED){
        activeFrame->Draw((m.dwMousePosition.X/2)-activeFrame->posX,m.dwMousePosition.Y-activeFrame->posY,colour);
        RefreshDisplay();
    }
}

void KeyEventProc(KEY_EVENT_RECORD k){
    switch(k.wVirtualKeyCode)
    {
        case 'S':
            activeFrame->Save();
        break;
        case 'L':
        break;
        case 'N':
            NewIMG();
        break;
        case '1':
            colour = 0b0011; //Red
        break;
        case '2':
            colour = 0b0101; //Green
        break;
        case '3':
            colour = 0b1001; //Blue
        break;
        case '4':
            colour = 0b0111; //Yellow
        break;
        case '5':
            colour = 0b1101; //Cyan
        break;
        case '6':
            colour = 0b1011; //Purple
        break;
        case '7':
            colour = 0b1111; //White
        break;
        case '8':
            colour = 0b0001; //Black
        break;
    }
}

void GetInput(){ //Does what it says on the tin
    INPUT_RECORD inputBuffer[1];
    DWORD nRead;
    ReadConsoleInput(hInput,inputBuffer,1,&nRead);
    for(int a=0;a<(int)nRead;a++){
        switch(inputBuffer[a].EventType){//Call appropriate event functions
                case MOUSE_EVENT:
                    MouseEventProc(inputBuffer[a].Event.MouseEvent);
                break;
                case KEY_EVENT:
                    KeyEventProc(inputBuffer[a].Event.KeyEvent);
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
    //IDFK optimize this later it works rn
    //Seriously remember to just get this working and focus on cleaning it up later
    //Better to pad out your portfolio with literally anything than obsess over nitpicks
    CONSOLE_CURSOR_INFO curInfo;
    GetConsoleCursorInfo(hConsole,&curInfo);
    curInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole,&curInfo);
    topHeader.createFilledBox(0,0,30,2);
    NewIMG();
    RefreshDisplay();

    while(true)
    {
        GetInput();
    }
    return 0;
}
