#ifndef INCLUDED_GIMLIB_EXTEND_METHOD
#define INCLUDED_GIMLIB_EXTEND_METHOD


/***********************************************************************/
/*!	@class extender_base
	@brief base class template for extend method functor
************************************************************************
	@details
		This class template help to create extend method.
		It's not for using 
	@code
		Hoge { int n_; };

		// create const method
		EXTENDER_START(Hoge, GetSquare, int)
			EXTEND_METHOD0(const)
			{
				return this_.n_ * this_.n_;
			}
		EXTENDER_END

		// create non const method
		EXTENDER_START(Hoge, Square, void)
			EXTEND_METHOD0(EXTENDER_NON_CONST)
			{
				this_.n_ *= this_.n_;
			}
		EXTENDER_END

		// create non const method passed a parameter
		EXTENDER_START(Hoge, Add, void)
			EXTEND_METHOD1(int, arg, EXTENDER_NON_CONST)
			// EXTEND_METHOD1(int, arg, ) // you can omit last parameter that means not const
			{
				this_.n_ += arg;
			}
		EXTENDER_END
	@endcode
************************************************************************/
template <typename TRG_TYPE, typename EXTENDER, typename RET_TYPE> struct extender_base {
	typedef TRG_TYPE target_type;
	typedef RET_TYPE result_type;
	friend RET_TYPE operator->*(TRG_TYPE& trg, EXTENDER&& ext)
	{ return ext(trg); }
	friend RET_TYPE operator->*(const TRG_TYPE& trg, EXTENDER&& ext)
	{ return ext(trg); }
};


#define EXTENDER_START(TRG_TYPE, EXTENDER, RET_TYPE)						\
	struct EXTENDER : public extender_base<TRG_TYPE, EXTENDER, RET_TYPE> {	\
		typedef EXTENDER this_type;

#define EXTEND_METHOD0(IS_CONST)						\
	result_type operator()(IS_CONST target_type& this_)

#define EXTEND_METHOD1(ARG_TYPE1, arg1, IS_CONST)		\
	this_type(ARG_TYPE1 arg1##_)						\
		: arg1(arg1##_) {}								\
	ARG_TYPE1 arg1;										\
	result_type operator()(IS_CONST target_type& this_)

#define EXTEND_METHOD2(ARG_TYPE1, arg1, ARG_TYPE2, arg2, IS_CONST)	\
	this_type(ARG_TYPE1 arg1##_, ARG_TYPE2 arg2##_)					\
		: arg1(arg1##_), arg2(arg2##_) {}							\
	ARG_TYPE1 arg1;													\
	ARG_TYPE2 arg2;													\
	result_type operator()(IS_CONST target_type& this_)

#define EXTEND_METHOD3(ARG_TYPE1, arg1, ARG_TYPE2, arg2, ARG_TYPE3, arg3, IS_CONST)	\
	this_type(ARG_TYPE1 arg1##_, ARG_TYPE2 arg2##_, ARG_TYPE3, arg3##_)				\
		: arg1(arg1##_), arg2(arg2##_), arg3(arg3##_) {}							\
	ARG_TYPE1 arg1;																	\
	ARG_TYPE2 arg2;																	\
	ARG_TYPE3 arg3;																	\
	result_type operator()(IS_CONST target_type& this_)

#define EXTENDER_END	};

#define EXTENDER_NON_CONST

#endif
