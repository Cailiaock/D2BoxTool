
// BoxTool.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CBoxToolApp: 
// �йش����ʵ�֣������ BoxTool.cpp
//

class CBoxToolApp : public CWinApp
{
public:
	CBoxToolApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CBoxToolApp theApp;
