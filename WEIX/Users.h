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
	int Year;
	int Month;
	int Day;
	void SetFromString(string a);
	void SetBirthday();
	string GetDateString();
};
class BaseUserZYS {
public:

	map<int,map<string, BaseUserZYS*>> GlobalFriendMap;//ȫ΢Xͨ�õĺ����б�,��������:<Globalid,ProductCode>
	map<string, int> GroupMap;//����Ӧ�õ�Ⱥ�б�

	static string LastGlobalid;//����ȫ��id
	string RECORDid;//���û������ݿ��Ӧ��Ψһid
	string Name;//�ǳ�
	string id;//�ֱ𵽸�����Ʒ�ĺ� ����QQ��
	int ProductCode;//��ʾ���û����ڵİ汾(�˴���Ҫ��;����̳�)
	DateZYS Birthday;
	
	DateZYS ReGistDate;


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
	//��ӡ��Ϣ
};