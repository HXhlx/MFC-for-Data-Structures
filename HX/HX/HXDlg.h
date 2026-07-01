
// HXDlg.h : header file
//

#pragma once
#include "Tree.h"
#include <vector>

struct TraversalState
{
    std::vector<std::wstring> steps;
    int currentIndex;
    bool isRunning;

    TraversalState() : currentIndex(0), isRunning(false) {}
};

// CHXDlg dialog
class CHXDlg : public CDialogEx
{
// Construction
public:
    CHXDlg(CWnd* pParent = NULL);

// Dialog Data
    enum { IDD = IDD_HX_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

// Implementation
protected:
    HICON m_hIcon;

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
    afx_msg void OnBnClickedClean();
    afx_msg void OnSelchangeWay();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnDestroy();
    afx_msg void OnTimer(UINT_PTR nIDEvent);

    void DrawTree(const BiTNode* node, int level, COLORREF color);
    void DrawTreeRecursive(const BiTNode* node, int level, COLORREF color);
    void DrawLines(const BiTNode* node, int level);
    void RedrawTree();
    void DrawNodeHighlight(const BiTNode* node, int level, COLORREF color);

    CComboBox m_wndWay;
    CListCtrl m_wndResult;
    CListCtrl m_wndCode;
    CStatic m_wndPicture;

    int m_nDeep;
    CString m_strInput;
    BiTreePtr m_tree;
    CFont m_font;

    TraversalType m_nCurrentType;
    TraversalState m_state;
    std::vector<const BiTNode*> m_nodeSteps;
    static const UINT_PTR TIMER_ID = 1001;
};
