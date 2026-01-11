#pragma once

#include "IDObject.h"

#include <concepts>
#include <compare>
#include <optional>
#include <stdexcept>

template<class T, class U>
concept is_three_way_comparable = requires()
{
	std::is_same_v<T, U>;
	typename std::compare_three_way_result_t<T, U>;
};

template<class T>
class CommonVectorObject
{
public:
	explicit CommonVectorObject(const IDSubobject<T>& objectID, const T& object)
		noexcept(std::is_nothrow_copy_constructible_v<IDSubobject<T>> && std::is_nothrow_copy_constructible_v<std::optional<T>>) : _objectID(objectID), _object(object) {};
	explicit CommonVectorObject(const IDSubobject<T>& objectID, T&& object)
		noexcept(std::is_nothrow_copy_constructible_v<IDSubobject<T>> && std::is_nothrow_move_constructible_v<std::optional<T>>) : _objectID(objectID), _object(std::move(object)) {};

	template<class... Arguments>
	explicit CommonVectorObject(const IDSubobject<T>& objectID, Arguments... arguments)
		noexcept(std::is_nothrow_constructible_v<std::optional<T>, Arguments...>) : _objectID(objectID), _object(arguments...) {};

	CommonVectorObject(const CommonVectorObject<T>& other) noexcept(std::is_nothrow_copy_constructible_v<IDSubobject<T>> && std::is_nothrow_copy_constructible_v<std::optional<T>>)
		: _objectID(other._objectID), _object(other._object) {};
	CommonVectorObject(CommonVectorObject<T>&& other) noexcept(std::is_nothrow_move_constructible_v<IDSubobject<T>>&& std::is_nothrow_move_constructible_v<std::optional<T>>)
		: _objectID(std::move(other._objectID)), _object(std::move(other._object))
	{
		other._objectID = IDSubobject<T>();
	}

	CommonVectorObject<T>& operator=(const CommonVectorObject<T>& other)
		noexcept(std::is_nothrow_copy_assignable_v<IDSubobject<T>>&& std::is_nothrow_copy_assignable_v<std::optional<T>>)
	{
		_objectID = other._objectID;
		_object = other._object;

		return *this;
	}

	CommonVectorObject<T>& operator=(CommonVectorObject<T>&& other)
		noexcept(std::is_nothrow_move_assignable_v<IDSubobject<T>>&& std::is_nothrow_move_assignable_v<std::optional<T>>)
	{
		_objectID = std::move(other._objectID);
		_object = std::move(other._object);

		other._objectID = IDSubobject<T>();

		return *this;
	}

	void AssignValueToFreePosition(const IDSubobject<T>& objectID, const T& object)
	{
		if (_object.has_value())
			throw std::runtime_error("CommonVectorObject::AssignValueToFreePosition Error: Program tried to replace existing value with const object!");

		_objectID = objectID;
		_object = object;
	}

	void AssignValueToFreePosition(const IDSubobject<T>& objectID, T&& object)
	{
		if (_object.has_value())
			throw std::runtime_error("CommonVectorObject::AssignValueToFreePosition Error: Program tried to replace existing value with non const object!");

		_objectID = objectID;
		_object = std::move(object);
	}

	template<class... Arguments>
	void EmplaceValueInFreePosition(const IDSubobject<T>& objectID, Arguments... arguments)
	{
		if (_object.has_value())
			throw std::runtime_error("CommonVectorObject::EmplaceValueInFreePosition Error: Program tried to replace existing value!");

		_objectID = objectID;
		_object.emplace(arguments);
	}

	const IDSubobject<T>& GetObjectID() const noexcept { return _objectID; }

	T& GetObject()
	{
		if (!_object.has_value())
			throw std::runtime_error("CommonVectorObject::GetObject Error: Program tried to get a non-existent object data!");

		return _object.value();
	}
	const T& GetObject() const
	{
		if (!_object.has_value())
			throw std::runtime_error("CommonVectorObject::GetObject Const Error: Program tried to get a non-existent object data!");

		return _object.value();
	}

	bool HasValue() const noexcept { return _object.has_value(); }

	void DeleteObject() noexcept(std::is_nothrow_destructible_v<std::optional<T>>) { _object.reset(); }

	bool operator==(bool has_value) const noexcept(noexcept(_object.has_value() == has_value)) { return _object.has_value() == has_value; }
	bool operator==(const IDSubobject<T>& ID) const noexcept(noexcept(ID == _objectID)) { return ID == _objectID; }

	template <class U>
		requires (is_three_way_comparable<T, U>)
	std::compare_three_way_result_t<T, U> operator<=>(const CommonVectorObject<U>& rhs) const
		noexcept(noexcept(_objectID != rhs._objectID) && noexcept(_objectID <=> rhs._objectID) && noexcept(_object <=> rhs._object))
	{
		if (_objectID != rhs._objectID)
			return _objectID <=> rhs._objectID;
		else
			return _object <=> rhs._object;
	}

	bool operator==(const CommonVectorObject<T>& rhs) const noexcept(noexcept(_objectID == rhs._objectID) && noexcept(_object == rhs._object))
	{
		if (_objectID == rhs._objectID)
			return _object == rhs._object;
		else
			return false;
	}

	bool operator==(const T& other) const noexcept(noexcept(_object == other)) { return _object == other; };
	bool operator==(const std::optional<T>& other) const noexcept(noexcept(_object == other)) { return _object == other; };

private:
	IDSubobject<T> _objectID;
	std::optional<T> _object;
};
