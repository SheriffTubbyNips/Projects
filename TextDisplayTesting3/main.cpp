#include <iostream>
#include <cmath>
#include <windows.h>

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);

#define SETCURSOR(x,y) SetConsoleCursorPosition(hConsole,{x,y})
#define slGrid 30

using namespace std;

char grid[slGrid][slGrid];
short curX,curY;

struct coord{
    int x;
    int y;
};

struct plot{
    char graphic;
    int layer = 0;
    int nPoints;
    coord* coords;
};

void displayGrid(){
    SETCURSOR(0,0);
    for(int a=(slGrid/2)*-1;a<=slGrid/2;a++){
        for(int b=(slGrid/2)*-1;b<=slGrid/2;b++){
            cout << " " << '.';
        }
        cout << "\n";
    }
}

void createLine(plot* line,int X, int Y,int L){
    line->nPoints = L;
    line->coords = new coord[L];
    for(int a=0;a<L;a++){
        line->coords[a].x = X+a;
        line->coords[a].y = Y;
    }
}

void createLine2(plot* line, int X1,int Y1,int X2,int Y2){ //Prob should rewrite this. Remember to try getting x-intercept to plot better lines
    // Solve for y=mx+b that should be easy right IDIOT!?!?
    float dX = X2 - X1;
    float dY = Y2 - Y1;
    float M = dY/dX;
    float B = Y1 - (M*X1);
    float L = floor( hypot( abs(dX),abs(dY) ) );
    float dir = dX/(abs(dX));
    cout << "L: " << L << " M: " << M << " B: " << B << "\ndX:dY = " << dX << ":" << dY << "\n+/-X: " << dir << "\n";
    line->nPoints = (int)abs(dX+1);
    line->coords = new coord[(int)abs(dX+1)];
    for(int a=0;a<=abs(dX+1);a++){
        line->coords[a].x = X1+(a*dir);
        line->coords[a].y = floor( M*(line->coords[a].x)+B );
    }
}

void createLine3(plot* line,int X1,int Y1,int X2,int Y2){
    //y=m*x+b
    //3 = 1.42857 * x + 0.142857
    float dX = X2-X1;
    float dY = Y2-Y1;
    float M = dY/dX;
    float yInt = Y1 - (M*X1);
    //float xInt =
    int xDir = dX/(abs(dX));
    int yDir = dY/(abs(dY));
    cout << " M: " << M << " Y Intercept: " << yInt << "\ndX:dY = " << dX << ":" << dY << "\n+/-X: " << xDir << "\n";
    if(dX>=dY){
        line->nPoints = (int)abs(dX);
        line->coords = new coord[(int)abs(dX)+1]; //Gotta +1 this because you take x1,y1 as your 0,0 point and include it so (EG:) 0 to 4 including 0 is 5 but if you declare it as 4 the array will be 0 to 3
        for(int a=0;a<=abs(dX);a++){
            line->coords[a].x = X1+(a*xDir);
            line->coords[a].y = round( M*(line->coords[a].x)+yInt );
        }
    }
    else{
        line->nPoints = (int)abs(dY);
        line->coords = new coord[(int)abs(dY)+1];
        for(int a=0;a<=abs(dY);a++){
            line->coords[a].y = Y1+(a*yDir);
            line->coords[a].x = round( (line->coords[a].y/M)-yInt);
        }
    }

}

void createFilledSquare(plot* square,int X,int Y,int L, int W,char graphic){
    square->nPoints = abs(L*W);
    square->coords = new coord[abs(L*W)];
    for(int a=Y;a<=L;a++){
        for(int b=X;b<=W;b++){
            square->graphic = '.';
            square->coords[b+(a*W)].x = X+b;
            square->coords[b+(a*W)].y = Y+a;
        }
    }
}

void MouseEventProc(MOUSE_EVENT_RECORD mer){
    #ifndef MOUSE_HWHEELED
    #define MOUSE_HWHEELED 0x0008
    #endif
    if(mer.dwEventFlags == MOUSE_MOVED){
        curX = (-slGrid/2-1)+round((float)mer.dwMousePosition.X/2);
        curY = ((-slGrid/2)+mer.dwMousePosition.Y)*-1;
        SETCURSOR(0,slGrid+2);
        cout<< "                        "; //Clear old text before writing new text best I can do because CLS breaks stuff 8^)
        SETCURSOR(0,slGrid+2);
        cout<< "X:" << curX << " Y:" << curY;
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
    //Code go below here big man


    displayGrid();
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
