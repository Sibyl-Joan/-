#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct//定义顾客结构体，包括编号和对应的某个部门信息 
{
	int c_custkey;    	  
	char c_mkgsegment[20]; 
}customer;				    

typedef struct//定义订单结构体，包括订单号、下单顾客编号、下单时间 
{
	int o_orderkey;    	  
	int o_custkey;    	 
	char o_orderdate[10]; 
}orders;				 

typedef struct//定义商品信息结构体，包括订单号、额外价格、发货日期 
{
	int l_orderkey;
	double l_extendedprice;
	char l_shipdate[10]; 
}lineitem;  

typedef struct//定义查询结果结构体，包括订单号、订货日期、额外价格 
{
	int l_orderkey;
	char o_orderdate[10]; 
	double l_extendedprice;
}select_result;

//定义结构体，相当于实现数据库建表功能 
 
customer * list_customer_txt() //读取customer.txt内容 
{
	FILE * fp;
	customer *a=NULL;
	a = (customer *)malloc(100*sizeof(customer));//行数100
	int i=0;
	char b;
	fp = fopen("home\\xiang\\桌面\\操作系统\\customer.txt","r");//引用文件所在位置 
	if(NULL==fp)
	{
		printf("Cannot open customer.txt!\n");
		return NULL;
	}
	while(!feof(fp))
	{	
		fscanf(fp,"%d%c%s",&a[i].c_custkey,&b,a[i].c_mkgsegment);
		i++;
	}
	fclose(fp);
	return a;
}

lineitem * list_lineitem_txt()//读取lineitem.txt内容
{
	FILE * fp;
	lineitem * l=NULL;
	l = (lineitem *)malloc(1000*sizeof(lineitem));//行数1000
	int i=0;
	char b,c;
	fp = fopen("home\\xiang\\桌面\\操作系统\\lineitem.txt","r");
	if(fp==NULL)
	{
		printf("Cannot open lineitem.txt!\n");
		return NULL;
	}
	while(!feof(fp))
	{
		fscanf(fp,"%d%c%lf%c%s",&l[i].l_orderkey,&c,&l[i].l_extendedprice,&b,l[i].l_shipdate);
		i++;
	}
	fclose(fp);
	return l; 
}

orders * list_orders_txt()//读取orders.txt内容 
{
	int i =0; 
	orders * a=NULL;
	a = (orders * )malloc(4000*sizeof(orders));//行数4000
	char b,c;
	long long d;
	FILE *fp;
	fp = fopen("home\\xiang\\桌面\\操作系统\\orders.txt","r");
	if(fp == NULL)
	{
		printf("Cannot open orders.txt!\n");
		return NULL;
	}
	while(!feof(fp))
	{	
		fscanf(fp,"%d%c%lld%c%s",&a[i].o_orderkey,&b,&d,&c,a[i].o_orderdate);
		a[i].o_custkey=d%100;
		i++;
	}
	fclose(fp);
	return a;
}


//编写用于读取三个表的数据记录的函数，相当于实现数据库录入数据的功能。调取文件本来可以写成单独一个函数，通用，但是实际用户操作是比较麻烦，由于行数等属性差异，使得需要键入的数据过多。

select_result * select(customer * cus,orders * ord,lineitem * item,int limit,char * order_date,char * ship_date)
{
	int i,j,k,l=0,m=0;
	select_result * result1=NULL;
	select_result * result2=NULL;
	select_result * result=NULL;
	result1 = (select_result *)malloc(1000*sizeof(select_result));
	result2 = (select_result *)malloc(1000*sizeof(select_result));
	result = (select_result *)malloc(limit*sizeof(select_result));
	for(i=0;i<100;i++)
	{
		for(j=0;j<4000;j++)
		{
			for(k=0;k<1000;k++)
			if(cus[i].c_custkey==ord[j].o_custkey&&ord[j].o_orderkey==item[k].l_orderkey&&(strcmp(ord[j].o_orderdate,order_date)<0)&&(strcmp(item[k].l_shipdate,ship_date)>0))//比较字符串
			{
				printf("%d,%s,%lf\n",item[k].l_orderkey,ord[j].o_orderdate,item[k].l_extendedprice);
				result1[l].l_orderkey=item[k].l_orderkey;
				strcpy(result1[l].o_orderdate,ord[j].o_orderdate);//实现连接
				result1[l].l_extendedprice=item[k].l_extendedprice;
				l++;
			}
		}
	}
	printf("求和\n");
	for(i=0;i<l;i++)
	{
		if(i==0)
		{
			result2[m].l_orderkey = result1[i].l_orderkey;
			strcpy(result2[m].o_orderdate,result1[i].o_orderdate);
			result2[m].l_extendedprice = result1[i].l_extendedprice;
			
		}
		if(i!=0&&result1[i].l_orderkey==result1[i-1].l_orderkey)
		{
			result2[m].l_extendedprice = result2[m].l_extendedprice + result1[i].l_extendedprice;
			
		}
		if(i!=0&&result1[i].l_orderkey!=result1[i-1].l_orderkey)
		{
			m++;
			result2[m].l_orderkey = result1[i].l_orderkey;
			strcpy(result2[m].o_orderdate,result1[i].o_orderdate);
			result2[m].l_extendedprice = result1[i].l_extendedprice;
		}
	}
	for(i=0;i<=m;i++)
	{
		printf("%d,%s,%lf\n",result2[i].l_orderkey,result2[i].o_orderdate,result2[i].l_extendedprice);
	}
}

//属性连接

int main()
{
	customer * cus = NULL;
	orders * ord = NULL;
	lineitem * item = NULL;
	//置空 
	cus = list_customer_txt();
	ord = list_orders_txt();
	item = list_lineitem_txt();
	//赋值 
	int limit=1;
	char order_date[]="1995-03-28";
	char ship_date[]="1995-04-27";
	select(cus,ord,item,limit,order_date,ship_date);
	return 0;
}
