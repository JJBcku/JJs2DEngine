#pragma once

#include <stdint.h>
#include <limits>
#include <compare>
#include <utility>

typedef uint64_t IDType;

template<class T>
class IDSubobject
{
public:
	explicit IDSubobject(IDType subID = std::numeric_limits<IDType>::max()) noexcept(std::is_nothrow_constructible_v<IDType>) : _subID(subID) {};
	IDSubobject(const IDSubobject& rhs) noexcept(std::is_nothrow_copy_constructible_v<IDType>) : _subID(rhs._subID) {};
	IDSubobject(IDSubobject&& rhs) noexcept(std::is_nothrow_move_constructible_v<IDType>) : _subID(std::move(rhs._subID)) {};

	~IDSubobject() noexcept(std::is_nothrow_destructible_v<IDType>) {};

	IDSubobject& operator=(const IDSubobject& rhs) noexcept(std::is_nothrow_copy_assignable_v<IDType>)
	{
		_subID = rhs._subID;
		return *this;
	}

	IDSubobject& operator=(IDSubobject&& rhs) noexcept(std::is_nothrow_move_assignable_v<IDType>)
	{
		_subID = std::move(rhs._subID);
		return *this;
	}

	bool operator==(const IDSubobject& rhs) const noexcept(noexcept(_subID == rhs._subID)) { return _subID == rhs._subID; };
	std::strong_ordering operator<=>(const IDSubobject& rhs) const noexcept(noexcept(_subID <=> rhs._subID)) { return _subID <=> rhs._subID; };

	bool operator==(const IDType& rhs) const noexcept(noexcept(_subID == rhs)) { return _subID == rhs; };
	std::strong_ordering operator<=>(const IDType& rhs) const noexcept(noexcept(_subID <=> rhs)) { return _subID <=> rhs; };


private:
	IDType _subID;

};

template<class T>
class IDObject
{
public:
	explicit IDObject(IDType objectID = std::numeric_limits<IDType>::max(), IDType vectorID = std::numeric_limits<IDType>::max())
		noexcept(std::is_nothrow_constructible_v<IDSubobject<T>, IDType>) : _objectID(objectID), _vectorID(vectorID) {};
	explicit IDObject(IDType objectID, const IDSubobject<T>& vectorID)
		noexcept(std::is_nothrow_constructible_v<IDSubobject<T>, IDType> && std::is_nothrow_copy_constructible_v<IDSubobject<T>>) : _objectID(objectID), _vectorID(vectorID) {};
	explicit IDObject(const IDSubobject<T>& objectID, size_t vectorID = std::numeric_limits<IDType>::max())
		noexcept(std::is_nothrow_constructible_v<IDSubobject<T>, IDType> && std::is_nothrow_copy_constructible_v<IDSubobject<T>>) : _objectID(objectID), _vectorID(vectorID) {};
	explicit IDObject(const IDSubobject<T>& objectID, const IDSubobject<T>& vectorID)
		noexcept(std::is_nothrow_copy_constructible_v<IDSubobject<T>>) : _objectID(objectID), _vectorID(vectorID) {};

	~IDObject() noexcept(std::is_nothrow_destructible_v<IDSubobject<T>>) {};

	IDObject(const IDObject& rhs) noexcept(std::is_nothrow_copy_constructible_v<IDSubobject<T>>) : _objectID(rhs._objectID), _vectorID(rhs._vectorID) {};
	IDObject(IDObject&& rhs) noexcept(std::is_nothrow_move_constructible_v<IDSubobject<T>>) : _objectID(std::move(rhs._objectID)), _vectorID(std::move(rhs._vectorID)) {};

	IDObject<T>& operator=(const IDObject<T>& rhs) noexcept(std::is_nothrow_copy_assignable_v<IDSubobject<T>>)
	{
		_objectID = rhs._objectID;
		_vectorID = rhs._vectorID;
		return *this;
	}
	IDObject<T>& operator=(IDObject<T>&& rhs) noexcept(std::is_nothrow_move_assignable_v<IDSubobject<T>>)
	{
		_objectID = std::move(rhs._objectID);
		_vectorID = std::move(rhs._vectorID);
		return *this;
	}

	bool operator==(const IDObject<T>& rhs) const noexcept(noexcept(_objectID == rhs._objectID))
	{
		if (_objectID == rhs._objectID)
			return _vectorID == rhs._vectorID;
		else
			return false;
	}
	std::strong_ordering operator<=>(const IDObject<T>& rhs) const noexcept(noexcept(_objectID <=> rhs._objectID))
	{
		if (_objectID == rhs._objectID)
			return _vectorID <=> rhs._vectorID;
		else
			return _objectID <=> rhs._objectID;
	}

	const IDSubobject<T>& GetObjectID() const noexcept { return _objectID; };
	const IDSubobject<T>& GetVectorID() const noexcept { return _vectorID; };

private:
	IDSubobject<T> _objectID;
	IDSubobject<T> _vectorID;
};
