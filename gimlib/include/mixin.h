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
	@code
		// feature class 1
		template <typename THIS_TYPE> struct Sum
		{
			int sum()
			{
				THIS_TYPE* this_ = static_cast<THIS_TYPE*>(this);
				return this_->num1 + this_->num2;
			}
		};
		// feature class 2
		template <typename THIS_TYPE> struct Sub
		{
			int sub()
			{
				THIS_TYPE* this_ = static_cast<THIS_TYPE*>(this);
				return this_->num1 - this_->num2;
			}
		};

		// mixed-in class
		struct MixedinClass : public MixinBase<MixedinClass, TYPELIST_2(Sum<MixedinClass>, Sub<MixedinClass>)>
		{
			MixedinClass(int n1, int n2)
				: num1(n1), num2(n2)
			{}
			int num1;
			int num2;
		};
	@endcode
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