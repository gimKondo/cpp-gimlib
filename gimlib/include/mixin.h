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

template <typename THIS_TYPE, template <class> class T, typename U>
class MixinBase< THIS_TYPE, Typelist<T<THIS_TYPE>, U > > : public T<THIS_TYPE>, public MixinBase<THIS_TYPE, U>
{
};

template <typename THIS_TYPE, template <class> class T>
class MixinBase< THIS_TYPE, Typelist< T<THIS_TYPE>, NullType> > : public T<THIS_TYPE>
{
};

/////////////////////////////////////////////////////
} // end namespace GimLib
/////////////////////////////////////////////////////

#endif