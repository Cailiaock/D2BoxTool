
// BoxToolDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BoxTool.h"
#include "BoxToolDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
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


// CBoxToolDlg �Ի���



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


// CBoxToolDlg ��Ϣ�������

BOOL CBoxToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	GetDlgItem(txtSM)->SetWindowTextW(_T("�ó����ڰ���1.13�汾�²���ͨ���������汾δ���ԣ�\r\nʹ��ǰ����ȷ�ϰ���2�Ѿ��رգ�������ܵ��´浵�𻵣�\r\n�������Զ��ڡ�c:\\�����ӱ��ݡ��б����˴浵������������󣬿��Ե���Ŀ¼���ҵ���Ӧ�ļ��ָ���\r\n���κ������ͨ��������ϵ����:lhtcym@gmail.com\r\n\r\nʹ�÷�����\r\n��save�ļ����µ�_LOD_SharedStashSave.sss��Ȼ������������"));
	
	if (!LoadExcel()
		|| !LoadConfig()
		|| !LoadItemType())
	{
		exit(0);
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CBoxToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
		MessageBox(_T("�Ҳ����ļ�:" + sFilePath));
		return;
	}

	//�����ļ�
	CString sFileName;
	int n = sFilePath.GetLength() - sFilePath.ReverseFind('\\') - 1;
	sFileName = sFilePath.Right(n);

	if (!DirectoryExist(_T("c:\\�����ӱ���")))
		CreateDirectory(_T("c:\\�����ӱ���"), NULL);

	CString bakpath;
	CTime nowtime = CTime::GetCurrentTime();
	bakpath.Format(_T("c:\\�����ӱ���\\%d-%d-%d_%d-%d-%d_"), nowtime.GetYear(),
		nowtime.GetMonth(),
		nowtime.GetDay(),
		nowtime.GetHour(),
		nowtime.GetMinute(),
		nowtime.GetSecond());

	CString s = bakpath + sFileName;
	BOOL copyfile = CopyFile(sFilePath, s, false);
	if (!copyfile)
	{
		MessageBox(_T("�����ļ�ʧ�ܣ�"), _T("��ʾ"), 0);
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
		MessageBox(_T("����ɹ���"), _T("��ʾ"), 0);
}

BOOL CBoxToolDlg::DirectoryExist(CString Path)
{
	WIN32_FIND_DATA fd;
	BOOL ret = FALSE;
	HANDLE hFind = FindFirstFile(Path, &fd);
	if ((hFind != INVALID_HANDLE_VALUE) && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{  //Ŀ¼����
		ret = TRUE;
	}
	FindClose(hFind);
	return ret;
}