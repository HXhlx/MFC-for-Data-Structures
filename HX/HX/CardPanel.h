#pragma once

class CCardPanel : public CStatic
{
    DECLARE_DYNAMIC(CCardPanel)

public:
    CCardPanel();
    virtual ~CCardPanel();

    void SetTitle(LPCTSTR lpszTitle);
    void SetCornerRadius(int nRadius);
    void SetColors(COLORREF crBackground, COLORREF crBorder);

protected:
    CString m_strTitle;
    CFont m_titleFont;
    int m_nCornerRadius;
    COLORREF m_crBackground;
    COLORREF m_crBorder;
    COLORREF m_crTitle;

    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg LRESULT OnNcHitTest(CPoint point);
    DECLARE_MESSAGE_MAP()
};
