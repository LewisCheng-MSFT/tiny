// *������ѧϰĿ�ģ��ش�������
// �ļ�: scan.h
// ����: ����
// ����: 2010/3/20
// ����: TINY��������ɨ�����ӿ�

#ifndef _SCAN_H_
#define _SCAN_H_

// �Ǻŵ���󳤶�
#define MAXTOKENLEN 40

// �洢ÿ���ǺŵĴ��أ�lexeme�����ؼ��üǺŵ��ַ����ɣ�
extern char tokenString[MAXTOKENLEN + 1];

// ����Դ�ļ��е���һ���Ǻ�
TokenType getToken(void);

#endif