#include"mineGame.h"
#include <QLayout>
#include <windows.h>
#include <QProcess>

mineGame::mineGame(QWidget* parent): QMainWindow(parent)
{
    //开机动画
    setWindowTitle (tr("豆豆豆豆豆的宇宙无敌的扫雷游戏")); //设置主窗口标题
    resize (600, 450);                                //设置主窗口的尺寸

    //使主窗口程序在初始化时休眠几秒钟
    Sleep (1000);

    //设置中心窗体
    mine_land = new mineLand;       //设置一个mine_land的对象
    setCentralWidget(mine_land);    //将mine_land设置为中心窗口，所谓的中心窗口就是中心部件

    //设置游戏说明对话框
    helps = new GameInstruction(this);

    //设置游戏难度选择对话框
    options = new GameOption(this);/*GameOption所定义的对象option为一个难度选择的窗口，当点击这个
    窗口中的“确定”按钮时会触发信号new_level(levels),此时mine_land对象，即是显示地雷的中心窗口，会去触发
    相应的槽函数set_level(int)来设置游戏的难度。因为是mind_land对象来去触发相应的槽函数的，故这个槽函数
    应该是定义在mineLand这个类中的。因为是options这个对象发出信号的，故这个信号应该是在GameOption这个类
    中定义的。*/
    connect(options, SIGNAL(new_level(int)),mine_land,SLOT(set_level(int)));

    //设置游戏结束对话框
    game_end = new GameEnd(this);
    connect(mine_land, SIGNAL(win()), game_end,  SLOT(win_show()));
    connect(mine_land, SIGNAL(lost()), game_end, SLOT(lost_show()));

    //游戏菜单上的动作
    begin_action = new QAction(tr("开始"),this);
    begin_action->setStatusTip(tr("开始新游戏."));
    connect(begin_action,SIGNAL(triggered()),mine_land,SLOT(new_game()));

    option_action = new QAction(tr("选项"),this);
    option_action->setStatusTip(tr("选择游戏难度."));
    connect(option_action,SIGNAL(triggered()),this,SLOT(new_level()));

    close_action = new QAction(tr("结束游戏"),this);
    connect(close_action,SIGNAL(triggered()),this,SLOT(close()));

    calcuate_action = new QAction(tr("计算器"), this);
    connect(calcuate_action, SIGNAL(triggered(bool)), this, SLOT(show_calcuate()));

    //help菜单上的动作
    info_action = new QAction(tr("说明"),this);
    info_action->setStatusTip(tr("胜利条件."));
    connect(info_action,SIGNAL(triggered()),this,SLOT(show_help()));

    //把动作添加到对应的菜单
    game_menu = new QMenu(tr("游戏."),this);
    game_menu->addAction(begin_action);
    game_menu->addAction(option_action);
    game_menu->addSeparator();  //加个分割线
    game_menu->addAction(close_action);
    game_menu->addAction(calcuate_action);

    help_menu = new QMenu(tr("信息"),this);
    help_menu->addAction(info_action);
    //把菜单+到菜单栏上
    menus = menuBar();
    menus->addMenu(game_menu);
    menus->addMenu(help_menu);
    //生成状态栏
    statusBar();
    layout()->setSizeConstraint(QLayout::SetFixedSize);  //固定大小，谢绝拉伸
    setWindowIcon(QIcon(tr(":/images/read.jpg")));
}
void mineGame::new_level()
{
    options->exec();
}
void mineGame::show_help()
{
    helps->exec();
}

void mineGame::show_calcuate()
{
    QProcess *game = new QProcess();
    game->start("F:\\QT5_All_Code\\N_Queen_version2"
                "\\mine\\build-Calculate-Desktop_Qt_5_6_0_MinGW_32bit-Debug"
                "\\debug\\Calculate.exe");
}



