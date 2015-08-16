#pragma once
#include <string>
#include "Tokenizer.h"
#include "HandElem.h"
#include "Tree.cpp"

class Hand 
{
public:
	Hand();
	virtual ~Hand();
	void Init();
	void Clear();

public:
	TreeNode<HandElem>* ParseFile(std::string strFileName);
	void Draw(GLUquadricObj *);
	std::vector<Quadric *> &GetQuadrics(void) {return m_vecQuadrics;}
	std::string GetFileName(void) {return m_strFileName;}
	bool IsInit(void) {return m_bIsInit;}

public:
	void	SetDOF(double fValue, int nIndex);
	double	GetDOF(int nIndex);

public:
	void Update();				// update the model view matrix
	void Normalize(void);		// move the center of OBB to the origin of the x-y plane
	bmu::Vector3 GetOBBSpan(void);	// get the width, height and z-distance from the origin
	bmu::Vector4 GetSkinColor(void) {return m_vSkinColor;}	// get the skin color
	void SetSkinColor(bmu::Vector4 vSkinColor) {m_vSkinColor = vSkinColor;}

private:
	HandElem* m_pHandElems[16];				// pointers to each hand element
	std::vector<Quadric *> m_vecQuadrics;	// pointers to each quadric
	TreeNode<HandElem> *m_pRoot;			// root of the hand element tree
	Tree<HandElem> m_trHand;				// the hand element tree
	Tokenizer m_tkSFP;						// the hand model file parser

private:
	std::string m_strFileName;				// the hand model file name
	bool m_bIsInit;							// flag of model initialization
	bmu::Vector4 m_vOBBMin, m_vOBBMax;			// vertices of the bounding volume
	bmu::Vector4 m_vSkinColor;

private:
	static const double z0;		// normalized z-distance should be suitable for model projection
};
