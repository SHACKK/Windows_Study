// LoginDlg.cpp: 구현 파일
//

#include "pch.h"
#include "SockMFC.h"
#include "LoginDlg.h"
#include "afxdialogex.h"


// LoginDlg 대화 상자

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edUserID);
	DDX_Control(pDX, IDC_EDIT2, m_edUserPW);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_BN_CLICKED(IDC_bnLogin, &CLoginDlg::OnBnClickedbnlogin)
	ON_BN_CLICKED(IDC_bnCancel, &CLoginDlg::OnBnClickedbncancel)
END_MESSAGE_MAP()

// LoginDlg 메시지 처리기

BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	hSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (hSocket == INVALID_SOCKET)
	{
		MessageBox(TEXT("소켓 생성 실패"), TEXT("에러"), MB_ICONERROR);
		CDialogEx::OnCancel();
		return FALSE;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CLoginDlg::OnBnClickedbnlogin()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strUserID, strUserPW;
	GetDlgItemText(IDC_EDIT1, strUserID);
	GetDlgItemText(IDC_EDIT2, strUserPW);

	//소켓 연결
	sockaddr_in service;
	service.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &(service.sin_addr.s_addr));
	service.sin_port = htons(56000);
	nConnectResult = ::connect(hSocket, (sockaddr*)&service, (int)sizeof(service));
	if (nConnectResult == SOCKET_ERROR)
	{

		MessageBox(TEXT("서버 연결 실패"), TEXT("에러"), MB_ICONERROR);
		return;
	}
	else 
	{
		this->EndDialog((int)hSocket);
	}
}

void CLoginDlg::OnBnClickedbncancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (MessageBox(TEXT("종료할까요?"), TEXT("궁금"), MB_YESNO) == IDYES)
	{
		this->EndDialog(INVALID_SOCKET);
	}
}

void CLoginDlg::OnCancel()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	OnBnClickedbncancel();
}


void CLoginDlg::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	OnBnClickedbnlogin();
}

