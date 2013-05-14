#ifndef INCLUDED_GIMLIB_LINQ
#define INCLUDED_GIMLIB_LINQ

#include <functional>
#include <vector>
#include <map>
#include <type_traits>

/////////////////////////////////////////////////////
namespace GimLib	{

/***********************************************************************/
/*!	@namespace Linq
	@brief providing Linq-like system
************************************************************************
	@details
		You can treat several containers by special style
		that is similar to LINQ.
		Refer to following code sample, if you wanna know how to use.
	@code
		std::vector<int> nums = {2,6,4,5,6,3,7,9,8,8,1};
		std::function<bool(const int&)> myCond = [](int e) { return e % 2 == 0; };
		std::function<int(const int&)> myConv = [](int e) { return e * 2; };
		// you can get query that evaluate lazily
		auto query = From(nums)
					| Where(myCond)
					| Select(myConv);
		query.all(); //evaluation

		// you can get result as array that evaluated immediately
		auto resArr = From(nums)
					| Where(myCond)
					| Select(myConv)
					| Eval::All;

		// you can get a first matched element that evaluated immediately
		auto resElm = From(nums)
					| Where(myCond)
					| Select(myConv)
					| Eval::First;
	@endcode
************************************************************************
	@author		gim_kondo
	@version	1.0
	@date		2013/5/8
************************************************************************/
namespace Linq		{
/////////////////////////////////////////////////////

template <typename T> struct is_pass_as_value
{
	static const bool value = std::is_fundamental<T>::value || std::is_enum<T>::value;
};

template <typename ELM, bool IS_PASS_AS_VALUE = is_pass_as_value<ELM>::value> struct CondType
{
	typedef ELM ElmType;
	typedef std::function<bool(ElmType)> Type;
};
template <typename ELM> struct CondType<ELM, false>
{
	typedef const ELM& ElmType;
	typedef std::function<bool(ElmType)> Type;
};

template <typename ELM, typename RET, bool IS_PASS_AS_VALUE = is_pass_as_value<ELM>::value> struct ConvType
{
	typedef ELM ElmType;
	typedef RET RetType;
	typedef std::function<RET(ElmType)> Type;
};
template <typename ELM, typename RET> struct ConvType<ELM, RET, false>
{
	typedef const ELM& ElmType;
	typedef RET RetType;
	typedef std::function<RET(ElmType)> Type;
};

/***********************************************************************/
/*!	@class Where_t
	@brief express Where section on LINQ
************************************************************************
	@details
************************************************************************/
template <typename ELM>
class Where_t
{
public:
	typedef typename CondType<ELM>::Type Cond;
	typedef typename CondType<ELM>::ElmType ElmType;
	Where_t(Cond cond) : cond_(cond) {}
	Cond cond_;
	bool operator()(ElmType elm) { return cond_(elm); }
};

/***********************************************************************/
/*!	@fn Where
	@brief make Where_t object
************************************************************************
	@param[in]	cond	condition to filter container
	@return		Select_t object
************************************************************************
	@details
************************************************************************/
template <typename ELM>
Where_t<ELM> Where(std::function<bool(const ELM&)> cond)
{
	return Where_t<ELM>(cond);
}
template <typename ELM>
Where_t<ELM> Where(std::function<bool(ELM)> cond)
{
	return Where_t<ELM>(cond);
}

/***********************************************************************/
/*!	@class Select_t
	@brief express Select section on LINQ
************************************************************************
	@details
************************************************************************/
template <typename RType, typename OType>
class Select_t
{
public:
	typedef typename ConvType<OType, RType>::Type Conv;
	typedef typename ConvType<OType, RType>::ElmType ElmType;
	Select_t(Conv conv) : conv_(conv) {}
	Conv conv_;
	RType operator()(ElmType o) { return conv_(o); }
};

/***********************************************************************/
/*!	@fn Select
	@brief make Select_t object
************************************************************************
	@param[in]	conv	element converter
	@return		Select_t object
************************************************************************
	@details
************************************************************************/
template <typename RET, typename ELM>
Select_t<RET, ELM> Select(const std::function<RET(const ELM&)>& conv)
{
	return Select_t<RET, ELM>(conv);
}
template <typename RET, typename ELM>
Select_t<RET, ELM> Select(const std::function<RET(ELM)>& conv)
{
	return Select_t<RET, ELM>(conv);
}


namespace Eval {
/***********************************************************************/
/*!	@enum tagAll
	@brief use on get result array of query by array
************************************************************************/
enum tagAll { All };

/***********************************************************************/
/*!	@enum tagFirst
	@brief use on get first matched element of query
************************************************************************/
enum tagFirst { First };
}

/***********************************************************************/
/*!	@class SelectQuery
	@brief Query for select
************************************************************************
	@details
************************************************************************/
template <typename CONTAINER, typename ELM, typename ReturnElem>
class SelectQuery
{
public:
	typedef Where_t<ELM> WhereType;
	typedef Select_t<ReturnElem, ELM> SelectType;
	typedef std::vector<ReturnElem> ReturnArray;

	//! constructor
	SelectQuery(const CONTAINER& container, WhereType where, SelectType& select)
		: container_(container), where_(where), select_(select)
	{}
	//! constructor
	SelectQuery(CONTAINER&& container, WhereType where, SelectType& select)
		: container_(std::move(container)), where_(where), select_(select)
	{}

	//! get filtered array
	ReturnArray operator|(Eval::tagAll)
	{
		return all();
	}
	//! get first matched element
	ReturnElem operator|(Eval::tagFirst)
	{
		return first();
	}

private:
	CONTAINER container_;
	WhereType where_;
	SelectType select_;

private:
	//! get filtered array
	ReturnArray all()
	{
		ReturnArray ret;
		for(CONTAINER::iterator itr = container_.begin(); itr != container_.end(); ++itr)
		{
			if (where_(*itr))
			{
				ret.push_back(select_(*itr));
			}
		}
		return ret;
	}

	//! get first matched element
	ReturnElem first()
	{
		for(CONTAINER::iterator itr = container_.begin(); itr != container_.end(); ++itr)
		{
			if (where_(*itr))
			{
				return select_(*itr);
			}
		}
		return ReturnElem();
	}
};

/***********************************************************************/
/*!	@class From_t
	@brief express From section on LINQ
************************************************************************
	@details
************************************************************************/
template <typename CONTAINER, typename ELM>
class From_t
{
private:
	static bool zzDefWhere(const ELM&) { return true; }

public:
	CONTAINER container_;
	Where_t<ELM> where_;


	//! construct with copying container
	explicit From_t(const CONTAINER& container)
		: container_(container), where_(zzDefWhere)
	{}
	//! construct with moving container
	explicit From_t(CONTAINER&& container)
		: container_(std::move(container)), where_(zzDefWhere)
	{}

	//! set Where section
	From_t& operator|( const Where_t<ELM>& whr )
	{
		where_ = whr;
		return *this;
	}

	//! set Select section and return select query
	template <typename RType>
	SelectQuery<CONTAINER,ELM,RType> operator|( Select_t<RType, ELM>& sel )
	{
		return SelectQuery<CONTAINER,ELM,RType>(container_, where_, sel);
	}
};

/***********************************************************************/
/*!	@fn From
	@brief make From_t object
************************************************************************
	@param[in]	container
	@return		From_t object
************************************************************************
	@details
************************************************************************/
template <typename C>
From_t<C, typename C::value_type> From(const C& container)
{
	return From_t<C, typename C::value_type>(container);
}
template <typename C>
From_t<C, typename C::value_type> From(C&& container)
{
	return From_t<C, typename C::value_type>(std::move(container));
}


/////////////////////////////////////////////////////
} // end namespace Linq
} // end namespace GimLib
/////////////////////////////////////////////////////

#endif