
// penwu2.0Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "penwu2.0.h"
#include "penwu2.0Dlg.h"
#include "afxdialogex.h"
//#include "global.h"

extern BOOL RUN;//线程启停
extern BOOL   L;		   //左侧喷雾
extern BOOL   R;		   //右侧喷雾
extern BOOL AUTO;         //单选按钮状态;自动手动
extern float V;           //速度
extern float D;		   //距离
extern BOOL NOZZLE[8];//初始化8个灯的状态


#ifdef _DEBUG
#define new DEBUG_NEW
#endif





// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CSprayDlg 对话框



CSprayDlg::CSprayDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSprayDlg::IDD, pParent)
	, m_AUTO(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSprayDlg::DoDataExchange(CDataExchange* pDX)
{
	
	V = 1.0;
	D = 1.0;
	L = TRUE;
	R = TRUE;
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_AUTO);
	DDX_Check(pDX, IDC_CHECK1, L);
	DDX_Check(pDX, IDC_CHECK2, R);
	DDX_Text(pDX, IDC_OPERATOR1,V);
	DDX_Text(pDX, IDC_OPERATOR2, D);
	

	//DDX_Control(pDX, IDC_BMP, m_cBmp1);
	//DDX_Control(pDX, IDC_BMP, m_cBmp1);
}

BEGIN_MESSAGE_MAP(CSprayDlg, CDialogEx)
	ON_MESSAGE(WM_DISPLAY, OnDisplay)

	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RADIO1, &CSprayDlg::OnClickedRadio1)
	ON_BN_CLICKED(IDC_BUTTON1, &CSprayDlg::OnClickedButtonRun)
	ON_WM_CTLCOLOR()
	
	ON_BN_CLICKED(IDC_CHECK1, &CSprayDlg::OnBnClickedCheckL)
	ON_BN_CLICKED(IDC_CHECK2, &CSprayDlg::OnBnClickedCheckR)
	ON_BN_CLICKED(IDC_BUTTON10, &CSprayDlg::OnBnClickedButtonExit)
END_MESSAGE_MAP()


// CSprayDlg 消息处理程序

BOOL CSprayDlg::OnInitDialog()
{
	
	
	CDialogEx::OnInitDialog();

	
	//m_icon_open.SizeToContent();
	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	m_button.LoadBitmaps(IDB_ON);
	m_button.SubclassDlgItem(IDC_BUTTON1, this);
	m_button.SizeToContent();
	m_button2.LoadBitmaps(IDB_M);
	m_button2.SubclassDlgItem(IDC_BUTTON2, this);
	m_button2.SizeToContent();
	m_button3.LoadBitmaps(IDB_M);
	m_button3.SubclassDlgItem(IDC_BUTTON3, this);
	m_button3.SizeToContent();
	m_button4.LoadBitmaps(IDB_M);
	m_button4.SubclassDlgItem(IDC_BUTTON4, this);
	m_button4.SizeToContent();
	m_button5.LoadBitmaps(IDB_M);
	m_button5.SubclassDlgItem(IDC_BUTTON5, this);
	m_button5.SizeToContent();
	m_button6.LoadBitmaps(IDB_M);
	m_button6.SubclassDlgItem(IDC_BUTTON6, this);
	m_button6.SizeToContent();
	m_button7.LoadBitmaps(IDB_M);
	m_button7.SubclassDlgItem(IDC_BUTTON7, this);
	m_button7.SizeToContent();
	m_button8.LoadBitmaps(IDB_M);
	m_button8.SubclassDlgItem(IDC_BUTTON8, this);
	m_button8.SizeToContent();
	m_button9.LoadBitmaps(IDB_M);
	m_button9.SubclassDlgItem(IDC_BUTTON9, this);
	m_button9.SizeToContent();
	m_button10.LoadBitmaps(IDB_OUT);
	m_button10.SubclassDlgItem(IDC_BUTTON10, this);
	m_button10.SizeToContent();

	CBitmap bmp;
	bmp.LoadBitmapW(IDB_BG);
	m_brBk.CreatePatternBrush(&bmp);
	bmp.DeleteObject();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSprayDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSprayDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSprayDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSprayDlg::OnClickedRadio1()
{   //单击次数为偶数，选中；奇数则未选中
	if (m_AUTO == FALSE)  //此处只能用成员变量
	{
		m_AUTO = TRUE;
		AUTO = FALSE;
		 //RadioButton未选中
	}		
	else	
	{
		m_AUTO =FALSE;
		AUTO = TRUE;//RadioButton选中		
	}
 UpdateData(FALSE);
}




void CSprayDlg::OnClickedButtonRun()
{
	
	if (RUN == FALSE)
		RUN = TRUE;
	else
		RUN = FALSE;
	if (RUN == TRUE)
	{
		//读取edit控件值
		UpdateData(TRUE);
		m_button.LoadBitmaps(IDB_OFF);
		m_button.Invalidate();
	}
	else
	{
		m_button.LoadBitmaps(IDB_ON);
		m_button.Invalidate();
	}
	if (m_pWorkThread == NULL)
	{
		m_pWorkThread = (CWorkThread*)AfxBeginThread(RUNTIME_CLASS(CWorkThread));
		m_pWorkThread->PostThreadMessage(WM_WORK, NULL, NULL);
	}
	// TODO:  在此添加控件通知处理程序代码
}


HBRUSH CSprayDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (pWnd == this)
	{
		return m_brBk;
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}






void CSprayDlg::OnBnClickedCheckL()
{
		CButton *pCheckbox = (CButton*)GetDlgItem(IDC_CHECK1);
		if (pCheckbox->GetCheck())
		{
			L = TRUE;
		}
		else
		{
			L = FALSE; 
		}
		// TODO:  在此添加控件通知处理程序代码	
}


void CSprayDlg::OnBnClickedCheckR()
{
	CButton *pCheckbox = (CButton*)GetDlgItem(IDC_CHECK2);
	if (pCheckbox->GetCheck())
	{
		R = TRUE;	//
	}
	else
	{
		R = FALSE;//
	}// TODO:  在此添加控件通知处理程序代码
}
LRESULT  CSprayDlg::OnDisplay(WPARAM wParam, LPARAM lParam)

{
	if (NOZZLE[0]==TRUE )m_button5.LoadBitmaps(IDB_L);
	else m_button5.LoadBitmaps(IDB_M);
	m_button5.Invalidate();
	if (NOZZLE[1] == TRUE)m_button4.LoadBitmaps(IDB_L);
	else m_button4.LoadBitmaps(IDB_M);
	m_button4.Invalidate();
	if (NOZZLE[2] == TRUE)m_button3.LoadBitmaps(IDB_L);
	else m_button3.LoadBitmaps(IDB_M);
	m_button3.Invalidate();
	if (NOZZLE[3] == TRUE)m_button2.LoadBitmaps(IDB_L);
	else m_button2.LoadBitmaps(IDB_M);
	m_button2.Invalidate();
	if (NOZZLE[4] == TRUE)m_button6.LoadBitmaps(IDB_L);
	else m_button6.LoadBitmaps(IDB_M);
	m_button6.Invalidate();
	if (NOZZLE[5] == TRUE)m_button7.LoadBitmaps(IDB_L);
	else m_button7.LoadBitmaps(IDB_M);
	m_button7.Invalidate();
	if (NOZZLE[6] == TRUE)m_button8.LoadBitmaps(IDB_L);
	else m_button8.LoadBitmaps(IDB_M);
	m_button8.Invalidate();
	if (NOZZLE[7] == TRUE)m_button9.LoadBitmaps(IDB_L);
	else m_button9.LoadBitmaps(IDB_M);
	m_button9.Invalidate();
	return 0;
	}

	




void CSprayDlg::OnBnClickedButtonExit()
{
	// TODO:  在此添加控件通知处理程序代码
	/*if (m_pWorkThread)
	{
		m_pWorkThread->PostThreadMessage(WM_QUIT,NULL,NULL);
		if (WAIT_OBJECT_0 == WaitForSingleObject(m_pWorkThread->m_hThread, INFINITE))
		{
			PostQuitMessage(0);
		}
		
	}
	else */
	PostQuitMessage(0);
	 
}

