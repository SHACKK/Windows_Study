
// FTPServerDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "FTPServer.h"
#include "FTPServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFTPServerDlg 대화 상자



CFTPServerDlg::CFTPServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FTPSERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFTPServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ltConnectionInfo);
	DDX_Control(pDX, IDC_LIST2, m_ltlogMessage);
}

BEGIN_MESSAGE_MAP(CFTPServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_bnStartUp, &CFTPServerDlg::OnBnClickedbnstartup)
	ON_BN_CLICKED(IDC_bnShutDown, &CFTPServerDlg::OnBnClickedbnshutdown)
END_MESSAGE_MAP()


// CFTPServerDlg 메시지 처리기

BOOL CFTPServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	GetWindowRect(&m_rtWindow);
	SetWindowPos(nullptr, 0, 0, m_rtWindow.Width(), 140, SWP_NOMOVE | SWP_NOZORDER);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CFTPServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CFTPServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CFTPServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

UINT CFTPServerDlg::AccpetThread(LPVOID pContext)
{
	CFTPServerDlg& dlg = *(CFTPServerDlg*)pContext;
	sockaddr RemoteInfo;
	int nRemoteInfoSize = (int)sizeof(RemoteInfo);
	dlg.hConnectionSocket = ::accept(dlg.hListenSocket, &RemoteInfo, &nRemoteInfoSize);
	return 0;
}

void CFTPServerDlg::OnBnClickedbnstartup()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString strPortNumber, strMaxConn;
	GetDlgItemText(IDC_edProtNumer, strPortNumber);
	GetDlgItemText(IDC_edMaxConn, strMaxConn);

	if (strPortNumber.IsEmpty())
	{
		MessageBox(TEXT("포트 번호를 입력하세요"), TEXT("알림"), MB_OK);
		return;
	}

	if (strMaxConn.IsEmpty())
	{
		MessageBox(TEXT("최대 연결수를 입력하세요"), TEXT("알림"), MB_OK);
		return;
	}

	if (bServerRunning)
	{
		MessageBox(TEXT("Server is already running"), TEXT("Warnning"), MB_ICONWARNING);
		return;
	}
	else
	{
		SetWindowPos(nullptr, 0, 0, m_rtWindow.Width(), m_rtWindow.Height(), SWP_NOZORDER | SWP_NOMOVE);

		bServerRunning = true;
		m_ltlogMessage.AddString(TEXT("[INFO] SERVER STARTUP"));

		////////////////////////////////// socket ////////////////////////////////// 
		hListenSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (hListenSocket != INVALID_SOCKET)	m_ltlogMessage.AddString(TEXT("[SUCCESS] SOCKET CREATE"));
		else	m_ltlogMessage.AddString(TEXT("[ERROR] SOCKET CREATE"));

		/////////////////////////////////// bind /////////////////////////////////// 
		sockaddr_in service;
		service.sin_family = AF_INET;
		inet_pton(AF_INET, "127.0.0.1", &(service.sin_addr.s_addr));
		service.sin_port = htons(56000);
		int nRet = ::bind(hListenSocket, (sockaddr*)&service, (int)sizeof(service));
		if (nRet == SOCKET_ERROR)
		{
			m_ltlogMessage.AddString(TEXT("[ERROR] SOCKET BIND\n[ERROR CODE] %d", WSAGetLastError()));
			return;
		}
		m_ltlogMessage.AddString(TEXT("[SUCCESS] SOCKET BIND"));

		/////////////////////////////////// listen /////////////////////////////////// 
		nRet = ::listen(hListenSocket, 200);
		if (nRet == SOCKET_ERROR)
		{
			m_ltlogMessage.AddString(TEXT("[ERROR] SOCKET LISTEN\n[ERROR CODE] %d", WSAGetLastError()));
			return;
		}
		m_ltlogMessage.AddString(TEXT("[SUCCESS] SOCKET LISTEN"));

		AfxBeginThread(AccpetThread, this);
	}
	
}

void CFTPServerDlg::OnBnClickedbnshutdown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SetWindowPos(nullptr, 0, 0, m_rtWindow.Width(), 140, SWP_NOMOVE | SWP_NOZORDER);
	::closesocket(hListenSocket);
	bServerRunning = false;
	m_ltlogMessage.AddString(TEXT("[INFO] SERVER SHUTDOWN"));
}


void CFTPServerDlg::OnEnChangeedmaxconn()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CFTPServerDlg::OnEnChangeedprotnumer()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
