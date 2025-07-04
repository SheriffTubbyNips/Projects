#pragma once

#include <cmath>
#include <windows.h>
#include <list>

#define BGWHITE BACKGROUND_RED|BACKGROUND_GREEN|BACKGROUND_BLUE
#define FGWHITE FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE

#define RadToDeg(x) 180/3.1415926*x
#define DegToRad(x) 3.1415926/180*x

struct plot
{
    struct coord
    {
        int x = 0;
        int y = 0;
    };
    std::list<coord> coords;

    void createPoint(int x,int y)
    {
        coords.push_back({x,y});
    }

    void createLine(int X1,int Y1,int X2,int Y2)
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
        //Used for a check to optimize vertical line generation.
        bool slopeUndef = false;
        //Either calculates the slope or flags that slope is undefined.
        float slope;
        std::isinf(dY/dX) ? slopeUndef = true : slope = dY/dX;
        float yInt = Y1-(slope*X1);
        int length = abs(dX)>abs(dY) ? abs(dX) : abs(dY);
        if(slopeUndef == true){ //Case for undefined (vertical) slopes
            for(int a = 0; a <= length; a++){
                /*
                Line is vertical just increment Y and leave X unchanged
                and add coordinates to the list.
                */
                coords.push_back({X1,a*incY+Y1});
            }
        }
        else if(abs(dY)>abs(dX)){ //Case for steep slopes
            int x,y;
            for(int a = 0; a <= length; a++){
                //DY is the large value so you can just increment it and solve for X.
                y = a*incY+Y1;
                x = std::round( (y-yInt)/slope );
                //Add coordinates to the list.
                coords.push_back({x,y});
            }
        }
        else{ //Case for gentle slopes
            int x,y;
            for(int a = 0; a <= length; a++){
                //DX is the large value so you can just increment it and solve for Y.
                x = a*incX+X1;
                y = std::round(slope*x+yInt);
                //Add coordinates to the list.
                coords.push_back({x,y});

            }
        }
    }

    void createBox(int X,int Y,int W,int L)
    {
        this->createLine(X,Y,X+W,Y);
        this->createLine(X+W,Y,X+W,Y+L);
        this->createLine(X+W,Y+L,X,Y+L);
        this->createLine(X,Y+L,X,Y);
    }

    void createFilledBox(int X,int Y,int W,int L)
    {
        for(int a = 0; a < L; a++){
            this->createLine(X,Y+a,X+W-1,Y+a);
        }
    }

    void Translate(int x,int y)
    {
        /*
        Pretty simple. Iterate through each coordinate in the list and add the
        distance it is being moved by to the old values.
        */
        for(std::list<coord>::iterator it = coords.begin();it != coords.end(); it++)
            {
                it->x += x;
                it->y += y;
            }
    }

    /*
    The Rotate function rounds the final value.
    Multiple rotations upon the same plot can
    cause rounding errors.
    */
    void Rotate(int deg,int originX = 0,int originY = 0)
    {
        /*
        Temp variables are required for rotation because
        the new Y coordinate calculation has to use the old
        X coordinate. So you can't overwrite it immediately
        or else the rotation will not calculate properly.
        */
        float x,y;
        // Iterate through each coordinate in the list and calculate their rotated position
        for(std::list<coord>::iterator it = coords.begin();it!=coords.end();it++)
        {
            // Translate origin
            it->x -= originX;
            it->y -= originY;
            // Rotate
            x = it->x*cos(DegToRad(deg)) - it->y*sin( DegToRad(deg));
            y = it->x*sin(DegToRad(deg)) + it->y*cos( DegToRad(deg));
            // Translate origin back
            x += originX;
            y += originY;
            // Round coordinates and push to list
            it->x = std::round(x);
            it->y = std::round(y);
        }
    }
};

