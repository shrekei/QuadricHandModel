#pragma once
#include "ConsolePrinter.h"
#include "ShowModelPanel.h"

// CHtouchDlg dialog
class CHtouchDlg : public CDialogEx
{
// Construction
public:
	CHtouchDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_HTOUCH_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CButton m_btnModelPanel;

private:
	CShowModelPanel	m_dlgShowModel;
	Hand*			m_pModel;
	int				m_nDofIndex;

private:	
	void ParseModelFile(std::string strFileName) { m_pModel->ParseFile(strFileName); }

public:
//	afx_msg void OnFileOpen();
//	afx_msg void OnFileClose();
//	afx_msg void OnFileExit();
	afx_msg void OnConsoleExit();
	afx_msg void OnConsoleInput();
	afx_msg void OnConsoleOutput();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnNormalizeModel();
//	afx_msg void OnDetectHand();
	afx_msg void OnShowProjection();
	// user message handler
	afx_msg LRESULT OnUserRepaint(WPARAM, LPARAM);
//	afx_msg void OnCaptureTemplate();
//	afx_msg void OnViewVideo();
//	virtual void OnFinalRelease();
	virtual BOOL DestroyWindow();
	afx_msg void OnClickedButtonIncdof();
	afx_msg void OnClickedButtonDecdof();
};
