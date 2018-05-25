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
extern map<string, BaseUser*> QQUserList;
extern map<string, BaseUser*> WeChatUserList;
extern bool CanBack;
//��ʼ������!!!!
int init()
{
	UserList.insert(pair<int, map<string, BaseUser*>>(1, QQUserList));
	UserList.insert(pair<int, map<string, BaseUser*>>(2, WeChatUserList));

	//��ȡ�����û���Ϣ
	string SqlString = "SELECT * FROM USERS";
	Exe(SqlString, CreatCallBack);

	//��ȡ������Ϣ
	SqlString = "SELECT * FROM FRIEND";
	Exe(SqlString, GetFriendCallBack);


	return 0;
}
//SQL�ص�����------------------------------------------------------------------------------------------
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
	if (BaseUser::LastGlobalid <= argv[1])//ˢ��LastGlobalid
	{
		BaseUser::LastGlobalid = argv[1];
		String_Add(&BaseUser::LastGlobalid);
	}
		switch (argv[5][0])
	{
	case '1':
	{
		UserList[1].insert(pair<string, BaseUser*>(argv[1], (BaseUser*)((new QQUser(argv)))));
		if (QQUser::LastQQid <= argv[7])//ˢ��id
		{
			QQUser::LastQQid = argv[7];
			String_Add(&QQUser::LastQQid);
		}
	}
	case '2':
	{
		UserList[2].insert(pair<string, BaseUser*>(argv[1], (BaseUser*)(new WeChatUser(argv))));
		if (WeChatUser::LastWeChatid <= argv[7])//ˢ��id
		{
			WeChatUser::LastWeChatid = argv[7];
			String_Add(&WeChatUser::LastWeChatid);
		}
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
//��ȡ����ʹ�õ�SQL�ص�
int GetFriendCallBack(void *NotUsed, int argc, char **argv, char **azColName)
{
	string FromGB = argv[1];
	string ToGB = argv[2];
	int ProductCode = atoi(argv[3]);
	UserList[ProductCode][FromGB]->CreatFriendRelationship(ToGB);
	printf("��Ʒ%d��ȡ%s��%s�ĺ��ѹ�ϵ�ɹ�\n", ProductCode, FromGB.c_str(), ToGB.c_str());
	return 0;
}


//���ܺ���----------------------------------------------------------------------------------------------
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
void CreatUserView()
{
	system("cls");
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
	return;
}