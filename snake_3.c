#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#define WIDE 60
#define HIGH 20


//定义蛇
struct BODY{
    int X;
    int Y;                 
};                                //一个蛇身体
struct SNAKE{
    struct BODY body[WIDE*HIGH];
    int size;
}snake;                           //定义一条蛇

//定义食物
struct FOOD{
    int x;
    int y;
}food;                            //定义一个食物


//初始化蛇
void initSnake(void){
    snake.size = 2;

    snake.body[0].X = WIDE/2;
    snake.body[0].Y = HIGH/2;     //初始化蛇头

    snake.body[1].X = WIDE/2-1;
    snake.body[1].Y = HIGH/2;     //初始化一节蛇身
}

//初始化食物
void initFood(void){                        
    food.x = rand() % (WIDE - 2) + 1;       //食物可以出现的范围
    food.y = rand() % (HIGH - 2) + 1;
}


//展示分数
int score = 0;


//定义墙
void initWall(void)
{
    for(size_t i = 0; i <= HIGH; i++)          //外循环：行数
    {
        for(size_t j = 0; j <= WIDE; j++)      //内循环：列数
        {
            if(i == 0 || i == HIGH)
            {
                printf("-");                  //设计上下墙
            }
            else if(j == 0 || j == WIDE)
            {
                printf("|");                  //设计左右墙
            }
            else
            {
                printf(" ");                  //中间区域
            }
        }
        printf("\n");                         //在每一行打印完后实现换行
    }
}


//画食物
void drawFood(void)
{
    COORD coord;
    coord.X = food.x;
    coord.Y = food.y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    putchar('#');
}


//初始化界面控制
void initUI(void) 
{         
    COORD coord = {0};                         //光标的移动位置
    for (size_t i = 0; i < snake.size; i++)
    {
        coord.X = snake.body[i].X;
        coord.Y = snake.body[i].Y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        if(i == 0)
            putchar('@');
        else
            putchar('*');
    }
}


//用户按下键得到的坐标偏移量
int kX = 1;                                    //开局默认向右移动
int kY = 0;

int lastX = 0;
int lastY = 0;


//开启游戏
void playGame(void)
{
    char head = 'd';                           //蛇刚开局自动向右移动
    COORD coord;

    //判断蛇撞墙 
    while (snake.body[0].X > 0 && snake.body[0].X < WIDE
        && snake.body[0].Y > 0 && snake.body[0].Y < HIGH)        
    {
        if (_kbhit()){
            head = _getch();                   //判断用户是否按键      
        }

        switch (head)
        {
            case 'w': case 'W': if (kY != 1)  { kX = 0;  kY = -1; } break;
            case 's': case 'S': if (kY != -1) { kX = 0;  kY = 1;  } break;
            case 'a': case 'A': if (kX != 1)  { kX = -1; kY = 0;  } break;
            case 'd': case 'D': if (kX != -1) { kX = 1;  kY = 0;  } break;
            default:
                break;
        }

        //去除蛇尾
        lastX = snake.body[snake.size - 1].X;
        lastY = snake.body[snake.size - 1].Y;
        coord.X = lastX;
        coord.Y = lastY;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        putchar(' ');

        //蛇身体移动（让尾给前赋值）
        for (int i = snake.size - 1; i > 0; i--)
        {
            snake.body[i].X = snake.body[i - 1].X;
            snake.body[i].Y = snake.body[i - 1].Y;  
        }
        snake.body[0].X += kX;
        snake.body[0].Y += kY;                 //蛇头的移动

        //蛇头撞身体
        for (size_t i = 1; i < snake.size; i++)
        {
            if (snake.body[0].X == snake.body[i].X
             && snake.body[0].Y == snake.body[i].Y)
            {
                return;                        //游戏结束（蛇头撞身体）
            }
        }

        //蛇头撞食物检测
        if (snake.body[0].X == food.x && snake.body[0].Y == food.y)
        {
            initFood();                        //重新生成食物
            drawFood();                        //绘制新食物
            snake.size++;                      //身体增长
            score += 10;                       //加分
        }

        //重新绘制蛇
        initUI();

        //将光标移至不干扰游戏的位置
        coord.X = 0;
        coord.Y = HIGH + 2;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

        //延时控制移动速度
        Sleep(100);
    }
}


//打印成绩
void showScore(void)
{
    COORD coord;
    coord.X = 0;
    coord.Y = HIGH + 2;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    printf("Game Over!!!\n");
    printf("成绩为:%d\n\n\n", score);
}


//调用函数
int main(void)
{
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);                                    //防止中文乱码

    CONSOLE_CURSOR_INFO cci;
    cci.dwSize = sizeof(cci);
    cci.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);   //让光标不乱闪烁

    srand((unsigned) time(NULL));                                 //播种随机数种子

    initSnake();                                                  //初始化蛇
    initFood();                                                   //初始化食物
    initWall();                                                   //定义墙
    drawFood();                                                   //画食物
    initUI();                                                     //画蛇
    playGame();                                                   //启动游戏
    showScore();                                                  //显示分数
    
    system("pause");                                              //用于暂停黑窗口
    return EXIT_SUCCESS;
}

