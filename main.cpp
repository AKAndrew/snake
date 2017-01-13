#include <iostream>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include <ctime>

using namespace std;
enum {menu, gaming} gameState;
enum {none, up, down, right_, left_} moves;
enum {faster, slower, doublePoints, extraPoints, decreaseSize} special;
//enum {singlePlayer, versus, highScores, quit} menu;

unsigned int width=25, height=36,cursor=12, score, scoreH, delay;

struct
{
    struct
    {
        unsigned int x, y;
    } normal;

    struct
    {
        unsigned int x, y;
        clock_t timer;
        char name[25];
        bool running, dp;
    } special;
} target;

struct
{
    struct
    {
        unsigned int x, y;
    } head;
    struct
    {
        unsigned int length, x[100], y[100];
    } body;
} snake;

void startUp()
{
    delay = 50;
    srand(time(0));
    gameState = menu;
    moves=none;

    target.special.x = rand()%(width-1) + 1;
    target.special.y = rand()%(height-1) + 1;
    target.special.running = 0;
    strcpy(target.special.name,"none");

    target.normal.x = rand()%(width-1) + 1;
    target.normal.y = rand()%(height-1) + 1;

    snake.head.x = rand()%(width-1) + 1;
    snake.head.y = rand()%(height-1) + 1;
    snake.body.length=0;

    score=0;
    scoreH=0;
}

void muv()
{
    switch(moves)
    {
    case up://up
        snake.head.y--;
        break;

    case left_://left
        snake.head.x--;
        break;

    case right_://right
        snake.head.x++;
        break;

    case down://down
        snake.head.y++;
        break;

    default:

        break;
    }

    if(snake.head.x < 1) snake.head.x = height-1;
    else if(snake.head.x >= height) snake.head.x = 1;

    if(snake.head.y < 1) snake.head.y = width-1;
    else if(snake.head.y >= width) snake.head.y = 1;

    if(snake.head.x == target.normal.x && snake.head.y == target.normal.y)
    {
        if(target.special.dp) score+=20;
        else score+=10;
        target.normal.x = rand()%(width-1) + 1;
        target.normal.y = rand()%(height-1) + 1;
        snake.body.length++;
    }

    if(snake.head.x == target.special.x && snake.head.y == target.special.y)
    {
        srand(time(0));
        switch(rand()%5 + 1)
        {
        case faster:
            target.special.running = 1;
            delay=30;
            strcpy(target.special.name,"faster");
            break;
        case slower:
            target.special.running = 1;
            delay=200;
            strcpy(target.special.name,"slower");
            break;
        case doublePoints:
            target.special.running = 1;
            target.special.dp = 1;
            strcpy(target.special.name,"double points");
            break;
        case extraPoints:
            target.special.running = 1;
            score+=50;
            strcpy(target.special.name,"extra points");
            break;
        case decreaseSize:
            target.special.running = 1;
            if(snake.body.length > 0) snake.body.length--;
            strcpy(target.special.name,"size decreased");
            break;
        }
    }

    if(target.special.running != 0)
    {
        target.special.x=NULL;
        target.special.y=NULL;
        target.special.timer+=delay;
        if(target.special.timer/1000 >= 10)
        {
            target.special.timer=0;
            target.special.dp=0;
            target.special.running=0;
            strcpy(target.special.name,"none");
            delay=50;
            target.special.x = rand()%(width-1) + 1;
            target.special.y = rand()%(height-1) + 1;
        }
    }
}

void keys()
{
    if(kbhit())
    {
        switch(getch())
        {
        case 72: //up
            if(gameState == gaming)
                moves = up;
            else
                cursor--;
            break;

        case 75: //left
            moves = left_;
            break;


        case 77: //right
            moves = right_;
            break;

        case 80: //down
            if(gameState == gaming)
                moves = down;
            else
                cursor++;
            break;

        case 13:
            if(gameState == menu)
                switch(cursor)
                {
                case 12: //single
                    gameState=gaming;
                    break;
                case 13: //versus
                    gameState=gaming;
                    break;
                case 14: //highscore
                    break;
                case 15: //quit
                    break;

                }

        }
    }
}

void print()
{
    for(int x=0; x<=width; x++)
    {
        for(int y=0; y<=height; y++)
        {
            if((x == 0 || x == width) && (y >= 0 || y <= height))cout<<"-";
            else if((x > 0 || x < width) && (y == 0 || y == height)) cout<<"|";
            else
                switch(gameState)
                {
                case menu:
                    if( x == 10 && y == 15)
                    {
                        cout<<"MENU";
                        y=y+strlen("MENU") - 1;
                    }
                    else if( x == 12 && y == 13)
                    {
                        cout<<"play solo";
                        y=y+strlen("play solo") - 1;
                    }
                    else if( x == 13 && y == 12)
                    {
                        cout<<"play versus";
                        y=y+strlen("play versus") - 1;
                    }
                    else if( x == 14 && y == 12)
                    {
                        cout<<"high scores";
                        y=y+strlen("high scores") - 1;
                    }
                    else if( x == 15 && y == 15)
                    {
                        cout<<"quit";
                        y=y+strlen("quit") - 1;
                    }
                    else if( x == cursor && y == 10)
                    {
                        cout<<">";
                        y=y+strlen(">") - 1;
                    }
                    else cout<<" ";
                    break;
                case gaming:
                    if(snake.head.y == x && snake.head.x == y) cout<<"O";
                    else if(target.normal.y == x && target.normal.x == y) cout<<"~";
                    else if(target.special.y == x && target.special.x == y) cout<<"@";
                    else
                        cout<<" ";
                        if(snake.body.length > 0)
                        {
                        for(int part=0; part<snake.body.length; part++)
                            if(snake.body.x[part] == y && snake.body.y[part] == x)
                                cout<<"o";
                        }
                    break;
                }
        }
        cout<<endl;
    }
    if(gameState==gaming) cout<<"Your score: "<<score<<endl<<"Special: "<<target.special.name<<", "<<10-(target.special.timer/1000)<<endl<<endl<<"Highest score: "<<scoreH;
}


int main()
{
    startUp();
    while(1)
    {
        print();
        keys();
        muv();
        Sleep(delay);
        system("cls");
    }
    return 0;
}
