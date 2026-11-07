#pragma once

#include "PblHashTable.h"

#define PBL_DECLARE_DERIVED_RTTI(Class, Base) \
public: \
    static PblHash rttiHash##Class; \
    static const char rttiName##Class[]; \
 \
public: \
    virtual bool IsRtti(unsigned int uRtti) const \
    { \
        if (rttiHash##Class == uRtti) \
        { \
            return true; \
        } \
        return Base::IsRtti(uRtti); \
    } \
 \
    static unsigned int GetRtti() { return rttiHash##Class; } \
    static const char* GetRttiName() { return rttiName##Class; } \
    virtual unsigned int GetDerivedRtti() const { return rttiHash##Class; } \
    virtual const char* GetDerivedRttiName() const { return rttiName##Class; }

#define PBL_DECLARE_ROOT_RTTI(Class) \
public: \
    static PblHash rttiHash##Class; \
    static const char rttiName##Class[]; \
 \
public: \
    virtual bool IsRtti(unsigned int uRtti) const { return rttiHash##Class == uRtti; } \
    static unsigned int GetRtti() { return rttiHash##Class; } \
    static const char* GetRttiName() { return rttiName##Class; } \
    virtual unsigned int GetDerivedRtti() const { return rttiHash##Class; } \
    virtual const char* GetDerivedRttiName() const { return rttiName##Class; }

// the two statics of a class that declares one of the above (the _IN form for a class in a namespace)
#define PBL_IMPLEMENT_RTTI(Class) \
PblHash Class::rttiHash##Class(#Class); \
const char Class::rttiName##Class[] = #Class;

#define PBL_IMPLEMENT_RTTI_IN(Namespace, Class) \
PblHash Namespace::Class::rttiHash##Class(#Class); \
const char Namespace::Class::rttiName##Class[] = #Class;
