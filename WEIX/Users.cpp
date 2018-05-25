#pragma once
#include "Users.h"
#include "Func.h"
extern string LastRECORDid;
extern map<int, map<string, BaseUser*> > UserList;


//---------------------������-------------------------------
void Date::SetFromString(string a)
{
	Year = atoi(a.substr(0, 4).c_str());
	Month = atoi(a.substr(4, 2).c_str());
	Day = atoi(a.substr(6, 2).c_str());
}
void Date::SetBirthday()
{
	int *MonthDayLimit;
	do
	{
		cout << "����������:" << endl;
		cin >> Year;
		cout << "��������·�:" << endl;
		cin >> Month;
		cout << "���������:" << endl;
		cin >> Day;
		if (Year % 100 != 0 && Year % 4 == 0)MonthDayLimit = LeapMonthDay;//���������
		else MonthDayLimit = MonthDay;
		if (!(Year > 0 && Year <= 2018 && Month > 0 && Month <= 12 && Day > 0 && Day <= MonthDayLimit[Month - 1]))
		{
			cout << "��������,����������" << endl;
			fflush(stdin);
		}
		else
			return;
	} while (!(Year > 0 && Year <= 2018 && Month > 0 && Month <= 12 && Day>0 && Day <= MonthDayLimit[Month - 1]));
}
string Date::GetDateString()
{
	char Res[9];
	sprintf(Res, "%04d%02d%02d",Year, Month, Day);
	return Res;
}


//---------------------����---------------------------------
//ͨ�ò��ֵ�ע��
BaseUser::BaseUser()
{
	RECORDid = LastRECORDid;
	String_Add(&LastRECORDid);
	Global_id = LastGlobalid;
	String_Add(&LastGlobalid);
	cout << "�����ǳ�";
	cin >> Name;
	Birthday.SetBirthday();
	ReGistDate.SetBirthday();
	string Sqlstr = "INSERT INTO USERS (RECORDid,GLOBAL_ID,NAME,BIRTHDAY,REGIST_DATE)"\
		"VALUES('" + RECORDid + "','" + Global_id + "','" + Name + "','" + Birthday.GetDateString() + "','" + ReGistDate.GetDateString() + "');";
	Exe(Sqlstr);
}
//���ļ���ȡͨ�ò���
BaseUser::BaseUser(char **Attrs)//�����ݿ��г�ʼ��
{
	RECORDid = Attrs[0];
	Global_id = Attrs[1];
	Name = Attrs[2];
	Birthday.SetFromString(Attrs[3]);
	ReGistDate.SetFromString(Attrs[4]);
}
//�������ѹ�ϵ
int BaseUser::CreatFriendRelationship(string Target_Globalid)
{
	string ScureSqt = "SELECT * FROM FRIEND WHERE FROMGB='"+Global_id+"' AND TOGB='"+Target_Globalid+"';";
	if (Exe(ScureSqt) == 1)
	{
		printf("���ѹ�ϵ�Ѵ���!\n");
		return 0;
	}
	string SqlStr = "INSERT INTO FRIEND(FROMGB,TOGB,PRODUCTCODE)"\
		"VALUES('" + Global_id + "', '" + Target_Globalid + "', '" + to_string(ProductCode) + "'); ";
	Exe(SqlStr);
	GlobalFriendMap[ProductCode].insert(pair<string, BaseUser*>(Target_Globalid, UserList[ProductCode][Target_Globalid]));
	return 0;
}



//---------------------΢��---------------------------------
//΢���û�ע��
WeChatUser::WeChatUser() :BaseUser()//QQע��
{
	id = LastWeChatid;
	String_Add(&id);
	//UserList.insert(pair<string,QQUser*>(QQid, this));//����QQȫ���û�
	string Sqlstr = "UPDATE USERS SET Id = '" + id + "' where RECORDid = " + RECORDid + ";";
	Exe(Sqlstr);
}
//�ӱ��ض�ȡ΢���û�
WeChatUser::WeChatUser(char **Attrs):BaseUser(Attrs)
{
	ProductCode = 2;
	id = Attrs[7];
}
//����������
int WeChatUser::SetName(string NewName)
{
	Name = NewName;
	string Sqlstr = "UPDATE USERS SET NAME = '" + NewName + "' where RECORDid = " + RECORDid + ";";
	Exe(Sqlstr);
	return 0;
}
//����½
int WeChatUser::LoginCheck() { return 0; }
//��Ⱥ�б�ɾ��
int WeChatUser::DeledFromGroup() { return 0; }
//�ı�ȺȨ��
int WeChatUser::PermissionChange() { return 0; }



//---------------------QQ---------------------------------
//QQע��
QQUser::QQUser() :BaseUser()
{
	ProductCode = 1;
	id = LastQQid;
	String_Add(&id);
	//UserList.insert(pair<string,QQUser*>(QQid, this));//����QQȫ���û�
	string Sqlstr = "UPDATE USERS SET Id = '" + id + "' where RECORDid = " + RECORDid + ";" + "UPDATE USERS SET PRODUCTCODE = '" + to_string(ProductCode) + "' where RECORDid = " + RECORDid + ";";
	Exe(Sqlstr);
}
//���ļ���ȡQQ�û�
QQUser::QQUser(char **Attrs) :BaseUser(Attrs)
{
	ProductCode = 1;
	id = Attrs[7];
}
//����������
int QQUser::SetName(string NewName) {
	Name = NewName;
	string Sqlstr = "UPDATE USERS SET NAME = '" + NewName + "' where RECORDid = " + RECORDid + ";";
	Exe(Sqlstr);
	return 0;
}
//����½
int QQUser::LoginCheck() { return 0; }
//��Ⱥ�б�ɾ��
int QQUser::DeledFromGroup() { return 0; }
//�ı�ȺȨ��
int QQUser::PermissionChange() { return 0; }