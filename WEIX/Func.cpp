#pragma once
#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<conio.h>
#include<map>
#include<wchar.h>
#include<Windows.h>
#include "sqlite3.h"
#include "Users.h"
using namespace std;
bool Called = false;
extern bool CanBack;


//��ʼ������!!!!
int init()
{
	GlobalDataZYS::Products.push_back("QQ");
	GlobalDataZYS::Products.push_back("΢��");
	GlobalDataZYS::UserList.insert(pair<int, map<string, BaseUserZYS*>>(1, GlobalDataZYS::QQUserList));
	GlobalDataZYS::UserList.insert(pair<int, map<string, BaseUserZYS*>>(2, GlobalDataZYS::WeChatUserList));

	//��ȡ�����û���Ϣ
	string SqlString = "SELECT * FROM USERS";
	Exe(SqlString, CreatCallBack);

	//��ȡ������Ϣ
	SqlString = "SELECT * FROM FRIEND";
	Exe(SqlString, GetFriendCallBack);


	return 0;
}
//SQL�ص�����------------------------------------------------------------------------------------------
//Ĭ��SQL�ص�����,������ͨ��ѯ,����
int callback(void *NotUsed, int argc, char **argv, char **azColName) {
	Called = true;
	int i;
	for (i = 0; i<argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}
//���ļ���ȡʱʹ�õĻص�����,������ʼ��һ���û���
int CreatCallBack(void *NotUsed, int argc, char **argv, char **azColName) {
	int i;
	string a = argv[5];
	if (GlobalDataZYS::LastGlobalid <= argv[1])//ˢ��LastGlobalid
	{
		GlobalDataZYS::LastGlobalid = argv[1];
		String_Add(&GlobalDataZYS::LastGlobalid);
	}
		switch (argv[5][0])
	{
	case '1':
	{
		GlobalDataZYS::UserList[1].insert(pair<string, BaseUserZYS*>(argv[1], (BaseUserZYS*)((new QQUserZYS(argv)))));
		if (GlobalDataZYS::LastQQid <= argv[7])//ˢ��id
		{
			GlobalDataZYS::LastQQid = argv[7];
			String_Add(&GlobalDataZYS::LastQQid);
		}
	}
	case '2':
	{
		GlobalDataZYS::UserList[2].insert(pair<string, BaseUserZYS*>(argv[1], (BaseUserZYS*)(new WeChatUserZYS(argv))));
		if (GlobalDataZYS::LastWeChatid <= argv[7])//ˢ��id
		{
			GlobalDataZYS::LastWeChatid = argv[7];
			String_Add(&GlobalDataZYS::LastWeChatid);
		}
	}
	default:
		break;
	}
	for (i = 0; i<argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}
//��ȡ����ʹ�õ�SQL�ص�
int GetFriendCallBack(void *NotUsed, int argc, char **argv, char **azColName)
{
	string FromGB = argv[1];
	string ToGB = argv[2];
	int ProductCode = atoi(argv[3]);
	GlobalDataZYS::UserList[ProductCode][FromGB]->Friends.GetRelationShip(GlobalDataZYS::UserList[ProductCode][FromGB], ToGB);
	printf("��Ʒ%d��ȡ%s��%s�ĺ��ѹ�ϵ�ɹ�\n", ProductCode, FromGB.c_str(), ToGB.c_str());
	return 0;
}

//���ܺ���----------------------------------------------------------------------------------------------
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
int Exe(string SqlStr,int (*callbackfunc)(void *, int , char **, char **))
{
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
//�����û��˵�
void CreatUserView()
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
	return;
}
//�õ�һ�����Ϸ�Χ��ѡ��
int GetOption(int Min, int Max)
{
	char Option;
	do
	{
		Option = _getch();
	} while (Option -'0'<Min  || Option-'0' > Max);
	return Option - '0';
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
