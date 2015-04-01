
// mfcDlg.cpp : ŽÀ‘•ƒtƒ@ƒCƒ‹
//

#include "stdafx.h"
#include "mfc.h"
#include "mfcDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CmfcDlg::CmfcDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CmfcDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CmfcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE, m_StaticPicture);
}

BEGIN_MESSAGE_MAP(CmfcDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
END_MESSAGE_MAP()


BOOL CmfcDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	m_nFrameCount = 0;
	SetTimer(1, 250, NULL);

	return TRUE;
}

void CmfcDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CmfcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CmfcDlg::OnTimer(UINT_PTR nIDEvent)
{
	CDC *pdc = m_StaticPicture.GetDC();

	// setup pen & brush
	CBrush brush_b;
	brush_b.CreateSolidBrush(RGB(0, 0, 0));

	CRect rect_b(0, 0, 200, 200);
	pdc->FillRect(&rect_b, &brush_b);

	CBrush brush_f;
	switch (m_nFrameCount % 4) {
	case 0:
		brush_f.CreateSolidBrush(RGB(255, 0, 0));
		break;
	case 1:
		brush_f.CreateSolidBrush(RGB(0, 255, 0));
		break;
	case 2:
		brush_f.CreateSolidBrush(RGB(0, 0, 255));
		break;
	case 3:
		brush_f.CreateSolidBrush(RGB(0, 0, 0));
		break;
	}

	CRect rect_f(100, 100, 101, 101);
	pdc->FillRect(&rect_f, &brush_f);

	// cleanup objests...
	brush_f.DeleteObject();
	brush_b.DeleteObject();
	m_StaticPicture.ReleaseDC(pdc);

	m_nFrameCount++;

	CDialogEx::OnTimer(nIDEvent);
}
