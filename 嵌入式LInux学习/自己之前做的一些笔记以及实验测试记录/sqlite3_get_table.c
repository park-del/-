//不使用回调函数来获取通过select查询到的结果
#include <stdio.h>
#include <sqlite3.h>

int main( int argc, char **argv) 
{ 
	sqlite3 * db;
	int result;            //result用来记录执行数据库相关函数是否成功的一个标志的 

	char * errmsg = NULL;  //与错误相关的，不用管
	char **dbResult;  
	
	int nRow, nColumn;     //存放查询出来的行数与列数
 	
	result = sqlite3_open( argv[1], &db );  //创建一个名称为“argv[1]”的数据库，db是指向了这个数据库的指针
	if( result != SQLITE_OK )               //执行sqlite_open()函数成功的话就会返回SQLITE_OK,这里是如果执行不成功的话就会去执行if语句里面的内容
	{ 
		return -1; 
	} 

	result = sqlite3_get_table( db, argv[2], &dbResult, &nRow, &nColumn, &errmsg ); /* 函数中的第1个参数表示了要进行操作的数据库；函数中的第2个参数表示了所执行的SQL查询语句，
	注意这里要是查询语句的（实参为一个字符串）；函数中的第3个参数存放查询到的结果，dbResult[i]表示的是查询到的元组的第i个属性的名称，dbResult[i][j]表示的是查询到的元组的第i
	个字段中的第j个元组，即dbResult[i][j]中记录的就是查询到的第j个元组的第i个字段（按照列的顺序来看，函数实参为一个二维字符数组的地址）；函数中的第4个参数表示了查询到的元组
	的个数；函数中的第5个参数表示了查询到元组的字段的个数；函数中的第6个参数表示执行这个函数是所产生的错误信息，不用管。
	
	执行成功的话函数sqlite3_get_table()函数会返回SQLITE_OK。
	*/
	

	//修改：
	if( SQLITE_OK == result )  //执行查询操作成功的话，也即是执行sqlite3_get_table()函数成功的话，就会执行下面的if语句里面的内容将查询到的结果给打印出来。
	{
		int index = 1;
		int t=0;
        int i,j;

		printf( "find %d records\n", nRow );      //输出查询到几条记录，也即是查询到了多少个元组

		for(i = 0; i < nRow ; i++ )               //nRow行
		{
			printf( "\nNO. %d record\n", i+1 );   //依次打印查询到的每一个元组的信息
			
			for(j = 0 ; j < nColumn; j++ )        //nColumn列
			{
				printf("%s:%s  ", dbResult[j], dbResult[index*nColumn+t]);
				t++;
			} 

			t=0;      //输出完一个元组之后另t为0
			index++;  //输出完一个元组之后另index++
			printf("-------\n"); 
		}
	}
}
/*
查询到的结果为nColumu列的话：
（1）则dbResult[0]~dbResult[nColumu-1]依次就是为查询到的元组的字段的名称的。
（2）则dbResult[nColumn]~dbResult[2*nColumn-1]依次就是为查询到的第1个元组中的各字段的。
（3）则dbResult[2*nColumn]~dbResult[3*nColumu-1]依次就是为查询到的第2个元组中的各字段的。
（4）则dbResult[3*nColumn]~dbResult[4*nColumn-1]依次就是为查询到的第3个元组中的各字段的

第2个元组中的第3个字段为：dbResult[2*nColumn+（3-1）]
第2个元组中的第4个字段为：dbResult[2*nColumn+（4-1）]
第2个元组中的第i个字段为：dbResult[2*nColumn+（i-1）]
*/


//一般使用sqlite3_open()函数来执行除了查询之外的其他SQL语句，使用sqlite_3_table()函数来执行SQL的查询语句，即select语句。
