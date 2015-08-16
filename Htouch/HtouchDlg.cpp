
// HtouchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Htouch.h"
#include "HtouchDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About
CConsolePrinter *g_pCslPrt;

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CHtouchDlg dialog


CHtouchDlg::CHtouchDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHtouchDlg::IDD, pParent)
	, m_nDofIndex(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pModel = NULL;
}

void CHtouchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GB_MODEL, m_btnModelPanel);
	DDX_Text(pDX, IDC_DOF_NO, m_nDofIndex);
	DDV_MinMaxInt(pDX, m_nDofIndex, 0, 26);
}

BEGIN_MESSAGE_MAP(CHtouchDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
//	ON_COMMAND(ID_FILE_OPEN, &CHtouchDlg::OnFileOpen)
//	ON_COMMAND(ID_FILE_CLOSE, &CHtouchDlg::OnFileClose)
//	ON_COMMAND(ID_FILE_EXIT, &CHtouchDlg::OnFileExit)
	ON_COMMAND(ID_TOOLS_CONSOLEEXIT, &CHtouchDlg::OnConsoleExit)
	ON_COMMAND(ID_TOOLS_CONSOLEINPUT, &CHtouchDlg::OnConsoleInput)
	ON_COMMAND(ID_TOOLS_CONSOLEOUTPUT, &CHtouchDlg::OnConsoleOutput)
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_NORMALIZE_MODEL, &CHtouchDlg::OnNormalizeModel)
//	ON_COMMAND(ID_DETECT_HAND, &CHtouchDlg::OnDetectHand)
	ON_COMMAND(ID_SHOW_PROJECTION, &CHtouchDlg::OnShowProjection)
	ON_MESSAGE(WM_USER_REPAINT, OnUserRepaint)
	ON_BN_CLICKED(IDC_BUTTON_INCDOF, &CHtouchDlg::OnClickedButtonIncdof)
	ON_BN_CLICKED(IDC_BUTTON_DECDOF, &CHtouchDlg::OnClickedButtonDecdof)
END_MESSAGE_MAP()


// CHtouchDlg message handlers

BOOL CHtouchDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_dlgShowModel.Create(IDD_MODELPANEL_DIALOG, &m_btnModelPanel);
	
	CRect rtModelPanel;
	m_btnModelPanel.GetClientRect(&rtModelPanel);	
	rtModelPanel.top+=10; 
	rtModelPanel.bottom-=2; 
	rtModelPanel.left+=2; 
	rtModelPanel.right-=2; 
	m_dlgShowModel.MoveWindow(&rtModelPanel);	
	m_dlgShowModel.ShowScrollBar(SB_VERT, false);
	m_dlgShowModel.ShowScrollBar(SB_HORZ, false);
	m_dlgShowModel.ShowWindow(TRUE);

	// init the hand model
	m_pModel = new Hand();
	m_dlgShowModel.Reset();		
	m_dlgShowModel.Init();
	m_pModel->ParseFile("../Data/hand1.skel");
	m_dlgShowModel.AttachModel(m_pModel);

	m_nDofIndex = 0;
	UpdateData(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CHtouchDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CHtouchDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CHtouchDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CHtouchDlg::OnConsoleExit()
{
	// TODO: Add your command handler code here
	g_pCslPrt->Destroy();
}

void CHtouchDlg::OnConsoleInput()
{
	// TODO: Add your command handler code here
	g_pCslPrt->Destroy();
	g_pCslPrt->Instance(true);
}


void CHtouchDlg::OnConsoleOutput()
{
	// TODO: Add your command handler code here
	g_pCslPrt->Destroy();
	g_pCslPrt->Instance(false);
}

BOOL CHtouchDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	m_dlgShowModel.Zoom(nFlags, zDelta, pt);

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}


void CHtouchDlg::OnNormalizeModel()
{
	// TODO: Add your command handler code here
	if (m_pModel->IsInit())
	{
		m_pModel->Normalize();
	}
}

void CHtouchDlg::OnShowProjection()
{
	// TODO: Add your command handler code here	
	m_dlgShowModel.ShowProjection();
}


LRESULT CHtouchDlg::OnUserRepaint(WPARAM w, LPARAM l)
{
	m_dlgShowModel.UserRepaint();
	return 0;
}

BOOL CHtouchDlg::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_pModel != NULL)
	{
		delete m_pModel;
	}
	m_dlgShowModel.Reset();	
	return CDialogEx::DestroyWindow();
}


void CHtouchDlg::OnClickedButtonIncdof()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	double fInc;
	if (m_nDofIndex >= 0 && m_nDofIndex < 3)
		fInc = 1.0;
	else
		fInc = 0.1;
	double fOrg = m_pModel->GetDOF(m_nDofIndex);
	m_pModel->SetDOF(fInc + fOrg, m_nDofIndex);
	printf("%lf, %lf\n", fOrg, fInc); 
}


void CHtouchDlg::OnClickedButtonDecdof()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	double fDec;
	if (m_nDofIndex >= 0 && m_nDofIndex < 3)
		fDec = 1.0;
	else
		fDec = 0.1;
	double fOrg = m_pModel->GetDOF(m_nDofIndex);
	m_pModel->SetDOF(fOrg - fDec, m_nDofIndex);
	printf("%lf, %lf\n", fOrg, fDec); 
}
