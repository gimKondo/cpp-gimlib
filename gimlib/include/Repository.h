#ifndef INCLUDED_GIMLIB_REPOSITORY
#define INCLUDED_GIMLIB_REPOSITORY


#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>

#include "container_make.h"

///////////////////////////////////////
namespace GimLib {
///////////////////////////////////////

enum CondCompareType
{
	COND_EQ, COND_NEQ, COND_LT, COND_LTE, COND_GT, COND_GTE,
};

template <typename ENTITY> struct LeafCond;
template <typename ENTITY> struct AndCond;
template <typename ENTITY> struct OrCond;

//--------------------------------
// Interface of condition builder
//--------------------------------
template <typename ENTITY>
class CondBuilder
{
public:
	virtual void visit(const LeafCond<ENTITY>*	cond) = 0;
	virtual void visit(const AndCond<ENTITY>*	cond) = 0;
	virtual void visit(const OrCond<ENTITY>*	cond) = 0;
};

//--------------------------------
// Interface of condition section
//--------------------------------
template <typename ENTITY> struct IWhereCond
{
	virtual ~IWhereCond() {}
	virtual void acceptCondBuilder(CondBuilder<ENTITY>& builder) = 0;
};

//--------------------------------
// Condition section
//--------------------------------
template <typename ENTITY> struct LeafCond : public IWhereCond<ENTITY>
{
	LeafCond(const std::wstring& key, const std::wstring& val, CondCompareType cmpr)
		: Key(key), Value(val), Cmpr(cmpr)
	{}
	virtual void acceptCondBuilder(CondBuilder<ENTITY>& builder) override { return builder.visit(this); }
	std::wstring	Key		;
	std::wstring	Value	;
	CondCompareType	Cmpr	;
};

//--------------------------------
// AND condition
//--------------------------------
template <typename ENTITY> struct AndCond : public IWhereCond<ENTITY>
{
	AndCond(IWhereCond* cond1, IWhereCond* cond2)
		:Cond1(cond1), Cond2(cond2)
	{}
	virtual void acceptCondBuilder(CondBuilder<ENTITY>& builder) override { return builder.visit(this); }
	std::shared_ptr<IWhereCond> Cond1;
	std::shared_ptr<IWhereCond> Cond2;
};

//--------------------------------
// OR condition
//--------------------------------
template <typename ENTITY> struct OrCond : public IWhereCond<ENTITY>
{
	OrCond(IWhereCond* cond1, IWhereCond* cond2)
		:Cond1(cond1), Cond2(cond2)
	{}
	virtual void acceptCondBuilder(CondBuilder<ENTITY>& builder) override { return builder.visit(this); }
	std::shared_ptr<IWhereCond> Cond1;
	std::shared_ptr<IWhereCond> Cond2;
};



//Valude type on Repository
enum DB_ValueType
{
	DB_VT_INT	,
	DB_VT_STRING,
};

//Field infomaton on Repository
struct DB_FieldInfo
{
	DB_FieldInfo(DB_ValueType vt, const std::wstring& kname, bool isPrimaryKey)
		: VType(vt), KName(kname), IsPrimaryKey(isPrimaryKey)
	{}
	DB_ValueType	VType;
	std::wstring	KName;
	bool			IsPrimaryKey;
};

//Field data on Repository
struct DB_FieldData
{
	DB_FieldData(DB_ValueType vt, const std::wstring& kname, bool isPrimaryKey, std::wstring& data)
		: FieldInfo(vt, kname, isPrimaryKey), StringData(data)
	{}
	DB_FieldData(DB_ValueType vt, const std::wstring& kname, bool isPrimaryKey, int data)
		: FieldInfo(vt, kname, isPrimaryKey), IntData(data)
	{}
	DB_FieldInfo	FieldInfo;
	int				IntData;
	std::wstring	StringData;
};

//--------------------------------
// Interface of Repository
//--------------------------------
template <typename ENTITY> class IRepository
{
public:
	virtual void setup() = 0;
	virtual std::vector<ENTITY> findAll() = 0;
	virtual std::vector<ENTITY> findAll(const std::shared_ptr<IWhereCond<ENTITY>>& cond) = 0;
	virtual std::shared_ptr<ENTITY> findFirst(const std::shared_ptr<IWhereCond<ENTITY>>& cond) = 0;
	virtual bool insert(const ENTITY& entity) = 0;
};

///////////////////////////////////////
} // end namespace GimLib
///////////////////////////////////////

#endif