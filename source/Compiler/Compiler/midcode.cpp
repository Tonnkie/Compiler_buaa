#include "glob.h"
#include "def.h"

fourvarcode midcode[MAXMIDCODE];
int midcodeiter = 0;								//count
int labelcnt = 0;									//label count
int varcnt = 0;										//variant count
vector <char*> hisvar;								//history variant

//�����м����
void insmidcode(char* op, char* t1, char* t2, char* t3) {
	if (strcmp(op, "func") == 0) {
		midcoderst << endl << endl;
	}
	midcoderst << "\t\t";
	midcoderst << op << "\t," << t1 << "\t," << t2 << "\t," << t3 << endl;
	strcpy(midcode[midcodeiter].op, op);
	strcpy(midcode[midcodeiter].var1, t1);
	strcpy(midcode[midcodeiter].var2, t2);
	strcpy(midcode[midcodeiter].var3, t3);
	midcodeiter++;
}
void insmidcode(char* op, int t1, int t2, char* t3) {
	midcoderst << "\t\t";
	midcoderst << op << "\t," << t1 << "\t," << t2 << "\t," << t3 << endl;
	strcpy(midcode[midcodeiter].op, op);
	char t[MAXIDENLEN];
	sprintf(t, "%d", t1);
	strcpy(midcode[midcodeiter].var1, strcmp(t, "0") == 0 ? "  " : t);
	sprintf(t, "%d", t2);
	strcpy(midcode[midcodeiter].var2, t );
	strcpy(midcode[midcodeiter].var3, t3);
	midcodeiter++;
}
void insmidcode(char* op, char* t1, int t2, char* t3) {
	midcoderst << "\t\t";
	midcoderst << op << "\t," << t1 << "\t," << t2 << "\t," << t3 << endl;
	strcpy(midcode[midcodeiter].op, op);
	char t[MAXIDENLEN];
	strcpy(midcode[midcodeiter].var1, t1);
	sprintf(t, "%d", t2);
	strcpy(midcode[midcodeiter].var2, t);
	strcpy(midcode[midcodeiter].var3, t3);
	midcodeiter++;
}
//�����µı��
char* nextlab() {
	char *label = (char*)malloc(sizeof(char) * 20);
	strcpy(label, "_LABEL_");
	sprintf(label, "_LABEL_%d", labelcnt++);
	return label;
}

//�����µ���ʱ����
char* nextvar()
{
	char *var = (char*)malloc(sizeof(char) * 20);
	sprintf(var, "$_%d", varcnt++);
	hisvar.push_back(var);
	return var;
}

void freetempvar() {
	for (int i = 0; i <(int) hisvar.size(); i++) {
		free(hisvar[i]);
	}
}

char* itoa(int i) {
	char *temp = (char*)malloc(sizeof(char) * 10);
	sprintf(temp, "%d", i);
	return temp;
}