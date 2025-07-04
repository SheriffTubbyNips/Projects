#pragma once

#include <windows.h>
#include <list>

HANDLE conOut = GetStdHandle(STD_OUTPUT_HANDLE);

struct Frame
{
    #define sX 30
    #define sY 30
    bool frameData[sY][sX][5]; //Container for frame bits
    //Data formats as (Transparency,Intensity,Blue,Green,Red)
    WORD colour;

    void Clear() //Sets all frame bits to 0.
    {
        for(int a = 0;a < sY;a++)
        {
            for(int b = 0;b < sX;b++)
            {
                for(int c = 0;c < 5;c++)
                {
                    frameData[a][b][c] = false;
                }
            }
        }
    }

    Frame() //Constructor. Sets frame data bits to 0;
    {
        this->Clear();
    }

    void Display()//Definitely does not display the frame
    {
        for(int y = 0;y < sY;y++)//Scan through frame columns (y axis)
        {
            for(int x = 0;x < sX;x++)//Scan through the row of each y axis level
            {
                colour = 0;
                SetConsoleCursorPosition(conOut,{(short)(x*2),(short)y});
                if(!frameData[y][x][4])
                {
                    if(frameData[y][x][0]){colour += BACKGROUND_RED;}
                    if(frameData[y][x][1]){colour += BACKGROUND_GREEN;}
                    if(frameData[y][x][2]){colour += BACKGROUND_BLUE;}
                    if(frameData[y][x][3]){colour += BACKGROUND_INTENSITY;}
                    SetConsoleTextAttribute(conOut,colour);
                    std::cout << "  ";
                }
                else{}
            }
        }
    }
};
