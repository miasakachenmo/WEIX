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
string GlobalDataZYS::LastGroupid = "1234567";
map<int, map<string, BaseUserZYS*> > GlobalDataZYS::UserList;
map<int, map<string, BaseGroup*>> GlobalDataZYS::Groups;//Ⱥ��
string GlobalDataZYS::CurrentUser = "";
map<string, BaseUserZYS*> GlobalDataZYS::QQUserList;
map<string, BaseUserZYS*> GlobalDataZYS::WeChatUserList;
string GlobalDataZYS::Permissions[4][5] = { {"", "Ⱥ��","����Ա","ȺԱ","������" } ,{ "", "У��","��ʦ","ѧ��","������" }
,{ "", "�ҳ�","����","ȺԱ","������" } ,{ "", "Ⱥ��","����Ա","ȺԱ","������" } };
vector<string> GlobalDataZYS::Products;//��Ʒ�ŵ���Ʒ����ӳ��
map<string, function<BaseGroup*(string *argv)>> GlobalDataZYS::GroupFactory;
map<int, function<int(BaseGroup* Group)>> GlobalDataZYS::GroupDecorater;
vector<string> GlobalDataZYS::GroupTypeNames = {"��ͨȺ","ʦ��Ⱥ","����Ⱥ",""};
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