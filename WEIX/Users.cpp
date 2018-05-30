#pragma once
#include "Users.h"
#include "Func.h"
//extern string LastRECORDid;
//extern map<int, map<string, BaseUserZYS*> > UserList;
//extern vector<string> Products;

//---------------------������-------------------------------
DateZYS::DateZYS()
{
	DateName = "";
}
DateZYS::DateZYS(string Name)
{
	DateName = Name;
}
void DateZYS::SetFromString(string a)
{
	Year = atoi(a.substr(0, 4).c_str());
	Month = atoi(a.substr(4, 2).c_str());
	Day = atoi(a.substr(6, 2).c_str());
}
void DateZYS::SetBirthday()
{
	int *MonthDayLimit;
	do
	{
		cout << "����"+DateName+"���:" << endl;
		cin >> Year;
		cout << "����"+DateName+"�·�:" << endl;
		cin >> Month;
		cout << "����"+DateName+"��:" << endl;
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
string DateZYS::GetDateString()
{
	char Res[9];
	sprintf(Res, "%04d%02d%02d",Year, Month, Day);
	return Res;
}
//�б���----------------------------------------------------
int FriendList::CreatRelationShip(BaseUserZYS* Master, string Target_Globalid)
{
	//��֤�Ƿ��Ѿ�����
	string ScureSqt = "SELECT * FROM FRIEND WHERE FROMGB='" + Master->GetGlobalid() + "' AND TOGB='" + Target_Globalid + "';";
	if (Exe(ScureSqt) == 1)
	{
		printf("���ѹ�ϵ�Ѵ���!\n");
		return 0;
	}

	//����������������ݿ���ȥ
	string SqlStr = "INSERT INTO FRIEND(FROMGB,TOGB,PRODUCTCODE)"\
		"VALUES('" + Master->GetGlobalid() + "', '" + Target_Globalid + "', '" + to_string(Master->ProductCode) + "'); ";
	Exe(SqlStr);

	//д���ڴ�
	List[Master->ProductCode].insert(pair<string, string>(Target_Globalid, GlobalDataZYS::UserList[Master->ProductCode][Target_Globalid]->Name));
	return 0;
}
int FriendList::GetRelationShip(BaseUserZYS* Master, string Target_Globalid)
{
	//д���ڴ�
	List[Master->ProductCode].insert(pair<string, string>(Target_Globalid, GlobalDataZYS::UserList[Master->ProductCode][Target_Globalid]->Name));
	return 0;
}
int FriendList::ShowList(BaseUserZYS* Master, string Target_Globalid)
{
	//UNDONE SHOWLIST
	map<string, string>::iterator iter;
	printf("�˺�      ����");
	for (iter = List[Master->ProductCode].begin(); iter != List[Master->ProductCode].end(); iter++)
	{
		printf("%s      %s\n", GlobalDataZYS::UserList[Master->ProductCode][iter->first]->id.c_str(),iter->second.c_str());
	}
	return 0;
}

//ǿ������-----------------------------------------------------------------------------------
int WeakBindWithQQUserZYS::BindTo(BaseUserZYS* Master, string Taget_Globalid)
{
	Master->Global_id = Taget_Globalid;
	return 0;
}
int StrongBindWithQQUserZYS::BindTo(BaseUserZYS* Master, string Taget_Globalid)
{
	Master->Global_id = Taget_Globalid;
	Master->id = GlobalDataZYS::UserList[1][Taget_Globalid]->id;
	return 0;
 }


#pragma region ��Ʒ
//---------------------����---------------------------------
//ͨ�ò��ֵ�ע��
BaseUserZYS::BaseUserZYS() :Birthday("����"), ReGistDate("ע����")
{
	RECORDid = GlobalDataZYS::LastRECORDid;
	String_Add(&GlobalDataZYS::LastRECORDid);
	Global_id = GlobalDataZYS::LastGlobalid;
	String_Add(&GlobalDataZYS::LastGlobalid);
	cout << "�����ǳ�\n";
	cin >> Name;
	Birthday.SetBirthday();
	ReGistDate.SetBirthday();
	cout << "��������\n";
	Pwd = SetPwd();
	string Sqlstr = "INSERT INTO USERS (RECORDid,GLOBAL_ID,NAME,BIRTHDAY,REGIST_DATE,PWD)"\
		"VALUES('" + RECORDid + "','" + Global_id + "','" + GBKToUTF8(Name.c_str()) + "','" + Birthday.GetDateString() + "','" + ReGistDate.GetDateString() + "','" + Pwd + "');";
	Exe(Sqlstr);
}
//���ļ���ȡͨ�ò���
BaseUserZYS::BaseUserZYS(char **Attrs) :Birthday("����"), ReGistDate("ע����") //�����ݿ��г�ʼ��
{
	RECORDid = Attrs[0];
	Global_id = Attrs[1];
	Name = Attrs[2];
	Birthday.SetFromString(Attrs[3]);
	ReGistDate.SetFromString(Attrs[4]);
	Pwd = Attrs[7];
}
//�������ѹ�ϵ
int BaseUserZYS::CreatFriendRelationship(string Target_Globalid)
{
	string ScureSqt = "SELECT * FROM FRIEND WHERE FROMGB='" + Global_id + "' AND TOGB='" + Target_Globalid + "';";
	if (Exe(ScureSqt) == 1)
	{
		printf("���ѹ�ϵ�Ѵ���!\n");
		return 0;
	}
	string SqlStr = "INSERT INTO FRIEND(FROMGB,TOGB,PRODUCTCODE)"\
		"VALUES('" + Global_id + "', '" + Target_Globalid + "', '" + to_string(ProductCode) + "'); ";
	Exe(SqlStr);
	GlobalFriendMap[ProductCode].insert(pair<string, BaseUserZYS*>(Target_Globalid, GlobalDataZYS::UserList[ProductCode][Target_Globalid]));
	return 0;
}
//����
int BaseUserZYS::SetName(string NewName)
{
	Name = NewName;
	string Sqlstr = "UPDATE USERS SET NAME = '" + NewName + "' where RECORDid = " + RECORDid + ";";
	Exe(Sqlstr);
	return 0;
}
//��ӡ������Ϣ
int BaseUserZYS::PrintMessage()
{
	printf("%s�û�\n�ǳ�:%s\n�˺�:%s\n����:%s\nע������:%s\n", GlobalDataZYS::Products[ProductCode - 1].c_str(), Name.c_str(), id.c_str(), Birthday.GetDateString().c_str(), ReGistDate.GetDateString().c_str());
	return 0;
}
//�õ�ȫ����Ϣ
string BaseUserZYS::GetGlobalid()
{
	return Global_id;
}
//��������
string BaseUserZYS::SetPwd()
{
	Pwd = "";
	do
	{
		cout << "��������\n";
		Pwd = InputPwd();//�㷨����PWD����Ϊ��
		cout << "ȷ������\n";
		if (Pwd != InputPwd())//ȷ������
		{
			cout << "�ٴ���������벻��ȷ,��������!";
			continue;
		}
	} while (false);
	return Pwd;
}
//��֤����
bool BaseUserZYS::CheckPwd(string InputPwd)
{
	if (InputPwd == Pwd)
		return true;
	else
		return false;
}
//�������ݿ�
int BaseUserZYS::OnUpDate()
{
	string SqlStr;
	char Buffer[200];
	sprintf(Buffer, "UPDATE USERS SET GLOBAL_ID='%s',NAME='%s',BIRTHDAY='%s',REGIST_DATE='%s',Id='%s',"
		"PWD='%s'  WHERE Recordid=%s;",Global_id.c_str(),Name.c_str(),(Birthday.GetDateString()).c_str(),(ReGistDate.GetDateString()).c_str(),id.c_str(),Pwd.c_str(),RECORDid.c_str());
	SqlStr= Buffer;
	Exe(SqlStr);
	return 0;
}

//UNDONE QQ��΢�ŵĵ�½���ģ��
//---------------------΢��---------------------------------
//΢���û�ע��
WeChatUserZYS::WeChatUserZYS() :BaseUserZYS()//QQע��
{
	ProductCode = 2;
	id = GlobalDataZYS::LastWeChatid;
	String_Add(&id);
	//UserList.insert(pair<string,QQUserZYS*>(QQid, this));//����QQȫ���û�
	string Sqlstr = "UPDATE USERS SET Id = '" + id + "' where RECORDid = " + RECORDid + ";" + "UPDATE USERS SET PRODUCTCODE = '" + to_string(ProductCode) + "' where RECORDid = " + RECORDid + ";";
	Exe(Sqlstr);
}
//�ӱ��ض�ȡ΢���û�
WeChatUserZYS::WeChatUserZYS(char **Attrs) :BaseUserZYS(Attrs)
{
	ProductCode = 2;
	id = Attrs[7];
}
//����½
bool WeChatUserZYS::LoginCheck() { return 1; }
//��Ⱥ�б�ɾ��
int WeChatUserZYS::DeledFromGroup() { return 0; }
//�ı�ȺȨ��
int WeChatUserZYS::PermissionChange() { return 0; }


//---------------------QQ---------------------------------
//QQע��
QQUserZYS::QQUserZYS() :BaseUserZYS()
{
	ProductCode = 1;
	id = GlobalDataZYS::LastQQid;
	String_Add(&id);
	//UserList.insert(pair<string,QQUserZYS*>(QQid, this));//����QQȫ���û�
	string Sqlstr = "UPDATE USERS SET Id = '" + id + "' where RECORDid = " + RECORDid + ";" + "UPDATE USERS SET PRODUCTCODE = '" + to_string(ProductCode) + "' where RECORDid = " + RECORDid + ";";
	Exe(Sqlstr);
}
//���ļ���ȡQQ�û�
QQUserZYS::QQUserZYS(char **Attrs) :BaseUserZYS(Attrs)
{
	ProductCode = 1;
	id = Attrs[7];
}
//����½
bool QQUserZYS::LoginCheck() { return true; }
//��Ⱥ�б�ɾ��
int QQUserZYS::DeledFromGroup() { return 0; }
//�ı�ȺȨ��
int QQUserZYS::PermissionChange() { return 0; }
#pragma endregion

