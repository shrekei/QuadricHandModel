// Tree.cpp: implementation of the Tree class.
//
//////////////////////////////////////////////////////////////////////
#ifndef TREE_DEF_CPP
#define TREE_DEF_CPP

#include "Tree.h"
using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//***** implementation for treenode *****///  
template<class T> void TreeNode<T>::SetParent(TreeNode<T> *node)
{  
	_parent = node;  
}

template<class T> void TreeNode<T>::InsertChildren(TreeNode<T> *node)
{  
	_children.push_back(node);  
}

//***** Implementation for Tree*****//

//build a null tree
template<class T> Tree<T>::Tree()
{	
}

//build a tree with a root 
template<class T> Tree<T>::Tree(const T data)
{  
	_nodes.push_back(new TreeNode<T>(data));
}  

//copy constructor
template<class T> Tree<T>::Tree(const Tree<T> &t)
{  
	if(t._nodes.empty())
	{
		return;
	}
	Clone(t._nodes.front(), _nodes, 0);
}

// transform the forest into a combined tree
template<class T> Tree<T>::Tree(const T data, const list<Tree<T>*> &lit)
{  
	TreeNode<T> *root = new TreeNode<T>(data); //set the root  
	_nodes.push_back(root); 
	list<Tree<T> *>::const_iterator it;

	for(it = lit.begin(); it!=lit.end(); it++)
	{  
		if(!((*it)->_nodes.empty()))
		{
			// this new tree should not be deleted afterwards since
			// the node information is needed in the combined tree
			Tree<T> *tp = new Tree<T>(**it);  
			TreeNode<T> *p = tp->_nodes.front();  
			root->_children.push_back(p); 
			p->_parent = root;          
			list<TreeNode*>::iterator lit1 = tp->_nodes.begin();  
			list<TreeNode*>::iterator lit2 = tp->_nodes.end();  
			list<TreeNode*>::iterator lit3 = _nodes.end();  
			_nodes.insert(lit3, lit1, lit2);  
		}  
	}  
}  

// deconstructor, free all node info
template<class T> Tree<T>::~Tree()
{  
	for(list<TreeNode<T>*>::iterator it = _nodes.begin(); it!=_nodes.end(); it++)
	{  
		delete *it;
	}  
}  

template<class T> Tree<T>& Tree<T>::operator=(const Tree<T> &t)
{
	if (this != &t)
	{
		Clear();  

		// this new tree should be reserved since we need 
		// to copy all node info of the source tree
		Tree<T>* p = new Tree<T>(t);  
		_nodes = p->_nodes;  
	}
	return *this; 
}

template<class T> bool Tree<T>::operator==(const Tree<T> &t)
{  
	if (_nodes.size() != t._nodes.size())
	{  
		return false;  
	}

	list<TreeNode<T> *>::iterator it = _nodes.begin();  
	list<TreeNode<T> *>::const_iterator _it = t._nodes.begin();  
	while (it != _nodes.end() && _it != t._nodes.end())
	{  
		if ((*it)->GetData() != (*_it)->GetData())
		{  
			return false;  
		}  
		it++;  
		_it++;  
	}  
	return true;
}  

template<class T> bool Tree<T>::operator !=(const Tree<T> &t)
{  
	return !(operator==(t));
}  

template<class T> void Tree<T>::Clear()
{  
	for (list<TreeNode<T> *>::iterator it = _nodes.begin(); it!=_nodes.end(); it++)
	{  
		delete *it;  
	}  
	_nodes.clear();  
}  

template<class T> bool Tree<T>::IsEmpty() const
{  
	return _nodes.empty();  
}  

template<class T> int Tree<T>::Size() const
{  
	return (int)_nodes.size();  
}  

template<class T> int Tree<T>::Leaves()
{  
	int i = 0;  
	list<TreeNode<T> *>::iterator it = _nodes.begin();  
	while (it!=_nodes.end())
	{  
		if ((*it)->_children.size()==0)
		{  
			i++;  
		}  
		it++;  
	}  
	return i;  
}  

template<class T> int Tree<T>::TreeHeight()
{  
	if (_nodes.size()!=0)
	{  
		TreeNode<T>* root_node = _nodes.front();  
		return NodeHeight(root_node);  
	}  
	else
	{  
		return -1;
	}  
}  

template<class T> int Tree<T>::NodeHeight(TreeNode<T> *node)
{  
	if (!node)
	{  
		return -1;  
	}  
	else
	{  
		list<TreeNode<T> *> plist = node->_children;  
		if (plist.size()==0)
		{  
			return 0;  
		}  
		int hA = 0;  
		for (list<TreeNode<T> *>::iterator it = plist.begin(); it!=plist.end(); it++)
		{  
			int hB = NodeHeight(*it);  
			if(hB > hA)
			{  
				hA = hB;  
			}  
		}
		return hA + 1;  
	}  
} 

template<class T> void Tree<T>::InsertSubTree(TreeNode<T> *p,
											  list<TreeNode<T> *> lstChildren)
{
	if (lstChildren.size == 0)
		return ;

	TreeNode<T> *pSubtreeRoot = *(lstChildren.begin());
	if (p == NULL)
	{
		p = pSubtreeRoot;
	}
	else
	{
		p->InsertChildren(pSubtreeRoot);
		pSubtreeRoot->SetParent(p);
	}	
	for (list<TreeNode<T> *>::iterator lt = lstChildren.begin(); lt != lstChildren.end(); lt++)
	{  
		_nodes.push_back(*lt);
	}
}

template<class T> void Tree<T>::BuildNodeList(TreeNode<T> *root, list<TreeNode<T> *> &lstNodes)
{
	lstNodes.push_back(root);

	if (root->_children.size() == 0)
	{
		return ;
	}
	else
	{
		TreeNode<T>::List lstChildren = root->_children;
		for (TreeNode<T>::List::iterator iter = lstChildren.begin(); iter != lstChildren.end(); iter++)
		{
			BuildNodeList(*iter, lstNodes);
		}
	}
}

template<class T> void Tree<T>::BuildTree(TreeNode<T> *root)
{
	Clear();
	BuildNodeList(root, this->_nodes);
}

template<class T> Iterator<T> Tree<T>::begin()
{  
	return Iterator(this, _nodes.begin());  
}  

template<class T> Iterator<T> Tree<T>::end()
{  
	return Iterator<T>(this,_nodes.end());  
}  

template<class T> TreeNode<T>* Tree<T>::Root() const
{
	if (Size() == 0)
	{
		return NULL;
	}
	return *_nodes.begin();  
}  

template<class T> bool Tree<T>::IsRoot(Iterator<T> it)
{  
	TreeNode<T> p = *it;  
	if (p._parent == 0)
	{  
		return true;  
	}  
	return false;  
}  

template<class T> bool Tree<T>::IsLeaf(Iterator<T> it)
{  
	TreeNode<T> p = *it;  
	if (p._children.size() == 0)
	{  
		return true;  
	}  
	return false;  
}  

template<class T> Iterator<T> Tree<T>::Parent(Iterator<T> it)
{  
	TreeNode<T> p = *it;  
	Tree<T>* t = it._tree;  
	Iterator<T> par_iter(t, p._parent);  
	return par_iter;  
}  

template<class T> int Tree<T>::ChildrenNum(Iterator<T> it)
{  
	TreeNode<T> p = *it;  
	return (int)p._children.size();  
}  

//***** Implementation for Tree::Iterator*****//
template<class T> Iterator<T>::Iterator()
{  
}  

template<class T> Iterator<T>::Iterator(const Iterator<T> &it)
{  
	_tree = it._tree;  
	_lit = it._lit;
}  

template<class T> Iterator<T>::Iterator(Tree<T>* t, TreeNode<T> *n)
{  
	_tree = t;  
	list<TreeNode<T> *> &nodes = _tree->_nodes;  
	_lit = find(nodes.begin(), nodes.end(), n);
}

template<class T> Iterator<T>::Iterator(Tree<T> *t, typename list<TreeNode<T> *>::iterator lt)
{  
	_tree = t;  
	_lit = lt;  
}  

template<class T> void Iterator<T>::operator=(const Iterator<T> &it)
{  
	_tree = it._tree;  
	_lit = it._lit;  
}  

template<class T> bool Iterator<T>::operator ==(const Iterator<T> &it)
{  
	// working on the same tree and pointing to the same position
	return _tree == it._tree && _lit == it._lit;  
}  

template<class T> bool Iterator<T>::operator !=(const Iterator<T> &it)
{  
	return _tree != it._tree || _lit != it._lit;  
}  

template<class T> Iterator<T>& Iterator<T>::operator++()
{  
	++_lit;  
	return *this;  
}  

template<class T> Iterator<T> Iterator<T>::operator++(int) 
{
	// postfix increment needs to return the unchanged obj
	Iterator it(*this);  
	++_lit;  
	return it;  
}

template<class T> T Iterator<T>::operator*() const
{  
	return ((*_lit)->GetData());  
}  

template<class T> bool Iterator<T>::operator!()
{  
	return _lit == _tree->_nodes.end();  
}  

//**************** clone the tree **********************//
// the assumption here is that the input node does not
// necessarily contain the pointer to its parent
template<class T> TreeNode<T>* Clone(TreeNode<T>* node, TreeNode<T>* nodep, std::list<TreeNode<T> *> &nodes)
{
	TreeNode<T>* cp = new TreeNode<T>(node->GetData(), nodep);
	nodes.push_back(cp);  
	TreeNode<T>::List& l = node->_children;  
	TreeNode<T>::List& cl = cp->_children;  
	for (list<TreeNode<T> *>::iterator lt = l.begin(); lt!=l.end(); lt++)
	{  
		cl.push_back(Clone(*lt, cp, nodes));  
	}  
	return cp;  
}

template<class T> void TraverseTree(TreeNode<T> *node)
{
	if (!node)
	{  
		return;  
	}  
	else
	{
		node->PrintInfo();
		list<TreeNode<T> *> plist = node->_children;  
		if (plist.size() == 0)
		{  
			return;  
		}  
		for (list<TreeNode<T> *>::iterator it = plist.begin(); it!=plist.end(); it++)
		{
			TraverseTree(*it);
		}
	}  
}

template<class T, class F>
void TraverseTree(TreeNode<T> *node, F &func)
{
	if (!node)
	{  
		return;  
	}  
	else
	{
		func(node);

		list<TreeNode<T> *> plist = node->_children;  
		if (plist.size() == 0)
		{  
			return;  
		}  
		for (list<TreeNode<T> *>::iterator it = plist.begin(); it!=plist.end(); it++)
		{
			TraverseTree(*it, func);
		}
	}
}

template<class T> void DrawGLNode(TreeNode<T> *node, GLUquadricObj *quad_obj)
{
	node->GetData()->Draw(quad_obj);
}

template<class T> void UpdateGLNode(TreeNode<T> *node)
{
	node->GetData()->Update();
}

template<class T> void DrawGLTree(TreeNode<T> *node, GLUquadricObj *quad_obj)
{
	if (node == NULL || quad_obj == NULL)
	{  
		return;  
	}  
	else
	{
		glPushMatrix();
		DrawGLNode(node, quad_obj);

		list<TreeNode<T> *> plist = node->_children;  
		if (plist.size() != 0)
		{
			for (list<TreeNode<T> *>::iterator it = plist.begin(); it!=plist.end(); it++)
			{
				DrawGLTree(*it, quad_obj);
			}
		}
		glPopMatrix();
		return;  
	}
}

template<class T> void UpdateGLTree(TreeNode<T> *node)
{
	if (!node)
	{  
		return;  
	}  
	else
	{
		glPushMatrix();
		UpdateGLNode(node);
		list<TreeNode<T> *> plist = node->_children;  
		if (plist.size() != 0)
		{
			for (list<TreeNode<T> *>::iterator it = plist.begin(); it!=plist.end(); it++)
			{
				UpdateGLTree(*it);
			}
		}
		glPopMatrix();
		return;  
	}
}

// traverse the whole tree with back tracking, make sure the "treenode operator" backtraks 
// when recursion finishes on a subtree
template<class T, class OP> void BackTrackTree(TreeNode<T> *pRoot, OP &opTracker)
{
	if (!pRoot)
	{
		opTracker.BackTrack();
		return;
	}
	else
	{
		opTracker.SearchNode(pRoot);
		list<TreeNode<T> *> plist = pRoot->_children;  
		if (plist.size() == 0)
		{ 
			opTracker.BackTrack();
			return;  
		}  
		for (list<TreeNode<T> *>::iterator it = plist.begin(); it!=plist.end(); it++)
		{
			BackTrackTree(*it, opTracker);
		}
		opTracker.BackTrack();
	}
}
#endif