#ifndef INCLUDED_GIMLIB_MIXIN
#define INCLUDED_GIMLIB_MIXIN

#include "TypeList.h"

/////////////////////////////////////////////////////
namespace GimLib {
/////////////////////////////////////////////////////

/***********************************************************************/
/*!	@brief helper class for creating class that mixed in features
************************************************************************
	@par Method
		 Name                   | Summary
		------------------------|-------------------------------
		                        | 
************************************************************************
	@detail
		You can mix-in features, if you extend from this class
		that receiving feature class as type parameter.
		Features are passed as TypeList.
************************************************************************
	@note
		now writing...
************************************************************************
	@author		gim_kondo
	@version	1.0
	@date		2012/10/14
************************************************************************/
template <typename THIS_TYPE, typename FEATURE_LIST> class MixinBase;

template <typename THIS_TYPE, typename T, typename U>
class MixinBase< THIS_TYPE, Typelist<T, U> > : public T, public MixinBase<THIS_TYPE, U>
{
};

template <typename THIS_TYPE, typename T>
class MixinBase< THIS_TYPE, Typelist< T, NullType> > : public T
{
};

/////////////////////////////////////////////////////
} // end namespace GimLib
/////////////////////////////////////////////////////

#endif