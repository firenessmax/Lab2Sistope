//librerias C
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
//librerias cpp
#include <iostream>
#include <string>



int adyacencia[3][3]={{0,0,1},
					  {0,0,2},
					  {1,2,0}};
//#import "classes.cpp"
typedef struct n{
	int a;
	inline struct n operator >>(struct n* b){
		b->a=a;
		//irrelevante
		struct n c;
		return c;
	}
}nuevo;

int main(int argc, char const *argv[])
{
	nuevo A,B;
	A.a=1;
	B.a=2;
	A>>&B;
	std::cout<<B.a<<std::endl;
	return 0;
}
