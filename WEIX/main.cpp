#pragma  once 
#include "Users.h"
#include<conio.h>
#include <stdio.h>
#include<functional>
#include<map>
#include "Func.h"

using namespace std;
//全局/静态变量区
bool CanBack;//菜单判断是否返回的逻辑
#pragma region 静态变量区
int DateZYS::MonthDay[12] = { 31,27,31,30,31,30,31,31,30,31,30,31 };
int DateZYS::LeapMonthDay[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
string GlobalDataZYS::LastGlobalid = "888";
string GlobalDataZYS::LastQQid = "888";
string GlobalDataZYS::LastRECORDid = "1";
string GlobalDataZYS::LastWeChatid = "999";
map<int, map<string, BaseUserZYS*> > GlobalDataZYS::UserList;
map<string, BaseUserZYS*> GlobalDataZYS::QQUserList;
map<string, BaseUserZYS*> GlobalDataZYS::WeChatUserList;
vector<string> GlobalDataZYS::Products;//产品号到产品名的映射
BaseUserZYS *CurrentUser;

#pragma endregion


//TODO 密码的PY实现
//TODO 把列表类抽象出来
//TODO 菜单们 优化使用逻辑 比如注册后显示全部信息
//TODO 登陆菜单
//TODO GROUP类和好友类的合并?
int main()
{
	init();
	system("cls");
	vector <function<int()>> Options;
	Options.push_back(CreatUserView);
	Options.push_back([]() {
		LoginView();
		return 0; });
	while (true)
	{
		system("cls");
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