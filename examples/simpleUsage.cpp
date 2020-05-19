#include "cppproperty/property.h"

using propType = int;
using storageType = std::uint8_t;

constexpr propType test_get(storageType& v) {
	return v;
}

constexpr void test_set(storageType& current, const propType& v) {
	current = v;
}

constexpr void test_move(storageType& current, propType&& v) {
    current = v;
}

constexpr propType test_const_get(const storageType& v) {
	return v;
}

struct test_struct {
private:
    storageType _offset;
    static constexpr propType test_get(test_struct& self, propType& v) { return v + self._offset; }
    static constexpr void test_set(test_struct&, propType& current, propType value) { current = value; }
    static constexpr propType test_const_get(const test_struct& self, const propType& v) { return ::test_const_get(v) + self._offset; }
    static constexpr void test_move(const test_struct& self, propType& current, propType&& value) { current = value; }

public:
    property<test_struct, propType, propType, test_struct::test_get, test_struct::test_set, test_struct::test_const_get, test_struct::test_move> x;

test_struct(propType _x, storageType offset) : _offset(offset),  x(_x, *this) {}

};

int main() {
    auto prop = property<void, propType, storageType, test_get, test_set, test_const_get, test_move>(0xFF);
    propType size = sizeof(prop);
    prop = 5;
    const auto& constProp = prop;
    test_struct ts(3, 1);
    ts.x = 31;
    auto defaultProp = property<void, propType, storageType>(0x11);
    return ((size - prop + constProp) * 256) + ts.x - defaultProp;
}