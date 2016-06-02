#include <memory>
#include <vector>
#include "thread_singleton.hpp"
#include <assert.h>
#include <array>


namespace krys
<%

using std::unique_ptr;
using std::make_unique;


template<typename T>
struct recycle_deleter;

template<typename T>
class object_pool;

template<typename T>
unique_ptr<T, recycle_deleter<T>> make_pool_object ();


template<typename T>
using recycle_ptr = unique_ptr<T, recycle_deleter<T>>;



template<typename T>
class object_pool
{
public:
	friend class thread_singleton<object_pool<T>>;
	friend struct recycle_deleter<T>;
	friend unique_ptr<T, recycle_deleter<T>> make_pool_object<T> ();
public:

	bool empty () const noexcept
	{
		return pool_data_.empty ();
	}

	auto size () const noexcept
	{
		return pool_data_.size ();
	}

private: /// construction
	template<typename deleter>
	auto take () noexcept
   	{
		unique_ptr<T, deleter> ptr {pool_data_.back ().release ()};
		pool_data_.pop_back ();
		return ptr;
	}

	explicit object_pool (size_t size = 0) noexcept:pool_data_ (size)
	{
		static_assert (!std::is_array<T>::value, "T cannot be an array");
	}
	compl object_pool () = default;

	auto recycle (T* obj) noexcept
	{
		assert (obj);
		assert (pool_data_.size () <= max_spare_);

		if (pool_data_.size () == max_spare_)
		{
			delete obj;
		}
		else
		{
			pool_data_.emplace_back (obj);
		}
	}

	void set_max_spare (uint32_t val)
	{
		max_spare_ = val;
		if (pool_data_.size () > val)
		{
			pool_data_.resize (val);
			pool_data_.shrink_to_fit ();
		}
	}

private: /// data
	uint32_t max_spare_ = static_cast<uint32_t> (-1);
	std::vector<unique_ptr<T>> pool_data_;
};

template<typename T>
struct recycle_deleter
{
	void operator () (T* ptr)
	{
		thread_singleton<object_pool<T>>::instance ().recycle (ptr);
	}
};


template<typename T>
inline recycle_ptr<T> make_pool_object ()
{
	auto& pool = thread_singleton<object_pool<T>>::instance ();
	if (pool.empty ())
	{
		return unique_ptr<T, recycle_deleter<T>> (make_unique<T>().release ());
	}
	else
	{
		return pool.template take<recycle_deleter<T>> ();
	}
}

template<typename T>
inline void set_max_free_object (uint32_t max)
{
	auto& pool = thread_singleton<object_pool<T>>::instance ();
	pool.set_max_spare (max);
}


%>
