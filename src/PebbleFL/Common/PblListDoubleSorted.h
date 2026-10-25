#pragma once

#include <stddef.h>

#include "PblListDouble.h"

class PblListDoubleSorted : public PblListDouble
{
public:
    class Node
    {
    public:

        Node(float fSortValue = 0.0f)
        {
            mSortValue = fSortValue;
        }

        // FUNCTION: SWBF2 0x004d26d0
        ~Node()
        {
        }

        void* GetObjectA(PblListDoubleSorted const* pList, unsigned int uOffset) const;

        Node* GetNext(PblListDoubleSorted* const pList) const
        {
            PblListDouble::Node* pNext = mNode.mNext;
            return pNext == &pList->mTerminator
                ? (Node*)pList
                : (Node*)((char*)pNext - offsetof(Node, mNode));
        }

        Node* GetPrevious(PblListDoubleSorted* const pList) const
        {
            PblListDouble::Node* pPrevious = mNode.mPrev;
            return pPrevious == &pList->mTerminator
                ? (Node*)pList
                : (Node*)((char*)pPrevious - offsetof(Node, mNode));
        }

        float mSortValue;
        PblListDouble::Node mNode;
    };

    class Itor : public PblListDouble::Itor
    {
    public:
        Itor(Node* pNode) : PblListDouble::Itor(&pNode->mNode) {}
        Itor(const Itor& itor) : PblListDouble::Itor(itor) {}
        Itor(const PblListDoubleSorted* pList) : PblListDouble::Itor(pList) {}
        Itor(const PblListDoubleSorted& list) : PblListDouble::Itor(list) {}
        Itor() {}

        Node* GetNode(const PblListDoubleSorted* pList)
        {
            PblListDouble::Node* pNode = PblListDouble::Itor::GetNode();
            return pNode == &pList->mTerminator
                ? (Node*)pList
                : (Node*)((char*)pNode - offsetof(Node, mNode));
        }
    };

    Itor Begin() const { return Itor(this); }
    Itor End() { return Itor(GetLast()); }

    PblListDoubleSorted(bool bSortOnAdd = false)
    {
        mFlags = bSortOnAdd ? 1 : 0;
    }

    void SetSortOnAdd(bool bSortOnAdd)
    {
        if (bSortOnAdd)
        {
            mFlags |= 1;
        }
        else
        {
            mFlags = (unsigned char)(mFlags & ~1u);
        }
    }

    // FUNCTION: SWBF2 0x004d26c0
    ~PblListDoubleSorted()
    {
    }

    Node const* GetTerminator() const
    {
        return (Node const*)this;
    }

    // FUNCTION: SWBF2 0x004fbe10
    Node* GetFirst() const
    {
        PblListDouble::Node* pNext = mTerminator.mNext;
        return pNext == &mTerminator
            ? (Node*)pNext
            : (Node*)((char*)pNext - offsetof(Node, mNode));
    }

    Node* GetLast() const
    {
        PblListDouble::Node* pPrevious = mTerminator.mPrev;
        return pPrevious == &mTerminator
            ? (Node*)pPrevious
            : (Node*)((char*)pPrevious - offsetof(Node, mNode));
    }

    // FUNCTION: SWBF2 0x004d2640
    void Add(Node* pNode)
    {
        if (mFlags & 1)
        {
            for (PblListDouble::Node* pLink = mTerminator.mNext; pLink != &mTerminator;
                 pLink = pLink->mNext)
            {
                Node* pCur = (Node*)((char*)pLink - offsetof(Node, mNode));
                if (pCur == GetTerminator())
                {
                    break;
                }

                if (pNode->mSortValue > pCur->mSortValue)
                {
                    AddBefore(pLink, &pNode->mNode);
                    return;
                }
            }
        }

        AddLast(&pNode->mNode);
    }

    // FUNCTION: SWBF2 0x005a9420
    void Sort()
    {
        PblListDouble::Node* pLink = mTerminator.mNext;
        Node* pPrev = (pLink == &mTerminator) ? 0 : (Node*)((char*)pLink - offsetof(Node, mNode));

        for (pLink = pLink->mNext; pLink != &mTerminator; pLink = pLink->mNext)
        {
            Node* pNode = (Node*)((char*)pLink - offsetof(Node, mNode));
            if (pNode == 0)
            {
                break;
            }

            if (pNode->mSortValue > pPrev->mSortValue)
            {
                Node* pAfter;
                PblListDouble::Node* pScan = &pNode->mNode;
                for (;;)
                {
                    pScan = pScan->mPrev;
                    if (pScan == &mTerminator)
                    {
                        pAfter = 0;
                        break;
                    }
                    pAfter = (Node*)((char*)pScan - offsetof(Node, mNode));
                    if (pAfter == 0 || !(pNode->mSortValue > pAfter->mSortValue))
                    {
                        break;
                    }
                }

                pNode->mNode.Remove();

                PblListDouble::Node* pAt = pAfter ? pAfter->mNode.mNext : mTerminator.mNext;
                AddBefore(pAt, &pNode->mNode);
            }

            pPrev = pNode;
            pLink = &pNode->mNode;
        }
    }

    unsigned char mFlags;
};
 
 
