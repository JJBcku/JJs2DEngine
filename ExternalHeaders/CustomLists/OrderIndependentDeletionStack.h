#pragma once

#include "CommonVectorObject.h"

#include <vector>
#include <cassert>

template<class T>
class OrderIndependentDeletionStack
{
public:
	OrderIndependentDeletionStack(size_t initialCapacity)
	{
		_nextID = std::numeric_limits<IDType>::lowest();
		_vectorID = GetNextVectorID();

		if (initialCapacity == 0)
			return;

		if (initialCapacity > _list.max_size())
			throw std::runtime_error("OrderIndependentDeletionStack::OrderIndependentDeletionStack Error: Initial capacity is above data list's maximum size!");

		if (initialCapacity > _deletedList.max_size())
			throw std::runtime_error("OrderIndependentDeletionStack::OrderIndependentDeletionStack Error: Initial capacity is above freed indexes list's maximum size!");

		if (initialCapacity > _additionOrder.max_size())
			throw std::runtime_error("OrderIndependentDeletionStack::OrderIndependentDeletionStack Error: Initial capacity is above order of additions list's maximum size!");

		_list.reserve(initialCapacity);
		_deletedList.reserve(_list.capacity());
		_additionOrder.reserve(_list.capacity());
	}

	~OrderIndependentDeletionStack() noexcept(std::is_nothrow_destructible_v<decltype(_list)>)
	{

	}

	OrderIndependentDeletionStack(const OrderIndependentDeletionStack<T>&) = delete;
	OrderIndependentDeletionStack(OrderIndependentDeletionStack<T>&& rhs) noexcept(std::is_nothrow_move_constructible_v<decltype(_list)>) : _nextID(std::move(rhs._nextID)),
		_vectorID(std::move(rhs._vectorID)), _list(std::move(rhs._list)), _deletedList(std::move(rhs._deletedList)), _additionOrder(std::move(rhs._additionOrder))
	{
		rhs._nextID = std::numeric_limits<IDType>::lowest();
		rhs._vectorID = GetNextVectorID();
	}

	OrderIndependentDeletionStack& operator=(const OrderIndependentDeletionStack<T>&) noexcept = delete;
	OrderIndependentDeletionStack& operator=(OrderIndependentDeletionStack<T>&& rhs) noexcept
	{
		_nextID = std::move(rhs._nextID);
		_vectorID = std::move(rhs._vectorID);

		_list = std::move(rhs._list);
		_deletedList = std::move(rhs._deletedList);
		_additionOrder = std::move(rhs._additionOrder);

		rhs._nextID = std::numeric_limits<IDType>::lowest();
		rhs._vectorID = GetNextVectorID();

		return *this;
	}

	IDObject<T> AddObject(const T& value, size_t addOnReserve)
	{
		if (!_deletedList.empty())
		{
			size_t pos = _deletedList.back();
			_list[pos].AssignValueToFreePosition(IDSubobject<T>(GetNextId()), value);
			_deletedList.pop_back();
			auto& ID = _list[pos].GetObjectID();
			_additionOrder.push_back(pos);
			return IDObject<T>(ID, _vectorID);
		}
		else
		{
			CheckCapacity(addOnReserve);

			_list.emplace_back(IDSubobject<T>(GetNextId()), value);
			auto& ID = _list.back().GetObjectID();
			_additionOrder.push_back(_list.size() - 1);
			return IDObject<T>(ID, _vectorID);
		}
	}

	IDObject<T> AddObject(T&& value, size_t addOnReserve)
	{
		if (!_deletedList.empty())
		{
			size_t pos = _deletedList.back();
			_list[pos].AssignValueToFreePosition(IDSubobject<T>(GetNextId()), std::move(value));
			_deletedList.pop_back();
			auto& ID = _list[pos].GetObjectID();
			_additionOrder.push_back(pos);
			return IDObject<T>(ID, _vectorID);
		}
		else
		{
			CheckCapacity(addOnReserve);

			_list.emplace_back(IDSubobject<T>(GetNextId()), std::move(value));
			auto& ID = _list.back().GetObjectID();
			_additionOrder.push_back(_list.size() - 1);
			return IDObject<T>(ID, _vectorID);
		}
	}

	bool RemoveObject(IDObject<T> objectID, bool throwOnIDNotFound)
	{
		if (objectID.GetVectorID() != _vectorID)
			throw std::runtime_error("OrderIndependentDeletionStack RemoveObject Error: Program tried to user an ID from another instance of this list!");

		auto it = std::find(_list.begin(), _list.end(), objectID.GetObjectID());

		if (it == _list.cend())
		{
			if (throwOnIDNotFound)
				throw std::runtime_error("OrderIndependentDeletionStack::RemoveObject Error: Program tried to delete a non-existent entry in a list!");
			else
				return false;
		}
		else
		{
			if (!it->HasValue())
				return false;

			size_t position = static_cast<size_t>(static_cast<size_t>(std::distance(_list.begin(), it)));

			it->DeleteObject();
			_deletedList.push_back(position);

			auto orderingIterator = std::find(_additionOrder.begin(), _additionOrder.end(), position);

			if (orderingIterator == _additionOrder.cend())
				throw std::runtime_error("OrderIndependentDeletionStack::RemoveObject Error: Program tried to delete a non-existent entry in an additon order list!");

			_additionOrder.erase(orderingIterator);

			return true;
		}
	}

	size_t GetSize() const noexcept { return _list.size(); }
	size_t GetUsedSize() const noexcept
	{
		assert(_list.size() >= _deletedList.size());

		return _list.size() - _deletedList.size();
	}

	size_t GetCapacity() const noexcept { return _list.capacity(); }
	size_t GetUnusedCapacity() const noexcept
	{
		assert(_list.capacity() >= _deletedList.capacity());

		return _list.capacity() - _list.size();
	}

	size_t GetUnusedAndDeletedCapacity() const noexcept
	{
		size_t ret = GetUnusedCapacity() + _deletedList.size();
		assert(ret >= _deletedList.size());
		return ret;
	}

	void ShrinkToFit(size_t changeToCapacity, bool addToCapacity) noexcept(std::is_nothrow_move_constructible_v<T> || std::is_nothrow_copy_constructible_v<T>)
	{
		if (_deletedList.empty())
			return;

		_list.shrink_to_fit();

		std::vector<CommonVectorObject<T>> tempList;
		std::vector<size_t> tempOrderList;

		if (addToCapacity)
		{
			size_t fullres = GetUsedSize() + changeToCapacity;

			if (fullres < changeToCapacity)
				throw std::runtime_error("UnsortedList::ShrinkToFit Error: Reservation amount overflowed!");

			tempList.reserve(fullres);
		}
		else
		{
			size_t usedSize = GetUsedSize();
			if (changeToCapacity > usedSize)
			{
				tempList.reserve(changeToCapacity);
			}
			else
			{
				tempList.reserve(usedSize);
			}
		}
		tempOrderList.reserve(tempList.capacity());

		for (size_t i = 0; i < _additionOrder.size(); ++i)
		{
			if (!_list[_additionOrder[i]].HasValue())
				continue;

			if constexpr (std::is_nothrow_move_constructible_v<T>)
			{
				tempList.push_back(std::move(_list[_additionOrder[i]]));
			}
			else if constexpr (std::is_nothrow_copy_constructible_v<T>)
			{
				tempList.push_back(_list[_additionOrder[i]]);
			}
			else if constexpr (std::is_move_constructible_v<T>)
			{
				tempList.push_back(_list[_additionOrder[i]]);
			}
			else if constexpr (std::is_copy_constructible_v<T>)
			{
				tempList.push_back(_list[_additionOrder[i]]);
			}
			else
				throw std::runtime_error("UnsortedList::ShrinkToFit Error: Program tried to copy uncopyable data!");

			tempOrderList.push_back(tempList.size() - 1);
		}

		_list = std::move(tempList);
		_deletedList.clear();
		_deletedList.shrink_to_fit();
		_deletedList.reserve(_list.capacity());

		_additionOrder = std::move(tempOrderList);
	}

	T& GetObject(size_t positionInOrderList)
	{
		if (positionInOrderList >= _additionOrder.size())
			throw std::runtime_error("OrderIndependentDeletionStack::GetObject Error: Program tried to read data from outside the addition order list!");

		size_t positionInDataList = _additionOrder[(_additionOrder.size() - positionInOrderList) - 1];

		if (positionInDataList >= _list.size())
			throw std::runtime_error("OrderIndependentDeletionStack::GetObject Const Error: An ID from the addition order list does not exist in the object list!");

		return _list[positionInDataList].GetObject();
	}

	const T& GetObject(size_t positionInOrderList) const
	{
		if (positionInOrderList >= _additionOrder.size())
			throw std::runtime_error("OrderIndependentDeletionStack::GetObject Const Error: Program tried to read data from outside the addition order list!");

		size_t positionInDataList = _additionOrder[(_additionOrder.size() - positionInOrderList) - 1];

		if (positionInDataList >= _list.size())
			throw std::runtime_error("OrderIndependentDeletionStack::GetObject Const Error: An ID from the addition order list does not exist in the object list!");

		return _list[positionInDataList].GetObject();
	}

	void Reset(size_t capacityAfterReset)
	{
		_list.clear();
		_deletedList.clear();
		_additionOrder.clear();

		_list.shrink_to_fit();
		_deletedList.shrink_to_fit();
		_additionOrder.shrink_to_fit();

		_vectorID = GetNextVectorID();
		_nextID = std::numeric_limits<IDType>::lowest();

		if (capacityAfterReset == 0)
			return;

		_list.reserve(capacityAfterReset);
		_deletedList.reserve(_list.capacity());
		_additionOrder.reserve(_list.capacity());
	}


private:
	IDType _nextID;
	IDType _vectorID;
	std::vector<CommonVectorObject<T>> _list;
	std::vector<size_t> _deletedList;
	std::vector<size_t> _additionOrder;

	static IDType _nextVectorID;

	static IDType GetNextVectorID()
	{
		if (_nextVectorID == std::numeric_limits<IDType>::max())
			throw std::runtime_error("OrderIndependentDeletionStack::GetNextVectorID Error: Vector ID overflow!");

		return _nextVectorID++;
	}

	IDType GetNextId()
	{
		IDType ret = _nextID;
		if (_nextID == std::numeric_limits<IDType>::max())
			throw std::runtime_error("OrderIndependentDeletionStack::GetNextId Error: Id system overflowed!");

		_nextID++;
		return ret;
	}

	void ReserveAdditional(size_t addToCapacity)
	{
		size_t reserved = _list.capacity() + addToCapacity;

		if (reserved < addToCapacity)
			throw std::runtime_error("OrderIndependentDeletionStack::ReserveAdditional Error: Reserved amount overflowed!");

		size_t minimumMaximumSize = std::min(_list.max_size(), std::min(_deletedList.max_size(), _additionOrder.max_size()));

		if (reserved > minimumMaximumSize)
		{
			if (_list.capacity() == _list.max_size())
				throw std::runtime_error("OrderIndependentDeletionStack::ReserveAdditional Error: Program tried to expand data list vector when it's already at maximum size!");

			if (_deletedList.capacity() == _deletedList.max_size())
				throw std::runtime_error("OrderIndependentDeletionStack::ReserveAdditional Error: Program tried to expand freed indexes vector when it's already at maximum size!");

			if (_additionOrder.capacity() == _additionOrder.max_size())
				throw std::runtime_error("OrderIndependentDeletionStack::ReserveAdditional Error: Program tried to expand addition order vector when it's already at maximum size!");

			reserved = minimumMaximumSize;
		}

		_list.reserve(reserved);
		_deletedList.reserve(_list.capacity());
		_additionOrder.reserve(_list.capacity());
	}

	void CheckCapacity(size_t addOnReserve)
	{
		if (_list.capacity() != _list.size())
			return;

		if (addOnReserve == 0)
		{
			size_t capacity = _list.capacity();
			if (capacity != 0)
				ReserveAdditional(capacity);
			else
				ReserveAdditional(1);
		}
		else
		{
			ReserveAdditional(addOnReserve);
		}
	}
};

template<class T>
IDType OrderIndependentDeletionStack<T>::_nextVectorID = std::numeric_limits<IDType>::lowest();
