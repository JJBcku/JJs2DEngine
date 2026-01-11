#pragma once

#include "UnsortedIDVector.h"

template<class T>
class IterativeUnsortedIDVector : public UnsortedIDVector<T>
{
public:
	IterativeUnsortedIDVector(size_t reserve) noexcept(std::is_nothrow_constructible_v<UnsortedIDVector<T>, size_t>) : UnsortedIDVector<T>(reserve) {}

	IterativeUnsortedIDVector(const IterativeUnsortedIDVector<T>&) noexcept = delete;
	IterativeUnsortedIDVector(IterativeUnsortedIDVector<T>&& rhs) noexcept(std::is_nothrow_move_constructible_v<UnsortedIDVector<T>>) : UnsortedIDVector<T>(std::move(rhs)) {};

	IterativeUnsortedIDVector& operator=(const IterativeUnsortedIDVector<T>&) noexcept = delete;
	IterativeUnsortedIDVector& operator=(IterativeUnsortedIDVector<T>&&) noexcept = default;

	T& GetObject(size_t index)
	{
		if (index >= UnsortedIDVector<T>::_list.size())
			throw std::runtime_error("IterativeUnsortedIDVector::GetObject Error: Program tried to read data from outside of the list!");

		return UnsortedIDVector<T>::_list[index].GetObject();
	}

	T& GetObject(IDObject<T> ID)
	{
		return UnsortedIDVector<T>::GetObject(ID);
	}

	const T& GetObject(size_t index) const
	{
		if (index >= UnsortedIDVector<T>::_list.size())
			throw std::runtime_error("IterativeUnsortedIDVector::GetObject Const Error: Program tried to read data from outside of the list!");

		return UnsortedIDVector<T>::_list[index].GetObject();
	}

	const T& GetObject(IDObject<T> ID) const
	{
		return UnsortedIDVector<T>::GetObject(ID);
	}
};
