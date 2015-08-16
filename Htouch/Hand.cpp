#include "Hand.h"
#include "Global.h"
#include <cctype>
#include <algorithm>
#include <stack>
using namespace std;

const double Hand::z0 = 80;

Hand::Hand()
{
	Init();
}

Hand::~Hand()
{
	Clear();
}

void Hand::Init()
{
	m_pRoot = NULL;
	m_strFileName = "";
	m_vecQuadrics.clear();
	m_bIsInit = false;
	m_vSkinColor = bmu::Vector4(0.761, 0.517, 0.439, 1.0);
}

void Hand::Clear()
{
	m_pRoot = NULL;
	m_strFileName = "";
	m_vecQuadrics.clear();
	m_trHand.Clear();
	m_tkSFP.Close();
	m_bIsInit = false;
}

TreeNode<HandElem>* Hand::ParseFile(string strFileName)
{
	m_strFileName = strFileName;
	TreeNode<HandElem> *root = NULL;
	stack<TreeNode<HandElem> *> stkNodes;
	stack<char> stkBraces;
	char c;
	
	m_tkSFP.Open(strFileName.c_str());
	if (m_tkSFP.IsEOF() == true)
	{
		m_tkSFP.Abort("Null Hand File!");
		return root;
	}

	// read the tree structure
	do
	{
		string strToken = m_tkSFP.GetToken();
		if (strToken != HandElem::s_strElem)
		{
			m_tkSFP.Abort("Wrong Hand File Format!");
			return root;
		}
		else
		{
			// read the joint name		
			string strName = m_tkSFP.GetToken();
			m_tkSFP.SkipWhitespace();
			
			// get to the '{'
			c = m_tkSFP.GetChar();
			if (c != '{')
			{
				m_tkSFP.Abort("Wrong Hand File Format!");
				return root;
			}
			
			// parse the joint info, and push it into stack
			HandElem *elem = new HandElem(strName);
			elem->ReadInfo(m_tkSFP);
			m_pHandElems[elem->GetLabel()] = elem;
			// copy the pointers to quadarics to the vector of the hand
			for (vector<Quadric *>::const_iterator it = elem->GetQuadrics().begin();
				it != elem->GetQuadrics().end(); it++)
			{
				m_vecQuadrics.push_back(*it);
			}
			// insert the hand element to the tree
			TreeNode<HandElem> *pNode = new TreeNode<HandElem>(elem);
			if (stkNodes.empty())
			{
				pNode->SetParent(NULL);
				root = pNode;
			}
			else
			{
				pNode->SetParent(stkNodes.top());				
				stkNodes.top()->InsertChildren(pNode); 
			}
			stkBraces.push('{');
			stkNodes.push(pNode);
			
			// check whether these are some '}'
			// for every '}', pop the stack
			do
			{
				if (m_tkSFP.IsEOF() == true)
				{
					break;
				}
				m_tkSFP.SkipWhitespace();
				c = m_tkSFP.CheckChar();
				
				if (c == '}')
				{
					stkBraces.pop();
					stkNodes.pop();
					// igonore the '}'
					c = m_tkSFP.GetChar();
				}
			}while (c == '}'); 
		}
	}while (!(stkNodes.empty()));
	
	m_tkSFP.Close();
	m_pRoot = root;
	m_trHand.BuildTree(m_pRoot);
	TraverseTree(m_pRoot);
	Update();
	m_bIsInit = true;
		
//	Normalize();

	return root;
}

double Hand::GetDOF(int nIndex)
{
	bmu::Vector3 vOffset = m_pHandElems[0]->GetOffset();	// global offset
	bmu::Vector3 vPose = m_pHandElems[0]->GetPose();		// global angles
	double fValue;
	switch(nIndex)
	{
	case 0:	fValue = vOffset.x;	break;
	case 1:	fValue = vOffset.y;	break;
	case 2:	fValue = vOffset.z;	break;
	case 3:	fValue = vPose.x;	break;
	case 4:	fValue = vPose.y;	break;
	case 5:	fValue = vPose.z;	break;
	case 6: fValue = m_pHandElems[1]->GetPose().x;		break;	// thumb tm 2Dofs - x and z 
	case 7: fValue = m_pHandElems[1]->GetPose().z;		break;	// thumb tm 2Dofs - x and z 
	case 8: fValue = m_pHandElems[2]->GetPose().x;		break;	// index mcp 2Dofs - x and z 
	case 9: fValue = m_pHandElems[2]->GetPose().z;		break;	// index mcp 2Dofs - x and z 
	case 10: fValue = m_pHandElems[3]->GetPose().x;		break;	// middle mcp 2Dofs - x and z 
	case 11: fValue = m_pHandElems[3]->GetPose().z;		break;	// middle mcp 2Dofs - x and z 
	case 12: fValue = m_pHandElems[4]->GetPose().x;		break;	// ring mcp 2Dofs - x and z 
	case 13: fValue = m_pHandElems[4]->GetPose().z;		break;	// ring mcp 2Dofs - x and z 
	case 14: fValue = m_pHandElems[5]->GetPose().x;		break;	// pinky mcp 2Dofs - x and z 
	case 15: fValue = m_pHandElems[5]->GetPose().z;		break;	// pinky mcp 2Dofs - x and z 
	case 16: fValue = m_pHandElems[6]->GetPose().x;		break;	// thumb mcp 2Dofs - x and z 
	case 17: fValue = m_pHandElems[6]->GetPose().z;		break;	// thumb mcp 2Dofs - x and z 
	case 18: fValue = m_pHandElems[7]->GetPose().x;		break;	// index pip 1Dofs - x
	case 19: fValue = m_pHandElems[8]->GetPose().x;		break;	// middle pip 1Dofs - x
	case 20: fValue = m_pHandElems[9]->GetPose().x;		break;	// ring pip 1Dofs - x
	case 21: fValue = m_pHandElems[10]->GetPose().x;		break;	// pinky pip 1Dofs - x
	case 22: fValue = m_pHandElems[11]->GetPose().x;		break;	// thumb ip 1Dofs - x
	case 23: fValue = m_pHandElems[12]->GetPose().x;		break;	// index dip 1Dofs - x
	case 24: fValue = m_pHandElems[13]->GetPose().x;		break;	// middle dip 1Dofs - x
	case 25: fValue = m_pHandElems[14]->GetPose().x;		break;	// ring dip 1Dofs - x
	case 26: fValue = m_pHandElems[15]->GetPose().x;		break;	// pinky dip 1Dofs - x
	default: printf("Wrong Index!\n"); break;
	}
	return fValue;
}

void Hand::SetDOF(double fValue, int nIndex)
{
	bmu::Vector3 vOffset = m_pHandElems[0]->GetOffset();	// global offset
	bmu::Vector3 vPose = m_pHandElems[0]->GetPose();		// global angles
	switch(nIndex)
	{
	case 0:	m_pHandElems[0]->SetOffset(bmu::Vector3(fValue, vOffset.y, vOffset.z));	break;
	case 1:	m_pHandElems[0]->SetOffset(bmu::Vector3(vOffset.x, fValue, vOffset.z));	break;
	case 2:	m_pHandElems[0]->SetOffset(bmu::Vector3(vOffset.x, vOffset.y, fValue));	break;
	case 3:	m_pHandElems[0]->SetPose(bmu::Vector3(fValue, vPose.y, vPose.z));		break;
	case 4:	m_pHandElems[0]->SetPose(bmu::Vector3(vPose.x, fValue, vPose.z));		break;
	case 5:	m_pHandElems[0]->SetPose(bmu::Vector3(vPose.x, vPose.y, fValue));		break;
		
	case 6: m_pHandElems[1]->SetPose(bmu::Vector3(fValue, 0, 0));		break;	// thumb tm 2Dofs - x and z 
	case 7: m_pHandElems[1]->SetPose(bmu::Vector3(0, 0, fValue));		break;	// thumb tm 2Dofs - x and z 
	case 8: m_pHandElems[2]->SetPose(bmu::Vector3(fValue, 0, 0));		break;	// index mcp 2Dofs - x and z 
	case 9: m_pHandElems[2]->SetPose(bmu::Vector3(0, 0, fValue));		break;	// index mcp 2Dofs - x and z 
	case 10: m_pHandElems[3]->SetPose(bmu::Vector3(fValue, 0, 0));		break;	// middle mcp 2Dofs - x and z 
	case 11: m_pHandElems[3]->SetPose(bmu::Vector3(0, 0, fValue));		break;	// middle mcp 2Dofs - x and z 
	case 12: m_pHandElems[4]->SetPose(bmu::Vector3(fValue, 0, 0));		break;	// ring mcp 2Dofs - x and z 
	case 13: m_pHandElems[4]->SetPose(bmu::Vector3(0, 0, fValue));		break;	// ring mcp 2Dofs - x and z 
	case 14: m_pHandElems[5]->SetPose(bmu::Vector3(fValue, 0, 0));		break;	// pinky mcp 2Dofs - x and z 
	case 15: m_pHandElems[5]->SetPose(bmu::Vector3(0, 0, fValue)); 		break;	// pinky mcp 2Dofs - x and z 
	case 16: m_pHandElems[6]->SetPose(bmu::Vector3(fValue, 0, 0));		break;	// thumb mcp 2Dofs - x and z 
	case 17: m_pHandElems[6]->SetPose(bmu::Vector3(0, 0, fValue));		break;	// thumb mcp 2Dofs - x and z 
	case 18: m_pHandElems[7]->SetPose(bmu::Vector3(fValue, 0, 0));		break;	// index pip 1Dofs - x
	case 19: m_pHandElems[8]->SetPose(bmu::Vector3(fValue, 0, 0));		break;	// middle pip 1Dofs - x
	case 20: m_pHandElems[9]->SetPose(bmu::Vector3(fValue, 0, 0));		break;	// ring pip 1Dofs - x
	case 21: m_pHandElems[10]->SetPose(bmu::Vector3(fValue, 0, 0));		break;	// pinky pip 1Dofs - x
	case 22: m_pHandElems[11]->SetPose(bmu::Vector3(fValue, 0, 0));		break;	// thumb ip 1Dofs - x
	case 23: m_pHandElems[12]->SetPose(bmu::Vector3(fValue, 0, 0));		break;	// index dip 1Dofs - x
	case 24: m_pHandElems[13]->SetPose(bmu::Vector3(fValue, 0, 0));		break;	// middle dip 1Dofs - x
	case 25: m_pHandElems[14]->SetPose(bmu::Vector3(fValue, 0, 0));		break;	// ring dip 1Dofs - x
	case 26: m_pHandElems[15]->SetPose(bmu::Vector3(fValue, 0, 0));		break;	// pinky dip 1Dofs - x
	default: printf("Wrong Index!\n"); break;
	}
	Update();
}

void Hand::Draw(GLUquadricObj *quad_obj)
{	
	DrawGLTree(m_trHand.Root(), quad_obj);
}

void Hand::Update()
{
	// update the model view matrix of all quadrics
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	UpdateGLTree(m_trHand.Root());
}

void Hand::Normalize(void)
{
	//get the bounding vertices of the hand model
	m_vOBBMin.Set(INF, INF, INF);
	m_vOBBMax.Set(-INF, -INF, -INF);
	for (vector<Quadric *>::const_iterator it = m_vecQuadrics.begin(); it != m_vecQuadrics.end(); it++)
	{
		const bmu::Vector4 *pOBB = (*it)->GetOBB();
		for (int i = 0; i < 8; i++)
		{
			// update m_vOBBMin
			if (pOBB[i].x < m_vOBBMin.x)
			{
				m_vOBBMin.x = pOBB[i].x;
			}
			if (pOBB[i].y < m_vOBBMin.y)
			{
				m_vOBBMin.y = pOBB[i].y;
			}
			if (pOBB[i].z < m_vOBBMin.z)
			{
				m_vOBBMin.z = pOBB[i].z;
			}

			// update m_vOBBMax
			if (pOBB[i].x > m_vOBBMax.x)
			{
				m_vOBBMax.x = pOBB[i].x;
			}
			if (pOBB[i].y > m_vOBBMax.y)
			{
				m_vOBBMax.y = pOBB[i].y;
			}
			if (pOBB[i].z > m_vOBBMax.z)
			{
				m_vOBBMax.z = pOBB[i].z;
			}
		}
	}

	// move the center of OBB to the origin of the x-y plane
	bmu::Vector3 vOBBCenter;
	vOBBCenter.x = (m_vOBBMax.x - m_vOBBMin.x) / 2 + m_vOBBMin.x;
	vOBBCenter.y = (m_vOBBMax.y - m_vOBBMin.y) / 2 + m_vOBBMin.y;
	vOBBCenter.Scale(-1.0);

	// add the displacement to the current offset of the root node
	vOBBCenter.x += m_pRoot->GetData()->GetOffset().x;
	vOBBCenter.y += m_pRoot->GetData()->GetOffset().y;
	
	// make sure that the front surface is z0 meters away from the image plane 
	vOBBCenter.z = z0 + (m_vOBBMax.z - m_vOBBMin.z) / 2;
	(m_pRoot->GetData())->SetOffset(vOBBCenter);
	Update();

#ifdef HTOUCH_DEBUG
	printf("CENTER V: %lf, %lf, %lf\n", vOBBCenter.x, vOBBCenter.y, vOBBCenter.z);
	printf("MIN V: %lf, %lf, %lf\n", m_vOBBMin.x, m_vOBBMin.y, m_vOBBMin.z);
	printf("MAX V: %lf, %lf, %lf\n", m_vOBBMax.x, m_vOBBMax.y, m_vOBBMax.z);
	printf("ROOT: %lf, %lf, %lf\n", m_pRoot->GetData()->GetOffset().x, m_pRoot->GetData()->GetOffset().y, m_pRoot->GetData()->GetOffset().z);
	printf("ROOT POSE: %lf, %lf, %lf\n", m_pRoot->GetData()->GetPose().x, m_pRoot->GetData()->GetPose().y, m_pRoot->GetData()->GetPose().z);
#endif
}

bmu::Vector3 Hand::GetOBBSpan(void)
{
	return bmu::Vector3(m_vOBBMax.x - m_vOBBMin.x, m_vOBBMax.y - m_vOBBMin.y, z0);
}