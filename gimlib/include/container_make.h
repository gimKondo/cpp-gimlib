#ifndef INCLUDED_GIMLIB_CONTAINER_MAKE
#define INCLUDED_GIMLIB_CONTAINER_MAKE

#include <vector>
#include <list>
#include <deque>
#include <set>
#include <map>
#include <memory>

/////////////////////////////////////////////////////
namespace GimLib	{
/////////////////////////////////////////////////////

/***********************************************************************/
/*!	@class ElementTypePolicyRawType
	@brief raw element policy
************************************************************************
	@details
        
************************************************************************/
template <typename T> struct ElementTypePolicyRawType
{
    typedef T Type;
};
/***********************************************************************/
/*!	@class ElementTypePolicySharedPtr
	@brief shared_ptr element policy
************************************************************************
	@details
        
************************************************************************/
template <typename T> struct ElementTypePolicySharedPtr
{
    typedef std::shared_ptr<T> Type;
};

/***********************************************************************/
/*!	@class ElementCreatePolicyConstructor
	@brief create policy that create by constructor
************************************************************************
	@details
        
************************************************************************/
template <typename T> struct ElementCreatePolicyConstructor
{
    template <class... ARGS>
    static T Create(ARGS... args) { return T(args...); }
};
/***********************************************************************/
/*!	@class ElementCreatePolicyMakeShared
	@brief create policy that create by make_shared
************************************************************************
	@details
        
************************************************************************/
template <typename T> struct ElementCreatePolicyMakeShared
{
    template <class... ARGS>
    static std::shared_ptr<T> Create(ARGS... args) { return std::make_shared<T>(args...); }
};

/////////////////////////////////////////////////////
namespace Private {
/////////////////////////////////////////////////////
template < typename DERIVED
         , typename CONTAINER
         , typename VALUE_TYPE
         , typename ELM_TYPE
         , typename CREATOR
         >
         struct make_push_back_container
{
    typedef ELM_TYPE ELEMENT;
	DERIVED& operator()(const ELM_TYPE& e)
	{
		c_.push_back(e);
		return static_cast<DERIVED&>(*this);
	}
    template <class... ARGS>
	DERIVED& operator()(ARGS... args)
	{
		c_.push_back(CREATOR::Create(args...));
		return static_cast<DERIVED&>(*this);
	}
	CONTAINER operator--(int) { return std::move(c_); }
protected:
	CONTAINER c_;
	explicit make_push_back_container(const ELM_TYPE& e) { c_.push_back(e); }
    template <class... ARGS>
	make_push_back_container(ARGS... args) { c_.push_back(CREATOR::Create(args...)); }
};
/////////////////////////////////////////////////////
} // end namespace Private
/////////////////////////////////////////////////////

/***********************************************************************/
/*!	@class make_vector
	@brief make vector that contained elements
************************************************************************
	@details
		you can get vector contained 1, 2 and 3, by following code.
	@code
		std::vector<int> vec0 = make_vector<int>(1)(2)(3)--;
	@endcode
************************************************************************/
template < typename T
         , template <typename T> class ElementTypePolicy = ElementTypePolicyRawType
         , template <typename T> class ElementCreatePolicy = ElementCreatePolicyConstructor
         >
struct make_vector
	: public Private::make_push_back_container
        < make_vector<T, ElementTypePolicy, ElementCreatePolicy>
        , std::vector<typename ElementTypePolicy<T>::Type>
        , T
        , typename ElementTypePolicy<T>::Type
        , ElementCreatePolicy<T>
        >
{
	typedef Private::make_push_back_container
        < make_vector<T, ElementTypePolicy, ElementCreatePolicy>
        , std::vector<typename ElementTypePolicy<T>::Type>
        , T
        , typename ElementTypePolicy<T>::Type
        , ElementCreatePolicy<T>
        >
        Base;
	explicit make_vector(const ELEMENT& e) : Base(e) {}
    template <class... ARGS>
	make_vector(ARGS... args) : Base(args...) { }
};

/***********************************************************************/
/*!	@class make_list
	@brief make list that contained elements
************************************************************************
	@details
************************************************************************/
template < typename T
         , template <typename T> class ElementTypePolicy = ElementTypePolicyRawType
         , template <typename T> class ElementCreatePolicy = ElementCreatePolicyConstructor
         >
struct make_list
	: public Private::make_push_back_container
        < make_list<T, ElementTypePolicy, ElementCreatePolicy>
        , std::list<typename ElementTypePolicy<T>::Type>
        , T
        , typename ElementTypePolicy<T>::Type
        , ElementCreatePolicy<T>
        >
{
	typedef Private::make_push_back_container
        < make_list<T, ElementTypePolicy, ElementCreatePolicy>
		, std::list<typename ElementTypePolicy<T>::Type>
        , T
        , typename ElementTypePolicy<T>::Type
        , ElementCreatePolicy<T>
        >
        Base;
	explicit make_list(const ELEMENT& e) : Base(e) {}
    template <class... ARGS>
	make_list(ARGS... args) : Base(args...) { }
};

/***********************************************************************/
/*!	@class make_deque
	@brief make deque that contained elements
************************************************************************
	@details
************************************************************************/
template < typename T
         , template <typename T> class ElementTypePolicy = ElementTypePolicyRawType
         , template <typename T> class ElementCreatePolicy = ElementCreatePolicyConstructor
         >
struct make_deque
	: public Private::make_push_back_container
        < make_deque<T, ElementTypePolicy, ElementCreatePolicy>
        , std::deque<typename ElementTypePolicy<T>::Type>
        , T
        , typename ElementTypePolicy<T>::Type
        , ElementCreatePolicy<T>
        >
{
	typedef Private::make_push_back_container
        < make_deque<T, ElementTypePolicy, ElementCreatePolicy>
		, std::deque<typename ElementTypePolicy<T>::Type>
        , T
        , typename ElementTypePolicy<T>::Type
        , ElementCreatePolicy<T>
        >
        Base;
	explicit make_deque(const ELEMENT& e) : Base(e) {}
    template <class... ARGS>
	make_deque(ARGS... args) : Base(args...) { }
};

/***********************************************************************/
/*!	@class make_set
	@brief make set that contained elements
************************************************************************
	@details
************************************************************************/
template < typename T
         , template <typename T> class ElementTypePolicy = ElementTypePolicyRawType
         , template <typename T> class ElementCreatePolicy = ElementCreatePolicyConstructor
         >
struct make_set
{
    typedef typename ElementTypePolicy<T>::Type ELEMENT;
	make_set& operator()(const ELEMENT& e)
	{
		c_.insert(e);
		return *this;
	}
    template <class... ARGS>
	make_set& operator()(ARGS... args)
	{
		c_.insert(CREATOR::Create(args...));
		return *this;
	}
	std::set<T> operator--(int) { return std::move(c_); }
	explicit make_set(const ELEMENT& e) { c_.insert(e); }
    template <class... ARGS>
	make_set(ARGS... args) { c_.insert(CREATOR::Create(args...)); }
private:
	std::set<T> c_;
};

/***********************************************************************/
/*!	@class make_map
	@brief make map that contained elements
************************************************************************
	@details
************************************************************************/
template <typename KEY, typename VAL> struct make_map
{
	make_map& operator()(const KEY& key, const VAL& val)
	{
		c_[key] = val;
		return *this;
	}
	std::map<KEY, VAL> operator()() { return std::move(c_); }
	operator std::map<KEY, VAL>() { return std::move(c_); }
	make_map() { }
	make_map(const KEY& key, const VAL& val) { c_[key] = val; }
private:
	std::map<KEY, VAL> c_;
};

/////////////////////////////////////////////////////
} // end namespace GimLib
/////////////////////////////////////////////////////

#endif
