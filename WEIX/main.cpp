#pragma  once 
#include "Users.h"
#include<conio.h>
#include <stdio.h>
#include<functional>
#include<map>
#include "Func.h"
using namespace std;
//ȫ��/��̬������
int DateZYS::MonthDay[12] = { 31,27,31,30,31,30,31,31,30,31,30,31 };
int DateZYS::LeapMonthDay[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
bool CanBack;//�˵��ж��Ƿ񷵻ص��߼�
string BaseUserZYS::LastGlobalid = "888";
string QQUserZYS::LastQQid = "888";
string LastRECORDid ="1";
string WeChatUserZYS::LastWeChatid = "999";
map<int, map<string, BaseUserZYS*> > UserList;
map<string, BaseUserZYS*> QQUserList;
map<string, BaseUserZYS*> WeChatUserList;
BaseUserZYS *CurrentUser;

vector<string> Products;//��Ʒ�ŵ���Ʒ����ӳ��




//TODO �˵��� �Ż�ʹ���߼� ����ע�����ʾȫ����Ϣ
//TODO ��½�˵�
//TODO �����������������Ļ�������

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
