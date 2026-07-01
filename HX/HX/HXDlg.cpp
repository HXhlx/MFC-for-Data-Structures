
// HXDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HX.h"
#include "HXDlg.h"
#include "afxdialogex.h"
#include "Logger.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();
    enum { IDD = IDD_ABOUTBOX };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
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

CHXDlg::CHXDlg(CWnd* pParent)
    : CDialogEx(CHXDlg::IDD, pParent)
    , m_strInput(_T(""))
    , m_nDeep(0)
    , m_nCurrentType(TraversalType::Preorder)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_font.CreatePointFont(150, _T("Consolas"));
}

void CHXDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_WAY, m_wndWay);
    DDX_Control(pDX, IDC_RESULT, m_wndResult);
    DDX_Control(pDX, IDC_CODE, m_wndCode);
    DDX_Text(pDX, IDC_DEEP, m_nDeep);
    DDX_Text(pDX, IDC_INPUT, m_strInput);
    DDX_Control(pDX, IDC_PICTURE, m_wndPicture);
}

BEGIN_MESSAGE_MAP(CHXDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_SHOW, &CHXDlg::OnBnClickedShow)
    ON_BN_CLICKED(IDC_DISPLAY, &CHXDlg::OnBnClickedDisplay)
    ON_BN_CLICKED(IDC_START, &CHXDlg::OnBnClickedStart)
    ON_BN_CLICKED(IDC_STOP, &CHXDlg::OnBnClickedStop)
    ON_BN_CLICKED(IDC_CLEAN, &CHXDlg::OnBnClickedClean)
    ON_CBN_SELCHANGE(IDC_WAY, &CHXDlg::OnSelchangeWay)
    ON_WM_ERASEBKGND()
    ON_WM_DESTROY()
    ON_WM_TIMER()
    ON_WM_SIZE()
END_MESSAGE_MAP()

BOOL CHXDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

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

    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    SetWindowText(_T("\u4e8c\u53c9\u6811\u904d\u5386\u53ef\u89c6\u5316"));

    CWnd* pWnd = nullptr;
    pWnd = GetDlgItem(IDC_SHOW);
    if (pWnd) pWnd->SetWindowText(_T("\u663e\u793a\u4e8c\u53c9\u6811"));
    pWnd = GetDlgItem(IDC_DISPLAY);
    if (pWnd) pWnd->SetWindowText(_T("\u6f14\u793a\u5b9e\u4f8b"));
    pWnd = GetDlgItem(IDC_START);
    if (pWnd) pWnd->SetWindowText(_T("\u5f00\u59cb"));
    pWnd = GetDlgItem(IDC_STOP);
    if (pWnd) pWnd->SetWindowText(_T("\u505c\u6b62"));
    pWnd = GetDlgItem(IDC_CLEAN);
    if (pWnd) pWnd->SetWindowText(_T("\u6e05\u7a7a\u904d\u5386"));

    m_wndWay.ResetContent();
    m_wndWay.InsertString(0, _T("\u5148\u5e8f\u904d\u5386\u4e8c\u53c9\u6811"));
    m_wndWay.InsertString(1, _T("\u4e2d\u5e8f\u904d\u5386\u4e8c\u53c9\u6811"));
    m_wndWay.InsertString(2, _T("\u540e\u5e8f\u904d\u5386\u4e8c\u53c9\u6811"));
    m_wndWay.SetCurSel(0);

    CRect rect;
    m_wndResult.GetClientRect(rect);
    m_wndResult.InsertColumn(0, _T("\u904d\u5386\u7ed3\u679c"), LVCFMT_CENTER, rect.Width());
    m_wndResult.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    m_wndCode.GetClientRect(rect);
    m_wndCode.InsertColumn(0, _T("\u4ee3\u7801"), LVCFMT_CENTER, rect.Width());
    m_wndCode.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    OnSelchangeWay();

    // Subclass buttons as CMFCButton for modern styling
    m_btnShow.SubclassDlgItem(IDC_SHOW, this);
    m_btnDisplay.SubclassDlgItem(IDC_DISPLAY, this);
    m_btnStart.SubclassDlgItem(IDC_START, this);
    m_btnStop.SubclassDlgItem(IDC_STOP, this);
    m_btnClean.SubclassDlgItem(IDC_CLEAN, this);

    // Create status bar at bottom
    CRect rc;
    GetClientRect(&rc);
    m_wndStatusBar.Create(_T("Ready"), WS_CHILD | WS_VISIBLE | SS_LEFTNOWORDWRAP | SS_SUNKEN,
        CRect(0, rc.bottom - 22, rc.right, rc.bottom), this);
    m_wndStatusBar.SetFont(GetFont());

    return TRUE;
}

void CHXDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CHXDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this);
        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

HCURSOR CHXDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

static int CountNodes(const BiTNode* node)
{
    if (!node) return 0;
    return 1 + CountNodes(node->lchild.get()) + CountNodes(node->rchild.get());
}

void CHXDlg::OnBnClickedShow()
{
    UpdateData();
    if (m_strInput.IsEmpty())
    {
        AfxMessageBox(_T("\u8bf7\u8f93\u5165\u4e8c\u53c9\u6811\uff01"), MB_ICONWARNING);
        return;
    }

    int n = 1;
    for (int i = 0; i < m_strInput.GetLength(); i++)
    {
        if (--n < 0)
        {
            AfxMessageBox(_T("\u8f93\u5165\u4e0d\u5408\u6cd5\uff01"), MB_ICONERROR);
            return;
        }
        if (m_strInput[i] != _T('#'))
            n += 2;
    }
    if (n)
    {
        AfxMessageBox(_T("\u8f93\u5165\u4e0d\u5408\u6cd5\uff01"), MB_ICONERROR);
        return;
    }

    std::wstring inputStr(m_strInput.GetString());
    size_t index = 0;
    m_tree = CreateBt(inputStr, index);
    m_nDeep = BiTreeDepth(m_tree.get());
    UpdateData(FALSE);

    RedrawTree();
    LOG("Tree created, depth=" + std::to_string(m_nDeep));
    CString status;
    status.Format(_T("Tree loaded  |  Nodes: %d  |  Depth: %d"), CountNodes(m_tree.get()), m_nDeep);
    UpdateStatusBar(status.GetString());
}

void CHXDlg::OnBnClickedDisplay()
{
    m_strInput = _T("abc#d####");
    UpdateData(FALSE);
    OnBnClickedShow();
}

void CHXDlg::OnBnClickedStart()
{
    UpdateData();
    if (!m_tree)
    {
        AfxMessageBox(_T("\u8bf7\u8f93\u5165\u4e8c\u53c9\u6811\uff01"), MB_ICONERROR);
        return;
    }

    if (m_state.isRunning)
    {
        AfxMessageBox(_T("\u904d\u5386\u6b63\u5728\u8fdb\u884c\u4e2d\uff01"), MB_ICONWARNING);
        return;
    }

    int sel = m_wndWay.GetCurSel();
    m_nCurrentType = static_cast<TraversalType>(sel);
    LOG("OnBnClickedStart: m_nCurrentType=" + std::to_string(sel));
    if (sel < 0 || sel > 2)
    {
        AfxMessageBox(_T("\u8bf7\u9009\u62e9\u904d\u5386\u65b9\u5f0f\uff01"), MB_ICONWARNING);
        return;
    }

    // Update code display to match selected traversal
    OnSelchangeWay();

    m_wndResult.DeleteAllItems();
    RedrawTree();

    // Use Morris traversal - O(1) space
    MorrisTraversalWithNodes(m_tree.get(), m_nCurrentType, m_state.steps, m_nodeSteps);
    m_state.currentIndex = 0;
    m_state.isRunning = true;

    SetTimer(TIMER_ID, 500, NULL);
    LOG("Morris traversal animation started, type=" + std::to_string(static_cast<int>(m_nCurrentType)));

    const wchar_t* typeNames[] = { L"Preorder", L"Inorder", L"Postorder" };
    int ti = static_cast<int>(m_nCurrentType);
    CString status;
    status.Format(_T("Traversing (%s)  |  Step 0 / %zu"), typeNames[ti], m_state.steps.size());
    UpdateStatusBar(status.GetString());
}

void CHXDlg::OnBnClickedStop()
{
    if (m_state.isRunning)
    {
        KillTimer(TIMER_ID);
        m_state.isRunning = false;
        RedrawTree();
        UpdateStatusBar(L"Stopped");
    }
}

void CHXDlg::OnBnClickedClean()
{
    OnBnClickedStop();
    m_wndResult.DeleteAllItems();
    m_state.steps.clear();
    m_nodeSteps.clear();
    RedrawTree();
    UpdateStatusBar(L"Ready");
}

void CHXDlg::OnSelchangeWay()
{
    int selIdx = m_wndWay.GetCurSel();
    if (selIdx == CB_ERR)
        selIdx = 0;

    auto type = static_cast<TraversalType>(selIdx);
    LOG("OnSelchangeWay called, sel=" + std::to_string(selIdx));

    m_wndCode.DeleteAllItems();

    std::wstring code = GetTraversalCode(type);
    std::wistringstream iss(code);
    std::wstring line;
    int idx = 0;
    while (std::getline(iss, line))
    {
        m_wndCode.InsertItem(idx, line.c_str());
        idx++;
    }
}

void CHXDlg::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == TIMER_ID && m_state.isRunning)
    {
        if (m_state.currentIndex < (int)m_state.steps.size())
        {
            CString step = m_state.steps[m_state.currentIndex].c_str();
            m_wndResult.InsertItem(m_state.currentIndex, step);
            m_wndResult.EnsureVisible(m_state.currentIndex, FALSE);

            if (m_state.currentIndex < (int)m_nodeSteps.size() && m_nodeSteps[m_state.currentIndex])
            {
                RedrawTree();
                DrawNodeHighlight(m_nodeSteps[m_state.currentIndex], 
                    m_nodeSteps[m_state.currentIndex]->level, 
                    RGB(255, 0, 0));
            }

            CString status;
            const wchar_t* typeNames[] = { L"Preorder", L"Inorder", L"Postorder" };
            int ti = static_cast<int>(m_nCurrentType);
            status.Format(_T("Traversing (%s)  |  Step %d / %zu"), typeNames[ti], m_state.currentIndex + 1, m_state.steps.size());
            UpdateStatusBar(status.GetString());

            m_state.currentIndex++;
        }
        else
        {
            KillTimer(TIMER_ID);
            m_state.isRunning = false;
            RedrawTree();
            CString status;
            status.Format(_T("Traversal complete  |  %zu nodes visited"), m_state.steps.size());
            UpdateStatusBar(status.GetString());
            AfxMessageBox(_T("\u904d\u5386\u5b8c\u6210\uff01"), MB_ICONINFORMATION);
        }
    }

    CDialogEx::OnTimer(nIDEvent);
}

void CHXDlg::DrawTree(const BiTNode* node, int level, COLORREF color)
{
    if (!node)
        return;

    CDC* pDC = m_wndPicture.GetDC();
    CRect rect;
    m_wndPicture.GetClientRect(rect);

    int address = node->address;
    int x = rect.right * address / (int)(pow(2.0, level - 1) + 1);
    int y = rect.bottom * level / (m_nDeep + 1);

    CPen pen;
    pen.CreatePen(PS_SOLID, 3, color);
    CPen* pOldPen = pDC->SelectObject(&pen);

    int radius = max(10 * m_nDeep / level, 8);
    pDC->Ellipse(x - radius, y - radius, x + radius, y + radius);

    CFont* pOldFont = pDC->SelectObject(&m_font);
    pDC->SetBkMode(TRANSPARENT);

    CString str;
    str.Format(_T("%c"), node->data);
    pDC->TextOut(x - 2 * m_nDeep / level, y - 2 * m_nDeep / level - 7, str);

    pDC->SelectObject(pOldFont);
    pDC->SelectObject(pOldPen);
    ReleaseDC(pDC);
}

void CHXDlg::DrawTreeRecursive(const BiTNode* node, int level, COLORREF color)
{
    if (!node)
        return;
    DrawTree(node, level, color);
    DrawTreeRecursive(node->lchild.get(), level + 1, color);
    DrawTreeRecursive(node->rchild.get(), level + 1, color);
}

void CHXDlg::DrawLines(const BiTNode* node, int level)
{
    if (!node || level >= m_nDeep)
        return;

    CDC* pDC = m_wndPicture.GetDC();
    CRect rect;
    m_wndPicture.GetClientRect(rect);

    int x = rect.right * node->address / (int)(pow(2.0, level - 1) + 1);
    int y = rect.bottom * level / (m_nDeep + 1);

    CPen pen;
    pen.CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
    CPen* pOldPen = pDC->SelectObject(&pen);

    if (node->lchild)
    {
        int lx = rect.right * node->lchild->address / (int)(pow(2.0, level) + 1);
        int ly = rect.bottom * (level + 1) / (m_nDeep + 1);
        pDC->MoveTo(x, y);
        pDC->LineTo(lx, ly);
    }

    if (node->rchild)
    {
        int rx = rect.right * node->rchild->address / (int)(pow(2.0, level) + 1);
        int ry = rect.bottom * (level + 1) / (m_nDeep + 1);
        pDC->MoveTo(x, y);
        pDC->LineTo(rx, ry);
    }

    pDC->SelectObject(pOldPen);
    ReleaseDC(pDC);

    DrawLines(node->lchild.get(), level + 1);
    DrawLines(node->rchild.get(), level + 1);
}

void CHXDlg::RedrawTree()
{
    CDC* pDC = m_wndPicture.GetDC();
    CRect rect;
    m_wndPicture.GetClientRect(rect);
    CBrush brush(RGB(170, 170, 170));
    pDC->FillRect(&rect, &brush);
    ReleaseDC(pDC);

    if (m_tree)
    {
        DrawLines(m_tree.get(), 1);
        DrawTreeRecursive(m_tree.get(), 1, RGB(0, 0, 0));
    }
}

void CHXDlg::DrawNodeHighlight(const BiTNode* node, int level, COLORREF color)
{
    if (!node)
        return;

    CDC* pDC = m_wndPicture.GetDC();
    CRect rect;
    m_wndPicture.GetClientRect(rect);

    int address = node->address;
    int x = rect.right * address / (int)(pow(2.0, level - 1) + 1);
    int y = rect.bottom * level / (m_nDeep + 1);

    CPen pen;
    pen.CreatePen(PS_SOLID, 4, color);
    CPen* pOldPen = pDC->SelectObject(&pen);

    int radius = max(12 * m_nDeep / level, 10);
    pDC->Ellipse(x - radius, y - radius, x + radius, y + radius);

    CFont* pOldFont = pDC->SelectObject(&m_font);
    pDC->SetBkMode(TRANSPARENT);

    CString str;
    str.Format(_T("%c"), node->data);
    pDC->TextOut(x - 2 * m_nDeep / level, y - 2 * m_nDeep / level - 7, str);

    pDC->SelectObject(pOldFont);
    pDC->SelectObject(pOldPen);
    ReleaseDC(pDC);
}

BOOL CHXDlg::OnEraseBkgnd(CDC* pDC)
{
    CRect rect;
    GetClientRect(rect);
    CBrush brush(RGB(240, 240, 240));
    pDC->FillRect(&rect, &brush);
    return TRUE;
}

void CHXDlg::OnDestroy()
{
    OnBnClickedStop();
    CDialogEx::OnDestroy();
}

void CHXDlg::UpdateStatusBar(const std::wstring& text)
{
    if (m_wndStatusBar.GetSafeHwnd())
        m_wndStatusBar.SetWindowText(text.c_str());
}

void CHXDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);
    if (m_wndStatusBar.GetSafeHwnd())
        m_wndStatusBar.SetWindowPos(nullptr, 0, cy - 22, cx, 22, SWP_NOZORDER | SWP_NOACTIVATE);
}
