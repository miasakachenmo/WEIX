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
string GlobalDataZYS::LastGroupid = "1234567";
map<int, map<string, BaseUserZYS*> > GlobalDataZYS::UserList;
map<int, map<string, BaseGroup*>> GlobalDataZYS::Groups;//群们
string GlobalDataZYS::CurrentUser = "";
map<string, BaseUserZYS*> GlobalDataZYS::QQUserList;
map<string, BaseUserZYS*> GlobalDataZYS::WeChatUserList;
string GlobalDataZYS::Permissions[4][5] = { {"", "群主","管理员","群员","禁言中" } ,{ "", "校长","老师","学生","禁言中" }
,{ "", "家长","孩子","群员","禁言中" } ,{ "", "群主","管理员","群员","禁言中" } };
vector<string> GlobalDataZYS::Products;//产品号到产品名的映射
map<string, function<BaseGroup*(string *argv)>> GlobalDataZYS::GroupFactory;
map<int, function<int(BaseGroup* Group)>> GlobalDataZYS::GroupDecorater;
vector<string> GlobalDataZYS::GroupTypeNames = {"普通群","师生群","亲子群",""};
map<int, function<int(BaseGroup* Group)>> GlobalDataZYS::GroupUndecorater;
#pragma endregion

int main()
{
	init();
	system("cls");
	vector <function<int()>> Options;
	Options.push_back(CreatUserView);
	string b;
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