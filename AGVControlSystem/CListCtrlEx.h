#pragma once
#include "afxcmn.h"

typedef struct 
{
	COLORREF colText;
	COLORREF colTextBk;
}TEXT_BK;

class CListCtrlEx :
	public CListCtrl
{
public:
	CListCtrlEx();
	~CListCtrlEx();
protected:
	DECLARE_MESSAGE_MAP()
public:
	CMap<DWORD, DWORD&, TEXT_BK, TEXT_BK&> MapItemColor;
	bool m_bEnableTips;
	int m_nItem;
	int m_nSubItem;
	CToolTipCtrl m_toolTip;
public:
	// ����ĳһ�е�ǰ��ɫ�ͱ���ɫ
	void SetItemColor(DWORD item, COLORREF textColor, COLORREF textBkColor);
	// ����ȫ���е�ǰ��ɫ�ͱ���ɫ
	void SetAllItemColor(DWORD item, COLORREF textColor, COLORREF textBkColor);
	// �����ɫӳ���
	void ClearColor();
public:
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	BOOL EnableTips();
};

