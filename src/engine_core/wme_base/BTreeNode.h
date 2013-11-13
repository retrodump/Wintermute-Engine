// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once
#include "BBase.h"

class CBTreeNode : public CBBase
{
public:
	CBTreeNode(CBGame* inGame);
	virtual ~CBTreeNode();

	virtual void Destroy() {}
	void DeleteChildren();

	void AttachChild(CBTreeNode& Node);
	void DetachChild(CBTreeNode& Node);
	CBTreeNode* GetNextSibling()
	{
		return m_NextSibling;
	}

	CBTreeNode* GetPrevSibling()
	{
		return m_PrevSibling;
	}

	CBTreeNode* GetOwner()
	{
		return m_Owner;
	}

	CBTreeNode* GetFirstChild()
	{
		return m_FirstChild;
	}

	CBTreeNode* GetLastChild()
	{
		return m_LastChild;
	}


protected:
	CBTreeNode* m_Owner;
	CBTreeNode* m_PrevSibling;
	CBTreeNode* m_NextSibling;
	CBTreeNode* m_FirstChild;
	CBTreeNode* m_LastChild;

};
