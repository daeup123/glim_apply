// testDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "test.h"
#include "testDlg.h"
#include "afxdialogex.h"
#include "iostream"
#include "math.h"
#include <string>
#include <afxwin.h> 
#include <afxmt.h>  

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

#define PADDINGSIZE 20
#define M_PI 3.14159265358979323846
#define WM_UPDATE_CIRCLE_POINTS (WM_USER + 100)

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


// CtestDlg 대화 상자



CtestDlg::CtestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TEST_DIALOG, pParent)
{
	this->m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CtestDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_EDIT_Radius, this->m_editRadius);
	DDX_Control(pDX, IDC_EDIT_Tickness, this->m_editThickness);
	DDX_Control(pDX, IDC_EDIT_Circle1, this->m_editCirclePoints[0]);
	DDX_Control(pDX, IDC_EDIT_Circle2, this->m_editCirclePoints[1]);
	DDX_Control(pDX, IDC_EDIT_Circle3, this->m_editCirclePoints[2]);
	CDialogEx::DoDataExchange(pDX);

}

BEGIN_MESSAGE_MAP(CtestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_GETMINMAXINFO()
	ON_WM_GETMINMAXINFO()
	ON_EN_CHANGE(IDC_EDIT_Radius, &CtestDlg::OnEnChangeEditRadius)
	ON_EN_CHANGE(IDC_EDIT_Tickness, &CtestDlg::OnEnChangeEditTickness)
	ON_BN_CLICKED(IDC_BUTTON_INIT, &CtestDlg::OnBnClickedButtonInit)
	ON_BN_CLICKED(IDC_BUTTON_RANDOM, &CtestDlg::OnBnClickedButtonRandom)
	ON_MESSAGE(WM_UPDATE_CIRCLE_POINTS, &CtestDlg::OnUpdateCirclePoints)
	ON_MESSAGE(WM_USER + 101, &CtestDlg::OnEnableRandomButton)
END_MESSAGE_MAP()


// CtestDlg 메시지 처리기

BOOL CtestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = this->GetSystemMenu(FALSE);
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
	this->m_image.Create(this->m_nImageWidth, -(this->m_nImageHeight), this->m_nBpp);
	// 팔레트 설정을 해야 색상이 올바르게 나옵니다.
	if (this->m_nBpp == 8)
	{
		static RGBQUAD rgb[256];
		for (int i = 0; i < 256; i++)
		{
			rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
		}
		this->m_image.SetColorTable(0, 256, rgb);
	}

	int nPitch = this->m_image.GetPitch();
	unsigned char* pBits = (unsigned char*)this->m_image.GetBits();

	memset(pBits, 0xff, nPitch * this->m_nImageHeight);

	// 처음에 반지름, 두께 설정
	CString myCString;
	myCString.Format(_T("%d"), this->m_nRadius);
	this->m_editRadius.SetWindowText(myCString);
	memset(myCString.GetBuffer(), 0, myCString.GetLength());
	myCString.Format(_T("%d"), this->m_nThickness);
	this->m_editThickness.SetWindowText(myCString);

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	this->SetIcon(this->m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	this->SetIcon(this->m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CtestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CtestDlg::OnPaint()
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
	if (this->IsIconic())
	{
		this->SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		this->GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, this->m_hIcon);
	}
	else
	{
		this->m_image.Draw(dc.m_hDC, PADDINGSIZE, PADDINGSIZE);

		// 3개의 점이 모두 있으면 계산된 원 그리기
		if (this->m_nCountCircle == 3)
		{
			double cx, cy, r;
			if (this->CalcCircleFrom3Points(this->m_pointCircles[0], this->m_pointCircles[1], this->m_pointCircles[2], cx, cy, r))
			{
				this->DrawSmoothCircle(&dc, cx + PADDINGSIZE, cy + PADDINGSIZE, r, 0);
			}
		}
	}
}
// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CtestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(this->m_hIcon);
}

void CtestDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (this->m_bDragging && (nFlags & MK_LBUTTON) && this->m_nSelectedCircle != -1) {
		int dx = point.x - this->m_prevPt.x;
		int dy = point.y - this->m_prevPt.y;

		unsigned char* pBits = (unsigned char*)this->m_image.GetBits();
		int nPitch = this->m_image.GetPitch();

		// 화면 전체 초기화
		memset(pBits, 0xff, nPitch * this->m_nImageHeight);

		// 원 좌표 업데이트
		this->m_pointCircles[this->m_nSelectedCircle].x += dx;
		this->m_pointCircles[this->m_nSelectedCircle].y += dy;

		// --- 1. Edit 컨트롤에 좌표 표시 ---
		CString strCoord;
		strCoord.Format(_T("%d, %d"),
			this->m_pointCircles[this->m_nSelectedCircle].x,
			this->m_pointCircles[this->m_nSelectedCircle].y);

		this->m_editCirclePoints[this->m_nSelectedCircle].SetWindowText(strCoord);

		// 모든 원 다시 그리기
		for (int i = 0; i < this->m_nCountCircle; i++) {
			int color = 0; // 전부 검정색
			this->DrawCircle(pBits,
				this->m_pointCircles[i].x,
				this->m_pointCircles[i].y,
				this->m_nRadius, nPitch, color);
		}

		// 화면 갱신
		this->Invalidate(FALSE);
		this->Check_Count(this->m_nCountCircle, 0);
		this->m_prevPt = point;
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

void CtestDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// 클릭 좌표 → 이미지 좌표 변환
	int clickX = point.x - PADDINGSIZE;
	int clickY = point.y - PADDINGSIZE;

	// 어떤 원을 눌렀는지 검사
	this->m_nSelectedCircle = -1;
	for (int i = 0; i < this->m_nCountCircle; i++) {
		int dx = clickX - this->m_pointCircles[i].x;
		int dy = clickY - this->m_pointCircles[i].y;
		if (dx * dx + dy * dy <= this->m_nRadius * this->m_nRadius) {
			this->m_nSelectedCircle = i;
			break;
		}
	}

	if (this->m_nSelectedCircle != -1) {
		this->m_bDragging = true;
		this->m_prevPt = point;
		this->SetCapture();  // 드래그 중 마우스 캡처
	}
	else {
		// 기존 로직: 새 원 추가
		if (this->m_image && this->m_nCountCircle < 3) {
			unsigned char* pBits = (unsigned char*)this->m_image.GetBits();
			int nPitch = this->m_image.GetPitch();

			this->DrawCircle(pBits, clickX, clickY, this->m_nRadius, nPitch, 0);
			this->SavePoint(clickX, clickY);
			this->Invalidate(FALSE);
			this->Check_Count(this->m_nCountCircle, 0);
		}
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CtestDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (this->m_bDragging)
	{
		this->m_bDragging = false;
		ReleaseCapture();  // 마우스 캡처 해제
	}
	CDialogEx::OnLButtonUp(nFlags, point);
}

// 원그리기?!?

void CtestDlg::DrawCircle(unsigned char* pBits, int nCenterX, int nCenterY, int radius, int nPitch, int color)
{
	// 원을 그린다.

	int nWidth = this->m_nImageWidth;
	int nHeight = this->m_nImageHeight;
	for (int i = nCenterX - radius; i <= nCenterX + radius; i++)
	{
		for (int j = nCenterY - radius; j <= nCenterY + radius; j++)
		{
			if (i >= 0 && i < nWidth && j >= 0 && j < nHeight)
			{
				double dx = nCenterX - i;
				double dy = nCenterY - j;
				if (this->xyinCircle(dx, dy, radius))
				{
					pBits[j * nPitch + i] = color; // 팔레트 0 = 검정
				}
			}
		}
	}
}

void CtestDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	lpMMI->ptMinTrackSize.x = this->m_nImageWidth + 370;
	lpMMI->ptMinTrackSize.y = this->m_nImageHeight + 170;
	CDialogEx::OnGetMinMaxInfo(lpMMI);
}

bool CtestDlg::xyinCircle(double dx, double dy, int nRadius)
{
	bool bResult = false;

	if (dx * dx + dy * dy <= nRadius * nRadius)
	{
		bResult = true;
	}
	return bResult;
}

void CtestDlg::OnEnChangeEditRadius()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// ENM_CHANGE가 있으면 마스크에 ORed를 플래그합니다.
	unsigned char* pBits = (unsigned char*)this->m_image.GetBits();
	int nPitch = this->m_image.GetPitch();
	for (int i = 0; i < this->m_nCountCircle; i++)
	{
		int color = 255; // 전부 검정색
		this->DrawCircle(pBits, this->m_pointCircles[i].x, this->m_pointCircles[i].y, this->m_nRadius, nPitch, color);
	}
	CString strRadius;
	this->m_editRadius.GetWindowText(strRadius);
	int nRadius = _ttoi(strRadius);
	this->m_nRadius = nRadius;
	// 모든 원 다시 그리기
	for (int i = 0; i < this->m_nCountCircle; i++)
	{
		int color = 0; // 전부 검정색
		this->DrawCircle(pBits, this->m_pointCircles[i].x, this->m_pointCircles[i].y, this->m_nRadius, nPitch, color);
	}
	// 화면 갱신
	this->Invalidate(FALSE);
	this->Check_Count(this->m_nCountCircle, 0);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CtestDlg::OnEnChangeEditTickness()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// ENM_CHANGE가 있으면 마스크에 ORed를 플래그합니다.
	this->Check_Count(this->m_nCountCircle, 255);
	CString strTickness;
	this->m_editThickness.GetWindowText(strTickness);
	int nTickness = _ttoi(strTickness);
	this->m_nThickness = nTickness;
	// 모든 원 다시 그리기
	unsigned char* pBits = (unsigned char*)this->m_image.GetBits();
	int nPitch = this->m_image.GetPitch();
	for (int i = 0; i < this->m_nCountCircle; i++)
	{
		int color = 0; // 전부 검정색
		this->DrawCircle(pBits, this->m_pointCircles[i].x, this->m_pointCircles[i].y, this->m_nRadius, nPitch, color);
	}
	// 화면 갱신
	this->Invalidate(FALSE);
	this->Check_Count(this->m_nCountCircle, 0);

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

// 그려진 원을 눌렀는지 확인
bool CtestDlg::CheckinCircle(unsigned char* pBits, CPoint point, int nPitch)
{
	bool bResult = false;

	// 클릭한 점이 검정색이면 원 안에 있는 것
	if ((int)pBits[(point.y - PADDINGSIZE) * nPitch + (point.x - PADDINGSIZE)] == 0)
	{
		bResult = true;
	}
	return bResult;
}

void CtestDlg::OnBnClickedButtonInit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
		// 스레드 종료 요청
	this->m_bStopThread = true;

	// 스레드 종료 대기 (짧게)
	if (this->m_pRandomThread)
	{
		WaitForSingleObject(this->m_pRandomThread->m_hThread, 200);
		this->m_pRandomThread = nullptr;
	}

	this->initmember();
	// 화면에 즉시 출력
	//this->Invalidate(FALSE);
}

void CtestDlg::initmember()
{
	unsigned char* pBits = (unsigned char*)this->m_image.GetBits();
	int nPitch = this->m_image.GetPitch();

	// 원 개수만큼 반복
	for (int i = 0; i < this->m_nCountCircle; i++)
	{
		int nCenterX = this->m_pointCircles[i].x;
		int nCenterY = this->m_pointCircles[i].y;

		// 흰색(255번 팔레트 인덱스)으로 다시 칠하기
		this->DrawCircle(pBits, nCenterX, nCenterY, this->m_nRadius, nPitch, 255);

		this->m_editCirclePoints[i].SetWindowText(_T(""));
	}

	this->m_nCountCircle = 0;
	// 배열 초기화
	for (int i = 0; i < 3; i++)
	{
		this->m_pointCircles[i] = CPoint(0, 0);
	}

	// 화면 갱신
	this->Invalidate(FALSE);
}

void CtestDlg::SavePoint(int nCenterX, int nCenterY)
{
	this->m_editCirclePoints[this->m_nCountCircle].SetWindowText(
		CString(to_string(nCenterX).c_str()) + _T(", ") + CString(to_string(nCenterY).c_str()));

	// 포인트저장
	this->m_pointCircles[this->m_nCountCircle].x = nCenterX;
	this->m_pointCircles[this->m_nCountCircle].y = nCenterY;

	this->m_nCountCircle++;
}

bool CtestDlg::CalcCircleFrom3Points(const CPoint& p1, const CPoint& p2, const CPoint& p3,
	double& cx, double& cy, double& r)
{
	double x1 = p1.x, y1 = p1.y;
	double x2 = p2.x, y2 = p2.y;
	double x3 = p3.x, y3 = p3.y;

	double det = x1 * (y2 - y3) - y1 * (x2 - x3) + (x2 * y3 - x3 * y2);
	if (fabs(det) < 1e-8) return false; // 거의 일직선이면 원 없음

	double x1s = x1 * x1 + y1 * y1;
	double x2s = x2 * x2 + y2 * y2;
	double x3s = x3 * x3 + y3 * y3;

	cx = (x1s * (y2 - y3) + x2s * (y3 - y1) + x3s * (y1 - y2)) / (2.0 * det);
	cy = (x1s * (x3 - x2) + x2s * (x1 - x3) + x3s * (x2 - x1)) / (2.0 * det);
	r = sqrt((x1 - cx) * (x1 - cx) + (y1 - cy) * (y1 - cy));

	return true;
}

void CtestDlg::DrawSmoothCircle(CDC* pDC, double cx, double cy, double r, int color)
{
	if (r <= 0) return;

	int steps = (int)ceil(2.0 * M_PI * r / 1.0); // 원주 / 원하는 픽셀 간격
	steps = max(32, steps); // 최소 점 수 32
	double dTheta = 2.0 * M_PI / steps;
	COLORREF clr = (color == 0) ? RGB(0, 0, 0) : RGB(255, 255, 255); // 색상 변환

	CPen pen(PS_SOLID, this->m_nThickness, clr);
	CPen* pOldPen = pDC->SelectObject(&pen);

	// 이전 점 좌표
	int prevX = (int)llround(cx + r);
	int prevY = (int)llround(cy);

	for (int i = 1; i <= steps; ++i)
	{
		double theta = i * dTheta;
		int x = (int)llround(cx + r * cos(theta));
		int y = (int)llround(cy + r * sin(theta));

		// 이미지 영역 안에서만 그리기
		if (x >= (PADDINGSIZE + this->m_nThickness) && x < this->m_nImageWidth + (PADDINGSIZE - this->m_nThickness) &&
			y >= (PADDINGSIZE + this->m_nThickness) && y < this->m_nImageHeight + (PADDINGSIZE - this->m_nThickness) &&
			prevX >= (PADDINGSIZE + this->m_nThickness) && prevX < this->m_nImageWidth + (PADDINGSIZE - this->m_nThickness) &&
			prevY >= (PADDINGSIZE + this->m_nThickness) && prevY < this->m_nImageHeight + (PADDINGSIZE - this->m_nThickness))
		{
			pDC->MoveTo(prevX, prevY);
			pDC->LineTo(x, y);
		}

		prevX = x;
		prevY = y;
	}

	pDC->SelectObject(pOldPen);
}


void CtestDlg::Check_Count(int count, int color)
{
	if (count == 3)
	{
		// 3개의 점으로 원 계산
		double cx, cy, r;
		if (this->CalcCircleFrom3Points(this->m_pointCircles[0], this->m_pointCircles[1], this->m_pointCircles[2], cx, cy, r))
		{
			// 계산된 원 그리기 - OnPaint에서 처리되도록 Invalidate 호출
			this->Invalidate(FALSE);
		}
	}
}

void CtestDlg::OnBnClickedButtonRandom()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (this->m_nCountCircle == 3)
	{
		this->m_bStopThread = false; // 스레드 진행 허용
		this->GetDlgItem(IDC_BUTTON_RANDOM)->EnableWindow(FALSE);
		this->m_pRandomThread = AfxBeginThread(ThreadRandomPoints, this);
	}
}
UINT CtestDlg::ThreadRandomPoints(LPVOID pParam)
{
	CtestDlg* pThis = (CtestDlg*)pParam;

	for (int i = 0; i < 10; i++)
	{
		// Sleep 500ms를 10번으로 쪼개서 즉시 종료 체크
		for (int t = 0; t < 50; t++)
		{
			if (pThis->m_bStopThread) goto EndThread;
			Sleep(10); // 10ms씩 체크
		}

		if (pThis->m_bStopThread) break;

		CPoint* newPoints = new CPoint[3];
		for (int j = 0; j < 3; j++)
		{
			newPoints[j].x = PADDINGSIZE + rand() % pThis->m_nImageWidth;
			newPoints[j].y = PADDINGSIZE + rand() % pThis->m_nImageHeight;
		}

		pThis->PostMessage(WM_UPDATE_CIRCLE_POINTS, 0, (LPARAM)newPoints);
	}

EndThread:
	// 스레드 끝나면 랜덤 버튼 활성화
	pThis->PostMessage(WM_USER + 101, 0, 0);
	return 0;
}

LRESULT CtestDlg::OnUpdateCirclePoints(WPARAM wParam, LPARAM lParam)
{
	CPoint* newPoints = (CPoint*)lParam;
	unsigned char* pBits = (unsigned char*)this->m_image.GetBits();
	int nPitch = this->m_image.GetPitch();

	// 화면 초기화
	memset(pBits, 0xff, nPitch * this->m_nImageHeight);

	for (int i = 0; i < 3; i++)
	{
		this->m_pointCircles[i] = newPoints[i];

		CString strCoord;
		strCoord.Format(_T("%d, %d"), this->m_pointCircles[i].x, this->m_pointCircles[i].y);
		this->m_editCirclePoints[i].SetWindowText(strCoord);

		// 원 그리기
		this->DrawCircle(pBits, this->m_pointCircles[i].x, this->m_pointCircles[i].y, this->m_nRadius, nPitch, 0);
	}

	// 화면 갱신
	this->Invalidate(FALSE);
	this->Check_Count(this->m_nCountCircle, 0);

	delete[] newPoints; // 메모리 해제

	return 0;
}

LRESULT CtestDlg::OnEnableRandomButton(WPARAM wParam, LPARAM lParam)
{
	this->GetDlgItem(IDC_BUTTON_RANDOM)->EnableWindow(TRUE);
	return 0;
}