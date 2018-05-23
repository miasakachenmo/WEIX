#pragma once
#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<map>
#include "sqlite3.h"
using namespace std;
//SQL�ص�����
static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
	//sqlite���õĻص����� ���ڴ�ӡ����״̬
	int i;
	for (i = 0; i<argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}
//��һ�������ַ����Լ�1
static void String_Add(string *a)
{//while����ͨ��
	int len = (*a).length();
	for (int i = len-1; i >= 0; i--)
	{
		if ((*a)[i] == '9')
		{
			if (i == 0)
			{
				(*a)[0] = '0';
				(*a) = '1' + (*a);
				return;
			}
			else
			{
				(*a)[i] = '0';
				continue;
			}
		}
		else {
			(*a)[i]++;
			return;
		}
	}
}
//��Ĭ�����ݿ�
static sqlite3* OpenDb()
{
	sqlite3 *db;
	int rc;
	do
	{
		rc = sqlite3_open("USERS.db", &db);
		if (rc != SQLITE_OK)//�������ݿ��ʧ�ܵ����
		{
			cout << "���ݿ��ʧ��!�����������" << endl;
			system("pause");
		}
	} while (rc != SQLITE_OK);
	return db;
}
//ִ��ָ��SQLITE���
static int Exe(string SqlStr)
{
	sqlite3 *db = OpenDb();
	char *zErrMsg = 0;
	int rc = 0;
	do
	{
		rc = sqlite3_exec(db, SqlStr.c_str(), callback, 0, &zErrMsg);
		if (rc != SQLITE_OK)//�������ݿ��ʧ�ܵ����
		{
			cout << "���ݿ����ʧ��!�����������" << endl;
			system("pause");
		}
	} while (rc != SQLITE_OK);
	return 0;
}