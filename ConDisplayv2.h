#pragma once

#include <windows.h>
#include <bitset>
#include <fstream>
#include <cmath>


HANDLE conOut = GetStdHandle(STD_OUTPUT_HANDLE);

struct Image
{
    int xMax,yMax;
    std::string name;
    std::fstream fs;
    std::bitset<4>** data;


    Image(int sizeX,int sizeY,std::string n = "default") : xMax(sizeX),yMax(sizeY), name(n) //Hacky constructor because the proper way still confuses me
    {
        data = new std::bitset<4>*[yMax];
        for(int a = 0;a < yMax;a++)
        {
            data[a] = new std::bitset<4>[xMax];
        }
    }

    void Display(int posX = 0, int posY = 0)
    {
        CONSOLE_SCREEN_BUFFER_INFO info;
        GetConsoleScreenBufferInfo(conOut,&info);
        for(int y = 0; y < yMax; y++)
        {
            for(int x = 0; x < xMax && x+posX < info.dwSize.X; x++)
            {
                SetConsoleCursorPosition(conOut,{short( x+posX ),short( posY+y )});
                if(data[y][x][0])
                {
                    WORD colour = 0;
                    if(data[y][x][1]){colour += BACKGROUND_RED;}
                    if(data[y][x][2]){colour += BACKGROUND_GREEN;}
                    if(data[y][x][3]){colour += BACKGROUND_BLUE;}
                    SetConsoleTextAttribute(conOut,colour);
                    std::cout << " ";
                }
            }
        }
        SetConsoleTextAttribute(conOut,0);
    }

    void Save(std::string path = "default")
    {
        if(path == "default"){path = name;}
        path += ".succ";
        fs.open(path,std::ios::out|std::ios::trunc|std::ios::binary);
        for(int y=0;y<yMax;y++)
        {
            for(int x=0;x<xMax;x++)
            {
                for(int d=0;d<3;d++)
                {
                    fs.put(data[y][x][d]);
                }
            }
        }
        fs.close();
    }

    void Load(std::string path)
    {
        fs.open(path,std::ios::in|std::ios::binary);
        for(int y=0;y<yMax;y++)
        {
            for(int x=0;x<xMax;x++)
            {
                for(int d=0;d<3;d++)
                {
                    data[y][x][d] = fs.get();
                }
            }
        }
        fs.close();
    }

    void Draw(int x,int y,int binary)
    {
        //Check that X/Y is in-bounds before drawing.
        if( y >= 0 && y < yMax && x >= 0 && x < xMax ){data[y][x] = binary;}
    }

    void DrawLine(int X1,int Y1,int X2,int Y2,int colour)
    {
        //Get Delta X and Y
        float dX = X2-X1;
        float dY = Y2-Y1;
        /*
        The largest axis does not need to be calculated because the line always has to span
        between the start and end points. incX/Y is used to plot each point of the line
        along this known axis. incX/Y can be 0 for vertical/horizontal lines. Otherwise
        it is 1 or -1. The other axis needs to be solved for in order to generate the correct
        slope for the line.
        */
        int incX = dX != 0 ? abs(dX)/dX : 0;
        int incY = dY != 0 ? abs(dY)/dY : 0;
        //Either calculates the slope or flags that slope is undefined.
        bool slopeUndef = false;
        float slope;
        if(dX == 0){slopeUndef = true;}
        else{slope = dY/dX;}
        //Get Y intercept
        float yInt = Y1-(slope*X1);
        /*
        Due to limitations of rasterization line length is not accurate
        to Euclidian distance. Since the line has to extend from the start
        to end points you can assume the length is the higher delta value.
        */
        int length = abs(dX)>abs(dY) ? abs(dX) : abs(dY);
        /*
        Case for vertical-line optimization also use this
        for single-point lines since it works for that too.
        */
        if(slopeUndef == true){
            for(int a = 0; a <= length; a++){
                /*
                Line is vertical just increment Y and leave X unchanged
                */
                Draw(X1,a*incY+Y1,colour);
            }
        }
        else if(abs(dY)>abs(dX)){ //Case for steep slopes
            int x,y;
            for(int a = 0; a <= length; a++){
                //DY is the large value so you can just increment it and solve for X.
                y = a*incY+Y1;
                x = round( (y-yInt)/slope );
                Draw(x,y,colour);
            }
        }
        else if(abs(dX)>abs(dY)){ //Case for gentle slopes
            int x,y;
            for(int a = 0; a <= length; a++){
                //DX is the large value so you can just increment it and solve for Y.
                x = a*incX+X1;
                y = round(slope*x+yInt);
                Draw(x,y,colour);
            }
        }
    }

    void DrawBox(int X,int Y,int W,int L,int colour)
    {
        /*
        A bit semantic and not needed. But typically a shape is thought of
        in terms of where it's placed and the length/width. But a line
        meanwhile is plotted from X1/Y1 to X2/Y2. The shape generators
        use the line function. So without subtracting 1 a box with length/width of
        1 would show as having a length/width of 2.
        */
        W -= 1;
        L -= 1;
        DrawLine(X,Y,X+W,Y,colour);
        DrawLine(X+W,Y,X+W,Y+L,colour);
        DrawLine(X+W,Y+L,X,Y+L,colour);
        DrawLine(X,Y+L,X,Y,colour);
    }

    void DrawFilledBox(int X,int Y,int W,int L,int colour)
    {
        W -= 1;
        L -= 1;
        for(int a = 0; a <= L; a++){
            DrawLine(X,Y+a,X+W,Y+a,colour);
        }
    }
};

void ClearDisplay()
    {
        CONSOLE_SCREEN_BUFFER_INFO info;
        GetConsoleScreenBufferInfo(conOut,&info);
        SetConsoleCursorPosition(conOut,{0,0});
        for(int a = 0;a < info.dwSize.X*info.dwSize.Y;a++){std::cout << " ";}
    }



