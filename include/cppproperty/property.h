#pragma clang diagnostic push
#pragma ide diagnostic ignored "google-explicit-constructor"
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <utility>

template<typename T, typename TReal, auto get>
concept static_property_getter = get == 0 || requires(TReal& t) {
    { get(t) } -> std::convertible_to<T> ;
};

template<typename T, typename TReal, auto set>
concept static_property_setter = set == 0 || requires(TReal& t, const T& v) {
    { set(t, v) };
};

template<typename T, typename TReal, auto move>
concept static_property_mover = move == 0 || requires(TReal& t, T&& v) {
    { move(t, std::move(v)) };
};

template<typename T, typename TReal, auto const_get>
concept static_property_const_getter = const_get == 0 || requires(const TReal& t) {
    { const_get(t) } -> std::convertible_to<T>;
};

template<typename Owner, typename T, typename TReal, auto get>
concept instance_property_getter = get == 0 || requires(Owner& o, TReal& t) {
    { get(o, t) } -> std::convertible_to<T>;
};

template<typename Owner, typename T, typename TReal, auto set>
concept instance_property_setter = set == 0 || requires(Owner& o, TReal& t, const T& v) {
    { set(o, t, v) };
};

template<typename Owner, typename T, typename TReal, auto move>
concept instance_property_mover = move == 0 || requires(Owner& o, TReal& t, T&& v) {
    { move(o, t, std::move(v)) };
};

template<typename Owner, typename T, typename TReal, auto const_get>
concept instance_property_const_getter = const_get == 0 || requires(const Owner& o, const TReal& t) {
    { const_get(o, t) } -> std::convertible_to<T>;
};

template<typename T, typename TReal, auto get, auto set, auto const_get, auto move>
concept static_property_methods = static_property_getter<T, TReal, get>
                                    && static_property_setter<T, TReal, set>
                                    && static_property_const_getter<T, TReal, const_get>
                                    && static_property_mover<T, TReal, move>;

template<typename Owner, typename T, typename TReal, auto get, auto set, auto const_get, auto move>
concept instance_property_methods = not std::is_same_v<Owner, void>
                                    && instance_property_getter<Owner, T, TReal, get>
                                    && instance_property_setter<Owner, T, TReal, set>
                                    && instance_property_const_getter<Owner, T, TReal, const_get>
                                    && instance_property_mover<Owner, T, TReal, move>;

template<typename Owner, typename T, typename TReal = T, auto get = nullptr,
         auto set = nullptr, auto const_get = nullptr, auto move = nullptr>
    requires ((std::is_same_v<Owner, void> && static_property_methods<T, TReal, get, set, const_get, move>)
        || instance_property_methods<Owner, T, TReal, get, set, const_get, move>
                 )
struct property;

template<typename T, typename TReal, auto get, auto set, auto const_get, auto move>
		requires static_property_methods<T, TReal, get, set, const_get, move>
struct property<void, T, TReal,  get, set, const_get, move> {
	constexpr property(T value) : _value(value) {}
	constexpr property(const property& other) = default;
	constexpr property(property&& other) noexcept = default;
	constexpr property& operator=(const property& other) = default;
	constexpr property& operator=(property&& other) noexcept = default;

	constexpr property& operator=(const T& value) requires (set == 0) {
		_value = value;
		return *this;
	}
	
	constexpr property& operator=(const T& value) requires (set != 0) {
		set(_value, value);
		return *this;
	}

    constexpr property& operator=(T&& value) requires (move == 0) {
        _value = value;
        return *this;
    }

	constexpr property& operator=(T&& value) requires (move != 0) {
	    move(_value, std::move(value));
	    return *this;
	}
		
	constexpr operator T() requires (get != 0) {
		return get(_value);
	}
	
	constexpr operator T() const noexcept requires (const_get == 0) {
		return _value;
	}

	constexpr operator T() const requires (const_get != 0) {
		return const_get(_value);
	}

private:
    TReal _value;
};

template<typename Owner, typename T, auto get, auto set, auto const_get, auto move>
    requires instance_property_methods<Owner, T, T, get, set, const_get, move>
struct property<Owner, T, T,  get, set, const_get, move>{
    constexpr property(T value, Owner& owner) : _value(value), _owner(owner) {}
    constexpr property(const property& other) = default;
    constexpr property(property&& other) noexcept = default;
    property& operator=(const property& other) = delete;
    property& operator=(property&& other) = delete;

    constexpr property& operator=(const T& value) requires (set == 0) {
         _value = value;
         return *this;
    }

    constexpr property& operator=(const T& value) const requires (set != 0) {
		set(_owner, _value, value);
        return *this;
    }

    constexpr property& operator=(T&& value) requires (move == 0) {
        _value = value;
        return *this;
    }

    constexpr property& operator=(T&& value) requires (move != 0) {
        move(_owner, _value, std::move(value));
        return *this;
    }

    constexpr operator T() requires (get != 0) {
		return get(_owner, _value);
    }
    
    constexpr operator T() const noexcept requires (const_get == 0) {
        return _value;
    }
    
    constexpr operator T() const requires (const_get != 0) {
        return const_get(_owner, _value);
    }

private:
	T _value;
	Owner& _owner;
};
#pragma clang diagnostic pop