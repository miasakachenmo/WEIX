#pragma once
#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<conio.h>
#include<map>
#include<wchar.h>
#include<Windows.h>
#include "sqlite3.h"
#include "Users.h"
using namespace std;
bool Called = false;
extern bool CanBack;

#pragma region SQL函数
int callback(void *NotUsed, int argc, char **argv, char **azColName) {
	Called = true;
	int i;
	for (i = 0; i<argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}
//从文件读取时使用的回调函数,用来初始化一个用户类
int CreatCallBack(void *NotUsed, int argc, char **argv, char **azColName) {
	int i;
	string a = argv[5];
	if (GlobalDataZYS::LastGlobalid <= argv[1])//刷新LastGlobalid
	{
		GlobalDataZYS::LastGlobalid = argv[1];
		String_Add(&GlobalDataZYS::LastGlobalid);
	}
	switch (argv[5][0])
	{
	case '1':
	{
		GlobalDataZYS::UserList[1].insert(pair<string, BaseUserZYS*>(argv[1], (BaseUserZYS*)((new QQUserZYS(argv)))));
		if (GlobalDataZYS::LastQQid <= argv[7])//刷新id
		{
			GlobalDataZYS::LastQQid = argv[7];
			String_Add(&GlobalDataZYS::LastQQid);
		}
	}
	case '2':
	{
		GlobalDataZYS::UserList[2].insert(pair<string, BaseUserZYS*>(argv[1], (BaseUserZYS*)(new WeChatUserZYS(argv))));
		if (GlobalDataZYS::LastWeChatid <= argv[7])//刷新id
		{
			GlobalDataZYS::LastWeChatid = argv[7];
			String_Add(&GlobalDataZYS::LastWeChatid);
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
	GlobalDataZYS::UserList[ProductCode][FromGB]->Friends.GetRelationShip(GlobalDataZYS::UserList[ProductCode][FromGB], ToGB);
	printf("产品%d读取%s到%s的好友关系成功\n", ProductCode, FromGB.c_str(), ToGB.c_str());
	return 0;
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
int Exe(string SqlStr, int(*callbackfunc)(void *, int, char **, char **))
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
#pragma endregion

#pragma region 功能函数
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
//初始化函数!!!!
int init()
{
	GlobalDataZYS::Products.push_back("QQ");
	GlobalDataZYS::Products.push_back("微信");
	GlobalDataZYS::UserList.insert(pair<int, map<string, BaseUserZYS*>>(1, GlobalDataZYS::QQUserList));
	GlobalDataZYS::UserList.insert(pair<int, map<string, BaseUserZYS*>>(2, GlobalDataZYS::WeChatUserList));

	//读取所有用户信息
	string SqlString = "SELECT * FROM USERS";
	Exe(SqlString, CreatCallBack);

	//读取好友信息
	SqlString = "SELECT * FROM FRIEND";
	Exe(SqlString, GetFriendCallBack);


	return 0;
}
//得到一个符合范围[MIN,MAX]的选项
int GetOption(int Min, int Max)
{
	char Option;
	do
	{
		Option = _getch();
	} while (Option - '0'<Min || Option - '0' > Max);
	return Option - '0';
}
//输入密码提供的不显示效果
string InputPwd()
{
	string CorrectChars = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz{}[];':\"<>,./?\\!@#$%^&*()-_=+`~";
	string Pwd = "";
	do
	{
		char temp = _getch();
		if (temp == 13)//回车
		{
			if (Pwd == "")
			{
				cout << "不能为空!请重新输入\n";
				continue;
			}
			break;
		}
		if (CorrectChars.find(temp) == -1)
			continue;
		Pwd += temp;
	} while (true);
	return Pwd;
}


//GBK转UTF8(来源:CSDN)
string GBKToUTF8(const char* strGBK)
{
	int len = MultiByteToWideChar(CP_ACP, 0, strGBK, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, strGBK, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	string strTemp = str;
	if (wstr) delete[] wstr;
	if (str) delete[] str;
	return strTemp;
}
//UTF8转GBK(来源:CSDN)
string UTF8ToGBK(const char* strUTF8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, NULL, 0);
	wchar_t* wszGBK = new wchar_t[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, wszGBK, len);
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char* szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
	string strTemp(szGBK);
	if (wszGBK) delete[] wszGBK;
	if (szGBK) delete[] szGBK;
	return strTemp;
}
#pragma endregion

#pragma region 菜单
//登陆菜单
bool LoginView(int ProductCode=0)
{
	system("cls");
	int i = GlobalDataZYS::Products.size()+1;
	int ProductCode;
	string Account = "";
	
	if (ProductCode == 0)
	{
		printf("选择你想登陆的产品\n");
		char Option = ChooseProductView();
		if (Option == i)//选择退出
			return false;
		else//选择到了产品
		{
			ProductCode = Option;
		}
	}
	do
	{
		system("cls");
		printf("输入账号\n");
		cin >> Account;
		//匿名函数 作用是寻找对应id的账号
		auto iterfind = find_if(GlobalDataZYS::UserList[ProductCode].begin(), GlobalDataZYS::UserList[ProductCode].end(), [Account](pair<string, BaseUserZYS*> user) {
			if (user.second->id == Account)
				return true;
		});
		if (iterfind == GlobalDataZYS::UserList[ProductCode].end())
		{
			printf("账号错误!请按1重新输入或者按2退出\n");
			if (GetOption(1, 2) == 2)
				return false;
			continue;
		}
		else
		{
			if (iterfind->second->LoginCheck())
			{
				printf("登陆成功!!!!!");
				return true;
			}
		}
	} while (true);
}
//创建用户菜单
void CreatUserView()
{
	system("cls");
	while (1)
	{
		char Option;
		printf("1.创建一个QQ用户\n2.创建一个微信用户\n3.退出\n");
		Option = GetOption(1, 3);
		system("cls");
		BaseUserZYS *a;

		switch (Option)
		{
		case 1:
		{
			a = new QQUserZYS;
			break;
		}
		case 2:
		{
			a = new WeChatUserZYS;
			break;
		}
		case 3:
			goto there;
		default:
		{
			a = NULL;
			break;
		}
		}
		a->PrintMessage();
		GlobalDataZYS::UserList[a->ProductCode].insert(pair<string, BaseUserZYS*>(a->Global_id, a));
		system("pause");
		system("cls");
	}
there:
	return;
}
//选择产品菜单
int ChooseProductView()
{
	int i;
	for (i = 0; i < GlobalDataZYS::Products.size(); i++)
		printf("%d %s\n", i + 1, GlobalDataZYS::Products[i]);
	printf("%d.  退出\n", i);
	return GetOption(1, i);
}
#pragma endregion


