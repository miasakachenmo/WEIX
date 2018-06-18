#pragma once
#include "Users.h"
//---------------------日期类-------------------------------
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
		cout << "输入"+DateName+"年份:" << endl;
		cin >> Year;
		cout << "输入"+DateName+"月份:" << endl;
		cin >> Month;
		cout << "输入"+DateName+"日:" << endl;
		cin >> Day;
		if (Year % 100 != 0 && Year % 4 == 0)MonthDayLimit = LeapMonthDay;//年份是闰年
		else MonthDayLimit = MonthDay;
		if (!(Year > 0 && Year <= 2018 && Month > 0 && Month <= 12 && Day > 0 && Day <= MonthDayLimit[Month - 1]))
		{
			cout << "输入有误,请重新输入" << endl;
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
//列表类----------------------------------------------------
int FriendList::CreatRelationShip(BaseUserZYS* Master, string Target_Globalid)
{

	//验证是否已经存在
	string ScureSqt = "SELECT * FROM FRIEND WHERE FROMGB='" + Master->GetGlobalid() + "' AND TOGB='" + Target_Globalid + "';";
	if (Exe(ScureSqt) == 1)
	{
		printf("好友关系已存在!\n");
		return 0;
	}

	//若不存在则调用数据库存进去
	string SqlStr = "INSERT INTO FRIEND(FROMGB,TOGB,PRODUCTCODE)"\
		"VALUES('" + Master->GetGlobalid() + "', '" + Target_Globalid + "', '" + to_string(Master->ProductCode) + "'); ";
	Exe(SqlStr);

	//写入内存
	List.insert(pair<string, string>(Target_Globalid, GlobalDataZYS::UserList[Master->ProductCode][Target_Globalid]->Name));
	return 0;
}
int FriendList::GetRelationShip(BaseUserZYS* Master, string Target_Globalid)
{
	//写入内存
	List.insert(pair<string, string>(Target_Globalid, GlobalDataZYS::UserList[Master->ProductCode][Target_Globalid]->Name));
	return 0;
}
int FriendList::ShowList(int ProductCode)
{
	//UNDONE SHOWLIST
	map<string, string>::iterator iter;
	printf("账号      密码\n");
	for (iter = List.begin(); iter != List.end(); iter++)
	{
		printf("%s      %s\n", GlobalDataZYS::UserList[ProductCode][iter->first]->id.c_str(),iter->second.c_str());
	}
	system("pause");
	return 0;
}

//强弱绑定类-----------------------------------------------------------------------------------
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


#pragma region 用户
#pragma region 基类

BaseUserZYS::BaseUserZYS() :Birthday("生日"), ReGistDate("注册日")
{
	RECORDid = GlobalDataZYS::LastRECORDid;
	String_Add(&GlobalDataZYS::LastRECORDid);
	Global_id = GlobalDataZYS::LastGlobalid;
	String_Add(&GlobalDataZYS::LastGlobalid);
	cout << "输入昵称\n";
	cin >> Name;
	Birthday.SetBirthday();
	ReGistDate.SetBirthday();
	cout << "输入密码\n";
	Pwd = SetPwd();
	RECORDid = GlobalDataZYS::LastRECORDid;
	String_Add(&GlobalDataZYS::LastRECORDid);
	string Sqlstr = "INSERT INTO USERS (RECORDid,GLOBAL_ID,NAME,BIRTHDAY,REGIST_DATE,PWD)"\
		"VALUES('" + RECORDid + "','" + Global_id + "','" + GBKToUTF8(Name.c_str()) + "','" + Birthday.GetDateString() + "','" + ReGistDate.GetDateString() + "','" + Pwd + "');";
	Exe(Sqlstr);
	CreatMenuMap();
}
//从文件读取通用部分
BaseUserZYS::BaseUserZYS(char **Attrs) :Birthday("生日"), ReGistDate("注册日") //从数据库中初始化
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
		cout << "输入密码" << endl;
		string InPwd = InputPwd();
		if (InPwd == Pwd)
			return 1;
		else
		{
			cout << "你还有" << Try << "次机会" << endl;
			cout << "继续尝试吗(1.继续2.退出)" << endl;
			if (GetOption(1, 2) == 2)
				return 0;
		}
	}
	return 0;
}
//创建好友关系
int BaseUserZYS::CreatFriendRelationship(string Target_Globalid)
{
	string ScureSqt = "SELECT * FROM FRIEND WHERE FROMGB='" + Global_id + "' AND TOGB='" + Target_Globalid + "';";
	if (Exe(ScureSqt) == 1)
	{
		printf("好友关系已存在!\n");
		return 0;
	}
	string SqlStr = "INSERT INTO FRIEND(FROMGB,TOGB,PRODUCTCODE)"\
		"VALUES('" + Global_id + "', '" + Target_Globalid + "', '" + to_string(ProductCode) + "'); ";
	Exe(SqlStr);
	GlobalFriendMap[ProductCode].insert(pair<string, BaseUserZYS*>(Target_Globalid, GlobalDataZYS::UserList[ProductCode][Target_Globalid]));
	return 0;
}
//改名
int BaseUserZYS::SetName(string NewName)
{
	Name = NewName;
	string Sqlstr = "UPDATE USERS SET NAME = '" + NewName + "' where RECORDid = " + RECORDid + ";";
	Exe(Sqlstr);
	return 0;
}
//打印基本信息
int BaseUserZYS::PrintMessage()
{
	printf("%s用户\n昵称:%s\n账号:%s\n生日:%s\n注册日期:%s\n", GlobalDataZYS::Products[ProductCode - 1].c_str(), Name.c_str(), id.c_str(), Birthday.GetDateString().c_str(), ReGistDate.GetDateString().c_str());
	system("pause");
	return 0;
}
//得到全局信息
string BaseUserZYS::GetGlobalid()
{
	return Global_id;
}
//设置密码
string BaseUserZYS::SetPwd()
{
	Pwd = "";//重置密码
	do
	{
		cout << "输入密码\n";
		Pwd = InputPwd();//算法决定PWD不会为空
		cout << "确认密码\n";
		if (Pwd != InputPwd())//确认密码
		{
			cout << "再次输入的密码不正确,重新输入!";
			continue;
		}
		else
		{
			break;
		}
	} while (true);
	OnUpDate();//同步到数据库
	return Pwd;
}
//验证密码
bool BaseUserZYS::CheckPwd(string InputPwd)
{
	if (InputPwd == Pwd)
		return true;
	else
		return false;
}
//更新数据库
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
	AddFunc("登陆到其他服务", [this]() {
		int size = GlobalDataZYS::UserList.size();
		vector<BaseUserZYS*> BindUserList;
		int Ops = 1;
		//在所有产品中寻找这个人
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
		cout << "按对应的id进入服务,按0退出" << endl;
		int Option = GetOption(0, BindUserList.size());
		if (Option == 0)
			return 0;
		else
			BindUserList[Option - 1]->ShowFoos();
		return 0;
	});
	AddFunc("查看基本信息", [this]() {
		PrintMessage();
		return 0;
	});
	AddFunc("修改名字", [this]() {
		system("cls");
		string NewName;
		cout << "输入新名字" << endl;
		cin >> NewName;
		SetName(NewName);
		return 0; });
	AddFunc("好友列表", [this]() {
		Friends.ShowList(this->ProductCode);
		return 0; });
	AddFunc("修改密码", [this]() {
		SetPwd();
		return 0;
	});
	AddFunc("加好友", [this]() {
		system("cls");
		string Target, TOGB;
		cout << "输入对方ID" << endl;
		cin >> Target;

		while (true)
		{
			TOGB = idToGlobalid(ProductCode, Target);
			if (TOGB == "")
			{
				cout << "不存在!,按1继续,按2退出" << endl;
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
	AddFunc("创建群", [this]() {
		CreatGroup();
		return 0;
	});
	AddFunc("从其他服务添加好友", [this]() {
		vector<string> Friends;
		
		return 0 ; });

}

#pragma endregion


#pragma region  WEICHAT
WeChatUserZYS::WeChatUserZYS() :BaseUserZYS()//QQ注册
{
	ProductCode = 2;
	id = GlobalDataZYS::LastWeChatid;
	String_Add(&GlobalDataZYS::LastWeChatid);
	string Sqlstr = "UPDATE USERS SET Id = '" + id + "' where RECORDid = " + RECORDid + ";" + "UPDATE USERS SET PRODUCTCODE = '" + to_string(ProductCode) + "' where RECORDid = " + RECORDid + ";";
	Exe(Sqlstr);

	CreatMenuMap();
}
//从本地读取微信用户
WeChatUserZYS::WeChatUserZYS(char **Attrs) :BaseUserZYS(Attrs)
{
	ProductCode = 2;
	id = Attrs[6];
	if (GlobalDataZYS::LastWeChatid <= Attrs[6])//刷新id
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

//从群中被删除
int WeChatUserZYS::DeledFromGroup() { return 0; }
//改变群权限
int WeChatUserZYS::PermissionChange() { return 0; }

void WeChatUserZYS::CreatMenuMap()
{
	AddFunc("申请加入群聊", [this]() {
		string JoinGroupGlobalid=GetCorrectGroup(ProductCode);
		if (JoinGroupGlobalid == "")
			return 0;
		else
			//TODO 申请加入的功能
			GlobalDataZYS::Groups[ProductCode][JoinGroupGlobalid]->CreatRelationShip(this,"3");//普通群员
		return 0; });
	AddFunc("群列表", [this]() {
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
//从文件读取QQ用户
QQUserZYS::QQUserZYS(char **Attrs) :BaseUserZYS(Attrs)
{
	ProductCode = 1;
	id = Attrs[6];
	if (GlobalDataZYS::LastQQid <= Attrs[6])//刷新id
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

//从群中被删除
int QQUserZYS::DeledFromGroup() { return 0; }
//改变群权限
int QQUserZYS::PermissionChange() { return 0; }
void QQUserZYS::CreatMenuMap()
{
}
#pragma endregion

#pragma endregion


#pragma region 菜单项

void MenuInterface::AddFunc(string FooName, function<int()> Foo)
{
	Menu.insert(pair<string, function<int()>>(FooName, Foo));
	return;
}
int MenuInterface::ShowFoos()
{
	map<string, function<int()>>::iterator iter;
	string *Names = new string[Menu.size() + 1];
	int i = 0;
	for (iter = Menu.begin(); iter != Menu.end(); iter++, i++)
	{
		cout << i << "." << iter->first << endl;
		Names[i] = iter->first;
	}
	//打印函数列表
	while (true)
	{
		system("cls");
		for (int j = 0; j < i; j++) cout << j << "." << Names[j] << endl;
		cout << i << ".退出" << endl;
		//TODO 改一下这里

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


#pragma region 群
//全局id,权限
BaseGroup::BaseGroup()
{
	CreatMenuMap();
}

BaseGroup::BaseGroup(char ** argvs)
{
	Groupid = argvs[1];
	GroupName = argvs[5];
	ProductCode = atoi(argvs[4]);
	CreatRelationShip(GlobalDataZYS::UserList[ProductCode][argvs[2]], argvs[3]);//把这个用户加进去
	CreatMenuMap();
}

//对群来说创建关系的函数,需要确认是否已经存在!但是并没有把数据加入到用户的grouplist里,注意一下
int BaseGroup::CreatRelationShip(BaseUserZYS * Target, string PermissionCode)//这是群的创建好友函数!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!总搞混
{
	//验证是否已经存在关系
	if (List.find(Target->GetGlobalid()) != List.end())
	{
		cout << "已经是群成员了!!!!!!";
		return 0;
	}
	//从数据库中确认
	string ScureSqt = "SELECT * FROM GROUPS WHERE GB='" + Target->GetGlobalid() + "' AND GROUPID='" + Groupid + "';";
	if (Exe(ScureSqt) == 1)
	{
		cout << Target->Name << "已经是群成员了!!\n";
		return 0;
	}
	//TODO:验证是否有这个群 验证个P 这就是群的方法,验证应该加在用户类中
	//若不存在则调用数据库存进去
	string SQL = "INSERT INTO GROUPS(GB,GROUPID,PRODUCTCODE,PERMISSIONCODE,GROUPNAME)"\
		"VALUES('" + Target->GetGlobalid() + "', '" + Groupid + "', '" + to_string(Target->ProductCode) + "','" + PermissionCode + "','"+GlobalDataZYS::Groups[Target->ProductCode][Groupid]->GroupName+"');";
	Exe(SQL);
	//把群插入到用户的列表里
	Target->Groups.List.insert(pair<string, string>(Groupid,PermissionCode));
	//把用户插入到群的列表里
	List.insert(pair<string, string>(Target->GetGlobalid(), PermissionCode));
	return 0;
}
//从数据库获取群成员的函数 注意没有任何检验!
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
		cout << i  << ". " << GlobalDataZYS::UserList[ProductCode][iter->first]->Name <<"  "<< GlobalDataZYS::Permissions[atoi(iter->second.c_str())]<< endl;
	}
	system("pause");
	return 0;
}
//符合返回1,不符合返回0
int BaseGroup::PermissionCheck(BaseUserZYS * User, string MinPermission)
{
	if (this->List[User->GetGlobalid()] <= MinPermission)
		return 1;
	else 
		return 0;
}

void BaseGroup::CreatMenuMap()
{
	AddFunc("群员列表", [this]() {
		ShowList(ProductCode);
		return 0;
	});
}

void BaseGroup::SetPermissionCode(string TargetGB,string NewCode)
{
	//检验一下人数是否超过限制
	auto iterfind =find_if(List.begin(), List.end(), [](pair<string, string> User) {
		if (User.second == "1")
			return 1;
		else
			return 0;
			});
	if (iterfind != List.end())
	{
		cout << "操作失败,不能有两名群主!"<<endl;
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
	cout << "输入想要升为管理的群员序号,用空格分开" << endl;
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
	BaseGroup *Temp = new BaseGroup;
	Temp->Groupid = GlobalDataZYS::LastGroupid;

	cout << "输入群名称" << endl;
	cin >> Temp->GroupName;
	//cout << "输入群类型" << endl;
	//cin>>
	Temp->Groupid = GlobalDataZYS::LastGroupid;
	String_Add(&GlobalDataZYS::LastGroupid);
	GlobalDataZYS::Groups[GroupMaster->ProductCode].insert(pair<string, BaseGroup*>(Temp->Groupid, Temp));
	Temp->ProductCode = GroupMaster->ProductCode;
	Temp->CreatRelationShip(GroupMaster, "1");
	//	GlobalDataZYS::Groups[GroupMaster->ProductCode].insert(pair<string, BaseGroup*>(Temp->Groupid, Temp));//群插入群列表
	//插入数据库
	return Temp;
}
/*WeChatGroupZYS::WeChatGroupZYS(char ** attrs)
{
	Groupid = attrs[1];
	GroupName = attrs[5];
}*/



#pragma endregion


int GroupList::CreatRelationShip(BaseUserZYS * Master, string Target_Globalid)
{
	//TODO: 加入群的操作,此处需要验证一下是否存在群 这个验证应该加在别处
	/*	while(GlobalDataZYS::Groups[Master->ProductCode].find(Target_Globalid) == GlobalDataZYS::Groups[Master->ProductCode].end())
	{
		cout << "不存在群号为" + Target_Globalid + "的群" << "按1重新输入,按2退出"<<endl;
		if (GetOption(1, 2) == 2)
			return 0;
		else
		{
			cout << "输入群号" << endl;
			cin >> Target_Globalid;
		}
	};*/

	GlobalDataZYS::Groups[Master->ProductCode][Target_Globalid]->CreatRelationShip(Master, "3");
	return 0;
}

//读取群员关系
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
	cout << "    群名     群号      状态"<<endl;
	for (i = List.begin(); i != List.end(); i++,Count++)
	{
		cout << Count << "." << GlobalDataZYS::Groups[ProductCode][i->first]->GroupName << "    " << i->first << "  " << GlobalDataZYS::Permissions[atoi(i->second.c_str())] << endl;
		Entrence.push_back(GlobalDataZYS::Groups[ProductCode][i->first]);
	}
	cout << "输入0退出,输入序号进入群"<<endl;
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
				cout << "输入的序号不正确! 按0退出按1重新输入" << endl;
				if (GetOption(0, 1) == 0)
					return 0;
				else continue;
			}
		}
	}
	return 0;
}

WeChatGroupZYS::WeChatGroupZYS() :BaseGroup()
{

}
WeChatGroupZYS::WeChatGroupZYS(char ** argvs):BaseGroup(argvs)
{
}

void WeChatGroupZYS::CreatMenuMap()
{
	//需求:管理员
	if (PermissionCheck(GlobalDataZYS::CurrentUser, "2"))
		AddFunc("踢人", [this]() {

		ShowList(ProductCode);
		return 0;
		});

	if (PermissionCheck(GlobalDataZYS::CurrentUser, "2"))
		AddFunc("设置管理员", [this]() {
		

		return 0;
	});
}
