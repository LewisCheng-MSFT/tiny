// *������ѧϰĿ�ģ��ش�������
// �ļ�: code.h
// ����: ����
// ����: 2010/3/20
// ����: TINY�������Ĵ����ͷŹ��ܣ��Լ���TM�����Ľӿ�

#ifndef _CODE_H_
#define _CODE_H_

// ���������
#define pc 7

// �ڴ�ָ�룬ָ���ڴ涥�ˣ���ʱ�洢�ã�
#define mp 6

// ȫ��ָ�룬ָ���ڴ�׶ˣ�ȫ�ִ洢�ã�
#define gp 5

// �ۼ���
#define ac 0

// �ڶ��ۼ���
#define ac1 1

//---------------------------�����ͷŹ���-----------------------------

// �ͷ�һ�����Ĵ������TMָ��
//	op = ��������
//	r = Ŀ��Ĵ���
//	s = ��һԴ�Ĵ���
//	t = �ڶ�Դ�Ĵ���
//	c = ��TraceCode = trueʱҪ��ӡ��ע��
void emitRO(char* op, int r, int s, int t, char* c);

// �ͷ�һ���Ĵ������ڴ��TMָ��
//	op = ��������
//	r = Ŀ��Ĵ���
//	d = ƫ����
//	s = ���Ĵ���
//	c = ��TraceCode = trueʱҪ��ӡ��ע��
void emitRM(char* op, int r, int d, int s, char* c);

// ����howManyָ���Ĵ����ַ�����Ա��Ժ���backpatch��
// �������ص�ǰ����ĵ�ַ
int emitSkip(int howMany);

// ����locָ������ǰ�����ĵ�ַ
void emitBackup(int loc);

// �ָ���ǰ�����ַ����ߵ���ǰδ���ͷŵĵ�ַ
void emitRestore(void);

// �ڴ����ļ��д�ӡcָ����ע����
void emitComment(char* c);

// ���ͷ�һ���Ĵ������ڴ��TMָ��ʱ��ת��һ���������õ�һ����pcΪ��ַ���������
void emitRM_Abs(char* op, int r, int a, char* c);

#endif