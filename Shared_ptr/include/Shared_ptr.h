#pragma once

#include <functional>

template<class T>
class Shared_ptr {
	//alias declarations
	using del_t = std::function<void(T*)>;

public:// ctor && dtor
	Shared_ptr(T* p = nullptr, del_t del = [](T* p) {delete p; }):
		ptr(p),ref_count(new size_t(p!=nullptr)),deleter(del) {}
	~Shared_ptr() {
		decrementAndDestory();
	}

public:// copy operation
	Shared_ptr(const Shared_ptr& rhs)
		:ptr(rhs.ptr), ref_count(rhs.ref_count), deleter(rhs.deleter) {
		++*ref_count;
	}
	Shared_ptr& operator=(const Shared_ptr& rhs);

public:// move operation
	Shared_ptr(Shared_ptr&& rhs) noexcept
		:ptr(rhs.ptr), ref_count(rhs.ref_count), deleter(std::move(rhs.deleter)) {
		rhs.ptr = nullptr;
		rhs.ref_count = nullptr;
	}
	Shared_ptr& operator=(Shared_ptr&& rhs) noexcept;

public: // operator overload
	T& operator*() const noexcept { return *ptr; }
	T* operator->() const noexcept { return &(operator*()); }

public: // implicit cast for bool
	operator bool() const noexcept { return ptr ? true : false; }

public:// getter && setter
	T * get() const noexcept { return ptr; }
	size_t use_count() const noexcept{ return *ref_count; }
	bool unique() const noexcept { return *ref_count == 1; }
	void reset(T* p = nullptr,const del_t& d= [](T* p) {delete p; });

private:// aux interface
	void decrementAndDestory();
	void swap(Shared_ptr& rhs) noexcept;

private:// data member
	T * ptr;
	size_t* ref_count;
	del_t deleter;
};

template<class T>
inline Shared_ptr<T>& Shared_ptr<T>::operator=(const Shared_ptr<T>& rhs) {
	// increment first to ensure safty for self-assignment and avoid identity test
	++*rhs.ref_count;
	decrementAndDestory();
	ptr = rhs.ptr, ref_count = rhs.ref_count, deleter = rhs.deleter;
	return *this;
}

template<class T>
inline Shared_ptr<T> & Shared_ptr<T>::operator=(Shared_ptr<T>&& rhs) noexcept{
	swap(rhs);
	rhs.decrementAndDestory();
	return *this;
}

template<class T>
inline void Shared_ptr<T>::reset(T* p, const del_t& d){
	if (ptr != p) {
		decrementAndDestory();
		ptr = p;
		ref_count = new std::size_t(p != nullptr);
	}
	deleter = d;
}

template<class T>
inline void Shared_ptr<T>::decrementAndDestory(){
	if (ptr && --*ref_count == 0) {
		delete ref_count;
		deleter(ptr);
	}
	else if (!ptr)
		delete ref_count;
	ptr = nullptr;
	ref_count = nullptr;
}

template<class T>
inline void Shared_ptr<T>::swap(Shared_ptr & rhs) noexcept{
	using std::swap;
	swap(ptr, rhs.ptr);
	swap(ref_count, rhs.ref_count);
	swap(deleter, rhs.deleter);
}
