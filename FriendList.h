#pragma once
#include <list>
#include <tuple>
#include <string>
#include <stdio.h>

using namespace std;

typedef tuple <
	//wxid1
	wstring,
	//wxName
	wstring,
	//v1
	wstring,
	//nickName
	wstring
> USER_INFO;

void HookGetFriendList();		//HOOK��ȡ�����б��call
void GetUserListInfo();		   //��ȡ�����б�
void UnHookGetFriendList();	   //ж��HOOK��ȡ�����б��call
void SaveUserListInfo();		//���ͺ����б�
void SendTextMessage(wchar_t* wxid, wchar_t* msg);	//�����ı���Ϣ
void SendFileMessage(wchar_t* wxid, wchar_t* filepath);	//�����ļ���Ϣ
void SendImageMessage(wchar_t* wxid, wchar_t* filepath);//����ͼƬ��Ϣ
void DeleteUser(wchar_t* wxid);	//ɾ������
void SendXmlCard(wchar_t* RecverWxid, wchar_t* SendWxid, wchar_t* NickName); //����XML��Ƭ
string GetUserInfoList();// ��ȡ������Ϣ