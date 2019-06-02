
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
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	deep = 0;
	font.CreatePointFont(150, "Consolas");
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
	ON_BN_CLICKED(IDC_STEP, &CHXDlg::OnBnClickedStep)
	ON_BN_CLICKED(IDC_CLEAN, &CHXDlg::OnBnClickedClean)
	ON_CBN_SELCHANGE(IDC_WAY, &CHXDlg::OnSelchangeWay)
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
		pDC->FillRect(rect, &CBrush(RGB(0, 255, 255)));
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
	if (input=="")
	{
		MessageBox("请输入二叉树!");
		return;
	}
	ofstream os("tree.txt");
	os.clear();
	os << input;
	os.close();
	Open();
	CreateBt(tree);
	deep = 1;
	BiTreeDepth(tree, 1, deep);
	Close();
	is.open("tree.txt");
	CPreorder(tree);
	is.close();
	UpdateData(FALSE);
}


void CHXDlg::OnBnClickedDisplay()
{
	// TODO: 在此添加控件通知处理程序代码
	Open();
	CreateBt(tree);
	deep = 1;
	BiTreeDepth(tree, 1, deep);
	Close();
	is.open("tree.txt");
	string str;
	is >> str;
	UpdateData(FALSE);
	MessageBox(str.c_str());
	is.close();
	is.open("tree.txt");
	CPreorder(tree);
	is.close();
}


void CHXDlg::OnBnClickedStart()
{
	// TODO: 在此添加控件通知处理程序代码
	
}


void CHXDlg::OnBnClickedStop()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CHXDlg::OnBnClickedStep()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CHXDlg::OnBnClickedClean()
{
	// TODO: 在此添加控件通知处理程序代码
	is.open("tree.txt");
	CPreorder(tree);
	is.close();
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
void CHXDlg::CPreorder(BiTree T, int level)
{
	static int* order = new int[deep]();
	if (T)
	{
		order[level - 1]++;
		char ch;
		CDC* pDC = picture.GetDC();
		CRect rect;
		CString str;
		picture.GetClientRect(rect);
		int x = rect.right * (2 * order[level - 1] - 1) / pow(2., level), y = rect.bottom * level / (deep + 1);
		do is >> ch;
		while (ch == '#');
		str.Format("%c", ch);
		CPen pen;
		pen.CreatePen(PS_SOLID, 3, RGB(255, 255, 0));
		pDC->SelectObject(&pen);
		if (level > 1)
		{
			pDC->MoveTo(x, y);
			pDC->LineTo((2 * ((order[level - 1] + 1) / 2) - 1) * rect.right / pow(2., level - 1), rect.bottom * (level - 1) / (deep + 1));
		}
		pDC->Ellipse(x - 10 * deep / level, y - 10 * deep / level, x + 10 * deep / level, y + 10 * deep / level);
		pDC->SelectObject(&font);
		pDC->SetBkMode(TRANSPARENT);
		pDC->TextOut(x - 2 * deep / level, y - 2 * deep / level - 7, str);
		CPreorder(T->lchild, level + 1);
		CPreorder(T->rchild, level + 1);
	}
}