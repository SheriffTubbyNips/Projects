#include <iostream>
#include "E:\\Documents\\CodeStuff\\C++\\Headers\\ConDisplayv2.h"

using namespace std;

static WORD BACKGROUND_WHITE = BACKGROUND_BLUE|BACKGROUND_RED|BACKGROUND_GREEN;
HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
int colour = 0b0101;
int canvasX = 13;
int canvasY = 1;

Image* canvas;
Image* canvasBorder;
Image UI(12,30,"UI");

void (*MouseEventProc)(MOUSE_EVENT_RECORD m);
void (*KeyEventProc)(KEY_EVENT_RECORD k);

void SetCursor(int x,int y)
{
    SetConsoleCursorPosition(conOut,{(short)x,(short)y});
}

void Print(string text,int x,int y,WORD c)
{
    SetConsoleCursorPosition(conOut,{(short)x,(short)y});
    SetConsoleTextAttribute(conOut,c);
    cout << text;
    SetConsoleTextAttribute(conOut,0);
}

void RefreshDisplay()
{
    //Redraw image canvas
    canvas->Display(canvasX,canvasY);

    //Redraw UI stuff
    canvasBorder->Display(canvasX-1,canvasY-1);
    UI.Display();
    SetCursor(0,0);
    Print("File:" + canvas->name,0,0,BACKGROUND_WHITE);
}

void NewIMG()
{
    string name;
    string exit;
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
    delete canvas;
    delete canvasBorder;
    canvas = new Image(x,y,name);
    canvasBorder = new Image(x+2,y+2);
    canvasBorder->DrawBox(0,0,x+2,y+2,0b1111);
    ClearDisplay();
}

void Draw(MOUSE_EVENT_RECORD m)
{
    if(m.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED){
        canvas->Draw( m.dwMousePosition.X - canvasX, m.dwMousePosition.Y - canvasY, colour );
        RefreshDisplay();
    }
}

void ColourMenu(MOUSE_EVENT_RECORD m)
{

}

void KeyDefault(KEY_EVENT_RECORD k){
    switch(k.wVirtualKeyCode)
    {
        case 'S':
            canvas->Save();
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
                    ClearDisplay();
                    RefreshDisplay();
                break;
                case MENU_EVENT:
                break;
                case FOCUS_EVENT:
                break;
        }
    }
}

struct ScreenHandler
{
    struct Data
    {
        char character = ' ';
        WORD colour = 0;
    };
    int sizeX;
    int sizeY;

    Data** screenBuffer;

    ScreenHandler(int x, int y) : sizeX(x), sizeY(y)
    {
        screenBuffer = new Data*[sizeY];
        for(int i = 0; i < sizeY; i++)
        {
            screenBuffer[i] = new Data[sizeX];
        }
    }

    void Display()
    {
        for(int y = 0; y < sizeY; y++)
        {
            for(int x = 0; x < sizeX; x++)
            {
                SetConsoleTextAttribute(conOut,screenBuffer[y][x].colour);
                cout << screenBuffer[y][x].character;
            }
        }
    }


};

int main()
{
    //IDFK optimize this later it works rn
    //Seriously remember to just get this working and focus on cleaning it up later
    //Better to pad out your portfolio with literally anything than obsess over nitpicks

    //Buncha initialization stuff
    CONSOLE_CURSOR_INFO curInfo;
    GetConsoleCursorInfo(conOut,&curInfo);
    curInfo.bVisible = FALSE;
    SetConsoleCursorInfo(conOut,&curInfo);

    MouseEventProc = Draw;
    ScreenHandler S;
    S.Display();

    while(true)
    {
        GetInput();
    }
    return 0;
}
