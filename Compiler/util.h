// *������ѧϰĿ�ģ��ش�������
// �ļ�: util.h
// ����: ����
// ����: 2010/3/20
// ����: TINY��������ʵ�ú���

#ifndef _UTIL_H_
#define _UTIL_H_

// ����Ǻż���������б��ļ�
void printToken(TokenType, const char*);

// Ϊ�﷨�������µ����ڵ�
TreeNode* newStmtNode(StmtKind);

// Ϊ�﷨�������µı��ʽ�ڵ�
TreeNode* newExpNode(ExpKind);

// ���䲢����ĳ�����ַ�����һ�ݿ���
char* copyString(char*);

// ����﷨�����б��ļ��������������������
void printTree(TreeNode*);

#endif