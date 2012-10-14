#ifndef INCLUDED_GIMLIB_EXTENSION_STL_EXTENSION
#define INCLUDED_GIMLIB_EXTENSION_STL_EXTENSION

/******************************************************************************/
/*! @file	stl_extension.h
    @brief	STL�R���e�i�̊g�����\�b�h���`
*******************************************************************************
    �t�@�C���̏ڍׂȐ���
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
/*! @brief �R���e�i���v�fval�������Ă��邩���ׂ�uhas�v
	***************************************************************************
	@param[in]	val	�v�f
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
/*! @brief map���L�[key�������Ă��邩���ׂ�uhasKey�v
	***************************************************************************
	@param[in]	key	�L�[
******************************************************************************/
template <typename KEY>
Private::HasKey_impl<KEY> hasKey(const KEY& key)
{
    return Private::HasKey_impl<KEY>(key);
}

/******************************************************************************/
/*! @brief �R���e�i��PRED�̏����𖞂����v�f�������Ă��邩���ׂ�uany�v
	***************************************************************************
	@param[in]	pred	���莮
******************************************************************************/
template <typename PRED>
Private::Any_impl<PRED> any(const PRED& pred)
{
    return Private::Any_impl<PRED>(pred);
}

/******************************************************************************/
/*! @brief �R���e�i�̑S�v�f��PRED�̏����𖞂��������ׂ�uall�v
	***************************************************************************
	@param[in]	pred	���莮
******************************************************************************/
template <typename PRED>
Private::All_impl<PRED> all(const PRED& pred)
{
    return Private::All_impl<PRED>(pred);
}

/******************************************************************************/
/*! @brief vector��A������uconcat�v
	***************************************************************************
	@param[in]	lhs �A������vector
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
/*! @brief	�R���e�i���v�fval�������Ă��邩���ׂ�uhas�v
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
/*! @brief	map���L�[key�������Ă��邩���ׂ�uhasKey�v�̎�����
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
/*! @brief	�R���e�i��PRED�̏����𖞂����v�f�������Ă��邩���ׂ�uany�v�̎���
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
/*! @brief �R���e�i�̑S�v�f��PRED�̏����𖞂��������ׂ�uall�v�̎���
	***************************************************************************
	@param[in]	pred	���莮
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
/*! @brief vector��A������uconcat�v�̎���
	***************************************************************************
	@param[in]	pred	���莮
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
/*! @brief �g�����\�b�h�p�̉��Z�q�I�[�o���C�h
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