#include <QtGui>
#include "managementform.h"
#include "EditRecord.h"   //可以使用这个EditRecord类来定义一个弹出的窗口
enum
{
        TABLE_IMPORT,
        TABLE_EXPORT,
	    TABLE_STORAGE
};

const char *table[3]={"import","export","storage"};
bool ok_flag=false;

char edit_id[10];
char edit_name[20];
char edit_producer[20];
int  edit_price;

ManagementForm::ManagementForm(QWidget *parent)
: QDialog(parent)
{
	ui.setupUi(this);  //ui界面放置到当前这个窗口中
	ui.increaseradioButton->setChecked(true);  //“升序”按钮初始时被选中
	ui.storageradioButton->setChecked(true);   //“降序”按钮初始时被选中
	
	selected_radio=0; //刚开始是升序，这是选中的是升序还是降序的一个标志

	table_status=TABLE_STORAGE;  //初始时表的状态为“库存”状态
	
	sql_for_tableWidget("data.db","select * from storage;"); /*在data.db数据库中查询storage表中的
	所有内容，并冲新设置ui界面中的tableWidget控件所显示的行数，这样多余的就会被清除掉，将查询到的
	字段显示在tableWidget控件中。*/
}
 
//data.db这个数据库中是有3张表的，分别是为storage表、import表、export表。

void ManagementForm::on_importRecordradioButton_clicked()  //“入库单”按钮被点击时
{
 
	table_status=TABLE_IMPORT;   //修改表的状态为“入库”
	sql_for_tableWidget("data.db","select * from import;"); /*将查询得到的“data.db”数据库中的import
	表中的所有字段显示在tableWidget控件中。多余的行被清楚掉，使用过设置tableWidget控件的行数来将多余的行
	给清除掉的。*/
	return;
}
void ManagementForm::on_exportRecordradioButton_clicked() //“出库单”按钮被点击时
{ 
	table_status=TABLE_EXPORT;   //修改表的状态为“出库”
	sql_for_tableWidget("data.db","select * from export;"); /*将查询得到的“data.db”数据库中export表中的所有字段
	显示在tableWidget控件中。同样原来tableWidget控件中多余的行也是会被清除掉的。*/
    
	return;
}
void ManagementForm::on_storageradioButton_clicked()  //“库存”按钮被点击时
{
        
	table_status=TABLE_STORAGE;  //修改表的状态为“库存”，刚开始时表的状态就是为“库存的”
	sql_for_tableWidget("data.db","select * from storage;");
    return;
}
 
void ManagementForm::on_refreshStorageButton_clicked()  //“更新库存”按钮被点击时
{
       
	table_status=TABLE_STORAGE;
	repaint();

	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	rc = sqlite3_open("data.db", &db);  //打开data.db这个数据库
	if( rc ){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(1);
	}

	char **dbResult_import;   
	int nRow_import, nColumn_import;

	char **dbResult_export;
	int nRow_export, nColumn_export;
	rc = sqlite3_get_table(db,"select * from import;", &dbResult_import, &nRow_import,&nColumn_import, &zErrMsg);  /*查询import表中所有的记录，将查询到的结果保存在
	dbResult_import一维数组中。*/
	if( rc!=SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}	
	rc = sqlite3_get_table(db,"select * from export;", &dbResult_export, &nRow_export,&nColumn_export, &zErrMsg);/*查询export表中所有的记录，将查询到的结果保存在
	dbResult_export一维数组中。*/
	if( rc!=SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}

	
        rc = sqlite3_exec(db,"create table storage(ID varchar(10),name varchar(20),producer varchar(20),price smallint);", 0, 0, &zErrMsg); //创建一张storage表
        if( rc!=SQLITE_OK ){
                fprintf(stderr, "SQL error: %s\n", zErrMsg);  //创建失败的话，则说明了storage表已经存在了，就会执行这个if语句里面的内容
                sqlite3_free(zErrMsg);
        }
	
        rc = sqlite3_exec(db,"delete from storage;", 0, 0, &zErrMsg); /*如果“delete from storage”后面指明了where子句，则就是删除指定的元组，如果没有指明where子句的话则就是
        删除表中所有的记录的。*/
        if( rc!=SQLITE_OK ){
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
        }


	int i,j;
	int index_import=nColumn_import;  //index_import初始时指向import表中第1条记录中第1个字段
	int index_export=nColumn_export;  //index_export初始时指向export表中第1条记录中第1个字段

	//由于是“select *”，即是查询表中所有的记录，故将下面的查询import表中所有的记录直接叙述为import表中所有的记录，将下面查询export表中所有的记录直接叙述为export表中所有的记录
	for(i=0; i<nRow_import; i++){     //执行import表中记录个数次循环
		
		bool out_flag=false;          //mark for export.
		
		index_export=nColumn_export;  //index_export依次指向出货表中的每条记录中的第1个字段
		
		/*
        库存中的每一条记录都是有一个编号，且是唯一的，这里是只将进货表的编号字段依次与出货表的编号字段依次进行比较的。
        
		刚开始是进货表中的index_export指向了进货表中的第1个字段，然后将第1个字段依次与出货表中的所有记录的第1个字段进行比较，
		如果相同的话就返回0，将out_flag置为0，执行break跳出for循环，此时出货以进货刚好抵消了，是不需要往storage表中插入数据的；
		如果将进货表中的一条记录的第1个字段依次与出货表中每一条记录的第1个字段相比较发现都不相同的话，此时就会正常退出这个while循环，
		是不会执行if语句里面的内容的，此时是会执行if(!out_flag)语句里面的内容的，将进货表中这一行中的字段插入到storage表中。
        */


		for(j=0;j<nRow_export;j++)    //执行export表中记录个数次循环（只将进货表中一条记录的第1个字段依次与出货表中的每条记录的第1个字段相比较）
		{
			if(strcmp(dbResult_import[index_import],dbResult_export[index_export])==0) /*进货表中的某一条记录的第1个字段与出货表中的某条记录第1个字段相等的话，置out_flag=true
			，同时跳出循环，不会向这个storage表中插入记录。*/
			{
				out_flag=true;
				break;
			}
			index_export+=nColumn_export;  //控制index_export依次指向的是为出货表中的每1条记录的第1个字段
		}

		if(!out_flag)
		{
			char ss[100];memset(ss,0,100);
            sprintf(ss,"insert into storage values('%s', '%s', '%s', %d);",dbResult_import[index_import],dbResult_import[index_import+1],dbResult_import[index_import+2],atoi((const char*)dbResult_import[index_import+3]));//currently support 4 columns. make perfect later.
            rc = sqlite3_exec(db,ss, 0, 0, &zErrMsg);
	
		}
		index_import+=nColumn_import;     //控制index_import依次指向的是为进货表中的每1条记录的第1个字段
	}

	//create new table storage end
	sqlite3_close(db);
	sql_for_tableWidget("data.db","select * from storage;");  //执行这个指令会将从data.db数据库中的storage表中查询到的所有元组放到ui界面中的tableWidget这个控件中
    return;
}

/*
MangementForm类中的sql_for_tableWidget()成员函数中的第1个参数（字符串）是为要操作地数据
库的名称的，第2个参数（字符串）是为要执行的SQL语句的。
*/
void ManagementForm::sql_for_tableWidget(char * fp, char * sql_str) 
{	
	repaint();
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	rc = sqlite3_open(fp, &db); //db指针指向名称为“fp字符串”的数据库
	
	if( rc ){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(1);
	}

	char **dbResult;
	int nRow, nColumn;

	rc = sqlite3_get_table(db,sql_str, &dbResult, &nRow,&nColumn, &zErrMsg);
	/*
	执行查询的结果，一维的字符串数组dbResult存放查询的结果（当二维字符数组dbResult
	当做是一维的字符串数组来进行处理）
    */

	if( rc!=SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}

	sqlite3_close(db); //关闭db指针指向的数据库，此时dbResult字符串数组中已经存放了查询的结果了

	int i,j;

	/*
	是要将查询到的所有的字段在tableWidget中的对应位置上进行更新的，这里采用的方法
	是将
	*/
	int index=nColumn;
	ui.tableWidget->setRowCount(nRow);  //设置ui界面的行数
	for(i=0; i<nRow; i++){
		for(j=0;j<nColumn;j++){
			QTableWidgetItem *item = new QTableWidgetItem(dbResult[index]); /*创建一个表格中单元格
			的对象item，这个  */
			ui.tableWidget->setItem(i, j, item); /* ui界面中的名称为tableWidget的控件调用setItem()
			成员函数更新该控件中的第i行，第j列中的内容为item。*/
			index++;
		}
	}

        return;
}

const char *column[4]={"ID","name","producer","price"};
const char *con[2]={"and","or"};
const char *radio[2]={"asc","desc"};



//这里是实现过滤查询的
void ManagementForm::on_querypushButton_clicked()    //点击“查询”按钮会触发的槽函数
{
	char sql_str[150];      //定义字符串sql_str
	memset(sql_str,0,150);  //内存清空的操作，对sql_str这个数组的150个空间全部赋值为0
	
	char sql_str1[35];      //定义字符串sql_str1
	memset(sql_str1,0,35);
	
	sprintf(sql_str1,"select * from %s where",table[table_status]); /* sql_str1这个字符串的内容就是“select * from table[status] where”
	table_status的值是为TABLE_IMPORT、TABLE_EXPORT、TABLE_STORAGE的。
	TABLE_IMPORT、TABLE_EXPORT、TABLE_STORAGE是为所定义的一个枚举值的，其中TABLE_IMPORT为0、TABLE_EXPORT为1、TABLE_STORAGE为2。
	
	则此时：
	table[TABLE_IMPORT]就是为table[0]，这个字符串中的内容就是为“import”。
    table[TABLE_EXPORT]就是为table[1]，这个字符串中的内容就是为“export”。
	table[TABLE_STORAGE]就是为table[2]，这个字符串中的内容就是为“storage”。
	*/

	QString s =  ui.textlineEdit->text();     //获取过滤查询中第1个单行编辑框中的内容，存放到字符串S中
	QByteArray ba = s.toLatin1();             //S转换编码个数之后存放到ba中

	char sql_str2[40];memset(sql_str2,0,40);  //定义sql_str2数组
	sprintf(sql_str2,"%s='%s'",column[ui.columncomboBox->currentIndex()],ba.data());   /* ui.columncomBox是为ui界面中的“编号、名称、厂商”这个选项框的，这里是获得这个选
	项框中所选择的内容的索引的，即这个选项框中所选择的是为“编号”对应的索引，还是“名称”对应的索引，还是“厂商”对应的索引。
	“编号”对应的索引为0：column[0]为“ID”
	“名称”对应的索引为1：column[1]为“name”
	“厂商”对应的索引为2：column[2]为“producer”

     column[0],column[1],column[2]中的内容刚好是为storage表、import表、export表中的前3个字段的。
	
	 sql_str2这个字符串中的内容就是“ID=？”或者“name=？”或者“producer=？”，刚好是为sql_str1后面的where子句的内容。
	 注意：上述中的“？”是为ba.data()的，即ui.textlineEdit->text()的，就是过滤查询布局框中的第1个单行编辑框中输入的内容。
	 */
	
	QString s1 =  ui.littlelineEdit->text();  //获取过滤查询中“小于价格”单行编辑框中的内容s1
	QString s2 =  ui.biglineEdit->text();     //获取过滤查询中“大于价格”单行编辑框中的内容s2
	
	char sql_str3[15];memset(sql_str3,0,15);  //定义字符串sql_str3
	
	if(s1.length())    //“小于价格”单行编辑框中的内容不为空的话就会执行这个if语句里面的内容，也即是在这个单行编辑框中输入了数据的话。
	{
		sprintf(sql_str3,"price>%d",s1.toInt());  //s1.toInt()是将字符类型的数字转换为数值类型的数字的，sql_str3这个字符串为“price>34”（假设向这个单行编辑框中输入的内容为34）
	}

	char sql_str4[15];memset(sql_str4,0,15);
	
	if(s2.length())   //“大于价格”单行编辑框中的内容不为空的话就会执行这个if语句里面的内容，也即是这个单行编辑框中输入了数据的话。
    {
        sprintf(sql_str4,"price<%d",s2.toInt()); //sql_str4这个字符串为“price<34”（假设向这个单行编辑框中输入的内容为34）
    }
	
	char sql_str5[15];
	memset(sql_str5,0,15);      
	sprintf(sql_str5,"order by %s",column[ui.columncomboBox2->currentIndex()]); /*  ui.columncomboBox2就是ui设计界面中的第2个选项框的，这里是获得第2个选项框中当前值的索引的，
	此时的sql_str5这个字符串就为“order by ID”或者“order by name”或者“order by producer”的。*/
        
	char sql_str6[15];
	memset(sql_str6,0,15);
	sprintf(sql_str6,"%s",radio[selected_radio]);  /* selected_radio为两个值的，为0或者为1，为0的话就表示升序，为1的话就表示降序。
	即radio[0]这个字符串为“asc”，radio[1]这个字符串为"desc"。  
	sql_str6这个字符串就是为“asc”或者“desc”的。（“asc”表示的是升序的意思，“desc”表示的是降序的意思） */
        
	/*
    sql_str  : “完整的SQL语句，由下面的sql_str1、sql_str2、sql_str3、sql_str4、sql_str5拼接构成”
	sql_str1 : “select * from table[status] where“（通过status，即当前表的状态，就是在当前窗口的tableWidget控件中显示的是哪张表的内容，那么就会对哪张表执行查询的操作）
	sql_str2 : “ID=? 或者 name=? 或者 producer=?”（sql_str1中where后面的子句）
	sql_str3 : “price>?”
	sql_str4 : “price<?”
    sql_str5 : “order by id 或者 order by name 或者 order by producer ”
	sql_str6 : “asc 或者 desc”

    在过滤布局框中从左到右，从上到下，s这个字符串存放的是第1个单行编辑框中的内容，s1这个字符串存放的是第2个单行编辑框中的内容，s2这个字符串中存放的是第3个单行编辑框中的内容。
    */

     if (s.length())  //第1个单行编辑框中输入有数据的话，即是“where id=?”中的这个“?”存在
	{
		if((!s1.length()) && (!s2.length())){   //如果s1和s2中都没有输入数据的话
			sprintf(sql_str,"%s %s %s %s;",sql_str1,sql_str2,sql_str5,sql_str6);  //sql_str这个字符串为“select * from table[status] where | ID=? | order by id| asc”
		}
		else if((!s1.length()) && s2.length()){ //如果s1中没有输入数据，而s2中有输入数据的话
			sprintf(sql_str,"%s %s %s %s %s %s;",sql_str1,sql_str2,con[ui.andorcomboBox->currentIndex()],sql_str4,sql_str5,sql_str6);/*注意：这里的ui.andorcomboBox就是“且或”
	        选项框的，其中“且”对应的索引为0，“或”对应的索引为“1”，则con[0]=“and”，cno[1]=“or”																																 */
			//sql_str这个字符串为“select * from table[status]  where | ID=? | and | price<? | order by id | asc”
		}
		else if(s1.length() && (!s2.length())){ //如果s1中输入数据，而s2华中没有输入数据的话
			sprintf(sql_str,"%s %s %s %s %s %s;",sql_str1,sql_str2,con[ui.andorcomboBox->currentIndex()],sql_str3,sql_str5,sql_str6); /*和上面的一个if语句是一样的，
			sql_str这个字符串为“select * from table[status] where | ID=? | and | price>? | order by id | asc”*/
		}
		else { //如果s1和s2中都有输入数据的话
			sprintf(sql_str,"%s %s %s price between %d and %d %s %s;",sql_str1,sql_str2,con[ui.andorcomboBox->currentIndex()],s1.toInt(),s2.toInt(),sql_str5,sql_str6);
			//sql_str这个字符串为“select * from table[status] where | ID=? | and | price>? and price<? | order by id | asc ”
		}
	}
	 else if((!s1.length()) && s2.length()){  //第1个单行编辑框中没有输入数据，且s1中没有输入数据，s2中有输入数据
		sprintf(sql_str,"%s %s %s %s;",sql_str1,sql_str4,sql_str5,sql_str6);
		//sql_str为“select * from table[status] where | price<? | order by id | asc”
	}
	 else if(s1.length() && (!s2.length())){ //第1个单行编辑框中没有输入数据，且s1中输入数据，s2中没有输入数据
		sprintf(sql_str,"%s %s %s %s;",sql_str1,sql_str3,sql_str5,sql_str6);
		//sql_str为“select * from table[status] where | price<? | order by id | asc”
	}
	 else if(s1.length() && s2.length()){   //第1个单行编辑框中没有输入数据，且s1中没有输入数据，s2中有输入数据
		sprintf(sql_str,"%s price between %d and %d %s %s;",sql_str1,s1.toInt(),s2.toInt(),sql_str5,sql_str6);
		//sql_str为“select * from table[status] where | price>? | order by id | asc”
	}
	 else{ //第1个单行编辑框中没有输入数据，且s1中没有输入数据，s2中没有输入数据
		 sprintf(sql_str,"select * from %s %s %s;",table[table_status],sql_str5,sql_str6);
		 //sql_str为“select * from table[status] | order by id | asc”
	}


	QMessageBox::information(this,"Information",sql_str);  /*在当前窗口中弹出一个消息对话框，消息对话框的标题为“Information”，消息对话框中的内容为字符串“sql_str”，sql_str
    表示的就是执行的操作的。  */
	sql_for_tableWidget("data.db",sql_str);                //刷新ui界面中的tableWidget窗口，使得ui界面中的tableWidget窗口显示“sql_str”SQL语句查询的结果。

}

//点击“升序”按钮会将selected_radio置为0，点击“降序”按钮会将selected_radio置为1。是根据selected_radio这个标志来判断过滤查询的结果是为升序还是降序的。
void ManagementForm::on_increaseradioButton_clicked()      //点击“升序”按钮会触发这个槽函数
{
	selected_radio=0;
}
void ManagementForm::on_descendradioButton_clicked()       //点击“降序”按钮会触发这个槽函数
{
	selected_radio=1;
}



void ManagementForm::on_addpushButton_clicked()   //点击“增加记录”按钮时会触发的槽函数
{	
	ok_flag=false;
	memset(edit_id,0,10);
	memset(edit_name,0,20);
	memset(edit_producer,0,20);
	edit_price=0;

	EditRecordDlg *dlg=new EditRecordDlg;       //定义一个EditRecord类的窗口，这个类是自定义的
	dlg->exec();  /*显示这个dlg窗口，即是当点击了“增加记录”的按钮时就会弹出这个窗口，这里调用exec()函数的作用就是一直循环显示这个窗口，程序会卡在这里，不会接着往下执行，
	可以在这个窗口上进行输入的操作，当点击了“确定”或者“取消”按钮时就会关闭这个窗口，同时置ok_flag标志，关闭这个窗口之后程序就会接着往下执行了。
	
	执行到这里卡住时是点击主界面中任何一个按钮都是没有用的。
	*/

	if(ok_flag)  //根据点击的是主界面中的“确定”按钮还是“取消”按钮来判断是否要执行这个if语句里面的内容
	{
		    sqlite3 *db;
        	char *zErrMsg = 0;
        	int rc;
        	
			rc = sqlite3_open("data.db", &db);  //db指针指向了“data.db”这个数据库
        	if( rc ){
                	fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db))
						;
                	sqlite3_close(db);
                	exit(1);
        	}
		    char ss[100];memset(ss,0,100);

		sprintf(ss,"insert into %s values('%s', '%s', '%s', %d);",table[table_status],edit_id,edit_name,edit_producer,edit_price);//currently support 4 columns. make perfect later.
		//ss这个字符串为“insert into table[table_status] values(id,name,producer,price)”,注意，这里的id、name、producer、price都是为对应的的编辑框里面的内容的

		rc = sqlite3_exec(db,ss, 0, 0, &zErrMsg);  //在db指针指向的“data.db”数据库中执行这个“ss”SQL语句
		sqlite3_close(db);
		
		memset(ss,0,100); //清空ss字符串数组，置为0
		sprintf(ss,"select * from %s;",table[table_status]);  //ss这个字符串为“select * from table[table_status]”
		sql_for_tableWidget("data.db",ss);                    //刷新ui设计界面中的tableWidget这个控件里面的内容
	}
}

void ManagementForm::on_deletepushButton_clicked()      //点击“删除记录”按钮时会触发的槽函数
{
	if(QMessageBox::question(this,"Question",tr("Do you want to delete the current record?"),QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Ok)==QMessageBox::Cancel)
	{
		return;
	}  /*在当前的this窗口中弹出一个“提示框”，提示框的标题是“Question”，提示框中的内容是“Do you want to delete the current record?”，提示框中有“OK 和 cancel”两个按钮，
	如果选择的是Cancel按钮的话就会执行这个if语句里面的内容。*/


	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	rc = sqlite3_open("data.db", &db);  //db指针指向一个数据库
	if( rc ){
	fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(1);
	}

	int row=ui.tableWidget->currentRow(); //row记录ui界面中的tableWidget控件中的当前行
 
	char id_str[10];memset(id_str,0,10);
	
	QTableWidgetItem *item=ui.tableWidget->item(row,0);  //创建一个单元格对象指针item，这个单元格对象指针指向了ui设计界面中tableWidget这个控件的row行中的第0个单元格
    QString str=item->text();  //str记录item这个单元格中的内容
	sprintf(id_str,"%s",(str.toLatin1()).data());  //id_str这个字符串中的内容为str转换了编码格式之后的值

	char name_str[20];memset(name_str,0,20);
        item=ui.tableWidget->item(row,1);
        str=item->text();
        sprintf(name_str,"%s",(str.toLatin1()).data()); 
	
        char producer_str[20];memset(producer_str,0,20);
        item=ui.tableWidget->item(row,2);
        str=item->text();
        sprintf(producer_str,"%s",(str.toLatin1()).data());
	
	int price_str;
        item=ui.tableWidget->item(row,3);
        str=item->text();
	price_str=str.toInt();

	/*
    执行上述的操作之后，id_str为选中的行中的第0个单元格中的内容，name_str为选中的第1个单元格中的内容，producer_str为选中的第2个单元格中的内容，price_str为选中的第3个单元格中
	的内容。
    */
	
	char sql_str[150];memset(sql_str,0,150);
	sprintf(sql_str,"delete from %s where ID='%s' and name='%s' and producer='%s' and price=%d;",table[table_status],id_str,name_str,producer_str,price_str);
	//sql_str这个字符串中的内容为“delete from table[table_status] where ID=id_str and name=name_str and producer=producer_str and price=price_str”
	
	rc = sqlite3_exec(db,sql_str, 0, 0, &zErrMsg);   //执行“sql_str”这个字符串中的内容
	sqlite3_close(db);

	memset(sql_str,0,150); //清空“sql_str”这个字符串中的内容
        sprintf(sql_str,"select * from %s;",table[table_status]); //sql_str这个字符串中的内容为“select * from table[table_status]”
        sql_for_tableWidget("data.db",sql_str);  //刷新ui界面中的tableWidget这个控件中的内容，这个控件中的内容显示的是SQL语句“sql_str”执行的结果

	return;
}
void ManagementForm::on_modifypushButton_clicked()      //点击修改记录按钮时会触发的槽函数
{
	sqlite3 *db;
        char *zErrMsg = 0;
        int rc;
        rc = sqlite3_open("data.db", &db);  //db指向了“data.db”这个数据库
        if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
                sqlite3_close(db);
                exit(1);
        }
	int row=ui.tableWidget->currentRow();  //row记录了tableWidget控件当前所选中的行
        //int column=ui.tableWidget->columnCount();//4
        char id_str[10];memset(id_str,0,10);

        QTableWidgetItem *item=ui.tableWidget->item(row,0);
        QString str=item->text();
        sprintf(id_str,"%s",(str.toLatin1()).data());

        char name_str[20];memset(name_str,0,20);
        item=ui.tableWidget->item(row,1);
        str=item->text();
        sprintf(name_str,"%s",(str.toLatin1()).data());

        char producer_str[20];memset(producer_str,0,20);
        item=ui.tableWidget->item(row,2);
        str=item->text();
        sprintf(producer_str,"%s",(str.toLatin1()).data());

        int price_str;
        item=ui.tableWidget->item(row,3);
        str=item->text();
        price_str=str.toInt();
	
      	/*
        执行上述的操作之后，id_str为选中的行中的第0个单元格中的内容，name_str为选中的第1个单元格中的内容，producer_str为选中的第2个单元格中的内容，price_str为选中的第3个单元格中
	    的内容。
        */

	    ok_flag=false;
	    memset(edit_id,0,10);         strcat(edit_id,id_str);
        memset(edit_name,0,20);       strcat(edit_name,name_str);
        memset(edit_producer,0,20);   strcat(edit_producer,producer_str);
    	edit_price=price_str;
	
        EditRecordDlg *dlg=new EditRecordDlg;
        dlg->exec();  //程序会一直卡在这里，直到关闭了这个“修改记录”的窗口之后程序才会接着执行，即是单击了“修改记录窗口中的”确定或者取消按钮之后。
        if(!ok_flag)  //点击的是取消按钮的话
        {
                return;
        }

		//点击的是确定按钮的话
	    char sql_str[150];memset(sql_str,0,150);
        sprintf(sql_str,"update %s set ID='%s', name='%s',producer='%s',price=%d where ID='%s' and name='%s' and producer='%s' and price=%d;",table[table_status],edit_id,edit_name,edit_producer,edit_price,id_str,name_str,producer_str,price_str);
        //sql_str这个字符串中的内容为“update table[table_status] set ID=edit_id,name=edit_name,producer=editproducer,price=edit_price where ID=id_str …………”
		
		rc = sqlite3_exec(db,sql_str, 0, 0, &zErrMsg); //执行上述这个sql_str语句
        sqlite3_close(db);

        memset(sql_str,0,150); //清空字符串sql_str
        sprintf(sql_str,"select * from %s;",table[table_status]); //sql_str这个字符串里面的内容为“select * from table[table_status]”
        sql_for_tableWidget("data.db",sql_str);

	return;
}
