#include "glob.h"
#include "def.h"
//�˴�������Ϊ�˷��� ����\��ֵ
#define INT		1
#define CHAR	2

int		mf = 0;				//������������
int		value;				//���泣����ֵ��2�Ժ�����˵����char��1���ں�����˵����ΪInt��0����ֵΪvoid����Ӧprocedure��
int		address;			//�����ĵ�ַ����������ֵ�ĵ�ַ
int		paranum;			//ȫ�ֱ������������������壬��Ϊ�����Ĳ�������ֻ���ڲ������ӳ���ִ�н���֮���֪��������ֻ���ں���Ų�����ű���Ϣ
int		kind;				//���ڴ���ı�־��������
int		returnnum = 0;		//��¼�Ƿ��з���ֵ
int		hiscnt = 1;			//���ű���������ʷ��¼
int		factortype = 0;		//���ӵ�����
int		synlevel = 0;			//��ǰָ�����ں���������0��ʾ�������

char	name[MAXIDENLEN]=" ";	//��־�������֣��������������ͺ���
char	nowitem[200];		//������ʽ��ʱ�������浱ǰ��
char	procname[100];		//������������

symtable maintable;

//------------------------------------------------------------------------------------------
void init() {
	for (int i = 0; i < MAXSYMTABLENUM; i++) {
		maintable.element[i].is_return = false;
	}
}

void pushtable(char* name, int kind, int value, int address, int paranum, int isVec) {
	//���Ƿ���
	if (maintable.index >= MAXSYMTABLENUM) {
		error(OUTOFTABLE);
		exit(0);
	}
	//��������
	if (kind == FUNCTION|| kind == PROCEDURE) {
		int iter = 1;
		//�����ǲ鿴�Ƿ�����
		for (iter = 1; iter <= maintable.ftotal; iter++) {
			if (!strcmp(maintable.element[maintable.findextable[iter]].name, name)) {
				error(SYMBOLCONFLICT);
				return;
			}
		}
		maintable.findextable[++maintable.ftotal] = maintable.index;
	}
	else {
		//�����ǲ鿴�Ƿ������������ڲ��ı���������������
		for (int iter = maintable.findextable[maintable.ftotal]; iter < maintable.index; iter++) {
			if (!strcmp(maintable.element[iter].name, name)) {
				error(SYMBOLCONFLICT);
				return;
			}
		}
	}
	strcpy(maintable.element[maintable.index].name, name);
	maintable.element[maintable.index].kind = kind;
	maintable.element[maintable.index].value = value;
	maintable.element[maintable.index].address = maintable.index;
	maintable.element[maintable.index].paranum = isVec ? isVec : paranum;
	if (maintable.index <= hiscnt) {
		symtablehis << "------------------------" << endl;
	}
	hiscnt = maintable.index;
	symtablehis << maintable.index << "\t" << name << "\t" << kind << "\t" << value << "\t" << address << "\t" << paranum << "\t" << isVec << endl;
	maintable.index++;
}

void flushst() {
	int x = maintable.index - 1;
	int y;
	while ((
		maintable.element[x].kind == CINT	|| 
		maintable.element[x].kind == CCHAR	|| 
		maintable.element[x].kind == AINT	||
		maintable.element[x].kind == ACHAR	||
		maintable.element[x].kind == VINT	||
		maintable.element[x].kind == VCHAR	||
		maintable.element[x].kind == PARA) 
		&& 
		strcmp(maintable.element[x].name, procname) 
		!= 0) {
		maintable.element[x].kind = 0;
		maintable.element[x].address = 0;
		maintable.element[x].paranum = 0;
		maintable.element[x].value = 0;
		y = 0;
		while (y < 30)
			maintable.element[x].name[y++] = '\0';
		x--;
	}
	maintable.findextable[maintable.ftotal] = 0;
	maintable.ftotal--;
	maintable.index = x + 1;
	return;
}

int searchst(char* name, int flag, int flag2 ) { 
	int n;
	if (flag == 1) {
		n = maintable.index;
		while (n >= 0) {
			if (strcmp(maintable.element[n].name, name) == 0 && (
				maintable.element[n].kind == FUNCTION || maintable.element[n].kind == PROCEDURE))
				break;
			n--;
		}
		if (n < 0 ) {
			return -1 * FUNCTIONNOTFOUND;
		}
		return maintable.element[n].address;
	}
	else {
		n = maintable.index - 1;
		while (n >= 0 ) {
			if (strcmp(maintable.element[n].name, name) == 0)
				break;
			n--;
		}
		if (n < 0) {
			return -1;
		}

		//������������ǲ����ģ��򽫲�������Ե�ַ���أ��˴���Ե�ַΪ�ڷ��ű��е�ջ��λ��
		if (maintable.element[n].kind == CINT
			|| maintable.element[n].kind == CCHAR
			|| maintable.element[n].kind == AINT
			|| maintable.element[n].kind == ACHAR
			|| maintable.element[n].kind == VINT
			|| maintable.element[n].kind == VCHAR
			|| maintable.element[n].kind == PARA)
			return maintable.element[n].address;

		return -1;
	}
}

//�������鶨��ĳ���
int arrnum(char *name) {
	int n = maintable.index-1;
	//���ҷ��ű����Ƿ���
	while (n >= 0) {
		if (strcmp(maintable.element[n].name, name) == 0)
			return maintable.element[n].paranum;
		n--;
	}
	return 0;
}

//------------------------------------------------------------------------------------------------------------
//checked
//1.<����> :: =   <�ֳ���>.
void wprogram() {
	init();
	getsym();
	program();
	if (symid != PERIOD) {
		error(PERIODLACK, 0);
	}
}
vector<char*>	tmpfuncname;

//2.<�ֳ���> ::=   [<����˵������>][<����˵������>]{[<����˵������>]| [<����˵������>]}<�������>
void program() {
	if (symid == CONSTTK) {
		conststate();
	}
	if (symid == VARTK) {
		varstate();
	}
	
	//���뺯��/���̲���
	while (symid == FUNCTK || symid == PROCETK) {
		synlevel++;
		//15.<����˵������>  :: = <�����ײ�><�ֳ���>{ ; <�����ײ�><�ֳ���> };
		//17.<�����ײ�>   ::=   function <��ʶ��>[<��ʽ������>]: <��������>; 
		//18.<��ʽ������> :: = '('<��ʽ������>{; <��ʽ������>}')'
		//19.<��ʽ������>:: = [var]<��ʶ��>{, <��ʶ��>}: <��������>
		// function f(var a,b,c:integer):integer
		// function f:integer
		// function f(a:integer):integer
		// function f(a,b:integer):char
		// ����fun1������������һ������func2����ô�����func1��end���С�����
		if (symid == FUNCTK) {
			getsym();
			strcpy(name, id.c_str());
			varcnt = 0;     //��ʱ�������¿�ʼ����
			returnnum = 0;//���Ƿ���ڷ���ֵ�ı�ǳ�ʼ����0��ʾ�޷���ֵ����δ����return
			address = 0;
			paranum = 0;
			strcpy(procname, name);//����������
			char tmp_func[MAXIDENLEN] = " ";
			strcpy(tmp_func, name);
			tmpfuncname.push_back(tmp_func);
			getsym();
			/*------Ԥ������----*/
			int tempid = symid;//�ָ��ֳ���
			char tempch = ch;//�ָ��ֳ���
			int tempcnum = cnum;//�ָ��ֳ���
			if (symid == LPARENT) {
				getsym();
				while (symid != RPARENT) {
					getsym();
				}
				getsym();
			}
			if (symid == COLON) {
				getsym();
			}
			/*
			�˴���Ҫ��ǰ��
			*/
			kind = FUNCTION;//��ʾ��ǰ�ĺ������ӳ��򣬼���������
			//�������˴���paranum��ȻΪ0�������ں����������庯��ִ���������¸�paranum��ֵ
			if (symid == INTTK) {
				value = 1;//2���ں�����˵û�з���ֵ��Ҳ����procedure��1���ں�����˵����ʾ����ֵΪInt 0����ֵΪvoid
				pushtable(procname, kind, value, address, paranum);
				insmidcode("func", "int", "  ", procname);
			}
			if (symid == CHARTK) {
				value = 2;//2���ں�����˵û�з���ֵ��Ҳ����procedure��1���ں�����˵����ʾ����ֵΪInt 0����ֵΪvoid
				pushtable(procname, kind, value, address, paranum);
				insmidcode("func", "char", "  ", procname);
			}

			//�ָ�Ԥ��ǰ�ֳ�
			symid = tempid;
			cnum = tempcnum;
			ch = tempch;
			//--------Ԥ������

			if (symid == LPARENT) {
				getsym();
				parametertable();
				//ȱ'('��������')'����С����
				if (symid != RPARENT) {
					error(RIGHTPARENTLACK);
					continue;
				}
				getsym();
			}
			if (symid != COLON) {
				error(COLONLACK);
			}
			else {
				getsym();
			}
			if (symid != CHARTK && symid != INTTK) {
				error(RETURNTYPELACK);
			}
			getsym();
			if (symid != SEMICN) {
				error(SEMICOLONLACK, -1);
			}
			else {
				getsym();
			}
			program();
			getsym();
		}
		//14.<����˵������>  ::=  <�����ײ�><�ֳ���>{; <�����ײ�><�ֳ���>};
		//16.<�����ײ�>  ::=   procedure<��ʶ��>[<��ʽ������>];   //�޷���ֵ�Ŀ��Կ��ɺ���
		//18.<��ʽ������> :: = '('<��ʽ������>{; <��ʽ������>}')'
		//19.<��ʽ������>:: = [var]<��ʶ��>{, <��ʶ��>}: <��������>
		// procedure p(var a,b,c:integer)
		// procedure p(a,b,c:integer)
		// procedure p(a:char)
		// procedure p;
		else if (symid == PROCETK) {
			getsym();
			if (symid != IDENT) {
				error(IDENTIFIERLACK);
				continue;
			}
			varcnt = 0;          //��ʱ�������¿�ʼ����
			strcpy(name, id.c_str());
			kind = PROCEDURE;
			value = 0;
			address = 0;
			paranum = 0;
			pushtable(name, kind, value, address, paranum);
			strcpy(procname, name);
			char tmp_func[MAXIDENLEN] = " ";
			strcpy(tmp_func, name);
			tmpfuncname.push_back(tmp_func);
			insmidcode("func", "void", "  ", procname);
			getsym();
			if (symid == LPARENT) {
				getsym();
				parametertable();
				//ȱ'('��������')'����С����
				if (symid != RPARENT) {
					error(RIGHTPARENTLACK);
					continue;
				}
				getsym();
			}
			if (symid != SEMICN) {
				error(SEMICOLONLACK,-1);
			}
			else {
				getsym();
			}
			program();
			if (symid != SEMICN) {
				error(SEMICOLONLACK);
			}
			else {
				getsym();
			}
		}
	}
	while (symid == BEGINTK) {
		//38.<�������>  ::=   begin<���>{; <���>}end
		//�˴����ܼ򵥵Ľ��俴���Ǹ�����䣬ʹ��complexsentence()���ݹ飬�˴���һ��ĸ�����仹�ǲ�ͬ��
		//�����������û�г����ͱ�����������
		varcnt = 0;        //��ʱ�������¿�ʼ����
		//������ʵ����Ҫ����һ��ָ���ʾ�����Ŀ�ʼ��������
		char* tmp = NULL;
		//strcpy(name, "funcmain");
		if (synlevel == 0) {
			strcpy(name, "main");
			kind = FUNCTION;
			value = 0;
			address = 0;
			paranum = 0;
			mf++; //�������ĸ���+1-----------------------
			pushtable(name, kind, value, address, paranum);
			strcpy(procname, name);
			insmidcode("func", "  ", "  ", procname);
			insmidcode("begin", "  ", "  ", "  ");
		}
		else {
			tmp = tmpfuncname.back();
			insmidcode("begin", "  ", "  ",tmp);//�����Ϊ����ת����ǵ�
			tmpfuncname.pop_back();
		}
		getsym();
		sentencelist();
		if (symid != ENDTK) {
			error(ENDLACK);
		}
		if(synlevel!=0){
			int s = searchst(tmp,1);
			if (s > 0 && maintable.element[s].kind == FUNCTION) {
				if (!maintable.element[s].is_return) {
					error(NONERETURN);
				}
			}
		}
		flushst();//������ű�������յ��Ǵ��Ӻ����ķ��ű�,������Ӻ�����
		synlevel--;
		insmidcode("end", "  ", "  "," ");
		strcpy(name, " ");
		getsym();
		if (symid != PERIOD) {
			if (symid != SEMICN) {
				error(SEMICOLONLACK);
			}
		}
	}
	return;    //Դ�������������
}

//3.<����˵������> ::=   const<��������>{,<��������>};
//4.<��������>::= <��ʶ��>�� <����>
//5.<����>:: = [+| -] <�޷�������> | <�ַ�>
//6.<�ַ�>::=  '<��ĸ>' | '<����>'		//�����ڶ���ʱֻ����һ����ĸ�������֣��ɡ�������������
//ATTENTION:�������ֱ�Ӷ������ͣ������Ķ���û�и������ͣ������Ҫ���ݵ�ʽ����Ľ���������ж�
//�����������Ӳ���ͬʱ����
// const a=123,b=+32,c=-123,d='a',e='8';
// const a=1;
// const a='8';
//�ú�������constdef������ͬʱ��program����
void conststate() {
	getsym();//�õ�const �����IDENT
	address = 0;
//	paranum = 0;
	do {
		constdef();
	} while (symid == COMMA);
	if (symid != SEMICN) {
		error(SEMICOLONLACK);
		return;
	}
	getsym();
	return;
}

//4.<��������>::= <��ʶ��>�� <����>
//5.<����>:: = [+| -] <�޷�������> | <�ַ�>
//6.<�ַ�>::=  '<��ĸ>' | '<����>'		//�����ڶ���ʱֻ����һ����ĸ�������֣��ɡ�������������
//ATTENTION:���ﲻ����int��char, int, �����߽���tclass����
//�ú�����conststate��������
//�����������Ӳ���ͬʱ����
// const a=123,b=+32,c=-123,d='a',e='8';
// const a=1;
// const a='8';
//�˴��������    a=123      b=+32     c=-123    d='a'
void constdef() {
	char temp[200];
	if (symid == COMMA)
		getsym();
	if (symid == IDENT) {
		strcpy(name, id.c_str());
		getsym();
		if (symid == EQL) {
			getsym();
			if (symid == PLUS || symid == MINU) {
				kind = CINT;
				int symb = symid;
				getsym();
				if (symid == INTCON) {
					if (symb == PLUS) {
						value = num;
					}
					else {
						value = 0 - num;
					}
					pushtable(name, kind, value, address, paranum);
					sprintf(temp, "%d", value);
					insmidcode("const", "int", temp, name);
				}
			}
			else if (symid == INTCON) {
				kind = CINT;
				value = num;
				pushtable(name, kind, value, address, paranum);
				sprintf(temp, "%d", value);
				insmidcode("const", "int", temp, name);
			}
			else if (symid == CHARCON) {
				kind = CCHAR;
				value = id.c_str()[0];
				pushtable(name, kind, value, address, paranum);
				sprintf(temp, "%d", value);
				insmidcode("const", "char", temp, name);
			}
			else {
				error(UNKNOWRIGHTSYM);//unknow right sym
				return;
			}
		}
		else {
			error(CONSTNOTINIT);
			return;
		}
	}
	else {
		error(CSTDEFINEFAIL);
	}
	getsym();
	return;
}


//10.<����˵������>  ::=   var <����˵��> ; {<����˵��>;}
//11.<����˵��> :: = <��ʶ��>{ , <��ʶ��> } : <����>
//12.<����>   :: = <��������> | array'['<�޷�������>']' of <��������>
/* ���ӣ�
var
	a,a1,a2:integer;
	b:array[100] of integer;
	c:char;
	d:integer;   //<-���һ����������";"
*/
void varstate() {
	getsym();//�õ�var�����IDENT
	value = 0;
	address = 0;
	paranum = 0;
	while (symid == IDENT) {
		string *s = new string[100]; //��̬���� ����ķ�11��100��IDENT����
		int index = 0;
		s[index++] = id;

		getsym();
		while (symid == COMMA) {
			getsym();
			if (symid != IDENT) {
				error(IDENTIFIERLACK);
				continue;
			}
			s[index++] = id;
			getsym();
		}
		if (symid != COLON) {//������ ð��":"
			error(COLONLACK);//����integer\char\array,����ֱ�ӵ��ֺţ�
		}
		else {
			getsym();
		}

		if (symid == INTTK) {
			kind = VINT;
			for (int i = 0; i < index; i++) {
				strcpy(name, s[i].c_str());
				pushtable(name, kind, value, address, paranum);
				insmidcode("int", "  ", "  ", name);
			}
			getsym();
		}
		else if (symid == CHARTK) {
			kind = VCHAR;
			for (int i = 0; i < index; i++) {
				strcpy(name, s[i].c_str());
				pushtable(name, kind, value, address, paranum);
				insmidcode("char", "  ", "  ", name);
			}
			getsym();
		}
		else if (symid == ARRAYTK) {
			int tmpnum;
			kind = symid;		//integer | char
			//��������С
			getsym();
			if (symid != LBRACK) {
				error(LBRACKLACK);
			}
			else {
				getsym();
			}
			if (symid != INTCON) {
				error(ARRAYLENGTHLACK);//������һ��IDNET���߷ֺ�
			}
			else {
				tmpnum = num;
				getsym();
				if (symid != RBRACK) {
					error(RIGHTBRACKLACK, 2);
				}
				else {
					getsym();
				}
				if (symid != OFTK) {
					error(DOLACK);
				}else{
					getsym();
				}
				if (symid == CHARTK) {
					kind = ACHAR;
				}
				else {
					kind = AINT;
				}
				for (int i = 0; i < index; i++) {
					strcpy(name, s[i].c_str());
					pushtable(name, kind, value, address, paranum, tmpnum);
					if (kind == AINT)
						insmidcode("inta", 0, num, name);
					else if (kind == ACHAR)
						insmidcode("chara", 0, num, name);
					getsym();
				}
			}
		}
		if (symid != SEMICN) {
			error(SEMICOLONLACK, 2);
		}
		else {
			getsym();
		}
	}
	if (symid == CONSTTK) {
		errnum++;
		printf("Error %d: Line %d, ��������λ�ô���\n", errnum, lnum);
		conststate();
	}
}

//18.<��ʽ������> :: = '('<��ʽ������>{; <��ʽ������>}')'
//19.<��ʽ������>:: = [var]<��ʶ��>{, <��ʶ��>}: <��������>
//�˴�����"("��")"֮�����������
// function f(var a,b,c:integer):integer
// function f:integer
// function f(a:integer;var b,c:char;d:integer):integer
// function f(a,b:integer):char
// procedure p(var a,b,c:integer)
// procedure p(a,b,c:integer)
// procedure p(a:char)
// procedure p;
void parametertable() {
	int pi;		//��¼(a:integer;var b,c:char;d:integer)�����ڣ����i�ĳ�ʼλ��
	int i = 0;		//��¼��������
	while (symid == VARTK || symid == IDENT) {
		pi = i;
		int var_flag = 0;
		string *s = new string[100];				//��̬���� ����ķ�11��100��IDENT����
		if (symid == VARTK) {
			var_flag = 1;
			getsym();
		}
		do {
			if (symid == COMMA)
				getsym();
			if (symid != IDENT) {
				error(IDENTIFIERLACK, 4);
			}
			else {
				s[i++] = id;
				getsym();
			}
		} while (symid == COMMA);
		if (symid != COLON) {
			error(COLONLACK);
		}
		else {
			getsym();
		}
		if (symid == INTTK || symid == CHARTK) {
			kind = PARA;			//4��ʾΪ��������
			value = 0;
			paranum = 0;
			for (int j = pi; j < i; j++) {
				address = j;							//��ַΪj����������λ�ã���ַȫ��Ϊ��Ե�ַ
				strcpy(name, s[j].c_str());
				pushtable(name, kind, value, address, paranum);						//����������������ű�
				if(var_flag==1){
					if (symid == INTTK) insmidcode("para", "int", "var", name);
					else if (symid == CHARTK) insmidcode("para", "char", "var", name);
				}
				else {
					if (symid == INTTK) insmidcode("para", "int", "  ", name);
					else if (symid == CHARTK) insmidcode("para", "char", "  ", name);
				}
			}
			getsym();
		}
		if (symid == SEMICN) {
			getsym();
			continue;
		}
		break;
	}
	paranum = i;//��ǰ�Ĳ�������
	maintable.element[maintable.findextable[maintable.ftotal]].paranum = paranum;//���뺯������������Ϣ
	return;
}

//38.<�������>  ::=   begin<���>{; <���>}end
//20.<���>  ::=   <��ֵ���>|<�������>|<��ѭ�����>|<���̵������>|<�������>|<�����>|
//					<д���>|<forѭ�����>|<��>
void complexsentence() {
	if (symid != BEGINTK) {
		error(BEGINLACK);
	}
	else {
		getsym();
	}
	sentencelist();
	if (symid != ENDTK) {
		error(ENDLACK);
	}
	getsym();
	return;//end��������";"����"."
}

//20.<���>  ::=   <��ֵ���>|<�������>|<��ѭ�����>|<���̵������>|<�������>|<�����>|<д���>|<forѭ�����>|<��>
//21.<��ֵ���>  ::=  <��ʶ��> := <���ʽ>| <������ʶ��> := <���ʽ> | <��ʶ��>'['<���ʽ>']':= <���ʽ>
//33.<�������> :: = if<����>then<���> | if<����>then<���>else<���>
//35.<��ѭ�����>::=   do<���> while<����>
//37.<���̵������>  :: = <��ʶ��>[<ʵ�ڲ�����>]
//38.<�������>  :: = begin<���>{ ; <���> }end
//39.<�����>  :: = read'('<��ʶ��>{, <��ʶ��>}')'
//40.<д���>  :: = write'('<�ַ���>, <���ʽ>')' | write'('<�ַ���>')' | write'('<���ʽ>')'
//36.<forѭ�����> :: = for <��ʶ��>  : = <���ʽ> ��downto | to�� <���ʽ> do <���> //����Ϊ1

void sentencelist() {
	bool flag = false;
	do {
		if (flag && symid != SEMICN) {
			error(SEMICOLONLACK);
		}
		else {
			if (symid == SEMICN)
				getsym();
		}
		sentence();
		flag = true;
	} while (symid == IDENT || symid == IFTK || symid == DOTK || symid == BEGINTK
		|| symid == READTK || symid == WRITETK || symid == FORTK || symid == SEMICN );
}

//checked!
//33.<�������> :: = if<����>then<���> | if<����>then<���>else<���>
//31.<����>  ::=   <���ʽ><��ϵ�����><���ʽ>
// if a>0 then
void ifsentence()
{
	char label1[10], label2[10], conditionvalue[30];
	strcpy(label1, nextlab());  //���������if��������label1��label1��if�Ľ�������
	strcpy(label2, nextlab());  //�������else����else�Ľ���λ����label2
	getsym();
	//��������
	condition();
	strcpy(conditionvalue, nowitem);  //���������ֵ��nowitem����,�˴�Ӧ���Ǹ���ʱ����$_x
	insmidcode("jne", "  ", "  ", label1);  //�Ƚϣ�Ϊ�ٵ�ʱ����ת
	if (symid != THENTK) {
		error(KEYWORDERROR, 4);
	}
	else {
		getsym();
	}
	sentence();
	insmidcode("jmp", "  ", "  ", label2);//��ִ��else�Ĳ���
	insmidcode("lab:", "  ", "  ", label1);
	if (symid != ELSETK) {
		insmidcode("lab:", "  ", "  ", label2);
		return;
	}
	else {
		getsym();
		sentence();
		insmidcode("lab:", "  ", "  ", label2);
		return;
	}
}

//checked
//31.<����>  ::=   <���ʽ><��ϵ�����><���ʽ>
//32.<��ϵ�����>  :: = < | <= | > | >= |= | <>
void condition()
{
	char place1[30], place2[30];
	expression();
	strcpy(place1, nowitem);  //����������һ�����ʽ
	if (symid == LSS || symid == LEQ || symid == GRE || symid == GEQ || symid == NEQ || symid == EQL) {
		if (symid == LSS) {
			getsym();
			expression();
			strcpy(place2, nowitem);
			insmidcode("<", place1, place2, "  ");
		}
		else if (symid == LEQ) {
			getsym();
			expression();
			strcpy(place2, nowitem);
			insmidcode("<=", place1, place2, "  ");
		}
		else if (symid == GRE) {
			getsym();
			expression();
			strcpy(place2, nowitem);
			insmidcode(">", place1, place2, "  ");
		}
		else if (symid == GEQ) {
			getsym();
			expression();
			strcpy(place2, nowitem);
			insmidcode(">=", place1, place2, "  ");
		}  
		else if (symid == NEQ) {
			getsym();
			expression();
			strcpy(place2, nowitem);
			insmidcode("<>", place1, place2, "  ");
		}
		else if (symid == EQL) {
			getsym();
			expression();
			strcpy(place2, nowitem);
			insmidcode("==", place1, place2, "  ");
		}
	}
	else {
		error(UNKNOWNOPRRATOR);
	}
	return;
}

//checked
//23.<���ʽ>  :: = [+| -]<��>{<�ӷ������><��>}
void expression()
{
	char place1[30], place2[30], place3[30];
	if (symid == PLUS || symid == MINU) {
		factortype = INT;
		if (symid == PLUS) {
			getsym();
			item();//���������Ľ�������ȫ�ֱ���nowitem
			strcpy(place3, nowitem);//����Ľ��������ʱ������λ��
		}
		if (symid == MINU) {
			getsym();
			item();//��
			strcpy(place1, nowitem);
			strcpy(place3, nextvar());
			insmidcode("-", "0 ", place1, place3); //place3 = 0 - (place1)
		}
	}
	else {
		item();
		strcpy(place3, nowitem);
	}
	while (symid == PLUS || symid == MINU) {
		factortype = INT;
		strcpy(place1, place3);
		if (symid == PLUS) {
			getsym();
			item();
			strcpy(place2, nowitem);
			strcpy(place3, nextvar());
			insmidcode("+", place1, place2, place3);//place3=place1+place2
			continue;
		}
		else {
			getsym();
			item();
			strcpy(place2, nowitem);
			strcpy(place3, nextvar());
			insmidcode("-", place1, place2, place3);
			continue;
		}
	}
	strcpy(nowitem, place3);//�ѱ��ʽ������ֵ�����p֮�� 
	return;
}

//	35|36.<ѭ�����>
void loopsentence()
{
	int s = 0;
	char names[30] ;// names1[30],names2[30],place2[30], place3[30]
	char label1[10], label2[10];// op_flag[5];
	strcpy(label1, nextlab());
	strcpy(label2, nextlab());

	//35.<��ѭ�����>::=   do<���> while<����>
	if (symid == DOTK) {
		getsym();
		insmidcode("lab:", "  ", "  ", label1);
		sentence();
		//getsym();
		if (symid != WHILETK) {
			//error
		}
		else {
			getsym();
		}
		condition();
		insmidcode("jne", "  ", "  ", label2);
		insmidcode("jmp", "  ", "  ", label1);
		insmidcode("lab:", "  ", "  ", label2);
		return;
	}
	//36.<forѭ�����> :: = for <��ʶ��>  : = <���ʽ> ��downto | to�� <���ʽ> do <���> //����Ϊ1
	//���ñ��ʽ��	expression() 
	//������䣺	sentence()
	if (symid == FORTK) {
		char item1[200];
		char item2[200];
		getsym();
		char ident[20];
		if (symid != IDENT) {
			error(UNACCEPTABLESENTENCE);//���Ϸ��ľ���
			return;
		}
		strcpy(names, id.c_str());
		strcpy(ident, id.c_str());
		getsym();
		if (symid != ASSIGN) {
			error(UNACCEPTABLESENTENCE);
			return;
		}
		s = searchst(names, 0);
		if (s < 0) {     //����ǳ���
			if (s == -1) error(VARNOTDEFINE, 1);  //"="����ǲ��Ϸ��ı�ʶ��
			else    error(ASSIGNCONST);
			return;
		}
		getsym();
		expression();//checked
		strcpy(item1, nowitem);
		int updown = symid;
		getsym();
		expression();//checked
		strcpy(item2, nowitem);
		if (updown == DOWNTK) {
			insmidcode(">=", item1, item2, "  ");
		}
		else {
			insmidcode("<=", item1, item2, "  ");
		}
		insmidcode("jne", "  ", "  ", label2);
		insmidcode("=", item1, "  ", names);
		insmidcode("lab:", "  ", "  ", label1);
		if (updown == DOWNTK) {
			insmidcode(">=", ident, item2, "  ");
		}
		else if (updown==TOTK){
			insmidcode("<=", ident, item2, "  ");
		}
		else {
			error(DOWNTOLACK);
		}
		insmidcode("jne", "  ", "  ", label2);
		if (symid != DOTK) {
			error(DOLACK);
		}
		else {
			getsym();
		}
		sentence();

		if (updown == DOWNTK) {
			insmidcode(">", ident, item2, "  ");
		}
		else {
			insmidcode("<", ident, item2, "  ");
		}
		insmidcode("jne", "  ", "  ", label2);

		if (updown == DOWNTK) {
			char*  tmp = nextvar();
			insmidcode("-", ident, "1", tmp);
			insmidcode("=", tmp, "  ", ident);
		}
		else {
			char*  tmp = nextvar();
			insmidcode("+", ident, "1", tmp);
			insmidcode("=", tmp, "  ", ident);
		}
		insmidcode("jmp", "  ", "  ", label1);
		insmidcode("lab:", "  ", "  ", label2);
		return;
	}
}



//checked
//<��ֵ���>  <���̵������>
//21.<��ֵ���>  ::=  <��ʶ��> := <���ʽ>| <������ʶ��> := <���ʽ> | <��ʶ��>'['<���ʽ>']':= <���ʽ>
//37.<���̵������>  ::=  <��ʶ��>[<ʵ�ڲ�����>]
//27.<ʵ�ڲ�����> ::= '(' <ʵ�ڲ���> {, <ʵ�ڲ���>}')'
void assignsentence() {
	//ident�Ǵ˿̵�symbol
	int s;
	int isVec = 0;
	char names[MAXIDENLEN];
	strcpy(names, id.c_str());	
	getsym();
	/*
	����ֵ��䣾;
	21.<��ֵ���>  ::=  <��ʶ��> := <���ʽ>| <������ʶ��> : = <���ʽ> | <��ʶ��>'['<���ʽ>']':= <���ʽ>
	��ֵ����û�зֺ�
	*/
	char place2[100];
	int flagL_A = 0;//LBRACK(if) to ASSIGN(if) 

	//��һ���ִ���Ⱥ���ߵĲ���
	if (symid == LBRACK) { //���������
		s = searchst(names, 0);
		if (s == -1)
		{
			error(VARNOTDEFINE, 1);
			return;
		}else if (s < -1) {
			s = -s / 2;
		}
		if (maintable.element[s].kind != AINT && maintable.element[s].kind != ACHAR) {
			error(ARRAYLACK);
		}
		getsym();
		expression();
		strcpy(place2, nowitem);
		if (isdigit(nowitem[0])) {
			int arrnums = 0;
			arrnums = arrnum(names);
			if (atoi(nowitem) >= arrnums) {
				printf("Warning: Line:%d index of array out of boundary, which should be less than %d.\n", lnum, atoi(nowitem));
			}
		}
		isVec = 1;
		if (symid != RBRACK) {
			error(RIGHTBRACKLACK, 1);
			return;
		}
		getsym();
		flagL_A = 1;
	}
	if (symid == ASSIGN) {
		//�˴�����Ϊelse if����Ϊ��һ��if�����󣬿��ܻ���н������if
		//���ű�������Ӧ��Ҫ�溯������ֵ�ĸ���,�Һ���ֻ����һ������ֵ,����ֵ�����ٸ�������ı������
		//<������ʶ��> :: = <���ʽ>     
		//23.<���ʽ>  :: = [+| -]<��>{<�ӷ������><��>}
		//-----------------------------��������ֵ��������������������ĸ�ֵ����
		int t = searchst(names, 1); 
		if (t < 0) {
			//��ͨ�ĸ�ֵ
			s = searchst(names, 0);
			if (s < -1) {						//����ǳ���
				error(ASSIGNCONST);				//"="����ǲ��Ϸ��ı�ʶ��
				return;
			}
			getsym();
			expression();
			char place3[30];
			strcpy(place3, nowitem);
			//=, 2, , temp			temp = 2;
			//[]=, a, i, t			a[i] = t;
			if (isVec) {
				insmidcode("[]=", names, place2, place3);
			}
			else {
				insmidcode("=", nowitem, "  ", names);
			}
			return;
		}
		else {
			
			//û�������

			maintable.element[t].is_return = true;
			getsym();
			expression();
			char place3[30];
			strcpy(place3, nowitem);
			insmidcode("ret", "  ", "  ", place3);
			return;
		}
	}
	//37.<���̵������>  ::=  <��ʶ��>[<ʵ�ڲ�����>]
	//27.<ʵ�ڲ�����> ::= '(' <ʵ�ڲ���> {, <ʵ�ڲ���>}')'
	//28.<ʵ�ڲ���> ::=   <���ʽ> 
	//˵�������һ�������ĺ����������û�зֺ�"��",Ҳ������
	if (symid == LPARENT) {
		//37.<���̵������>  ::=  <��ʶ��>[<ʵ�ڲ�����>]
		//27.<ʵ�ڲ�����> ::= '(' <ʵ�ڲ���> {, <ʵ�ڲ���>}')'
		//28.<ʵ�ڲ���> ::=   <���ʽ> 
		getsym();
		valueofpara();  //���㲢��¼������ֵ�Լ�����
		if (symid != RPARENT) {
			error(RIGHTPARENTLACK, 1);
			return;
		}
		s = searchst(names, 1);
		if (s == -1 * FUNCTIONNOTFOUND) {
			error(FUNCTIONNOTFOUND);
			return;
		}
		if (s >= 0) {
			//���������ʵ�θ������βθ�����ƥ��
			if (maintable.element[s].paranum != paranum) {
				error(FORMALPARANUMUNMATCH);
				getsym();
			}
			insmidcode("call", names, "  ", "  ");
			int presym = symid;
			getsym();
			if (symid != ENDTK) {
				if (symid != SEMICN) {
					error(SEMICOLONLACK, -1);
				}
			}
		}
	}
	else {
		//�˴�Ĭ�ϵ�����ʶ����Ϊ��������
		s = searchst(names, 1);
		if (s == -1 * FUNCTIONNOTFOUND) {
			error(FUNCTIONNOTFOUND);
			return;
		}
		if (s >= 0) {
			if (maintable.element[s].paranum != 0) {
				error(FORMALPARANUMUNMATCH);
				getsym();
			}
			insmidcode("call", names, "  ", "  ");
			int presym = symid;
		}
		else {
			error(UNKNOWN);
		}
	}
	return;
}

//20.<���>  :: = <��ֵ���> | <�������> | <��ѭ�����> | <���̵������> 
//					| <�������> | <�����> | <д���> | <forѭ�����> | <��>
void sentence() {

	if (symid == BEGINTK) {
		complexsentence();
		return;
	}
	//�������
	if (symid == IFTK) {
		ifsentence();
		return;
	}
	//ѭ�����
	if (symid == DOTK || symid == FORTK) {
		loopsentence();
		return;
	}

	//<��ֵ���>  <���̵������>
	//21.<��ֵ���>  ::=  <��ʶ��> := <���ʽ>| <������ʶ��> := <���ʽ> | <��ʶ��>'['<���ʽ>']':= <���ʽ>
	//37.<���̵������>  ::=  <��ʶ��>[<ʵ�ڲ�����>]
	//27.<ʵ�ڲ�����> ::= '(' <ʵ�ڲ���> {, <ʵ�ڲ���>}')'
	if (symid == IDENT) {
		assignsentence();
		return;
	}

	//�����
	//39.<�����>  :: = read'('<��ʶ��>{, <��ʶ��>}')'
	if (symid == READTK) {
		readsentence();
		return;
	}

	//д���
	if (symid == WRITETK) {
		writesentence();
		return;
	}

	//�����
	if (symid == SEMICN || symid == ENDTK ) {
		return;
	}
	//���Ϸ��ľ���
	error(UNACCEPTABLESENTENCE);
	return;
}

//checked
//26.<�����������> :: = <��ʶ��>[<ʵ�ڲ�����>]
//27.<ʵ�ڲ�����> :: = '(' <ʵ�ڲ���> {, <ʵ�ڲ���>}')'
//28.<ʵ�ڲ���> :: = <���ʽ>
//23.<���ʽ>  ::=   [+|-]<��>{<�ӷ������><��>}
//24.<��>  :: = <����>{ <�˷������><����> }
//25.<����>:: = <��ʶ��> | <��ʶ��>'['<���ʽ>']' | <�޷�������> | '('<���ʽ>')' | <�����������>
void valueofpara()
{
	int j = 0;
	vector<string> paraqueue;
	bool flag = false;
	do {
		if (flag && symid != COMMA) {
			error(COMMALACK);
		}
		else {
			if (symid == COMMA) {
				getsym();
			}
		}
		if (symid == PLUS || symid == MINU || symid == IDENT || symid == LPARENT || symid == CHARCON || symid == INTCON) {
			expression();
			paraqueue.push_back(nowitem);
			flag = true;
			j++;
		}
		else {
			error(UNKNOWN);
		}
	} while (symid == COMMA);
	char tc[20];
	for (int i = 0; i <(int) paraqueue.size(); i++) {
		strcpy(tc, paraqueue[i].c_str());
		insmidcode("fupa", "  ", "  ", tc);//function parameters
	}
	paranum = j;
	paraqueue.~vector();
	return;
}

//checked
//39.<�����>  ::=   read'('<��ʶ��>{,<��ʶ��>}')'
//9.<��ʶ��> ::=   <��ĸ>{<��ĸ>|<����>}
void readsentence()
{
	char names[30];
	int s;
	getsym();
	if (symid != LPARENT) {
		error(LEFTPARENTLACK, 1);
		return;
	}
	bool flag = false;
	do {
		if (flag && symid != COMMA) {
			error(COMMALACK);
		}
		getsym();
		if (symid != IDENT) {
			error(IDENTIFIERLACK, 3);         //���Ϸ��ı�־��
			return;
		}
		strcpy(names, id.c_str());
		s = searchst(names, 0);
		if (s < 0) {
			if (s == -1)
				error(VARNOTDEFINE, 1);            //δ�����־��
			else
				error(ASSIGNCONST, 1);
			return;
		}
		else {
			insmidcode("rd", "  ", "  ", names);
			flag = true;
			getsym();
		}
	} while (symid == COMMA);

	if (symid != RPARENT) {
		error(RIGHTPARENTLACK, 1);
		return;
	}
	getsym();
	return;
}

//checked
//40.<д���>  ::=   write'('<�ַ���>,<���ʽ>')'|write'('<�ַ���>')'|write'('<���ʽ>')'
void writesentence()
{
	char place1[200] = "", place2[30] = ""; //place1���ַ�����place2�Ǳ��ʽ
	getsym();
	if (symid != LPARENT) {
		error(LEFTPARENTLACK, 1);
		return;
	}
	getsym();
	if (symid == STRCON) {
		strcpy(place1, id.c_str());
		getsym();
		if (symid == COMMA) {
			getsym();
			expression();
			strcpy(place2, nowitem);
		}
	}
	else {
		expression();
		strcpy(place2, nowitem);
	}
	if (symid != RPARENT) {
		error(RIGHTPARENTLACK, 1);
		return;
	}
	insmidcode("wt", place1, place2, (factortype == CHAR) ? "char" : "int");
	getsym();
	return;
}

//checked
//24.<��>  :: = <����>{ <�˷������><����> }
void item()
{
	char place1[200], place2[200], place3[200];
	factor();
	strcpy(place3, nowitem);//���ֲ�����Ϊ�˶Ը�ֻ�и�ֵ�����
	while (symid == MULT || symid == DIV) {
		factortype = INT;
		strcpy(place1, place3);
		if (symid == MULT) {
			getsym();
			factor();
			strcpy(place2, nowitem);
			strcpy(place3, nextvar());//������ʱ����
			insmidcode("*", place1, place2, place3);
			continue;
		}
		if (symid == DIV) {
			getsym();
			factor();
			strcpy(place2, nowitem);
			strcpy(place3, nextvar());
			if (strcmp(place2, "0") == 0) {
				error(DIVIDEZERO);
			}
			insmidcode("/", place1, place2, place3);
			continue;
		}
	}
	strcpy(nowitem, place3);  //ÿһ���������ֵ���ڱ���nowitem����
	return;
}

//25.<����>:: = <��ʶ��> | <��ʶ��>'['<���ʽ>']'| <�޷�������> | '('<���ʽ>')'| <�����������>
//8.<�޷�������>::=   <����>{<����>}
void factor()
{
	char names[30], place3[30];
	//<��ʶ��> | <��ʶ��>'['<���ʽ>']'|<�����������>
	if (symid == IDENT) {
		strcpy(names, id.c_str());
		getsym();
		//�������ÿ�����������Ҳ����û�������ţ�����
		//call, f, , a			a = f(...)
		//call, f, ,			a = f

		if (symid == LPARENT) {//����в����ĺ�����������
			getsym();
			valueofpara();
			if (symid != RPARENT) {
				error(LEFTPARENTLACK, 2);
				return;
			}
			int t = searchst(names, 1);
			if (t < 0) {
				if (t == -1 * FUNCTIONRETURNNULL)
					error(FUNCTIONRETURNNULL);					//�����޷���ֵ�����ܳ����ڱ��ʽ��	
				else if (t == -1 * FUNCTIONNOTFOUND)
					error(FUNCTIONNOTFOUND, 1);
				return;
			}

			if (maintable.element[t].paranum != paranum) {
				error(FORMALPARANUMUNMATCH);
				getsym();
			}

			strcpy(place3, nextvar());//������ʱ����
			insmidcode("call", names, "  ", place3);//�����õķ���ֵ�������ʱ��������
			strcpy(nowitem, place3);
			getsym();
			return;
		}
		else if (symid != LBRACK) {
			//��������ַ����ж���ͨ��ʶ���ͺ�����ʶ��������
			int t1, t2;
			t1 = searchst(names, 0);				//������ҵ���ʶ����Ӧ�ĵ�ַ,���������
			t2 = searchst(names, 1);				//�麯�����Ƿ����
			strcpy(nowitem, names);

			if (t1 >= 0) {
				strcpy(nowitem, names);
				return;
			}
			else if (t2 > 0) {
				strcpy(place3, nextvar());//������ʱ����
				insmidcode("call", names, "  ", place3);//�����õķ���ֵ�������ʱ��������
				strcpy(nowitem, place3);
				return;
			}
			else {
				error(VARNOTDEFINE);
			}
		}
		if (symid == LBRACK) {      //��������������
			getsym();
			int tmptype = factortype;
			expression();
			factortype = tmptype;
			char tmp[30];
			strcpy(tmp, nowitem);

			if (isdigit(nowitem[0])) {
				int arrnums = 0;
				arrnums = arrnum(names);
				if (atoi(tmp) >= arrnums) {
					printf("warning: Line:%d index of array out of boundary, which should be less than %d.\n", lnum, atoi(tmp));
				}
			}
			if (symid != RBRACK) {
				error(RIGHTBRACKLACK);
				return;
			}
			else {
				strcpy(nowitem, nextvar());
				insmidcode("geta", names, tmp, nowitem);
				getsym();
			}
		}
		return;
	}

	//'('<���ʽ>')'
	//23.<���ʽ>  :: = [+| -]<��>{<�ӷ������><��>}
	if (symid == LPARENT) {
		getsym();
		expression();
		if (symid != RPARENT) {
			error(RIGHTPARENTLACK, 2);
			return;
		}
		getsym();
		return;
	}

	//<�޷�������> 
	//8.<�޷�������>::=   <����>{<����>}
	if (symid == INTCON) {
		sprintf(nowitem, "%d", 1 * num);
		factortype = INT;
		getsym();
		return;
	}
	error(EXPRESSIONERROR);                  //���ʽȱʧ�����
	return;
}