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
class Date
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
class BaseUser {
public:

	map<string, int> GlobalFriendMap;//ȫ΢Xͨ�õĺ����б�,��������:<Globalid,ProductCode>
	map<string, string> FriendMap;//�����û��ĺ����б� ��������: <Globalid,ProductCode>
	map<string, int> GroupMap;//����Ӧ�õ�Ⱥ�б�

	static string LastGlobalid;//����ȫ��id
	string RECORDid;//���û������ݿ��Ӧ��Ψһid
	string Name;//�ǳ�
	string id;//�ֱ𵽸�����Ʒ�ĺ� ����QQ��
	int ProductCode;//��ʾ���û����ڵİ汾(�˴���Ҫ��;����̳�)
	Date Birthday;
	
	Date ReGistDate;


	//����ģʽ  ,  �ڻ����в�ִ���ļ����� 
	BaseUser();
	//�����ݿ��г�ʼ��
	BaseUser(char **Attrs);
	//����������
	virtual int SetName(string NewName)=0;
	//����½
	virtual int LoginCheck()=0;
	//��Ⱥ�б�ɾ��
	virtual int DeledFromGroup()=0;
	//�ı�ȺȨ��
	virtual int PermissionChange()=0;
	//�������ѹ�ϵ
	int CreatFriendRelationship(string Target_Globalid);


private:
	string Global_id;//ȫ��ID
	Date SignDay;//ע����(��������X��)
};
class WeChatUser :public virtual BaseUser
{
public:
	//static vector<int> FriendProductList;
	static string LastWeChatid;
	//�����洢�Ĺ���̬��Ա
	
	
	//΢��ע��
	WeChatUser();
	//�����ݿ��ȡ΢���û�
	WeChatUser(char **Attrs);
	//����������
	virtual int SetName(string NewName);
	//����½
	virtual int LoginCheck();
	//��Ⱥ�б�ɾ��
	virtual int DeledFromGroup();
	//�ı�ȺȨ��
	virtual int PermissionChange();
};
class QQUser :public virtual BaseUser
{
public:
	//static vector<int> FriendProductList;
	static string LastQQid;
	//�����洢�Ĺ���̬��Ա
	QQUser();//QQע��
	QQUser(char **Attrs);
	//����������
	virtual int SetName(string NewName);
	//����½
	virtual int LoginCheck();
	//��Ⱥ�б�ɾ��
	virtual int DeledFromGroup();
	//�ı�ȺȨ��
	virtual int PermissionChange();
};