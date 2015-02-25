// *������ѧϰĿ�ģ��ش�������
// �ļ�: analyze.cpp
// ����: ����
// ����: 2010/3/20
// ����: TINY�����������������ʵ��

#include "globals.h"
#include "symtab.h"
#include "analyze.h"

// �����ڴ��ַ�ļ�����
static int location = 0;

// �����﷨����ͨ�õݹ麯��
// ����preProc����Ӧ����ǰ�������
// ����postProc����Ӧ���ں��������
// �﷨����ָ��tָ��
static void traverse(TreeNode* t,
					 void (* preProc)(TreeNode*),
					 void (* postProc)(TreeNode*))
{
	if (!t) return;
	if (preProc)
		preProc(t);
	for (int i = 0; i < MAXCHILDREN; i++)
		traverse(t->child[i], preProc, postProc);
	if (postProc)
		postProc(t);
	// �������������ʱ����
	traverse(t->sibling, preProc, postProc);
}

// ��t����д�ŵı�ʶ��������ű�
static void insertNode(TreeNode* t)
{
	switch (t->nodekind)
	{
	case StmtK:
		if (t->kind.stmt == AssignK || t->kind.stmt == ReadK)
		{
			// �����ڷ��ű��У����Կ����µĶ���
			if (st_lookup(t->attr.name) == -1)
				st_insert(t->attr.name, t->lineno, location++);
			else
				// �Ѿ��ڷ��ű��У�����������ַ�������к���ӽ�ȥ
				st_insert(t->attr.name, t->lineno, 0);
		}
		break;
	case ExpK:
		if (t->kind.exp == IdK)
		{
			if (st_lookup(t->attr.name) == -1)
				st_insert(t->attr.name, t->lineno, location++);
			else
				st_insert(t->attr.name, t->lineno, 0);
		}
		break;
	}
}

// ǰ������﷨����������ű�
void buildSymtab(TreeNode* syntaxTree)
{
	traverse(syntaxTree, insertNode, NULL);
	if (TraceAnalyze)
	{
		fprintf(listing, "\nSymbol table:\n\n");
		printSymTab(listing);
	}
}

static void typeError(TreeNode* t, char* message)
{
	fprintf(listing, "Type error at line %d: %s\n", t->lineno, message);
	Error = true;
}

// �ڵ������ִ�����ͼ��
static void checkNode(TreeNode* t)
{
	switch (t->nodekind)
	{
	case ExpK:
		switch (t->kind.exp)
		{
		case OpK:
			if (t->child[0]->type != Integer || t->child[1]->type != Integer)
				typeError(t, "Op applied to non-integer");
			if (t->attr.op == EQ || t->attr.op == LT)
				t->type = Boolean;
			else
				t->type = Integer;
			break;
		case ConstK:
		case IdK:
			t->type = Integer;
			break;
		}
		break;
	case StmtK:
		switch (t->kind.stmt)
		{
		case IfK:
			if (t->child[0]->type == Integer)
				typeError(t->child[0], "if test is not Boolean");
			break;
		case AssignK:
			if (t->child[0]->type != Integer)
				typeError(t->child[0], "assignment of non-integer value");
			break;
		case WriteK:
			if (t->child[0]->type != Integer)
				typeError(t->child[0], "write of non-integer value");
			break;
		case RepeatK:
			if (t->child[1]->type == Integer)
				typeError(t->child[1], "repeat test is not Boolean");
			break;
		}
		break;
	}
}

// ��������﷨����ִ�����ͼ��
void typeCheck(TreeNode* syntaxTree)
{
	traverse(syntaxTree, NULL, checkNode);
}