
// BoxToolDlg.h : ͷ�ļ�
//

#pragma once

#include "Box.h"

// CBoxToolDlg �Ի���
class CBoxToolDlg : public CDialogEx
{
// ����
public:
	CBoxToolDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_BoxTool_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedbtnzl();
	BOOL DirectoryExist(CString Path);
};
