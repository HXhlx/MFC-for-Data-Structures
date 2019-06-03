
// HXDlg.cpp : ʵ���ļ�
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
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CHXDlg �Ի���




CHXDlg::CHXDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHXDlg::IDD, pParent)
	, input(_T(""))
	, sway(_T(""))
{
	tree = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	deep = 0;
	font.CreatePointFont(150, "Consolas");
	color = RGB(0, 0, 0);
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
END_MESSAGE_MAP()


// CHXDlg ��Ϣ�������

BOOL CHXDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	CRect rect;
	CString str[] = { "��","��","��" };
	for (size_t i = 0; i < 3; i++)way.InsertString(i, str[i] + "�����������");
	sway = "�������������";
	result.GetClientRect(rect);
	result.InsertColumn(0, "�������", LVCFMT_CENTER, rect.Width());
	result.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	code.GetClientRect(rect);
	code.InsertColumn(0, "����", LVCFMT_CENTER, rect.Width());
	code.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	UpdateData(FALSE);
	OnSelchangeWay();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CHXDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CHXDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CHXDlg::OnBnClickedShow()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	if (input == "")
	{
		MessageBox("�����������!", "����", MB_ICONWARNING);
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	if (tree == NULL)
	{
		MessageBox("�����������!", "����", MB_ICONERROR);
		return;
	}
	if(sway=="�������������")
	{
		color = RGB(0, 255, 255);
		CPostorder(tree);
		return;
	}
	if(Tree.empty())result.DeleteAllItems();
	SetTimer(0, 1000, NULL);
}


void CHXDlg::OnBnClickedStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	KillTimer(0);
}


void CHXDlg::OnBnClickedClean()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	color = RGB(0, 0, 0);
	CreateTree(tree);
}


void CHXDlg::OnSelchangeWay()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	code.DeleteAllItems();
	string str, strf;
	CString space[] = { "","","    ","    ","        ","        ","        ","    ","" };
	ifstream file("Tree.cpp");
	if (sway == "�������������")str = "Preorder";
	else if (sway == "�������������")str = "Inorder";
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
	static BiTree T = tree;
	if (T)
	{
		Tree.push_back(T);
		DrawTree(T, T->level);
		CString str;
		str.Format("%c", T->data);
		result.InsertItem(result.GetItemCount(), str);
		T = T->lchild;
	}
	else if (!Tree.empty())
	{
		result.InsertItem(result.GetItemCount(), "NULL");
		T = Tree.back();
		Tree.pop_back();
		T = T->rchild;
	}
	else
	{
		result.InsertItem(result.GetItemCount(), "NULL");
		Tree.clear();
		KillTimer(0);
	}
}

void CHXDlg::NRInorder()
{
	static BiTree T = tree;
	static bool flag = true;
	if (T)
	{
		Tree.push_back(T);
		T = T->lchild;
	}
	else if (!Tree.empty())
	{
		result.InsertItem(result.GetItemCount(), "NULL");
		T = Tree.back();
		DrawTree(T, T->level);
		CString str;
		str.Format("%c", T->data);
		result.InsertItem(result.GetItemCount(), str);
		Tree.pop_back();
		T = T->rchild;
	}
	else
	{
		result.InsertItem(result.GetItemCount(), "NULL");
		Tree.clear();
		KillTimer(0);
	}
}

void CHXDlg::CPostorder(BiTree T)
{
	if (T)
	{
		CString str;
		str.Format("%c", T->data);
		result.InsertItem(result.GetItemCount(), str);
	}
	else result.InsertItem(result.GetItemCount(), "NULL");
	if (T)
	{
		CPostorder(T->lchild);
		CPostorder(T->rchild);
		Sleep(1000);
		DrawTree(T, T->level);
	}
}
void CHXDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (sway == "�������������")
	{
		color = RGB(255, 255, 0);
		NRPreorder();
	}
	else if (sway == "�������������")
	{
		color = RGB(255, 0, 255);
		NRInorder();
	}
	CDialogEx::OnTimer(nIDEvent);
}
