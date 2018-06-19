#pragma once
#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<conio.h>
#include<map>
#include<wchar.h>
#include<Windows.h>
#include <cstdlib>
#include "sqlite3.h"
#include "Users.h"
using namespace std;
bool Called = false;
extern bool CanBack;
int SqlCallCount = 0;
class BaseUserZYS;
#pragma region SQL����
int callback(void *NotUsed, int argc, char **argv, char **azColName) {
	Called = true;
	int i;
	for (i = 0; i<argc; i++) {
		if(argv[i]==NULL)
			continue;
		string temp = argv[i];
		temp = UTF8ToGBK(argv[i]);
		cout << azColName[i] << ":" <<temp << endl;
	}
	printf("\n");
	return 0;
}
//���ļ���ȡʱʹ�õĻص�����,������ʼ��һ���û���
int CreatCallBack(void *NotUsed, int argc, char **argvs, char **azColName) {
	int i;
	string argv[10];
	const char *fuck[10];
	for (int i = 0; i <=7 ; i++)
	{
		argv[i] = argvs[i];
	}
	for (int i = 0; i <= 7; i++)
	{
		argv[i] = UTF8ToGBK(argv[i].c_str());
	}

	string a = argv[5];//productcode
	if (GlobalDataZYS::LastRECORDid <= argv[0])
	{
		GlobalDataZYS::LastRECORDid = argv[0];
		String_Add(&GlobalDataZYS::LastRECORDid);
	}
	if (GlobalDataZYS::LastGlobalid <= argv[1])//ˢ��LastGlobalid
	{
		GlobalDataZYS::LastGlobalid = argv[1];
		String_Add(&GlobalDataZYS::LastGlobalid);
	}
	//TODO ���ù���ģʽʵ�ָ����� switch�ó�
	switch (argv[5][0])
	{
	case '1':
	{
		GlobalDataZYS::UserList[1].insert(pair<string, BaseUserZYS*>(argv[1], (BaseUserZYS*)((new QQUserZYS(argv)))));
		break;
	}
	case '2':
	{
		GlobalDataZYS::UserList[2].insert(pair<string, BaseUserZYS*>(argv[1], (BaseUserZYS*)(new WeChatUserZYS(argv))));
		break;
	}
	default:
		break;
	}
	for (i = 0; i<=7; i++) {
		cout << azColName[i] << ":" << argv[i]<<endl;
	}
	printf("\n");
	return 0;
}
//��ȡ����ʹ�õ�SQL�ص�
int GetFriendCallBack(void *NotUsed, int argc, char **argvs, char **azColName)
{
	string argv[10];
	const char *fuck[10];
	for (int i = 0; i <= 3; i++)
	{
		argv[i] = argvs[i];
		argv[i] = UTF8ToGBK(argv[i].c_str());
	}

	string FromGB = argv[1];
	string ToGB = argv[2];
	int ProductCode = atoi(argv[3].c_str());
	GlobalDataZYS::UserList[ProductCode][FromGB]->Friends.GetRelationShip(GlobalDataZYS::UserList[ProductCode][FromGB], ToGB);
	printf("��Ʒ%d��ȡ%s��%s�ĺ��ѹ�ϵ�ɹ�\n", ProductCode, FromGB.c_str(), ToGB.c_str());
	return 0;
}
int GetGroupCallBack(void * NotUsed, int argc, char ** argvs, char ** azColName)
{
	string argv[10];
	const char *fuck[10];
	for (int i = 0; i <= 6; i++)
	{
		argv[i] =argvs[i];
		argv[i]= UTF8ToGBK(argv[i].c_str());
	}
	string Groupid = argv[1];
	int ProductCode = atoi(argv[4].c_str());
	//���Ⱥ�Ѿ����� ��ɺ��Ѽ���
	if (GlobalDataZYS::Groups[ProductCode].find(Groupid)!= GlobalDataZYS::Groups[ProductCode].end())
	{
		GlobalDataZYS::Groups[ProductCode][Groupid]->List.insert(pair<string, string>(argv[2],argv[3]));
		GlobalDataZYS::UserList[ProductCode][argv[2]]->Groups.List.insert(pair<string, string>(argv[1], argv[3]));
		//->CreatRelationShip(GlobalDataZYS::UserList[ProductCode][argv[2]], argv[3]);
		return 0;
	}
	//�Ҳ��� ��������
	if (GlobalDataZYS::LastGroupid <= Groupid)
	{
		String_Add(&GlobalDataZYS::LastGroupid);
	}
	string TypeCode = argv[4];
	GlobalDataZYS::Groups[ProductCode].insert(pair<string, BaseGroup*>(argv[1],  GlobalDataZYS::GroupFactory[TypeCode+"0"](argv) ));//���ù���ģʽ���෴�䴴��һ��Ⱥ
	GlobalDataZYS::Groups[ProductCode][Groupid]->List.insert(pair<string, string>(argv[2], argv[3]));
	GlobalDataZYS::UserList[ProductCode][argv[2]]->Groups.List.insert(pair<string, string>(argv[1], argv[3]));
	return 0;
}
//չʾ�Ļص�����
int ShowCallBack(void * NotUsed, int argc, char ** argv, char ** azColName)
{
	SqlCallCount++;
	cout << SqlCallCount << " .";
	int i;
	for (i = 0; i < argc; i++) {
		string temp = argv[i];
		temp = UTF8ToGBK(argv[i]);
		cout <<temp << endl;
	}
	printf("\n");
	system("pause");
	return 0;
}
//��Ĭ�����ݿ�
sqlite3* OpenDb()
{
	sqlite3 *db;
	int rc;
	do
	{
		rc = sqlite3_open("USERS.db", &db);
		if (rc != SQLITE_OK)//�������ݿ��ʧ�ܵ����
		{
			cout << "���ݿ��ʧ��!�����������" << endl;
			system("pause");
		}
	} while (rc != SQLITE_OK);
	return db;
}
//ִ��ָ��SQLITE���
int Exe(string SqlStr, int(*callbackfunc)(void *, int, char **, char **))
{

	//string UtfStr = GBKToUTF8(SqlStr.c_str());//�ĳ�UTF��ʽ ��ֹ��������
	sqlite3 *db = OpenDb();
	char *zErrMsg = 0;
	int rc = 0;
	do
	{
		Called = false;
		rc = sqlite3_exec(db, SqlStr.c_str(), callbackfunc, 0, &zErrMsg);
		if (rc != SQLITE_OK)//�������ݿ��ʧ�ܵ����
		{
			cout << "���ݿ����ʧ��!�����������" << endl;
			printf("SQL error: %s\n", zErrMsg);
			system("pause");
		}
	} while (rc != SQLITE_OK);
	sqlite3_close(db);
	if (Called == false)
		return 0;
	else
		return 1;
}
#pragma endregion

#pragma region ���ܺ���
//��һ�������ַ����Լ�1
void String_Add(string *a)
{//while����ͨ��
	int len = (*a).length();
	for (int i = len - 1; i >= 0; i--)
	{
		if ((*a)[i] == '9')
		{
			if (i == 0)
			{
				(*a)[0] = '0';
				(*a) = '1' + (*a);
				return;
			}
			else
			{
				(*a)[i] = '0';
				continue;
			}
		}
		else {
			(*a)[i]++;
			return;
		}
	}
}
//��ʼ������!!!!
int init()
{
	GlobalDataZYS::Products.push_back("QQ");
	GlobalDataZYS::Products.push_back("΢��");

	GlobalDataZYS::UserList.insert(pair<int, map<string, BaseUserZYS*>>(1, *(new map<string, BaseUserZYS*>)));
	GlobalDataZYS::UserList.insert(pair<int, map<string, BaseUserZYS*>>(2, *(new map<string, BaseUserZYS*>)));
	
	GlobalDataZYS::Groups.insert(pair<int, map<string, BaseGroup*>>(1, *(new map<string, BaseGroup*>)));
	GlobalDataZYS::Groups.insert(pair<int, map<string, BaseGroup*>>(2, *(new map<string, BaseGroup*>)));

	//װ������ĳ�ʼ��
	GlobalDataZYS::GroupDecorater.insert(pair<int, function<int(BaseGroup*)>>(0, [](BaseGroup*) {
		return 0; }));
	GlobalDataZYS::GroupDecorater.insert(pair<int, function<int(BaseGroup*)>>(1,[](BaseGroup* Group) {
		Group->AddFunc("������ҵ",[Group]() {
			Group->PermissionCheck(GlobalDataZYS::CurrentUser, "2");
			string HomeWork;
			cout << "������ҵ����" << endl;
			cin >> HomeWork;
			string Sql = "UPDATE GROUPS SET EXTRA ='" + HomeWork + "' WHERE PRODUCTCODE='" + to_string(Group->ProductCode) + "' AND GROUPID='" + Group->Groupid + "' AND PERMISSIONCODE>2;";
			Exe(Sql);
			return 0; });
		Group->AddFunc("�鿴��ҵ", [Group]() {
			string Sql = "SELECT EXTRA  FROM GROUPS WHERE GROUPID='" + Group->Groupid + "' AND GB='" + GlobalDataZYS::CurrentUser + "' AND PRODUCTCODE='" + to_string(Group->ProductCode) + "';";
			SqlCallCount = 0;
			Exe(Sql,ShowCallBack);
			Sql = "Update GROUPS SET EXTRA=null WHERE  GROUPID='" + Group->Groupid + "' AND GB='" + GlobalDataZYS::CurrentUser + "' AND PRODUCTCODE='" + to_string(Group->ProductCode) + "';";
			Exe(Sql);
			return 0; });
		return 0; }));
	GlobalDataZYS::GroupDecorater.insert(pair<int, function<int(BaseGroup*)>>(2, [](BaseGroup *Group) {
		Group->AddFunc("������ɫ����", []() {return 0; });
		return 0; }));
	GlobalDataZYS::GroupDecorater.insert(pair<int, function<int(BaseGroup*)>>(3, [](BaseGroup*Group) {

		return 0; }));
	//��װ�����ĳ�ʼ��
	GlobalDataZYS::GroupUndecorater.insert(pair<int, function<int(BaseGroup*)>>(0, [](BaseGroup*Group) {
		return 0; }));
	GlobalDataZYS::GroupUndecorater.insert(pair<int, function<int(BaseGroup*)>>(1, [](BaseGroup*Group) {
		Group->Menu.erase(Group->Menu.find("��У��ɫ����"));
		return 0; }));
	GlobalDataZYS::GroupUndecorater.insert(pair<int, function<int(BaseGroup*)>>(2, [](BaseGroup*Group) {
		Group->Menu.erase(Group->Menu.find("������ɫ����"));
		return 0; }));
	GlobalDataZYS::GroupUndecorater.insert(pair<int, function<int(BaseGroup*)>>(3, [](BaseGroup*Group) {

		return 0; }));
	//����ģʽ�ĳ�ʼ��
	GlobalDataZYS::GroupFactory.insert(pair<string, function<BaseGroup*(string *argv)>>("10", [](string *argv) {
		BaseGroup* Res;
		if (argv == 0)
		{
			Res = new WeChatGroupZYS();
			return Res;
		}
		Res = new WeChatGroupZYS(argv);
		GlobalDataZYS::GroupDecorater[argv[6][1]-'0'](Res);//װ��
		//TODO QQGROUP
		return Res; }));
	GlobalDataZYS::GroupFactory.insert(pair<string, function<BaseGroup*(string *argv)>>("20", [](string *argv) {
		BaseGroup* Res;
		if (argv == 0)
		{
			Res = new WeChatGroupZYS();
			return Res;
		}
		string debug[7];
		for (int i = 0; i < 7; i++)
		{
			debug[i] = argv[i];
		}
		Res = new WeChatGroupZYS(argv);
		char debuga = argv[6][1];
		GlobalDataZYS::GroupDecorater[argv[6][1] - '0'](Res);//װ��
		return Res; }));


	//��ȡ�����û���Ϣ
	string SqlString = "SELECT * FROM USERS";
	Exe(SqlString, CreatCallBack);

	//��ȡ������Ϣ
	SqlString = "SELECT * FROM FRIEND";
	Exe(SqlString, GetFriendCallBack);

	//��ȡ������Ϣ
	SqlString = "SELECT * FROM GROUPS";
	Exe(SqlString, GetGroupCallBack);

	return 0;
}
//�õ�һ�����Ϸ�Χ[MIN,MAX]��ѡ��
int GetOption(int Min, int Max)
{
	char Option;
	do
	{
		Option = _getch();
	} while (Option - '0'<Min || Option - '0' > Max);
	return Option - '0';
}
int GetBigOption(int Min, int Max)
{
	string Strop;
	int Option;
	while (1)
	{
		cin >> Strop;
		if(Strop.size()>8)
			continue;
		
		Option = atoi(Strop.c_str());
		
		if (Option <= Max && Option >= Min)
		{
			return Option;
		}
		else
		{
			continue;
		}
	}

	return 0;
}
//���������ṩ�Ĳ���ʾЧ��
string InputPwd()
{
	string CorrectChars = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz{}[];':\"<>,./?\\!@#$%^&*()-_=+`~";
	string Pwd = "";
	do
	{
		char temp = _getch();
		if (temp == 13)//�س�
		{
			if (Pwd == "")
			{
				cout << "����Ϊ��!����������\n";
				continue;
			}
			break;
		}
		if (CorrectChars.find(temp) == -1)
			continue;
		Pwd += temp;
	} while (true);
	return Pwd;
}
//��������� ����NULL ������� ����id��Ӧ��ȫ��id
string idToGlobalid(int ProductCode, string id)
{
	auto iterfind = find_if(GlobalDataZYS::UserList[ProductCode].begin(), GlobalDataZYS::UserList[ProductCode].end(), [id](pair<string, BaseUserZYS*> user) {
		if (user.second->id == id)
			return 1;
		else
		{
			return 0;
		}
	});
	if (iterfind == GlobalDataZYS::UserList[ProductCode].end())
		return "";
	else
		return iterfind->first;
}

int GetBindUsers( BaseUserZYS  * User, vector<BaseUserZYS*>& BindUserList)
{
	int size = GlobalDataZYS::UserList.size();
	//�����в�Ʒ��Ѱ�������
	for (int i = 1; i <= size; i++)
	{
		if (i == User->ProductCode)
			continue;
		map<string, BaseUserZYS*>::iterator j;
		j = GlobalDataZYS::UserList[i].find(User->GetGlobalid());
		if (j == GlobalDataZYS::UserList[i].end())
			continue;
		else
		{
			BindUserList.push_back(j->second);
		}
	}
	return 0;
}

string GetCorrectGroup(int ProductCode)
{
	string Target_Globalid;
	cout << "����Ⱥ��" << endl;
	cin >> Target_Globalid;
	while (GlobalDataZYS::Groups[ProductCode].find(Target_Globalid) == GlobalDataZYS::Groups[ProductCode].end())
	{
		cout << "������Ⱥ��Ϊ" + Target_Globalid + "��Ⱥ" << "��1��������,��2�˳�" << endl;
		if (GetOption(1, 2) == 2)
			return "";
		else
		{
			cout << "����Ⱥ��" << endl;
			cin >> Target_Globalid;
		}
	}
	return Target_Globalid;
}

//GBKתUTF8(��Դ:CSDN)
string GBKToUTF8(const char* strGBK)
{
	int len = MultiByteToWideChar(CP_ACP, 0, strGBK, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, strGBK, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	string strTemp = str;
	if (wstr) delete[] wstr;
	if (str) delete[] str;
	return strTemp;
}
//UTF8תGBK(��Դ:CSDN)
string UTF8ToGBK(const char* strUTF8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, NULL, 0);
	wchar_t* wszGBK = new wchar_t[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, wszGBK, len);
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char* szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
	string strTemp(szGBK);
	if (wszGBK) delete[] wszGBK;
	if (szGBK) delete[] szGBK;
	return strTemp;
}
#pragma endregion

#pragma region �˵�
//��½�˵�
int LoginView(int ProductCode)
{
	system("cls");
	int i = GlobalDataZYS::Products.size()+1;
	string Account = "";
	
	if (ProductCode == 0)
	{
		printf("ѡ�������½�Ĳ�Ʒ\n");
		char Option = ChooseProductView();
		if (Option == i)//ѡ���˳�
			return 0;
		else//ѡ���˲�Ʒ
		{
			ProductCode = Option;
		}
	}
		system("cls");
		//�������� ������Ѱ�Ҷ�Ӧid���˺�
		string Globalid;
		while (true)
		{
			printf("�����˺�\n");
			cin >> Account;
			Globalid = idToGlobalid(ProductCode, Account);
			if (Globalid != "")//�ҵ�
				break;
			else//û�ҵ�
			{
				printf("�˺Ŵ���!�밴1����������߰�2�˳�\n");
				if (GetOption(1, 2) == 2)
					return 0;
			}
		}
		if (GlobalDataZYS::UserList[ProductCode][Globalid]->LoginCheck())
		{
			GlobalDataZYS::CurrentUser = GlobalDataZYS::UserList[ProductCode][Globalid]->GetGlobalid();
			GlobalDataZYS::UserList[ProductCode][Globalid]->ShowFoos();
			GlobalDataZYS::CurrentUser = "";
			return 0;
		}
		return 0;
}
//�����û��˵�
int CreatUserView()
{
	system("cls");
	while (1)
	{
		char Option;
		printf("1.����һ��QQ�û�\n2.����һ��΢���û�\n3.�˳�\n");
		Option = GetOption(1, 3);
		system("cls");
		BaseUserZYS *a;

		switch (Option)
		{
		case 1:
		{
			a = new QQUserZYS;
			break;
		}
		case 2:
		{
			a = new WeChatUserZYS;
			break;
		}
		case 3:
			goto there;
		default:
		{
			a = NULL;
			break;
		}
		}
		a->PrintMessage();
		GlobalDataZYS::UserList[a->ProductCode].insert(pair<string, BaseUserZYS*>(a->Global_id, a));
		system("pause");
		system("cls");
	}
there:
	return 0;
}
//ѡ���Ʒ�˵�
int ChooseProductView()
{
	int i;
	for (i = 0; i < (int)GlobalDataZYS::Products.size(); i++)
		printf("%d %s\n", i + 1, GlobalDataZYS::Products[i].c_str());
	printf("%d.�˳�\n", i+1);
	return GetOption(1, i+1);
}
#pragma endregion
