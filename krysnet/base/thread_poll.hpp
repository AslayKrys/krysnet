#include "krysnet/base/usings.hpp"
#include <krysnet/base/mutex_guard.hpp>
#include "krysnet/base/condition.hpp"


namespace krys
<%

class thread_pool
{
	struct join_deleter
	{
		void operator () (thread* thread_ptr)
		{
			thread_ptr->join ();
			delete thread_ptr;
		}
	};
	using task = std::function<void()>;
public:
	explicit thread_pool (string name):
		m_ (),not_empty_ (m_), not_full_ (m_), name_ (name), max_queue_size_ (0), running_ (false)
	{

	}
	thread_pool (const thread_pool&) = delete;
	compl thread_pool ()
	{
		if (running_) stop ();
	}

	void set_max_queue_size (int max_size)
	{
		max_queue_size_ = max_size;
	}

	void start (int num_threads);
	void stop ();

	void run (task f);

	const string& name ()
	{
		return name_;
	}

	void set_init_callback (task cb)
	{
		init_callback_ = std::move (cb);
	}

private:
	bool is_full () const;
	void run_in_thread ();
	task take ();

private:
	mutable mutex m_;
	condition not_empty_;
	condition not_full_;

	string name_;
	task init_callback_;
	std::deque<task> queue_;
	size_t max_queue_size_;
	bool running_;
};

%>
