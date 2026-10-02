#include"mineButton.h"
#include <QDebug>

/*用mineButton所定义的一个对象就是一个按钮，在扫雷时每一个点击的都是一个按钮，即都是用mineButton所定义的
  一个对象。
  可以在按钮中填充文字或者图像的。
*/
mineButton::mineButton(int i , QWidget* parent):QPushButton(parent)
{
    NO = i;               //设置每个按钮单独的编号，按钮的编号是用来区分每一个按钮的
    setFixedSize(36,36);  //设置当前对象的尺寸，当前对象是按钮，则所设置的就是按钮的大小
}

void mineButton::mousePressEvent(QMouseEvent* event)    //鼠标按下事件，
{
    if( event->button() == Qt::LeftButton )//每一个mineButton都有这样的一个函数，当鼠标左键按下时
    {
        if(button_type == 0 and enable_click == true)//如果鼠标左键按下的这个按钮的button_type为0
        {                                            //并且可以被按下的话。
            emit get_mine();  //点到地雷了
            setIcon(QIcon(":/images/bomber.jpg"));//在该按钮中填充地雷的图片
        }
        if(button_type > 0 and button_type < 9 and enable_click == true)//如果是数字，则显示数字图片
        {
            QString name = ":/images/" + QString::number(button_type);/*资源文件中数字图片的名称就是
            该数字，QString::number(button_type)是将当前这个button_type数字转化为字符型的。这样name
            就是对应数字图片的文件夹的路径了。 */
            if (button_type <= 3)
                name = name + ".jpg";
            else
                name = name + ".png";
            setIcon(QIcon(name));
        }
        if(button_type == 9 and enable_click == true)  //如果是空白，显示空白图片
        {
            setIcon(QIcon(":/images/cd.png"));
            emit NO_button(NO);   //无论点击数字还是空白，都发射信号
        }
        enable_click = false;     //左键点击完后，不在接受点击
    }
    if(event->button() == Qt::RightButton)  //当鼠标右键按下时（右键可以打开也可以合上）
    {
        if(right_click == false and enable_click == true)  //如果尚未被右键标记
        {
            setIcon(QIcon(":/images/know.png"));
            right_click = true;  //已经被右键标记
        }
        else if(right_click == true and enable_click == true)
        {
            setIcon(QIcon(":/images/empty.png"));
            right_click = false;  //取消右键标记
        }
    }
    if(right_click == true and button_type == 0)  //右键标记到正确的地雷，发射信号
    emit find_mine();

    setIconSize(size());
    QPushButton::mousePressEvent(event);
}
bool mineButton::type_button()const
{
    //如果按钮被右键标记且该按钮类型为地雷，返回true
    if(right_click == true and button_type == 0)
        return true;
    return false;
}

void mineButton::resetType(int i)
{
    button_type = i;
    right_click = false;
    enable_click = true;  //初始化按钮的各项状态
    setIcon(QIcon(":/images/empty.png"));
    setIconSize(size());
}

void mineButton::like_click()  //该函数用于模拟鼠标点击
{
    if(enable_click == false)  //如果已左键点击过，直接退出
        return;
    if(button_type == 0)
        setIcon(QIcon(":/images/bomber.jpg"));
    if(button_type > 0 and button_type < 9)  //如果是数字，则显示数字图片
    {
        QString name = ":/images/" + QString::number(button_type);
//        qDebug() << button_type << "\n";
        if (button_type <= 3)
            name = name + ".jpg";
        else
            name = name + ".png";
        setIcon(QIcon(name));
    }
    if(button_type == 9)  //如果是空白，显示空白图片
        setIcon(QIcon(":/images/cd.png"));
    setIconSize(size());
    enable_click = false;
}
bool mineButton::is_click() const
{
    if (enable_click == false)
        return true;
    return false;
}
void mineButton::unclick()
{
    enable_click = false;
}
