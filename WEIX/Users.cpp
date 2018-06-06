#pragma once
#include "Users.h"
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
	List.insert(pair<string, string>(Target_Globalid, GlobalDataZYS::UserList[Master->ProductCode][Target_Globalid]->Name));
	return 0;
}
int FriendList::GetRelationShip(BaseUserZYS* Master, string Target_Globalid)
{
	//д���ڴ�
	List.insert(pair<string, string>(Target_Globalid, GlobalDataZYS::UserList[Master->ProductCode][Target_Globalid]->Name));
	return 0;
}
int FriendList::ShowList(int ProductCode)
{
	//UNDONE SHOWLIST
	map<string, string>::iterator iter;
	printf("�˺�      ����\n");
	for (iter = List.begin(); iter != List.end(); iter++)
	{
		printf("%s      %s\n", GlobalDataZYS::UserList[ProductCode][iter->first]->id.c_str(),iter->second.c_str());
	}
	system("pause");
	return 0;
}

//ǿ������-----------------------------------------------------------------------------------
int IWeakBindWithQQUserZYS::BindTo(BaseUserZYS* Master, string Taget_Globalid)
{
	
	string Orinid = Master->Global_id;
	Master->Global_id = Taget_Globalid;
	string SQL = "UPDATE FRIEND SET FROMGB='" + Taget_Globalid + "' WHERE PRODUCTCODE='" + to_string(Master->ProductCode) + "' AND FROMGB='" + Orinid + "'";
	Exe(SQL);
	SQL = "UPDATE FRIEND SET TOGB='" + Taget_Globalid + "' WHERE PRODUCTCODE='" + to_string(Master->ProductCode) + "' AND TOGB='" + Orinid + "'";
	Exe(SQL);
	Master->OnUpDate();
	return 0;
}
int IStrongBindWithQQUserZYS::BindTo(BaseUserZYS* Master, string Taget_Globalid)
{
	string Orinid = Master->Global_id;
	Master->Global_id = Taget_Globalid;
	string SQL = "UPDATE FRIEND SET FROMGB='"+Taget_Globalid+"' WHERE PRODUCTCODE='"+to_string(Master->ProductCode)+"' AND FROMGB='"+Orinid+"'";
	Exe(SQL);
	SQL = "UPDATE FRIEND SET TOGB='" + Taget_Globalid + "' WHERE PRODUCTCODE='" + to_string(Master->ProductCode) + "' AND TOGB='" + Orinid + "'";
	Exe(SQL);
	Master->id = GlobalDataZYS::UserList[1][Taget_Globalid]->id;
	return 0;
 }


#pragma region ��Ʒ
#pragma region ����

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
	RECORDid = GlobalDataZYS::LastRECORDid;
	String_Add(&GlobalDataZYS::LastRECORDid);
	string Sqlstr = "INSERT INTO USERS (RECORDid,GLOBAL_ID,NAME,BIRTHDAY,REGIST_DATE,PWD)"\
		"VALUES('" + RECORDid + "','" + Global_id + "','" + GBKToUTF8(Name.c_str()) + "','" + Birthday.GetDateString() + "','" + ReGistDate.GetDateString() + "','" + Pwd + "');";
	Exe(Sqlstr);
	CreatMenuMap();
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
	CreatMenuMap();
}

int BaseUserZYS::LoginCheck()
{
	system("cls");
	int Try = 2;
	for (; Try >= 0; Try--)
	{
		cout << "��������" << endl;
		string InPwd = InputPwd();
		if (InPwd == Pwd)
			return 1;
		else
		{
			cout << "�㻹��" << Try << "�λ���" << endl;
			cout << "����������(1.����2.�˳�)" << endl;
			if (GetOption(1, 2) == 2)
				return 0;
		}
	}
	return 0;
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
	system("pause");
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
	Pwd = "";//��������
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
		else
		{
			break;
		}
	} while (true);
	OnUpDate();//ͬ�������ݿ�
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
		"PWD='%s'  WHERE Recordid=%s;", Global_id.c_str(), Name.c_str(), (Birthday.GetDateString()).c_str(), (ReGistDate.GetDateString()).c_str(), id.c_str(), Pwd.c_str(), RECORDid.c_str());
	SqlStr = Buffer;
	Exe(SqlStr);
	return 0;
}

void BaseUserZYS::CreatMenuMap()
{
	AddFunc("�鿴������Ϣ", [this]() {
		PrintMessage();
		return 0;
	});
	AddFunc("�޸�����", [this]() {
		system("cls");
		string NewName;
		cout << "����������" << endl;
		cin >> NewName;
		SetName(NewName);
		return 0; });
	AddFunc("�����б�", [this]() {
		Friends.ShowList(this->ProductCode);
		return 0; });
	AddFunc("�޸�����", [this]() {
		SetPwd();
		return 0;
	});
	AddFunc("�Ӻ���", [this]() {
		system("cls");
		string Target, TOGB;
		cout << "����Է�ID" << endl;
		cin >> Target;

		while (true)
		{
			TOGB = idToGlobalid(ProductCode, Target);
			if (TOGB == "")
			{
				cout << "������!,��1����,��2�˳�" << endl;
				if (GetOption(1, 2) == 2)
					return 0;
				else
					cin >> Target;
			}
			else
				break;
		}
		Friends.CreatRelationShip(this, TOGB);
		return 0;

	});
}

#pragma endregion


#pragma region  WEICHAT
WeChatUserZYS::WeChatUserZYS() :BaseUserZYS()//QQע��
{
	ProductCode = 2;
	id = GlobalDataZYS::LastWeChatid;
	String_Add(&id);
	string Sqlstr = "UPDATE USERS SET Id = '" + id + "' where RECORDid = " + RECORDid + ";" + "UPDATE USERS SET PRODUCTCODE = '" + to_string(ProductCode) + "' where RECORDid = " + RECORDid + ";";
	Exe(Sqlstr);
	CreatMenuMap();
}
//�ӱ��ض�ȡ΢���û�
WeChatUserZYS::WeChatUserZYS(char **Attrs) :BaseUserZYS(Attrs)
{
	ProductCode = 2;
	id = Attrs[6];
	if (GlobalDataZYS::LastWeChatid <= Attrs[6])//ˢ��id
	{
		GlobalDataZYS::LastWeChatid = Attrs[6];
		String_Add(&GlobalDataZYS::LastWeChatid);
	}
	OnUpDate();
	CreatMenuMap();
}

//��Ⱥ�б�ɾ��
int WeChatUserZYS::DeledFromGroup() { return 0; }
//�ı�ȺȨ��
int WeChatUserZYS::PermissionChange() { return 0; }

void WeChatUserZYS::CreatMenuMap()
{
	Menu.insert(pair<string, function<int()>>("�󶨵�QQ", [this]() {
		//�Ӽ�� ����Ƿ��Ѿ��󶨹�
		
		string QQid, TOGB;
		cout << "������󶨵���QQ��" << endl;
		while (true)
		{
			cin >> QQid;
			TOGB = idToGlobalid(1, QQid);
			if (TOGB == "")//QQ�Ų�����
			{
				cout << "QQ�Ų�����!��1�˳����߰�2����" << endl;
				if (GetOption(1, 2) == 2)
					return 0;
				else
					continue;
			}
			if (GlobalDataZYS::UserList[ProductCode].find(TOGB)!= GlobalDataZYS::UserList[ProductCode].end())//�Ѿ����󶨹�,�ж�ӦID��΢�ź���
			{
				cout << "QQ���Ѿ��а󶨵�΢����!��1�˳����߰�2��������" << endl;
				if (GetOption(1, 2) == 2)
					return 0;
				else
					continue;
			}
			BindTo(this, TOGB);
			return 0;
		}
		return 0;
	}));
	//AddFunc("����Ⱥ��", [this]() {
		//WeChatGroupZYS.CreatGroup();
		//return 0; });
}
#pragma endregion


#pragma region QQ

QQUserZYS::QQUserZYS() :BaseUserZYS()
{
	ProductCode = 1;
	id = GlobalDataZYS::LastQQid;
	String_Add(&GlobalDataZYS::LastQQid);
	string Sqlstr = "UPDATE USERS SET Id = '" + id + "' where RECORDid = " + RECORDid + ";" + "UPDATE USERS SET PRODUCTCODE = '" + to_string(ProductCode) + "' where RECORDid = " + RECORDid + ";";
	Exe(Sqlstr);
}
//���ļ���ȡQQ�û�
QQUserZYS::QQUserZYS(char **Attrs) :BaseUserZYS(Attrs)
{
	ProductCode = 1;
	id = Attrs[6];
	if (GlobalDataZYS::LastQQid <= Attrs[6])//ˢ��id
	{
		GlobalDataZYS::LastQQid = Attrs[6];
		String_Add(&GlobalDataZYS::LastQQid);
	}
	OnUpDate();
}

//��Ⱥ�б�ɾ��
int QQUserZYS::DeledFromGroup() { return 0; }
//�ı�ȺȨ��
int QQUserZYS::PermissionChange() { return 0; }
void QQUserZYS::CreatMenuMap()
{
}
#pragma endregion

#pragma endregion


#pragma region �˵���

void MenuInterface::AddFunc(string FooName, function<int()> Foo)
{
	Menu.insert(pair<string, function<int()>>(FooName, Foo));
	return;
}
int MenuInterface::ShowFoos()
{
	map<string, function<int()>>::iterator iter;
	string *Names = new string[Menu.size() + 1];
	int i = 1;
	for (iter = Menu.begin(); iter != Menu.end(); iter++, i++)
	{
		cout << i << "." << iter->first << endl;
		Names[i] = iter->first;
	}
	//��ӡ�����б�
	while (true)
	{
		system("cls");
		for (int j = 1; j < i; j++) cout << j << "." << Names[j] << endl;
		cout << i << ".�˳�" << endl;
		int Option = GetOption(1, i);
		if (Option == i)
			return 0;
		else
		{
			Menu[Names[Option]]();
		}
	}
}
#pragma endregion


#pragma region Ⱥ

//������ϵ�ĺ���,��Ҫȷ���Ƿ��Ѿ�����!
int BaseGroup::CreatRelationShip(BaseUserZYS * Target, string PermissionCode)//����Ⱥ�Ĵ������Ѻ���!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!�ܸ��
{
	//��֤�Ƿ��Ѿ����ڹ�ϵ
	if (List.find(Target->GetGlobalid()) != List.end())
	{
		cout << "�Ѿ���Ⱥ��Ա��!!!!!!";
	}
	string ScureSqt = "SELECT * FROM GROUPS WHERE GB='" + Target->GetGlobalid() + "' AND GROUPID='" + Groupid + "';";
	if (Exe(ScureSqt) == 1)
	{
		cout << Target->Name << "�Ѿ���Ⱥ��Ա��!!\n";
		return 0;
	}
	//TODO:��֤�Ƿ������Ⱥ ��֤��P �����Ⱥ�ķ���,��֤Ӧ�ü����û�����
	//����������������ݿ���ȥ
	string SqlStr = "INSERT INTO GROUPS(GB,GROUPID,PRODUCTCODE,PERMISSIONCODE)"\
		"VALUES('" + Target->GetGlobalid() + "', '" + Groupid + "', '" + to_string(Target->ProductCode) + "','" + PermissionCode + "'); ";
	Exe(SqlStr);

	//д���ڴ�
	List.insert(pair<string, string>(Target->GetGlobalid(), PermissionCode));
	return 0;
}
//���������ݻ�ȡȺ��Ա�ĺ��� ע��û���κμ���!
int BaseGroup::GetRelationShip(BaseUserZYS * Target, int PermissionCode)
{
	
	List.insert(pair<string, string>(Target->GetGlobalid(), to_string(PermissionCode)));
	return 0;
}

int BaseGroup::ShowList(int ProductCode)
{
	
	return 0;
}

BaseGroup * BaseGroup::CreatGroup(BaseUserZYS * GroupMaster)
{
	BaseGroup *Temp = new BaseGroup;
	cout << "����Ⱥ����" << endl;
	cin >> Temp->GroupName;
	//cout << "����Ⱥ����" << endl;
	//cin>>
	Temp->Groupid = GlobalDataZYS::LastGroupid;
	String_Add(&GlobalDataZYS::LastGroupid);
	Temp->CreatRelationShip(GroupMaster, "1");
	//	GlobalDataZYS::Groups[GroupMaster->ProductCode].insert(pair<string, BaseGroup*>(Temp->Groupid, Temp));//Ⱥ����Ⱥ�б�
	//�������ݿ�
	string Sqlstr = "INSERT INTO USERS (GROUPID,GB,PRODUCTCODE,PERMISSIONCODE,GROUPNAME)"\
		"VALUES('" + Temp->Groupid + "','" + GroupMaster->GetGlobalid() + "','" + to_string(GroupMaster->ProductCode) + "','" + "1'" + ",'" + Temp->GroupName + "');";
	Exe(Sqlstr);
	return Temp;
}

WeChatGroupZYS::WeChatGroupZYS(char ** attrs)
{
	Groupid = attrs[1];
	GroupName = attrs[5];
}


#pragma endregion