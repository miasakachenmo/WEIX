#pragma  once 
#include "Users.h"
#include<conio.h>
#include <stdio.h>
#include<map>
#include "Func.h"
using namespace std;
//ȫ��/��̬������
int Date::MonthDay[12] = { 31,27,31,30,31,30,31,31,30,31,30,31 };
int Date::LeapMonthDay[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
string BaseUser::LastGlobalid = "888";
string QQUser::LastQQid = "888";
string LastRECORDid ="1";
string WeChatUser::LastWeChatid = "999";
map<int, map<string, BaseUser*> > UserList;

map<string, BaseUser*> QQUserList;
map<string, BaseUser*> WeChatUserList;
//TODO ��ʼ������(����Globalid Lastxxxid)
int main()
{
	init();
//	system("cls");
	
	system("pause");
	return 0;
}
