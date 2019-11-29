﻿
// TestClientDlg.h: 헤더 파일
//

#pragma once


// CTestClientDlg 대화 상자
class CTestClientDlg : public CDialogEx
{
// 생성입니다.

private:
	SOCKET mh_socket;

public:
	CTestClientDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	void AddEventString(CString parm_string);
	void ConnectProcess(LPARAM lParam);
	void DestroySocket();
	void ReadFrameData();
	void ReceiveData(char* parm_p_buffer, int parm_size);
	void SendFrameData(SOCKET param_h_socket, unsigned char param_id, const void* parm_p_data, int param_size);


// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TESTCLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CListBox m_event_list;
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedSendBtn();
	afx_msg void OnBnClickedOk();
};
