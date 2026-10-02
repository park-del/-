#include "mainwindow.h"
#include <QMenuBar>
#include <QToolBar>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
    resize(600,400);

//菜单栏：一个窗口只能创建一个
    QMenuBar *bar=menuBar(); //创建一个“菜单栏”，menBar()这个函数会返回一个菜单栏的对象，通过对这个对象
    //的操作就可以对这个窗口中的菜单栏进行操作了（注意：在一个窗口中菜单栏有且只能有一个）。

    setMenuBar(bar); //将菜单栏放入到当前这个窗口中


    QMenu *fileMenu = bar->addMenu("文件");  //在这个菜单栏中添加一个“文件”的“菜单”
    QMenu *editMenu = bar->addMenu("编辑");  //在这个菜单栏中添加一个“编辑”的“菜单”

    fileMenu->addAction("新建");  //在这个菜单栏的“文件菜单”中添加一个“新建”的“菜单项”
    fileMenu->addSeparator();    //在“新建”和“打开”的两个菜单项之间添加一个分割线
    fileMenu->addAction("打开");  //在这个菜单栏的“文件菜单”中添加一个“打开”的“菜单项”
    editMenu->addAction("新建");  //在这个菜单栏的“编辑菜单”中添加一个“新建”的“菜单项”



//工具栏：一个窗口是可以创建多个工具栏的
    QToolBar *toolBar= new QToolBar(this); /*创建一个工具栏，同时指明这个工具栏的父亲为当前这个窗口
    注意：工具栏和按钮不一样，按钮指明了父亲之后就会贴在该窗口上，但是工具栏却不是这样的。*/
/*    addToolBar(toolBar);  工具栏要想贴在窗口上必须调用左边的这个函数，函数参数为要贴的工具栏对象。
      但在左边的这个函数默认是将工具栏贴在窗口的上部的，可以利用帮助文档查看该函数的功能，可以看到该函数
      是可以修改默认工具栏贴在的窗口的位置的（该函数是有多个重载函数的，每一个重载函数可以完成一些不同的
      功能）*/
    addToolBar(Qt::LeftToolBarArea,toolBar); /*将工具栏toolBar贴在这个窗口上，且默认贴的位置是
    窗口的左边*/

   toolBar->setAllowedAreas(Qt::LeftToolBarArea |Qt::RightToolBarArea); //设置工具栏只能够左右停靠
   toolBar->setFloatable(false); //设置工具栏不能浮动，即是一撒手工具栏就会回到左边或者右边。

   toolBar->setMovable(false); /*总开关，将这个设置为false后工具栏就不能够移动了，工具栏将会一直停留在左方
   或者右方或者上方或者下方，具体停靠在哪取决于上面函数所设置的位置。*/

   toolBar->addAction("新建");  //在工具栏中添加“新建”项
   toolBar->addSeparator();    //在工具栏中上方的“新建”项与下方的“打开”项之间添加分割线
   toolBar->addAction("打开"); //在工具栏中添加“打开”项

   QPushButton *btn=new QPushButton("aa",this);
   toolBar->addWidget(btn);  //可以在工具栏中添加控件，如在工具栏中添加一个按钮

/*
  总结：
      菜单栏：
      (1)调用menuBar()函数会返回一个 QMenuBar的对象，这个对象就是菜单栏，通过一个指针bar指向了这个菜单栏。
         对bar的操作就是对菜单栏的操作。
      (2)QMenu定义的一个对象就是一个菜单，一个菜单栏中是有多个菜单的，故可以定义多个菜单，这些菜单是按照从左
         到右的顺序依次排列在菜单栏中的。
         对这用QMenu定义的某一个对象所进行的操作就是对菜单栏中某一个菜单的操作，可以调用该对象的成员函数就能
         够完成对菜单相应的设置了。如 对象.addMenu("文件")就是对这个菜单起一个名字；对象.addAction("新建")
         就是在这个菜单中添加一个新建的菜单项；对象.addSeparator()就是在这个菜单后面添加一个分隔线。

         注意：setMenuBar(bar);是将这个菜单栏放入到当前的窗口中，没有放入的话当前的窗口中是不会显示菜单栏的，
              setMenuBar(bar);相当于是this->setsetMenuBar(bar);其中this表示的是当前窗口的意思。
              这写菜单栏的设置都是在窗口的默认构造函数进行的，故在创建一个窗口时是会自动执行的。

      工具栏：
      (1)QToolBar定义的一个对象就是一个工具栏，用QToolBar定义了多少对象就定义了多少工具栏，对这些对象的
         的操作就是对这些工具栏的操作。
      (2)可以通过 对象.setAllowedAreas()来设置这个工具栏在打开窗口时默认显示的位置。
         可以通过 对象.setFloatable(false)来设置这个工具栏不能浮动，此时工具栏是只能停在上下左右四条边上的。
         可以通过 对象.setMovable(false);来设置这个工具栏不能够移动，此时工具栏是只能显示在工具栏在窗口中
                 默认显示的位置的。
         注意：区分移动和浮动，可以浮动指的是可以在窗口的屏幕上显示，不能浮动指的是不能够在屏幕的屏幕上显示，
              但是却可以对它进行移动到窗口的四周的，即不能浮动也是可以移动的，不能移动的话就一点都不能动，
              无论是什么动，都只能在窗口的默认位置上显示。

         可以通过 对象.addAction("新建");来为这个工具栏增加一个新建的工具。
         可以通过 对象.addSeparator();来为这个工具栏添加一个分隔线。
         可以通过 对象.addWidget(btn);来为这个工具栏添加一个btn的控件，如果btn是按钮的话，则就是为这个工具
                 栏添加了一个按钮。

  注意：上述程序中setMenuBar(bar);和addToolBar(Qt::LeftToolBarArea,toolBar);中的对象其实是this，
       this指的是当前这个窗口，this调用相应的成员函数指的是当前这个窗口进行相应的操作。
       如this->setMenuBar(bar);就是当前这个窗口添加bar这个菜单栏。
         this->addToolBar(Qt::LeftToolBarArea,toolBar);就是当前这个窗口左边添加toolBar这个工具栏。
      其中，添加的菜单栏和工具栏分别又是一个对象的，可以对它们进行操作（即调用它们的成员函数）来完成对要添加
      的这个菜单栏和工具栏的属性一些设置。

  注意：只有菜单栏和菜单才有对象，菜单项只是菜单中的内容，菜单项是没有对象，也就没有与之相对应的类；
       工具栏有对象，而工具栏中的内容是没有对象的，也就没有与之对应的类。
       所谓的什什么栏就是贴在窗口上的的框框。
*/
}

MainWindow::~MainWindow()
{
}

