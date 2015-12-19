#pragma once
#ifndef compiler_glob_h
#define compiler_glob_h
/*--------------------���ļ�����---------------------------*/
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <map>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <string>
using namespace std;
/*--------------------ȫ������---------------------------*/
extern int constedge;
extern char* itoa(int i);

/*--------------------�ļ�����---------------------------*/

extern ofstream lexrst, midcoderst, symtablehis, asmrst;
extern FILE* src;											//Դ�ļ�
#define rsT asmrst

/*--------------------�ʷ�����---------------------------*/
extern void getch();
extern int getsym();

extern int kwdnum ;											//�ؼ��ֵ���Ŀ
extern int cnum;											//char�����л�����������λ�õ�ָ��
extern int ch ;												//every char
extern string id;											//sym��ʵ������
extern string prid;											//ǰһ����ʶ��������
extern int num ;											//�洢���ֵĴ�Сʱ��
extern int symid;											//��ʶ��id��
extern string sym;											//��ʶ������
extern int llen;											//�л��弼��ʱָ����ָ���λ��
extern int prllen;											//ǰ�л��弼��ʱָ����ָ���λ��
extern int line[500];										//�л��弼������
extern int backupline[500];									//�������У��л��弼������
extern int lnum;											//�е���Ŀ
extern char *word[];
extern char *wsym[];

/*--------------------���ű�-----------------------------*/
/*********
*                  symbol table
*  name    kind    value   address     paranum
*
*
**********/
#define MAXIDENLEN      30
#define MAXSYMTABLENUM  200
//	kind����
#define CONST       0   //����
#define VARIABLE    1   //����
#define FUNCTION    2   //����
#define PROCEDURE	22	//����
#define PARA        3   //����
#define CINT		4	//����int����
#define CCHAR		5	//����char����
#define AINT		6	//����int����
#define ACHAR		7	//����char����
#define VINT		8	//����int����
#define VCHAR		9	//����char����

//�����һ����ǩ������  ������ʶ������ͨ��ʶ��
typedef struct {
	char name[MAXIDENLEN];  //identifier name
	int kind;               
	int value;              //2�Ժ�����˵����char��1���ں�����˵����ΪInt��0����ֵΪvoid����Ӧprocedure��
	int address;            //��¼���λ��
	int paranum;            //��������
	bool is_return;
}symb;

typedef struct {
	symb element[MAXSYMTABLENUM];
	int index;							//�ܵ�����
	int ftotal;							//�ֳ�������
	int findextable[MAXSYMTABLENUM];	//��1��ʼ���ֳ����������飬function index table,���ڽ���������֮��������,��¼����function���ܵ�index�е���ţ�Ҳ����ʹ�á�ջ����˼��
}symtable;
extern symtable maintable;

extern int  value;						//���泣����ֵ�������ӳ���0��ʾvoid��1��ʾint
extern int  address;					//�����ĵ�ַ����������ֵ�ĵ�ַ
extern int  paranum;					//ȫ�ֱ������������������壬��Ϊ�����Ĳ�������ֻ���ڲ������ӳ���ִ�н���֮���֪��������ֻ���ں���Ų�����ű���Ϣ
extern int  kind;						//���ڴ���ı�־��������
extern char name[MAXIDENLEN];			//��־�������֣��������������ͺ���
extern char nowitem[200];
extern int returnnum;					//��¼�Ƿ��з���ֵ
extern char procname[100];

/*---------------------�﷨�������-------------------------*/
extern int mf;
void pushtable(char* name, int kind, int value, int address, int paranum, int isVec = 0);
void flushst();
int searchst(char* name, int flag, int flag2 = 0);

void wprogram();
void program();
void conststate();
void constdef();
void varstate();
void parametertable();
void complexsentence();
void sentencelist();
void ifsentence();
void condition();
void expression();
void loopsentence();
void assignsentence();
void sentence();
void valueofpara();
void readsentence();
void writesentence();
void item();
void factor();
int arrnum(char *name);

/*--------------------�м������Ԫʽ----------------------*/
extern void insmidcode(char* op, char* t1, char* t2, char* t3);
extern void insmidcode(char* op, int t1, int t2, char* t3);
extern void insmidcode(char* op, char t1, int t2, char* t3);
char* nextlab();
char* nextvar();

#define MAXMIDCODE 1000
extern int midcodeiter;
extern int labelcnt;
extern int varcnt;

typedef struct {
	char op[10];
	char var1[200];
	char var2[30];
	char var3[30];
}fourvarcode;
extern fourvarcode midcode[MAXMIDCODE];


/*----------------------������--------------------------*/
extern void error(int _errsig, int signal = 0);
extern int errnum;


/*-----------------------���----------------------------*/

typedef struct {
	char name[100];
	int level;
}constvarlevel;//�������ڲ���
extern constvarlevel cvlevel[1000];
extern int cvl;

typedef struct {
	char name[100];
	int kind;
	int paranum;
	bool isParaVar[20];
	int level;
}funcclass;
extern vector<funcclass> fc;
//������

typedef struct {
	char name[100];
	int address;
	int kind;
	int cnt;	
	//int level;
}tempvaraddress;//������
extern tempvaraddress addrtable[1000];
//��ʱ������ջ�еĵ�ַ��
typedef struct {
	int symbnum;
	int cnt;
}cntstruct;
extern cntstruct cnttable[200];
extern int ap;
extern int mi;
extern int sp;
extern int varreg[200];

int varaddr(char *name);
int findvartable(char *name);
void midcode2asm();
void mips_find_var(char* var, char* inst, char* reg, int type = 0, int addr = 0,int addrtype=0);
void insertaddress(int kind, int addr = -1, int nmi = -1);
void pushstack(char* item = "0", int lenth = 1);
void beginasm();
void inittempvar();
void funcasm();
void dataseg();
void jmpasm();
void printint();
void callasm();
void setlabasm();
void addasm();
void subasm();
void mulasm();
void divasm();
void greasm();
void geqasm();
void lssasm();
void leqasm();
void eqlasm();
void neqasm();
void assasm();
void aassasm();
void assaasm();
void rdasm();
void wtasm();
void fupaasm(int index);
void retasm();
void paraasm();
void jneasm();
void intcharasm();
void constdefasm();
void intcharaasm();
void constvarasm();
void enterfunc();

/*------------------------�Ż�-----------------------*/
#define OPTFLAG 0			//�Ż�����
extern FILE* optcodefile;
int isconst(char name[]);
void savesreg();
void loadsreg();
void cnt(char * name);
void cntopt();
void printOptimize();
void delsetlab();
void delpublic();
void combine();
void scan();
bool cmpcnt(cntstruct a, cntstruct b);

#endif

/*
ϵͳ����		syscall
ȡ������		li $s1, 100			$s1 = 100
��				add s1,s2,$s3		s3=s1+$s2
��������		addi s1,s2,100		s3=s1+100
��������		subi s1,s2,100		s3=s1-100
��				sub s1,s2,$s3		s3=s1-$s2
��				mult s2,s3			Hi��Lo=s2?s3
��				div s2,s3			Lo=s2/s3	Hi=s2 mods 3
ȡ��			lw s1,100(s2)		s1=memory[s2+100]
����			sw s1,100(s2)		memory[s2+100]=s1
beq				beq s1,s2,100		if��s1==s2�� goto PC+4+400
bne				beq s1,s2,100		if��$s1��=$s2�� goto PC+4+400
slt				slt s1,s2, $s3		if��s2<s3�� $s1=1;	else $s1=0;
j				j 2000				goto 8000
jr				ja $ra				goto $ra
jal				jal 2500			$ra=PC+4;goto 10000;
*/

/*
= , 2 , , temp			temp = 2;
[]= , a , i , t			a[i] = t;    ���鸳ֵ���

�����������	
inta, 0, num, name
chara,0, num, name

int , , , a 				int a;
//�����ԭ����int, , ,a ����Ϊint , , ,a, level(���ڲ���)
//charͬ��,constҲһ��
const,int,5 , a			const a = 5;


*,1,10, 
char, , 30, a			var a:array[30] of char;
fupa, , , a				a is a function parameter
call, f , , a			a = f()
call, f , ,				f()
<=, a , b ,				a <= b
<>,
jne , , , lable			if not satisfy(==false) then jump
jmp , , , label			jump to label
lab:, , , labx			set label
geta, a , n , b			b = a[n]
ret , , , (e)			f:=(expression);
wt , a , b , symbol		write("a", b)			symbol�����b������
rd , , , a				read(a)
func,int, , f			start of function f:integer
para,int, , a			f(a:integer; ...)
end , , , ,				end ������ǰ����
lab:, , , label$NO		�趨һ��label
*/