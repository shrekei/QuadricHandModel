// ShowModelPanel.cpp : implementation file
//

#include "stdafx.h"
#include "Htouch.h"
#include "ShowModelPanel.h"
#include "afxdialogex.h"
#include "ConsolePrinter.h"
#include <sys/timeb.h>

// CShowModelPanel dialog/////////////////////////////////////////////////////////////////////////////
extern CConsolePrinter *g_pCslPrt;

GLfloat arrufAmbientLight[4] = {0.3f, 0.3f, 0.3f, 1.0f};
GLfloat arrufDiffuseLight[4] = {0.5f, 0.5f, 0.5f, 1.0f};
GLfloat arrufSpecular[4] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat arrufLightPos[4] = { 100.0f, 100.0f, 100.0f, 1.0f}; //light position: positive along y-axis 
GLfloat arrufSpecref[4] = {0.8f, 0.8f, 0.8f, 1.0f};
GLfloat arrufSphereColor[4] = {1.0f, 1.0f, 1.0f};

GLfloat arrufSkinMatAmbient[4]  = {0.2f, 0.4f, 0.5f, 1.0f};
GLfloat arrufSkinMatDiffuse[4]  = {0.2f, 0.4f, 0.5f, 1.0f};
GLfloat arrufSkinMatSpecular[4] = {0.0f, 0.0f, 0.2f, 1.0f};
GLfloat arrufSkinMatEmission[4] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat arrufSkinMatShininess  = 30.0f;

GLfloat arrufMarbleMatAmbient[4] = {0.1f, 0.5f, 0.1f, 1.0f};
GLfloat arrufMarbleMatDiffuse[4]  = {0.1f, 0.5f, 0.1f, 1.0f};
GLfloat arrufMarbleMatSpecular[4] = {0.0f, 0.0f, 0.2f, 1.0f};
GLfloat arrufMarbleMatEmission[4] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat arrufMarbleMatShininess  = 30.0f;

GLUquadricObj *CShowModelPanel::s_pGluQuadricObj;

IMPLEMENT_DYNAMIC(CShowModelPanel, CDialog)

CShowModelPanel::CShowModelPanel(CWnd* pParent /*=NULL*/)
	: CDialog(CShowModelPanel::IDD, pParent)
{
}

CShowModelPanel::~CShowModelPanel()
{
}

void CShowModelPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CShowModelPanel, CDialog)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()


// CShowModelPanel message handlers

void CShowModelPanel::Init(void)
{
	// init the DC and RC	
	m_pDC = ::GetDC(m_hWnd);
	SetupPixelFormat();
	m_pGLRC = wglCreateContext(m_pDC);
	wglMakeCurrent(m_pDC, m_pGLRC);	

	// set the parameters for rendering
	s_pGluQuadricObj = gluNewQuadric();
	gluQuadricNormals(s_pGluQuadricObj, GLU_SMOOTH); //smooth normals
	gluQuadricTexture(s_pGluQuadricObj, GL_TRUE); // using texture

	// init the viewpoint
	m_gGlobalCam.Init();
	GetPanelSize(m_nWidth, m_nHeight);
	m_gGlobalCam.SetAspect(double(m_nWidth)/double(m_nHeight));

	// init the flags
	m_bLButtonDown = false;
	m_bRButtonDown = false;
	m_bIsInit = true;

	// init the gr pointer
	m_pModel = NULL;
	m_gProjector.SetTSize(cv::Size(320, 240));
}

void CShowModelPanel::Reset(void)
{	
	if (m_bIsInit == true)
	{
		// clear the background to blue
		glClearColor(0.0f,0.0f,1.0f,1.0f); 

		// disable the enabled mode
		if( glIsEnabled(GL_DEPTH_TEST) )
		{
			glDisable(GL_DEPTH_TEST);
		}
		if( glIsEnabled(GL_LIGHTING) )
		{
			glDisable(GL_LIGHTING);
		}

		// clear DC, RC and quadric object
		wglMakeCurrent(NULL, NULL); //clear DC and RC for the current thread
		wglDeleteContext(m_pGLRC);	//delete the RC
		::ReleaseDC(m_hWnd, m_pDC); //release the DC and RC
		if (s_pGluQuadricObj != NULL)
		{
			s_pGluQuadricObj = NULL;
		}

		m_nWidth = m_nHeight = 0;
		m_vEyePos.Zero();
		m_bLButtonDown = false;
		m_bRButtonDown = false;
		m_gGlobalCam.Reset();
		m_pModel = NULL;
	}
}

void CShowModelPanel::OnLButtonDown(UINT nFlags, CPoint p)
{
	// TODO: Add your message handler code here and/or call default
	if (nFlags & MK_CONTROL)
	{
	}
	else
	{
		CClientDC dc(this);
		dc.DPtoLP(&p);
		m_cvMousePos.x = p.x;
		m_cvMousePos.y = p.y;	
		m_bLButtonDown = true;
	}
	CDialog::OnLButtonDown(nFlags, p);
}


void CShowModelPanel::OnMouseMove(UINT nFlags, CPoint p)
{
	// TODO: Add your message handler code here and/or call default
	if (m_bLButtonDown == true)
	{
		int dx = p.x - m_cvMousePos.x;
		int dy = -(p.y - m_cvMousePos.y);
		m_cvMousePos.x = p.x;
		m_cvMousePos.y = p.y;
		
		const double rate = 1.0f;
		m_gGlobalCam.SetAzimuth(m_gGlobalCam.GetAzimuth() + dx * rate);
		m_gGlobalCam.SetIncline(m_gGlobalCam.GetIncline() - dy * rate);		
	}
	if (m_bRButtonDown == true)
	{
		int dx = p.x - m_cvMousePos.x;
		int dy = -(p.y - m_cvMousePos.y);
		m_cvMousePos.x = p.x;
		m_cvMousePos.y = p.y;
		
		const double rate = 1.0f;
		m_gGlobalCam.SetTranslation(dx, dy);
	}

	CDialog::OnMouseMove(nFlags, p);
}


void CShowModelPanel::OnLButtonUp(UINT nFlags, CPoint p)
{
	// TODO: Add your message handler code here and/or call default
	m_bLButtonDown = false;
	CDialog::OnLButtonUp(nFlags, p);
}


void CShowModelPanel::Zoom(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	if (nFlags & MK_CONTROL)
	{
		const double fRate = 0.1f;
		bool bZoom = zDelta > 0 ? true : false;
		double dblZoomScale = zDelta > 0 ? (zDelta * 1.2/ WHEEL_DELTA) : -1 * (zDelta * 1.2/ WHEEL_DELTA);
		if (bZoom)
		{
			m_gGlobalCam.SetDistance(m_gGlobalCam.GetDistance() * (1.0f - dblZoomScale * fRate));
		}
		else
		{
			m_gGlobalCam.SetDistance(m_gGlobalCam.GetDistance() * (1.0f + dblZoomScale * fRate));
		}
	}
	Invalidate();
}


void CShowModelPanel::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	Reset();	
	CDialog::OnClose();
}

void CShowModelPanel::DrawModel(void)
{	
	// Do not call CDialog::OnPaint() for painting messages	
	// set the surface para for the skin
	glMaterialfv(GL_FRONT, GL_AMBIENT,   arrufSkinMatAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE,   arrufSkinMatDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR,  arrufSkinMatSpecular);
	glMaterialfv(GL_FRONT, GL_EMISSION,  arrufSkinMatEmission);
	glMaterialf	(GL_FRONT, GL_SHININESS, arrufSkinMatShininess);

	// draw the Hand	
	if (m_pModel != NULL && m_pModel->IsInit() == true)
	{
		glEnable(GL_COLOR_MATERIAL);
		bmu::Vector4 vSkinColor = m_pModel->GetSkinColor();
		glColor4f(vSkinColor.x, vSkinColor.y, vSkinColor.z, vSkinColor.w);
		glPushMatrix();
		m_pModel->Draw(s_pGluQuadricObj);
		glPopMatrix();
		glDisable(GL_COLOR_MATERIAL);
	}
}

void CShowModelPanel::GetPanelSize(int &nWidth, int &nHeight)
{		
	CWnd *pDlgWnd = this;
	CRect rcPanel; ;
	pDlgWnd->GetClientRect(&rcPanel);
	nWidth = rcPanel.Width();
	nHeight = rcPanel.Height();
}

int CShowModelPanel::SetRCParameters(void)
{
	//set the background color	
	glClearColor(0.3,0.3,0.3,1);
	glClear(GL_COLOR_BUFFER_BIT);

	//enable depth test
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set the viewing environment
	m_gGlobalCam.Draw();

	// set the lighting condition
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, arrufAmbientLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, arrufAmbientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, arrufDiffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, arrufSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, arrufLightPos);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	return 0;
}

int CShowModelPanel::SetupPixelFormat(void)
{
	int nPixelFormat; 

	PIXELFORMATDESCRIPTOR pFD =
	{ 
		sizeof(PIXELFORMATDESCRIPTOR),		// size of pfd
			1,								// version number
			PFD_DRAW_TO_WINDOW |			// support window
			PFD_SUPPORT_OPENGL |			// support OpenGL
			PFD_DOUBLEBUFFER,				// double buffered
			PFD_TYPE_RGBA,					// RGBA type
			24,								// 24-bit color
			0, 0, 0, 0, 0, 0,				// color bits ignored
			0,								// no alpha buffer
			0,								// shift bit ignored
			0,								// no accumulation buffer
			0, 0, 0, 0,						// accum bits ignored
			32,								// 32-bit z-buffer
			0,								// no stencil buffer
			0,								// no auxiliary buffer
			PFD_MAIN_PLANE,					// main layer
			0,								// reserved
			0, 0, 0							// layer masks ignored
	};
	
	if((nPixelFormat = ChoosePixelFormat(m_pDC, &pFD)) == 0)
	{
		MessageBox("PixelFormat Initialization Failed", NULL, MB_OK);
		return 1;
	}	
	if(SetPixelFormat(m_pDC, nPixelFormat, &pFD) == FALSE)
	{
		MessageBox("PixelFormat Initialization Failed", NULL, MB_OK);
		return 1;
	}	
	return 0;
}


void CShowModelPanel::OnPaint()
{
//	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
	
	SetRCParameters();
	DrawScene();
	DrawModel();

	SwapBuffers(m_pDC);	
}


BOOL CShowModelPanel::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	Init();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CShowModelPanel::UserRepaint(void)
{
	m_pModel->Update();
}

void CShowModelPanel::DrawScene(void)
{
	DrawBasement();
//	DrawAxes();
}

void CShowModelPanel::DrawAxes(void)
{
	glPushMatrix();
	glBegin(GL_LINES);
	glVertex3f(-100, 0, 0);
	glVertex3f(100, 0, 0);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(0, -100, 0);
	glVertex3f(0, 100, 0);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(0, 0, -100);
	glVertex3f(0, 0, 100);
	glEnd();
	glPopMatrix();
}

void CShowModelPanel::DrawBasement(void)
{
	// set the surface para for the basement
	glMaterialfv(GL_FRONT, GL_AMBIENT,   arrufMarbleMatAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE,   arrufMarbleMatDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR,  arrufMarbleMatSpecular);
	glMaterialfv(GL_FRONT, GL_EMISSION,  arrufMarbleMatEmission);
	glMaterialf (GL_FRONT, GL_SHININESS, arrufMarbleMatShininess);

	// draw the basement
	int nBaseSide = 100;
	glPushMatrix();
	glTranslatef(0.0f, -18.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f( nBaseSide, 0, nBaseSide);
	glVertex3f( nBaseSide, 0, -nBaseSide);
	glVertex3f(-nBaseSide, 0, -nBaseSide);
	glVertex3f(-nBaseSide, 0, nBaseSide);
	glEnd();
	glPopMatrix();
}

void CShowModelPanel::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	
	if (nFlags & MK_CONTROL)
	{
	}
	else
	{
		CClientDC dc(this);
		dc.DPtoLP(&point);
		m_cvMousePos.x = point.x;
		m_cvMousePos.y = point.y;	
		m_bRButtonDown = true;
	}
	CDialog::OnRButtonDown(nFlags, point);
}


void CShowModelPanel::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_bRButtonDown = false;

	CDialog::OnRButtonUp(nFlags, point);
}

void CShowModelPanel::ShowProjection(void)
{	
	cv::destroyWindow("Contour");
	cv::destroyWindow("Silhouette");
	cv::destroyWindow("Depth");

	struct _timeb gsTimeStart, gsTimeEnd;
	int nTimeCost;
	_ftime(&gsTimeStart);	//start timing for search

	m_gProjector.CalcProjection(m_pModel);

	cv::Mat mtxPseudoDepth(m_gProjector.GetDepthImg().size(), CV_8UC3);
	cv::Mat mtxContour, mtxSilhouette;
	m_gProjector.GetContour().copyTo(mtxContour);
	m_gProjector.GetSilhouette().copyTo(mtxSilhouette);
	double fMinDepth, fMaxDepth;
	m_gProjector.GetMinMaxDepth(fMinDepth, fMaxDepth);
	MonoToColor(m_gProjector.GetDepthImg(), mtxPseudoDepth, fMinDepth - 0.02, fMaxDepth + 0.02, true);

	cv::flip(mtxContour, mtxContour, 0);
	cv::flip(mtxSilhouette, mtxSilhouette, 0);
	cv::flip(mtxPseudoDepth, mtxPseudoDepth, 0);

	cv::imshow("Contour", mtxContour);
	cv::imshow("Silhouette", mtxSilhouette);
	cv::imshow("Depth", mtxPseudoDepth);

	cv::waitKey(5);

	_ftime(&gsTimeEnd);	//end timing for search
	nTimeCost=(gsTimeEnd.time * 1000 + gsTimeEnd.millitm) - (gsTimeStart.time * 1000 + gsTimeStart.millitm);
	printf("Time Cost: %d ms\n", nTimeCost);
}