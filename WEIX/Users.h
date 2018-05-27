#define _CRT_SECURE_NO_WARNINGS
#pragma  once 
#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<map>
#include<sstream>
#include "sqlite3.h"
#include "Func.h"
using namespace std;
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
//�ӿں���
class BaseUserZYS;
class ListOpertion
{
public:
	map<int, map<string, string>> List;
	virtual int CreatRelationShip(BaseUserZYS* Master, string Target_Globalid) = 0;
	virtual int GetRelationShip(BaseUserZYS* Master, string Target_Globalid) = 0;
	virtual int ShowList(BaseUserZYS* Master, string Target_Globalid) = 0;
};
class FriendList :public virtual ListOpertion
{
public:
	virtual int CreatRelationShip(BaseUserZYS* Master, string Target_Globalid);
	virtual int GetRelationShip(BaseUserZYS* Master, string Target_Globalid);
	virtual int ShowList(BaseUserZYS* Master, string Target_Globalid);
};
class GroupList :public virtual ListOpertion
{
public:
	virtual int CreatRelationShip(BaseUserZYS* Master, string Target_Globalid);
	virtual int GetRelationShip(BaseUserZYS* Master, string Target_Globalid);
	virtual int ShowList(BaseUserZYS* Master, string Target_Globalid);
};


class GlobalDataZYS
{
	static string LastGlobalid;//����ȫ��id
	static string LastQQid;
	static string LastWeChatid;
	static map<int, map<string, BaseUserZYS*> > UserList;
};

class BaseUserZYS {
public:

	map<int,map<string, BaseUserZYS*>> GlobalFriendMap;//ȫ΢Xͨ�õĺ����б�,��������:<productcode,<globaoid>>
	map<string, int> GroupMap;//����Ӧ�õ�Ⱥ�б�

	static string LastGlobalid;//����ȫ��id
	string RECORDid;//���û������ݿ��Ӧ��Ψһid
	string Name;//�ǳ�
	string id;//�ֱ𵽸�����Ʒ�ĺ� ����QQ��
	int ProductCode;//��ʾ���û����ڵİ汾(�˴���Ҫ��;����̳�)
	DateZYS Birthday;
	DateZYS ReGistDate;

	FriendList Friends;

	//����ģʽ  ,  �ڻ����в�ִ���ļ����� 
	BaseUserZYS();
	//�����ݿ��г�ʼ��
	BaseUserZYS(char **Attrs);
	//����½
	virtual int LoginCheck()=0;
	//��Ⱥ�б�ɾ��
	virtual int DeledFromGroup()=0;
	//�ı�ȺȨ��
	virtual int PermissionChange()=0;
	//��ӡ��Ϣ
	int PrintMessage();
	//�������ѹ�ϵ
	int CreatFriendRelationship(string Target_Globalid);
	//����������
	int SetName(string NewName);
	//�õ�ȫ��id(��ȫ��id�ı���)
	string GetGlobalid();
	//��Ԫ����
	friend void CreatUserView();
private:
	string Global_id;//ȫ��ID
	DateZYS SignDay;//ע����(��������X��)
};
class WeChatUserZYS :public virtual BaseUserZYS
{
public:
	//static vector<int> FriendProductList;
	static string LastWeChatid;
	//�����洢�Ĺ���̬��Ա
	
	
	//΢��ע��
	WeChatUserZYS();
	//�����ݿ��ȡ΢���û�
	WeChatUserZYS(char **Attrs);

	//����½
	virtual int LoginCheck();
	//��Ⱥ�б�ɾ��
	virtual int DeledFromGroup();
	//�ı�ȺȨ��
	virtual int PermissionChange();
};
class QQUserZYS :public virtual BaseUserZYS
{
public:
	//static vector<int> FriendProductList;
	static string LastQQid;
	//�����洢�Ĺ���̬��Ա
	QQUserZYS();//QQע��
	QQUserZYS(char **Attrs);

	//����½
	virtual int LoginCheck();
	//��Ⱥ�б�ɾ��
	virtual int DeledFromGroup();
	//�ı�ȺȨ��
	virtual int PermissionChange();
};