#include "mainwindow.h"
#include <QStatusBar>
#include <QLabel>
#include <QDockWidget>
#include <QTextEdit>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
    resize(600,400);


 //状态栏：状态栏只能有一个
    QStatusBar *stBar=statusBar();  //定义了一个状态栏对象stBar，stBar就是所定义的状态栏
    setStatusBar(stBar);            //将这个状态栏贴在这窗口上

    QLabel *laber1=new QLabel("提示信息",this); /*定义了一个标签对象laber1，指明了这个标签的内容为
    “提示信息”，为这个标签指明了一个父类，这个标签是贴在这个窗口上的（为标签指明了父类后标签就会贴在这个
    窗口上了）。
    注意：标签和按钮是一样的，都是控件，为一个控件指明了其父类后这个控件就会贴在这个父类上。如为标签和按钮
    指明了其父类为某一个窗口时，该标签和按钮就会自动贴在这个窗口上。
    标签和按钮
    */
    stBar->addWidget(laber1); //将这个标签laber1贴在stBar这个状态栏中，这是贴在状态栏的左侧的

    QLabel *laber2=new QLabel("右侧提示信息",this); //定义了一个标签对象laber2
    stBar->addPermanentWidget(laber2);//将这个标签贴在laber2这个状态栏中，这是贴在状态栏的右侧的

 //铆接部件（也叫浮动窗口）：可以有多个
    QDockWidget *dockwidget=new QDockWidget("浮动"); /*铆接部件就是浮动窗口，浮动窗口也是一个控件
    ,故为某一个浮动窗口指明了一个父类窗口后，这个浮动窗口也就会贴在这个父类窗口上,也可以不指明这个浮动窗口
     的父类，由于下面有为这个窗口添加浮动窗口的设置，故该浮动窗口也将会贴在这个窗口上*/
    addDockWidget(Qt::BottomDockWidgetArea,dockwidget);  /*设置这个浮动窗口dockwidget在当前这个
  this窗口中的默认的位置为BottomDockWidgetArea，即是浮动窗口所停靠在窗口中的位置是在中心部件下面的，注意，
  是中心部件的下面，而不是窗口的下面，当窗口中没有中心部件时，浮动窗口就是在窗口的上面的（没有中心部件时，浮动
  窗口在窗口的上面也就是浮动部件在中心部件的下面）*/
    dockwidget->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea); /*利用上面
    的addDockWidget(Qt::BottomDockWidgetArea,dockwidget)设置的是这个窗口的默认的位置，但是这个窗口是
    可以移动到其他的位置的，利用dockwidget->setAllowedAreas(Qt::TopDockWidgetArea |
    Qt::BottomDockWidgetArea);对浮动窗口dockwidget进行设置后该浮动窗口只能移动到中心部件的上面或者下面，
    而该浮动窗口是不能够移动到中心部件的左边或者右边的。  */


 //设置中心部件：只能有一个
    QTextEdit *edit=new QTextEdit(this);  //定义一个指向中心部件的指针edit，暂时认为edit就是中心部件
    setCentralWidget(edit);    //edit指向了中心部件，在一个窗口中是只能有一个中心部件的。
    /*setCentralWidget(edit)是this这个对象调用的成员函数，该成员函数的功能是为this这个对象，即当前这个
      窗口设置一个核心edit，由于一个窗口是只能设置一个核心的，故接下来this这个窗口就不能够再设置核心了。*/

   /*
     QStatusBar定义的一个对象就是一个状态栏    （贴在窗口）
     QLabel定义的一个对象就是一个标签          （贴在窗口或者）
     QDockWidget定义的一个对象就是一个浮动窗口 （贴在窗口）
     QTextEdit定义的一个对象就是一个中心部件   （贴在窗口）

     另外浮动窗口和标签都是控件，控件是可以直接通过指明其父亲窗口来贴在这个窗口中的。

     与前面所说的菜单栏和工具栏一样，状态栏和浮动窗口和中心部件都是贴在这个窗口上的，故调用的是窗口的成员函数。
     即是this的成员函数：
         setStatusBar(stBar);          //为这个窗口设置一个stBar的状态栏
         addDockWidget(dockwidget);    //为这个窗口添加一个dockwidget的浮动窗口
         setCentralWidget(edit);       //为这个窗口设置一个edit的中心部件
         这些成员函数的对象都是this，故是这个窗口所设置或者添加的什么东西。

         stBar->addPermanentWidget(laber2); //这个状态栏添加一个标签
         dockwidget->setAllowedAreas(Qt::TopDockWidgetArea);//这个浮动窗口设置可以移动的位置
         以上中的stBar对象是一个状态栏对象，dockwidget对象是一个浮动窗口对象，故stBar调用其成员函数
         是用来设置stBar这个状态栏的一些属性的，dockwidget调用其成员函数是用来设置dockwidget这个浮动
         窗口的一些属性的。

         注意：状态栏是有对象的，而状态栏中的内容为标签，标签也是有对象的，故状态栏和标签都是有相应的类与之
              对应的。
              浮动窗口是有对象的，中心部件也是有对象的。
    */


}

MainWindow::~MainWindow()
{
}

