// *������ѧϰĿ�ģ��ش�������
// �ļ�: scan.h
// ����: ����
// ����: 2010/3/20
// ����: TINY��������ɨ����ʵ��

#include "globals.h"
#include "util.h"
#include "scan.h"

// ɨ����DFA��״̬
typedef enum {
	START,
	INASSIGN,
	INCOMMENT,
	INNUM,
	INID,
	DONE
} StateType;

// ��ʶ�������ֵĴ���
char tokenString[MAXTOKENLEN + 1];

// Դ���������뻺��ĳ���
#define BUFLEN 256

// �Ƿ��ѵ��ļ�ĩβ
static bool endfile;
// ���浱ǰ��
static char lineBuf[BUFLEN];
// ��lineBuf�еĵ�ǰλ��
static int linepos = 0;
// ��ǰ�������ַ����ĳ���
static int bufsize = 0;

// ��lineBuf��ȡ����һ���ַ�
// ����Ѷ��꣬������µ�һ��
static char getNextChar(void)
{
	if (linepos >= bufsize)
	{
		lineno++;
		if (fgets(lineBuf, BUFLEN - 1, source))
		{
			if (EchoSource)
				fprintf(listing, "%4d: %s", lineno, lineBuf);
			bufsize = strlen(lineBuf);
			linepos = 0;
			return lineBuf[linepos++];
		}
		else
			return EOF;
	}
	else
		return lineBuf[linepos++];
}

// ��lineBuf�л���һ���ַ�
static void ungetNextChar(void)
{
	linepos--;
}

// �����ֵĲ��ұ�
static struct {
	char* str;
	TokenType tok;
} reservedWords[MAXRESERVED] = {
	{"if", IF},
	{"then", THEN},
	{"else", ELSE},
	{"end", END},
	{"repeat", REPEAT},
	{"until", UNTIL},
	{"read", READ},
	{"write", WRITE}
};

// ���Բ����ϱ��ж�һ����ʶ���Ƿ��Ǳ�����
static TokenType reservedLookup(char* s)
{
	for (int i = 0; i < MAXRESERVED; i++)
		if (!strcmp(s, reservedWords[i].str))
			return reservedWords[i].tok;
	return ID;
}

//-----------------------------ɨ��������Ҫ����-------------------------------
// ����Դ�ļ��е���һ���Ǻ�
TokenType getToken(void)
{
	// ����tokenStringʱ���±�
	int tokenStringIndex = 0;
	// ���浱ǰ����Ա�����
	TokenType currentToken;
	// ��ǰ��״̬�����Ǵ�START��ʼ
	StateType state = START;
	// ��־��ָʾ�Ƿ񱣴浽tokenString
	bool save;
	while (state != DONE)
	{
		char c = getNextChar();
		save = true;
		switch (state)
		{
		case START:
			if (isdigit(c))
				state = INNUM;
			else if (isalpha(c))
				state = INID;
			else if (c == ':')
				state = INASSIGN;
			else if (c == ' ' || c == '\t' || c == '\n')
				save = false;
			else if (c == '{')
			{
				save = false;
				state = INCOMMENT;
			}
			else
			{
				state = DONE;
				switch (c)
				{
				case EOF:
					save = false;
					currentToken = ENDFILE;
					break;
				case '=':
					currentToken = EQ;
					break;
				case '<':
					currentToken = LT;
					break;
				case '+':
					currentToken = PLUS;
					break;
				case '-':
					currentToken = MINUS;
					break;
				case '*':
					currentToken = TIMES;
					break;
				case '/':
					currentToken = OVER;
					break;
				case '(':
					currentToken = LPAREN;
					break;
				case ')':
					currentToken = RPAREN;
					break;
				case  ';':
					currentToken = SEMI;
					break;
				default:
					currentToken = ERROR;
					break;
				}
			}
			break;
		case INCOMMENT:
			save = false;
			if (c == '}') state = START;
			break;
		case INASSIGN:
			state = DONE;
			if (c == '=')
				currentToken = ASSIGN;
			else
			{
				// �Żص�������
				ungetNextChar();
				save = false;
				currentToken = ERROR;
			}
			break;
		case INNUM:
			if (!isdigit(c))
			{
				// �Ż�������
				ungetNextChar();
				save = false;
				state = DONE;
				currentToken = NUM;
			}
			break;
		case INID:
			if (!isalpha(c))
			{
				// �Ż�������
				ungetNextChar();
				save = false;
				state = DONE;
				currentToken = ID;
			}
			break;
		case DONE:
		default:
			// ���õ�������
			fprintf(listing, "Scanner Bug: state = %d\n", state);
			state = DONE;
			currentToken = ERROR;
			break;
		}
		if (save && tokenStringIndex <= MAXTOKENLEN)
			tokenString[tokenStringIndex++] = c;
		if (state == DONE)
		{
			tokenString[tokenStringIndex] = '\0';
			if (currentToken == ID)
				currentToken = reservedLookup(tokenString);
		}
	}
	if (TraceScan)
	{
		fprintf(listing, "\t%d: ", lineno);
		printToken(currentToken, tokenString);
	}
	return currentToken;
} // getToken����