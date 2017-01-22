#include<stdio.h>


#define MAX 1000

int b,c;
int lines[10];
int *a;
char f;
double hgj;
float no1;



void function1(int x,int y)
{
    printf("hellow waorld");

}

void function2(){
	int i;
    function1(4,10);
    b = 0;
	for(i=0; i<5; i++)
	{
		printf("function2");
		function3();
	}

}

void function3() 
{

	lines[5] = 45;
    printf("%d",b);
	function2();
}

int main()
{
	printf("Hello World");

	for(int i=0;i<10;i++){
		printf("my name is bsse0513");
		if(MAX<i){
			printf("Impossible");
		}
	}
	function3();
	a=b;
	b=c;
	function2();

	return 0;
}
