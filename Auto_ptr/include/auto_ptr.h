#pragma once

// a tiny smart-pointer:auto_ptr
template<class T>
class auto_ptr {
// friend declarations
// make all auto_ptrs be friends
template<class U>
friend class auto_ptr<U>;

public:// ctor && dtor
	explicit auto_ptr(T* p = nullptr);
	~auto_ptr();

public:// copy ctor && assignment operator(template)
	template<class U>
	auto_ptr(auto_ptr<U>& rhs);
	template<class U>
	auto_ptr& operator=(auto_ptr<U>& rhs);

public:// dererfence interface
	T& operator*() const;
	T* operator->() const;

public:// interface for raw pointer
	T* get() const;
	T* release();
	void resset(T* p = nullptr);

private:
	T * pointee;
};

