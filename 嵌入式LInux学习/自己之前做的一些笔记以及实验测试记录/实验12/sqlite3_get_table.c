#include <stdio.h>
#include <sqlite3.h>

int main( int argc, char **argv) 
{ 
	sqlite3 * db;
	int result; 
	char * errmsg = NULL; 
	char **dbResult;  
	int nRow, nColumn; 
	int i , j; 
	int index; 
	result = sqlite3_open( argv[1], &db ); 
	if( result != SQLITE_OK ) 
	{ 
		return -1; 
	} 
	result = sqlite3_get_table( db, argv[2], &dbResult, &nRow, &nColumn, &errmsg );
	if( SQLITE_OK == result ) 
	{
		index = nColumn;
		printf( "find %d records\n", nRow ); 
		for( i = 0; i < nRow ; i++ ) 
		{
			printf( "NO. %d record\n", i+1 ); 
			for( j = 0 ; j < nColumn; j++ ) 
			{
				printf("field name:%s  value:%s\n", dbResult[j], dbResult[index]); 
				++index; 
			} 
			printf("-------\n"); 
		}
	}
}
