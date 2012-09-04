#ifndef INCLUDED_GIMLIB_STL_EXTENSION
#define INCLUDED_GIMLIB_EXTENSION_STL_EXTENSION

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
template <typename VAL> class Has_impl;
}

template <typename VAL>
Private::Has_impl<VAL> has(const VAL& val)
{
    return Private::Has_impl<VAL>(val);
}

///////////////////////////////////////
namespace Private {
///////////////////////////////////////

//***********************************************
//
//***********************************************
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
};
//! @brief �g�����\�b�h�p�̉��Z�q�I�[�o���C�h

//***********************************************
// �g�����\�b�h�p�̉��Z�q�I�[�o���C�h
//***********************************************
template <typename VAL, typename FUNC>
inline typename std::result_of<FUNC(const std::list<VAL>&)>::type
operator| (const std::list<VAL>& src, FUNC func)
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