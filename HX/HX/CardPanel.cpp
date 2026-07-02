#include "stdafx.h"
#include "CardPanel.h"

IMPLEMENT_DYNAMIC(CCardPanel, CStatic)

CCardPanel::CCardPanel()
    : m_nCornerRadius(10)
    , m_crBackground(RGB(255, 255, 255))
    , m_crBorder(RGB(218, 220, 224))
    , m_crTitle(RGB(60, 64, 67))
{
}

CCardPanel::~CCardPanel()
{
}

BEGIN_MESSAGE_MAP(CCardPanel, CStatic)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
    ON_WM_NCHITTEST()
END_MESSAGE_MAP()

void CCardPanel::SetTitle(LPCTSTR lpszTitle)
{
    m_strTitle = lpszTitle;
    Invalidate();
}

void CCardPanel::SetCornerRadius(int nRadius)
{
    m_nCornerRadius = nRadius;
    Invalidate();
}

void CCardPanel::SetColors(COLORREF crBackground, COLORREF crBorder)
{
    m_crBackground = crBackground;
    m_crBorder = crBorder;
    Invalidate();
}

BOOL CCardPanel::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}

void CCardPanel::OnPaint()
{
    CPaintDC dc(this);
    CRect rect;
    GetClientRect(rect);

    CBrush brushBg(m_crBackground);
    CPen penBorder(PS_SOLID, 1, m_crBorder);
    CPen* pOldPen = dc.SelectObject(&penBorder);
    CBrush* pOldBrush = dc.SelectObject(&brushBg);

    dc.RoundRect(rect, CPoint(m_nCornerRadius, m_nCornerRadius));

    dc.SelectObject(pOldBrush);
    dc.SelectObject(pOldPen);

    if (!m_strTitle.IsEmpty())
    {
        CFont titleFont;
        titleFont.CreatePointFont(95, _T("Segoe UI Semibold"));
        CFont* pOldFont = dc.SelectObject(&titleFont);
        dc.SetTextColor(m_crTitle);
        dc.SetBkMode(TRANSPARENT);

        CRect titleRect(rect.left + 14, rect.top + 6, rect.right - 14, rect.top + 26);
        dc.DrawText(m_strTitle, titleRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

        dc.SelectObject(pOldFont);
    }
}

LRESULT CCardPanel::OnNcHitTest(CPoint point)
{
    return HTTRANSPARENT;
}
