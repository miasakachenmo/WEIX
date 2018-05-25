#pragma  once 
#include "Users.h"
#include<conio.h>
#include <stdio.h>
#include<functional>
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
BaseUser *CurrentUser;
bool CanBack;
int main()
{
	
	init();
	system("cls");	
	while (true)
	{
		vector <function<void()>> Options;
		Options.push_back(CreatUserView);
		printf("1.用户创建\n2.用户登录\n");
		char Option;
		Option = _getch();
		if (Option <= '2'&&Option >= '1')
		{
			CanBack = false;
			Options[Option - '0'-1]();
			if (CanBack = true)
			{
				CanBack = false;
				return 0;
			}
		}
	}

	system("pause");
	return 0;
}
