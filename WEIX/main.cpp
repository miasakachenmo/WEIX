#pragma  once 
#include "Users.h"
#include<conio.h>
#include <stdio.h>
#include<functional>
#include<map>
#include "Func.h"
using namespace std;
//全局/静态变量区
int DateZYS::MonthDay[12] = { 31,27,31,30,31,30,31,31,30,31,30,31 };
int DateZYS::LeapMonthDay[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
bool CanBack;//菜单判断是否返回的逻辑
string BaseUserZYS::LastGlobalid = "888";
string QQUserZYS::LastQQid = "888";
string LastRECORDid ="1";
string WeChatUserZYS::LastWeChatid = "999";
map<int, map<string, BaseUserZYS*> > UserList;
map<string, BaseUserZYS*> QQUserList;
map<string, BaseUserZYS*> WeChatUserList;
BaseUserZYS *CurrentUser;

vector<string> Products;//产品号到产品名的映射




//TODO 菜单们 优化使用逻辑 比如注册后显示全部信息
//TODO 登陆菜单
//TODO 修正日期类和生日类的混淆问题

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
		Option = GetOption(1, 2);
		CanBack = false;
		Options[Option - 1]();
		if (CanBack == true)
		{
			CanBack = false;
			return 0;
		}
	}

	system("pause");
	return 0;
}
