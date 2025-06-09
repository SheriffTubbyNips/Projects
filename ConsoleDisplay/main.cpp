#include <iostream>
#include <cmath>
#include <windows.h>
#include <fstream>
#include <list>

using namespace std;

#define PI 3.1415926

#define TEXTDEFAULT FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED
#define BGWHITE BACKGROUND_RED|BACKGROUND_GREEN|BACKGROUND_BLUE

#define SETCURSOR(x,y) SetConsoleCursorPosition(hConsole,{(short)(x),(short)(y)})
#define RadToDeg(x) 180/PI*x
#define DegToRad(x) PI/180*x

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
DWORD mode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ~ENABLE_QUICK_EDIT_MODE;

int OFFSET_X = 5;
int OFFSET_Y = 2;
int SLGRID = 30;
int PIXELWIDTH = 2;

short curX,curY;

struct coord
{
    float x = 0;
    float y = 0;
};

struct plot
{
    WORD colour = FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED;
    char graphic = '?';
    int nPoints = 0; //This can prob be considered depreciated. Just get the size of the list lol.
    list<coord> coords;
};

void displayPlot(plot* p)
{
    for(coord c:p->coords){
        SETCURSOR(round(c.x)*2+SLGRID+OFFSET_X,SLGRID/2-round(c.y)+OFFSET_Y);
        SetConsoleTextAttribute(hConsole,p->colour);
        for(int a=1;a<=PIXELWIDTH;a++){
                cout << p->graphic;
        }
        SetConsoleTextAttribute(hConsole,TEXTDEFAULT);
    }
}

void rotatePlot(plot* p,int deg,int originX = 0,int originY = 0)
{
    float x,y;
    list<coord> newCoords;
    coord c;
    for(list<coord>::iterator i=p->coords.begin();i!=p->coords.end();i++){
        c = *i;
        x = (c.x-originX)*cos(DegToRad(deg)) - (c.y-originY)*sin(DegToRad(deg));
        y = (c.x-originX)*sin(DegToRad(deg)) + (c.y-originY)*cos(DegToRad(deg));
        c.x = x+originX;
        c.y = y+originY;
        newCoords.push_back(c);
    }
    p->coords = newCoords;
}

void createLine(plot* p,int X1,int Y1,int X2,int Y2,char g='X')
{
    float dX = X2-X1;
    float dY = Y2-Y1;
    int incX = dX != 0 ? abs(dX)/dX : 0;
    int incY = dY != 0 ? abs(dY)/dY : 0;
    float slope;
    bool slopeUndef = false;
    isinf(dY/dX) ? slopeUndef = true : slope = dY/dX;
    float yInt = Y1-(slope*X1);
    int length = abs(dX)>abs(dY) ? abs(dX) : abs(dY);
    coord c;
    p->nPoints = length;
    p->graphic = g;
    if(slopeUndef == true){ //Technically don't need this case but without it the point logs get messed up.
        for(int a = 0; a <= length; a++){
            c.x = a*incX+X1;
            c.y = a*incY+Y1;
            p->coords.push_back(c);
        }
    }
    else if(abs(dY)>abs(dX)){
        for(int a = 0; a <= length; a++){
            c.y = a*incY+Y1;
            c.x = (c.y-yInt)/slope;
            p->coords.push_back(c);
        }
    }
    else{
        for(int a = 0; a <= length; a++){
            c.x = a*incX+X1;
            c.y = slope*c.x + yInt;
            p->coords.push_back(c);
        }
    }
}

void createBox(plot* p,int x1,int y1,int x2,int y2,char g='X')
{
    createLine(p,x1,y1,x2,y1,g);
    createLine(p,x1,y2,x2,y2,g);
    createLine(p,x1,y1,x1,y2,g);
    createLine(p,x2,y1,x2,y2,g);
}

void createFilledBox(plot* p,int x1,int y1,int x2,int y2,char g='X')
{
    for(int a = 0; a <= y2-y1; a++){
        createLine(p,x1,y1+a,x2,y1+a,g);
    }
}

void createCircle(plot* p,int x,int y,int r,char g = 'X')
{
    list<coord> rasterCoords;
    coord point = {(float)x,(float)(y+r)};
    p->coords.push_back(point);
    p->graphic = g;
    for(int a = 0; a < 360; a++){
        if(round(p->coords.back().x) != round(rasterCoords.back().x) || round(p->coords.back().y) != round(rasterCoords.back().y)){
            rasterCoords.push_back(p->coords.back());
        }
        rotatePlot(p,-1,x,y);
    }
    rasterCoords.pop_back(); //Revolving creates a redundant start-point. Remove it here.
    p->coords = rasterCoords;
}

void MouseEventProc(MOUSE_EVENT_RECORD mer)
{
    #ifndef MOUSE_HWHEELED
    #define MOUSE_HWHEELED 0x0008
    #endif
    if(mer.dwEventFlags == MOUSE_MOVED){
        curX = round( (float)(mer.dwMousePosition.X-OFFSET_X-SLGRID)/2 );
        curY = SLGRID/2-mer.dwMousePosition.Y+OFFSET_Y;
        if((curX<=SLGRID/2 && curX>=-SLGRID/2) && (curY<=SLGRID/2 && curY>=-SLGRID/2)){
            SETCURSOR(0,SLGRID+OFFSET_Y+2);
            cout<< "                               \n                                                 "; //Clear old text before writing new text best I can do because CLS breaks stuff 8^)
            SETCURSOR(0,SLGRID+OFFSET_Y+2);
            cout << "Grid position is X:" << curX << " Y:" << curY << "\nAbsolute cursor position is X:" << mer.dwMousePosition.X << " Y:" << mer.dwMousePosition.Y;
        }
    }
}

void GetInput()
{
    INPUT_RECORD inputBuf[100];
    DWORD inputsRead;
    ReadConsoleInput(hInput,inputBuf,100,&inputsRead); //Fill input buffer
        for(int a=0;a<(int)inputsRead;a++){//Loop through buffer
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

    plot grid;
    createFilledBox(&grid,-SLGRID/2,-SLGRID/2,SLGRID/2,SLGRID/2,'.');
    plot axisLine;
    axisLine.colour = FOREGROUND_RED;
    createLine(&axisLine,-SLGRID/2,0,SLGRID/2,0,'.');
    createLine(&axisLine,0,-SLGRID/2,0,SLGRID/2,'.');
    displayPlot(&grid);
    displayPlot(&axisLine);
    //Code go below here big man
    plot S1;
    S1.colour = BGWHITE;
    createLine(&S1,5,5,5,10,' ');
    for(int a=0;a<=360;a++){
        rotatePlot(&S1,-1,5,5);
        displayPlot(&S1);
    }



    while(true){//Input buffer processing
        GetInput();
    }
    return 0;
}
