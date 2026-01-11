#pragma once

#include "CommonVectorObject.h"

#include <vector>
#include <stdexcept>
#include <utility>
#include <cassert>
#include <algorithm>

template<class T>
class UnsortedIDVector
{
public:
	explicit UnsortedIDVector(size_t initialCapacity)
	{
		_nextID = std::numeric_limits<IDType>::lowest();
		_vectorID = GetNextVectorID();

		if (initialCapacity != 0)
		{
			if (initialCapacity > _list.max_size())
				throw std::runtime_error("UnsortedIDVector::UnsortedIDVector Error: Initial capacity is above data list's maximum size!");

			if (initialCapacity > _deletedList.max_size())
				throw std::runtime_error("UnsortedIDVector::UnsortedIDVector Error: Initial capacity is above freed indexes list's maximum size!");

			if (initialCapacity > _IDList.max_size())
				throw std::runtime_error("UnsortedIDVector::UnsortedIDVector Error: Initial capacity is above ID list's maximum size!");

			_list.reserve(initialCapacity);
			_deletedList.reserve(_list.capacity());
			_IDList.reserve(_list.capacity());
		}
	}

	~UnsortedIDVector() noexcept(std::is_nothrow_destructible_v<IDType> && std::is_nothrow_destructible_v<decltype(_list)> && std::is_nothrow_destructible_v<decltype(_deletedList)> &&
		std::is_nothrow_destructible_v<decltype(_IDList)>) {};

	UnsortedIDVector(const UnsortedIDVector<T>& rhs) = delete;
	UnsortedIDVector(UnsortedIDVector<T>&& rhs) noexcept(std::is_nothrow_move_constructible_v<IDType>&& std::is_nothrow_move_constructible_v<decltype(_list)>
		&& std::is_nothrow_move_constructible_v<decltype(_deletedList)>&& std::is_nothrow_move_constructible_v<decltype(_IDList)>)
		: _nextID(std::move(rhs._nextID)), _vectorID(std::move(rhs._vectorID)), _list(std::move(rhs._list)), _deletedList(std::move(rhs._deletedList)), _IDList(std::move(rhs._IDList))
	{
		rhs._vectorID = GetNextVectorID();
		rhs._nextID = std::numeric_limits<IDType>::lowest();
	}

	UnsortedIDVector<T>& operator=(const UnsortedIDVector<T>&) = delete;
	UnsortedIDVector<T>& operator=(UnsortedIDVector<T>&& rhs)
	{
		_nextID = std::move(rhs._nextID);
		_vectorID = std::move(rhs._vectorID);

		_list = std::move(rhs._list);
		_deletedList = std::move(rhs._deletedList);
		_IDList = std::move(rhs._IDList);

		rhs._vectorID = GetNextVectorID();
		rhs._nextID = std::numeric_limits<IDType>::lowest();

		return *this;
	}

	IDObject<T> AddUniqueObject(const T& value, size_t addOnReserve)
	{
		auto found = std::find(_list.cbegin(), _list.cend(), value);

		if (found != _list.cend())
		{
			return IDObject<T>(found->GetObjectID(), _vectorID);
		}
		else
		{
			if (!_deletedList.empty())
			{
				size_t pos = _deletedList.back();
				std::pair<IDSubobject<T>, size_t> previousIDRecord = { _list[pos].GetObjectID() , pos};
				auto oldIDIterator = std::lower_bound(_IDList.begin(), _IDList.end(), previousIDRecord);
				if (oldIDIterator == _IDList.end() || *oldIDIterator != previousIDRecord)
					throw std::runtime_error("UnsortedIDVector::AddUniqueObject Error: Program failed to find previous ID in the list in copy function!");

				_list[pos].AssignValueToFreePosition(IDSubobject<T>(GetNextId()), value);
				_deletedList.pop_back();
				_IDList.erase(oldIDIterator);
				std::pair<IDSubobject<T>, size_t> nextIDRecord = { _list[pos].GetObjectID() , pos };
				auto newIDIterator = std::lower_bound(_IDList.begin(), _IDList.end(), nextIDRecord);
				_IDList.insert(newIDIterator, nextIDRecord);
				
				return IDObject<T>(_list[pos].GetObjectID(), _vectorID);
			}
			else
			{
				CheckCapacity(addOnReserve);

				_list.emplace_back(IDSubobject<T>(GetNextId()), value);

				std::pair<IDSubobject<T>, size_t> newIDRecord = { _list.back().GetObjectID(), _list.size() - 1 };
				auto IDIterator = std::lower_bound(_IDList.cbegin(), _IDList.cend(), newIDRecord);
				_IDList.insert(IDIterator, newIDRecord);
				return IDObject<T>(_list.back().GetObjectID(), _vectorID);
			}
		}
	}

	IDObject<T> AddUniqueObject(T&& value, size_t addOnReserve)
	{
		auto found = std::find(_list.cbegin(), _list.cend(), value);

		if (found != _list.cend())
		{
			return IDObject<T>(found->GetObjectID(), _vectorID);
		}
		else
		{
			if (!_deletedList.empty())
			{
				size_t pos = _deletedList.back();
				std::pair<IDSubobject<T>, size_t> previousIDRecord = { _list[pos].GetObjectID() , pos };
				auto oldIDIterator = std::lower_bound(_IDList.begin(), _IDList.end(), previousIDRecord);
				if (oldIDIterator == _IDList.end() || *oldIDIterator != previousIDRecord)
					throw std::runtime_error("UnsortedIDVector::AddUniqueObject Error: Program failed to find previous ID in the list in move function!");

				_list[pos].AssignValueToFreePosition(IDSubobject<T>(GetNextId()), std::move(value));
				_deletedList.pop_back();
				_IDList.erase(oldIDIterator);
				std::pair<IDSubobject<T>, size_t> nextIDRecord = { _list[pos].GetObjectID() , pos };
				auto newIDIterator = std::lower_bound(_IDList.begin(), _IDList.end(), nextIDRecord);
				_IDList.insert(newIDIterator, nextIDRecord);

				return IDObject<T>(_list[pos].GetObjectID(), _vectorID);
			}
			else
			{
				CheckCapacity(addOnReserve);

				_list.emplace_back(IDSubobject<T>(GetNextId()), std::move(value));

				std::pair<IDSubobject<T>, size_t> newIDRecord = { _list.back().GetObjectID(), _list.size() - 1 };
				auto IDIterator = std::lower_bound(_IDList.cbegin(), _IDList.cend(), newIDRecord);
				_IDList.insert(IDIterator, newIDRecord);
				return IDObject<T>(_list.back().GetObjectID(), _vectorID);
			}
		}
	}

	IDObject<T> AddObject(const T& value, size_t addOnReserve)
	{
		if (!_deletedList.empty())
		{
			size_t pos = _deletedList.back();
			std::pair<IDSubobject<T>, size_t> previousIDRecord = { _list[pos].GetObjectID() , pos };
			auto oldIDIterator = std::lower_bound(_IDList.begin(), _IDList.end(), previousIDRecord);
			if (oldIDIterator == _IDList.end() || *oldIDIterator != previousIDRecord)
				throw std::runtime_error("UnsortedIDVector::AddObject Error: Program failed to find previous ID in the list in copy function!");

			_list[pos].AssignValueToFreePosition(IDSubobject<T>(GetNextId()), value);
			_deletedList.pop_back();
			_IDList.erase(oldIDIterator);
			std::pair<IDSubobject<T>, size_t> nextIDRecord = { _list[pos].GetObjectID() , pos };
			auto newIDIterator = std::lower_bound(_IDList.begin(), _IDList.end(), nextIDRecord);
			_IDList.insert(newIDIterator, nextIDRecord);

			return IDObject<T>(_list[pos].GetObjectID(), _vectorID);
		}
		else
		{
			CheckCapacity(addOnReserve);

			_list.emplace_back(IDSubobject<T>(GetNextId()), value);

			std::pair<IDSubobject<T>, size_t> newIDRecord = { _list.back().GetObjectID(), _list.size() - 1 };
			auto IDIterator = std::lower_bound(_IDList.cbegin(), _IDList.cend(), newIDRecord);
			_IDList.insert(IDIterator, newIDRecord);
			return IDObject<T>(_list.back().GetObjectID(), _vectorID);
		}
	}

	IDObject<T> AddObject(T&& value, size_t addOnReserve)
	{
		if (!_deletedList.empty())
		{
			size_t pos = _deletedList.back();
			std::pair<IDSubobject<T>, size_t> previousIDRecord = { _list[pos].GetObjectID() , pos };
			auto oldIDIterator = std::lower_bound(_IDList.begin(), _IDList.end(), previousIDRecord);
			if (oldIDIterator == _IDList.end() || *oldIDIterator != previousIDRecord)
				throw std::runtime_error("UnsortedIDVector::AddObject Error: Program failed to find previous ID in the list in move function!");

			_list[pos].AssignValueToFreePosition(IDSubobject<T>(GetNextId()), std::move(value));
			_deletedList.pop_back();
			_IDList.erase(oldIDIterator);
			std::pair<IDSubobject<T>, size_t> nextIDRecord = { _list[pos].GetObjectID() , pos };
			auto newIDIterator = std::lower_bound(_IDList.begin(), _IDList.end(), nextIDRecord);
			_IDList.insert(newIDIterator, nextIDRecord);

			return IDObject<T>(_list[pos].GetObjectID(), _vectorID);
		}
		else
		{
			CheckCapacity(addOnReserve);

			_list.emplace_back(IDSubobject<T>(GetNextId()), std::move(value));

			std::pair<IDSubobject<T>, size_t> newIDRecord = { _list.back().GetObjectID(), _list.size() - 1 };
			auto IDIterator = std::lower_bound(_IDList.cbegin(), _IDList.cend(), newIDRecord);
			_IDList.insert(IDIterator, newIDRecord);
			return IDObject<T>(_list.back().GetObjectID(), _vectorID);
		}
	}

	template<class... Arguments>
	IDObject<T> EmplaceObject(size_t addOnReserve, Arguments... arguments)
	{
		if (!_deletedList.empty())
		{
			size_t pos = _deletedList.back();
			std::pair<IDSubobject<T>, size_t> previousIDRecord = { _list[pos].GetObjectID() , pos };
			auto oldIDIterator = std::lower_bound(_IDList.begin(), _IDList.end(), previousIDRecord);
			if (oldIDIterator == _IDList.end() || *oldIDIterator != previousIDRecord)
				throw std::runtime_error("UnsortedIDVector::EmplaceObject Error: Program failed to find previous ID in the list!");

			_list[pos].AssignValueToFreePosition(IDSubobject<T>(GetNextId()), arguments);
			_deletedList.pop_back();
			_IDList.erase(oldIDIterator);
			std::pair<IDSubobject<T>, size_t> nextIDRecord = { _list[pos].GetObjectID() , pos };
			auto newIDIterator = std::lower_bound(_IDList.begin(), _IDList.end(), nextIDRecord);
			_IDList.insert(newIDIterator, nextIDRecord);

			return IDObject<T>(_list[pos].GetObjectID(), _vectorID);
		}
		else
		{
			CheckCapacity(addOnReserve);

			_list.emplace_back(IDSubobject<T>(GetNextId()), arguments);

			std::pair<IDSubobject<T>, size_t> newIDRecord = { _list.back().GetObjectID(), _list.size() - 1 };
			auto IDIterator = std::lower_bound(_IDList.cbegin(), _IDList.cend(), newIDRecord);
			_IDList.insert(IDIterator, newIDRecord);
			return IDObject<T>(_list.back().GetObjectID(), _vectorID);
		}
	}

	bool RemoveObject(IDObject<T> objectID, bool throwOnIDNotFound)
	{
		if (objectID.GetVectorID() != _vectorID)
			throw std::runtime_error("UnsortedIDVector::RemoveObject Error: Program tried to user an ID from another instance of this list!");

		auto it = std::find(_list.begin(), _list.end(), objectID.GetObjectID());

		if (it == _list.cend())
		{
			if (throwOnIDNotFound)
				throw std::runtime_error("UnsortedIDVector::RemoveObject Error: Program tried to delete a non-existent entry in a list!");
			else
				return false;
		}
		else
		{
			if (it->HasValue())
			{
				it->DeleteObject();
				_deletedList.push_back(static_cast<size_t>(std::distance(_list.begin(), it)));
			}
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

		decltype(_list) tempList;
		decltype(_IDList) tempIDList;

		if (addToCapacity)
		{
			size_t fullres = GetUsedSize() + changeToCapacity;

			if (fullres < changeToCapacity)
				throw std::runtime_error("UnsortedIDVector::ShrinkToFit Error: Reservation amount overflowed!");

			tempList.reserve(fullres);
			tempIDList.reserve(_list.capacity());
		}
		else
		{
			size_t usedSize = GetUsedSize();
			if (changeToCapacity > usedSize)
			{
				tempList.reserve(changeToCapacity);
				tempIDList.reserve(_list.capacity());
			}
			else
			{
				tempList.reserve(usedSize);
				tempIDList.reserve(_list.capacity());
			}
		}

		for (size_t i = 0; i < _list.size(); ++i)
		{
			if (_list[i].HasValue())
			{
				if constexpr (std::is_nothrow_move_constructible_v<T>)
				{
					tempList.push_back(std::move(_list[i]));
				}
				else if constexpr (std::is_nothrow_copy_constructible_v<T>)
				{
					tempList.push_back(_list[i]);
				}
				else if constexpr (std::is_move_constructible_v<T>)
				{
					tempList.push_back(std::move(_list[i]));
				}
				else if constexpr (std::is_copy_constructible_v<T>)
				{
					tempList.push_back(_list[i]);
				}
				else
					throw std::runtime_error("UnsortedIDVector::ShrinkToFit Error: Program tried to copy uncopyable objects!");

				tempIDList.emplace_back(_list[i].GetObjectID(), tempList.size() - 1);
			}
		}

		std::stable_sort(tempIDList.begin(), tempIDList.end());

		_list = std::move(tempList);
		_IDList = std::move(tempIDList);
		_deletedList.clear();
		_deletedList.shrink_to_fit();
		_deletedList.reserve(_list.capacity());
	}

	bool CheckForID(IDObject<T> objectID) const
	{
		if (objectID.GetVectorID() != _vectorID)
			throw std::runtime_error("UnsortedIDVector::CheckForID Error: Program tried to use an ID from another instance of this list!");

		auto it = std::lower_bound(_IDList.cbegin(), _IDList.cend(), std::pair<IDSubobject<T>, size_t>(objectID.GetObjectID(), std::numeric_limits<IDType>::lowest()));

		return it != _IDList.cend() && it->first == objectID.GetObjectID();
	}

	bool CheckForObject(IDObject<T> objectID) const
	{
		if (objectID.GetVectorID() != _vectorID)
			throw std::runtime_error("UnsortedIDVector::CheckForObject Error: Program tried to use an ID from another instance of this list!");

		auto it = std::lower_bound(_IDList.cbegin(), _IDList.cend(), std::pair<IDSubobject<T>, size_t>(objectID.GetObjectID(), std::numeric_limits<IDType>::lowest()));

		if (it == _IDList.cend() || it->first != objectID.GetObjectID())
			return false;
		else
			return _list[it->second].HasValue();
	}

	std::vector<std::optional<T>> TryToGetObjectList(const std::vector<IDObject<T>>& IDList) const
	{
		std::vector<std::optional<T>> ret;
		ret.resize(IDList.size());

		std::vector<bool> IDTried(IDList.size(), false);

		for (size_t i = 0; i < IDList.size(); ++i)
		{
			if (IDTried[i])
				continue;

			if (IDList[i].GetVectorID() != _vectorID)
				throw std::runtime_error("UnsortedIDVector::TryToGetObjectList Error: Program tried to use an ID from another instance of this list!");

			auto IDListIterator = std::lower_bound(_IDList.cbegin(), _IDList.cend(), std::pair<IDSubobject<T>, size_t>(IDList[i].GetObjectID(), std::numeric_limits<IDType>::lowest()));

			for (size_t j = i; j < IDList.size(); ++j)
			{
				if (IDList[j] == IDList[i])
					IDTried[j] = true;
			}

			if (IDListIterator == _IDList.cend() || IDListIterator->first != IDList[i].GetObjectID() || !_list[IDListIterator->second].HasValue())
				continue;

			for (size_t j = i; j < IDList.size(); ++j)
			{
				if (IDList[j] == IDList[i])
					ret[j] = _list[IDListIterator->second].GetObject();
			}
		}

		return ret;
	}

	T& GetObject(IDObject<T> objectID)
	{
		if (objectID.GetVectorID() != _vectorID)
			throw std::runtime_error("UnsortedIDVector::GetObject Error: Program tried to use an ID from another instance of this list!");

		auto IDListIterator = std::lower_bound(_IDList.cbegin(), _IDList.cend(), std::pair<IDSubobject<T>, size_t>(objectID.GetObjectID(), std::numeric_limits<IDType>::lowest()));

		if (IDListIterator == _IDList.cend() || IDListIterator->first != objectID.GetObjectID() || !_list[IDListIterator->second].HasValue())
			throw std::runtime_error("UnsortedIDVector::GetObject Error: Program tried to get non-existent object!");

		return _list[IDListIterator->second].GetObject();
	}

	const T& GetObject(IDObject<T> objectID) const
	{
		if (objectID.GetVectorID() != _vectorID)
			throw std::runtime_error("UnsortedIDVector::GetObject Const Error: Program tried to use an ID from another instance of this list!");

		auto IDListIterator = std::lower_bound(_IDList.cbegin(), _IDList.cend(), std::pair<IDSubobject<T>, size_t>(objectID.GetObjectID(), std::numeric_limits<IDType>::lowest()));

		if (IDListIterator == _IDList.cend() || IDListIterator->first != objectID.GetObjectID() || !_list[IDListIterator->second].HasValue())
			throw std::runtime_error("UnsortedIDVector::GetObject Const Error: Program tried to get non-existent object!");

		return _list[IDListIterator->second].GetObject();
	}

	std::optional<T> TryToGetObject(IDObject<T> objectID, bool throwOnIDNotFound)
	{
		if (objectID.GetVectorID() != _vectorID)
			throw std::runtime_error("UnsortedIDVector::TryToGetObject Error: Program tried to use an ID from another instance of this list!");

		std::optional<T> ret;

		auto IDListIterator = std::lower_bound(_IDList.cbegin(), _IDList.cend(), std::pair<IDSubobject<T>, size_t>(objectID.GetObjectID(), std::numeric_limits<IDType>::lowest()));

		if (IDListIterator != _IDList.cend() && IDListIterator->first == objectID.GetObjectID() && _list[IDListIterator->second].HasValue())
			ret = _list[IDListIterator->second].GetObject();
		else if (throwOnIDNotFound && IDListIterator != _IDList.cend() && IDListIterator->first == objectID.GetObjectID())
			throw std::runtime_error("UnsortedIDVector::TryToGetObject Error: Program cannot find the specified object!");

		return ret;
	}

	std::vector<T> GetObjectList(const std::vector<IDObject<T>>& IDList) const
	{
		std::vector<T> ret;
		std::vector<size_t> foundPositionsList;
		foundPositionsList.resize(IDList.size());
		ret.reserve(IDList.size());

		std::vector<bool> IDTried(IDList.size(), false);

		for (size_t i = 0; i < IDList.size(); ++i)
		{
			if (IDTried[i])
				continue;

			if (IDList[i].GetVectorID() != _vectorID)
				throw std::runtime_error("UnsortedIDVector::GetObjectList Error: Program tried to use an ID from another instance of this list!");

			auto IDListIterator = std::lower_bound(_IDList.cbegin(), _IDList.cend(), std::pair<IDSubobject<T>, size_t>(IDList[i].GetObjectID(), std::numeric_limits<IDType>::lowest()));

			for (size_t j = i; j < IDList.size(); ++j)
			{
				if (IDList[j] == IDList[i])
					IDTried[j] = true;
			}

			if (IDListIterator == _IDList.cend() || IDListIterator->first != IDList[i].GetObjectID() || !_list[IDListIterator->second].HasValue())
				throw std::runtime_error("UnsortedIDVector::GetObjectList Error: Program failed to find an object!");

			for (size_t j = i; j < IDList.size(); ++j)
			{
				if (IDList[j] == IDList[i])
					foundPositionsList[j] = IDListIterator->second;
			}
		}

		for (size_t i = 0; i < foundPositionsList.size(); ++i)
			ret.push_back(_list[foundPositionsList[i]].GetObject());

		return ret;
	}

	void Reset(size_t capacityAfterReset)
	{
		_list.clear();
		_deletedList.clear();
		_IDList.clear();

		_list.shrink_to_fit();
		_deletedList.shrink_to_fit();
		_IDList.shrink_to_fit();

		_vectorID = GetNextVectorID();
		_nextID = std::numeric_limits<IDType>::lowest();

		if (capacityAfterReset == 0)
			return;

		_list.reserve(capacityAfterReset);
		_deletedList.reserve(_list.capacity());
		_IDList.reserve(_list.capacity());
	}

protected:
	IDType _nextID;
	IDType _vectorID;
	std::vector<CommonVectorObject<T>> _list;
	std::vector<size_t> _deletedList;
	std::vector<std::pair<IDSubobject<T>, size_t>> _IDList;

	static IDType _nextVectorID;

	static IDType GetNextVectorID()
	{
		if (_nextVectorID == std::numeric_limits<IDType>::max())
			throw std::runtime_error("UnsortedIDVector::GetNextVectorID Error: Vector ID overflow!");

		return _nextVectorID++;
	}

	IDType GetNextId()
	{
		IDType ret = _nextID;
		if (_nextID == std::numeric_limits<IDType>::max())
			throw std::runtime_error("UnsortedIDVector::GetNextId Error: Id system overflowed!");

		_nextID++;
		return ret;
	}

	void ReserveAdditional(size_t addToCapacity)
	{
		size_t reserved = _list.capacity() + addToCapacity;

		if (reserved < addToCapacity)
			throw std::runtime_error("UnsortedIDVector::ReserveAdditional Error: Reserved amount overflowed!");

		size_t minimumMaximumSize = std::min(_list.max_size(), std::min(_deletedList.max_size(), _IDList.max_size()));

		if (reserved > minimumMaximumSize)
		{
			if (_list.capacity() == _list.max_size())
				throw std::runtime_error("UnsortedIDVector::ReserveAdditional Error: Program tried to expand data list vector when it's already at maximum size!");

			if (_deletedList.capacity() == _deletedList.max_size())
				throw std::runtime_error("UnsortedIDVector::ReserveAdditional Error: Program tried to expand freed indexes vector when it's already at maximum size!");

			if (_IDList.capacity() == _IDList.max_size())
				throw std::runtime_error("UnsortedIDVector::ReserveAdditional Error: Program tried to expand ID list vector when it's already at maximum size!");

			reserved = minimumMaximumSize;
		}

		_list.reserve(reserved);
		_deletedList.reserve(_list.capacity());
		_IDList.reserve(_list.capacity());
	}

	void CheckCapacity(size_t addOnReserving)
	{
		if (_list.capacity() != _list.size())
			return;

		if (addOnReserving == 0)
		{
			size_t capacity = _list.capacity();
			if (capacity != 0)
				ReserveAdditional(capacity);
			else
				ReserveAdditional(1);
		}
		else
		{
			ReserveAdditional(addOnReserving);
		}
	}
};

template<class T>
IDType UnsortedIDVector<T>::_nextVectorID = std::numeric_limits<IDType>::lowest();
