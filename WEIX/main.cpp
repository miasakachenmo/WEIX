#pragma  once 
#include "Users.h"
#include<conio.h>
#include <stdio.h>
#include<map>
#include "Func.h"
using namespace std;
//全局/静态变量区
int Date::MonthDay[12] = { 31,27,31,30,31,30,31,31,30,31,30,31 };
int Date::LeapMonthDay[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
string BaseUser::LastGlobalid = "888";
string QQUser::LastQQid = "888";
string LastRECORDid ="1";
string WeChatUser::LastWeChatid = "999";
map<int, map<string, BaseUser*> > UserList;

map<string, BaseUser*> QQUserList;
map<string, BaseUser*> WeChatUserList;
//TODO 初始化机制(生成Globalid Lastxxxid)
int main()
{
	UserList.insert(pair<int, map<string, BaseUser*>>(1, QQUserList));
	UserList.insert(pair<int, map<string, BaseUser*>>(2, WeChatUserList));
	string SqlString = "SELECT * FROM USERS";
	int a=Exe(SqlString);
	Exe(SqlString, CreatCallBack);
	BaseUser *b =UserList[1]["888"];
	UserList[1]["888"]->CreatFriendRelationship("889");
//	system("cls");
	system("pause");
	return 0;
}
