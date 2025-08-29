
// testDlg.h: 헤더 파일
//

#pragma once


// CtestDlg 대화 상자
class CtestDlg : public CDialogEx
{
// 생성입니다.
public:
	CtestDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEST_DIALOG };
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

private:
	// 사용자 정의 메시지 핸들러
	afx_msg LRESULT OnUpdateCirclePoints(WPARAM wParam, LPARAM lParam);
	LRESULT OnEnableRandomButton(WPARAM wParam, LPARAM lParam);
	static UINT ThreadRandomPoints(LPVOID pParam);
	CWinThread* m_pRandomThread = nullptr; // 스레드 포인터
	volatile bool m_bStopThread = false;   // 스레드 종료 플래그

	// 원을 그릴 프레임 이미지
	CImage m_image;
	int m_nImageWidth = 640;
	int m_nImageHeight = 480;
	int m_nBpp = 8;

	int m_nCountCircle = 0; // 현재 원의 개수
	CPoint m_pointCircles[3]; // 최대 3개의 원
	int m_nSelectedCircle = -1;   // 현재 선택된 원 인덱스 (-1이면 선택 없음)
	bool m_bDragging = false;        // 드래그 중인지 여부
	CPoint m_prevPt;         // 이전 마우스 위치

	// 원그리는 함수들
	void DrawCircle(unsigned char* pBits, int nCenterX, int nCenterY, int radius, int nPitch, int color);
	bool xyinCircle(double dx, double dy, int nRadius);
	bool CheckinCircle(unsigned char* pBits, CPoint point, int nPitch);

	// 정원 그리는 함수들
	bool CalcCircleFrom3Points(const CPoint& p1, const CPoint& p2, const CPoint& p3,
		double& cx, double& cy, double& r);
	void DrawSmoothCircle(CDC* pDC, double cx, double cy, double r, int color = 0);
	void Check_Count(int count,int color);

	//UI 바인딩 변수
	bool m_bVisibleBtnRandom = false; // 랜덤버튼
	int m_nRadius = 10; // 반지름
	int m_nThickness = 3; // 두께

	//UI
	CEdit m_editRadius;
	CEdit m_editThickness;
	CEdit m_editCirclePoints[3];

	//함수
	void initmember(); // 맴버변수 초기화
	void SavePoint(int nCenterX, int nCenterY); // 중심좌표 저장
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnEnChangeEditRadius();
	afx_msg void OnEnChangeEditTickness();
	afx_msg void OnBnClickedButtonInit();
	afx_msg void OnBnClickedButtonRandom();
};
