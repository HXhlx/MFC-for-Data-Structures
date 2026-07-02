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
    CRect rect;
    GetClientRect(rect);

    CRect cardScreen;
    GetWindowRect(&cardScreen);

    HWND hParent = GetParent()->GetSafeHwnd();
    HWND hChild = ::GetWindow(hParent, GW_CHILD);
    while (hChild)
    {
        if (hChild != GetSafeHwnd() && ::IsWindowVisible(hChild))
        {
            CRect childScreen;
            ::GetWindowRect(hChild, &childScreen);
            CRect childInCard;
            childInCard.left = childScreen.left - cardScreen.left;
            childInCard.top = childScreen.top - cardScreen.top;
            childInCard.right = childScreen.right - cardScreen.left;
            childInCard.bottom = childScreen.bottom - cardScreen.top;

            if (childInCard.left < rect.right && childInCard.right > 0 &&
                childInCard.top < rect.bottom && childInCard.bottom > 0)
            {
                pDC->ExcludeClipRect(&childInCard);
            }
        }
        hChild = ::GetWindow(hChild, GW_HWNDNEXT);
    }

    CBrush brushBg(m_crBackground);
    CPen penBorder(PS_SOLID, 1, m_crBorder);
    CPen* pOldPen = pDC->SelectObject(&penBorder);
    CBrush* pOldBrush = pDC->SelectObject(&brushBg);

    pDC->RoundRect(rect, CPoint(m_nCornerRadius, m_nCornerRadius));

    pDC->SelectObject(pOldBrush);
    pDC->SelectObject(pOldPen);

    return TRUE;
}

void CCardPanel::OnPaint()
{
    CPaintDC dc(this);

    if (!m_strTitle.IsEmpty())
    {
        CFont titleFont;
        titleFont.CreatePointFont(95, _T("Segoe UI Semibold"));
        CFont* pOldFont = dc.SelectObject(&titleFont);
        dc.SetTextColor(m_crTitle);
        dc.SetBkMode(TRANSPARENT);

        CRect rect;
        GetClientRect(rect);
        CRect titleRect(rect.left + 14, rect.top + 6, rect.right - 14, rect.top + 26);
        dc.DrawText(m_strTitle, titleRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

        dc.SelectObject(pOldFont);
    }
}

LRESULT CCardPanel::OnNcHitTest(CPoint point)
{
    return HTTRANSPARENT;
}
