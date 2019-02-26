
// penwu2.0Dlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "WorkThread.h"

#define WM_WORK WM_USER+1
//#define WM_WORK WM_USER+1

//UINT ThreadFunc(LPVOID lpParam);
// CSprayDlg 对话框
class CSprayDlg : public CDialogEx
{
// 构造
public:
	CSprayDlg(CWnd* pParent = NULL);	// 标准构造函数
	CWorkThread* m_pWorkThread;
// 对话框数据
	enum { IDD = IDD_Spray_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	CBrush m_brBk;

// 实现
protected:
	HICON m_hIcon;
	CWinThread* pThread;
	//LRESULT OnDisplay(WPARAM wParam, LPARAM lParam);


	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnDisplay(WPARAM wParam, LPARAM lParam);
	BOOL m_AUTO;
	afx_msg void OnClickedRadio1();
	afx_msg void OnClickedButtonRun();
	CBitmapButton m_button;
	CBitmapButton m_button2;
	CBitmapButton m_button3;
	CBitmapButton m_button4;
	CBitmapButton m_button5;
	CBitmapButton m_button6;
	CBitmapButton m_button7;
	CBitmapButton m_button8;
	CBitmapButton m_button9;
	CBitmapButton m_button10;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedCheckL();
	afx_msg void OnBnClickedCheckR();
	afx_msg void OnBnClickedButtonExit();
	BOOL m_bAutoDelete = FALSE;
};
