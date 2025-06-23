#pragma once

#include <windows.h>
#include <bitset>
#include <fstream>

HANDLE conOut = GetStdHandle(STD_OUTPUT_HANDLE);

struct Frame
{
    std::fstream fs;
    int xMax,yMax;
    int posX = 0;
    int posY = 0;
    std::bitset<4>** data;


    Frame(int x,int y,int sizeX,int sizeY) : posX(x),posY(y),xMax(sizeX),yMax(sizeY) //Hacky constructor because the proper way still confuses me
    {
        data = new std::bitset<4>*[yMax];
        for(int a = 0;a < yMax;a++)
        {
            data[a] = new std::bitset<4>[xMax];
        }
    }

    void Draw(int x,int y,int binary)
    {
        if( y >= 0 && y < yMax && x >= 0 && x < xMax ){data[y][x] = binary;}
    }

    void Display()
    {
        for(int y=0;y<yMax;y++)
        {
            std::cout << "\n";
            for(int x=0;x<xMax;x++)
            {
                SetConsoleCursorPosition(conOut,{(x+posX)*2,posY+y});
                if(data[y][x][0])
                {
                    WORD colour = 0;
                    if(data[y][x][1]){colour += BACKGROUND_RED;}
                    if(data[y][x][2]){colour += BACKGROUND_GREEN;}
                    if(data[y][x][3]){colour += BACKGROUND_BLUE;}
                    SetConsoleTextAttribute(conOut,colour);
                    std::cout << "  ";
                }
            }
        }
    }

    void Save(std::string path = "default")
    {
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
        unsigned char bit;
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
};

void ClearDisplay()
{
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(conOut,&info);
    SetConsoleCursorPosition(conOut,{0,0});
    for(int a = 0;a < info.srWindow.Right*info.srWindow.Bottom;a++){std::cout << " ";}
}
