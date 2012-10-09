#ifndef INCLUDED_GIMLIB_EXTENSION_EXTENDABLE
#define INCLUDED_GIMLIB_EXTENSION_EXTENDABLE

/******************************************************************************/
/*! @file	extendable.h
    @brief	
*******************************************************************************
    @date       2012/10/10
    @author     gim_kondo
*******************************************************************************
    @par        History
******************************************************************************/

/******************************************************************************/
/*! @brief �g�����\�b�h�̎�����⏕����}�N��
	***************************************************************************
	@code
		//�N���X��`
		class Sample
		{
		public:
			Sample() {}
			EXTENABLE_CLASS_FORMULA(Sample)
		};

		//�g�����\�b�h��`
		EXTENSION_METHOD_BEGIN(Sample,foo)
			//�g�����\�b�h�̎�����
			void operator()(int param)	{
				//
			}
		EXTENSION_METHOD_END
	@endcode
	***************************************************************************
	@note	
******************************************************************************/
#define EXTENABLE_CLASS_FORMULA(CLASS)							\
	template <typename> struct ExtensionFunctor;				\
	template <typename METHOD> ExtensionFunctor<METHOD> Ext()	\
	{ return ExtensionFunctor<METHOD>(this); }
#define EXTENSION_METHOD_BEGIN(CLASS,METHOD)				\
	struct METHOD;											\
	template <> struct CLASS::ExtensionFunctor<METHOD> {	\
		ExtensionFunctor(CLASS* p) : this_(p) {}			\
		CLASS* this_;
#define EXTENSION_METHOD_END };


#endif