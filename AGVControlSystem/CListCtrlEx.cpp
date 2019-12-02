#include "stdafx.h"
#include "CListCtrlEx.h"


CListCtrlEx::CListCtrlEx()
{
	m_bEnableTips = false;
	m_nItem = -1;
	m_nSubItem = -1;
}


CListCtrlEx::~CListCtrlEx()
{
}
BEGIN_MESSAGE_MAP(CListCtrlEx, CListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CListCtrlEx::OnNMCustomdraw)
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


void CListCtrlEx::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = CDRF_DODEFAULT;
	NMLVCUSTOMDRAW *lplvdr = (NMLVCUSTOMDRAW*)pNMHDR;
	NMCUSTOMDRAW &nmcd = lplvdr->nmcd;
	switch (lplvdr->nmcd.dwDrawStage) { // �ж�״̬
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT: // ���δ��ITEM֮ǰ��Ҫ������ɫ�ĸı�
		TEXT_BK tb;
		DWORD dw = nmcd.dwItemSpec;
		if (MapItemColor.Lookup(dw, tb)) {
			// ������SetItemColor(DWORD item,COLORREF color)���õ�ITEM�ź�COLORREF��ģ���в��ң�Ȼ�������ɫ��ֵ��
			lplvdr->clrText = tb.colText;
			lplvdr->clrTextBk = tb.colTextBk;
			*pResult = CDRF_DODEFAULT;
		}
		break;
	}
}

// ����ĳһ�е�ǰ��ɫ�ͱ���ɫ
void CListCtrlEx::SetItemColor(DWORD item, COLORREF textColor, COLORREF textBkColor) 
{
	TEXT_BK tb;
	tb.colText = textColor;
	tb.colTextBk = textBkColor;

	MapItemColor.SetAt(item, tb); // ����ĳ�е���ɫ
	this->RedrawItems(item, item); // ����Ⱦɫ
	this->SetFocus(); // ���ý���
	UpdateWindow();
}

// ����ȫ���е�ǰ��ɫ�ͱ���ɫ
void CListCtrlEx::SetAllItemColor(DWORD item, COLORREF textColor, COLORREF textBkColor) 
{
	TEXT_BK tb;
	tb.colText = textColor;
	tb.colTextBk = textBkColor;

	if (item > 0) 
	{
		for (DWORD numItem = 0; numItem < item; numItem++) 
		{
			MapItemColor.SetAt(numItem, tb); // ����ĳ�е���ɫ
			this->RedrawItems(numItem, numItem); // ����Ⱦɫ
		}

		UpdateWindow();
	}
	return;
}

// �����ɫӳ���
void CListCtrlEx::ClearColor() 
{
	MapItemColor.RemoveAll();
}

void CListCtrlEx::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_bEnableTips)
	{
		CString str;
		LVHITTESTINFO lvhti;

		// �ж���굱ǰ���ڵ�λ��(��, ��)  
		lvhti.pt = point;
		SubItemHitTest(&lvhti);

		// �������ƶ�����һ����Ԫ����, ����д���; ����, ��������  
		if ((lvhti.iItem != m_nItem) || (lvhti.iSubItem != m_nSubItem))
		{
			// ���浱ǰ������ڵ�(��,��)  
			m_nItem = lvhti.iItem;
			m_nSubItem = lvhti.iSubItem;

			// �������ƶ���һ���Ϸ��ĵ�Ԫ����,����ʾ�µ���ʾ��Ϣ  
			// ����, ����ʾ��ʾ  

			if ((m_nItem != -1) && (m_nSubItem != -1))
			{
				// @@@@@@@@ �������޸�Ҫ��ʾ����ʾ��Ϣ  
				// ���������һ������---��õ�ǰ��Ԫ���������Ϣ, ������Ϊ�µ���ʾ��Ϣ  
				str = GetItemText(m_nItem, m_nSubItem);
				m_toolTip.AddTool(this, str);
				// ��ʾ��ʾ��  
				m_toolTip.Pop();
			}
			else
			{
				m_toolTip.AddTool(this, _T(""));
				// ��ʾ��ʾ��  
				m_toolTip.Pop();
			}

		}
	}

	CListCtrl::OnMouseMove(nFlags, point);
}


BOOL CListCtrlEx::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (::IsWindow(m_toolTip.GetSafeHwnd()))
	{
		m_toolTip.RelayEvent(pMsg);
	}

	return CListCtrl::PreTranslateMessage(pMsg);
}


void CListCtrlEx::OnDestroy()
{
	CListCtrl::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	// listctrl����ʱ, ͬʱ���� tooltipctrl  
	m_toolTip.DestroyWindow();
	m_toolTip.Detach();
}

BOOL CListCtrlEx::EnableTips()
{
	EnableToolTips(TRUE);
	// ����tooltip�ؼ�  
	m_bEnableTips = m_toolTip.Create(this, TTS_ALWAYSTIP);

	if (m_bEnableTips)
	{
		m_toolTip.Activate(TRUE);
		m_toolTip.SetDelayTime(TTDT_INITIAL, 0);
	}

	return m_bEnableTips;
}