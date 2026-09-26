#ifndef EDITRECORDDLG_H
#define EDITRECORDDLG_H

#include <QtGui>  //可以调用QtGui里面的一些东西，如QDialog对话框，信号与槽的机制

class EditRecordDlg : public QDialog
{
    Q_OBJECT
public:
    EditRecordDlg(QWidget *parent=0);
 
public slots:    //槽函数
    
	void okClicked();   
	void cancleClicked();
    
private:  //定以了QLable标签、QLineEdit单行文本框、QPushButton按钮
    
	QLabel *idLabel;
	QLineEdit *idEdit;
		
	QLabel *nameLabel;	
    QLineEdit *nameEdit;
	
	
	QLabel *producerLabel;
	QLineEdit *producerEdit;

	QLabel *priceLabel;
	QLineEdit *priceEdit;

	QPushButton *okButton;
	QPushButton *cancleButton;
};

#endif 
