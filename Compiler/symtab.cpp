// *������ѧϰĿ�ģ��ش�������
// �ļ�: symtab.cpp
// ����: ����
// ����: 2010/3/20
// ����: TINY�������ķ��ű�ʵ�֣�ֻ�������һ�����ű�

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

// ��ϣ��Ĵ�С
#define SIZE 211

// ���ڹ�ϣ�����еĳ���
#define SHIFT 4

// ��ϣ����
static int hash(char* key)
{
	int temp = 0;
	int i = 0;
	while (key[i] != '\0')
	{
		temp = ((temp << SHIFT) + key[i]) % SIZE;
		++i;
	}
	return temp;
}

// Դ�ļ��кŵ��б����ڱ�Ǳ��������õ�λ��
typedef struct LineListRec {
	int lineno;
	struct LineListRec* next;
}* LineList;

// Ͱ�б���һ����¼����һ������
// �������ƣ�������ڴ��ַ����������Դ�����е��к��б�
typedef struct BucketListRec {
	char* name;
	LineList lines;
	// �������ڴ��ַ
	int memloc;
	struct BucketListRec* next;
}* BucketList;

// ��ϣ��
static BucketList hashTable[SIZE];

// ���к����ڴ��ַ������ű�
// loc = �ڴ��ַ�������״β��룬�Ժ����
void st_insert(char* name, int lineno, int loc)
{
	int h = hash(name);
	BucketList l = hashTable[h];
	// �ڱ���Ѱ�ұ���
	while (l && strcmp(name, l->name))
		l = l->next;
	// ���������ڱ���
	if (!l)
	{
		l = (BucketList)malloc(sizeof(struct BucketListRec));
		l->name = name;
		l->lines = (LineList)malloc(sizeof(struct LineListRec));
		l->lines->lineno = lineno;
		l->lines->next = NULL;
		l->memloc = loc;
		// ��ӵ���Ͱ�е���λ
		l->next = hashTable[h];
		hashTable[h] = l;
	}
	else
	{ // �ڱ����ҵ��ˣ����Խ���Ҫ����к�
		LineList t = l->lines;
		while (t->next)
			t = t->next;
		t->next = (LineList)malloc(sizeof(struct LineListRec));
		t->next->lineno = lineno;
		t->next->next = NULL;
	}
}

// ����һ���������ڴ��ַ��-1��ʾδ�ҵ�
int st_lookup(char* name)
{
	int h = hash(name);
	BucketList l = hashTable[h];
	while (l && strcmp(name, l->name))
		l = l->next;
	if (!l)
		return -1;
	else
		return l->memloc;
}

// �����ű�����ݸ�ʽ����������б��ļ�
void printSymTab(FILE* listing)
{
	fprintf(listing, "Variable Name  Location     Line Numbers\n");
	fprintf(listing, "-------------  --------     ------------\n");
	for (int i = 0; i < SIZE; ++i)
	{
		if (hashTable[i])
		{
			BucketList l = hashTable[i];
			while (l)
			{
				LineList t = l->lines;
				fprintf(listing, "%-14s ", l->name);
				fprintf(listing, "%-8d  ", l->memloc);
				while (t)
				{
					fprintf(listing, "%4d ", t->lineno);
					t = t->next;
				}
				fprintf(listing, "\n");
				l = l->next;
			}
		}
	}
}