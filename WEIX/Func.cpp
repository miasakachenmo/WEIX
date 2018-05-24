#pragma once
#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<map>
#include "sqlite3.h"
#include "Users.h"
using namespace std;
extern vector<BaseUser*> UserList;
//Ĭ��SQL�ص�����,������ͨ��ѯ,����
int callback(void *NotUsed, int argc, char **argv, char **azColName) {
	int i;
	for (i = 0; i<argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}
//����ʱʹ�õĻص�����,������ʼ��һ���û���
int CreatCallBack(void *NotUsed, int argc, char **argv, char **azColName) {
	int i;
	string a = argv[5];
	switch (argv[5][0])
	{
	case '1':
	{
		UserList.push_back((BaseUser*)(new QQUser(argv)));
	}
	case '2':
	{
		UserList.push_back((BaseUser*)(new WeChatUser(argv)));
	}
	default:
		break;
	}
	for (i = 0; i<argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}
//��һ�������ַ����Լ�1
void String_Add(string *a)
{//while����ͨ��
	int len = (*a).length();
	for (int i = len - 1; i >= 0; i--)
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
sqlite3* OpenDb()
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
int Exe(string SqlStr,int (*callbackfunc)(void *, int , char **, char **))
{
	sqlite3 *db = OpenDb();
	char *zErrMsg = 0;
	int rc = 0;
	do
	{
		rc = sqlite3_exec(db, SqlStr.c_str(), callbackfunc, 0, &zErrMsg);
		if (rc != SQLITE_OK)//�������ݿ��ʧ�ܵ����
		{
			cout << "���ݿ����ʧ��!�����������" << endl;
			printf("SQL error: %s\n", zErrMsg);
			system("pause");
		}
	} while (rc != SQLITE_OK);
	sqlite3_close(db);
	return 0;
}
