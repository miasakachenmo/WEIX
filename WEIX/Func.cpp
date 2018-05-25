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
extern map<int , map<string, BaseUser*> > UserList;//参数说明:<ProductCode<Globalid,指针>>
bool Called = false;
extern map<string, BaseUser*> QQUserList;
extern map<string, BaseUser*> WeChatUserList;
extern bool CanBack;
//初始化函数!!!!
int init()
{
	UserList.insert(pair<int, map<string, BaseUser*>>(1, QQUserList));
	UserList.insert(pair<int, map<string, BaseUser*>>(2, WeChatUserList));

	//读取所有用户信息
	string SqlString = "SELECT * FROM USERS";
	Exe(SqlString, CreatCallBack);

	//读取好友信息
	SqlString = "SELECT * FROM FRIEND";
	Exe(SqlString, GetFriendCallBack);


	return 0;
}
//SQL回调函数------------------------------------------------------------------------------------------
//默认SQL回调函数,用于普通查询,插入
int callback(void *NotUsed, int argc, char **argv, char **azColName) {
	Called = true;
	int i;
	for (i = 0; i<argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}
//创建时使用的回调函数,用来初始化一个用户类
int CreatCallBack(void *NotUsed, int argc, char **argv, char **azColName) {
	int i;
	string a = argv[5];
	if (BaseUser::LastGlobalid <= argv[1])//刷新LastGlobalid
	{
		BaseUser::LastGlobalid = argv[1];
		String_Add(&BaseUser::LastGlobalid);
	}
		switch (argv[5][0])
	{
	case '1':
	{
		UserList[1].insert(pair<string, BaseUser*>(argv[1], (BaseUser*)((new QQUser(argv)))));
		if (QQUser::LastQQid <= argv[7])//刷新id
		{
			QQUser::LastQQid = argv[7];
			String_Add(&QQUser::LastQQid);
		}
	}
	case '2':
	{
		UserList[2].insert(pair<string, BaseUser*>(argv[1], (BaseUser*)(new WeChatUser(argv))));
		if (WeChatUser::LastWeChatid <= argv[7])//刷新id
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
//读取好友使用的SQL回调
int GetFriendCallBack(void *NotUsed, int argc, char **argv, char **azColName)
{
	string FromGB = argv[1];
	string ToGB = argv[2];
	int ProductCode = atoi(argv[3]);
	UserList[ProductCode][FromGB]->CreatFriendRelationship(ToGB);
	printf("产品%d读取%s到%s的好友关系成功\n", ProductCode, FromGB.c_str(), ToGB.c_str());
	return 0;
}


//功能函数----------------------------------------------------------------------------------------------
//让一个数字字符串自加1
void String_Add(string *a)
{//while测试通过
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
//打开默认数据库
sqlite3* OpenDb()
{
	sqlite3 *db;
	int rc;
	do
	{
		rc = sqlite3_open("USERS.db", &db);
		if (rc != SQLITE_OK)//处理数据库打开失败的情况
		{
			cout << "数据库打开失败!按任意键重试" << endl;
			system("pause");
		}
	} while (rc != SQLITE_OK);
	return db;
}
//执行指定SQLITE语句
int Exe(string SqlStr,int (*callbackfunc)(void *, int , char **, char **))
{
	sqlite3 *db = OpenDb();
	char *zErrMsg = 0;
	int rc = 0;
	do
	{
		Called = false;
		rc = sqlite3_exec(db, SqlStr.c_str(), callbackfunc, 0, &zErrMsg);
		if (rc != SQLITE_OK)//处理数据库打开失败的情况
		{
			cout << "数据库操作失败!按任意键重试" << endl;
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
//创建用户菜单
void CreatUserView()
{
	system("cls");
	while (1)
	{
		char Option;
		printf("1.创建一个QQ用户\n2.创建一个微信用户\n3.退出\n");
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