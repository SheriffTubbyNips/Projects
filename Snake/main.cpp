#include <iostream>
#include <vector>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

#define SETCURSOR(x,y) SetConsoleCursorPosition(hConsole,{(short)x,(short)y})
#define KBINPUT(x) GetAsyncKeyState(x) & 0x8000

int main()
{
    //Buncha Junk to manage the game window and console
    HWND hWindow = GetConsoleWindow();
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetWindowPos(hWindow,HWND_TOP,0,0,500,500,SWP_NOMOVE); //Need to have this before getting console screen buffer into so that the edge detect works right
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo; GetConsoleScreenBufferInfo(hConsole,&consoleInfo);

    struct snake{ //Struct for game objects
        int x; int y;
    }segment,food; //Instance two structs to act as placeholder for new snake segments, another for scoring-objectives (food)

    int startSize = 3; int startX = 10; int startY = 10;//Starting coordinates and length for snake
    int xMax = consoleInfo.srWindow.Right; int yMax = consoleInfo.srWindow.Bottom; //Get dimensions for window, to do edge-detection for lose conditions
    int menuCursor = 1;
    int frameCounter = 0;
    int gameSpeed = 10; //Lower values give higher speed. 10 is already pretty slow. Probably don't need to set higher.
    char direction = 'R';
    char playAgain;
    bool lost = false;
    while(!lost){
        while(true){
            system("CLS");//Refresh
            SETCURSOR(0,0);
            cout << "Welcome to snake bruh press enter to start\nUse (WASD) to control.\n\nDifficulty Select: (Effects snake move speed) \nEasy: \nMedium: \nHard: ";
            if(KBINPUT('W') && menuCursor != 1){menuCursor--;}
            if(KBINPUT('S') && menuCursor != 3){menuCursor++;}
            if(KBINPUT(VK_RETURN)){
                gameSpeed = (10/menuCursor);
                break;
            }
            SETCURSOR(8,(menuCursor+3));
            cout << "X";
            Sleep(100);
        }

        //Pre-loop setup stuff
        vector<snake> player; //Create a list of snake segments to represent the player
        segment.x = startX; segment.y = startY; //Assign starting coordinates to placeholder segment in order to generate snake head
        player.push_back(segment); //Add snake head to player list
        //Loop to create additional starting snake segments
        for(int a = 0;a<startSize-1;a++){//Start size is -1 to account for player head
            segment.x = player[a].x - 1; //Player[a].x is -1 in order to place each starting segment to the left of the one created before it
            segment.y = player[a].y;
            player.push_back(segment); //Add placeholder segment values to player list
        }
        srand(time(NULL));//Set RNG seed
        food.x = rand()%(xMax-1)+1; food.y = rand()%(yMax-1)+1;//Generate starting food object

        while(!lost){ //Gameplay loop. Runs until game lose condition is met
            //Bunca IF checks for keyboard input also prevents making a 180 turn as this would cause an instant loss and is generally against the SPIRIT of snake as a game 8^)
            if(KBINPUT('W') && direction != 'D'){direction = 'U';}//Move input (Up)
            if(KBINPUT('S') && direction != 'U'){direction = 'D';}//Move input (Down)
            if(KBINPUT('A') && direction != 'R'){direction = 'L';}//Move input (Left)
            if(KBINPUT('D') && direction != 'L'){direction = 'R';}//Move input (Right)
            if(frameCounter % gameSpeed == 0){//Sets the speed of the game without causing input lag. Input is taken every loop iteration but the game frames only update after a few iterations.
                switch(direction){//Sets placeholder segment coordinates to be in front of the snake based on current direction of motion
                    case 'U'://Moving up
                        segment.x = player[0].x; segment.y = player[0].y-1;
                    break;
                    case 'D'://Moving down
                        segment.x = player[0].x; segment.y = player[0].y+1;
                    break;
                    case 'L'://Moving left
                        segment.x = player[0].x-1; segment.y = player[0].y;
                    break;
                    case 'R'://Moving right
                        segment.x = player[0].x+1; segment.y = player[0].y;
                    break;
                }
                player.emplace(player.begin(),segment); player.pop_back(); //Add placeholder to snake front then remove rear segment to generate the illusion of motion
                if(player[0].x == food.x && player[0].y == food.y){//Check if player touched food object
                    segment.x = player[player.size()].x; segment.y = player[player.size()].y;//Set placeholder segment to position at rear of snake
                    player.push_back(segment);//Add placeholder segment to snake
                    srand(time(NULL));//Set RNG seed
                    food.x = rand()%(xMax-1)+1; food.y = rand()%(yMax-1)+1;//Generate new food object
                }
                system("CLS"); //Screen refresh
                SETCURSOR(food.x,food.y); cout << "*";//Display food object
                for(size_t a = 0;a<player.size();a++){ //Display loop for the snake
                    SETCURSOR(player[a].x,player[a].y); cout << "O"; //Draw snake body segments
                }
                for(size_t a = 0;a<player.size();a++){ //Checks lose conditions
                    if(player[0].x == player[a].x && player[0].y == player[a].y && a != 0){//Checks if the snake is touching itself. a!=0 because obviously I don't wanna compare the coordinates of 0 to 0, YOU JOKER, YOU.
                        lost = true;//Skill issue
                    }
                    if(player[0].x == 0 || player[0].x >= xMax || player[0].y == 0 || player[0].y >= yMax){//Checks if the snake hit the edge of the map
                        lost = true;//Not the comic. You're thinking of "loss". This is LOST, a variable to flag your skill issue at snake.
                    }
                }
            }
            Sleep(33);//This be here to cap the entire game loop's refresh rate
            frameCounter++;
        }
        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
        SETCURSOR(0,0);
        system("CLS");
        cout << "Ya lost, IDIOT!\nPlay again? (Y/N):" ;
        cin.clear();
        cin >> playAgain;
        if(playAgain == ('Y') || playAgain == ('y')){
            cout << "Cool!";
            Sleep(1000);
            lost = false;
        }
        if(playAgain == ('N') || playAgain == ('n')){
            cout << "K, bye!";
            Sleep(1000);
        }
    }
    return 0;
}
