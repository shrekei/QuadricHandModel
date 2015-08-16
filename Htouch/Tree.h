#pragma once
#include <list>  
#include <algorithm>
#include <glut.h>

template<class T> class TreeNode;
template<class T> class Tree; 
template<class T> class Iterator;
template<class T> TreeNode<T>* Clone(TreeNode<T> *, TreeNode<T> *, std::list<TreeNode<T> *> &);	//clone the tree
template<class T> void TraverseTree(TreeNode<T> *);
template<class T, class F> void TraverseTree(TreeNode<T> *, F&); 
template<class T, class OP> void BackTrackTree(TreeNode<T> *, OP&);		// traverse the whole tree with back tracking
template<class T> void DrawGLTree(TreeNode<T> *, GLUquadricObj *); 
template<class T> void UpdateGLTree(TreeNode<T> *); 
template<class T> void DrawGLNode(TreeNode<T> *, GLUquadricObj *);
template<class T> void UpdateGLNode(TreeNode<T> *);

//children link list representation of the tree
template<class T> class TreeNode
{ 
public:	
	typedef std::list<TreeNode<T> *> List; 

public:
	TreeNode() : _parent(NULL) {}
	TreeNode(T *data) : _data(data), _parent(NULL) {}
	TreeNode(T *data, TreeNode<T> *parent) : _data(data), _parent(parent) {}
	void SetParent(TreeNode<T> *);			//set its parent
	void InsertChildren(TreeNode<T> *);		//insert a node to children list
	void PrintInfo(void) { _data->PrintInfo(); }
	T* GetData(void) {return _data;}
	List& GetChildrenList(void) { return _children; }
	
public:
	T *_data;								//data
	TreeNode<T> *_parent;					//parent 
	List _children;							//children
};  

template<class T> class Tree  
{
public:  	
	Tree();									//default construction function  
	Tree(const Tree<T> &);					//copy construction function
	Tree(const T);							//build a tree with a root node
	Tree(const T, const std::list<Tree<T> *> &);
	~Tree();								//deconstructor  
	Tree<T>& operator=(const Tree<T> &);	//= overload  
	bool operator==(const Tree<T> &);		//== overload  
	bool operator!=(const Tree<T> &);		//!=overload  
	friend class Iterator<T>;				//Iterator SubClass

public:
	void Clear();							//clear the tree 
	bool IsEmpty() const;					//check whether it is empty  
	int Size() const;						//number of nodes
	int Leaves();							//number of leaves
	TreeNode<T>* Root() const;							//get the root
	int TreeHeight();						//get the tree height 	
	int NodeHeight(TreeNode<T> *);			//get the node height
	int Level(TreeNode<T> *, Iterator<T>);	//get the node level
	void InsertSubTree(TreeNode<T> *, std::list<TreeNode<T> *>);	//insert a sub-tree
	void BuildNodeList(TreeNode<T> *root, std::list<TreeNode<T> *> &lstNodes); //use children-list to store the tree
	void BuildTree(TreeNode<T> *root); //clear original tree, and build a new tree with a root node

public:
	static bool IsRoot(Iterator<T>);			//check whether it is the root
	static bool IsLeaf(Iterator<T>);			//check whether it is the leaf
	static Iterator<T> Parent(Iterator<T>);		//get its parent
	static int ChildrenNum(Iterator<T>);		//get the number of children  
	
public:
	Iterator<T> begin();						//Tree Begin  
	Iterator<T> end();							//Tree End  

private:  
	std::list<TreeNode<T> *> _nodes;			//node array  
	typename std::list<TreeNode<T> *>::iterator _nit;	//node iterator  
};

//Tree sub-class iterator  
template<class T> class Iterator
{
public:  
    Iterator();
    Iterator(const Iterator<T> &);
    Iterator(Tree<T> *, TreeNode<T> *);
    Iterator(Tree<T> *, typename std::list<TreeNode<T> *>::iterator);
	
public:
    void operator=(const Iterator<T> &); 
    bool operator==(const Iterator<T> &);  
    bool operator!=(const Iterator<T> &); 
    Iterator<T>& operator++();      
    Iterator<T> operator++(int);  
    T operator*() const; 
    bool operator!();   

    typedef typename std::list<TreeNode<T> *>::iterator List;  
    friend class Tree<T>;  
private:   
    Tree<T> *_tree;								//Tree data  
    typename std::list<TreeNode<T> *>::iterator _lit;	//List Iterator
};

