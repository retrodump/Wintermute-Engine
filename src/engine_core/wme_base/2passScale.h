#ifndef _2_PASS_SCALE_H_
#define _2_PASS_SCALE_H_

#include <math.h> 

#include "2PassScaleFilters.h" 

typedef struct 
{ 
   double *Weights;  // Normalized weights of neighboring pixels
   int WeightsSize;
   int Left,Right;   // Bounds of source pixels window
} ContributionType;  // Contirbution information for a single pixel

typedef struct 
{ 
   ContributionType *ContribRow; // Row (or column) of contribution weights 
   UINT WindowSize,              // Filter window size (of affecting source pixels) 
        LineLength;              // Length of line (no. or rows / cols) 
} LineContribType;               // Contribution information for an entire line (row or column)

typedef BOOL (*ProgressAnbAbortCallBack)(BYTE bPercentComplete);

template <class FilterClass>
class C2PassScale 
{
public:

    C2PassScale (ProgressAnbAbortCallBack callback = NULL) : 
        m_Callback (callback) {}

    virtual ~C2PassScale() {}

    RGBPixel * AllocAndScale (  
		CBGame* Game,
        RGBPixel   *pOrigImage, 
        UINT        uOrigWidth, 
        UINT        uOrigHeight, 
        UINT        uNewWidth, 
        UINT        uNewHeight);

    RGBPixel * Scale (  
        RGBPixel   *pOrigImage, 
        UINT        uOrigWidth, 
        UINT        uOrigHeight, 
        RGBPixel   *pDstImage,
        UINT        uNewWidth, 
        UINT        uNewHeight);

private:

    ProgressAnbAbortCallBack    m_Callback;
    BOOL                        m_bCanceled;

    LineContribType *AllocContributions (   UINT uLineLength, 
                                            UINT uWindowSize);

    void FreeContributions (LineContribType * p);

    LineContribType *CalcContributions (    UINT    uLineSize, 
                                            UINT    uSrcSize, 
                                            double  dScale);

    void ScaleRow ( CBGame* Game,
					RGBPixel           *pSrc, 
                    UINT                uSrcWidth,
                    RGBPixel           *pRes, 
                    UINT                uResWidth,
                    UINT                uRow, 
                    LineContribType    *Contrib);

    void HorizScale (   CBGame* Game,
						RGBPixel           *pSrc, 
                        UINT                uSrcWidth,
                        UINT                uSrcHeight,
                        RGBPixel           *pDst,
                        UINT                uResWidth,
                        UINT                uResHeight);

    void ScaleCol ( RGBPixel           *pSrc, 
                    UINT                uSrcWidth,
                    RGBPixel           *pRes, 
                    UINT                uResWidth,
                    UINT                uResHeight,
                    UINT                uCol, 
                    LineContribType    *Contrib);

    void VertScale (    RGBPixel           *pSrc, 
                        UINT                uSrcWidth,
                        UINT                uSrcHeight,
                        RGBPixel           *pDst,
                        UINT                uResWidth,
                        UINT                uResHeight);
};

template <class FilterClass>
LineContribType *
C2PassScale<FilterClass>::
AllocContributions (UINT uLineLength, UINT uWindowSize)
{
    LineContribType *res = new LineContribType; 
	if(!res) return NULL;

        // Init structure header 
    res->WindowSize = uWindowSize; 
    res->LineLength = uLineLength; 
        // Allocate list of contributions 
    res->ContribRow = new ContributionType[uLineLength];
	if(!res->ContribRow)
	{
		delete res;
		return NULL;
	}

    for (UINT u = 0 ; u < uLineLength ; u++) 
    {
        // Allocate contributions for every pixel
        res->ContribRow[u].Weights = new double[uWindowSize];
		res->ContribRow[u].WeightsSize = uWindowSize;
		if(res->ContribRow[u].Weights==NULL)
		{
			// !!!
			return NULL;
		}
    }
    return res; 
} 
 
template <class FilterClass>
void
C2PassScale<FilterClass>::
FreeContributions (LineContribType * p)
{ 
    for (UINT u = 0; u < p->LineLength; u++) 
    {
        // Free contribs for every pixel
        delete [] p->ContribRow[u].Weights;
    }
    delete [] p->ContribRow;    // Free list of pixels contribs
    delete p;                   // Free contribs header
} 
 
template <class FilterClass>
LineContribType *
C2PassScale<FilterClass>::
CalcContributions (UINT uLineSize, UINT uSrcSize, double dScale)
{ 
    FilterClass CurFilter;

    double dWidth;
    double dFScale = 1.0;
    double dFilterWidth = CurFilter.GetWidth();

    if (dScale < 1.0) 
    {    // Minification
        dWidth = dFilterWidth / dScale; 
        dFScale = dScale; 
    } 
    else
    {    // Magnification
        dWidth= dFilterWidth; 
    }
 
    // Window size is the number of sampled pixels
    int iWindowSize = 2 * (int)ceil(dWidth) + 1; 
 
    // Allocate a new line contributions strucutre
    LineContribType *res = AllocContributions (uLineSize, iWindowSize); 
	if(!res) return NULL;
 
    for (UINT u = 0; u < uLineSize; u++) 
    {   // Scan through line of contributions
        double dCenter = (double)u / dScale;   // Reverse mapping
        // Find the significant edge points that affect the pixel
        int iLeft = max (0, (int)floor (dCenter - dWidth)); 
        res->ContribRow[u].Left = iLeft; 
        int iRight = min ((int)ceil (dCenter + dWidth), int(uSrcSize) - 1); 
        res->ContribRow[u].Right = iRight;
 
        // Cut edge points to fit in filter window in case of spill-off
        if (iRight - iLeft + 1 > iWindowSize) 
        {
            if (iLeft < (int(uSrcSize) - 1 / 2)) 
            {
                iLeft++; 
            }
            else 
            {
                iRight--; 
            }
        }
        double dTotalWeight = 0.0;  // Zero sum of weights

		int iSrc;
        for (iSrc = iLeft; iSrc <= iRight; iSrc++)
        {   // Calculate weights
            dTotalWeight += (res->ContribRow[u].Weights[iSrc-iLeft] =  
                                dFScale * CurFilter.Filter (dFScale * (dCenter - (double)iSrc))); 
        }
//        ASSERT (dTotalWeight >= 0.0);   // An error in the filter function can cause this 
        if (dTotalWeight > 0.0)
        {   // Normalize weight of neighbouring points
            for (iSrc = iLeft; iSrc <= iRight; iSrc++)
            {   // Normalize point
                res->ContribRow[u].Weights[iSrc-iLeft] /= dTotalWeight; 
            }
        }
   } 
   return res; 
} 
 
 
template <class FilterClass>
void 
C2PassScale<FilterClass>::
ScaleRow (  CBGame* Game,
			RGBPixel           *pSrc, 
            UINT                uSrcWidth,
            RGBPixel           *pRes, 
            UINT                uResWidth,
            UINT                uRow, 
            LineContribType    *Contrib)
{
    RGBPixel *pSrcRow = &(pSrc[uRow * uSrcWidth]);
    RGBPixel *pDstRow = &(pRes[uRow * uResWidth]);

    for (UINT x = 0; x < uResWidth; x++) 
    {   // Loop through row


        BYTE r = 0;
        BYTE g = 0;
        BYTE b = 0;
        int iLeft = Contrib->ContribRow[x].Left;    // Retrieve left boundries
        int iRight = Contrib->ContribRow[x].Right;  // Retrieve right boundries

        for (int i = iLeft; i <= iRight; i++)
		{
			//Game->LOG(0, "  in %d %d", x, i);
			// Scan between boundries
            // Accumulate weighted effect of each neighboring pixel
			if(i-iLeft < 0 || i-iLeft>=Contrib->ContribRow[x].WeightsSize)
			{
				continue;
			}
			r += (BYTE)(Contrib->ContribRow[x].Weights[i-iLeft] * (double)(pSrcRow[i].red)); 
			g += (BYTE)(Contrib->ContribRow[x].Weights[i-iLeft] * (double)(pSrcRow[i].green)); 
			b += (BYTE)(Contrib->ContribRow[x].Weights[i-iLeft] * (double)(pSrcRow[i].blue)); 

			
        } 
        //pDstRow[x] = RGB(r,g,b); // Place result in destination pixel
		pDstRow[x].red = r;
		pDstRow[x].green = g;
		pDstRow[x].blue = b;
    } 
} 

template <class FilterClass>
void
C2PassScale<FilterClass>::
HorizScale (	CBGame* Game,
				RGBPixel           *pSrc, 
                UINT                uSrcWidth,
                UINT                uSrcHeight,
                RGBPixel           *pDst, 
                UINT                uResWidth,
                UINT                uResHeight)
{ 
//    TRACE ("Performing horizontal scaling...\n"); 
    if (uResWidth == uSrcWidth)
    {   // No scaling required, just copy
        memcpy (pDst, pSrc, sizeof (RGBPixel) * uSrcHeight * uSrcWidth);
    }	
    // Allocate and calculate the contributions
    LineContribType * Contrib = CalcContributions (uResWidth, uSrcWidth, double(uResWidth) / double(uSrcWidth)); 
	if(!Contrib) return;

    for (UINT u = 0; u < uResHeight; u++)
    {   // Step through rows
		/*
        if (NULL != m_Callback)
        {
            //
            // Progress and report callback supplied
            //
            if (!m_Callback (BYTE(double(u) / double (uResHeight) * 50.0)))
            {
                //
                // User wished to abort now
                //
                m_bCanceled = TRUE;
                FreeContributions (Contrib);  // Free contributions structure
                return;
            }
        }
		*/
        ScaleRow (  Game,
					pSrc, 
                    uSrcWidth,
                    pDst,
                    uResWidth,
                    u,
                    Contrib);    // Scale each row 
    }
    FreeContributions (Contrib);  // Free contributions structure
} 
 
template <class FilterClass>
void
C2PassScale<FilterClass>::
ScaleCol (  RGBPixel           *pSrc, 
            UINT                uSrcWidth,
            RGBPixel           *pRes, 
            UINT                uResWidth,
            UINT                uResHeight,
            UINT                uCol, 
            LineContribType    *Contrib)
{ 
    for (UINT y = 0; y < uResHeight; y++) 
    {    // Loop through column
        BYTE r = 0;
        BYTE g = 0;
        BYTE b = 0;
        int iLeft = Contrib->ContribRow[y].Left;    // Retrieve left boundries
        int iRight = Contrib->ContribRow[y].Right;  // Retrieve right boundries
        for (int i = iLeft; i <= iRight; i++)
        {   
			if(i-iLeft < 0 || i-iLeft>=Contrib->ContribRow[y].WeightsSize)
			{
				continue;
			}
			
			// Scan between boundries
            // Accumulate weighted effect of each neighboring pixel
            RGBPixel pCurSrc = pSrc[i * uSrcWidth + uCol];
            r += BYTE(Contrib->ContribRow[y].Weights[i-iLeft] * (double)(pCurSrc.red));
            g += BYTE(Contrib->ContribRow[y].Weights[i-iLeft] * (double)(pCurSrc.green));
            b += BYTE(Contrib->ContribRow[y].Weights[i-iLeft] * (double)(pCurSrc.blue));
        }
        //pRes[y * uResWidth + uCol] = RGB (r,g,b);   // Place result in destination pixel
		pRes[y * uResWidth + uCol].red = r;
		pRes[y * uResWidth + uCol].green = g;
		pRes[y * uResWidth + uCol].blue = b;
    }
} 
 

template <class FilterClass>
void
C2PassScale<FilterClass>::
VertScale ( RGBPixel           *pSrc, 
            UINT                uSrcWidth,
            UINT                uSrcHeight,
            RGBPixel           *pDst, 
            UINT                uResWidth,
            UINT                uResHeight)
{ 
//    TRACE ("Performing vertical scaling..."); 

    if (uSrcHeight == uResHeight)
    {   // No scaling required, just copy
        memcpy (pDst, pSrc, sizeof (RGBPixel) * uSrcHeight * uSrcWidth);
    }
    // Allocate and calculate the contributions
    LineContribType * Contrib = CalcContributions (uResHeight, uSrcHeight, double(uResHeight) / double(uSrcHeight)); 
    for (UINT u = 0; u < uResWidth; u++)
    {   // Step through columns
		/*
        if (NULL != m_Callback)
        {
            //
            // Progress and report callback supplied
            //
            if (!m_Callback (BYTE(double(u) / double (uResWidth) * 50.0) + 50))
            {
                //
                // User wished to abort now
                //
                m_bCanceled = TRUE;
                FreeContributions (Contrib);  // Free contributions structure
                return;
            }
        }
		*/
        ScaleCol (  pSrc,
                    uSrcWidth,
                    pDst,
                    uResWidth,
                    uResHeight,
                    u,
                    Contrib);   // Scale each column
    }
    FreeContributions (Contrib);     // Free contributions structure
} 

template <class FilterClass>
RGBPixel *
C2PassScale<FilterClass>::
AllocAndScale ( 
   CBGame* Game,
    RGBPixel   *pOrigImage, 
    UINT        uOrigWidth, 
    UINT        uOrigHeight, 
    UINT        uNewWidth, 
    UINT        uNewHeight)
{ 
    // Scale source image horizontally into temporary image
    m_bCanceled = FALSE;
    RGBPixel *pTemp = new RGBPixel [uNewWidth * uOrigHeight];
    HorizScale (Game,
				pOrigImage, 
                uOrigWidth,
                uOrigHeight,
                pTemp,
                uNewWidth,
                uOrigHeight);

    if (m_bCanceled)
    {
        delete [] pTemp;
        return NULL;
    }

    // Scale temporary image vertically into result image    
    RGBPixel *pRes = new RGBPixel [uNewWidth * uNewHeight];
    VertScale ( pTemp,
                uNewWidth,
                uOrigHeight,
                pRes,
                uNewWidth,
                uNewHeight);

    if (m_bCanceled)
    {
        delete [] pTemp;
        delete [] pRes;
        return NULL;
    }

    delete [] pTemp;
    return pRes;
} 

template <class FilterClass>
RGBPixel *
C2PassScale<FilterClass>::
Scale ( 
    RGBPixel   *pOrigImage, 
    UINT        uOrigWidth, 
    UINT        uOrigHeight, 
    RGBPixel   *pDstImage, 
    UINT        uNewWidth, 
    UINT        uNewHeight)
{ 
    // Scale source image horizontally into temporary image
    m_bCanceled = FALSE;
    RGBPixel *pTemp = new RGBPixel [uNewWidth * uOrigHeight];
    HorizScale (pOrigImage, 
                uOrigWidth,
                uOrigHeight,
                pTemp,
                uNewWidth,
                uOrigHeight);
    if (m_bCanceled)
    {
        delete [] pTemp;
        return NULL;
    }

    // Scale temporary image vertically into result image    
    VertScale ( pTemp,
                uNewWidth,
                uOrigHeight,
                pDstImage,
                uNewWidth,
                uNewHeight);
    delete [] pTemp;
    if (m_bCanceled)
    {
        return NULL;
    }
    return pDstImage;
} 


#endif //   _2_PASS_SCALE_H_
