#ifndef _SKLINE_PIC
#define _SKLINE_PIC

#pragma once
#include <core/swnd.h>

#define MAX_KLINE_COUNT		2
#define MAX_FLINE_COUNT		5
#define MAX_DATA_COUNT		5000

#define MOVE_ONESTEP		10		//ÿ��ƽ�Ƶ�������
#define K_WIDTH_TOTAL		16		//k��ռ���ܿ��(��x����)
#define UM_QUERY	(WM_USER + 321)
//һЩ�����Ľ�������Ϣ
#define RC_LEFT		40
#define RC_RIGHT	40
#define RC_TOP		25
#define RC_BOTTOM	25

#define RC_MAX		5
#define RC_MIN		5

typedef struct {
	DWORD	date;
	DWORD	time;
	float	open;
	float	high;
	float	low;
	float	close;
}DATA_FOR_SHOW;

typedef struct _KLINE_INFO {
	DATA_FOR_SHOW	pd[MAX_DATA_COUNT];	//data��Ϣ
	bool			bShow;				//�Ƿ���ʾ

	double			fMax;
	double			fMin;
	int				nDivY;				//y����ʾϵ��1
	int				nMulY;				//y����ʾϵ��2
	int				nDecimal;			//С��λ��
	TCHAR			sDecimal[10];		//������ʽ��
	int				nDecimalXi;			//10��m_nDecimal�η�
}KLINE_INFO;

typedef struct _FUTU_INFO {
	float			ftl[MAX_FLINE_COUNT][MAX_DATA_COUNT];	//��ͼ��
	BYTE			bft[MAX_FLINE_COUNT];					//��ʾ��ʽ 0Ϊ����ʾ,1Ϊ����ͼ,2Ϊ��״ͼ,3����ͼ
	COLORREF		clft[MAX_FLINE_COUNT];					//

	double			fMax;
	double			fMin;
	int				nDivY;				//y����ʾϵ��1
	int				nMulY;				//y����ʾϵ��2
	int				nDecimal;			//С��λ��
	TCHAR			sDecimal[10];		//������ʽ��
	int				nDecimalXi;			//10��m_nDecimal�η�
}FUTU_INFO;

namespace SOUI
{

	class SKlinePic : public SWindow
	{
		SOUI_CLASS_NAME(SKlinePic, L"klinePic")	//����xml��ǩ

	public:
		SKlinePic();
		~SKlinePic();

		CRect       m_rcAll;		//���¿����
		CRect		m_rcUpper;		//�Ͽ�����,K��
		CRect		m_rcLower;		//�¿�����,ָ��

		KLINE_INFO  m_Klines[MAX_KLINE_COUNT];	//����k�ߵ����ݺͲ��� �ڶ���ȫ��Ϊ��ɫ,���һ���γɶԱ�
		FUTU_INFO	m_Futu;			//��ͼ�������

		int		m_nTotal;		//�ܳ���
		int		m_nCyc;			//��������
		int		m_nKWidth;		//����֮��Ŀ��
		int		m_nMouseX;
		int		m_nMouseY;
		int		m_nMaxKNum;		//����ʾ��k���������
		int		m_nMove;		//����Ϊ����ƽ��,0Ϊ����
		bool	m_bHover;
		int		m_nTickPre;		//�����һ��ˢ��ʱ��
		int		m_nFirst;		//��ʼk��
		int		m_nEnd;			//����k��

		//��ͼָ��
		float	m_ztLine[MAX_FLINE_COUNT][MAX_DATA_COUNT];	//��ͼ��
		bool	m_bShowZtLine[MAX_FLINE_COUNT];

	protected:
		void		OnPaint(IRenderTarget *pRT);
		void		DrawArrow(IRenderTarget * pRT);
		void		GetMaxDiff(DWORD dk);		//�ж����������Сֵ��k������
		BOOL		IsInRect(int x, int y, int nMode = 0);	//�Ƿ���������,0Ϊȫ��,1Ϊ�Ϸ�,2Ϊ�·�
		SStringW	GetYPrice(int nY, DWORD nk);
		void		DrawData(IRenderTarget * pRT, DWORD dk);
		int			GetXData(int nx); 	//��ȡ����µ�����id
		int			GetYPos(double fDiff, DWORD dk);
		void		OnLButtonDown(UINT nFlags, CPoint point);
		int			OnCreate(LPCREATESTRUCT lpCreateStruct);

		void		GetFuTuMaxDiff();		//�жϸ�ͼ���������Сֵ��k������
		int			GetFuTuYPos(double fDiff);	//��ø�ͼyλ��
		SStringW	GetFuTuYPrice(int nY);		//��ø�ͼyλ�ü۸�

		SOUI_MSG_MAP_BEGIN()
			MSG_WM_PAINT_EX(OnPaint)
			MSG_WM_LBUTTONDOWN(OnLButtonDown)
			MSG_WM_MOUSEMOVE(OnMouseMove)
			MSG_WM_MOUSELEAVE(OnMouseLeave)
			MSG_WM_CREATE(OnCreate)
		SOUI_MSG_MAP_END()
	};
}
#endif // !_SKLINE_PIC