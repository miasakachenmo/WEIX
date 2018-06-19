#pragma once
#include "Users.h"
#include <set>
#include <iostream>
#include <string>
using namespace std;
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
	string ScureSqt = "SELECT * FROM FRIEND WHERE FROMGB='" + Master->GetGlobalid() + "' AND TOGB='" + Target_Globalid + "' AND PRODUCTCODE='"+to_string( Master->ProductCode)+"';";
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
//����:����id
int IWeakBindWithQQUserZYS::BindTo(BaseUserZYS* Master, string Taget_Globalid)
{
	
	string Orinid = Master->Global_id;
	Master->Global_id = Taget_Globalid;
	string SQL = "UPDATE FRIEND SET FROMGB='" + Taget_Globalid + "' WHERE PRODUCTCODE='" + to_string(Master->ProductCode) + "' AND FROMGB='" + Orinid + "'";
	Exe(SQL);
	SQL = "UPDATE FRIEND SET TOGB='" + Taget_Globalid + "' WHERE PRODUCTCODE='" + to_string(Master->ProductCode) + "' AND TOGB='" + Orinid + "'";
	Exe(SQL);
	SQL = "UPDATE GROUPS SET GB='" + Taget_Globalid + "' WHERE PRODUCTCODE='" + to_string(Master->ProductCode) + "' AND TOGB='" + Orinid + "'";
	Exe(SQL);
	Master->OnUpDate();
	return 0;
}
//ǿ��:��id
int IStrongBindWithQQUserZYS::BindTo(BaseUserZYS* Master, string Taget_Globalid)
{
	string Orinid = Master->Global_id;
	Master->Global_id = Taget_Globalid;
	string SQL = "UPDATE FRIEND SET FROMGB='"+Taget_Globalid+"' WHERE PRODUCTCODE='"+to_string(Master->ProductCode)+"' AND FROMGB='"+Orinid+"'";
	Exe(SQL);
	SQL = "UPDATE FRIEND SET TOGB='" + Taget_Globalid + "' WHERE PRODUCTCODE='" + to_string(Master->ProductCode) + "' AND TOGB='" + Orinid + "'";
	Exe(SQL);
	Master->id = GlobalDataZYS::UserList[1][Taget_Globalid]->id;
	SQL= "UPDATE GROUPS SET GB='" + Taget_Globalid + "' WHERE PRODUCTCODE='" + to_string(Master->ProductCode) + "' AND TOGB='" + Orinid + "'";
	Exe(SQL);
	Master->OnUpDate();
	return 0;
 }


#pragma region �û�
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
BaseUserZYS::BaseUserZYS(string *Attrs) :Birthday("����"), ReGistDate("ע����") //�����ݿ��г�ʼ��
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

	if (GlobalDataZYS::UserList[1].find(Global_id) == GlobalDataZYS::UserList[1].end())
	{
		AddFunc("�󶨵�QQ", [this]() {
			string QQid;
			string QQGb;
			while (1)
			{
				cout << "������󶨵�QQ��"<<endl;
				cin >> QQid;
				QQGb = idToGlobalid(1, QQid);
				if (QQGb == "")
				{
					cout << "����QQ������!,��0�˳�,��1����" << endl;
					if (GetOption(0, 1) == 0)
						return 0;
				}
				else 
					break;
			}
			BindTo(this, QQGb);
			return 0; });
	}
	AddFunc("��½����������", [this]() {
		int size = GlobalDataZYS::UserList.size();
		vector<BaseUserZYS*>BindUserList;
		GetBindUsers(this, BindUserList);
		int Ops = 1;
		//�����в�Ʒ��Ѱ�������
		for (int i = 1; i <= size; i++)
		{
			if(i==ProductCode)
				continue;
			map<string, BaseUserZYS*>::iterator j;
			j = GlobalDataZYS::UserList[i].find(Global_id);
			if(j== GlobalDataZYS::UserList[i].end())
				continue;
			else
			{
				cout << Ops << ".  " << GlobalDataZYS::Products[i - 1]<<endl;
				Ops++;
				BindUserList.push_back(j->second);
			}
		}
		cout << "����Ӧ��id�������,��0�˳�" << endl;
		int Option = GetOption(0, BindUserList.size());
		if (Option == 0)
			return 0;
		else
			BindUserList[Option - 1]->ShowFoos();
		return 0;
	});
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
	AddFunc("����Ⱥ", [this]() {
		CreatGroup();
		return 0;
	});
	AddFunc("������������Ӻ���", [this]() {
		set<string> AllFriends;
		map<int, string> FindedFriends;
		vector<BaseUserZYS*> BindUser;
		GetBindUsers(this, BindUser);
		//�������ҵ��İ󶨵��û�
		for (int i = 0; i < BindUser.size(); i++)
		{
			//����ÿ���ҵ����û��ĺ����б�
			map<string, string>::iterator j;
			for (j = BindUser[i]->Friends.List.begin(); j != BindUser[i]->Friends.List.end(); j++)
			{
				AllFriends.insert(j->first);
			}
		}
		set<string>::iterator i= AllFriends.begin();
		int j=1;
		for (; i != AllFriends.end(); i++)
		{
			if (Friends.List.find(*i) == Friends.List.end()&&GlobalDataZYS::UserList[ProductCode].find(*i)!= GlobalDataZYS::UserList[ProductCode].end())//û���ҵ��Ҵ���
			{
				cout << j << ".  " << GlobalDataZYS::UserList[ProductCode][*i]->Name << endl;
				FindedFriends.insert(pair<int, string>(j, *i));
				j++;

			}
		}
		while (true)
		{
			cout << "�����������ĺ���,��0�˳�"<<endl;
			int Option = GetBigOption(0, j - 1);
			if (Option == 0)
				return 0;
			Friends.CreatRelationShip(this, FindedFriends[Option]);
		}
		return 0 ; });

}

#pragma endregion


#pragma region  WEICHAT
WeChatUserZYS::WeChatUserZYS() :BaseUserZYS()//QQע��
{
	ProductCode = 2;
	id = GlobalDataZYS::LastWeChatid;
	String_Add(&GlobalDataZYS::LastWeChatid);
	string Sqlstr = "UPDATE USERS SET Id = '" + id + "' where RECORDid = " + RECORDid + ";" + "UPDATE USERS SET PRODUCTCODE = '" + to_string(ProductCode) + "' where RECORDid = " + RECORDid + ";";
	Exe(Sqlstr);

	CreatMenuMap();
}
//�ӱ��ض�ȡ΢���û�
WeChatUserZYS::WeChatUserZYS(string *Attrs) :BaseUserZYS(Attrs)
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

void WeChatUserZYS::CreatGroup()
{
	WeChatGroupZYS::CreatGroup(this);
	return;
}

//��Ⱥ�б�ɾ��
int WeChatUserZYS::DeledFromGroup() { return 0; }
//�ı�ȺȨ��
int WeChatUserZYS::PermissionChange() { return 0; }

void WeChatUserZYS::CreatMenuMap()
{
	AddFunc("�������Ⱥ��", [this]() {
		string JoinGroupGlobalid=GetCorrectGroup(ProductCode);
		if (JoinGroupGlobalid == "")
			return 0;
		else
			//TODO �������Ĺ���
			GlobalDataZYS::Groups[ProductCode][JoinGroupGlobalid]->CreatRelationShip(this,"3");//��ͨȺԱ
		return 0; });
	AddFunc("Ⱥ�б�", [this]() {
		Groups.ShowList(ProductCode);
		return 0; });
	

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
QQUserZYS::QQUserZYS(string *Attrs) :BaseUserZYS(Attrs)
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

void QQUserZYS::CreatGroup()
{
	//TODO QQCREATGROUP
	return;
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
	string *Names = new string[Menu.size() + 5];
	int i = 0;
	//��ӡ�����б�
	while (true)
	{
		system("cls");
		for (iter = Menu.begin(),i=0; iter != Menu.end(); iter++, i++)
		{
			Names[i] = iter->first;
		}
		for (int j = 0; j < i; j++) cout << j << "." << Names[j] << endl;
		cout << i << ".�˳�" << endl;
		//TODO ��һ������

		int Option = GetBigOption(0, i);
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
//ȫ��id,Ȩ��
BaseGroup::BaseGroup()
{
	CreatMenuMap();
}

BaseGroup::BaseGroup(string * argvs)
{
	Groupid = argvs[1];
	GroupName = argvs[5];
	ProductCode = atoi(argvs[4].c_str());
	GroupType = argvs[6];
	CreatRelationShip(GlobalDataZYS::UserList[ProductCode][argvs[2]], argvs[3]);//������û��ӽ�ȥ
	CreatMenuMap();
}

//��Ⱥ��˵������ϵ�ĺ���,��Ҫȷ���Ƿ��Ѿ�����!���ǲ�û�а����ݼ��뵽�û���grouplist��,ע��һ��
int BaseGroup::CreatRelationShip(BaseUserZYS * Target, string PermissionCode)//����Ⱥ�Ĵ������Ѻ���!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!�ܸ��
{
	//��֤�Ƿ��Ѿ����ڹ�ϵ
	if (List.find(Target->GetGlobalid()) != List.end())
	{
		cout << "�Ѿ���Ⱥ��Ա��!!!!!!";
		return 0;
	}
	//�����ݿ���ȷ��
	string ScureSqt = "SELECT * FROM GROUPS WHERE GB='" + Target->GetGlobalid() + "' AND GROUPID='" + Groupid + "';";
	if (Exe(ScureSqt) == 1)
	{
		cout << Target->Name << "�Ѿ���Ⱥ��Ա��!!\n";
		return 0;
	}
	//TODO:��֤�Ƿ������Ⱥ ��֤��P �����Ⱥ�ķ���,��֤Ӧ�ü����û�����
	//����������������ݿ���ȥ
	string SQL = "INSERT INTO GROUPS(GB,GROUPID,PRODUCTCODE,PERMISSIONCODE,GROUPNAME,GROUPTYPE)"\
		"VALUES('" + Target->GetGlobalid() + "', '" + Groupid + "', '" + to_string(Target->ProductCode) + "','" + PermissionCode + "','"+GlobalDataZYS::Groups[Target->ProductCode][Groupid]->GroupName+"','"+ GlobalDataZYS::Groups[Target->ProductCode][Groupid]->GroupType +"');";
	Exe(SQL);
	//��Ⱥ���뵽�û����б���
	Target->Groups.List.insert(pair<string, string>(Groupid,PermissionCode));
	//���û����뵽Ⱥ���б���
	List.insert(pair<string, string>(Target->GetGlobalid(), PermissionCode));
	return 0;
}
//�����ݿ��ȡȺ��Ա�ĺ��� ע��û���κμ���!
int BaseGroup::GetRelationShip(BaseUserZYS * Target, string PermissionCode)
{
	
	List.insert(pair<string, string>(Target->GetGlobalid(), PermissionCode));
	return 0;
}
int BaseGroup::ShowList(int ProductCode)
{
	map<string, string>::iterator iter = List.begin();
	for (int i = 1;iter!=List.end(); iter++,i++)
	{
		cout << i  << ". " << GlobalDataZYS::UserList[ProductCode][iter->first]->Name <<"  "<< GlobalDataZYS::Permissions[GlobalDataZYS::Groups[ProductCode][Groupid]->GroupType[1] - '0'][atoi(iter->second.c_str())]<< endl;
	}
	system("pause");
	return 0;
}
//���Ϸ���1,�����Ϸ���0
int BaseGroup::PermissionCheck(string UserGB, string MinPermission)
{
	if (this->List[UserGB] <= MinPermission)
		return 1;
	else
	{
		cout << "����Ȩ�޲���!�˲�����Ҫ " << GlobalDataZYS::Permissions[GlobalDataZYS::Groups[ProductCode][Groupid]->GroupType[1] - '0'][atoi(MinPermission.c_str())] << " Ȩ��";
		return 0;
	}

}

void BaseGroup::CreatMenuMap()
{
	AddFunc("ȺԱ�б�", [this]() {
		ShowList(ProductCode);
		return 0;
	});
	AddFunc("�ı�Ⱥ����", [this]() {
		cout << "��ǰȺ����Ϊ " << GlobalDataZYS::GroupTypeNames[GroupType[1] - '0'] << endl;
		cout << "���������޸�Ϊ��Ⱥ����"<<endl;
		int j = 1;
		for (int i = 0; i <  GlobalDataZYS::GroupTypeNames.size();i++)
		{
			if(i==GroupType[1]-'0')
				continue;
			else
			{
				cout << j << ". " << GlobalDataZYS::GroupTypeNames[i] << endl;
				j++;
			}
		}
		cout << j << ". " << "ȡ���޸�" << endl;
		
		int Opt=GetOption(1, j);
		if (j == 0)
		{
			return 0;
		}
		else
		{
			int debug = GroupType[1] - '0';
			GlobalDataZYS::GroupUndecorater[GroupType[1]-'0'](this);
			GroupType = to_string(ProductCode * 10 + Opt);
		}
		GlobalDataZYS::GroupDecorater[GroupType[1] - '0'](this);
		cout << "Ⱥ�����Ѿ��ɹ��޸�Ϊ" << GlobalDataZYS::GroupTypeNames[Opt]<<endl;
		string Sql = "UPDATE GROUPS SET GROUPTYPE='"+GroupType+"' WHERE GROUPID='"+Groupid+"' AND PRODUCTCODE='"+to_string(ProductCode)+"' ;";
		Exe(Sql);
		//�˵�ˢ��:
		return 0;
	});
}//�����˵�!!��Ҫ

void BaseGroup::SetPermissionCode(string TargetGB,string NewCode)
{
	//����һ�������Ƿ񳬹�����
	auto iterfind =find_if(List.begin(), List.end(), [](pair<string, string> User) {
		if (User.second == "1")
			return 1;
		else
			return 0;
			});
	if (iterfind != List.end())
	{
		cout << "����ʧ��,����������Ⱥ��!"<<endl;
		system("pause");
		return;
	}
	List[TargetGB] = NewCode;
	string test;
	string SQL = "UPDATE GROUPS SET PERMISSIONCODE='" + NewCode + "' WHERE GROUPID='" +Groupid + "' AND USERGB='" + TargetGB + "' AND PRODUCTCODE='"+to_string(ProductCode)+"';";
	Exe(SQL);
	return;
}

void BaseGroup::SetAdm()
{
	ShowList(ProductCode);
	cout << "������Ҫ��Ϊ�����ȺԱ���,�ÿո�ֿ�" << endl;
	char c;
	string GB;
	while (c = cin.get() != '\n')
	{
		cin.unget();
		cin >> GB;
		SetPermissionCode(GB, "2");
	}
	
}

BaseGroup* BaseGroup::CreatGroup(BaseUserZYS * GroupMaster)
{
	BaseGroup *Temp = GlobalDataZYS::GroupFactory[to_string(GroupMaster->ProductCode*10)](0);
	Temp->GroupType = to_string(GroupMaster->ProductCode * 10);
	Temp->Groupid = GlobalDataZYS::LastGroupid;
	cout << "����Ⱥ����" << endl;
	cin >> Temp->GroupName;
	//cout << "����Ⱥ����" << endl;
	//cin>>
	Temp->Groupid = GlobalDataZYS::LastGroupid;
	String_Add(&GlobalDataZYS::LastGroupid);
	GlobalDataZYS::Groups[GroupMaster->ProductCode].insert(pair<string, BaseGroup*>(Temp->Groupid, Temp));
	Temp->ProductCode = GroupMaster->ProductCode;
	Temp->CreatRelationShip(GroupMaster, "1");
	//	GlobalDataZYS::Groups[GroupMaster->ProductCode].insert(pair<string, BaseGroup*>(Temp->Groupid, Temp));//Ⱥ����Ⱥ�б�
	//�������ݿ�
	return Temp;
}


WeChatGroupZYS::WeChatGroupZYS() :BaseGroup()
{
	CreatMenuMap();
}
WeChatGroupZYS::WeChatGroupZYS(string * argvs) : BaseGroup(argvs)
{
	CreatMenuMap();
}

void WeChatGroupZYS::CreatMenuMap()
{
	//����:����Ա

	//TODO Ҳ��������Ը���
	AddFunc("����", [this]() {
		if (!PermissionCheck(GlobalDataZYS::CurrentUser, "2"))
			return 0;
		ShowList(ProductCode);
		cout << "ѡ��һ�����ߵ�,�������" << endl;
		int Option = GetBigOption(1, List.size());
		map<string, string>::iterator i,Deliter;
		int Index = 1;
		for (i = List.begin(); Index != Option; Index++, i++);
		string Sql = "DELETE FROM GROUPS WHERE GB='" + i->first + "' AND PRODUCTCODE='" + to_string(ProductCode) + "' AND GROUPID ='" + Groupid + "';";
		Exe(Sql);
		//˫��ɾ�� ���˵Ĺ���23333
		GlobalDataZYS::UserList[ProductCode][i->first]->Groups.List.erase(GlobalDataZYS::UserList[ProductCode][i->first]->Groups.List.find(Groupid));
		List.erase(i);
		return 0;
	});
	AddFunc("���ù���Ա", [this]() {
		if (PermissionCheck(GlobalDataZYS::CurrentUser, "2"))
			return 0;
		ShowList(ProductCode);
		cout << "ѡ��һ���˳�Ϊ����Ա,�������"<<endl;
		int Option = GetBigOption(1, List.size());
		map<string, string>::iterator i;
		int Index = 1;
		for (i = List.begin(); Index != Option;Index++,i++);
		
		SetPermissionCode(i->first, "2");
		cout << "�û� " << GlobalDataZYS::UserList[ProductCode][i->first]->Name << "�Ѿ�������Ϊ����Ա!" << endl;
		return 0;
	});
}

#pragma endregion


int GroupList::CreatRelationShip(BaseUserZYS * Master, string Target_Globalid)
{
	//TODO: ����Ⱥ�Ĳ���,�˴���Ҫ��֤һ���Ƿ����Ⱥ �����֤Ӧ�ü��ڱ�
	/*	while(GlobalDataZYS::Groups[Master->ProductCode].find(Target_Globalid) == GlobalDataZYS::Groups[Master->ProductCode].end())
	{
		cout << "������Ⱥ��Ϊ" + Target_Globalid + "��Ⱥ" << "��1��������,��2�˳�"<<endl;
		if (GetOption(1, 2) == 2)
			return 0;
		else
		{
			cout << "����Ⱥ��" << endl;
			cin >> Target_Globalid;
		}
	};*/

	GlobalDataZYS::Groups[Master->ProductCode][Target_Globalid]->CreatRelationShip(Master, "3");
	return 0;
}

//��ȡȺԱ��ϵ
int GroupList::GetRelationShip(BaseUserZYS * Master, string Target_Globalid)
{
	return 0;
}

int GroupList::ShowList(int ProductCode)
{
	string Option;
	map<string, string>::iterator i;
	vector<BaseGroup*> Entrence;
	int Count = 1;
	cout << "    Ⱥ��     Ⱥ��      ״̬"<<endl;
	for (i = List.begin(); i != List.end(); i++,Count++)
	{
		cout << Count << "." << GlobalDataZYS::Groups[ProductCode][i->first]->GroupName << "    " << i->first << "  " << GlobalDataZYS::Permissions[GlobalDataZYS::Groups[ProductCode][i->first]->GroupType[1] - '0'][atoi(i->second.c_str())] << endl;
		Entrence.push_back(GlobalDataZYS::Groups[ProductCode][i->first]);
	}
	cout << "����0�˳�,������Ž���Ⱥ"<<endl;
	while (1)
	{
		cin >> Option;
		if (Option == "0")
			return 0;
		else
		{
			int Index = atoi(Option.c_str());
			
			if (Index<=Entrence.size()&&Index>0)
			{
				Entrence[Index - 1]->ShowFoos();
				break;
			}
			else
			{
				cout << "�������Ų���ȷ! ��0�˳���1��������" << endl;
				if (GetOption(0, 1) == 0)
					return 0;
				else continue;
			}
		}
	}
	return 0;
}

