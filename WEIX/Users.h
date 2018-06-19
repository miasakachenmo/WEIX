#define _CRT_SECURE_NO_WARNINGS
#pragma  once 
#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<map>
#include<sstream>
#include<functional>
#include "sqlite3.h"
#include "Func.h"
using namespace std;
#pragma region ������
class DateZYS
{
public:
	static int MonthDay[12];
	static int LeapMonthDay[12];
	//�����ж������Ƿ���ȷ�ľ�̬��Ա

	int Year;
	int Month;
	int Day;


	string DateName;//���ڵ����� �������� ע����
	DateZYS();
	DateZYS(string Name);
	void SetFromString(string a);
	void SetBirthday();
	string GetDateString();
};
class BaseUserZYS;
class BaseGroup;
class GlobalDataZYS
{
public:
	static string LastRECORDid;
	static string LastGlobalid;//����ȫ��id
	static string LastQQid;
	static string LastWeChatid;
	static string LastGroupid;
	static vector<string> Products;
	static map<int, map<string, BaseUserZYS*> > UserList;
	static map<string, BaseUserZYS*> QQUserList;
	static map<string, BaseUserZYS*> WeChatUserList;
	static map<int, map<string, BaseGroup*>> Groups;//Ⱥ��
	static string CurrentUser;
	static string Permissions[];
	static map<string, function<BaseGroup*(char **argv)>> GroupFactory;//GroupType,���� Ⱥ����
	static map<int, function<int(BaseGroup* Group)>> GroupDecorater;//GroupType,���� Ⱥװ����
	static string GroupTypeNames[];
};

#pragma endregion

#pragma region �б�ӿ�
//�ӿں���
class ListOpertion
{

public:
	map<string, string> List;
	virtual int CreatRelationShip(BaseUserZYS* Master, string Target_Globalid) = 0;
	virtual int GetRelationShip(BaseUserZYS* Master, string Target_Globalid) = 0;
	virtual int ShowList(int ProductCode) = 0;
};
class FriendList :public virtual ListOpertion
{
	//����:<GB,����>
public:
	virtual int CreatRelationShip(BaseUserZYS* Master, string Target_Globalid);
	virtual int GetRelationShip(BaseUserZYS* Master, string Target_Globalid);
	virtual int ShowList(int ProductCode);
};
class GroupList :public virtual ListOpertion
{
	//Ⱥ�� Ȩ��
public:
	virtual int CreatRelationShip(BaseUserZYS* Master, string Target_Globalid);
	virtual int GetRelationShip(BaseUserZYS* Master, string Target_Globalid);
	virtual int ShowList(int ProductCode);
};
#pragma endregion

#pragma region �󶨽ӿ�
class IBindWithQQUserZYS
{
public:
	virtual int BindTo(BaseUserZYS* Master, string Taget_Globalid) = 0;
};
class IWeakBindWithQQUserZYS :virtual public IBindWithQQUserZYS
{
public:
	virtual int BindTo(BaseUserZYS* Master, string Taget_Globalid);
};
class IStrongBindWithQQUserZYS :virtual public IBindWithQQUserZYS
{
public:
	virtual int BindTo(BaseUserZYS* Master, string Taget_Globalid);
};
#pragma endregion

#pragma region �˵������ӿ�
class MenuInterface
{
public:
	map<string, function<int()>> Menu;//��������,����
	void AddFunc(string FooName,function<int()> Foo);//��Ӻ���
	virtual void CreatMenuMap()=0;
	int ShowFoos();
};
#pragma endregion

#pragma region �û�
class BaseUserZYS :virtual public IBindWithQQUserZYS,virtual public MenuInterface
{
public:

	map<int, map<string, BaseUserZYS*>> GlobalFriendMap;//ȫ΢Xͨ�õĺ����б�,��������:<productcode,<globaoid>>
	map<string, int> GroupMap;//����Ӧ�õ�Ⱥ�б�

	string RECORDid;//���û������ݿ��Ӧ��Ψһid
	string Name;//�ǳ�
	string id;//�ֱ𵽸�����Ʒ�ĺ� ����QQ��

	int ProductCode;//��ʾ���û����ڵİ汾(�˴���Ҫ��;����̳�)
	DateZYS Birthday;
	DateZYS ReGistDate;

	FriendList Friends;
	GroupList Groups;

	//����ģʽ  ,  �ڻ����в�ִ���ļ����� 
	BaseUserZYS();
	//�����ݿ��г�ʼ��
	BaseUserZYS(char **Attrs);
	//����½ �ɹ�����1 ʧ�ܷ���0
	int LoginCheck();
	//��Ⱥ�б�ɾ��
	virtual int DeledFromGroup() = 0;
	//�ı�ȺȨ��

	//����Ⱥ
	virtual void CreatGroup()=0;
	//��
	virtual int PermissionChange() = 0;
	//��ӡ��Ϣ
	int PrintMessage();
	//�������ѹ�ϵ
	int CreatFriendRelationship(string Target_Globalid);
	//����������
	int SetName(string NewName);
	//��������,����ֵΪ������,����ʱ�Ѿ����ú���
	string SetPwd();
	//�õ�����
	bool CheckPwd(string InputPwd);
	//�õ�ȫ��id(��ȫ��id�ı���)
	string GetGlobalid();
	//���ݸ���ʱ
	int OnUpDate();

	//�ӿں���
	virtual void CreatMenuMap();

	//��Ԫ����
	friend int CreatUserView();

	friend class IStrongBindWithQQUserZYS;
	friend class IWeakBindWithQQUserZYS;
private:
	string Global_id;//ȫ��ID
	string Pwd;
	DateZYS SignDay;//ע����(��������X��)
};
class WeChatUserZYS :public virtual BaseUserZYS, virtual public IWeakBindWithQQUserZYS
{
public:
	//static vector<int> FriendProductList;
	//�����洢�Ĺ���̬��Ա


	//΢��ע��
	WeChatUserZYS();
	//�����ݿ��ȡ΢���û�
	WeChatUserZYS(char **Attrs);
	//����Ⱥ���麯��
	virtual void CreatGroup();



	//��Ⱥ�б�ɾ��
	virtual int DeledFromGroup();
	//�ı�ȺȨ��
	virtual int PermissionChange();
	//�˵�
	virtual void CreatMenuMap();

};
class QQUserZYS :public virtual BaseUserZYS,public virtual IStrongBindWithQQUserZYS
{
public:
	//static vector<int> FriendProductList;
	//�����洢�Ĺ���̬��Ա
	QQUserZYS();//QQע��
	QQUserZYS(char **Attrs);

	virtual void CreatGroup();
	//��Ⱥ�б�ɾ��
	virtual int DeledFromGroup();
	//�ı�ȺȨ��
	virtual int PermissionChange();
	//�����˵�
	virtual void CreatMenuMap();
};
#pragma endregion 

#pragma region Ⱥ��
//�����
class BaseGroup: virtual public FriendList,virtual public MenuInterface
{
public:
	//list�ĺ���<GB,permissioncode>
	BaseGroup();
	BaseGroup(char **argvs);
	string Groupid;
	string GroupName;
	string GroupType;
	int ProductCode;
	virtual int CreatRelationShip(BaseUserZYS *Target, string PermissionCode);
	virtual int GetRelationShip(BaseUserZYS *Target, string PermissionCode);
	virtual int ShowList(int ProductCode);
	virtual int PermissionCheck(string UserGB,string MinPermission);
	virtual void CreatMenuMap();
	virtual void SetPermissionCode(string TargetGB,string NewCode);
	virtual void SetAdm();
	static BaseGroup* CreatGroup(BaseUserZYS* GroupMaster);
	
};
class  WeChatGroupZYS:virtual public BaseGroup,virtual public MenuInterface
{
public:
	WeChatGroupZYS();
	WeChatGroupZYS(char **argvs);
	virtual void CreatMenuMap();
	//��ȡȺ
private:
	
	
};
#pragma endregion 