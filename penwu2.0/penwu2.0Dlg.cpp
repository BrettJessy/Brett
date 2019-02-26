
// penwu2.0Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "penwu2.0.h"
#include "penwu2.0Dlg.h"
#include "afxdialogex.h"
//#include "global.h"

extern BOOL RUN;//�߳���ͣ
extern BOOL   L;		   //�������
extern BOOL   R;		   //�Ҳ�����
extern BOOL AUTO;         //��ѡ��ť״̬;�Զ��ֶ�
extern float V;           //�ٶ�
extern float D;		   //����
extern BOOL NOZZLE[8];//��ʼ��8���Ƶ�״̬


#ifdef _DEBUG
#define new DEBUG_NEW
#endif





// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CSprayDlg �Ի���



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


// CSprayDlg ��Ϣ�������

BOOL CSprayDlg::OnInitDialog()
{
	
	
	CDialogEx::OnInitDialog();

	
	//m_icon_open.SizeToContent();
	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
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
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSprayDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSprayDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSprayDlg::OnClickedRadio1()
{   //��������Ϊż����ѡ�У�������δѡ��
	if (m_AUTO == FALSE)  //�˴�ֻ���ó�Ա����
	{
		m_AUTO = TRUE;
		AUTO = FALSE;
		 //RadioButtonδѡ��
	}		
	else	
	{
		m_AUTO =FALSE;
		AUTO = TRUE;//RadioButtonѡ��		
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
		//��ȡedit�ؼ�ֵ
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


HBRUSH CSprayDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (pWnd == this)
	{
		return m_brBk;
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
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
		// TODO:  �ڴ���ӿؼ�֪ͨ����������	
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
	}// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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

