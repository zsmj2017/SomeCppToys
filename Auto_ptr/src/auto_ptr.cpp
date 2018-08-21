#include "auto_ptr.h"

template <class T>
inline auto_ptr<T>::auto_ptr(T* p):pointee(p) {}

template<class T>
auto_ptr<T>::~auto_ptr(){
	delete pointee;
}

template <class T>
template <class U>
inline auto_ptr<T>::auto_ptr(auto_ptr<U>& rhs) :pointee(rhs.release()) {}

template<class T>
template<class U>
auto_ptr<T>& auto_ptr<T>::operator=(auto_ptr<U>& rhs){
	if (this != &rhs)
		reset(rhs.release());
	return *this;
}

template<class T>
T& auto_ptr<T>::operator*() const {
	return *pointee;
}

template<class T>
T* auto_ptr<T>::operator->() const {
	return pointee;
}

template<class T>
T * auto_ptr<T>::get() const{
	return pointee;
}

template<class T>
T * auto_ptr<T>::release(){
	T* oldPointee = pointee;
	delete pointee;
	return oldPointee;
}

template<class T>
void auto_ptr<T>::resset(T* p){
	if (pointee != p) {
		delete pointee;
		pointee = p;
	}
}
