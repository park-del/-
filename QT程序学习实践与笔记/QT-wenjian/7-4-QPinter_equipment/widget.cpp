#include "widget.h"
#include "ui_widget.h"
#include <QPixmap>
#include <QPainter>
#include <QImage>
#include <QPicture>
Widget::Widget(QWidget *parent): QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);

//QPixmap绘图设备：可以用来定义一个纸张，在这个纸张上做的画可以保存在电脑中的磁盘上
    QPixmap pix(300,300); /*用Qpixmap定义了一个绘图设备（所谓的绘图设备就是画家要在什么上来画画），
    画家可以在窗口上画画，此时的绘图设备就是窗口，即是this。画家也是可以在其他的绘图设备上来画画的,如
    这里用QPixmap所定义的一个对象就是一个绘图设备，这个绘图设备就是一张纸，即定义了一个尺寸为300*300
    一个纸张pix，画家在这张纸上画好画了以后可以将这张画保存在C盘或者D盘上。
    如Pix.("D:\\pix.png")或者Pix.("D:/pix.png")就是将这张纸保存在D盘中pix.png的文件中的，纸上
    画好了画以后对纸张的操作也就是对画的操作。
    */

    pix.fill(Qt::white);             //设置这个纸张的填充色为白色，不设置的话默认是为黑色的。
    QPainter painter1(&pix);          //设置画家painter是在这个纸张pix上来进行画画的
    painter1.setPen(QPen(Qt::green)); //给画家声明了一个绿色的画笔(声明画笔时采用了匿名的对象)
    painter1.drawEllipse(QPoint(150,150),100,100); //画家在这张纸上画一个圆
    pix.save("D:\\QT-画家\\pix.png"); /*此时画家已经在这张纸上画好了画，对这张纸的操作也就是对这幅画
    的操作。 pix.save("D:\\QT-画家\\pix.png");是将这幅画保存在D盘中的QT-画家文件夹中的pix.png文件
    中的。
    注意：文件夹是不会自动创建的，文件是会自动创建的，即当D盘中没有QT-画家这个文件夹时在使用该路径时必须自
         己事先手动在该D盘中添加一个，而pix.png相当于是这幅画的名字，是不需要用户自己去手动添加的，是由
         系统自动添加的。
    */

/*QImage绘图设备：可以用来定义一个纸张，在这个纸张上做的画可以保存在电脑的磁盘上，不过其比QPixmap的绘图
  设备的功能要更加的强大，用其定一的指针可以用来填充像素点，如img是QImag绘图设被所定义的一个纸张，
  可以通过img.setPixel(i,j,value);来对这个绘图设备上的(i,j)位置填充像素点。
  value是用QRgb定义的一个对象，用QRgb定义的一个对象是一个像素点的类型，可以在定义时就指明这个像素点的颜色
  的。         */
    QImage img(300,300,QImage::Format_RGB32);//设置这个张纸img的大小和格式，常用RGB32的
    img.fill(Qt::white);

    QPainter painter2(&img);
    painter2.setPen(QPen(Qt::blue));
    painter2.drawEllipse(QPoint(150,150),100,100);
    img.save("D:\\QT-画家\\img.png");

//注意：在窗口上画画时要在该窗口的成员函数，即绘图事件这个成员函数中来完成；在除窗口外的其他绘图设备上画画
//时需要在该窗口的构造函数中来进行，而不是在绘图事件中进行，这一点要区分开。


//QPicture绘图设备：可以记录和重现绘图指令
     QPicture pic;
     QPainter painter3; //在创建这个画家的时候可以先不指定这个画家的绘图设备，后期利用begin()这个
     //函数也会可以指定的。
     painter3.begin(&pic);//begin()说明了这个画家开始画画了，此时为其指明了记录画画指令的一个设备pic
     painter3.setPen(QPen(Qt::cyan));
     painter3.drawEllipse(QPoint(150,150),100,100);
     painter3.end();      //end()说民这个画家已经将该幅画画完了，此时pic完成了对绘图指令的记录
     pic.save("D:\\QT-画家\\pic.zt"); //将pic中记录的绘图指令保存在一个.zt的文件中去
 /*
 注意:QPixmap和QImage绘图设备都是用来进行画画用的，它们所所定义的纸张应该是一个图片的的格式，故它们所
     定义纸张的文件名的后缀应该是.png; QPicture不是用来画画用的，而是用来记录和保存画画的指令用的，
     QPicture定义的对象也是一张纸，但是这张纸上的内容不是图片，而是该对象所记录的画画的一堆指令，这张
     纸存放时的文件格式不是为图片格式的(.png)，而是为.zt的格式的。

     pic所记录的绘画指令是从begin(&pic)开始到end结束的，即pic这个记录对象总共记录了下面这些指令
     painter3.setPen(QPen(Qt::cyan));
     painter3.drawEllipse(QPoint(150,150),100,100);
     即pic所记录的就是画家在(150,150)位置处用cyan颜色画出一个半径为100的圆
 */
}

void Widget::paintEvent(QPaintEvent *)
{
 /*QImage绘图设备所对应的代码
    QPainter painter(this);  //声明一个在窗口上画画的画家

    //利用Qiamge 对像素进行修改
    QImage img;  //定义一张空白纸
    img.load(":/photo/turtle.png"); //这张纸上加载图片（现在这张纸上已经有一幅画了）

    for(int i=50;i<100;i++)
    {
        for(int j=50;j<100;j++)
        {
            QRgb value=qRgb(255,0,0);//声明一个红色的像素点value，value被指明为红色的
            img.setPixel(i,j,value); //在这幅画中从(50,50)处开始填充红色的点
        }
    }

    painter.drawImage(0,0,img);   //画家在窗口的(0,0)位置处将这幅画贴在窗口上
    //注意:画家执行drawImage()的操作所画的就是一个imag的图片。
*/

    //QPicture绘图设备所对应的代码（完成对保存在某一个.zt的文件中的绘图指令的重现）
    QPainter painter(this);
    QPicture pic1;
    pic1.load("D:\\QT-画家\\pic.zt");/*pic1加载了pic.zt这个文件中的绘图指令，此时的pic1就已经
    包含了这些被记录的指令，可以利用pic1这个对象来对pic.zt中所包含的绘图指令进行重现。*/
    painter.drawPicture(50,50,pic1); /*painter执行drawPicture(0,0,pic1)的意思就是在指定的
    窗口或者绘图纸张（这里是窗口）的(0,0)位置处执行pic1所记录的绘图指令(pic1所记录的绘图指令就是
    画家在(150,150)位置处用cyan颜色画出一个半径为100的圆)。*/


//执行下面这段代码发现其与上面的圆不重合，说明了画家painter执行pic1的指令时是将窗口的(50,50)处当做原点
//重新建立坐标系的，而并不是从窗口的(0,0)处为原点的坐标的，要是从窗口的(0,0)出为原点的坐标的话，这两个圆
//就会重合了，但实际上两个圆是并没有重合的。
    QPainter painter0(this);
    painter0.setPen(QPen(Qt::cyan));
    painter0.drawEllipse(QPoint(150,150),100,100);
}

Widget::~Widget()
{
    delete ui;
}

