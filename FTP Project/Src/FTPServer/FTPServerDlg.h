
// FTPServerDlg.h: 헤더 파일
//

#pragma once


// CFTPServerDlg 대화 상자
class CFTPServerDlg : public CDialogEx
{
// 생성입니다.
public:
	CFTPServerDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FTPSERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	SOCKET hListenSocket;
	BOOL bServerRunning = false;
	CListBox m_ltConnectionInfo;
	CListBox m_ltlogMessage;
	afx_msg void OnBnClickedbnstartup();
	afx_msg void OnBnClickedbnshutdown();
	afx_msg void OnEnChangeedmaxconn();
	afx_msg void OnEnChangeedprotnumer();
};
