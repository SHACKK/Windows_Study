#pragma once


// LoginDlg 대화 상자

class CLoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CLoginDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
public:
	SOCKET hSocket;
	int nConnectResult = INVALID_SOCKET;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedbnlogin();
	afx_msg void OnBnClickedbncancel();
	CEdit m_edUserID;
	CEdit m_edUserPW;
	virtual void OnCancel();
	virtual void OnOK();
};