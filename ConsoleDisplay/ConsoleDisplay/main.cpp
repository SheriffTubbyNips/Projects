#include <iostream>
#include <cmath>
#include <windows.h>
#include <fstream>

using namespace std;

#define slGrid 30
#define gridOffsetX 5
#define PI 3.1415926

#define SETCURSOR(x,y) SetConsoleCursorPosition(hConsole,{x,y})
#define RadToDeg(x) 180/PI*x
#define DegToRad(x) PI/180*x

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
DWORD mode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ~ENABLE_QUICK_EDIT_MODE;

ofstream writeLog;

char grid[slGrid*slGrid];
short curX,curY;

struct coord
{
    float x = 0;
    float y = 0;
};

struct plot
{
    char graphic = '?';
    int nPoints;
    coord* coords;
};

void displayGrid()
{
    SETCURSOR(0,0);
    for(int a=(slGrid/2)*-1;a<=slGrid/2;a++){
        for(int b=0;b<gridOffsetX;b++){cout << " ";} //Horizontal offset for printing grid
        for(int b=(slGrid/2)*-1;b<=slGrid/2;b++){
            cout << '.' << " ";
        }
        cout << "\n";
    }
}

void displayPlot(plot* p)
{
    for(int a=0;a<=p->nPoints;a++){
        SETCURSOR(round(p->coords[a].x)*2+slGrid+gridOffsetX,(slGrid/2)-round(p->coords[a].y));
        cout << p->graphic;
    }
}

void createLine(plot* line,int X1,int Y1,int X2,int Y2,char g = 'X')
{
    writeLog.open("log.txt",ofstream::app);
    float dX = X2-X1;
    float dY = Y2-Y1;
    int incX = dX!=0 ? abs(dX)/dX : 0;
    int incY = dY!=0 ? abs(dY)/dY : 0;
    float slope; bool slopeUndef = false;
    isinf(dY/dX) ? slopeUndef = true : slope = dY/dX;
    float yInt = Y1-(slope*X1);
    int length = abs(dX)>abs(dY) ? abs(dX) : abs(dY);
    line->nPoints = length;
    line->coords = new coord[length+1];
    line->graphic = g;
    writeLog << "Line created:\n";
    slopeUndef ? writeLog << "Slope = Undefined" : writeLog << "Slope = " << slope;
    writeLog << "\nY Intercept = " << yInt << "\ndx: " << dX << " dy: " << dY << "\n";
    if(slopeUndef == true){ //Technically don't need this case but without it the point logs get messed up.
        for(int a=0;a<=length;a++){
            line->coords[a].x = a*incX+X1;
            line->coords[a].y = a*incY+Y1;
            writeLog << "P" << a << " = [" << line->coords[a].x << "," << line->coords[a].y << "]\n";
        }
    }
    else if(abs(dY)>abs(dX)){
        for(int a=0;a<=length;a++){
            line->coords[a].y = a*incY+Y1;
            line->coords[a].x = (line->coords[a].y-yInt)/slope;
            writeLog << "P" << a << " = [" << line->coords[a].x << "," << line->coords[a].y << "]\n";
        }
    }
    else{
        for(int a=0;a<=length;a++){
            line->coords[a].x = a*incX+X1;
            line->coords[a].y = slope*line->coords[a].x + yInt;
            writeLog << "P" << a << " = [" << line->coords[a].x << "," << line->coords[a].y << "]\n";
        }
    }
    writeLog << "--------------------\n\n";
    writeLog.close();
}

void rotatePlot(plot* p,int deg)
{
    float x,y;
    for(int a=0;a<=p->nPoints;a++){
        x = p->coords[a].x*cos(DegToRad(deg)) - p->coords[a].y*sin(DegToRad(deg));
        y = p->coords[a].x*sin(DegToRad(deg)) + p->coords[a].y*cos(DegToRad(deg));
        p->coords[a].x = x;
        p->coords[a].y = y;
    }
}

void createFilledSquare(plot* square,int X,int Y,int L, int W)
{
    square->nPoints =(L*W)-1;
    square->coords = new coord[L*W];
    for(int a=0;a<L*W;a++){
        square->coords[a].x = X+(a%W);
        square->coords[a].y = Y+(a/L);
    }
}

void MouseEventProc(MOUSE_EVENT_RECORD mer)
{
    #ifndef MOUSE_HWHEELED
    #define MOUSE_HWHEELED 0x0008
    #endif
    if(mer.dwEventFlags == MOUSE_MOVED){
        curX = round( (float)((mer.dwMousePosition.X-gridOffsetX)-slGrid)/2 );
        curY = ((slGrid/2)-mer.dwMousePosition.Y);
        if((curX<=slGrid/2 && curX>=-slGrid/2) && (curY<=slGrid/2 && curY>=-slGrid/2)){
            SETCURSOR(0,slGrid+2);
            cout<< "                               \n                                                 "; //Clear old text before writing new text best I can do because CLS breaks stuff 8^)
            SETCURSOR(0,slGrid+2);
            cout << "Grid position is X:" << curX << " Y:" << curY << "\nAbsolute cursor position is X:" << mer.dwMousePosition.X << " Y:" << mer.dwMousePosition.Y;
        }
    }
}

void GetInput()
{
    INPUT_RECORD inputBuf[100];
    DWORD inputsRead;
    ReadConsoleInput(hInput,inputBuf,100,&inputsRead); //Fill input buffer
        for(int a=0;a<inputsRead;a++){//Loop through buffer
            switch(inputBuf[a].EventType){//Call appropriate event functions
                case MOUSE_EVENT:
                    MouseEventProc(inputBuf[a].Event.MouseEvent);
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
    CONSOLE_CURSOR_INFO curInfo;
    GetConsoleCursorInfo(hConsole,&curInfo);
    curInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole,&curInfo);
    SetConsoleMode(hConsole,mode);
    writeLog.open("Log.txt", ofstream::trunc);
    writeLog.close();

    //Code go below here big man
    plot S1,S2,S3,S4,S5;
    displayGrid();
    createLine(&S1,4,4,4,10,'1');
    displayPlot(&S1);
    Sleep(100);
    int b = 90;
    for(int a=1;a<=360/b;a++){
        rotatePlot(&S1,-b);
        displayPlot(&S1);
        SETCURSOR(0,slGrid+5);
        cout << a;
        Sleep(100);
    }

    while(true){//Input buffer processing
        GetInput();
    }
    return 0;
}
