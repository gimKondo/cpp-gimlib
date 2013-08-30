#ifndef INCLUDED_GIMLIB_MIXIN
#define INCLUDED_GIMLIB_MIXIN

#include "TemplateList.h"

/////////////////////////////////////////////////////
namespace GimLib {
/////////////////////////////////////////////////////

/***********************************************************************/
/*!	@brief helper class for creating class that mixed in features
************************************************************************
	@details
		You can mix-in features, if you extend from this class
		that receiving feature class as type parameter.

		Mix-in classes can use members of mix-in class,
		if you cast this pointer to THIS_TYPE's pointer type.
		Yes, this is a kind of duck typing.

		Features are passed as TemplateList (refer to next sample).
		
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
		struct MixedinClass : public MixinBase<TEMPLATELIST_2(MixedinClass, Sum, Sub)>
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
		- now writing...
************************************************************************
	@author		gim_kondo
	@version	1.0
	@date		2012/10/14
************************************************************************/
template <typename FEATURE_LIST> class MixinBase;

template <typename THIS_TYPE, template <class> class T, typename U>
class MixinBase<TemplateList<THIS_TYPE, T, U > > : public T<THIS_TYPE>, public MixinBase<U> { };
// end of FEATURE_LIST
template <> class MixinBase<NullType> { };


/***********************************************************************/
/*!	@brief helper class for creating class that mixing in features
************************************************************************
	@details
		
	@code
		// feature class 1
		MIXIN_CLASS_DEFINE(Sum)
		{
			int sum()
			{
				// You can access mixed-in class's object by This()
				return This()->num1 + This()->num2;
			}
		};

		// mixed-in class
		struct MixedinClass : public MixinBase<TEMPLATELIST_1(MixedinClass, Sum)>
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
		- now writing...
************************************************************************
	@author		gim_kondo
	@version	1.0
	@date		2013/08/30
************************************************************************/
template <typename THIS_TYPE, typename MIXIN> struct MixinFeatureBase
{
	THIS_TYPE* This() { return static_cast<THIS_TYPE*>(this); }
	const THIS_TYPE* This() const { return static_cast<const THIS_TYPE*>(this); }
};

#define MIXIN_CLASS_DEFINE(classname)	\
	template <typename THIS_TYPE>		\
	struct classname : MixinFeatureBase<THIS_TYPE, classname<THIS_TYPE> >


/////////////////////////////////////////////////////
} // end namespace GimLib
/////////////////////////////////////////////////////

#endif