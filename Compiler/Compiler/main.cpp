#include "glob.h"
#include "def.h"
//��Ŀ->����->��������->C / C++->Ԥ������->Ԥ���������壬���ӣ�_CRT_SECURE_NO_DEPRECATE

FILE* optcodefile;
FILE* src;
ofstream lexrst, midcoderst, asmrst, symtablehis;

int main(int argc, char **argv) {
	char filename[500] = "in";
	printf("�����ļ����ƣ�Ĭ�϶�ȡĿ¼�µ�in.txt�ļ�,��ΪĬ��ֱ�ӻس�):\n");
	char tempsrc[500];
	gets_s(tempsrc);//C11��׼
	if (strlen(tempsrc) > 2) {
		if (tempsrc[0] == '\"') {
			strcpy_s(filename, tempsrc + 1);
			filename[strlen(filename) - 1] = 0;
		}
		else {
			strcpy_s(filename, tempsrc);
		}
	}
	fopen_s(&src, filename, "r");
	printf("�ɹ�����\n");
	lexrst.open("lexrst");
	midcoderst.open("midcode");
	symtablehis.open("symbolTable");
	asmrst.open("rst.asm");
	if (src == NULL)
	{
		error(NOSUCHFILE);
		system("pause");
		return 0;
	}

	symtablehis << "index name kind value address paranum isVec" << endl;

	wprogram();
	if (mf < 1) error(MAINLACK);
	do  getsym(); while (ch == '\n' || ch == ' ' || ch == '\t');
	if (symid != -1)  error(MOREWORD);
	if (errnum == 0) {
		printf("\n\n����ɹ���û���﷨�������!\n\n");
		//scan();          //ɨ����Ԫʽ�ṹ���飬����Ż�
		//printf("�Ż������Ԫʽ �������...\n");
		midcode2asm();   //���ɻ����
		printf("���ָ�� �������...\n");
	}
	lexrst.close();
	midcoderst.close();
	symtablehis.close();
	asmrst.close();
	fclose(src);

	printf("\n-----------------------------\n");
	if (errnum == 0)
	{
		printf("Compile Success.\n");
	}
	printf("Errors:\t%d\tTotal Line: %d\n", errnum, lnum);
	system("pause");
	return 0;
}
