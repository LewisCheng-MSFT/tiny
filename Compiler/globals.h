// *������ѧϰĿ�ģ��ش�������
// �ļ�: globals.h
// ����: ����
// ����: 2010/3/20
// ����: TINY��������ȫ�����ͺͱ������������������ͷ�ļ�֮ǰ

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

// �����ֵĸ���
#define MAXRESERVED 8

typedef enum {
	// ���ǼǺ�
	ENDFILE, ERROR,
	// ������
	IF, THEN, ELSE, END, REPEAT, UNTIL, READ, WRITE,
	// ���ַ��Ǻ�
	ID, NUM,
	// ����Ǻţ�������
	ASSIGN, EQ, LT, PLUS, MINUS, TIMES, OVER, LPAREN, RPAREN, SEMI
} TokenType;

// Դ�ļ�(�ı�)
extern FILE* source;
// ����б��ļ�(�ı�)
extern FILE* listing;
// TMģ��������ļ�(�ı�)
extern FILE* code;

// Դ�ļ��е��к�
extern int lineno;

//----------------------�﷨�����е��﷨��--------------------------

typedef enum { StmtK, ExpK } NodeKind;
typedef enum { IfK, RepeatK, AssignK, ReadK, WriteK } StmtKind;
typedef enum { OpK, ConstK, IdK } ExpKind;

// ExpType���������ͼ��
typedef enum { Void, Integer, Boolean } ExpType;

#define MAXCHILDREN 3

typedef struct treeNode {
	struct treeNode* child[MAXCHILDREN];
	struct treeNode* sibling;
	int lineno;
	// �ڵ����ͣ����or���ʽ��
	NodeKind nodekind;
	// �������䣨���ʽ��������������䣨���ʽ����
	union { StmtKind stmt; ExpKind exp; } kind;
	// �ڵ�����ֵ������or����ֵor��ʶ������
	union {
		TokenType op;
		int val;
		char* name;
	} attr;
	// ˵�����ʽ��������ͣ��������ͼ��
	ExpType type;
} TreeNode;

//-----------------------Flags for tracing----------------------------

// EchoSource = trueʹ���﷨����ʱ��Դ�������кŻᱻ������б��ļ���
extern bool EchoSource;

// TraceScan = trueʹ��ÿ����ɨ������ļǺŵ���Ϣ�ᱻ������б��ļ���
extern bool TraceScan;

// TraceParse = trueʹ���﷨���ᱻ���Ի���������б��ļ��У����ӽڵ�������
extern bool TraceParse;

// TraceAnalyze = true ʹ�÷��ű�Ĳ���Ͳ�ѯ����������¼���б��ļ���
extern bool TraceAnalyze;

// TraceCode = true ʹ�ñ������ڲ���TM�����ļ�ʱ���Զ����ע��
extern bool TraceCode;

// Error = true ʹ�õ�һ���������ʱ����������������
extern bool Error;

#endif