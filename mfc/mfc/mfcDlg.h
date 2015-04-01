#pragma once
#include "afxwin.h"

class CmfcDlg : public CDialogEx
{
public:
	CmfcDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_MFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	HICON m_hIcon;

	DWORD  m_nFrameCount;

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_StaticPicture;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
