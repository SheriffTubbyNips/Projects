#include <iostream>
#include <cmath>
#include <windows.h>

using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);

#define SETCURSOR(x,y) SetConsoleCursorPosition(hConsole,{x,y})
#define slGrid 30
#define gridOffsetX 5

char grid[slGrid*slGrid];
short curX,curY;

struct coord{
    int x;
    int y;
};

struct plot{
    char graphic = 'X';
    int layer = 0;
    int nPoints;
    coord* coords;
};

void displayGrid(){
    SETCURSOR(0,0);
    for(int a=(slGrid/2)*-1;a<=slGrid/2;a++){
        for(int b=0;b<gridOffsetX;b++){cout << " ";} //Horizontal offset for printing grid
        for(int b=(slGrid/2)*-1;b<=slGrid/2;b++){
            cout << '.' << " ";
        }
        cout << "\n";
    }
}

void displayPlot(plot* p){
    for(int a=0;a<=p->nPoints;a++){
        SETCURSOR(slGrid+gridOffsetX+(p->coords[a].x*2),(slGrid/2)-p->coords[a].y);
        cout << p->graphic;
        SETCURSOR(70,a+1);
        cout << a << " : " << p->coords[a].x << " : " << p->coords[a].y;
    }
}

void createLine(plot* line,int X1,int Y1,int X2,int Y2){
    float dX = X2-X1; float dY = Y2-Y1;
    float slope = dY/dX;
    float yInt = Y1-(slope*X1);
    int length;
    if(abs(dY)>abs(dX)){
        length = abs(dY);
        line->nPoints = length;
        line->coords = new coord[length+1];
        for(int a=0;a<=length;a++){
            line->coords[a].y = Y1+(a*abs(dY)/dY);
            line->coords[a].x = round( (line->coords[a].y-yInt)/slope );
        }
    }
    else{
        length = abs(dX);
        line->nPoints = length;
        line->coords = new coord[length+1];
        for(int a=0;a<=length;a++){
            line->coords[a].x = X1+(a*(abs(dX)/dX));
            line->coords[a].y = round( slope*line->coords[a].x + yInt );
        }
    }
}

void createFilledSquare(plot* square,int X,int Y,int L, int W){
    square->nPoints =(L*W)-1;
    square->coords = new coord[L*W];
    for(int a=0;a<L*W;a++){
        square->coords[a].x = X+(a%W);
        square->coords[a].y = Y+(a/L);
    }
}

void MouseEventProc(MOUSE_EVENT_RECORD mer){
    #ifndef MOUSE_HWHEELED
    #define MOUSE_HWHEELED 0x0008
    #endif
    if(mer.dwEventFlags == MOUSE_MOVED){
        curX = round( (float)((mer.dwMousePosition.X-gridOffsetX)-slGrid)/2 );
        curY = ((-slGrid/2)+mer.dwMousePosition.Y)*-1;
        if((curX<=slGrid/2 && curX>=-slGrid/2) && (curY<=slGrid/2 && curY>=-slGrid/2)){
            SETCURSOR(0,slGrid+2);
            cout<< "                               \n                                                 "; //Clear old text before writing new text best I can do because CLS breaks stuff 8^)
            SETCURSOR(0,slGrid+2);
            cout << "Grid position is X:" << curX << " Y:" << curY << "\nAbsolute cursor position is X:" << mer.dwMousePosition.X << " Y:" << mer.dwMousePosition.Y;
        }
    }
}


void KeyEventProc(KEY_EVENT_RECORD ker){
}

int main()
{
    INPUT_RECORD inputBuf[100];
    DWORD inputsRead, mode;
    mode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ~ENABLE_QUICK_EDIT_MODE;
    SetConsoleMode(hConsole,mode);
    displayGrid();


    //Code go below here big man
    plot S1;
    createFilledSquare(&S1,5,5,10,10);
    displayPlot(&S1);


    while(true){//Input buffer processing
        ReadConsoleInput(hInput,inputBuf,100,&inputsRead); //Fill input buffer
        for(int a=0;a<inputsRead;a++){//Loop through buffer
            switch(inputBuf[a].EventType){//Call appropriate event functions
                case MOUSE_EVENT:
                    MouseEventProc(inputBuf[a].Event.MouseEvent);
                break;
                case KEY_EVENT:
                    KeyEventProc(inputBuf[a].Event.KeyEvent);
                break;
                case WINDOW_BUFFER_SIZE_EVENT:
                break;
                case MENU_EVENT:
                break;
                case FOCUS_EVENT:
                break;
            }
        }
    }//End of input processing

    return 0;
}
