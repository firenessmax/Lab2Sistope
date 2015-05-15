//librerias C
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
//librerias cpp
#include <iostream>
#include <string>
#include <typeinfo>
int adyacencia[3][3]={{0,0,1},
					  {0,0,2},
					  {1,2,0}};
#import "classes.cpp"


int main(int argc, char const *argv[])
{
	LAN l1[2];
	Puente p(3);
	Equipo a(1,&l1,&p);
	Equipo b(2,&l1,&p);
	a.sendMessage("holi",&b);
	return 0;
}
