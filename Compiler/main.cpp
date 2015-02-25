// *������ѧϰĿ�ģ��ش�������
// �ļ�: main.c
// ����: ����
// ����: 2010/3/20
// ����: TINY��������������

#pragma warning(disable:4996)

#include "globals.h"

// ����NO_PARSEΪtrue�Բ������дʷ������ı�����
#define NO_PARSE FALSE

// ����NO_ANALYZEΪtrue�Բ��������﷨�����ı�����
#define NO_ANALYZE FALSE

// ����NO_CODEΪtrue������������������Ŀ�����
#define NO_CODE FALSE

#include "util.h"
#if NO_PARSE
#include "scan.h"
#else
#include "parse.h"
#if !NO_ANALYZE
#include "analyze.h"
#if !NO_CODE
#include "cgen.h"
#endif
#endif
#endif

// ����ȫ�ֱ���
int lineno = 0;
FILE* source;
FILE* listing;
FILE* code;

// ��������ø����ٱ�־
bool EchoSource = true;
bool TraceScan = true;
bool TraceParse = true;
bool TraceAnalyze = true;
bool TraceCode = true;

bool Error = false;

int main(int argc, char* argv[])
{
	TreeNode* syntaxTree;
	// Դ�ļ���
	char pgm[20];
	if (argc != 2)
	{
		fprintf(stderr, "usage: %s <filename>\n", argv[0]);
		exit(1);
	}
	strcpy(pgm, argv[1]);
	if (strchr(pgm, '.') == NULL)
		strcat(pgm, ".tny");
	source = fopen(pgm, "r");
	if (source == NULL)
	{
		fprintf(stderr, "File %s not found\n", pgm);
		exit(1);
	}
	// ���б��ļ���Ϊ��׼���
	listing =  stdout;
	fprintf(listing, "\nTINY COMPILATION: %s\n", pgm);
#if NO_PARSE
	while (getToken() != ENDFILE);
#else
	syntaxTree = parse();
	if (TraceParse)
	{
		fprintf(listing, "\nSyntax tree:\n");
		printTree(syntaxTree);
	}
#if !NO_ANALYZE
	if (!Error)
	{
		fprintf(listing, "\nBuilding Symbol Table...\n");
		buildSymtab(syntaxTree);
		fprintf(listing, "\nChecking Types...\n");
		typeCheck(syntaxTree);
		fprintf(listing, "\nType Checking Finished\n");
	}
#if !NO_CODE
	if (!Error)
	{
		char* codefile;
		int fnlen = strcspn(pgm, ".");
		codefile = (char*)calloc(fnlen + 4, sizeof(char));
		strncpy(codefile, pgm, fnlen);
		strcat(codefile, ".tm");
		code = fopen(codefile, "w");
		if (code == NULL)
		{
			printf("Unable to open %s\n", codefile);
			exit(1);
		}
		codeGen(syntaxTree, codefile);
		fclose(code);
	}
#endif
#endif
#endif
	return 0;
}