#include"common.h"
#include "parser.h"
#include"system.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/param.h>
#include<sys/linker.h>
#include<stdio.h>

#define INVALID_ARGUMENT_ERROR "Arugment not recognised"
#define MODULE_NOT_FOUND_ERROR "Module acpi_ibm.ko not loaded"

#define VERSION "0.1"

int main(int argc, char *argv[])
{
	if(kldfind("acpi_ibm")==-1)
		error(MODULE_NOT_FOUND_ERROR,NULL);		
		
	char *confpath=NULL;
	char op;
	bool daemonize = false;
	
	while( (op = getopt(argc, argv, "vdc:")) != -1)
	{
		switch(op)
		{
			case 'v':
				printf("Version: %s\n",VERSION);
				exit(0);
				break;
			case 'd':
				daemonize = true;
				break;
			case 'c':
				confpath=optarg;
				break;
			case '?':
				error(INVALID_ARGUMENT_ERROR,NULL);
				break;
			default:		
				confpath="/usr/local/etc/bsdfan.conf";				
		}
	}
	
	struct Config *conf;

	conf = readConfig(confpath);

	setFan(MANUAL,conf->levels);
	
	int oldtemp=0;

	if(daemonize)
		daemon(0,0);
		
	while(1)
	{	
		int time =2;		

		int cur_temp = getTemp();

		if(oldtemp-cur_temp>2)
			time =1;		

		oldtemp = cur_temp;

		adjustLevel(cur_temp,conf);
	
		sleep(time);	
	}

}