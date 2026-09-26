#include<stdio.h>
void fun1()
{
printf("今天是个好日子||\n");
}

void fun2()
{
printf("你好吗？？\n");
}


int main()
{
    fun1();
	fun2();
	fun1=fun2;
	fun1();

}