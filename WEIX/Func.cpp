#pragma once
#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<conio.h>
#include<map>
#include "sqlite3.h"
#include "Users.h"
using namespace std;
extern map<int , map<string, BaseUser*> > UserList;//����˵��:<ProductCode<Globalid,ָ��>>
bool Called = false;
//Ĭ��SQL�ص�����,������ͨ��ѯ,����
int callback(void *NotUsed, int argc, char **argv, char **azColName) {
	Called = true;
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
		UserList[1].insert(pair<string, BaseUser*>(argv[1], (BaseUser*)((new QQUser(argv)))));
	}
	case '2':
	{
		UserList[2].insert(pair<string, BaseUser*>(argv[1], (BaseUser*)(new WeChatUser(argv))));
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
		Called = false;
		rc = sqlite3_exec(db, SqlStr.c_str(), callbackfunc, 0, &zErrMsg);
		if (rc != SQLITE_OK)//�������ݿ��ʧ�ܵ����
		{
			cout << "���ݿ����ʧ��!�����������" << endl;
			printf("SQL error: %s\n", zErrMsg);
			system("pause");
		}
	} while (rc != SQLITE_OK);
	sqlite3_close(db);
	if (Called == false)
		return 0;
	else
		return 1;
}
//�����û��˵�
int CreatUserView()
{
	while (1)
	{
		char Option;
		printf("1.����һ��QQ�û�\n2.����һ��΢���û�\n3.�˳�\n");
		Option = _getch();
		system("cls");
		switch (Option)
		{
		case '1':
		{
			QQUser a;
			break;
		}
		case '2':
		{
			WeChatUser b;
			break;
		}
		case '3':
			goto there;
		default:
			break;
		}
		system("cls");
	}
there:
	return 0;
}