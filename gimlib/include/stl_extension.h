#ifndef INCLUDED_GIMLIB_EXTENSION_STL_EXTENSION
#define INCLUDED_GIMLIB_EXTENSION_STL_EXTENSION

/******************************************************************************/
/*! @file	stl_extension.h
    @brief	STLコンテナの拡張メソッドを定義
*******************************************************************************
    ファイルの詳細な説明
*******************************************************************************
    @date       2012/7/31
    @author     gim_kondo
*******************************************************************************
    @par        History
******************************************************************************/

#include <functional>
#include <algorithm>
#include <list>
#include <vector>
#include <set>
#include <map>

///////////////////////////////////////
namespace GimLib {
namespace StlExt {
///////////////////////////////////////

namespace Private {
template <typename VAL	>	class Has_impl		;
template <typename KEY	>	class HasKey_impl	;
template <typename PRED	>	class Any_impl		;
template <typename PRED	>	class All_impl		;
template <typename VAL	>	class Concat_impl	;
}

/******************************************************************************/
/*! @brief コンテナが要素valを持っているか調べる「has」
	***************************************************************************
	@param[in]	val	要素
	***************************************************************************
	@code
		std::vector<int> nums;
		// omitted
		if (nums|has(3)) { cout << "nums has 3" << endl; }
	@endcode
	***************************************************************************
	@note	supported list, vector and set
******************************************************************************/
template <typename VAL>
Private::Has_impl<VAL> has(const VAL& val)
{
    return Private::Has_impl<VAL>(val);
}

/******************************************************************************/
/*! @brief mapがキーkeyを持っているか調べる「hasKey」
	***************************************************************************
	@param[in]	key	キー
******************************************************************************/
template <typename KEY>
Private::HasKey_impl<KEY> hasKey(const KEY& key)
{
    return Private::HasKey_impl<KEY>(key);
}

/******************************************************************************/
/*! @brief コンテナがPREDの条件を満たす要素を持っているか調べる「any」
	***************************************************************************
	@param[in]	pred	判定式
******************************************************************************/
template <typename PRED>
Private::Any_impl<PRED> any(const PRED& pred)
{
    return Private::Any_impl<PRED>(pred);
}

/******************************************************************************/
/*! @brief コンテナの全要素がPREDの条件を満たすか調べる「all」
	***************************************************************************
	@param[in]	pred	判定式
******************************************************************************/
template <typename PRED>
Private::All_impl<PRED> all(const PRED& pred)
{
    return Private::All_impl<PRED>(pred);
}

/******************************************************************************/
/*! @brief vectorを連結する「concat」
	***************************************************************************
	@param[in]	lhs 連結するvector
******************************************************************************/
template <typename VAL>
Private::Concat_impl<VAL> concat(const std::vector<VAL>& lhs)
{
    return Private::Concat_impl<VAL>(lhs);
}


///////////////////////////////////////
namespace Private {
///////////////////////////////////////

/******************************************************************************/
/*! @brief	コンテナが要素valを持っているか調べる「has」
	@note	supported list, vector and set
******************************************************************************/
template <typename VAL> class Has_impl
{
	VAL val_;
public:
	typedef bool result_type;
	Has_impl(const VAL& val) : val_(val) {}

	template <typename IVAL>
	bool operator()(const std::list<IVAL>& src) const
	{
		return std::find(src.begin(), src.end(), val_) != src.end();
	}
	template <typename IVAL>
	bool operator()(const std::vector<IVAL>& src) const
	{
		return std::find(src.begin(), src.end(), val_) != src.end();
	}
	template <typename IVAL>
	bool operator()(const std::set<IVAL>& src) const
	{
		return src.find(val_) != src.end();
	}
};

/******************************************************************************/
/*! @brief	mapがキーkeyを持っているか調べる「hasKey」の実装部
******************************************************************************/
template <typename KEY> class HasKey_impl
{
	KEY key_;
public:
	typedef bool result_type;
	HasKey_impl(const KEY& key) : key_(key) {}
	template <typename IKEY, typename IVAL>
	bool operator()(const std::map<IKEY,IVAL>& src) const
	{
		return src.find(key_) != src.end();
	}
};

/******************************************************************************/
/*! @brief	コンテナがPREDの条件を満たす要素を持っているか調べる「any」の実装
******************************************************************************/
template <typename PRED> class Any_impl
{
	PRED pred_;
public:
	typedef bool result_type;
	Any_impl(const PRED& pred) : pred_(pred) {}
	template <typename CONTAINER> bool operator()(const CONTAINER& src) const
	{
		return std::any_of(src.begin(), src.end(), pred_);
	}
};

/******************************************************************************/
/*! @brief コンテナの全要素がPREDの条件を満たすか調べる「all」の実装
	***************************************************************************
	@param[in]	pred	判定式
******************************************************************************/
template <typename PRED> class All_impl
{
	PRED pred_;
public:
	typedef bool result_type;
	All_impl(const PRED& pred) : pred_(pred) {}
	template <typename CONTAINER> bool operator()(const CONTAINER& src) const
	{
		return std::all_of(src.begin(), src.end(), pred_);
	}
};

/******************************************************************************/
/*! @brief vectorを連結する「concat」の実装
	***************************************************************************
	@param[in]	pred	判定式
******************************************************************************/
template <typename VAL>	class Concat_impl
{
	const std::vector<VAL>& lhs_;
public:
	typedef void result_type;
	Concat_impl(const std::vector<VAL>& lhs) : lhs_(lhs) {}
	template <typename IVAL> void operator()(std::vector<IVAL>& rhs) const
	{
		rhs.insert(rhs.end(), lhs_.begin(), lhs_.end());
	}
};


/******************************************************************************/
/*! @brief 拡張メソッド用の演算子オーバライド
******************************************************************************/
//list
template <typename VAL, typename FUNC>
inline typename std::result_of<FUNC(const std::list<VAL>&)>::type
operator| (const std::list<VAL>& src, FUNC func)
{
	return func(src);
}
template <typename VAL, typename FUNC>
inline typename std::result_of<FUNC(const std::list<VAL>&)>::type
operator| (std::list<VAL>& src, FUNC func)
{
	return func(src);
}
//vector
template <typename VAL, typename FUNC>
inline typename std::result_of<FUNC(const std::vector<VAL>&)>::type
operator| (const std::vector<VAL>& src, FUNC func)
{
	return func(src);
}
template <typename VAL, typename FUNC>
inline typename std::result_of<FUNC(const std::vector<VAL>&)>::type
operator| (std::vector<VAL>& src, FUNC func)
{
	return func(src);
}
//set
template <typename VAL, typename FUNC>
inline typename std::result_of<FUNC(const std::set<VAL>&)>::type
operator| (const std::set<VAL>& src, FUNC func)
{
	return func(src);
}
template <typename VAL, typename FUNC>
inline typename std::result_of<FUNC(const std::set<VAL>&)>::type
operator| (std::set<VAL>& src, FUNC func)
{
	return func(src);
}
//map
template <typename KEY, typename VAL, typename FUNC>
inline typename std::result_of<FUNC(const std::map<KEY,VAL>&)>::type
operator| (const std::map<KEY,VAL>& src, FUNC func)
{
	return func(src);
}
template <typename KEY, typename VAL, typename FUNC>
inline typename std::result_of<FUNC(const std::map<KEY,VAL>&)>::type
operator| (std::map<KEY,VAL>& src, FUNC func)
{
	return func(src);
}

///////////////////////////////////////
} // end namespace Private
///////////////////////////////////////

///////////////////////////////////////
} // end namespace StlExt
} // end namespace GimLib
///////////////////////////////////////



#endif