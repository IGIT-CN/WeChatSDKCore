#pragma once

#define HELLO_API 1

#ifdef HELLO_API  
#define HELLO_API 
#else  
#define HELLO_API extern "C" __declspec(dllimport)  
#endif 


HELLO_API void HookGetFriendList();		//HOOK��ȡ�����б��call
HELLO_API void GetUserListInfo();		   //��ȡ�����б�
HELLO_API void UnHookGetFriendList();	   //ж��HOOK��ȡ�����б��call
HELLO_API void SendUserListInfo();		//���ͺ����б�
HELLO_API void SendTextMessage(wchar_t* wxid, wchar_t* msg);	//�����ı���Ϣ
HELLO_API void SendFileMessage(wchar_t* wxid, wchar_t* filepath);	//�����ļ���Ϣ
HELLO_API void SendImageMessage(wchar_t* wxid, wchar_t* filepath);//����ͼƬ��Ϣ
HELLO_API void DeleteUser(wchar_t* wxid);	//ɾ������
HELLO_API void SendXmlCard(wchar_t* RecverWxid, wchar_t* SendWxid, wchar_t* NickName); //����XML��Ƭ