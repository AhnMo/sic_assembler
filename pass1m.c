#include <stdio.h>
#include <stdlib.h>
#include <iostream>

int MAX_LEN = 30;

char SYMTAB[][8]={NULL};

char DRTV[][6]={"BYTE", "WORD", "RESB", "RESW"};

/*
char OPTAB[59][8]={"ADD", "ADDF", "ADDR", "AND", "CLEAR", "COMP", "COMPF", "COMPR", "DIV", "DIVF",
				   "DIVR", "FIX", "FLOAT", "HIO", "J", "JEQ", "JGT", "JLT", "JSUB", "LDA", "LDB",
				   "LDCH", "LDF", "LDL", "LDS", "LDT", "LDX", "LPS", "MUL", "MULF", "MULR", "NORM",
				   "OR", "RD", "RMO", "RSUB", "SHIFTL", "SHIFTR", "SIO", "SSK", "STA", "STB", "STCH",
				   "STF", "STI", "STL", "STS", "STSW", "STT", "STX", "SUB", "SUBF", "SUBR", "SVC",
				   "TD", "TIO", "TIX", "TIXR", "WD"};

int OPCODE[59]={0x18, 0x58, 0x90, 0x40, 0xB4, 0x28, 0x88, 0xA0, 0x24, 0x64,
			    0x9C, 0xC4, 0xC0, 0xF4, 0x3C, 0x30, 0x34, 0x38, 0x48, 0x00, 0x68,
			    0x50, 0x70, 0x08, 0x6C, 0x74, 0x04, 0xD0, 0x20, 0x60, 0x98, 0xC8,
			    0x44, 0xD8, 0xAC, 0x4C, 0xA4, 0xA8, 0xF0, 0xEC, 0x0C, 0x78, 0x54,
			    0x80, 0xD4, 0x14, 0x7C, 0xE8, 0x84, 0x10, 0x1C, 0x5C, 0x94, 0xB0,
			    0xE0, 0xF8, 0x2C, 0xB8, 0xDC};
*/

int path1()
{
	FILE *inFile, *interFileR, *interFileW;
	char buffer[50];									  //input파일에서 한 라인을 받을 버퍼
	char label[10]={}, opcode[10]={}, operand[10]={};  //하나의 라인을 3개로 나눠서 저장
	char *token;
	int LOCCTR;         // 로케이터
	int startAdd, Plength;  // 시작주소, 프로그램 길이
	int i;
	int symcnt=0;
	int errorFlag=0;

	inFile=fopen("input.txt","r");  
	interFileR=fopen("inter.txt", "r");
	interFileW=fopen("inter.txt", "a");//파일 열어줌
	
	fgets(buffer, MAX_LEN,inFile); //한 라인 읽어옴
	token=strtok(buffer," /t");
	strcpy(label,token);
	token=strtok(buffer," /t");
	strcpy(opcode,token);
	token=strtok(buffer," /t");
	strcpy(operand,token);
	
	if(strcmp("START", opcode)==0) 
	{
		startAdd = atoi(operand);
		LOCCTR = startAdd;
		fprintf(interFileW, "START %d\n", LOCCTR);
	}
	else 
	{
		LOCCTR = 0;
		startAdd = 0;
	}//첫라인읽어와서 START인지 판단. 이후 필요한 값들 초기화

	while(strcmp("END", label)!=0) 
	{
		if(label[0] == '.')
			goto NEXT;
		else
			if(strcmp(NULL, label)!=0) 
			{
				if(strcmp(DRTV[0], label)
				if(strcmp(DRTV[1], label)
				if(strcmp(DRTV[2], label)
				if(strcmp(DRTV[3], label)

			}


	





NEXT:	fprintf(interFileW, "");
		for(i=0 ; i<10 ; i++) {
			label[i]=NULL;
			opcode[i]=NULL;
			operand[i]=NULL;
		}
		fgets(buffer, MAX_LEN,inFile); //한 라인 읽어옴
		token=strtok(buffer," /t");
		strcpy(label,token);
		token=strtok(buffer," /t");
		strcpy(opcode,token);
		token=strtok(buffer," /t");
		strcpy(operand,token);
	}
	fprintf(interFileW, "");
	Plength = LOCCTR - startAdd;

	return errorFlag;
}

	