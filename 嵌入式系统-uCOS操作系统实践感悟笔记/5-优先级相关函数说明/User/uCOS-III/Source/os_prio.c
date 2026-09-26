#include "os.h"


/*
*********************************************************************************************************
*                                           全局变量
*********************************************************************************************************
*/

/* 定义优先级表，在os.h中用extern声明 */
CPU_DATA   OSPrioTbl[OS_PRIO_TBL_SIZE];

/*
*********************************************************************************************************
*                                           函数实现
*********************************************************************************************************
*/


 
void OS_PrioInit( void ) //初始化优先级表
{
	CPU_DATA i;
	
 
	for( i=0u; i<OS_PRIO_TBL_SIZE; i++ )
	{
		OSPrioTbl[i] = (CPU_DATA)0; /*当有32个优先级时，宏OS_PRIO_TBL_SIZE为1，只进行了1次循环，对OSPrioTbl[0]整体赋值了0。（由于优先级是由某一二进制位来进行控制的，故
		这里的对32位的整型变量OSPrioTbl[0]赋值0就相当于整体上对32个二进制位都赋值了0,32个优先级刚开始的时候都设置为了0）*/
	}
}


void  OS_PrioInsert (OS_PRIO  prio)   //对优先级表OSPrioTbl中表示prio优先级的那一位置为1
{
    CPU_DATA  bit;        //定义了一个32位的整型（表示的范围为0~65535*65535）bit
    CPU_DATA  bit_nbr;
    OS_PRIO   ix;         


    
	ix             = prio / DEF_INT_CPU_NBR_BITS; /*获取prio这个优先级对应了优先级表OSPrioTbl中的哪个数组下标的索引，如果为prio为36的话，由于 DEF_INT_CPU_NBR_BITS为32，则
	36/32=1，表示36这个优先级所对应的数组OSPrioTbl的索引是为1的。*/
	
    
	bit_nbr        = (CPU_DATA)prio & (DEF_INT_CPU_NBR_BITS - 1u); /*首先分析DEF_INT_CPU_NBR_BITS - 1u是为2^5-1=31的（…… 0001 1111），由于36是为2^5+2^2的（…… 0010 
	0100）。当31与上36时得到的是为4的，即是36-32后的那个数。 */
	
 
    bit            = 1u;  /*bit刚开始是为1的，所进行的就是将这个1向左移动多少位（由于优先级排列的顺序是从高位到低位进行的，即当优先级表的索引为1时从高位到低位所对应的优先
	级依次是为32、33、34、35、36、……的，故当优先级为36时1需要向左移动的位数为27的）*/
    bit          <<= (DEF_INT_CPU_NBR_BITS - 1u) - bit_nbr; /* DEF_INT_CPU_NBR_BITS - 1u是为31的，减去4=27,1向左移动27位到达第36号优先级所对应的那一位。（此时是只有
	第36号优先级所对应的那一位是为1，其他都是为0的）*/
	
    OSPrioTbl[ix] |= bit;  //这里是只对优先级表中第36位（是从第0位开始的）置1，其他的都是保持不变的
}

 
void  OS_PrioRemove (OS_PRIO  prio)  //清楚优先级表OSPrioTbl中第prio位
{
    CPU_DATA  bit;
    CPU_DATA  bit_nbr;
    OS_PRIO   ix;


 
    ix             = prio / DEF_INT_CPU_NBR_BITS;
	
	  bit_nbr        = (CPU_DATA)prio & (DEF_INT_CPU_NBR_BITS - 1u);
	
    bit            = 1u;
    bit          <<= (DEF_INT_CPU_NBR_BITS - 1u) - bit_nbr;
	
	/* 上面所进行的操作与OS_PrioInsert（）函数中所进行的操作是一样的，以清楚第36号优先级所对应的那一位为例来进行分析，此时bit就是第36号优先级所对应的那一位为1，其余全是为0的。*/
    OSPrioTbl[ix] &= ~bit;  //这里所进行的就是只对第36号优先级所对应的那一位置为0，其与全都保持不变
}


OS_PRIO  OS_PrioGetHighest (void)  //获取优先级表中的最高优先级
{
    CPU_DATA  *p_tbl;
    OS_PRIO    prio;


    prio  = (OS_PRIO)0;
    
	  p_tbl = &OSPrioTbl[0];  //p_tbl指向了优先级表的首地址
	
 
    while (*p_tbl == (CPU_DATA)0)   /*这里是32位整体来进行的判断，获取优先级表中优先级最高的就是遍历这个优先级表，找到第1个为1的，如果p_tbl[0]=0的话，那么就只需要去优先级表
	  所对应的下一个索引去查找就行了。 */
		{
        prio += DEF_INT_CPU_NBR_BITS; //prio+32，prio指向了优先级表中（优先级表按照其下标可以分为第0段，第1段，第2段，……）不为0的那一段的首地址。
        p_tbl++;  //p_tbl指向了优先级表中不为0的那一段
    }
	
    prio += (OS_PRIO)CPU_CntLeadZeros(*p_tbl); //p_tbl指向了优先级表中不为0的那一段，prio加上p_tbl中前导0的个数指向的就是优先级表中第1个不为0那一位，此时的prio对应的就是游戏那几。
    return (prio);
}
