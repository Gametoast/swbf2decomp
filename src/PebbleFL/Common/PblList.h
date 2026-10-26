#pragma once

template <class TYPE>
class PblList
{
public:
    class Node
    {
    public:

        Node() : _pList(0), _pNext(0), _pPrev(0), _pObject(0) {}

        bool IsInList() const { return _pList != 0; }

        PblList<TYPE>* GetList() { return _pList; }

        PblList<TYPE>* _pList;
        Node* _pNext;
        Node* _pPrev;
        TYPE* _pObject;
    };

    class Itor
    {
    public:

        Itor() : _pNode(0) {}

        Itor(const Itor& itor) : _pNode(itor._pNode) {}

        Itor(const PblList<TYPE>& list) { _pNode = list._head._pNext; }

        // TEMPLATE: SWBF2 0x0049e1c0
        // PblList<Factory<Weapon,WeaponClass,WeaponDesc> >::Itor::Itor
        // TEMPLATE: SWBF2 0x005d3b50
        // PblList<Factory<Ordnance,OrdnanceClass,OrdnanceDesc> >::Itor::Itor
        Itor(Node* pNode) : _pNode(pNode) {}

        // TEMPLATE: SWBF2 0x0049e1f0
        // PblList<Factory<Weapon,WeaponClass,WeaponDesc> >::Itor::operator Factory<Weapon,WeaponClass,WeaponDesc> *
        // TEMPLATE: SWBF2 0x005d3bb0
        // PblList<Factory<Ordnance,OrdnanceClass,OrdnanceDesc> >::Itor::operator Factory<Ordnance,OrdnanceClass,OrdnanceDesc> *
        // TEMPLATE: SWBF2 0x00487f80
        // PblList<FlagItem>::Itor::operator FlagItem *
        operator TYPE*() const { return _pNode->_pObject; }

        // TEMPLATE: SWBF2 0x0049e1e0
        // PblList<Factory<Weapon,WeaponClass,WeaponDesc> >::Itor::operator*
        // TEMPLATE: SWBF2 0x005d3b90
        // PblList<Factory<Ordnance,OrdnanceClass,OrdnanceDesc> >::Itor::operator*
        // TEMPLATE: SWBF2 0x00563920
        // PblList<FlagItem>::Itor::operator*
        // TEMPLATE: SWBF2 0x005d3720
        // PblList<Controllable>::Itor::operator*
        TYPE& operator*() const { return *_pNode->_pObject; }
        // TEMPLATE: SWBF2 0x0049e1d0
        // PblList<Factory<Weapon,WeaponClass,WeaponDesc> >::Itor::operator->
        // TEMPLATE: SWBF2 0x005d3b70
        // PblList<Factory<Ordnance,OrdnanceClass,OrdnanceDesc> >::Itor::operator->
        TYPE* operator->() const { return _pNode->_pObject; }

        // TEMPLATE: SWBF2 0x005d3bd0
        // PblList<Factory<Ordnance,OrdnanceClass,OrdnanceDesc> >::Itor::operator++
        // TEMPLATE: SWBF2 0x00487f90
        // PblList<FlagItem>::Itor::operator++
        Itor& operator++() { _pNode = _pNode->_pNext; return *this; }

        Itor& operator--() { _pNode = _pNode->_pPrev; return *this; }
        Node* GetNode() { return _pNode; }

        Node* _pNode;
    };

    PblList()
    {
        _head._pList = this;
        Node* pNode = &_head;
        _head._pPrev = pNode;
        _head._pNext = pNode;
        _head._pObject = 0;
        _iCount = 0;
    }

    int GetCount() const { return _iCount; }

    // TEMPLATE: SWBF2 0x0049e1b0
    // PblList<Factory<Weapon,WeaponClass,WeaponDesc> >::GetFirst
    // TEMPLATE: SWBF2 0x00487fa0
    // PblList<FlagItem>::GetFirst
    Node* GetFirst() const { return _head._pNext; }

    Itor Begin() const { return Itor(GetFirst()); }

    Node* GetLast() const { return _head._pPrev; }

    Itor End() const { return Itor(const_cast<Node*>(&_head)); }

    void AddBefore(Node* pAt, Node* pNode, TYPE* pObject)
    {
        ++_iCount;
        pNode->_pList = this;
        pNode->_pNext = pAt;
        pNode->_pObject = pObject;
        pNode->_pPrev = pAt->_pPrev;
        pAt->_pPrev = pNode;
        pNode->_pPrev->_pNext = pNode;
    }

    void AddLast(Node* pNode, TYPE* pObject) { AddBefore(&_head, pNode, pObject); }

    void Add(Node* pNode, TYPE* pObject) { AddBefore(&_head, pNode, pObject); }

    void AddFirst(Node* pNode, TYPE* pObject)
    {
        AddBefore(_head._pNext, pNode, pObject);
    }

    void AddAfter(Node* pAt, Node* pNode, TYPE* pObject)
    {
        AddBefore(pAt->_pNext, pNode, pObject);
    }

    // TEMPLATE: SWBF2 0x00460930
    // PblList<CollisionObject>::Remove
    void Remove(Node* pNode)
    {
        --_iCount;
        pNode->_pList = 0;
        pNode->_pObject = 0;
        pNode->_pNext->_pPrev = pNode->_pPrev;
        pNode->_pPrev->_pNext = pNode->_pNext;
    }

    void Clear()
    {
        while (_iCount > 0)
        {
            Node* pNode = _head._pNext;
            TYPE* pObject = pNode->_pObject;
            Remove(pNode);
            delete pObject;
        }
    }

    // TEMPLATE: SWBF2 0x0043a180
    // PblList<Character>::RemoveAll
    // TEMPLATE: SWBF2 0x0056f8c0
    // PblList<LensFlare>::RemoveAll
    // TEMPLATE: SWBF2 0x00695020
    // PblList<PrecipitationEffect::RainShadowRegion>::RemoveAll
    // TEMPLATE: SWBF2 0x006edbe0
    // PblList<RedShadowProxy>::RemoveAll
    // TEMPLATE: SWBF2 0x00526ab0
    // PblList<FlashyTextElement>::RemoveAll
    void RemoveAll()
    {
        while (_iCount > 0)
        {
            Remove(_head._pNext);
        }
    }

    Node _head;
    int _iCount;
};
