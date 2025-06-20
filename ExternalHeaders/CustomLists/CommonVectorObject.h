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
	CommonVectorObject(const IDSubobject<T>& objectID, const T& object) : _objectID(objectID), _object(object)
	{
		std::memset(_padding, 0, sizeof(_padding));
	}

	CommonVectorObject(const IDSubobject<T>& objectID, T&& object) noexcept : _objectID(objectID), _object(std::move(object))
	{
		std::memset(_padding, 0, sizeof(_padding));
	}

	CommonVectorObject(const CommonVectorObject<T>& other) noexcept : _objectID(other._objectID), _object(other._object)
	{
		std::memset(_padding, 0, sizeof(_padding));
	}

	CommonVectorObject(CommonVectorObject<T>&& other) noexcept : _objectID(std::move(other._objectID)), _object(std::move(other._object))
	{
		std::memset(_padding, 0, sizeof(_padding));
	}

	CommonVectorObject<T>& operator=(const CommonVectorObject<T>& other)
	{
		_objectID = other._objectID;
		_object = other._object;

		std::memset(_padding, 0, sizeof(_padding));
	}

	CommonVectorObject<T>& operator=(CommonVectorObject<T>&& other)
	{
		_objectID = std::move(other._objectID);
		_object = std::move(other._object);

		std::memset(_padding, 0, sizeof(_padding));
	}

	void ReplaceValue(const IDSubobject<T>& objectID, const T& object)
	{
		if (_object.has_value())
			throw std::runtime_error("ListObjectTemplate::ReplaceValue Error: Program tried to replace existing value with const object!");

		_object = object;
		_objectID = objectID;
	}

	void ReplaceValue(const IDSubobject<T>& objectID, T&& object)
	{
		if (_object.has_value())
			throw std::runtime_error("ListObjectTemplate::ReplaceValue Error: Program tried to replace existing value with non const object!");

		_object = std::move(object);
		_objectID = objectID;
	}

	IDSubobject<T> GetObjectID() const { return _objectID; }

	std::optional<T>& GetObjectOptional() { return _object; }
	const std::optional<T>& GetConstObjectOptional() const { return _object; }
	std::optional<T> GetObjectOptionalCopy() const { return _object; }

	T& GetObject() { return _object.value(); }
	const T& GetConstObject() const { return _object.value(); }
	T GetObjectCopy() const { return _object.value(); }

	bool HasValue() const { return _object.has_value(); }

	void DeleteObject() { _object.reset(); }

	bool operator==(bool has_value) const { return _object.has_value() == has_value; }
	bool operator==(const IDSubobject<T>& ID) const { return ID == _objectID; }

	template <class U>
		requires (is_three_way_comparable<T, U>)
	std::compare_three_way_result_t<T, U> operator<=>(const CommonVectorObject<U>& rhs) const noexcept
	{
		if (_objectID != rhs._objectID)
			return _objectID <=> rhs._objectID;

		if (_object.has_value())
		{
			if (rhs._object.has_value())
				return _object.value() <=> rhs._object.value();

			return std::compare_three_way_result_t<T, U>::greater;
		}

		if (rhs._object.has_value())
			return std::compare_three_way_result_t<T, U>::less;

		return std::compare_three_way_result_t<T, U>::equivalent;
	}

	bool operator==(const CommonVectorObject<T>&) const noexcept = default;

	bool operator==(const T& other) const noexcept { return _object == other; };
	bool operator==(const std::optional<T>& other) const noexcept { return _object == other; };

private:
	IDSubobject<T> _objectID;
	std::optional<T> _object;
	char _padding[16 - (sizeof(_object) % 8)];
};
