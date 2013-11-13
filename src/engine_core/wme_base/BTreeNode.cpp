// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include ".\BTreeNode.h"


//////////////////////////////////////////////////////////////////////////
CBTreeNode::CBTreeNode(CBGame* inGame) : CBBase(inGame)
{
	m_FirstChild = NULL;
	m_LastChild = NULL;
	m_PrevSibling = NULL;
	m_NextSibling = NULL;
	m_Owner = NULL;
}

//////////////////////////////////////////////////////////////////////////
CBTreeNode::~CBTreeNode(void)
{
	while(m_FirstChild ) DetachChild(*m_FirstChild);

	// detaches this object from it's parent
	if(m_Owner)	m_Owner->DetachChild(*this);
}


//////////////////////////////////////////////////////////////////////////
void CBTreeNode::AttachChild(CBTreeNode& Node)
{
	if(Node.m_Owner) Node.m_Owner->DetachChild(Node);

	Node.m_PrevSibling = m_LastChild;
	Node.m_NextSibling = NULL;
	if(m_LastChild) m_LastChild->m_NextSibling = &Node;
	if(!m_FirstChild) m_FirstChild = &Node;
	m_LastChild = &Node;
	Node.m_Owner = this;
}


//////////////////////////////////////////////////////////////////////////
void CBTreeNode::DetachChild(CBTreeNode& Node)
{
	if(Node.m_PrevSibling) Node.m_PrevSibling->m_NextSibling = Node.m_NextSibling;
	if(Node.m_NextSibling) Node.m_NextSibling->m_PrevSibling = Node.m_PrevSibling;
	if(!Node.m_PrevSibling)	m_FirstChild = Node.m_NextSibling;
	if(!Node.m_NextSibling)	m_LastChild = Node.m_PrevSibling;
	Node.m_PrevSibling = NULL;
	Node.m_NextSibling = NULL;
	Node.m_Owner = NULL;
}


//////////////////////////////////////////////////////////////////////////
void CBTreeNode::DeleteChildren()
{
	CBTreeNode* Node = GetFirstChild();
	while(Node)
	{
		Node->DeleteChildren();
		delete Node;
		Node = GetFirstChild();
	}
}
