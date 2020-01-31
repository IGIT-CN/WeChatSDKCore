#include "stdafx.h"
#include "Login.h"
#include <stdio.h>
#include "offset.h"
#include <strstream>


//���ݵ�ԭ����
BYTE backCode[5] = { 0 };
BYTE jmpCode[5] = { 0 };

DWORD pEcx = 0;		//ecx�Ĵ���
DWORD dwRetAddr = (DWORD)GetModuleHandle(L"WeChatWin.dll") + QrCodeOffset + 5;	//���ص�ַ


//************************************************************
// ��������: HookQrCode
// ����˵��: HOOK΢�Ŷ�ά��
// ��    ��: GuiShou
// ʱ    ��: 2019/6/30
// ��    ��: DWORD dwHookOffset Ҫhook��ƫ�� 
// �� �� ֵ: void
//************************************************************
void HookQrCode(DWORD dwHookOffset)
{
	//�õ�ģ���ַ
	DWORD dwWeChatWinAddr = (DWORD)GetModuleHandle(L"WeChatWin.dll");

	//������ҪHOOK�ĵ�ַ
	DWORD dwHookAddr = dwWeChatWinAddr + dwHookOffset;

	//�������
	jmpCode[0] = 0xE9;
	//����ƫ��
	*(DWORD*)(&jmpCode[1]) = (DWORD)ShowPic - dwHookAddr - 5;

	// ������ǰ���������ڻ�ԭ
	DWORD OldProtext = 0;

	// ��ΪҪ�������д�����ݣ�����Ϊ������ǲ���д�ģ�������Ҫ�޸�����
	VirtualProtect((LPVOID)dwHookAddr, 5, PAGE_EXECUTE_READWRITE, &OldProtext);

	//����ԭ�е�ָ��
	memcpy(backCode, (void*)dwHookAddr, 5);

	//д���Լ��Ĵ���
	memcpy((void*)dwHookAddr, jmpCode, 5);

	// ִ�����˲���֮����Ҫ���л�ԭ
	VirtualProtect((LPVOID)dwHookAddr, 5, OldProtext, &OldProtext);
}


//************************************************************
// ��������: ShowPic
// ����˵��: ��ʾ��ά��
// ��    ��: GuiShou
// ʱ    ��: 2019/6/16
// ��    ��: void
// �� �� ֵ: void
//************************************************************
void  __declspec(naked) ShowPic()
{
	//���ݼĴ���
	__asm pushad;
	__asm pushfd;
	//ȡ��ecx������
	__asm mov pEcx, ecx;
	SaveImg(pEcx);
	//�ָ��Ĵ���
	__asm popfd;
	__asm popad;

	//��ת�����ص�ַ
	__asm jmp dwRetAddr;
}


//************************************************************
// ��������: SaveImg
// ����˵��: �����ά��ͼƬ
// ��    ��: GuiShou
// ʱ    ��: 2019/6/30
// ��    ��: DWORD qrcode ��ά��ͼƬ���ڵĵ�ַ
// �� �� ֵ: void
//************************************************************
void SaveImg(DWORD qrcode)
{
	//��ȡͼƬ����
	DWORD dwPicLen = qrcode + 0x4;
	size_t cpyLen = (size_t)*((LPVOID*)dwPicLen);
	//����ͼƬ������
	char PicData[0xFFF] = { 0 };
	memcpy(PicData, *((LPVOID*)qrcode), cpyLen);

	char szTempPath[MAX_PATH] = { 0 };
	char szPicturePath[MAX_PATH] = { 0 };
	GetTempPathA(MAX_PATH, szTempPath);

	sprintf_s(szPicturePath, "%s%s", szTempPath, "qrcode.png");
	//���ļ�д��TempĿ¼��
	HANDLE hFile = CreateFileA(szPicturePath, GENERIC_ALL, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == NULL)
	{
		MessageBoxA(NULL, "����ͼƬ�ļ�ʧ��", "����", 0);
		return;
	}

	DWORD dwRead = 0;
	if (WriteFile(hFile, PicData, cpyLen, &dwRead, NULL) == 0)
	{
		MessageBoxA(NULL, "д��ͼƬ�ļ�ʧ��", "����", 0);
		return;
	}
	CloseHandle(hFile);
	
	//���֮��ж��HOOK
	UnHookQrCode(QrCodeOffset);

}


//************************************************************
// ��������: UnHook
// ����˵��: ж��HOOK
// ��    ��: GuiShou
// ʱ    ��: 2019/6/16
// ��    ��: void
// �� �� ֵ: void
//************************************************************
void UnHookQrCode(DWORD dwHookOffset)
{
	//�õ�ģ���ַ
	DWORD dwWeChatWinAddr = (DWORD)GetModuleHandle(L"WeChatWin.dll");

	DWORD dwHookAddr = dwWeChatWinAddr + dwHookOffset;

	// ������ǰ���������ڻ�ԭ
	DWORD OldProtext = 0;

	// ��ΪҪ�������д�����ݣ�����Ϊ������ǲ���д�ģ�������Ҫ�޸�����
	VirtualProtect((LPVOID*)dwHookAddr, 5, PAGE_EXECUTE_READWRITE, &OldProtext);

	// Hook ����������д���Լ��Ĵ���
	memcpy((LPVOID*)dwHookAddr, backCode, 5);

	// ִ�����˲���֮����Ҫ���л�ԭ
	VirtualProtect((LPVOID*)dwHookAddr, 5, OldProtext, &OldProtext);
}


//************************************************************
// ��������: GotoQrCode
// ����˵��: ��ת����ά�봰��
// ��    ��: GuiShou
// ʱ    ��: 2019/6/16
// ��    ��: void
// �� �� ֵ: void
//************************************************************
void GotoQrCode()
{
	DWORD dwBaseAddress = (DWORD)GetModuleHandle(L"WeChatWin.dll");
	DWORD dwCallAddr1 = dwBaseAddress + GoToQrCode1;
	DWORD dwCallAddr2 = dwBaseAddress + GoToQrCode2;

	__asm {
		call dwCallAddr1;
		mov ecx, eax;
		call dwCallAddr2;
	}
}


/*ע����½*/
void LogoutWeChat()
{
	DWORD dwBaseAddress = (DWORD)GetModuleHandle(L"WeChatWin.dll");
	DWORD dwCallAddress = dwBaseAddress + Logout;
	HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)dwCallAddress, 0, NULL, 0);
	if (hThread != 0)
	{
		CloseHandle(hThread);
	}
}

//************************************************************
// ��������: IsWxVersionValid
// ����˵��: ���΢�Ű汾�Ƿ�ƥ��
// ��    ��: GuiShou
// ʱ    ��: 2019/6/30
// ��    ��: void
// �� �� ֵ: BOOL
//************************************************************
BOOL IsWxVersionValid()
{
	DWORD wxBaseAddress = (DWORD)GetModuleHandle(TEXT("WeChatWin.dll"));
	const string wxVersoin = "2.6.8.52";

	WCHAR VersionFilePath[MAX_PATH];
	if (GetModuleFileName((HMODULE)wxBaseAddress, VersionFilePath, MAX_PATH) == 0)
	{
		return FALSE;
	}

	string asVer = "";
	VS_FIXEDFILEINFO* pVsInfo;
	unsigned int iFileInfoSize = sizeof(VS_FIXEDFILEINFO);
	int iVerInfoSize = GetFileVersionInfoSize(VersionFilePath, NULL);
	if (iVerInfoSize != 0) {
		char* pBuf = new char[iVerInfoSize];
		if (GetFileVersionInfo(VersionFilePath, 0, iVerInfoSize, pBuf)) {
			if (VerQueryValue(pBuf, TEXT("\\"), (void**)&pVsInfo, &iFileInfoSize)) {
				//���汾2.6.7.57
				//2
				int s_major_ver = (pVsInfo->dwFileVersionMS >> 16) & 0x0000FFFF;
				//6
				int s_minor_ver = pVsInfo->dwFileVersionMS & 0x0000FFFF;
				//7
				int s_build_num = (pVsInfo->dwFileVersionLS >> 16) & 0x0000FFFF;
				//57
				int s_revision_num = pVsInfo->dwFileVersionLS & 0x0000FFFF;

				//�Ѱ汾����ַ���
				strstream wxVer;
				wxVer << s_major_ver << "." << s_minor_ver << "." << s_build_num << "." << s_revision_num;
				wxVer >> asVer;
			}
		}
		delete[] pBuf;
	}

	//�汾ƥ��
	if (asVer == wxVersoin)
	{
		return TRUE;
	}

	//�汾��ƥ��
	return FALSE;
}


//************************************************************
// ��������: CheckIsLogin
// ����˵��: ���΢���Ƿ��½
// ��    ��: GuiShou
// ʱ    ��: 2019/6/30
// ��    ��: void
// �� �� ֵ: void
//************************************************************
int CheckIsLogin()
{
	//��ȡWeChatWin�Ļ�ַ
	DWORD  dwWeChatWinAddr = (DWORD)GetModuleHandle(L"WeChatWin.dll");

	DWORD dwIsLogin = dwWeChatWinAddr + LoginSign_Offset + 0x194;
	if (*(DWORD*)dwIsLogin != 0)
	{
		//���ҵ�½���ھ��
		HWND hLogin = FindWindow(NULL, L"Login");
		if (hLogin == NULL)
		{
			return 2;
		}
		return 1;
	}
	return 0;
}