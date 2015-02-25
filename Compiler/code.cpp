// *������ѧϰĿ�ģ��ش�������
// �ļ�: code.cpp
// ����: ����
// ����: 2010/3/20
// ����: TINY�������Ĵ����ͷŹ��ܵ�ʵ��

#include "globals.h"
#include "code.h"

// ��ǰָ���ͷŵ�TM��ַ��
static int emitLoc = 0;

// ������Ϊֹ�ͷų�����ߵ�TM��ַ
// Ϊ����emitSkip��emitBackup��emitRestore����ʹ��
static int highEmitLoc = 0;

// �ڴ����ļ��д�ӡcָ����ע����
void emitComment(char* c)
{
	if (TraceCode)
		fprintf(code, "* %s\n", c);
}

// �ͷ�һ�����Ĵ������TMָ��
//	op = ��������
//	r = Ŀ��Ĵ���
//	s = ��һԴ�Ĵ���
//	t = �ڶ�Դ�Ĵ���
//	c = ��TraceCode = trueʱҪ��ӡ��ע��
void emitRO(char* op, int r, int s, int t, char* c)
{
	fprintf(code, "%3d:  %5s  %d,%d,%d ", emitLoc++, op, r, s, t);
	if (TraceCode)
		fprintf(code, "\t%s", c);
	fprintf(code, "\n");
	if (highEmitLoc < emitLoc)
		highEmitLoc = emitLoc;
}

// �ͷ�һ���Ĵ������ڴ��TMָ��
//	op = ��������
//	r = Ŀ��Ĵ���
//	d = ƫ����
//	s = ���Ĵ���
//	c = ��TraceCode = trueʱҪ��ӡ��ע��
void emitRM(char* op, int r, int d, int s, char* c)
{
	fprintf(code, "%3d:  %5s  %d,%d(%d) ", emitLoc++, op, r, d, s);
	if (TraceCode)
		fprintf(code, "\t%s", c);
	fprintf(code, "\n");
	if (highEmitLoc < emitLoc)
		highEmitLoc = emitLoc;
}

// ����howManyָ���Ĵ����ַ�����Ա��Ժ���backpatch��
// �������ص�ǰ����ĵ�ַ
int emitSkip(int howMany)
{
	int i = emitLoc;
	emitLoc += howMany;
	if (highEmitLoc < emitLoc)
		highEmitLoc = emitLoc;
	return i;
}

// ����locָ������ǰ�����ĵ�ַ
void emitBackup(int loc)
{
	if (loc > highEmitLoc)
		emitComment("Bug in emitBackup");
	emitLoc = loc;
}

// �ָ���ǰ�����ַ����ߵ���ǰδ���ͷŵĵ�ַ
void emitRestore(void)
{
	emitLoc = highEmitLoc;
}

// ���ͷ�һ���Ĵ������ڴ��TMָ��ʱ��ת��һ���������õ�һ����pcΪ��ַ���������
void emitRM_Abs(char* op, int r, int a, char* c)
{
	fprintf(code, "%3d:  %5s  %d,%d(%d) ", emitLoc, op, r, a - (emitLoc + 1), pc);
	++emitLoc;
	if (TraceCode)
		fprintf(code, "\t%s", c);
	fprintf(code, "\n");
	if (highEmitLoc < emitLoc)
		highEmitLoc = emitLoc;
}