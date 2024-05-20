// N3UIImage.cpp: implementation of the CN3UIImage class.
//
//////////////////////////////////////////////////////////////////////

#include "N3UIImage.h"
#include "N3Texture.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3UIImage::CN3UIImage()
{
	m_eType = UI_TYPE_IMAGE;

	m_pVB = NULL;
	m_pTexRef = NULL;
	m_szTexFN = "";
	m_pAnimImagesRef = NULL;

	ZeroMemory(&m_frcUVRect, sizeof(m_frcUVRect));
	m_Color = D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff);
	m_fAnimFrame = 30.0f;
	m_iAnimCount = 0;
	m_fCurAnimFrame = 0.0f;
}

CN3UIImage::~CN3UIImage()
{
	if (m_pVB) { m_pVB->Release();	m_pVB = NULL; }
	s_MngTex.Delete(&m_pTexRef);
	if (m_pAnimImagesRef) { delete[] m_pAnimImagesRef; m_pAnimImagesRef = NULL; }
}

void CN3UIImage::Release()
{
	CN3UIBase::Release();
	if (m_pVB) { m_pVB->Release();	m_pVB = NULL; }
	s_MngTex.Delete(&m_pTexRef);
	m_szTexFN = "";

	ZeroMemory(&m_frcUVRect, sizeof(m_frcUVRect));
	m_Color = D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff);
	m_fAnimFrame = 30.0f;
	m_iAnimCount = 0;
	m_fCurAnimFrame = 0.0f;
	if (m_pAnimImagesRef) { delete[] m_pAnimImagesRef; m_pAnimImagesRef = NULL; }
}

void CN3UIImage::Init(CN3UIBase* pParent)
{
	CN3UIBase::Init(pParent);
	CreateVB();
}

bool CN3UIImage::CreateVB()
{
	HRESULT hr;
	if (m_pVB) { m_pVB->Release(); m_pVB = NULL; }
	hr = s_lpD3DDev->CreateVertexBuffer(4 * sizeof(__VertexTransformed), 0, FVF_TRANSFORMED, D3DPOOL_MANAGED, &m_pVB, NULL);
	return SUCCEEDED(hr);
}


void CN3UIImage::SetVB()
{
	if (UISTYLE_IMAGE_ANIMATE & m_dwStyle)	// animate imageÀÌ¸é vertex buffer releaseÇÏ±â
	{
		if (m_pVB) { m_pVB->Release(); m_pVB = NULL; }
	}
	else
	{
		if (m_pVB)
		{
			__VertexTransformed* pVertices;
			m_pVB->Lock(0, 0, (void**)&pVertices, 0);

			uint32_t dwColor = 0xffffffff;
			float fRHW = 1.0f;
			// -0.5f¸¦ ÇØÁÖÁö ¾ÊÀ¸¸é °¡²û ÀÌ¹ÌÁö°¡ ÇÑ µ¾Æ®¾¿ ¹Ð¸®´Â °æ¿ì°¡ ÀÖ´Ù.(¿Ö ±×·±Áö´Â È®½ÇÇÏ°Ô ¸ð¸£°ÚÀ½)
			pVertices[0].Set((float)m_rcRegion.left - 0.5f, (float)m_rcRegion.top - 0.5f, UI_DEFAULT_Z, fRHW, m_Color, m_frcUVRect.left, m_frcUVRect.top);
			pVertices[1].Set((float)m_rcRegion.right - 0.5f, (float)m_rcRegion.top - 0.5f, UI_DEFAULT_Z, fRHW, m_Color, m_frcUVRect.right, m_frcUVRect.top);
			pVertices[2].Set((float)m_rcRegion.right - 0.5f, (float)m_rcRegion.bottom - 0.5f, UI_DEFAULT_Z, fRHW, m_Color, m_frcUVRect.right, m_frcUVRect.bottom);
			pVertices[3].Set((float)m_rcRegion.left - 0.5f, (float)m_rcRegion.bottom - 0.5f, UI_DEFAULT_Z, fRHW, m_Color, m_frcUVRect.left, m_frcUVRect.bottom);

			m_pVB->Unlock();
		}
	}
}


void CN3UIImage::SetTex(const std::string& szFN)
{
	m_szTexFN = szFN;
	s_MngTex.Delete(&m_pTexRef);
	// animate imageÀÏ¶§¸¸ texture ÁöÁ¤ÇÏ±â
	if (!(UISTYLE_IMAGE_ANIMATE & m_dwStyle)) m_pTexRef = s_MngTex.Get(szFN);
}

void CN3UIImage::SetRegion(const RECT& Rect)
{
	CN3UIBase::SetRegion(Rect);
	for (UIListItor itor = m_Children.begin(); m_Children.end() != itor; ++itor)
	{
		(*itor)->SetRegion(Rect);
	}
	SetVB();
}

void CN3UIImage::SetUVRect(float left, float top, float right, float bottom)
{
	m_frcUVRect.left = left;	m_frcUVRect.top = top;
	m_frcUVRect.right = right;	m_frcUVRect.bottom = bottom;
	SetVB();
}

void CN3UIImage::Tick()
{
	CN3UIBase::Tick();
	if (m_iAnimCount > 0)		// Animate ImageÀÏ¶§ ÇöÀç frame °è»ê
	{
		m_fCurAnimFrame += (s_fSecPerFrm * m_fAnimFrame);
		while (m_fCurAnimFrame >= (float)m_iAnimCount)
		{
			m_fCurAnimFrame -= ((float)m_iAnimCount);
		}
	}
}

void CN3UIImage::Render()
{
	if (!m_bVisible) return;

	if (UISTYLE_IMAGE_ANIMATE & m_dwStyle) // AnimateµÇ´Â ÀÌ¹ÌÁöÀÌ¸é
	{
		__ASSERT(m_fCurAnimFrame >= 0.0f && m_fCurAnimFrame < (float)m_iAnimCount, "animate image °¡ ÀÌ»óÀÛµ¿");
		__ASSERT(m_pAnimImagesRef, "ÃÊ±âÈ­ ÀÌ»ó");
		m_pAnimImagesRef[(int)m_fCurAnimFrame]->Render();
	}
	else
	{
		if (m_pVB && m_pTexRef)
		{
			s_lpD3DDev->SetStreamSource(0, m_pVB, 0, sizeof(__VertexTransformed));
			s_lpD3DDev->SetFVF(FVF_TRANSFORMED);

			s_lpD3DDev->SetTexture(0, m_pTexRef->Get());
			s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

			s_lpD3DDev->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
		}

		CN3UIBase::Render();
	}
}

void CN3UIImage::RenderIconWrapper()
{
	if (!m_bVisible) return;

	if (m_pVB)
	{
		s_lpD3DDev->SetStreamSource(0, m_pVB, 0, sizeof(__VertexTransformed));
		s_lpD3DDev->SetFVF(FVF_TRANSFORMED);
		s_lpD3DDev->SetTexture(0, NULL);

		s_lpD3DDev->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
	}

	CN3UIBase::Render();
}

POINT CalculateCoordinate(float percent, float right, float top, float left, float bottom)
{
	float radius = right - left;
	POINT p;
	if (percent <= 87.5 && percent >= 62.5)
	{
		p.x = right;
		p.y = bottom - (percent - 62.5) / 25 * radius;
	}
	else if (percent <= 62.5 && percent >= 37.5)
	{
		p.x = left + (percent - 37.5) / 25 * radius;
		p.y = bottom;
	}
	else if (percent <= 37.5 && percent >= 12.5)
	{
		p.x = left;
		p.y = top + (percent - 12.5) / 25 * radius;
	}
	else
	{
		if (percent >= 87.5)
			percent -= 87.5;
		else
			percent += 12.5;

		p.x = right - percent / 25 * radius;
		p.y = top;
	}

	return p;
}


void CN3UIImage::RenderIconCooldown(float percent)
{
	if (!m_bVisible) return;

	// Ýkonun merkezini ve köþelerini hesaplýyoruz
	float left = m_rcRegion.left;
	float bottom = m_rcRegion.bottom;
	float right = m_rcRegion.right;
	float top = m_rcRegion.top;
	float centerX = (left + right) / 2.0f;
	float centerY = (top + bottom) / 2.0f;
	float halfWidth = (right - left) / 2.0f;
	float halfHeight = (bottom - top) / 2.0f;
	POINT p = CalculateCoordinate(percent, m_rcRegion.right, m_rcRegion.top, m_rcRegion.left, m_rcRegion.bottom);

	const D3DVERTEX2 fixedVertices[COOLDOWN_FIXED_VERTICE_COUNT] = {
		{ right, top, 0.0f, 1.0f, GetColor()},
		{ right, top + halfHeight, 0.0f, 1.0f, GetColor() },
		{ right, bottom, 0.0f, 1.0f, GetColor() },
		{ left + halfWidth, bottom, 0.0f, 1.0f, GetColor() },
		{ left, bottom, 0.0f, 1.0f, GetColor() },
		{ left, top + halfHeight, 0.0f, 1.0f, GetColor() },
		{ left, top, 0.0f, 1.0f, GetColor() }
	};

	// Vertex verilerini tutmak için dizi oluþturuyoruz
	const int numVertices = percent / 12.5 + 1; 
	D3DVERTEX2* vertices = new D3DVERTEX2[numVertices + 2];
	memset(vertices, 0, sizeof(vertices));
	// Ýlk vertex merkez noktasý olacak
	vertices[0] = { centerX, centerY, 0.0f, 1.0f, GetColor() };
	// Son vertex tepe noktasý olacak
	vertices[numVertices + 1] = { centerX, (float)m_rcRegion.top, 0.0f, 1.0f, GetColor() };
	// percent'e göre hesaplanmýþ 1 adet vertex olacak
	auto index = 1;
	vertices[index++] = {(float)p.x, (float)p.y, 0.0f, 1.0f, GetColor()};

	// kalan vertexler, daha önceden belirli olan vertexlerden kopyalanacak
	int startIndex = COOLDOWN_FIXED_VERTICE_COUNT - (int)(percent / 12.5);
	for (size_t i = startIndex; i < COOLDOWN_FIXED_VERTICE_COUNT; i++)
	{
		vertices[index++] = { fixedVertices[i].x, fixedVertices[i].y, fixedVertices[i].z, fixedVertices[i].rhw, fixedVertices[i].color };
	}

	s_lpD3DDev->SetTexture(0, NULL);
	s_lpD3DDev->SetFVF(FVF_TRANSFORMEDCOLOR);
	s_lpD3DDev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, numVertices + 1, vertices, sizeof(D3DVERTEX2));
	delete[] vertices;

	D3DVERTEX2 sparkVertices[4];
	sparkVertices[0] = { centerX, centerY, 0.0f, 1.0f, COOLDOWN_SPARK_COLOR };
	sparkVertices[1] = { (float)p.x, (float)p.y, 0.0f, 1.0f, COOLDOWN_SPARK_COLOR };
	sparkVertices[2] = sparkVertices[0];
	sparkVertices[3] = { centerX, (float)m_rcRegion.top, 0.0f, 1.0f, COOLDOWN_SPARK_COLOR };
	s_lpD3DDev->SetTexture(0, NULL);
	s_lpD3DDev->SetFVF(FVF_TRANSFORMEDCOLOR);
	s_lpD3DDev->DrawPrimitiveUP(D3DPT_LINELIST, 2, sparkVertices, sizeof(D3DVERTEX2));


	CN3UIBase::Render();
}

BOOL CN3UIImage::MoveOffset(int iOffsetX, int iOffsetY)
{
	if (FALSE == CN3UIBase::MoveOffset(iOffsetX, iOffsetY)) return FALSE;
	SetVB();
	return TRUE;
}

void CN3UIImage::SetColor(D3DCOLOR color)
{
	if (m_Color == color) return;
	m_Color = color;
	if ((UISTYLE_IMAGE_ANIMATE & m_dwStyle) && m_pAnimImagesRef)
	{
		int i;
		for (i = 0; i < m_iAnimCount; ++i) m_pAnimImagesRef[i]->SetColor(color);
	}
	SetVB();
}

bool CN3UIImage::Load(HANDLE hFile)
{
	if (false == CN3UIBase::Load(hFile)) return false;
	DWORD dwNum;
	// texture Á¤º¸
	__ASSERT(NULL == m_pTexRef, "load ÇÏ±â Àü¿¡ ÃÊ±âÈ­°¡ µÇÁö ¾Ê¾Ò½À´Ï´Ù.");
	int	iStrLen = 0;
	ReadFile(hFile, &iStrLen, sizeof(iStrLen), &dwNum, NULL);			// ÆÄÀÏ ÀÌ¸§ ±æÀÌ
	char szFName[MAX_PATH] = "";
	if (iStrLen > 0)
	{
		ReadFile(hFile, szFName, iStrLen, &dwNum, NULL);		// ÆÄÀÏ ÀÌ¸§
		szFName[iStrLen] = '\0';
		this->SetTex(szFName);
	}

	ReadFile(hFile, &m_frcUVRect, sizeof(m_frcUVRect), &dwNum, NULL);	// uvÁÂÇ¥
	ReadFile(hFile, &m_fAnimFrame, sizeof(m_fAnimFrame), &dwNum, NULL);

	// Animate µÇ´Â imageÀÌ¸é °ü·ÃµÈ º¯¼ö ¼¼ÆÃ
	m_iAnimCount = 0; // animate image ¼ö Á¤ÇÏ±â
	for (UIListItor itor = m_Children.begin(); m_Children.end() != itor; ++itor)
	{
		if (UI_TYPE_IMAGE == (*itor)->UIType()) m_iAnimCount++;
	}

	if ((UISTYLE_IMAGE_ANIMATE & m_dwStyle) && m_iAnimCount > 0)
	{
		m_pAnimImagesRef = new CN3UIImage * [m_iAnimCount];
		ZeroMemory(m_pAnimImagesRef, sizeof(CN3UIImage*) * m_iAnimCount);
		int i = 0;
		for (UIListItor itor = m_Children.begin(); m_Children.end() != itor; ++itor)
		{
			if (UI_TYPE_IMAGE == (*itor)->UIType()) m_pAnimImagesRef[i] = (CN3UIImage*)(*itor);
			__ASSERT(m_pAnimImagesRef[i]->GetReserved() == (uint32_t)i, "animate Image load fail");	// Á¦´ë·Î Á¤·ÄÀÌ µÇÁö ¾Ê¾ÒÀ»°æ¿ì ½ÇÆÐÇÑ´Ù.
			++i;
		}
	}

	SetVB();	// vertex ¼¼ÆÃ
	return true;
}

void CN3UIImage::operator = (const CN3UIImage& other)
{
	CN3UIBase::operator = (other);

	m_Color = other.m_Color;
	m_fAnimFrame = other.m_fAnimFrame;
	m_fCurAnimFrame = other.m_fCurAnimFrame;
	m_frcUVRect = other.m_frcUVRect;
	m_iAnimCount = other.m_iAnimCount;

	if (other.m_pTexRef) m_pTexRef = s_MngTex.Get(other.m_pTexRef->FileName());
	m_szTexFN = other.m_szTexFN;

	// Animate µÇ´Â imageÀÌ¸é °ü·ÃµÈ º¯¼ö ¼¼ÆÃ
	m_iAnimCount = m_Children.size();	// animate image ¼ö Á¤ÇÏ±â
	if ((UISTYLE_IMAGE_ANIMATE & m_dwStyle) && m_iAnimCount > 0)
	{
		m_pAnimImagesRef = new CN3UIImage * [m_iAnimCount];
		ZeroMemory(m_pAnimImagesRef, sizeof(CN3UIImage*) * m_iAnimCount);
		int i = 0;
		for (UIListItor itor = m_Children.begin(); m_Children.end() != itor; ++itor)
		{
			__ASSERT(UI_TYPE_IMAGE == (*itor)->UIType(), "animate image childÀÇ UI typeÀÌ image°¡ ¾Æ´Ï´Ù.");
			m_pAnimImagesRef[i] = (CN3UIImage*)(*itor);
			__ASSERT(m_pAnimImagesRef[i]->GetReserved() == (uint32_t)i, "animate Image load fail");	// Á¦´ë·Î Á¤·ÄÀÌ µÇÁö ¾Ê¾ÒÀ»°æ¿ì ½ÇÆÐÇÑ´Ù.
			++i;
		}
	}

	SetVB();	// vertex ¼¼ÆÃ
}

#ifdef _N3TOOL
bool CN3UIImage::Save(HANDLE hFile)
{
	ReorderChildImage();	// child imageµé ¼ø¼­´ë·Î Á¤·Ä
	if (false == CN3UIBase::Save(hFile)) return false;
	DWORD dwNum;
	// texture Á¤º¸
	if (m_pTexRef) m_szTexFN = m_pTexRef->FileName();
	int iStrLen = m_szTexFN.size();
	WriteFile(hFile, &iStrLen, sizeof(iStrLen), &dwNum, NULL);			// ÆÄÀÏ ±æÀÌ
	if (iStrLen > 0)	WriteFile(hFile, m_szTexFN.c_str(), iStrLen, &dwNum, NULL);	// ÆÄÀÏ ÀÌ¸§

	WriteFile(hFile, &m_frcUVRect, sizeof(m_frcUVRect), &dwNum, NULL);		// uvÁÂÇ¥
	WriteFile(hFile, &m_fAnimFrame, sizeof(m_fAnimFrame), &dwNum, NULL);	// Animate frame

	return true;
}

void CN3UIImage::ChangeImagePath(const std::string& szPathOld, const std::string& szPathNew)
{
	CN3UIBase::ChangeImagePath(szPathOld, szPathNew);

	std::string szOld = szPathOld, szNew = szPathNew;

	if (!szOld.empty()) ::CharLower(&(szOld[0]));
	if (!szNew.empty()) ::CharLower(&(szNew[0]));
	if (!m_szTexFN.empty()) ::CharLower(&(m_szTexFN[0]));

	if (m_pTexRef) m_szTexFN = m_pTexRef->FileName();
	int i = m_szTexFN.find(szOld);
	if (i >= 0)
	{
		std::string szF = m_szTexFN.substr(0, i);
		std::string szL = m_szTexFN.substr(i + szOld.size());
		m_szTexFN = szF + szNew + szL;
		s_MngTex.Delete(&m_pTexRef);
		m_pTexRef = s_MngTex.Get(m_szTexFN);
	}
}

void CN3UIImage::GatherImageFileName(std::set<std::string>& setImgFile)
{
	CN3UIBase::GatherImageFileName(setImgFile); // child Á¤º¸

	std::string szImgFN = m_szTexFN;
	if (!szImgFN.empty())
	{
		::CharLower(&(szImgFN[0]));
		setImgFile.insert(szImgFN);
	}
}

// childÀÇ image°¡ m_dwReserved¿¡ µé¾î°¡ÀÖ´Â ¼ýÀÚ ¼ø¼­¿¡ ¸Â°Ô Àç¹èÄ¡
void CN3UIImage::ReorderChildImage()
{
	if (m_iAnimCount <= 0) return;
	CN3UIBase** pNewList = new CN3UIBase * [m_iAnimCount];
	ZeroMemory(pNewList, sizeof(CN3UIBase*) * m_iAnimCount);

	int i;
	for (i = 0; i < m_iAnimCount; ++i)
	{
		CN3UIBase* pSelChild = NULL;
		for (UIListItor itor = m_Children.begin(); m_Children.end() != itor; ++itor)
		{
			CN3UIBase* pChild = (*itor);
			__ASSERT(UI_TYPE_IMAGE == pChild->UIType(), "image°¡ ¾Æ´Ñ child°¡ ÀÖ½À´Ï´Ù.");
			if (NULL == pSelChild) pSelChild = pChild;
			else if (pSelChild->GetReserved() > pChild->GetReserved()) pSelChild = pChild;
		}
		__ASSERT(pSelChild, "Á¦ÀÏ ÀÛÀº m_dwReserved¸¦ °¡Áø child°¡ ¾ø´Ù.");
		pNewList[i] = pSelChild;
		RemoveChild(pSelChild);
	}

	for (i = 0; i < m_iAnimCount; ++i) m_Children.push_back(pNewList[i]);	// ÀÛÀº ¼ø¼­´ë·Î ³Ö±â

	delete[] pNewList;
}

CN3UIImage* CN3UIImage::GetChildImage(int iIndex)
{
	if (iIndex >= 0 && iIndex < m_iAnimCount)	return m_pAnimImagesRef[iIndex];
	return NULL;
}

void CN3UIImage::SetAnimImage(int iAnimCount)
{
	// ÀÌ¹Ì ¼³Á¤ µÇ¾î ÀÖ´Â°ÍÀÌ ÀÖÀ¸¸é Áö¿ì±â
	int i;
	if (m_pAnimImagesRef)
	{
		for (i = 0; i < m_iAnimCount; ++i)
		{	// ÀÚ½Ä Áö¿ì±â
			if (m_pAnimImagesRef[i]) { delete m_pAnimImagesRef[i]; m_pAnimImagesRef[i] = NULL; }
		}
		delete[] m_pAnimImagesRef; m_pAnimImagesRef = NULL;
	}
	m_iAnimCount = iAnimCount;

	// 0À¸·Î ¼³Á¤ÇÏ¸é º¸Åë image·Î ÀüÈ¯
	if (0 == m_iAnimCount)
	{
		SetStyle(m_dwStyle & (~UISTYLE_IMAGE_ANIMATE));
		CreateVB();
		SetVB();
	}
	else
	{
		SetStyle(m_dwStyle | UISTYLE_IMAGE_ANIMATE);
		s_MngTex.Delete(&m_pTexRef);
		SetVB();

		m_pAnimImagesRef = new CN3UIImage * [m_iAnimCount];
		ZeroMemory(m_pAnimImagesRef, sizeof(CN3UIImage*) * m_iAnimCount);
		for (i = 0; i < m_iAnimCount; ++i)
		{
			m_pAnimImagesRef[i] = new CN3UIImage();
			m_pAnimImagesRef[i]->Init(this);
			m_pAnimImagesRef[i]->SetReserved(i);
			m_pAnimImagesRef[i]->SetRegion(m_rcRegion);
		}
	}
}

bool CN3UIImage::ReplaceAllTextures(const std::string& strFind, const std::string& strReplace)
{
	if (strFind.size() <= 0 || strReplace.size() <= 0) return false;
	while (m_pTexRef)
	{
		char szFindDir[_MAX_DIR], szFindFName[_MAX_FNAME], szFindExt[_MAX_EXT];
		char szReplaceDir[_MAX_DIR], szReplaceFName[_MAX_FNAME], szReplaceExt[_MAX_EXT];
		char szTexDir[_MAX_DIR], szTexFName[_MAX_FNAME], szTexExt[_MAX_EXT];
		_splitpath(strFind.c_str(), NULL, szFindDir, szFindFName, szFindExt);
		_splitpath(strReplace.c_str(), NULL, szReplaceDir, szReplaceFName, szReplaceExt);
		_splitpath(m_pTexRef->FileName().c_str(), NULL, szTexDir, szTexFName, szTexExt);

		if (lstrlen(szFindFName) == 0 || lstrlen(szFindExt) == 0 ||
			lstrlen(szReplaceFName) == 0 || lstrlen(szReplaceExt) == 0) return false;

		std::string strNew(szTexDir);
		if (lstrcmpi(szFindFName, "*") == 0)
		{
			if (lstrcmpi(szFindExt, ".*") == 0)
			{	// *.* -> 
				if (lstrcmpi(szReplaceFName, "*") == 0)	strNew += szTexFName;
				else strNew += szReplaceFName;
				if (lstrcmpi(szReplaceExt, ".*") == 0) strNew += szTexExt;
				else strNew += szReplaceExt;
			}
			else
			{	// *.tga ->
				if (lstrcmpi(szFindExt, szTexExt) != 0) break;	// È®ÀåÀÚ°¡ °°Áö ¾ÊÀ¸¹Ç·Î ±×³É ¸®ÅÏ

				if (lstrcmpi(szReplaceFName, "*") == 0)	strNew += szTexFName;
				else strNew += szReplaceFName;
				if (lstrcmpi(szReplaceExt, ".*") == 0) strNew += szTexExt;
				else strNew += szReplaceExt;
			}
		}
		else
		{
			if (lstrcmpi(szFindFName, szTexFName) != 0) break;	// ÀÌ¸§ÀÌ °°Áö ¾ÊÀ¸¹Ç·Î ±×³É ¸®ÅÏ

			if (lstrcmpi(szFindExt, ".*") == 0)
			{	// abc.* ->
				if (lstrcmpi(szReplaceFName, "*") == 0)	strNew += szFindFName;
				else strNew += szReplaceFName;
				if (lstrcmpi(szReplaceExt, ".*") == 0) strNew += szTexExt;
				else strNew += szReplaceExt;
			}
			else
			{	// Ã£´Â ÆÄÀÏ¸í°ú È®ÀåÀÚ°¡ ÁöÁ¤µÇ¾î ÀÖÀ»°æ¿ì // abc.tga ->
				if (lstrcmpi(szFindExt, szTexExt) != 0) break;	// È®ÀåÀÚ°¡ °°Áö ¾ÊÀ¸¹Ç·Î ±×³É ¸®ÅÏ

				if (lstrcmpi(szReplaceFName, "*") == 0)	strNew += szFindFName;
				else strNew += szReplaceFName;
				if (lstrcmpi(szReplaceExt, ".*") == 0) strNew += szTexExt;
				else strNew += szReplaceExt;
			}
		}
		// ÅØ½ºÃÄ ´Ù½Ã ÁöÁ¤ÇÏ±â
		SetTex(strNew);
		break;
	}
	return CN3UIBase::ReplaceAllTextures(strFind, strReplace);
}
#endif
