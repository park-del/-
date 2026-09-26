#include "EditRecord.h"
//extern是用来声明外部变量的
extern bool ok_flag;
extern char edit_id[10];
extern char edit_name[20];
extern char edit_producer[20];
extern int  edit_price;

//这里所定义的EidtRecordDlg窗口是为点击管理窗口时的“增加记录”、“修改记录”时所弹出的窗口“编辑记录”和“修改记录”的窗口

EditRecordDlg::EditRecordDlg(QWidget *parent)
	: QDialog(parent){

    setWindowTitle(tr("Edit a record"));
    
	//编号标签
    idLabel = new QLabel(tr("ID:")); //创建一个idLabel标签，idLabel标签中的内容设置的是
	idEdit = new QLineEdit;          //
    idEdit->setText(edit_id);        //
    idEdit->setEnabled(true);        //设置idEdit这个编辑框可编辑

	//名称标签
    nameLabel = new QLabel(tr("name:"));
    nameEdit = new QLineEdit;       
    nameEdit->setText(edit_name);
    nameEdit->setEnabled(true);

	//厂商标签
    producerLabel = new QLabel(tr("producer:"));
    producerEdit = new QLineEdit;
    producerEdit->setText(edit_producer);
    producerEdit->setEnabled(true);

	//价格标签
    priceLabel = new QLabel(tr("price:"));
    priceEdit = new QLineEdit;
    
	char s[10];
	memset(s,0,10);
    sprintf(s,"%d",edit_price);
    priceEdit->setText(s);
    priceEdit->setEnabled(true);

    okButton = new QPushButton(tr("OK"));
    cancleButton = new QPushButton(tr("Cancle"));

    connect(okButton,SIGNAL(clicked()),this,SLOT(okClicked()));         //ok按钮信号槽的连接
    connect(cancleButton,SIGNAL(clicked()),this,SLOT(cancleClicked()));  //cancel按钮信号槽的连接
   

//下面所创建的网格布局框left位于所弹出的窗口中的左边，垂直布局框right位于所弹出的窗口的右边的

    QGridLayout *left = new QGridLayout;  //创建了一个网格布局框left
	left->addWidget(idLabel,0,0); //所弹出的窗口是按照一个标签、一个编辑框来编排的
    left->addWidget(idEdit,0,1);

    left->addWidget(nameLabel,1,0);
    left->addWidget(nameEdit,1,1);

    left->addWidget(producerLabel,2,0);
    left->addWidget(producerEdit,2,1);

    left->addWidget(priceLabel,3,0);
    left->addWidget(priceEdit,3,1);
    
	QVBoxLayout *right = new QVBoxLayout; //创建了一个垂直布局框right
    right->addWidget(okButton);           //垂直布局框中放入确定和取消按钮
    right->addWidget(cancleButton);
    
	QHBoxLayout *mainlayout = new QHBoxLayout(this); //创建了一个水平布局框，同时指明了这个水平布局框是位于当前窗口中的
    mainlayout->addLayout(left); //水平布局框中添加网格布局框和垂直布局框
    mainlayout->addLayout(right);
}

void EditRecordDlg::okClicked()  //点击弹出的“增加记录”或者“删除记录”窗口中的“确定”按钮时会执行的槽函数
{
	ok_flag=1;
	memset(edit_id,0,sizeof(edit_id));
	memset(edit_name,0,sizeof(edit_name));
	memset(edit_producer,0,sizeof(edit_producer));
	
	edit_price=0;
	/*
    sprintf(str, "Pi 的值 = %f", M_PI);
    puts(str);

    输出结果：
	Pi 的值 = 3.141593

    sprintf(edit_id,"%s",((idEdit->text()).toLatin1()).data())这句话的作用就是将
	idEidt这个编辑框中的内容放置到edit_id中。

    其他的也是一样的，这样：
	edit_id：存放编辑框idEdit中输入的数据。
	edit_name:存放编辑框nameEdit中输入的数据。
	edit_producer:存放编辑框producerEdit中输入的数据。
    */
	sprintf(edit_id,"%s",((idEdit->text()).toLatin1()).data());
	sprintf(edit_name,"%s",((nameEdit->text()).toLatin1()).data());
	sprintf(edit_producer,"%s",((producerEdit->text()).toLatin1()).data());

	edit_price=(priceEdit->text()).toInt(); //edit_price存放编辑框priceEdit中输入的数据
 
	close();  //点击确定之后就会关闭这个弹出的窗口
}
void EditRecordDlg::cancleClicked() //点击弹出的“增加记录”或者“删除记录”窗口中的“取消”按钮时会执行的槽函数
{
    close();  //执行close()函数会关闭当前这个窗口
}

