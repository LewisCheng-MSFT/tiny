// *������ѧϰĿ�ģ��ش�������
// �ļ�: symtab.h
// ����: ����
// ����: 2010/3/20
// ����: TINY�������ķ��ű�ӿ�

#ifndef _SYMTAB_H_
#define _SYMTAB_H_

// ���к����ڴ��ַ������ű�
// loc = �ڴ��ַ�������״β��룬�Ժ����
void st_insert(char* name, int lineno, int loc);

// ����һ���������ڴ��ַ��-1��ʾδ�ҵ�
int st_lookup(char* name);

// �����ű�����ݸ�ʽ����������б��ļ�
void printSymTab(FILE* listing);

#endif