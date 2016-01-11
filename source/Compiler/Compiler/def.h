#pragma once
#ifndef compiler_def_h
#define compiler_def_h

/*-----------------------��ʶ��-------------------------*/
//�谴�ֵ�������
#define ARRAYTK		0
#define BEGINTK		1
#define CHARTK		2
#define CONSTTK		3	
#define DOTK		4
#define DOWNTK		5
#define ELSETK		6
#define ENDTK		7
#define FORTK		8
#define FUNCTK		9
#define IDENT       10	
#define IFTK		11
#define INTTK		12
#define OFTK		13
#define PROCETK		14
#define READTK		15
#define THENTK		16
#define TOTK		17
#define VARTK		18
#define WHILETK		19
#define WRITETK		20
//����Ϊ�ؼ��֣����·��Ų���Ҫ�����ֵ���
#define ASSIGN      21	//:=
#define SEMICN      22	//;
#define COMMA       23	//,
#define STRCON      24	//strcon
#define PLUS        25	//+
#define MINU        26	//-
#define MULT        27	//*
#define PERIOD      28	//.
#define DIV         29	// /
#define COLON       30	//:
#define LSS         31	//<
#define QMARK       32	//' single quotation
#define LEQ         33	//<=
#define DQMARK      34	//"	double
#define GRE         35	//>
#define LPARENT     36	//(
#define GEQ         37	//>=
#define RPARENT     38	//)
#define EQL         39	//=      ����ʹ�ó�����1����������Ĺ�ϵ��������ȼ���"=="��2��Ϊ�����ĸ�ֵ�ĵȺ�
#define LBRACK      40	//[
#define NEQ         41	//<>
#define RBRACK      42	//]
#define INTCON		43	//������10
#define CHARCON		44	//������'a'

/*-----------------------��������-------------------------*/
#define NOSUCHFILE				404	//�ļ�������
#define FILEINCOMPLETE			0	//�ļ�������	
#define DOUBLEQUOTESLACK		1	//˫����ȱʧ
#define UNACCEPTATLECHAR		2	//�����Խ��ܵ�char
#define SINGLEQUOTESLACK		3	//������ȱʧ
#define OUTOFTABLE				4	//���ű�
#define SYMBOLCONFLICT			5	//�ַ�����ͻ
#define CSTDEFINEFAIL			6   //��������ʧ��
#define VARNOTINIT				7   //����δ��ʼ��
#define UNKNOWRIGHTSYM			8   //�Ⱥ��ұ��ַ��Ƿ�
#define SEMICOLONLACK			9   //��ʧ������
#define KEYWORDERROR			10  //�ؼ��ִ���
#define IDENTIFIERLACK			11  //��ʧ��־��
#define RIGHTBRACKLACK			12  //��ʧ��]��
#define FUNCTIONNOTFOUND		13  //���ú���δ����
#define FORMALPARANUMUNMATCH	14  //�βθ�����ƥ��
#define VARNOTDEFINE			15  //δ�������
#define LEFTPARENTLACK			16  //��ʧ������
#define RIGHTPARENTLACK			17  //��ʧ������
#define IMMLACK					18  //��ʧ������
#define LBRACKLACK				19	//��ʧ��[��
#define FUNCTIONRETURNNULL		20  //�����޷���ֵȴ�����ڲ���
#define EXPRESSIONERROR			21  //���ʽȱʧ�����
#define UNACCEPTABLESENTENCE	22  //���Ӳ��Ϸ�
#define ASSIGNCONST				23  //��������ֵ
#define OFLACK					24	//ȱʧof
#define NONERETURN				25  //ȱ�ٷ���ֵ
#define PLUSMINULACK			26  //ȱ�١�+����-��
#define MAINLACK				27  //ȱ��main����
#define MOREWORD				28  //main���������ж����ַ�
#define CONSTNOTINIT			29  //����û�г�ʼ��
#define PERIODLACK				30	//ȱʧ�������ĵ��"."
#define COLONLACK				31	//ȱʧ��
#define ARRAYLENGTHLACK			32	//����ȱʧ����
#define DOLACK					33	//ȱʧdo
#define BEGINLACK				34  //ȱʧbegin
#define ENDLACK					35	//ȱʧend
#define DIVIDEZERO				36	//����Ϊ0
#define DOWNTOLACK				37	//downto/toȱʧ
#define COMMALACK				38	//����ȱʧ
#define ARRAYLACK				39	//����Ԫ�ز�����
#define UNKNOWNOPRRATOR			40	//δ�����ϵ�����
#define RETURNTYPELACK			41	//����ȱ�ٷ���ֵ����
#define PARANUMZERO				42  //�������� �ڲ�����λΪ0
#define UNKNOWN					50	//δ֪����

#define QUIT				1		//ֱ���˳�����
#define DONOTHING			2		//�����κδ���
#define ICA					3		//������һ��������������INTTK/CHARTK/ARRAYTK		
#define DIWFIRWB			4		//������һ�����¼��ϵ�Ԫ��
//{ DOTK IFTK WHILETK FORTK IDENT READTK WRITETK BEGINTK}
#define CS					5		//������һ�����Ż��߷ֺ�
#define CLR					6		//������һ�����Ż�������
#define IWFRSPIFCV			7		//������һ������е�FIRST����Ԫ��֮һ
#define IWFIRWE				8		//������һ�����¼��ϵ�Ԫ�أ�
//{IFTK WHILETK FORTK IDENT READTK WRITETK ELSETK}
#define IWFXXXANE			9		//������һ�����¼��ϵ�Ԫ�أ�
//{ IFTK WHILETK FORTK IDENT RETURNTK READTK WRITETK SEMICN ELSETK RPARENT COMMA PLUS MINU MULT DIV LSS LEQ GRE GEQ NEQ EQL}
#define	IC					10		//����INTCON;


#endif