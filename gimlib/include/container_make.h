#ifndef INCLUDED_GIMLIB_CONTAINER_MAKE
#define INCLUDED_GIMLIB_CONTAINER_MAKE

#include <vector>
#include <list>
#include <deque>
#include <set>
#include <map>

/////////////////////////////////////////////////////
namespace GimLib	{
/////////////////////////////////////////////////////

/////////////////////////////////////////////////////
namespace Private {
/////////////////////////////////////////////////////
template <typename D, typename CONTAINER> struct make_push_back_container
{
	typedef typename CONTAINER::value_type VALUE;
	D& operator()(const VALUE& e)
	{
		c_.push_back(e);
		return static_cast<D&>(*this);
	}
	D& operator,(const VALUE& e)
	{
		c_.push_back(e);
		return static_cast<D&>(*this);
	}
	CONTAINER operator()() { return std::move(c_); }
	operator CONTAINER() { return std::move(c_); }
protected:
	CONTAINER c_;
	make_push_back_container() {}
	explicit make_push_back_container(const VALUE& e) { c_.push_back(e); }
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
		std::vector<int> vec0 = make_vector<int>(1)(2)(3);
		std::vector<int> vec1 = (make_vector<int>(),1,2,3);
	@endcode
************************************************************************/
template <typename T> struct make_vector
	: public Private::make_push_back_container<make_vector<T>, std::vector<T>>
{
	typedef Private::make_push_back_container<make_vector, std::vector<T>> Base;
	make_vector() { }
	explicit make_vector(const T& e) : Base(e) {}
};

/***********************************************************************/
/*!	@class make_list
	@brief make list that contained elements
************************************************************************
	@details
************************************************************************/
template <typename T> struct make_list
	: public Private::make_push_back_container<make_list<T>, std::list<T>>
{
	typedef Private::make_push_back_container<make_list, std::list<T>> Base;
	make_list() { }
	explicit make_list(const T& e) : Base(e) {}
};

/***********************************************************************/
/*!	@class make_deque
	@brief make deque that contained elements
************************************************************************
	@details
************************************************************************/
template <typename T> struct make_deque
	: public Private::make_push_back_container<make_deque<T>, std::deque<T>>
{
	typedef Private::make_push_back_container<make_deque, std::deque<T>> Base;
	make_deque() { }
	explicit make_deque(const T& e) : Base(e) {}
};

/***********************************************************************/
/*!	@class make_set
	@brief make set that contained elements
************************************************************************
	@details
************************************************************************/
template <typename T> struct make_set
{
	make_set& operator()(const T& e)
	{
		c_.insert(e);
		return *this;
	}
	make_set& operator,(const T& e)
	{
		c_.insert(e);
		return *this;
	}
	std::set<T> operator()() { return std::move(c_); }
	operator std::set<T>() { return std::move(c_); }
	make_set() { }
	explicit make_set(const T& e) { c_.insert(e); }
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