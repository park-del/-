#include <QDialog>
#include <QTextEdit>
#include <QCloseEvent>
class GameInstruction:public QDialog
{

private:
    QTextEdit  *instruction;
public:
    GameInstruction(QWidget *parent = 0);
protected:
    void closeEvent(QCloseEvent *);  //定义了一个关闭的事件

};
