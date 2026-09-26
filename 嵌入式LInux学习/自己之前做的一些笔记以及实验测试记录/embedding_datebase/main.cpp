#include <QApplication>
#include "qtranslator.h"
//#include "mainform.h"

#include "managementform.h"
int main(int argc, char *argv[])
{
QApplication app(argc, argv);

//Qt国际化的步骤
QTranslator translator(0);
translator.load("qr_zh",".");
app.installTranslator(&translator);

//MainForm *dialog = new MainForm;
ManagementForm *dialog=new ManagementForm;  //显示的是MangementForm类的一个窗口
dialog->show();
return app.exec();

}
