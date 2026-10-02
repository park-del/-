#include "GameOption.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
//GameOption定义的一个对象为*options，option是难度选择对话框，在这里所说的当前窗口就是难度选择对话框。
GameOption::GameOption(QWidget* parent):QDialog(parent)
{
    levels = 1;     //默认为简单
    ok_button = new QPushButton(tr("确定"));
    group = new QGroupBox(tr("难度设定"));   //设置了一个group组并设置了group组的名称为"难度设定"

    easy_button = new QRadioButton(tr("简单难度")); //设置一个名称为“简单难度”的按键
    easy_button->setChecked(true);                 //按键默认选中“简单难度”
    mid_button = new QRadioButton(tr("中等难度"));  //设置一个名称为“中等难度”的按键
    hard_button = new QRadioButton(tr("困难"));    //设置一个名称为“困难”的按键

    //布局安装
    QVBoxLayout *button_layout = new QVBoxLayout; /*定义了一个垂直布局框button_layout,放入该垂直
    布局框button_layout的按钮将会自动垂直布局。
    扩展：QHBoxLayout *pLayout = new QHBoxLayout()，即用QHBoxLayout定义的一个对象是水平布局框。*/
    button_layout->addWidget(easy_button);//为这个垂直布局框button_layout中增加“简单难度”、“中等难度”
    button_layout->addWidget(mid_button); //、“困难”按钮
    button_layout->addWidget(hard_button);

    group->setLayout(button_layout); //将垂直布局框button_layout放入到group组中同时对其进行栅格布局

    QHBoxLayout *ok_layout = new QHBoxLayout;     //定义了一个水平布局框ok_layout
    ok_layout->addStretch();
    ok_layout->addWidget(ok_button);              //为这个水平布局框ok_layout中增加一个“确定”按钮

    QVBoxLayout *main_layout = new QVBoxLayout;   //定义了一个垂直布局框main_layou
    main_layout->addWidget(group);     //将垂直布局框button_layout增加到垂直布局框main_layou中
    main_layout->addLayout(ok_layout); //将水平布局框ok_layout增加到垂直布局框main_layou中
    setLayout(main_layout);            //这个垂直布局框main_layou在当前这个窗口上进行栅格布局
    setWindowTitle(tr(" "));           //设置当前这个窗口的标题为" "

    connect(ok_button,SIGNAL(clicked()),this,SLOT(chose_level()));/*点击"确定"按钮后当前这个按钮会
    触发槽函数chose_level()。*/
    connect(ok_button,SIGNAL(clicked()),this,SLOT(hide()));       /*点击"确定"按钮后当前这个窗口
    会触发槽函数hide()。*/
}
void GameOption::chose_level()
{
    if(easy_button->isChecked()) //确定哪一个按键被点击从而对levels进行赋值来设置游戏难度
        levels = 1;
    else if(mid_button->isChecked())
        levels = 2;
    else if(hard_button->isChecked())
        levels = 3;
    emit new_level(levels);    //发送一个new_level的信号（参数为当前的难度等级），信号也就是一个函数
}
void GameOption::closeEvent(QCloseEvent*)
{
    emit new_level(levels);
    hide();  //关闭对话框只是隐藏，点击确定按钮后将当前这个窗口给关掉
}
