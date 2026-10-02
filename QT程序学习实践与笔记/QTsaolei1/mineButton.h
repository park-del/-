#ifndef MINEBUTTON_H_
#define MINEBUTTON_H_H
#include <QPushButton>
#include <QMouseEvent>

class mineButton:public QPushButton
{

    Q_OBJECT
private:
    int NO;              //按钮的编号，为0-224，代表这是哪一个按钮的
    bool right_click;    //是否被鼠标点击过,false=未标记，ture=已经标记，
    int  button_type;    /*确定该按钮表示的是地雷还是数字还是空白，地雷=0,1-8为数字，9=空白,未点击过
    初始状态为10*/
    bool enable_click;   //是否接受点击
public:
    mineButton(int , QWidget* parent = 0);  //构造函数
    bool type_button()const;                //返回按钮类型
    bool is_click()const;                   //返回是否已经被鼠标左键点击过
    void resetType(int i = 0);              //设置按钮的类型
    void like_click();                      //设置按钮图片
    void unclick();                         //设置按钮不可被点击，放置游戏结束后再次点击按钮
protected:
    void mousePressEvent(QMouseEvent*);     //鼠标点击事件，设置图片
signals:
    void return_style();
    void find_mine();     //右键成功标记到地雷发射信号
    void get_mine();      //左键点到地雷
    void NO_button(int);  //该信号告诉父窗体，显示该按钮周围的空白按钮

};
#endif
