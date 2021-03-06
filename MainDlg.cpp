// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainDlg.h"	
#include "SKlinePic.h"
	
CMainDlg::CMainDlg() : SHostWnd(_T("LAYOUT:XML_MAINWND"))
{
	m_bLayoutInited = FALSE;
}

CMainDlg::~CMainDlg()
{
}

int CMainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	SetMsgHandled(FALSE);
	return 0;
}

BOOL CMainDlg::OnInitDialog(HWND hWnd, LPARAM lParam)
{
	m_bLayoutInited = TRUE;

	SKlinePic *pPic = FindChildByID2<SKlinePic>(R.id.kline_demo);

	time_t timeNow = time(0);
	tm tmNow;
	localtime_s(&tmNow, &timeNow);
	srand((int)timeNow);		//随机数种子
	for (int nk = 0; nk < 1; nk++)
	{
		pPic->m_Klines[nk].bShow = true;
		DATA_FOR_SHOW *p = pPic->m_Klines[nk].pd;
		for (int i = 0; i < 1000; i++, p++)
		{
			localtime_s(&tmNow, &timeNow);
			timeNow += 1;
			p->date = (tmNow.tm_year+1900) * 10000 + (tmNow.tm_mon + 1) * 100 + tmNow.tm_mday;
			p->time = (tmNow.tm_hour * 10000 + tmNow.tm_min * 100 + tmNow.tm_sec) * 1000;
			if (i == 0)
			{
				p->open = (float)(rand() % 10 + 500);
				p->close = (float)(rand() % 10 + 500);
				p->high = (float)((p->open > p->close ? p->open + rand() % 10 : p->close + rand() % 10));
				p->low = (float)((p->open < p->close ? p->open - rand() % 10 : p->close - rand() % 10));
			}
			else
			{
				DATA_FOR_SHOW *pm = p - 1;
				p->open = (float)(rand() % 11 + pm->close - 5);
				p->close = (float)(rand() % 21 + pm->close - 10);
				p->high = (float)((p->open > p->close ? p->open + rand() % 8 : p->close + rand() % 8));
				p->low = (float)((p->open < p->close ? p->open - rand() % 8 : p->close - rand() % 8));
			}
		}
		pPic->m_nTotal = 1000;
		wcscpy_s(pPic->m_Klines[nk].sDecimal, 9, L"%.0f");
		pPic->m_Klines[nk].nDecimalXi = 1;
	}

	//生成线条
	pPic->m_bShowZtLine[0] = true;
	pPic->m_bShowZtLine[1] = true;
	for (int i = 20; i < 1000; i++)
	{
		double f1 = 0;
		for (int j = 0; j < 20; j++)
		{
			f1 += pPic->m_Klines[0].pd[i - j].close;
			if (j == 9)
				pPic->m_ztLine[1][i] = (float)(f1 / 10);
		}
		pPic->m_ztLine[0][i] = (float)(f1 / 20);
	}

	//生成附图
	ZeroMemory(&pPic->m_Futu, sizeof(FUTU_INFO));
	//生成线条
	pPic->m_Futu.bft[0] = 2;
	wcscpy_s(pPic->m_Futu.sDecimal, 9, L"%.0f");
	pPic->m_Futu.nDecimalXi = 1;
	for (int i = 0; i < 1000; i++)
	{
		pPic->m_Futu.ftl[0][i] = (float)(rand() % 50 * 2 + 100);
	}

	SetTimer(1, 1000);

	return 0;
}

void CMainDlg::OnTimer(UINT_PTR nIDEvent)
{
	SetMsgHandled(FALSE);
	if (nIDEvent == 1)
	{
		FlashPic();
	}
}

int g_flash = 0;
void CMainDlg::FlashPic()
{
	SKlinePic *pPic = FindChildByID2<SKlinePic>(R.id.kline_demo);

	if (!pPic || pPic->m_nTotal >= 5000)
		return;
	tm tmNow;
	int nPos = pPic->m_nTotal;
	time_t timeNow = time(0);
	srand((int)timeNow);		//随机数种子

	if (g_flash < 10)
	{
		g_flash++;
		for (int nk = 0; nk < 1; nk++)
		{
			DATA_FOR_SHOW *p = pPic->m_Klines[nk].pd + nPos -1;
			p->close = (float)(rand() % 21 + p->close - 10);
			if (p->close > p->high)
				p->high = p->close;
			if (p->close < p->low)
				p->low = p->close;
		}

		//生成附图
		pPic->m_Futu.ftl[0][nPos -1] += 5;
		pPic->Invalidate();
	}
	else
	{
		g_flash = 0;
		for (int nk = 0; nk < 1; nk++)
		{
			DATA_FOR_SHOW *p = pPic->m_Klines[nk].pd + nPos;
			localtime_s(&tmNow, &timeNow);
			timeNow += 1;
			p->date = (tmNow.tm_year + 1900) * 10000 + (tmNow.tm_mon + 1) * 100 + tmNow.tm_mday;
			p->time = (tmNow.tm_hour * 10000 + tmNow.tm_min * 100 + tmNow.tm_sec) * 1000;
			DATA_FOR_SHOW *pm = p - 1;
			p->open = (float)(rand() % 11 + pm->close - 5);
			p->close = (float)(rand() % 21 + pm->close - 10);
			p->high = (float)((p->open > p->close ? p->open + rand() % 8 : p->close + rand() % 8));
			p->low = (float)((p->open < p->close ? p->open - rand() % 8 : p->close - rand() % 8));
			pPic->m_nTotal++;
		}

		//生成线条
		for (int i = 20; i < pPic->m_nTotal; i++)
		{
			double f1 = 0;
			for (int j = 0; j < 20; j++)
			{
				f1 += pPic->m_Klines[0].pd[i - j].close;
				if (j == 9)
					pPic->m_ztLine[1][i] = (float)(f1 / 10);
			}
			pPic->m_ztLine[0][i] = (float)(f1 / 20);
		}

		//生成附图
		pPic->m_Futu.ftl[0][nPos] = (float)(rand() % 50 * 2 + 100);
	}
	pPic->Invalidate();
}
//TODO:消息映射
void CMainDlg::OnClose()
{
	CSimpleWnd::DestroyWindow();
}

void CMainDlg::OnMaximize()
{
	SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
}
void CMainDlg::OnRestore()
{
	SendMessage(WM_SYSCOMMAND, SC_RESTORE);
}
void CMainDlg::OnMinimize()
{
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
}

void CMainDlg::OnSize(UINT nType, CSize size)
{
	SetMsgHandled(FALSE);
	if (!m_bLayoutInited) return;
	
	SWindow *pBtnMax = FindChildByName(L"btn_max");
	SWindow *pBtnRestore = FindChildByName(L"btn_restore");
	if(!pBtnMax || !pBtnRestore) return;
	
	if (nType == SIZE_MAXIMIZED)
	{
		pBtnRestore->SetVisible(TRUE);
		pBtnMax->SetVisible(FALSE);
	}
	else if (nType == SIZE_RESTORED)
	{
		pBtnRestore->SetVisible(FALSE);
		pBtnMax->SetVisible(TRUE);
	}
}

