#pragma once
#include <iterator>
#include <memory>
class List {
private:
	size_t len_;
	List *prev_;
	char *start_;
public:
	explicit List(size_t len, List *prev = nullptr) :
		len_(len),
		prev_(prev),
		start_((char*)std::malloc(sizeof(char) * len))
	{}
	~List() {
		std::free(start_);
		if (prev_ != nullptr)
			prev_->~List();
	}
	char* get_start() {
		return start_;
	}
};

class buffer {
private:
	List* now_;
	size_t len_;
	size_t cnt_of_busies_memes_;
public:
	explicit buffer(size_t len) :
		len_(len),
		now_(new (std::malloc(sizeof(List))) List(len)),
		cnt_of_busies_memes_(0)
	{}

	void upd_list() {
		now_ = new (std::malloc(sizeof(List))) List(len_, now_);
		cnt_of_busies_memes_ = 0;
	}
	char* get_free_pos() {
		return now_->get_start() + cnt_of_busies_memes_;
	}
	char* allocate(size_t n, size_t T_size) {
		size_t cnt_of_memes = n * T_size;
		if (cnt_of_busies_memes_ + cnt_of_memes > len_) {
			upd_list();
		}
		char* for_return = get_free_pos();
		cnt_of_busies_memes_ += cnt_of_memes;
		return for_return;
	}
	~buffer() {
		if (now_) {
			now_->~List();
			std::free(now_);
		}
	}
};

template < typename T >
class StackAllocator {
public:
	typedef T value_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;
	template< class U >
	struct rebind {
		typedef StackAllocator<U> other;
	};
	template <class U>
	friend class StackAllocator;
private:
	size_t len_;
	std::shared_ptr <buffer> buf_;
public:
	explicit StackAllocator(size_t len = 1e7) :
		len_(len),
		buf_(new (std::malloc(sizeof(buffer))) buffer(len_))
	{}
	template < typename U >
	StackAllocator(const StackAllocator < U >& other) {
		buf_ = other.buf_;
		len_ = other.len_;
	}
	T* allocate(size_t n) {
		return reinterpret_cast <T*> (buf_->allocate(n, sizeof(T)));
	}

	void deallocate(T* p, size_t n) {
		return;
	}

	~StackAllocator() {	}
};