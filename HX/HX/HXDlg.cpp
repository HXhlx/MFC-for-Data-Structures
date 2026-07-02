
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
    m_codeFont.CreatePointFont(100, _T("Consolas"));
    m_labelFont.CreatePointFont(90, _T("Segoe UI Semibold"));
    m_brushDialogBg.CreateSolidBrush(RGB(240, 242, 245));
    m_brushCodeBg.CreateSolidBrush(RGB(250, 251, 252));
}

void CHXDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_WAY, m_wndWay);
    DDX_Control(pDX, IDC_RESULT, m_wndResult);
    DDX_Control(pDX, IDC_CODE, m_wndCode);
    DDX_Text(pDX, IDC_INPUT, m_strInput);
    DDX_Control(pDX, IDC_PICTURE, m_wndPicture);
    DDX_Control(pDX, IDC_CARD_INPUT, m_cardInput);
    DDX_Control(pDX, IDC_CARD_TRAVERSAL, m_cardTraversal);
    DDX_Control(pDX, IDC_CARD_CANVAS, m_cardCanvas);
    DDX_Control(pDX, IDC_CARD_RESULT, m_cardResult);
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
    ON_WM_CTLCOLOR()
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

    m_cardInput.SetTitle(_T("\u8f93\u5165"));
    m_cardTraversal.SetTitle(_T("\u904d\u5386\u4ee3\u7801"));
    m_cardCanvas.SetTitle(_T("\u753b\u5e03"));
    m_cardResult.SetTitle(_T("\u904d\u5386\u7ed3\u679c"));

    CWnd* pWnd = nullptr;
    pWnd = GetDlgItem(IDC_SHOW);
    if (pWnd) pWnd->SetWindowText(_T("\u663e\u793a\u4e8c\u53c9\u6811"));
    pWnd = GetDlgItem(IDC_DISPLAY);
    if (pWnd) pWnd->SetWindowText(_T("\u6f14\u793a\u5b9e\u4f8b"));
    pWnd = GetDlgItem(IDC_START);
    if (pWnd) pWnd->SetWindowText(_T("\u5f00\u59cb\u904d\u5386"));
    pWnd = GetDlgItem(IDC_STOP);
    if (pWnd) pWnd->SetWindowText(_T("\u505c\u6b62"));
    pWnd = GetDlgItem(IDC_CLEAN);
    if (pWnd) pWnd->SetWindowText(_T("\u6e05\u7a7a\u904d\u5386"));

    m_wndWay.ResetContent();
    m_wndWay.InsertString(0, _T("\u5148\u5e8f\u904d\u5386\u4e8c\u53c9\u6811"));
    m_wndWay.InsertString(1, _T("\u4e2d\u5e8f\u904d\u5386\u4e8c\u53c9\u6811"));
    m_wndWay.InsertString(2, _T("\u540e\u5e8f\u904d\u5386\u4e8c\u53c9\u6811"));
    m_wndWay.SetCurSel(0);
    m_wndWay.SetFont(&m_labelFont);

    CRect rect;
    m_wndResult.GetClientRect(rect);
    m_wndResult.InsertColumn(0, _T("\u904d\u5386\u7ed3\u679c"), LVCFMT_CENTER, rect.Width());
    m_wndResult.SetExtendedStyle(LVS_EX_FULLROWSELECT);

    m_wndCode.SetFont(&m_codeFont);

    OnSelchangeWay();

    m_btnShow.SubclassDlgItem(IDC_SHOW, this);
    m_btnDisplay.SubclassDlgItem(IDC_DISPLAY, this);
    m_btnStart.SubclassDlgItem(IDC_START, this);
    m_btnStop.SubclassDlgItem(IDC_STOP, this);
    m_btnClean.SubclassDlgItem(IDC_CLEAN, this);

    CRect rc;
    GetClientRect(&rc);
    m_wndStatusBar.Create(_T("Ready"), WS_CHILD | WS_VISIBLE | SS_LEFTNOWORDWRAP | SS_SUNKEN,
        CRect(0, rc.bottom - 24, rc.right, rc.bottom), this);
    m_wndStatusBar.SetFont(GetFont());
    m_wndStatusBar.SetWindowText(_T("\u51c6\u5907\u5c31\u7eea"));

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
    SetDlgItemText(IDC_DEEP, std::to_wstring(m_nDeep).c_str());

    RedrawTree();
    LOG("Tree created, depth=" + std::to_string(m_nDeep));
    UpdateStatusBar(std::format(L"\u5df2\u52a0\u8f7d\u4e8c\u53c9\u6811  |  \u8282\u70b9\u6570: {}  |  \u6df1\u5ea6: {}", CountNodes(m_tree.get()), m_nDeep));
}

void CHXDlg::OnBnClickedDisplay()
{
    GetDlgItem(IDC_INPUT)->SetWindowText(_T("abc#d####"));
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

    const wchar_t* typeNames[] = { L"\u5148\u5e8f", L"\u4e2d\u5e8f", L"\u540e\u5e8f" };
    int ti = static_cast<int>(m_nCurrentType);
    UpdateStatusBar(std::format(L"\u6b63\u5728\u904d\u5386 ({})  |  \u6b65\u9aa4 0 / {}", typeNames[ti], m_state.steps.size()));
}

void CHXDlg::OnBnClickedStop()
{
    if (m_state.isRunning)
    {
        KillTimer(TIMER_ID);
        m_state.isRunning = false;
        RedrawTree();
        UpdateStatusBar(L"\u5df2\u505c\u6b62");
    }
}

void CHXDlg::OnBnClickedClean()
{
    OnBnClickedStop();
    m_wndResult.DeleteAllItems();
    m_state.steps.clear();
    m_state.currentIndex = 0;
    m_nodeSteps.clear();
    RedrawTree();
    UpdateStatusBar(L"\u51c6\u5907\u5c31\u7eea");
}

void CHXDlg::OnSelchangeWay()
{
    int selIdx = m_wndWay.GetCurSel();
    if (selIdx == CB_ERR)
        selIdx = 0;

    auto type = static_cast<TraversalType>(selIdx);
    LOG("OnSelchangeWay called, sel=" + std::to_string(selIdx));

    std::wstring code = GetTraversalCode(type);
    CString win32Text;
    for (size_t i = 0; i < code.size(); i++)
    {
        if (code[i] == L'\n' && (i == 0 || code[i - 1] != L'\r'))
            win32Text += L"\r\n";
        else
            win32Text += code[i];
    }
    m_wndCode.SetWindowText(win32Text);
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

            const wchar_t* typeNames[] = { L"\u5148\u5e8f", L"\u4e2d\u5e8f", L"\u540e\u5e8f" };
            int ti = static_cast<int>(m_nCurrentType);
            UpdateStatusBar(std::format(L"\u6b63\u5728\u904d\u5386 ({})  |  \u6b65\u9aa4 {} / {}", typeNames[ti], m_state.currentIndex + 1, m_state.steps.size()));

            m_state.currentIndex++;
        }
        else
        {
            KillTimer(TIMER_ID);
            m_state.isRunning = false;
            RedrawTree();
            UpdateStatusBar(std::format(L"\u904d\u5386\u5b8c\u6210  |  \u5171\u8bbf\u95ee {} \u4e2a\u8282\u70b9", m_state.steps.size()));
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

    int radius = max(10 * m_nDeep / level, 8);

    CPen pen(PS_SOLID, 2, RGB(70, 90, 120));
    CBrush brush(color);
    CPen* pOldPen = pDC->SelectObject(&pen);
    CBrush* pOldBrush = pDC->SelectObject(&brush);
    pDC->Ellipse(x - radius, y - radius, x + radius, y + radius);
    pDC->SelectObject(pOldBrush);
    pDC->SelectObject(pOldPen);

    CFont* pOldFont = pDC->SelectObject(&m_font);
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(RGB(255, 255, 255));

    CString str;
    str.Format(_T("%c"), node->data);
    pDC->SetTextAlign(TA_CENTER | TA_BASELINE);
    pDC->TextOut(x, y + 5, str);

    pDC->SelectObject(pOldFont);
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

    CPen pen(PS_SOLID, 2, RGB(95, 99, 104));
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
    CBrush brush(RGB(250, 251, 252));
    pDC->FillRect(&rect, &brush);

    CPen pen(PS_SOLID, 1, RGB(218, 220, 224));
    CPen* pOldPen = pDC->SelectObject(&pen);
    pDC->MoveTo(rect.left, rect.top);
    pDC->LineTo(rect.right, rect.top);
    pDC->SelectObject(pOldPen);

    ReleaseDC(pDC);

    if (m_tree)
    {
        DrawLines(m_tree.get(), 1);
        DrawTreeRecursive(m_tree.get(), 1, RGB(74, 144, 217));
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

    int radius = max(12 * m_nDeep / level, 10);

    CPen pen(PS_SOLID, 3, RGB(180, 40, 30));
    CBrush brush(color);
    CPen* pOldPen = pDC->SelectObject(&pen);
    CBrush* pOldBrush = pDC->SelectObject(&brush);
    pDC->Ellipse(x - radius, y - radius, x + radius, y + radius);
    pDC->SelectObject(pOldBrush);
    pDC->SelectObject(pOldPen);

    CFont* pOldFont = pDC->SelectObject(&m_font);
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(RGB(255, 255, 255));

    CString str;
    str.Format(_T("%c"), node->data);
    pDC->SetTextAlign(TA_CENTER | TA_BASELINE);
    pDC->TextOut(x, y + 5, str);

    pDC->SelectObject(pOldFont);
    ReleaseDC(pDC);
}

BOOL CHXDlg::OnEraseBkgnd(CDC* pDC)
{
    CRect rect;
    GetClientRect(rect);
    pDC->FillRect(&rect, &m_brushDialogBg);
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
    {
        m_wndStatusBar.Invalidate();
        m_wndStatusBar.SetWindowText(text.c_str());
        m_wndStatusBar.UpdateWindow();
    }
}

void CHXDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);
    if (m_wndStatusBar.GetSafeHwnd())
        m_wndStatusBar.SetWindowPos(nullptr, 0, cy - 24, cx, 24, SWP_NOZORDER | SWP_NOACTIVATE);
}

HBRUSH CHXDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

    if (pWnd->GetSafeHwnd() == m_wndStatusBar.GetSafeHwnd())
    {
        pDC->SetBkColor(RGB(240, 242, 245));
        pDC->SetTextColor(RGB(60, 64, 67));
        pDC->SetBkMode(OPAQUE);
        return (HBRUSH)m_brushDialogBg;
    }

    if (pWnd->GetDlgCtrlID() == IDC_CODE)
    {
        pDC->SetBkColor(RGB(250, 251, 252));
        pDC->SetTextColor(RGB(60, 64, 67));
        return (HBRUSH)m_brushCodeBg;
    }

    if (nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkMode(TRANSPARENT);
        pDC->SetTextColor(RGB(60, 64, 67));
        return (HBRUSH)GetStockObject(NULL_BRUSH);
    }

    return hbr;
}
