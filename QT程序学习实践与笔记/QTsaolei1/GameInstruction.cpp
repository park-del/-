#include "GameInstruction.h"
#include <QHBoxLayout>
#include <QFont>

GameInstruction::GameInstruction(QWidget* parent):QDialog(parent)
{
    instruction = new QTextEdit;        //定义了一个文本框instruction
    instruction->setFixedSize(400,400); //设置这个文本框的尺寸为400*400
    QString A(tr("第一条：基本定式不要忘，现场推理真够呛。\n"
                 "第二条：鼠标点击不要快，稳定节奏把空开。\n"
                 "第三条：顺手标雷不要惯，积累下来记录悬。\n"
                 "第四条：无从下手不要愣，就近猜雷把心横。\n"
                 "第五条：遇到猜雷不要怕，爆了脸上不留疤。\n"
                 "第六条：猜雷猜错不要悔，哭天抢地也白费。\n"
                 "第七条：碰上好局不要慌，紧盯局部慢扩张。\n"
                 "第八条：痛失好局不要恨，既然有缘定有份。\n"));

    instruction->append(A);             //设置这个文本框中的内容为A

    QFont font("黑体", 12, QFont::Bold); /*定义了一个字体对象，同时指明这个字体的字体格式为黑体，12号
     ，加粗*/
    instruction->setFont(font);         //设置这个文本框中的字体为font
    instruction->setReadOnly(true);     //只读(打开了窗口后是不能对这个文本框来进行编辑的)

    QHBoxLayout *main_layout = new QHBoxLayout;//定义了一个水平布局框main_layout
    main_layout->addWidget(instruction);       //将文本框instruction放入这个水平布局框main_layout中
    setLayout(main_layout);    //设置当前这个窗口为栅格布局的方式

    setFixedSize(400,400);     //设置当前这个窗口的尺寸
    setWindowTitle(tr("帮助")); //设置当前这个窗口的标题为帮助
}
void GameInstruction::closeEvent(QCloseEvent*)
{
    hide();  //不关闭当前的窗口，而是隐藏当前的窗口
}
