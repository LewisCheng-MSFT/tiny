// *������ѧϰĿ�ģ��ش�������
// �ļ�: parse.cpp
// ����: ����
// ����: 2010/3/20
// ����: TINY���������﷨������ʵ��

#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"

// ��ǰ�ļǺ�
static TokenType token;

// �ݹ���ú�����ԭ��
static TreeNode* stmt_sequence(void);
static TreeNode* statement(void);
static TreeNode* if_stmt(void);
static TreeNode* repeat_stmt(void);
static TreeNode* assign_stmt(void);
static TreeNode* read_stmt(void);
static TreeNode* write_stmt(void);
static TreeNode* exp(void);
static TreeNode* simple_exp(void);
static TreeNode* term(void);
static TreeNode* factor(void);

// ����﷨����
static void syntaxError(char* message)
{
	fprintf(listing, "\n>> ");
	fprintf(listing, "Syntax error at line %d: %s", lineno, message);
	Error = true;
}

// ���ԡ���һ���Ǻţ����Ϊ�������ģ����Զ�������һ���Ǻţ����򱨴�
static void eat(TokenType expected)
{
	if (token == expected)
		token = getToken();
	else
	{
		syntaxError("unexpected token -> ");
		printToken(token, tokenString);
		fprintf(listing, "	");
	}
}

// �����������
TreeNode* stmt_sequence(void)
{
	TreeNode* t = statement();
	TreeNode* p = t;
	while (token != ENDFILE && token != END && token != ELSE && token != UNTIL)
	{
		TreeNode* q;
		eat(SEMI);
		q = statement();
		if (q != NULL)
		{
			if (t == NULL)
				t = p = q;
			else
			{
				// ����pҲ����ΪNULL
				p->sibling = q;
				p = q;
			}
		}
	}
	return t;
}

// �������
TreeNode* statement(void)
{
	TreeNode* t = NULL;
	switch (token)
	{
	case IF:
		t = if_stmt();
		break;
	case REPEAT:
		t = repeat_stmt();
		break;
	case ID:
		t = assign_stmt();
		break;
	case READ:
		t = read_stmt();
		break;
	case WRITE:
		t = write_stmt();
		break;
	default:
		syntaxError("unexpected token -> ");
		printToken(token, tokenString);
		token = getToken();
		break;
	}
	return t;
}

// ����if���
TreeNode* if_stmt(void)
{
	TreeNode* t = newStmtNode(IfK);
	eat(IF);
	if (t != NULL)
		t->child[0] = exp();
	eat(THEN);
	if (t != NULL)
		t->child[1] = stmt_sequence();
	if (token == ELSE)
	{
		eat(ELSE);
		if (t != NULL)
			t->child[2] = stmt_sequence();
	}
	eat(END);
	return t;
}

// ����repeat���
TreeNode* repeat_stmt(void)
{
	TreeNode* t = newStmtNode(RepeatK);
	eat(REPEAT);
	if (t != NULL)
		t->child[0] = stmt_sequence();
	eat(UNTIL);
	if (t != NULL)
		t->child[1] = exp();
	return t;
}

// ������ֵ���
TreeNode* assign_stmt(void)
{
	TreeNode* t = newStmtNode(AssignK);
	if (t != NULL && token == ID)
		t->attr.name = copyString(tokenString);
	eat(ID);
	eat(ASSIGN);
	if (t != NULL)
		t->child[0] = exp();
	return t;
}

// ����read���
TreeNode* read_stmt(void)
{
	TreeNode* t = newStmtNode(ReadK);
	eat(READ);
	if (t != NULL && token == ID)
		t->attr.name = copyString(tokenString);
	eat(ID);
	return t;
}

// ����write���
TreeNode* write_stmt(void)
{
	TreeNode* t = newStmtNode(WriteK);
	eat(WRITE);
	if (t != NULL)
		t->child[0] = exp();
	return t;
}

// �������ʽ���߼����ʽ���������ʽ
TreeNode* exp(void)
{
	TreeNode* t = simple_exp();
	if (token == LT || token == EQ)
	{
		TreeNode* p = newExpNode(OpK);
		if (p != NULL)
		{
			p->child[0] = t;
			p->attr.op = token;
			t = p;
		}
		eat(token);
		if (t != NULL)
			t->child[1] = simple_exp();
	}
	return t;
}

// �����򵥱��ʽ���� <+|-> ��
TreeNode* simple_exp(void)
{
	TreeNode* t = term();
	while (token == PLUS || token == MINUS)
	{
		TreeNode* p = newExpNode(OpK);
		if (p != NULL)
		{
			p->child[0] = t;
			p->attr.op = token;
			t = p;
			eat(token);
			t->child[1] = term();
		}
	}
	return t;
}

// ��������� <*|/> ����
TreeNode* term(void)
{
	TreeNode* t = factor();
	while (token == TIMES || token == OVER)
	{
		TreeNode* p = newExpNode(OpK);
		if (p != NULL)
		{
			p->child[0] = t;
			p->attr.op = token;
			t = p;
			eat(token);
			t->child[1] = factor();
		}
	}
	return t;
}

// �������ӣ���������ʶ�����ӱ��ʽ
TreeNode* factor(void)
{
	TreeNode* t = NULL;
	switch (token)
	{
	case NUM:
		t = newExpNode(ConstK);
		if (t != NULL && token == NUM)
			t->attr.val = atoi(tokenString);
		eat(NUM);
		break;
	case ID:
		t = newExpNode(IdK);
		if (t != NULL && token == ID)
			t->attr.name = copyString(tokenString);
		eat(ID);
		break;
	case LPAREN:
		eat(LPAREN);
		t = exp();
		eat(RPAREN);
		break;
	default:
		syntaxError("unexpected token -> ");
		printToken(token, tokenString);
		token = getToken();
		break;
	}
	return t;
}

//------------------------------�﷨����������Ҫ����-----------------------------

// ���������������﷨��
TreeNode* parse(void)
{
	TreeNode* t;
	token = getToken();
	t = stmt_sequence();
	if (token != ENDFILE)
		syntaxError("Code ends before file\n");
	return t;
}