#include "pch.h"
#include "CImage.h"

#include "CEngine.h"



CImage::CImage()
	: m_hBit(0)
	, m_hDC(0)
	, m_iWidth(0)
	, m_iHeight(0)
	, m_pBit(nullptr)
{
}

CImage::~CImage()
{
	DeleteObject(m_hBit);
	DeleteDC(m_hDC);
}

int CImage::Load(const wstring& _strFilePath)
{
	m_hBit = (HBITMAP)LoadImageW(nullptr, _strFilePath.c_str(), IMAGE_BITMAP, 0, 0
		, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	if (nullptr == m_hBit)
	{
		return E_FAIL;
	}
		
	// ��Ʈ�� ���� Ȯ��
	BITMAP tBitInfo = {};
	GetObject(m_hBit, sizeof(BITMAP), &tBitInfo);

	m_iWidth = tBitInfo.bmWidth;
	m_iHeight = tBitInfo.bmHeight;
	//��Ʈ ���� ������ �����´�.
	m_pBit = tBitInfo.bmBits;

	int iErr = GetLastError();

	HDC hMainDC = CEngine::GetInst()->GetMainDC();
	m_hDC = CreateCompatibleDC(hMainDC);

	HBITMAP hPrevBit = (HBITMAP)SelectObject(m_hDC, m_hBit);
	DeleteObject(hPrevBit);

	return S_OK;
}

int CImage::Create(UINT _iWidth, UINT _iHeight)
{
	// ���� ������ ��Ʈ���� ����Ű�� ���� DC ����
	// ���� ������ ��Ʈ�ʰ� DC �� ���� ����
	HDC hMainDC = CEngine::GetInst()->GetMainDC();

	m_hBit = CreateCompatibleBitmap(hMainDC, _iWidth, _iHeight);
	m_hDC = CreateCompatibleDC(hMainDC);

	if (0 == m_hBit || 0 == m_hDC)
		return E_FAIL;

	HBITMAP hDefaultBitmap = (HBITMAP)SelectObject(m_hDC, m_hBit);
	DeleteObject(hDefaultBitmap);

	// ��Ʈ�� ���� Ȯ��
	BITMAP tBitInfo = {};
	GetObject(m_hBit, sizeof(BITMAP), &tBitInfo);

	m_iWidth = tBitInfo.bmWidth;
	m_iHeight = tBitInfo.bmHeight;

	return S_OK;
}


//�ȼ� �� ���� �Լ�
void CImage::SetPixelColor(int _x, int _y, tPixel _pixel)
{
	//�޸��� Ư���� ������ �����ؾ� �Ѵ�. 
	_y = m_iHeight - (_y + 1);
	tPixel* pPixel = (tPixel*)m_pBit;
	pPixel += (m_iWidth * _y + _x);

	*pPixel = _pixel;
}


tPixel CImage::GetPixelColor(int _x, int _y)
{
	// ���� ��Ʈ�� �ȼ��� �Ʒ��� ���� �����ϱ� ������,
		// ���������ͷ� ����ϴ� ������ ��ǥ��� ���߾��ֱ� ���ؼ� ���� ����� �����Ѵ�.
	_y = m_iHeight - (_y + 1);

	tPixel* pPixel = (tPixel*)m_pBit;
	pPixel += (m_iWidth * _y + _x);
	return *pPixel;
}