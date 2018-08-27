#pragma once

class Delete {
public:
	template <class T> 
	void operator()(T* ptr) const { delete ptr; }
};

template<class T,class D=Delete>
class Unique_ptr {
public:// ctor && dtor
	Unique_ptr(T* p=nullptr,const D& d=D()):ptr(p),deleter(d) {}
	~Unique_ptr() { deleter(ptr); }

public:// uncopyable
	Unique_ptr(const Unique_ptr&) = delete;
	Unique_ptr& operator=(const Unique_ptr&) = delete;

public:// move-only
	Unique_ptr(Unique_ptr&& rhs) noexcept :ptr(rhs.ptr), deleter(std::move(rhs.deleter)) { rhs.ptr = nullptr; }
	Unique_ptr& operator=(Unique_ptr&& rhs) noexcept;

public:// operator= for nullptr
	Unique_ptr & operator=(std::nullptr_t) noexcept;

public:// implicit cast interface to bool
	operator bool() const { return ptr?true:false; }

public:// operator overload:* ->
	T& operator*() const { return *ptr; }
	T* operator->() const { return &(operator*()); }

public:// getter and setter
	T* get() const noexcept { return ptr; }
	D& get_deleter() noexcept { return deleter; }
	const D& get_deleter() const noexcept { return deleter; }
	void reset(T* p = nullptr) noexcept;
	T* release() noexcept;

public:// swap for operator=
	void swap(Unique_ptr&) noexcept;

private:// data member
	T* ptr;
	D deleter;
};

template<class T, class D>
inline Unique_ptr<T,D>& Unique_ptr<T, D>::operator=(Unique_ptr<T,D>&& rhs) noexcept{
	if (ptr != rhs.ptr) {
		reset();
		::swap(*this, rhs);
	}
	return *this;
}

template<class T, class D>
inline Unique_ptr<T,D>& Unique_ptr<T, D>::operator=(std::nullptr_t) noexcept{
	reset();
	return *this;
}

template<class T, class D>
inline void Unique_ptr<T, D>::reset(T* p) noexcept{
	if (ptr != p) {
		deleter(ptr);
		ptr = p;
	}
}

template<class T, class D>
inline T* Unique_ptr<T, D>::release() noexcept{
	T* res = ptr;
	ptr = nullptr;
	return res;
}

template<class T, class D>
inline void Unique_ptr<T, D>::swap(Unique_ptr<T,D>& rhs) noexcept{
	using std::swap;
	swap(ptr, rhs.ptr);
	swap(deleter, rhs.deleter);
}
