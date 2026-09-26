#ifndef EDITRECORDDLG_H
#define EDITRECORDDLG_H

#include <QtGui>

class EditRecordDlg : public QDialog
{
    Q_OBJECT
public:
    EditRecordDlg(QWidget *parent=0);
 
public slots:
    	void okClicked();
	void cancleClicked();
    
private:
    	QLabel *idLabel;
	QLabel *nameLabel;
	QLabel *producerLabel;
	QLabel *priceLabel;

	QLineEdit *idEdit;
	QLineEdit *nameEdit;
	QLineEdit *producerEdit;
	QLineEdit *priceEdit;

	QPushButton *okButton;
	QPushButton *cancleButton;
};

#endif 
