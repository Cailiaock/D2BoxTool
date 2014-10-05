
// BoxToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BoxTool.h"
#include "BoxToolDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CBoxToolDlg 对话框



CBoxToolDlg::CBoxToolDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CBoxToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBoxToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBoxToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(btnZL, &CBoxToolDlg::OnBnClickedbtnzl)
END_MESSAGE_MAP()


// CBoxToolDlg 消息处理程序

BOOL CBoxToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	GetDlgItem(txtSM)->SetWindowTextW(_T("该程序在暗黑1.13版本下测试通过，其他版本未测试！\r\n使用前，请确认暗黑2已经关闭，否则可能导致存档损坏！\r\n程序已自动在“c:\\大箱子备份”中备份了存档。如果遇到错误，可以到该目录下找到对应文件恢复。\r\n有任何问题可通过邮箱联系作者:lhtcym@gmail.com\r\n\r\n使用方法：\r\n打开save文件夹下的_LOD_SharedStashSave.sss，然后点击“整理”。"));
	
	if (!LoadExcel()
		|| !LoadConfig()
		|| !LoadItemType())
	{
		exit(0);
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CBoxToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		if (nID == 61536)
		{
			free(Configs);
			Configs = NULL;

			free(ItemTypes);
			ItemTypes = NULL;

			free(ExcelItems);
			ExcelItems = NULL;

			free(excel_misc);
			excel_misc = NULL;

			free(excel_weapons);
			excel_weapons = NULL;

			free(excel_armor);
			excel_armor = NULL;
		}
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CBoxToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CBoxToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CBoxToolDlg::OnBnClickedbtnzl()
{
	CString sFilePath;
	GetDlgItem(txtFile)->GetWindowTextW(sFilePath);

	if (!PathFileExists(sFilePath))
	{
		MessageBox(_T("找不到文件:" + sFilePath));
		return;
	}

	//备份文件
	CString sFileName;
	int n = sFilePath.GetLength() - sFilePath.ReverseFind('\\') - 1;
	sFileName = sFilePath.Right(n);

	if (!DirectoryExist(_T("c:\\大箱子备份")))
		CreateDirectory(_T("c:\\大箱子备份"), NULL);

	CString bakpath;
	CTime nowtime = CTime::GetCurrentTime();
	bakpath.Format(_T("c:\\大箱子备份\\%d-%d-%d_%d-%d-%d_"), nowtime.GetYear(),
		nowtime.GetMonth(),
		nowtime.GetDay(),
		nowtime.GetHour(),
		nowtime.GetMinute(),
		nowtime.GetSecond());

	CString s = bakpath + sFileName;
	BOOL copyfile = CopyFile(sFilePath, s, false);
	if (!copyfile)
	{
		MessageBox(_T("备份文件失败！"), _T("提示"), 0);
		return;
	}

	TCHAR *tpath = sFilePath.GetBuffer(0);
	int slen = WideCharToMultiByte(CP_ACP, 0, tpath, -1, NULL, 0, NULL, NULL);
	slen++;
	char *path = new char[slen];
	memset(path, 0, slen);
	WideCharToMultiByte(CP_ACP, 0, tpath, -1, path, slen, NULL, NULL);
	sFilePath.ReleaseBuffer();

	int result;
	result = BagClean(path);
	if (result == 0)
		MessageBox(_T("整理成功！"), _T("提示"), 0);
}

BOOL CBoxToolDlg::DirectoryExist(CString Path)
{
	WIN32_FIND_DATA fd;
	BOOL ret = FALSE;
	HANDLE hFind = FindFirstFile(Path, &fd);
	if ((hFind != INVALID_HANDLE_VALUE) && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{  //目录存在
		ret = TRUE;
	}
	FindClose(hFind);
	return ret;
}