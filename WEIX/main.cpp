#pragma  once 
#include "Users.h"
#include<conio.h>
#include <stdio.h>
#include<functional>
#include<map>
#include "Func.h"

using namespace std;
//ȫ��/��̬������
bool CanBack;//�˵��ж��Ƿ񷵻ص��߼�
#pragma region ��̬������
int DateZYS::MonthDay[12] = { 31,27,31,30,31,30,31,31,30,31,30,31 };
int DateZYS::LeapMonthDay[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
string GlobalDataZYS::LastGlobalid = "888";
string GlobalDataZYS::LastQQid = "888";
string GlobalDataZYS::LastRECORDid = "1";
string GlobalDataZYS::LastWeChatid = "999";
map<int, map<string, BaseUserZYS*> > GlobalDataZYS::UserList;
map<string, BaseUserZYS*> GlobalDataZYS::QQUserList;
map<string, BaseUserZYS*> GlobalDataZYS::WeChatUserList;
vector<string> GlobalDataZYS::Products;//��Ʒ�ŵ���Ʒ����ӳ��
BaseUserZYS *CurrentUser;

#pragma endregion


//TODO �����PYʵ��
//TODO ���б���������
//TODO �˵��� �Ż�ʹ���߼� ����ע�����ʾȫ����Ϣ
//TODO ��½�˵�
//TODO GROUP��ͺ�����ĺϲ�?
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