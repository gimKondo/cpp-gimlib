#ifndef INCLUDED_GIMLIB_REPOSITORY
#define INCLUDED_GIMLIB_REPOSITORY


#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <functional>
#include <algorithm>

#include "container_make.h"

///////////////////////////////////////
namespace GimLib {
///////////////////////////////////////

/***********************************************************************/
/*!	@brief value type of field
************************************************************************
	@details
************************************************************************
	@note
************************************************************************
	@author		gim_kondo
	@version	1.0
	@date		2013/6/24
************************************************************************/
enum RPS_ValueType
{
	RPS_VT_INT		,
	RPS_VT_DOUBLE	,
	RPS_VT_STRING	,
};

/***********************************************************************/
/*!	@brief Field infomaton on Repository
************************************************************************
	@details
		
************************************************************************
	@author		gim_kondo
	@version	1.0
	@date		2013/6/24
************************************************************************/
struct RPS_FieldInfo
{
	RPS_FieldInfo(RPS_ValueType vt, const std::wstring& name)
		: ValueType(vt), Name(name)
	{}
	RPS_ValueType	ValueType;
	std::wstring	Name;
};

/***********************************************************************/
/*!	@brief data of field
************************************************************************
	@details
************************************************************************
	@par
		name		| expression
	---------------------------------------------
		ValueType	| data type
		HasData		| has data or not
		IntData		| data as integer
		DoubleData	| data as double
		StringData	| data as string
************************************************************************
	@note
************************************************************************
	@author		gim_kondo
	@version	1.0
	@date		2013/6/24
************************************************************************/
struct RPS_FieldData
{
	RPS_FieldData() : ValueType(RPS_VT_INT), HasData(false) {}
	explicit RPS_FieldData(int					data) : ValueType(RPS_VT_INT	), HasData(true), IntData	(data) {}
	explicit RPS_FieldData(double				data) : ValueType(RPS_VT_DOUBLE	), HasData(true), DoubleData(data) {}
	explicit RPS_FieldData(const std::wstring&	data) : ValueType(RPS_VT_STRING	), HasData(true), StringData(data) {}
	explicit RPS_FieldData(RPS_ValueType	vt	) : ValueType(vt), HasData(false) {}
	RPS_ValueType	ValueType	;
	bool			HasData		;
	union {
		int			IntData		;
		double		DoubleData	;
	};
	std::wstring	StringData	;
public:
	std::wstring ToString() const
	{
		if (!HasData) {
			return L"";
		}
		else if (ValueType == RPS_VT_STRING) {
			return StringData;
		}
		else if (ValueType == RPS_VT_INT) {
			std::wstringstream ss;
			ss << IntData;
			return ss.str();
		}
		else if (ValueType == RPS_VT_DOUBLE) {
			std::wstringstream ss;
			ss << DoubleData;
			return ss.str();
		}
		return L"";
	}
};

// constance that expressing empty data of field
const RPS_FieldData RPS_EMPTY_FIELD;


struct LeafCond;
struct AndCond;
struct OrCond;

/***********************************************************************/
/*!	@brief Interface of condition builder
************************************************************************
	@details
		
************************************************************************
	@note
		- now writing...
************************************************************************
	@author		gim_kondo
	@version	1.0
	@date		2013/6/24
************************************************************************/
class CondBuilder
{
public:
	virtual void visit(const LeafCond*	cond) = 0;
	virtual void visit(const AndCond*	cond) = 0;
	virtual void visit(const OrCond*	cond) = 0;
};

/***********************************************************************/
/*!	@brief Interface of condition section
************************************************************************
	@details
		
************************************************************************
	@note
		- now writing...
************************************************************************
	@author		gim_kondo
	@version	1.0
	@date		2013/6/24
************************************************************************/
struct IWhereCond
{
	virtual ~IWhereCond() {}
	virtual void acceptCondBuilder(CondBuilder& builder) = 0;
};

// type of comparation
enum CondCompareType
{
	COND_EQ, COND_NEQ, COND_LT, COND_LTE, COND_GT, COND_GTE,
};

/***********************************************************************/
/*!	@brief Condition section
************************************************************************
	@details
		
************************************************************************
	@note
		- now writing...
************************************************************************
	@author		gim_kondo
	@version	1.0
	@date		2013/6/24
************************************************************************/
struct LeafCond : public IWhereCond
{
	LeafCond(const std::wstring& key, int nVal, CondCompareType cmpr = COND_EQ)
		: Key(key), Value(nVal), Cmpr(cmpr)
	{}
	LeafCond(const std::wstring& key, double nVal, CondCompareType cmpr = COND_EQ)
		: Key(key), Value(nVal), Cmpr(cmpr)
	{}
	LeafCond(const std::wstring& key, const std::wstring& strVal, CondCompareType cmpr = COND_EQ)
		: Key(key), Value(strVal), Cmpr(cmpr)
	{}
	LeafCond(const std::wstring& key, const RPS_FieldData& data, CondCompareType cmpr = COND_EQ)
		: Key(key), Value(data), Cmpr(cmpr)
	{}
	virtual void acceptCondBuilder(CondBuilder& builder) override { return builder.visit(this); }
	std::wstring	Key		;
	RPS_FieldData	Value	;
	CondCompareType	Cmpr	;
};

/***********************************************************************/
/*!	@brief AND condition
************************************************************************
	@details
		
************************************************************************
	@note
		- now writing...
************************************************************************
	@author		gim_kondo
	@version	1.0
	@date		2013/6/24
************************************************************************/
struct AndCond : public IWhereCond
{
	AndCond(IWhereCond* cond1, IWhereCond* cond2)
		:Cond1(cond1), Cond2(cond2)
	{}
	virtual void acceptCondBuilder(CondBuilder& builder) override { return builder.visit(this); }
	std::shared_ptr<IWhereCond> Cond1;
	std::shared_ptr<IWhereCond> Cond2;
};

/***********************************************************************/
/*!	@brief OR condition
************************************************************************
	@details
		
	@code
	@endcode
************************************************************************
	@note
		- now writing...
************************************************************************
	@author		gim_kondo
	@version	1.0
	@date		2013/6/24
************************************************************************/
struct OrCond : public IWhereCond
{
	OrCond(IWhereCond* cond1, IWhereCond* cond2)
		:Cond1(cond1), Cond2(cond2)
	{}
	virtual void acceptCondBuilder(CondBuilder& builder) override { return builder.visit(this); }
	std::shared_ptr<IWhereCond> Cond1;
	std::shared_ptr<IWhereCond> Cond2;
};

/***********************************************************************/
/*!	@brief Interface of Repository
************************************************************************
	@details
		
	@code
	@endcode
************************************************************************
	@par
		name		| expression
	-----------------------------------------------------
		findAll		| (1) get all records
		 			| (2) get all records that satisfyed condition
		findFirst	| get first record  that satisfyed condition
		insert		| insert record
		update		| update record
		remove		| remove record
************************************************************************
	@note
		- now writing...
************************************************************************
	@author		gim_kondo
	@version	1.0
	@date		2013/6/24
************************************************************************/
template <typename ENTITY> class IRepository
{
public:
	virtual std::vector<ENTITY> findAll() = 0;
	virtual std::vector<ENTITY> findAll(const std::shared_ptr<IWhereCond>& cond) = 0;
	virtual std::shared_ptr<ENTITY> findFirst(const std::shared_ptr<IWhereCond>& cond) = 0;
	virtual bool insert(const ENTITY& entity) = 0;
	virtual bool update(const std::shared_ptr<IWhereCond>& cond, const ENTITY& entity) = 0;
	virtual bool remove(const std::shared_ptr<IWhereCond>& cond) = 0;
};

///////////////////////////////////////
} // end namespace GimLib
///////////////////////////////////////

#endif
