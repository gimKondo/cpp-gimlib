#ifndef INCLUDED_GIMLIB_LINQ
#define INCLUDED_GIMLIB_LINQ

#include <functional>
#include <vector>

/////////////////////////////////////////////////////
namespace GimLib	{
namespace Linq		{
/////////////////////////////////////////////////////

template <typename ELM>
class Where_t
{
public:
	typedef std::function<bool(const ELM&)> Cond;
	Where_t(const Cond& cond) : cond_(cond) {}
	Cond cond_;
	bool operator()(const ELM& elm) { return cond_(elm); }
};

template <typename ELM>
Where_t<ELM> Where(const typename Where_t<ELM>::Cond& cond)
{
	return Where_t<ELM>(cond);
}

template <typename RType, typename OType>
class Select_t
{
public:
	typedef std::function<RType(const OType&)> Conv;
	Select_t(const Conv& conv) : conv_(conv) {}
	Conv conv_;
	RType operator()(const OType& o) { return conv_(o); }
};

template <typename RType, typename OType>
Select_t<RType, OType> Select(const typename Select_t<RType, OType>::Conv& conv)
{
	return Select_t<RType, OType>(conv);
}

/***********************************************************************/
/*!	@brief 
************************************************************************
	@details
		You can treat several containers by special style
		that is similar to LINQ.
		
	@code
		std::vector<int> nums = {1, 2, 3, 4, 5, 6};
		auto arr = From_t<int>(nums)
					| Where<int>([](int n) { return n % 2 == 0; })
					| To_array();
		// Now, "arr" contains 2, 4 and 6.
	@endcode
************************************************************************
	@note
		- now writing...
************************************************************************
	@author		gim_kondo
	@version	1.0
	@date		2013/5/8
************************************************************************/
template <typename ELM>
class From_t
{
public:
	typedef std::vector<ELM> ARRAY;
	//
	ARRAY& arr_;
	std::vector< Where_t<ELM> > wheres_;

	//
	explicit From_t(ARRAY& arr) : arr_(arr) {}

	//
	From_t& operator|( const Where_t<ELM>& whr )
	{
		wheres_.push_back(whr);
		return *this;
	}

	template <typename RType>
	std::vector<RType> operator|( Select_t<RType, ELM>& sel )
	{
		typedef std::vector<RType> ReturnArray;
		ReturnArray ret;
		for(ARRAY::iterator itr = arr_.begin(); itr != arr_.end(); ++itr)
		{
			if (wheres_[0](*itr))
			{
				ret.push_back( sel(*itr) );
			}
		}
		return ret;
	}

	//
	ARRAY toArray()
	{
		ARRAY ret;
		for(ARRAY::iterator itr = arr_.begin(); itr != arr_.end(); ++itr)
		{
			if (wheres_[0](*itr))
			{
				ret.push_back(*itr);
			}
		}
		return ret;
	}
};

template <typename ELM>
From_t<ELM> From(typename From_t<ELM>::ARRAY& arr)
{
	return From_t<ELM>(arr);
}


/////////////////////////////////////////////////////
} // end namespace Linq
} // end namespace GimLib
/////////////////////////////////////////////////////

#endif