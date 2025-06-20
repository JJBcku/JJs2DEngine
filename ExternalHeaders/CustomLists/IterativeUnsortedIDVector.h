#pragma once

#include "UnsortedIDVector.h"

template<class T>
class IterativeUnsortedIDVector : public UnsortedIDVector<T>
{
public:
	IterativeUnsortedIDVector(size_t reserve) : UnsortedIDVector<T>(reserve) {}

	IterativeUnsortedIDVector(const IterativeUnsortedIDVector<T>&) noexcept = default;
	IterativeUnsortedIDVector(IterativeUnsortedIDVector<T>&&) noexcept = default;

	IterativeUnsortedIDVector& operator=(const IterativeUnsortedIDVector<T>&) noexcept = default;
	IterativeUnsortedIDVector& operator=(IterativeUnsortedIDVector<T>&&) noexcept = default;

	std::optional<T>& GetObjectOptional(size_t index)
	{
		if (index >= UnsortedIDVector<T>::_list.size())
			throw std::runtime_error("IterativeUnsortedIDVector::GetObjectOptional Error: Program tried to read data from outside of the list!");

		return UnsortedIDVector<T>::_list[index].GetObjectOptional();
	}

	std::optional<T>& GetObjectOptional(IDObject<T> ID)
	{
		return UnsortedIDVector<T>::GetObjectOptional(ID);
	}

	const std::optional<T>& GetConstObjectOptional(size_t index) const
	{
		if (index >= UnsortedIDVector<T>::_list.size())
			throw std::runtime_error("IterativeUnsortedIDVector::GetObjectOptional Error: Program tried to read data from outside of the list!");

		return UnsortedIDVector<T>::_list[index].GetConstObjectOptional();
	}

	const std::optional<T>& GetConstObjectOptional(IDObject<T> ID) const
	{
		return UnsortedIDVector<T>::GetConstObjectOptional(ID);
	}

	std::optional<T> GetObjectOptionalCopy(size_t index) const
	{
		if (index >= UnsortedIDVector<T>::_list.size())
			throw std::runtime_error("IterativeUnsortedIDVector::GetObjectOptional Error: Program tried to read data from outside of the list!");

		return UnsortedIDVector<T>::_list[index].GetObjectOptionalCopy();
	}

	std::optional<T> GetObjectOptionalCopy(IDObject<T> ID) const
	{
		return UnsortedIDVector<T>::GetObjectOptionalCopy(ID);
	}

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

	const T& GetConstObject(size_t index) const
	{
		if (index >= UnsortedIDVector<T>::_list.size())
			throw std::runtime_error("IterativeUnsortedIDVector::GetConstObject Error: Program tried to read data from outside of the list!");

		return UnsortedIDVector<T>::_list[index].GetConstObject();
	}

	const T& GetConstObject(IDObject<T> ID) const
	{
		return UnsortedIDVector<T>::GetConstObject(ID);
	}

	T GetObjectCopy(size_t index) const
	{
		if (index >= UnsortedIDVector<T>::_list.size())
			throw std::runtime_error("IterativeUnsortedIDVector::GetObjectCopy Error: Program tried to read data from outside of the list!");

		return UnsortedIDVector<T>::_list[index].GetObjectCopy();
	}

	T GetObjectCopy(IDObject<T> ID) const
	{
		return UnsortedIDVector<T>::GetObjectCopy(ID);
	}
};
