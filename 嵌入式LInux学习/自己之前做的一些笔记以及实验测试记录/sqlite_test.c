#include <stdio.h>
#include <sqlite3.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColName){ /* 回调函数中的第1个参数是用来接收sqlite3_open()函数所传递过来的参数的；函数中的第2个参数是用来
表示所查询到的元组的属性的数量的（注意不是从0开始的）；函数中的第3个参数是为一个字符串数组的，按照“z字型”的方式依次存放所查询到的各个字段（即通过select查询语句所查询到的
是为一张表的，将这张表按照z字型的顺序连接起来，按照顺序每一个argv[i]就表示所查询到的的字段，所查询到的结果是连续的，这个字段属于哪一个元组是不确定的）；函数中的第4个参数是
用来指明所查询到的元组属性的名称的，其与函数中的第2个参数是是对应着的。 */
 
  int i;
  for(i=0; i<argc; i++){  //argc代表查询到的元组的数量，这里进行的一遍argc次循环输出的就是查询结果中的第1个元组的。
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");  //输出完查询结果中的第1个元组之后就输出“\n”，通过“\n”来区分出不同的元组。
  return 0;
}

/*
下面的“sqlite3”，“sqlite3_open（）”，“sqlite3_exec（）”，“sqlite3_free（）”，“sqlite3_close（）”都是定义在sqlite3.h这个结构体里面的，使用这些时必须包含
sqlite3.h这个头文件。
*/

int main(int argc, char **argv){
  sqlite3 *db;      //使用sqlite3这个结构体定义一个数据库指针db
  
  char *zErrMsg = 0;
  int rc;
  
  if( argc!=3 ){   /* 命令行中输入的参数是要为3个的，即类似于“./sqlite_test  xyz.db  "insert into tbl0 values('cyc', 1);" ”的形式，当输入的形式是样时则argc为3，argv字符串
	数组是为argv[3]，其中argv[0]为“./sqlite_test”，argv[1]是为“xyz.db”，argv[2]是为“"insert into tbl0 values('cyc', 1);"”。 */
    fprintf(stderr, "Usage: %s DATABASE SQL-STATEMENT\n", argv[0]);
    exit(1);
  }
  
  rc = sqlite3_open(argv[1], &db);  /*使用sqlite3_open()函数创建了一个数据库，其中函数的第1个参数是为所创建的这个数据库的名字的，这里为“xyz.db”;函数中的第2个参数是为指向
  所创建的这个数据库的一个指针的，就是通过这个数据库的指针来完成这个数据库的操作的。*/
  if( rc ){
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);  //关闭db所指向的数据库
    exit(1);
  }
  rc = sqlite3_exec(db, argv[2], callback, 0, &zErrMsg); /* 使用sqlite_exec()函数是用来对某一个数据库执行相应的SQL语句的，函数中的第1个参数指向了某一个数据库；函数中的第2
  个参数是为要执行的SQL语句的；函数中的第3个参数是为一个回调函数的函数名的，就是当执行“select查询操作”后是会自动的调用这个回调函数来对查询的结果进行相应的处理的；函数中
  的第4个参数是为传递给这个回调函数的一个参数的（回调函数中的第1个参数会接收传递过来的这个参数）；函数中的第5个参数表示执行这个函数时的错误信息，不用管。 */

  if( rc!=SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }
  sqlite3_close(db);
  return 0;
}

 
