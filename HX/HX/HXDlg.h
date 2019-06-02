
// HXDlg.h : 头文件
//

#pragma once
#include "Tree.h"

// CHXDlg 对话框
class CHXDlg : public CDialogEx
{
	// 构造
public:
	CHXDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_HX_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedShow();
	afx_msg void OnBnClickedDisplay();
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedStep();
	afx_msg void OnBnClickedClean();
	afx_msg void OnSelchangeWay();
	void CPreorder(BiTree T,int level=1);
	CComboBox way;
	CListCtrl result;
	CListCtrl code;
	int deep;
	CString input;
	CString sway;
	CStatic picture;
	BiTree tree;
	std::ifstream is;
};
