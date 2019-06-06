
// HXDlg.cpp : 实现文件
//
#include "stdafx.h"
#include "HX.h"
#include "HXDlg.h"
#include "afxdialogex.h"
#include <fstream>
#include <string>
#include <vector>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
using namespace std;
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


// CHXDlg 对话框




CHXDlg::CHXDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHXDlg::IDD, pParent)
	, input(_T(""))
	, sway(_T(""))
{
	tree = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	font.CreatePointFont(150, "Consolas");
	color = RGB(0, 0, 0);
	colume = row = deep = 0;
}

void CHXDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WAY, way);
	DDX_Control(pDX, IDC_RESULT, result);
	DDX_Control(pDX, IDC_CODE, code);
	DDX_Text(pDX, IDC_DEEP, deep);
	DDX_Text(pDX, IDC_INPUT, input);
	DDX_CBString(pDX, IDC_WAY, sway);
	DDX_Control(pDX, IDC_PICTURE, picture);
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
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CHXDlg 消息处理程序

BOOL CHXDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CRect rect;
	CString str[] = { "先","中","后" };
	for (size_t i = 0; i < 3; i++)way.InsertString(i, str[i] + "序遍历二叉树");
	sway = "先序遍历二叉树";
	result.GetClientRect(rect);
	result.InsertColumn(0, "遍历结果", LVCFMT_CENTER, rect.Width());
	result.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	code.GetClientRect(rect);
	code.InsertColumn(0, "代码", LVCFMT_CENTER, rect.Width());
	code.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	UpdateData(FALSE);
	OnSelchangeWay();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CHXDlg::OnPaint()
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
		CDC* pDC = picture.GetDC();
		CRect rect;
		picture.GetClientRect(rect);
		pDC->FillRect(rect, &CBrush(RGB(170, 170, 170)));
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CHXDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CHXDlg::OnBnClickedShow()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if (input == "")
	{
		MessageBox("请输入二叉树!", "警告", MB_ICONWARNING);
		return;
	}
	int n = 1;
	for (size_t i = 0; i < input.GetLength(); i++)
	{
		if (--n < 0)
		{
			MessageBox("输入不合法!", "错误", MB_ICONERROR);
			return;
		}
		if (input[i] != '#')n += 2;
	}
	if (n)
	{
		MessageBox("输入不合法!", "错误", MB_ICONERROR);
		return;
	}
	ofstream os("tree.txt");
	os.clear();
	os << input;
	os.close();
	OnBnClickedDisplay();
}


void CHXDlg::OnBnClickedDisplay()
{
	// TODO: 在此添加控件通知处理程序代码
	CDC* pDC = picture.GetDC();
	CRect rect;
	picture.GetClientRect(rect);
	pDC->FillRect(&rect, &CBrush(RGB(170, 170, 170)));
	Open();
	CreateBt(tree);
	deep = 1;
	BiTreeDepth(tree, 1, deep);
	Close();
	UpdateData(FALSE);
	ifstream is("tree.txt");
	string str;
	is >> str;
	MessageBox(str.c_str());
	line(tree);
	is.close();
	color = RGB(0, 0, 0);
	CreateTree(tree);
}


void CHXDlg::OnBnClickedStart()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if (tree == NULL)
	{
		MessageBox("请输入二叉树!", "错误", MB_ICONERROR);
		return;
	}
	if (Tree.empty())
	{
		result.DeleteAllItems();
		CTree = tree;
		colume = row = 0;
	}
	if (sway == "先序遍历二叉树")
	{
		color = RGB(255, 255, 0);
		SetTimer(0, 1000, NULL);
	}
	else if (sway == "中序遍历二叉树")
	{
		color = RGB(255, 255, 0);
		SetTimer(1, 1000, NULL);
	}
	else
	{
		color = RGB(0, 255, 255);
		CPostorder(tree);
		return;
	}
}


void CHXDlg::OnBnClickedStop()
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(0);
	KillTimer(1);
}


void CHXDlg::OnBnClickedClean()
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(0);
	KillTimer(1);
	color = RGB(0, 0, 0);
	Tree.clear();
	CTree = tree;
	result.DeleteAllItems();
	CreateTree(tree);
}


void CHXDlg::OnSelchangeWay()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	code.DeleteAllItems();
	string str, strf;
	CString space[] = { "","","    ","    ","        ","        ","        ","    ","" };
	ifstream file("Tree.cpp");
	if (sway == "先序遍历二叉树")str = "Preorder";
	else if (sway == "中序遍历二叉树")str = "Inorder";
	else str = "Postorder";
	while (getline(file, strf))if (strf.find(str) != strf.npos)
	{
		for (int i = 0; i < 9; i++)
		{
			code.InsertItem(i, space[i] + strf.c_str());
			getline(file, strf);
		}
		break;
	}
	file.close();
}
void CHXDlg::line(BiTree T, int level)
{
	if (T)
	{
		CDC* pDC = picture.GetDC();
		CRect rect;
		picture.GetClientRect(rect);
		if (T->lchild)T->lchild->adress = 2 * T->adress - 1;
		if (T->rchild)T->rchild->adress = 2 * T->adress;
		int x = rect.right * T->adress / (pow(2., level - 1) + 1), y = rect.bottom * level / (deep + 1);
		CPen pen;
		pen.CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
		pDC->SelectObject(&pen);
		if (level < deep)
		{
			pDC->MoveTo(x, y);
			if (T->lchild)pDC->LineTo(T->lchild->adress * rect.right / (pow(2., level) + 1), rect.bottom * (level + 1) / (deep + 1));
			pDC->MoveTo(x, y);
			if (T->rchild)pDC->LineTo(T->rchild->adress * rect.right / (pow(2., level) + 1), rect.bottom * (level + 1) / (deep + 1));
		}
		line(T->lchild, level + 1);
		line(T->rchild, level + 1);
	}
}

void CHXDlg::CreateTree(BiTree T, int level)
{
	if (T)
	{
		DrawTree(T, level);
		CreateTree(T->lchild, level + 1);
		CreateTree(T->rchild, level + 1);
	}
}

void CHXDlg::DrawTree(BiTree T, int level)
{
	CDC* pDC = picture.GetDC();
	CRect rect;
	CString str;
	picture.GetClientRect(rect);
	int x = rect.right * T->adress / (pow(2., level - 1) + 1), y = rect.bottom * level / (deep + 1);
	str.Format("%c", T->data);
	CPen pen;
	pen.CreatePen(PS_SOLID, 3, color);
	pDC->SelectObject(&pen);
	pDC->Ellipse(x - 10 * deep / level, y - 10 * deep / level, x + 10 * deep / level, y + 10 * deep / level);
	pDC->SelectObject(&font);
	pDC->SetBkMode(TRANSPARENT);
	pDC->TextOut(x - 2 * deep / level, y - 2 * deep / level - 7, str);
}

void CHXDlg::NRPreorder()
{
	if (CTree)
	{
		Tree.push_back(CTree);
		DrawTree(CTree, CTree->level);
		CString str;
		str.Format("%c", CTree->data);
		result.InsertItem(result.GetItemCount(), str);
		CTree = CTree->lchild;
	}
	else if (!Tree.empty())
	{
		CTree = Tree.back();
		Tree.pop_back();
		CTree = CTree->rchild;
	}
	else
	{
		Tree.clear();
		KillTimer(0);
	}
}

void CHXDlg::NRInorder()
{
	if (CTree)
	{
		Tree.push_back(CTree);
		CTree = CTree->lchild;
	}
	else if (!Tree.empty())
	{
		CTree = Tree.back();
		DrawTree(CTree, CTree->level);
		CString str;
		str.Format("%c", CTree->data);
		result.InsertItem(result.GetItemCount(), str);
		Tree.pop_back();
		CTree = CTree->rchild;
	}
	else
	{
		Tree.clear();
		KillTimer(1);
	}
}

void CHXDlg::CPostorder(BiTree T)
{
	if (T)
	{
		CPostorder(T->lchild);
		CPostorder(T->rchild);
		Sleep(1000);
		DrawTree(T, T->level);
	}
	if (T)
	{
		CString str;
		str.Format("%c", T->data);
		result.InsertItem(result.GetItemCount(), str);
	}
	else result.InsertItem(result.GetItemCount(), "NULL");
}
void CHXDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	static int Flag = 0;
	switch (nIDEvent)
	{
	case 0:
		if (CTree)
		{
			row = 0;
			code.SetItemState(6, 0, LVIS_SELECTED);
			int n[] = { 2,4,5 };
			code.SetItemState(n[colume - 1], 0, LVIS_SELECTED);
			code.SetFocus();
			if (colume == 3)colume = 0;
			code.SetItemState(n[colume++], LVIS_SELECTED, LVIS_SELECTED);
			if (colume == 2)NRPreorder();
			Flag = 1;
		}
		else if(Flag)
		{
			code.SetItemState(4, 0, LVIS_SELECTED);
			code.SetFocus();
			code.SetItemState(5, LVIS_SELECTED, LVIS_SELECTED);
			Flag = 0;
		}
		else
		{
			colume = 0;
			code.SetItemState(5, 0, LVIS_SELECTED);
			int n[] = { 2,10,6 };
			if (row != 1)
			{
				code.SetItemState(n[row - 1], 0, LVIS_SELECTED);
				code.SetFocus();
				if (row == 3)row = 0;
				code.SetItemState(n[row++], LVIS_SELECTED, LVIS_SELECTED);
			}
			else
			{
				row++;
				code.SetItemState(2, 0, LVIS_SELECTED);
				NRPreorder();
				result.InsertItem(result.GetItemCount(), "NULL");
			}
		}
		break;
	case 1:
		if (CTree)
		{
			row = 0;
			code.SetItemState(6, 0, LVIS_SELECTED);
			int n[] = { 2,4 };
			code.SetItemState(n[colume - 1], 0, LVIS_SELECTED);
			code.SetFocus();
			if (colume == 2)
			{
				colume = 0;
				NRInorder();
			}
			code.SetItemState(n[colume++], LVIS_SELECTED, LVIS_SELECTED);
			
		}
		else
		{
			colume = 0;
			code.SetItemState(2, 0, LVIS_SELECTED);
			int n[] = { 2,10,5,6 };
			if (row != 1)
			{
				code.SetItemState(n[row - 1], 0, LVIS_SELECTED);
				code.SetFocus();
				if (row == 4)row = 0;
				result.InsertItem(result.GetItemCount(), "NULL");
				code.SetItemState(n[row++], LVIS_SELECTED, LVIS_SELECTED);
			}
			else
			{
				row++;
				code.SetItemState(6, 0, LVIS_SELECTED);
				NRInorder();
			}
		}
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


BOOL CHXDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rect;
	GetClientRect(rect);
	pDC->FillRect(&rect, &CBrush(RGB(0, 255, 255)));
	return TRUE;
	return CDialogEx::OnEraseBkgnd(pDC);
}
