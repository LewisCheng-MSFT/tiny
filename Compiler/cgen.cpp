// *������ѧϰĿ�ģ��ش�������
// �ļ�: cgen.cpp
// ����: ����
// ����: 2010/3/20
// ����: TINY�������Ĵ���������ʵ��

#include "globals.h"
#include "symtab.h"
#include "code.h"
#include "cgen.h"

// ��ʱ�������ڴ�ƫ��
// ÿ��һ����ʱ����������ʱ�����ᱻ��1
// ��������ʱ��������ȡʱ�����ᱻ��1
static int tmpOffset = 0;

// �ڲ��ݹ������������ԭ��
static void cGen(TreeNode* tree);

// ��һ�����ڵ㴦���ɴ���
static void genStmt(TreeNode* tree)
{
	TreeNode* p1, * p2, * p3;
	int savedLoc1, savedLoc2, currentLoc;
	int loc;
	switch (tree->kind.stmt)
	{
	case IfK:
		if (TraceCode)
			emitComment("-> if");
		p1 = tree->child[0];
		p2 = tree->child[1];
		p3 = tree->child[2];
		// Ϊ���Ա��ʽ��������
		cGen(p1);
		savedLoc1 = emitSkip(1);
		emitComment("if: jump to else belongs here");
		// ��then�����ϵݹ�
		cGen(p2);
		savedLoc2 = emitSkip(1);
		emitComment("if: jump to end belongs here");
		currentLoc = emitSkip(0);
		emitBackup(savedLoc1);
		emitRM_Abs("JEQ", ac, currentLoc, "if: jmp to else");
		emitRestore();
		// ��else�����ϵݹ�
		cGen(p3);
		currentLoc = emitSkip(0);
		emitBackup(savedLoc2);
		emitRM_Abs("LDA", pc, currentLoc, "jmp to end");
		emitRestore();
		if (TraceCode)
			emitComment("<- if");
		break;
	case RepeatK:
		if (TraceCode)
			emitComment("-> repeat");
		p1 = tree->child[0];
		p2 = tree->child[1];
		savedLoc1 = emitSkip(0);
		emitComment("repeat: jump after body comes back here");
		// ����ѭ����Ĵ���
		cGen(p1);
		// �������������Ĵ���
		cGen(p2);
		emitRM_Abs("JEQ", ac, savedLoc1, "repeat: jmp back to body");
		if (TraceCode)
			emitComment("<- repeat");
		break;
	case AssignK:
		if (TraceCode)
			emitComment("-> assign");
		// Ϊ�Ҳ�������������
		cGen(tree->child[0]);
		// ���ڴ洢ֵ
		loc = st_lookup(tree->attr.name);
		emitRM("ST", ac, loc, gp, "assign: store value");
		if (TraceCode)
			emitComment("<- assign");
		break;
	case ReadK:
		emitRO("IN", ac, 0, 0, "read integer value");
		loc = st_lookup(tree->attr.name);
		emitRM("ST", ac, loc, gp, "read: store value");
		break;
	case WriteK:
		// Ϊ���ʽ��������
		cGen(tree->child[0]);
		// ���������
		emitRO("OUT", ac, 0, 0, "write ac");
		break;
	}
}

// �ڱ��ʽ����������
static void genExp(TreeNode* tree)
{
	int loc;
	TreeNode* p1, * p2;
	switch (tree->kind.exp)
	{
	case ConstK:
		if (TraceCode)
			emitComment("-> Const");
		// ʹ��LDCָ���������ͳ���
		emitRM("LDC", ac, tree->attr.val, 0, "load const");
		if (TraceCode)
			emitComment("<- Const");
		break;
	case IdK:
		if (TraceCode)
			emitComment("-> Id");
		loc = st_lookup(tree->attr.name);
		emitRM("LD", ac, loc, gp, "load id value");
		if (TraceCode)
			emitComment("-> Id");
		break;
	case OpK:
		if (TraceCode)
			emitComment("-> Op");
		p1 = tree->child[0];
		p2 = tree->child[1];
		// Ϊac = �������������
		cGen(p1);
		// ���������ջ����
		emitRM("ST", ac, tmpOffset--, mp, "op: push left");
		// Ϊac = �Ҳ�����������
		cGen(p2);
		// ���������������
		emitRM("LD", ac1, ++tmpOffset, mp, "op: load left");
		switch (tree->attr.op)
		{
		case PLUS:
			emitRO("ADD", ac, ac1, ac, "op +");
			break;
		case MINUS:
			emitRO("SUB", ac, ac1, ac, "op -");
			break;
		case TIMES:
			emitRO("MUL", ac, ac1, ac, "op *");
			break;
		case OVER:
			emitRO("DIV", ac, ac1, ac, "op /");
			break;
		case LT:
			emitRO("SUB", ac, ac1, ac, "op <");
			emitRM("JLT", ac, 2, pc, "br if true");
			emitRM("LDC", ac, 0, ac, "false case");
			emitRM("LDA", pc, 1, pc, "unconditional jmp");
			emitRM("LDC", ac, 1, ac, "true case");
			break;
		case EQ:
			emitRO("SUB", ac, ac1, ac, "op ==");
			emitRM("JEQ", ac, 2, pc, "br if true");
			emitRM("LDC", ac, 0, ac, "false case");
			emitRM("LDA", pc, 1, pc, "unconditional jmp");
			emitRM("LDC", ac, 1, ac, "true case");
			break;
		default:
			emitComment("BUG: Unknown operator");
			break;
		}
		if (TraceCode)
			emitComment("-> Op");
		break;
	}
}

static void cGen(TreeNode* tree)
{
	if (tree)
	{
		switch (tree->nodekind)
		{
		case StmtK:
			genStmt(tree);
			break;
		case ExpK:
			genExp(tree);
			break;
		}
		cGen(tree->sibling);
	}
}

//----------------------------������������������--------------------------------

// ͨ�������﷨�����������ļ�
// ����codefileָ�������ļ����ļ�������������ע���е��ļ���
void codeGen(TreeNode* syntaxTree, char* codefile)
{
	char* s = (char*)malloc(strlen(codefile) + 7);
	strcpy(s, "File: ");
	strcat(s, codefile);
	emitComment("TINY Compilation to TM Code");
	emitComment(s);
	// ������׼��ڴ���
	emitComment("Standard prelude:");
	emitRM("LD", mp, 0, ac, "load maxaddress from location 0");
	emitRM("ST", ac, 0, ac, "clear location 0");
	emitComment("End of standard prelude");
	// ����TINY����Ĵ���
	cGen(syntaxTree);
	// ���
	emitComment("End of execution.");
	emitRO("HALT", 0, 0, 0, "");
}