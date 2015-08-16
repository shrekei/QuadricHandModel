#include "stdafx.h"
#include "HandProjector.h"
#include <omp.h>

Pixel::Pixel(void) : Center(0, 0)
{	
	IsForegrd = false;
	IsContour = false;
	Depth = 0.0;
}

Pixel::Pixel(cv::Point2f  ptCenter) : Center(ptCenter)
{	
	IsForegrd = false;
	IsContour = false;
	Depth = 0.0;
}

//------------------------------HandProjector---------------------------------//
HandProjector::HandProjector(void)
{
	Init();
}

HandProjector::~HandProjector(void)
{
	Reset();
}

void HandProjector::Init(void)
{
	m_pHandProjector = NULL;
	m_fDx = m_fDy = 0;
}

void HandProjector::Reset(void)
{
	if (m_pHandProjector != NULL)
	{
		//!! notice there are one more row and column
		for (int i = 0; i < m_sPanel.height + 1; i++)
		{
			delete[] m_pHandProjector[i];
		}
		delete[] m_pHandProjector;
		m_pHandProjector = NULL;
	}
}

Pixel &HandProjector::operator() (int y, int x)
{
	return m_pHandProjector[y][x];
}

void HandProjector::SetTSize(cv::Size sPanel)
{
	if (sPanel == m_sPanel)
		return ;

	Reset();	

	// The standard x-y coordinate is adopted, with axis at w /2 and h / 2
	// we add one more row and column for padding and tangency test
	m_sPanel = sPanel;
	m_pHandProjector = new Pixel*[m_sPanel.height + 1];
	for (int i = 0; i < m_sPanel.height + 1; i++)
	{
		m_pHandProjector[i] = new Pixel[m_sPanel.width + 1];
	}
	m_mtxContour.create(m_sPanel, CV_8UC3);
	m_mtxSilhouette.create(m_sPanel, CV_8UC1);
	m_mtxDepthImg.create(m_sPanel, CV_32FC1);
}

void HandProjector::CalcProjection(Hand* pHand)
{
	// make sure the projection of the model just fits the image
	bmu::Vector3 vOBBSpan = pHand->GetOBBSpan();
	double fXSpan = vOBBSpan.x;
	double fYSpan = vOBBSpan.y;
	double z0 = vOBBSpan.z;
	double fProjWidth = fXSpan * 1.0 / z0;
	double fProjHeight = fYSpan * 1.0 / z0;
	m_fDx = m_fDy = max(fProjWidth / m_sPanel.width, fProjHeight / m_sPanel.height);
	double fHeight = m_fDy * m_sPanel.height;
	double fWidth = m_fDx * m_sPanel.width;

	// set the center of each pixel
	for (int i = 0; i < m_sPanel.height + 1; i++)
	{
		for (int j = 0; j < m_sPanel.width + 1; j++)
		{
			m_pHandProjector[i][j].Center = cv::Point2f(j * m_fDx + m_fDx / 2 - fWidth / 2, i * m_fDy + m_fDy / 2 - fHeight / 2);
		}
	}

	// calculate the intersections to obtain the projection
	CalcIntscts(pHand);

	// generate the results: contour, silhouette and depth images
	GenerateContour();	

	// the depth value is scaled by 1 / 100, as the hand model is built at a scale of 100 / 1
	m_fMinDepth = 1e10;
	m_fMaxDepth = 0.0;
	for (int i = 0; i < m_sPanel.height; i++)
	{
		for (int j = 0; j < m_sPanel.width; j++)
		{
			if (m_pHandProjector[i][j].IsForegrd)
			{
				m_mtxSilhouette.at<uchar>(i, j) = 255;
				double fDepth = 1.0 * m_pHandProjector[i][j].Depth / 100;
				m_mtxDepthImg.at<float>(i, j) = fDepth;

				if (fDepth != 0.0)
				{
					if (fDepth < m_fMinDepth)
						m_fMinDepth = fDepth;
					if (fDepth > m_fMaxDepth)
						m_fMaxDepth = fDepth;
				}
			}
			else
			{
				m_mtxSilhouette.at<uchar>(i, j) = 0;
				m_mtxDepthImg.at<float>(i, j) = 0.0;
			}
		}
	}
//	cvFlip(m_pSilhouetteImg, 0);
}

void HandProjector::CalcIntscts(Hand *pHand)
{
	// for each pixel, test whether the ray across its left-bottom corner intersects with any quadrics
#pragma omp parallel for ordered
	for (int i = 0; i < m_sPanel.height + 1; i++)
	{
		for (int j = 0; j < m_sPanel.width + 1; j++)
		{
			double x = m_pHandProjector[i][j].Center.x - m_fDx / 2;
			double y = m_pHandProjector[i][j].Center.y - m_fDy / 2;

			// assign an invalid label for initilization
			Intsct istNear(NULL, -1, false, 0);
			double fDepth = INF;

			// find the nearest intersection point
			for (vector<Quadric *>::const_iterator it = pHand->GetQuadrics().begin();
				it != pHand->GetQuadrics().end(); it++)
			{
				vector<Intsct> vecTmp = (*it)->CheckIntscts(bmu::Vector3(x, y, 1));
				for (vector<Intsct>::iterator itst = vecTmp.begin(); itst != vecTmp.end(); itst++)
				{
					if ((itst->GetDepth()) < fDepth)
					{
						istNear.Set(itst->GetQuadric(), itst->GetLabel(), itst->IsTangent(), itst->GetDepth());
						fDepth = itst->GetDepth();
					}
				}
			}
			m_pHandProjector[i][j].NearIntsct = istNear;
			if (istNear.GetLabel() != -1)
			{
				m_pHandProjector[i][j].Depth = istNear.GetDepth();
			}
			else	// assign zero depth to background areas
			{
				m_pHandProjector[i][j].Depth = 0.0;
			}
		}
	}

	// tangency test, here we only test the pixels within the range (0:m_nHeight-1, 0:m_nWidth-1)
	// this accords to the size of the images to display
	m_vecEdgePoints.clear();
	for (int i = 0; i < m_sPanel.height; i++)
	{
		for (int j = 0; j < m_sPanel.width; j++)
		{
			Intsct istNear = m_pHandProjector[i][j].NearIntsct;

			// compare the labels and depths around the four corners of each pixel, if all same, then 
			// it is not tangent to any quadrics. Otherwise, if the depths have significant difference, 
			// it is tangent to certain quadric. Otherwise it may lie at the boundary of differenc joints
			m_pHandProjector[i][j].IsContour = false;
			m_pHandProjector[i][j].IsForegrd = true;
			
			if (istNear.IsTangent() == true)
			{
				m_pHandProjector[i][j].IsContour = true;
				m_vecEdgePoints.push_back(cvPoint(j, i));
			}
			else
			{
				int nLabel0 = istNear.GetLabel();
				int nLabel1 = m_pHandProjector[i + 1][j].NearIntsct.GetLabel();
				int nLabel2 = m_pHandProjector[i][j + 1].NearIntsct.GetLabel();
				int nLabel3 = m_pHandProjector[i + 1][j + 1].NearIntsct.GetLabel();
				
				if ((nLabel0 == nLabel1) && (nLabel1 == nLabel2) &&
					(nLabel2 == nLabel3))
				{
					m_pHandProjector[i][j].IsContour = false;

					// if all labels are -1, the pixel lies in the background
					if (nLabel0 < 0)						
						m_pHandProjector[i][j].IsForegrd = false;
				}
				else
				{
					double fDepth0 = istNear.GetDepth();
					double fDepth1 = m_pHandProjector[i + 1][j].NearIntsct.GetDepth();
					double fDepth2 = m_pHandProjector[i][j + 1].NearIntsct.GetDepth();
					double fDepth3 = m_pHandProjector[i + 1][j + 1].NearIntsct.GetDepth();
					double fDiff1 = fabs(fDepth1 - fDepth0);
					double fDiff2 = fabs(fDepth2 - fDepth0);
					double fDiff3 = fabs(fDepth3 - fDepth0);
					double fMaxDiff = max(fDiff1, max(fDiff2, fDiff3));

					if (fMaxDiff > 0.2)
					{
						int nNewLabel = -1;
						Quadric* pNewQuadric = NULL;
						if (nLabel0 >= 0) { nNewLabel = nLabel0; pNewQuadric = istNear.GetQuadric();}
						if (nLabel1 >= 0) { nNewLabel = nLabel1; pNewQuadric = m_pHandProjector[i + 1][j].NearIntsct.GetQuadric();}
						if (nLabel2 >= 0) { nNewLabel = nLabel2; pNewQuadric = m_pHandProjector[i][j + 1].NearIntsct.GetQuadric();}
						if (nLabel3 >= 0) { nNewLabel = nLabel3; pNewQuadric = m_pHandProjector[i + 1][j + 1].NearIntsct.GetQuadric();}
						m_pHandProjector[i][j].IsContour = true;
						m_pHandProjector[i][j].NearIntsct = Intsct(pNewQuadric, nNewLabel, istNear.IsTangent(), istNear.GetDepth());
						m_pHandProjector[i][j].Depth = istNear.GetDepth();
						m_vecEdgePoints.push_back(cvPoint(j, i));
					}
					else
					{
						m_pHandProjector[i][j].IsContour = false;
					}
				}
			}
		}
	}
}

void HandProjector::GenerateContour(void)
{
	// generate 2-D grid to perform contour sub-sampling to get the 
	// control points on the contour
	int nStep = 20;
	int nGridWidth = (m_sPanel.width% nStep == 0) ? m_sPanel.width / nStep : ( m_sPanel.width / nStep + 1);
	int nGridHeight = (m_sPanel.height % nStep == 0) ? m_sPanel.height / nStep : (m_sPanel.height / nStep + 1);
	// perform binarization
	for (int i = 0; i < nGridHeight; i++)
	{
		for (int j = 0; j < nGridWidth; j++)
		{
			vector<int> vecLabels;
			int nLeft = j * nStep;
			int nRight = (j + 1) * nStep <= m_sPanel.width ? (j + 1) * nStep : m_sPanel.width;
			int nBottom = i * nStep;
			int nTop = (i + 1) * nStep <= m_sPanel.height ? (i + 1) * nStep : m_sPanel.height;
			for (int ii = nBottom; ii < nTop; ii++)
			{
				for (int jj = nLeft; jj < nRight; jj++)
				{
					m_pHandProjector[ii][jj].IsSample = false;
					bool bContour = m_pHandProjector[ii][jj].IsContour;
					bool bForegrd = m_pHandProjector[ii][jj].IsForegrd;
					int nLabel = m_pHandProjector[ii][jj].NearIntsct.GetLabel();
					if (!bForegrd)
					{
						m_mtxContour.at<cv::Vec3b>(ii, jj) = cv::Vec3b(0, 0, 0);
					}
					else
					{
						if (bContour)
						{
							// in a single grid, each quadric is allowed to have one control point
							if (find(vecLabels.begin(), vecLabels.end(), nLabel) == vecLabels.end())
							{
								vecLabels.push_back(nLabel);
								bmu::Vector3 vNorm;
								double x = m_pHandProjector[ii][jj].Center.x;
								double y = m_pHandProjector[ii][jj].Center.y;
								bmu::Matrix33 mtxConic = m_pHandProjector[ii][jj].NearIntsct.GetQuadric()->GetConic();
								mtxConic.Transform(bmu::Vector3(x, y, 1), vNorm);
								m_pHandProjector[ii][jj].Norm = vNorm;
								m_pHandProjector[ii][jj].IsSample = true;
								m_mtxContour.at<cv::Vec3b>(ii, jj) = cv::Vec3b(0, 0, 255);
							}
							else
							{
								m_mtxContour.at<cv::Vec3b>(ii, jj) = cv::Vec3b(0, 0, 0);
							}
						}
						else
						{
							m_mtxContour.at<cv::Vec3b>(ii, jj) = cv::Vec3b(0, 0, 0);
						}
					}
				}
			}
			vecLabels.clear();
		}
	}

	// draw the measurement line, see reference "Model-based 3D tracking of an articulated hand"
	for (int i = 0; i < m_sPanel.height; i++)
	{
		for (int j = 0; j < m_sPanel.width; j++)
		{
			if (m_pHandProjector[i][j].IsSample == true)
			{
				bmu::Vector3 vNorm = m_pHandProjector[i][j].Norm;
				double fLength = 10;
				CvPoint pt1, pt2;
				if (vNorm.x != 0)
				{										
					double fSlope = vNorm.y / vNorm.x;
					double xRange = fLength / (sqrt(1 + fSlope * fSlope));
					double yRange = fLength * fSlope / (sqrt(1 + fSlope * fSlope));
					pt1.x = (int)(j - xRange / 2);
					pt1.y = (int)(i - yRange / 2);
					pt2.x = (int)(j + xRange / 2);
					pt2.y = (int)(i + yRange / 2);
				}
				else
				{
					pt1.x = j;
					pt1.y = i - fLength / 2;
					pt2.x = j;
					pt2.y = i + fLength / 2;
				}
				cv::line(m_mtxContour, pt1, pt2, cv::Scalar(0x00, 0x99, 0));
				cv::circle(m_mtxContour, cv::Point(j, i), 1, cv::Scalar(0, 0, 0x99), -1);
			}
		}
	}
//	cvFlip(m_pContourImg, 0);
}
