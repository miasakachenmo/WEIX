#pragma  once 
#include "Users.h"
#include<conio.h>
#include <stdio.h>
#include<functional>
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
BaseUser *CurrentUser;
bool CanBack;
vector<string> Products;
//TODO �˵��� �Ż�ʹ���߼� ����ע�����ʾȫ����Ϣ

int main()
{
	
	init();
	system("cls");	
	while (true)
	{
		vector <function<void()>> Options;
		Options.push_back(CreatUserView);
		printf("1.�û�����\n2.�û���¼\n");
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
