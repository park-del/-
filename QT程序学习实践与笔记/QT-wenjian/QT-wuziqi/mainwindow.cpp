#include <QPainter>
#include <QTimer>
#include <QSound>
#include <QMouseEvent>
#include <QMessageBox>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <math.h>
#include "mainwindow.h"

// -------全局遍历-------//
#define CHESS_ONE_SOUND ":/res/sound/chessone.wav"
#define WIN_SOUND ":/res/sound/win.wav"
#define LOSE_SOUND ":/res/sound/lose.wav"

const int kBoardMargin = 30;  // 棋盘边缘空隙
const int kRadius = 15;       // 棋子半径
const int kMarkSize = 6;      // 落子标记边长
const int kBlockSize = 40;    // 格子的大小
const int kPosDelta = 20;     // 鼠标点击的模糊距离上限

const int kAIDelay = 700;     // AI下棋的思考时间

// -------------------- //
/*
这个类中包含了有
私有的数据成员：*game，game_type,clickPosRoe,clickPosCol
保护的成员函数：paintEvent(),mouseMoveEvent(),mouseReleaseEvent()
私有的成员函数：initGame(),checkGame()
槽函数：chessOneByPerson(),chessOneByAI(),initPVPGame(),initPVEGame()
提示：被保护的成员函数就是Event，即某一事件，这是由系统所提供的成员函数，而不是由用户定义的；
     私有的成员函数则是由用户自己所定义的。
*/
MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)/*构造函数，在main函数中定义了
一个窗口时会自动执行这个构造函数*/
{
    this->setWindowIcon(QIcon(":/res/tubiao.jpg"));

    /*设置棋盘大小,即是设置当前this这个窗口的尺寸大小*/
    setFixedSize(kBoardMargin * 2 + kBlockSize * kBoardSizeNum,
                 kBoardMargin * 2 + kBlockSize * kBoardSizeNum);
//setStyleSheet("background-color:yellow;");  设置当前这个窗口的背景颜色为黄色

    setMouseTracking(true); /*开启鼠标追踪功能，此时只需要在对应的框中进行移动就可以触发鼠标移动
    的事件了，如果没有开启鼠标追踪功能的话要进点击鼠标左键进行移动才能触发鼠标移动事件，即
    mouseMoveEvent()这个函数。*/

//centralWidget()->setMouseTracking(true);

    // 添加菜单
    QMenu *gameMenu = menuBar()->addMenu(tr("Game"));
    /*用QMenu定义了一个菜单gemeMenu，利用匿名对象的方式创建了一个菜单栏，同时为这个菜单栏添加了一个
      Game的菜单，gemeMenu就是这个菜单*/

    QAction *actionPVP = new QAction("Person VS Person", this);
    connect(actionPVP, SIGNAL(triggered()), this, SLOT(initPVPGame()));
    gameMenu->addAction(actionPVP); //为这个菜单gameMenu添加了一个actionPVP的菜单项
    /*
    利用了信号与槽的机制，当菜单gameMenu中的菜单项"Person VS Person"被点击时，this
    即是这个窗口会触发槽函数initPVPGame()。（游戏模式为人与人对战的模式时执行的是这个函数,这个
    槽函数只是对人与人对战这个模式进行一些初始化）
    */

    QAction *actionPVE = new QAction("Person VS Computer", this);
    connect(actionPVE, SIGNAL(triggered()), this, SLOT(initPVEGame()));
    gameMenu->addAction(actionPVE);//为这个菜单gameMenu添加了一个actionPVE的菜单项
    /*
    利用了信号与槽的机制，当菜单gameMenu中的菜单项"Person VS Computer"被点击时，this
    即是这个窗口会触发槽函数initPVEGame()。（人与机器对战的模式时执行的是这个函数,这个槽函数只是
    对人与人对战这个模式进行一些初始化）
    */

    initGame();  /* 开始游戏(如果gameMenu这个菜单中两个菜单项都没有被点击的话，则默认执行的是
    “人人模式”下的五子棋游戏)*/
}

MainWindow::~MainWindow() //析构函数
{
    if (game)
    {
        delete game;
        game = nullptr;
    }
}

void MainWindow::initGame()  //对游戏模式的初始化
{
    game = new GameModel;
    initPVPGame();
}

void MainWindow::initPVPGame()  //人与人对战模式的初始化（点击"人与人对战"时执行的槽函数）
{/*
   game_type是自定义的枚举类型GameType的一个变量（这个枚举类型只有两个变量，即PERSON和BOT，分别
   表示游戏此时是“人人模式”还是“人机模式”）。
   *geme是自定义的一个类GameModel的一个对象指针，
*/
    game_type = PERSON;          //geme_type为PERSON表示游戏此时为“人人模式”
    game->gameStatus = PLAYING;  //修改游戏此时的状态为PLAYING,表示游戏此时正在进行中
    game->startGame(game_type);  /*开始游戏，startGame()函数的参数为上面已经赋好了值的
    geme_type，表示接下来所进行的是game_type模式，即“人人模式”的五子棋游戏*/

    update(); /* 利用update()函数，意思就是调用绘图事件，同时将新的图像显示在窗口上(原来的那个是被
    删除了)*/
}

void MainWindow::initPVEGame() //人与机器对战模式的初始化（点击"人与机器对战"时执行的槽函数）
{
    game_type = BOT;            //geme_type为BOT表示游戏此时为“人机模式”
    game->gameStatus = PLAYING; //修改游戏此时的状态为PLAYING,表示游戏此时正在进行中
    game->startGame(game_type);
    update();
}

//没下一个棋子都会执行下面的函数，对整个棋盘进行更新
void MainWindow::paintEvent(QPaintEvent *event)
{

    QPainter painter(this);  //定义一个画家来画棋盘，并指明画家是在这个窗口上来画画，
    painter.setRenderHint(QPainter::Antialiasing, true); /* 抗锯齿(减少画家所画东西的毛边，即
    是让画家来好好画这幅画) */
    painter.drawPixmap(0,0,QPixmap(":/res/background.jpg"));//添加背景图片
//    painter.drawPixmap(230,0,QPixmap(":/res/adf.jpg"));
//    painter.drawPixmap(460,0,QPixmap(":/res/adf.jpg"));


/*  QPen pen;    调整线条宽度
    pen.setWidth(2);
    painter.setPen(pen);
*/

    /*棋盘尺寸为15*15,这里所说的为15*15个格子，故需要画16条横线，16条竖线（需要进行16次循环）。*/
    for (int i = 0; i < kBoardSizeNum + 1; i++)
    {
   //查帮助文档可知四个参数的drawLine()的函数是从(x1,y1)处到(x2,y2)处画一条直线
        painter.drawLine(kBoardMargin + kBlockSize * i, kBoardMargin,
        kBoardMargin + kBlockSize * i, size().height() - kBoardMargin);
   /*
    第一个drawLine()函数画的是竖线线，第二个drawLine()函数画的是横线。
    size.height()返回这个窗口的总高度，size.width()返回的是这个窗口的总宽度。

    画的这16条竖线的起点和终点的y坐标一直是不变的分别为kBoardMargin和
    size().height() - kBoardMargin，即16条竖线与窗口的上下边界都是留有间隙的，变化的只有x坐标而已。

    画的这16条横线的起点和终点的x坐标一直是不变的分别为kBoardMargin和
    size().width() - kBoardMargin，即16条横线与窗口的左右边界都是留有间隙的，变化的只有y坐标而已。
   */
        painter.drawLine(kBoardMargin, kBoardMargin + kBlockSize * i,
        size().width() - kBoardMargin, kBoardMargin + kBlockSize * i);
    }

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);/*画刷是用来画棋子的，这也是棋子的风格，画一个圆，然后再用
    该画刷对这个圆进行填充就成为了棋子了*/


    /*在窗口中进行鼠标移动时会不断的更新鼠标当前位置所对应的那个点的行线与列线，即是不断更新
      clickPosRow与clickPosCol。
      如果clickPosRow>0与clickPosRow<15就说明行线不为边界线，同理列线也是一样。
      game->gameMapVec[clickPosRow][clickPosCol] == 0表明了图上的这个点是没有被填充棋子的。
    */
    if (clickPosRow > 0 && clickPosRow < kBoardSizeNum &&
        clickPosCol > 0 && clickPosCol < kBoardSizeNum &&
        game->gameMapVec[clickPosRow][clickPosCol] == 0)
    {
        if (game->playerFlag) //判断游戏的双方，白方为true，黑方为false
            brush.setColor(Qt::white); //设置刷子的颜色为白色
        else
            brush.setColor(Qt::black); //设置刷子的颜色为黑色
        painter.setBrush(brush); //画家使用刷子
        painter.drawRect(kBoardMargin + kBlockSize * clickPosCol - kMarkSize / 2,
                         kBoardMargin + kBlockSize * clickPosRow - kMarkSize / 2,
                         kMarkSize, kMarkSize);
        /*画家在指定的点上画一个半径为kMarkSize,即是半径为6的一个圆，由于画家已经使用了刷子，故这个
          所画的这个圆将会被白色或者黑色所填充。  */
    }

    /*每次利用update()函数调用绘图事件时画家painter以前所画的图形都是会被删除掉的，故每次更新时
      画家painter都要重新画棋盘（在该函数的上面可以看到）和重新复原之前所画的棋子*/
    for (int i = 0; i < kBoardSizeNum; i++)
        for (int j = 0; j < kBoardSizeNum; j++)
        {
            if (game->gameMapVec[i][j] == 1)       //为1则该棋子填充白色
            {
                brush.setColor(Qt::white);
                painter.setBrush(brush);
                painter.drawEllipse(kBoardMargin + kBlockSize * j - kRadius,
                                    kBoardMargin + kBlockSize * i - kRadius,
                                    kRadius * 2, kRadius * 2);
            }
            else if (game->gameMapVec[i][j] == -1) //为-1则该棋子填充黑色
            {
                brush.setColor(Qt::black);
                painter.setBrush(brush);
                painter.drawEllipse(kBoardMargin + kBlockSize * j - kRadius,
                                    kBoardMargin + kBlockSize * i - kRadius,
                                    kRadius * 2, kRadius * 2);
            }
        }

    // 判断输赢
    if (clickPosRow > 0 && clickPosRow < kBoardSizeNum &&
        clickPosCol > 0 && clickPosCol < kBoardSizeNum &&
        (game->gameMapVec[clickPosRow][clickPosCol] == 1 ||
            game->gameMapVec[clickPosRow][clickPosCol] == -1))
    {
        if (game->isWin(clickPosRow, clickPosCol) && game->gameStatus == PLAYING)
        { //如果当前所下的棋子使得游戏游戏赢了的话并且当前游戏正在进行中
            qDebug() << "win";
            game->gameStatus = WIN;   //使游戏的状态变为获胜的状态
            QSound::play(WIN_SOUND);  //播放获胜的音乐
            QString str;
            if (game->gameMapVec[clickPosRow][clickPosCol] == 1)//判断当前所下的棋子是白子
                str = "white player";                           //还是黑子。
            else if (game->gameMapVec[clickPosRow][clickPosCol] == -1)
                str = "black player";
            QMessageBox::StandardButton btnValue = QMessageBox::information(this,
                                                   "congratulations", str + " win!");
/* str这个字符串记录了是白方赢还是黑方赢，当有某一方赢时会弹出一个信息对话框，信息对话框的标题为
   congratulations,信息对话框的内容为str + " win!",该静态成员函数的返回值是一个StandardButton
   类型的对象，故用btnValue来接收该静态成员函数的返回值，信息对话框中哪一个按键按下就会返回哪一个按键，
   btnValue中存放的就是被按下的那个按键。 */

            // 重置游戏状态，否则容易死循环
            if (btnValue == QMessageBox::Ok)//说明了有一方赢得了游戏，重新开始游戏
            {
                game->startGame(game_type);
                game->gameStatus = PLAYING;
            }
        }
    }


    // 判断死局
    if (game->isDeadGame())
    {
        QSound::play(LOSE_SOUND);
        QMessageBox::StandardButton btnValue = QMessageBox::information(this, "oops",
                                               "dead game!");
        if (btnValue == QMessageBox::Ok)
        {
            game->startGame(game_type);
            game->gameStatus = PLAYING;
        }

    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
/*由于是在mainwindow这个类中进行声明的这个鼠标移动的事件，故当在mainwindow这个窗口中移动鼠标时会
  触发该事件(即是执行该mouseMoveEvent()函数)。
*/
    int x = event->x();  //通过event->x()和event->y()可以获得鼠标当前所在位置的坐标
    int y = event->y();
/*  棋盘最外围所画的那四条线上是不能够画棋子的，kBoardMargin为棋盘边缘的空隙，kBlockSize为格子的
    大小，为40，/2为20，只要鼠标移动的位置的x坐标>=第1条竖线向右走20,<最后一条竖线；y的坐标>=第1条
    横线向下走20，<最后一条横线的话就将当前的x与y坐标做适当处理后得到col、row
*/
    if (x >= kBoardMargin + kBlockSize / 2 &&
            x < size().width() - kBoardMargin &&
            y >= kBoardMargin + kBlockSize / 2 &&
            y < size().height()- kBoardMargin)
    {
       /* 获取最近的左上角的点(此时得到的col是鼠标当前的位置前面是第几条竖线，得到的row是鼠标当前位置
          的前面是第几条横线。
          注意:此时所得到的col和row都是一个整数，所得到的是鼠标当前的位置前面对应的是第几条竖线，
              当前位置的前面对应的是第几条横线，并不是当前位置的坐标。
              而下面的leftTopPosX和leftTopPosy分别得到的是鼠标当前位置的前面所对应的那一条横线与
              那一条竖线的交叉点的坐标。 */
        int col = x / kBlockSize;
        int row = y / kBlockSize;

        int leftTopPosX = kBoardMargin + kBlockSize * col;
        int leftTopPosY = kBoardMargin + kBlockSize * row;


        clickPosRow = -1; // 初始化最终的值
        clickPosCol = -1;
        int len = 0;     // 计算完后取整就可以了

        /*下面的四个if语句是确定一个误差在范围内的点（即是该点与当前鼠标位置所在处的距离小于kPosDelta
          ，即是20，为格子大小的一半），且只可能确定一个出来。
         kPosDelta是鼠标与某一个点的最短距离，小于这个最短距离的话就认为鼠标当前的位置就是该点。
         依次算出鼠标当前位置与其所对应的四个点之间的距离，棋子的半径大小刚好为20，只能确定出一个点
         竖线看x横线看y
*/

      /*len算的是鼠标当前位置与鼠标当前位置前面的那一条横线与前面的那一条竖线的交叉点的距离*/
        len = sqrt((x - leftTopPosX) * (x - leftTopPosX)
              + (y - leftTopPosY) * (y - leftTopPosY));
        if (len < kPosDelta)
        {
            clickPosRow = row;
            clickPosCol = col;
        }

      /*len算的是鼠标当前位置与鼠标当前位置前面的那一条横线与后面的那一条竖线的交叉点的距离*/
        len = sqrt((x - leftTopPosX - kBlockSize) * (x - leftTopPosX - kBlockSize)
               + (y - leftTopPosY) * (y - leftTopPosY));
        if (len < kPosDelta)
        {
            clickPosRow = row;
            clickPosCol = col + 1;
        }

      /*len算的是鼠标当前位置与鼠标当前位置后面的那一条横线与前面的那一条竖线的交叉点的距离*/
        len = sqrt((x - leftTopPosX) * (x - leftTopPosX)
             + (y - leftTopPosY - kBlockSize) * (y - leftTopPosY - kBlockSize));
        if (len < kPosDelta)
        {
            clickPosRow = row + 1;
            clickPosCol = col;
        }

      /*len算的是鼠标当前位置与鼠标当前位置后面的那一条横线与后面的那一条竖线的交叉点的距离*/
        len = sqrt((x - leftTopPosX - kBlockSize) * (x - leftTopPosX - kBlockSize)
              + (y - leftTopPosY - kBlockSize) * (y - leftTopPosY - kBlockSize));
        if (len < kPosDelta)
        {
            clickPosRow = row + 1;
            clickPosCol = col + 1;
        }
    }
/*
  执行上面的if语句可以获得鼠标当前位置所对应点的位置，即鼠标当前位置所对应的是第clickPosRow条横线与
  第clickPosCol条竖线所交叉的那一个点。

  注意:clickPosRow与clickPosCol的初值都是为-1，加1后为0，故图中的第1条横线所对应的clickPosCol为0，
      图中的第1条竖线所对应的clickPosRow为0，即是从第0条线开始数的，一直到最后的第15条线，第0条线和
      第15条线就是棋盘的（最左或者最上）与（最右或者最下）的那个线，也就是边界线。
*/
    update(); //存了坐标后也要重绘

}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
/*
 mouseReleaseEvent为鼠标释放事件，每次当鼠标点击进行释放后会触发该事件，由于要释放就要先点击，因此
 可以当做鼠标点击时就会执行该函数。
 先下棋的game->playerFlag为true。
 当为人机模式时：是人先下棋的，故人所定义的game->playerFlag为true，执行这个函数时是人要下棋，
              game_type == BOT表示当前的模式是人机模式，要执行if语句里面的内容game->playerFlag
              必须为true，即此时必须轮到人下棋的时候才能够执行if里面的内容进行放置一颗棋子。这是为了
              防止在机器下棋的时候人抢机器的棋。
 当为人人模式时：由于game_type == BOT一定为false，当鼠标按下释放时一定会执行if语句里面的内容，也就
              不存在抢棋的现象了。（两个人用同一个鼠标下棋）
*/
    if (!(game_type == BOT && !game->playerFlag))// 人下棋，并且不能抢机器的棋
    {
        chessOneByPerson();//调用这个函数来完成人在对应的位置上下棋子

        /*如果是人人模式的话，则下面的if语句是会执行的；如果是人机模式的话，会执行下面的if语句，
          这里的if语句是AI进行下棋子的操作。*/
        if (game->gameType == BOT && !game->playerFlag)
        {
            // 用定时器做一个延迟
            QTimer::singleShot(kAIDelay, this, SLOT(chessOneByAI()));
        }
    }

}

void MainWindow::chessOneByPerson()//人下子
{
/*  clickPosRow==-1或者clickPosCol== -1表示的是边界线，只有当前下子的位置为有效区域并且
   game->gameMapVec[clickPosRow][clickPosCol] == 0，即当前该位置上没有子的话才能下子。*/
    if (  clickPosRow != -1 && clickPosCol != -1 &&
          game->gameMapVec[clickPosRow][clickPosCol] == 0)
    {
        game->actionByPerson(clickPosRow, clickPosCol);/*调用game的actionByPerson()函数
   来完成在第clickPosRow条行线与第clickPosCol条列线交叉的那一个点上下子。*/
        QSound::play(CHESS_ONE_SOUND);

        update(); //更新棋盘的情况
    }
}

void MainWindow::chessOneByAI()//机器下子
{
    game->actionByAI(clickPosRow, clickPosCol);/*调用game的actionByPerson()函数
    来完成在第clickPosRow条行线与第clickPosCol条列线交叉的那一个点上下子。*/
    QSound::play(CHESS_ONE_SOUND);

    update();//更新棋盘的情况
}
