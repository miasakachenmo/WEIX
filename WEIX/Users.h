#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<map>
#include<tchar.h>
#include "sqlite3.h"
using namespace std;
class Date
{
public:
	static int MonthDay[12];
	static int LeapMonthDay[12];
	int Year;
	int Month;
	int Day;
	void SetBirthday()
	{
		int *MonthDayLimit;
		do
		{
			cout << "����������:"<<endl;
			cin >> Year;
			cout << "��������·�:"<<endl;
			cin >> Month;
			cout << "���������:"<<endl;
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
		} while (!(Year > 0 && Year <= 2018 && Month > 0 && Month <= 12&&Day>0&&Day<=MonthDayLimit[Month-1]));
	}
};
class BaseUser {
public:
	string Name;//�ǳ�
	string id;//�ֱ𵽸�����Ʒ�ĺ� ����QQ��
	int ProductCode;//��ʾ���û����ڵİ汾(�˴���Ҫ��;����̳�)
	Date Birthday;

	BaseUser()//����ģʽ  ,  �ڻ����в�ִ���ļ����� 
	{

		cout << "�����ǳ�";
		cin >> Name;
		Birthday.SetBirthday();
	}
	map<string, int> GlobalFriendMap;//ȫ΢Xͨ�õĺ����б�
	map<string, string> FriendMap;//����Ӧ�õĺ����б�
	map<string, int> GroupMap;//����Ӧ�õ�Ⱥ�б�
	virtual int SetName(string NewName) = 0;//����������
	virtual int LoginCheck() = 0;//����½
	virtual int DeledFromGroup() = 0;//��Ⱥ�б�ɾ��
	virtual int PermissionChange() = 0;//�ı�ȺȨ��
	virtual int WriteToFile()=0;//д���ļ�
	virtual int ReadFromFile() = 0;//��ȡ�ļ�
private:
	string Global_id;//ȫ��ID
	Date SignDay;//ע����(��������X��)
};
class QQUser:virtual BaseUser
{
public:
	static map<string, QQUser*> UserList;
	static vector<int> FriendProductList;
	static int LastQQid;
	//�����洢�Ĺ���̬��Ա
	string QQid;
	QQUser() :BaseUser()//QQע��
	{
		QQid = LastQQid;
		LastQQid++;
		UserList.insert(pair<string,QQUser*>(QQid, this));//����QQȫ���û�
	}

};