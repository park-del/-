#include <utility>
#include <stdlib.h>
#include <time.h>
#include "GameModel.h"

GameModel::GameModel()
{

}
void GameModel::startGame(GameType type) //开始游戏，传过来的参数指的是游戏的模式（游戏是人人模
{                                         //式还是人机模式，这是根据信号槽的机制来实现的）
    gameType = type;
    // 初始棋盘
    gameMapVec.clear();

    for (int i = 0; i < kBoardSizeNum; i++)
    {
/* 定义了一个lineBoard的容器（相当于是一个数组名为lineBoard的一维数组），这个容器中的每一个元素都是为
   int类型的  */
        std::vector<int> lineBoard;
        for (int j = 0; j < kBoardSizeNum; j++)
             lineBoard.push_back(0);//在这个容器lineBoard（即一维数组）的末尾依次添加j个0
        gameMapVec.push_back(lineBoard);/*在gameMapVec这个容器的末尾依次添加i个已经被赋好了值
的lineBoard容器（且lineBoard这个容器的大小都是为j个int类型的元素），这样gameMapVec这个容器就相当于
是一个数组名为gameMapVec的二维数组了。
 */
    }

   if (gameType == BOT) // 如果是AI模式，需要初始化评分数组
    {
        scoreMapVec.clear();
        for (int i = 0; i < kBoardSizeNum; i++)
        {
            std::vector<int> lineScores;//定义一个数组名为lineScore的一维数组
            for (int j = 0; j < kBoardSizeNum; j++)
                lineScores.push_back(0);//在这个一维数组lineScore的末尾依次添加j个0
            scoreMapVec.push_back(lineScores);/*scoreMapVec这个容器就相当于数组名为
            scoreMapVec且已经被赋好了初值为0的一个二维数组。 */
        }
    }
    playerFlag = true;  //先开始下起的那一方为true，后开始下棋的那一方为false
}


void GameModel::updateGameMap(int row, int col)
{
    if (playerFlag)
        gameMapVec[row][col] = 1;
    else
        gameMapVec[row][col] = -1;

    // 换手
    playerFlag = !playerFlag;
}

void GameModel::actionByPerson(int row, int col)    //人在row与col交叉的那个点上下棋子
{
    updateGameMap(row, col);
}

void GameModel::actionByAI(int &clickRow, int &clickCol)//机器在clickRow与clickCol交叉的
{  //那个点上下棋子。

    calculateScore(); // 计算评分

    int maxScore = 0; //maxScore用来记录最大的评分
    std::vector<std::pair<int, int>> maxPoints;/*定义一个二维数组maxPoints用来存放评分最大的
    的那个位置的坐标，因为评分最大的那个位置可能是有多个的，故需要定义一个二维数组来存放。 */

    for (int row = 1; row < kBoardSizeNum; row++)//row和col为0时是不能填充子的，故冲1开始循环
        for (int col = 1; col < kBoardSizeNum; col++)
        {
            if (gameMapVec[row][col] == 0)// 前提是这个坐标是空的，这样才能在这个坐标上来填充黑子
            {
                if (scoreMapVec[row][col] > maxScore) //找最大的数和坐标
                {
                    maxPoints.clear();
                    maxScore = scoreMapVec[row][col];
                    maxPoints.push_back(std::make_pair(row, col));
                }
                else if (scoreMapVec[row][col] == maxScore)  //如果有多个最大的数，都存起来
                    maxPoints.push_back(std::make_pair(row, col));
            }
        }

    // 随机落子，如果有多个点的话
    srand((unsigned)time(0));
    int index = rand() % maxPoints.size();/*随机数对这个容器中的元素数求余，假设容器中的元素数为
a的话（容器中的元素数为a也就是有a个相同评分且是最大的空位置），rand()%a求余就是0~a-1，分别对应这a个
评分最大的元素，是从这些评分最大的空位置中随机地选择一个的）*/

    std::pair<int, int> pointPair = maxPoints.at(index);/*访问容器中第index个元素
    并赋值给pointPair这个二维数组。*/
    clickRow = pointPair.first; //pointPair这个二维数组中的第1维度的下标
    clickCol = pointPair.second;//pointPair这个二维数组中的第2维度的下标
    updateGameMap(clickRow, clickCol);
}

// 最关键的计算评分函数
void GameModel::calculateScore()//机器就是根据这个函数来选择其要在哪一个位置上来下棋子的
{

    int personNum = 0; // 玩家连成子的个数
    int botNum = 0;    // AI连成子的个数
    int emptyNum = 0;  // 各方向空白位的个数


    scoreMapVec.clear();// 清空评分数组
    for (int i = 0; i < kBoardSizeNum; i++)
    {
        std::vector<int> lineScores;
        for (int j = 0; j < kBoardSizeNum; j++)
            lineScores.push_back(0);
        scoreMapVec.push_back(lineScores);//评分数组是一个已经被赋值为0的二维数组
    }


    /* 计分（此处是完全遍历，其实可以用bfs或者dfs加减枝降低复杂度，通过调整权重值，调整AI
       智能程度以及攻守风格）
       对整个棋盘进行遍历一遍，只要该点没有下棋子的话就进行计算，对没有下棋子的地方进行计算看在哪一个
       位置上下所得的分数最高。
    */
    for (int row = 0; row < kBoardSizeNum; row++)
        for (int col = 0; col < kBoardSizeNum; col++)
        {
           //找到某个空白的要下点的位置,row和col就代表了这个点的位置
            if (row > 0 && col > 0 &&
                gameMapVec[row][col] == 0)
            {
     /* 对某一个没有下棋子的位置遍历其周围八个方向，分别为(-1,-1)、(0,-1)(1,-1);(-1,0)(0,0)
        (1,0);(-1,1)(0,1)(1,1)。
        遍历的方向要去掉(0,0)这个方向，(0,0)为当前这个位置，不需要遍历。
        因为五子棋当下一个棋子时需要对八个方向进行判断才能判断所下的这个棋子是否赢，故要遍历八个方向。
 以当前空白的位置为(0,0)坐标原点建立坐标系，x为1表示x方向为正方向，x为-1表示x方向为负方向；
 y为1表示y方向为正方向，y为-1表示y方向为负方向。
 即是    (-1,-1):西南方向  (0,-1):正南方向    (1,-1):东南方向    (-1,0):正西方向
         (1,0) :正东方向   (-1,1):西北方向    (0,1):正北方向    (1,1) :东北方向
 依次是对每一个空位置的这些方向进行遍历的。
      */
                for (int y = -1; y <= 1; y++)
                for (int x = -1; x <= 1; x++)
                {
                   personNum = 0; //玩家连子的个数初始时设为0
                   botNum = 0;    //AI连子的个数初始时设为0
                   emptyNum = 0;  //在当前这个(x,y)的方向的空白数初始时设为0

    //这里以x与y都为1，即东北方向为例
                   if (!(y == 0 && x == 0))//除去(0,0)，其他八个方向会执行if语句里面的内容
                   {

                      // 对玩家白子评分（正反两个方向）
                      for (int i = 1; i <= 4; i++)
                      { //row对应的是行线，看的是y；x对应的是列线，看的是x
                         if (row + i * y > 0 && row + i * y < kBoardSizeNum &&
                             col + i * x > 0 && col + i * x < kBoardSizeNum &&
                             gameMapVec[row + i * y][col + i * x] == 1)//东北方向有白子的话
                          {  //白子就是玩家的字
                              personNum++; //从该点开始，东北方向的四个点中白点的个数
                          }
  /*举例：当x为1，y为1时,(row+i*y)所表示的行线与(col+i*x)所表示的列线的交叉点是位于(0,0)这个点的
         东北方向的，当i从1到<=4进行4次循环时所统计的就是以当前这个(0,0)点为原点的东北方向的四个点
         的情况。其他的7个方向也是一样的，x和y确定好了一个方向后，i从1到4进行4次循环就是从(0,0)点开
         始，判断x与y所确定好的那一个方向的四个点的情况。
  */
                          else
                          if (row + i * y > 0 && row + i * y < kBoardSizeNum &&
                              col + i * x > 0 && col + i * x < kBoardSizeNum &&
                              gameMapVec[row + i * y][col + i * x] == 0)//东北方向为空子时
                           {
                                    emptyNum++;//东北方向有一个空位置的话就跳出这个for循环
                                    break;
                            }
                            else
                                break;  //东北方向为黑子的话就跳出这个for循环(黑子是机器的子)
                       }


                       for (int i = 1; i <= 4; i++)//统计东北方向的反方向(东南方向)的各个棋子的情况
                       {
                          if (row - i * y > 0 && row - i * y < kBoardSizeNum &&
                              col - i * x > 0 && col - i * x < kBoardSizeNum &&
                              gameMapVec[row - i * y][col - i * x] == 1)//东南方向有白子的话
                          {
                               personNum++;//从该点开始，东南方向的四个点中白点的个数加上东北方向
                               //的四个点中白点的个数。
                          }
                          else
                          if (row - i * y > 0 && row - i * y < kBoardSizeNum &&
                              col - i * x > 0 && col - i * x < kBoardSizeNum &&
                              gameMapVec[row - i * y][col - i * x] == 0) // 空白位
                           {
                              emptyNum++;//东南方向有一个空位置的话就跳出这个for循环
                              break;
                           }
                           else
                           break;//东南方向为黑子的话就跳出这个for循环(黑子是机器的子)
                        }


                       if (personNum == 1)        //在东南东北这条斜线上有1颗白子时(杀二)
                            scoreMapVec[row][col] += 10;
                       else
                       if (personNum == 2)        //在东南东北这条斜线上有2颗白子时(杀三)
                       {
                          if (emptyNum == 1)  //这条斜线上有1个空位时
                             scoreMapVec[row][col] += 30;
                          else
                          if (emptyNum == 2) //这条斜线上有2个空位时
                             scoreMapVec[row][col] += 40;
                        }
                        else
                        if (personNum == 3)      //在东南东北这条斜线上有3颗白子时(杀四)
                        {
                                // 量变空位不一样，优先级不一样
                           if (emptyNum == 1)
                              scoreMapVec[row][col] += 60;
                           else
                           if (emptyNum == 2)
                              scoreMapVec[row][col] += 110;
                        }
                        else
                            if (personNum == 4) //在东南东北这条斜线上有4颗白子时(杀五)
                                scoreMapVec[row][col] += 10100;
//杀五的评分最高，应该优先在该位置上填充黑子，否则的玩家就要赢了



                      emptyNum = 0;  //进行一次清空
                            // 对AI黑子评分
                      for (int i = 1; i <= 4; i++)
                      {
                       if (row + i * y > 0 && row + i * y < kBoardSizeNum &&
                           col + i * x > 0 && col + i * x < kBoardSizeNum &&
                           gameMapVec[row + i * y][col + i * x] == 1)//东北方向有黑子的话
                       {
                           botNum++;//统计东北方向的黑子的个数
                       }
                       else
                       if (row + i * y > 0 && row + i * y < kBoardSizeNum &&
                           col + i * x > 0 && col + i * x < kBoardSizeNum &&
                           gameMapVec[row +i * y][col + i * x] == 0)//东北方向有空位的话
                       {
                           emptyNum++;//东北方向有一个空位就出界，同时退出这个for循环
                           break;
                       }
                       else
                           break;//东北方向有一个白子时就退出这个for循环
                      }


                     for (int i = 1; i <= 4; i++)
                     {
                     if (row - i * y > 0 && row - i * y < kBoardSizeNum &&
                         col - i * x > 0 && col - i * x < kBoardSizeNum &&
                         gameMapVec[row - i * y][col - i * x] == -1)//东南方向有黑子的话
                         {
                           botNum++;//统计东南和东北这条斜线上黑子的个数
                         }
                     else
                     if (row - i * y > 0 && row - i * y < kBoardSizeNum &&
                         col - i * x > 0 && col - i * x < kBoardSizeNum &&
                         gameMapVec[row - i * y][col - i * x] == 0) // 空白位
                      {
                           emptyNum++;//东南方向有一个空位就出界，同时退出这个for循环
                           break;
                      }
                      else
                           break;//东南方向有一个白子时就退出这个for循环
                     }

//是以当前要下的这个点为基点，东北方向四个点，东南方向四个点，故东南东北这条斜线上总共是有8个点的(原点除外)
                            if (botNum == 0)  //东南和东北这条斜线上没有黑子时(普通下子)
                                scoreMapVec[row][col] += 5;
                            else
                            if (botNum == 1)  //东南和东北这条斜线上有1颗黑子时(活二)
                                scoreMapVec[row][col] += 10;
                            else
                            if (botNum == 2) //东南和东北这条斜线上有2颗黑子
                            {
                                if (emptyNum == 1)  //东南和东北这条斜线上有1个空位(死三)
                                    scoreMapVec[row][col] += 25;
                                else
                                if (emptyNum == 2) //东南和东北这条斜线上有2个空位(活三)
                                    scoreMapVec[row][col] += 50;
                            }
                            else
                            if (botNum == 3)//东南和东北这条斜线上有2颗黑子
                            {
                                if (emptyNum == 1)//东南和东北这条斜线上有1个空位(死四)
                                    scoreMapVec[row][col] += 55;
                                else
                                if (emptyNum == 2)//东南和东北这条斜线上有2个空位(活四)
                                    scoreMapVec[row][col] += 100;
                            }
                            else
                            if (botNum >= 4)//东南和东北这条斜线上有4颗黑子(活五)
                                scoreMapVec[row][col] += 10000;

                        }//if(x!=0&&y!=0)的括号
                    } //for (int y = -1; y <= 1; y++);for (int x = -1; x <= 1; x++)括号

            }
        }
}

bool GameModel::isWin(int row, int col)
{
    // 横竖斜四种大情况，每种情况都根据当前落子往后遍历5个棋子，有一种符合就算赢
    // 水平方向
    for (int i = 0; i < 5; i++)
    {
        // 往左5个，往右匹配4个子，20种情况
        if (col - i > 0 &&
            col - i + 4 < kBoardSizeNum &&
            gameMapVec[row][col - i] == gameMapVec[row][col - i + 1] &&
            gameMapVec[row][col - i] == gameMapVec[row][col - i + 2] &&
            gameMapVec[row][col - i] == gameMapVec[row][col - i + 3] &&
            gameMapVec[row][col - i] == gameMapVec[row][col - i + 4])
            return true;
    }

    // 竖直方向(上下延伸4个)
    for (int i = 0; i < 5; i++)
    {
        if (row - i > 0 &&
            row - i + 4 < kBoardSizeNum &&
            gameMapVec[row - i][col] == gameMapVec[row - i + 1][col] &&
            gameMapVec[row - i][col] == gameMapVec[row - i + 2][col] &&
            gameMapVec[row - i][col] == gameMapVec[row - i + 3][col] &&
            gameMapVec[row - i][col] == gameMapVec[row - i + 4][col])
            return true;
    }

    // 左斜方向
    for (int i = 0; i < 5; i++)
    {
        if (row + i < kBoardSizeNum &&
            row + i - 4 > 0 &&
            col - i > 0 &&
            col - i + 4 < kBoardSizeNum &&
            gameMapVec[row + i][col - i] == gameMapVec[row + i - 1][col - i + 1] &&
            gameMapVec[row + i][col - i] == gameMapVec[row + i - 2][col - i + 2] &&
            gameMapVec[row + i][col - i] == gameMapVec[row + i - 3][col - i + 3] &&
            gameMapVec[row + i][col - i] == gameMapVec[row + i - 4][col - i + 4])
            return true;
    }

    // 右斜方向
    for (int i = 0; i < 5; i++)
    {
        if (row - i > 0 &&
            row - i + 4 < kBoardSizeNum &&
            col - i > 0 &&
            col - i + 4 < kBoardSizeNum &&
            gameMapVec[row - i][col - i] == gameMapVec[row - i + 1][col - i + 1] &&
            gameMapVec[row - i][col - i] == gameMapVec[row - i + 2][col - i + 2] &&
            gameMapVec[row - i][col - i] == gameMapVec[row - i + 3][col - i + 3] &&
            gameMapVec[row - i][col - i] == gameMapVec[row - i + 4][col - i + 4])
            return true;
    }

    return false;
}

bool GameModel::isDeadGame()
{
    // 所有空格全部填满
    for (int i = 1; i < kBoardSizeNum; i++)
        for (int j = 1; j < kBoardSizeNum; j++)
        {
            if (!(gameMapVec[i][j] == 1 || gameMapVec[i][j] == -1))
                return false;
        }
    return true;
}
