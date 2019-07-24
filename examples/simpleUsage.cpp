#include "cppproperty/property.h"

using propType = int;
using storageType = std::uint8_t;

constexpr propType test_get(storageType& v) {
	return v;
}

constexpr void test_set(storageType& current, propType v) {
	current = v;
}

constexpr propType test_const_get(const storageType& v) {
	return v;
}

struct test_struct {
private:
    storageType _offset;
    static constexpr propType test_get(test_struct& self, storageType& v) { return ::test_get(v) + self._offset; }
    static constexpr void test_set(test_struct&, storageType& current, propType value) { return ::test_set(current, value); }
    static constexpr propType test_const_get(const test_struct& self, const storageType& v) { return ::test_const_get(v) + self._offset; }

public:
    property<test_struct, propType, storageType, test_struct::test_get, test_struct::test_set, test_struct::test_const_get> x;

test_struct(propType _x, storageType offset) : _offset(offset),  x(_x, *this) {}

};

int main() {
    auto prop = property<void, propType, storageType, test_get, test_set, test_const_get>(0xFF);
    size_t size = sizeof(prop);
    prop = 5;
    const auto& constProp = prop;
    test_struct ts(3, 1);
    ts.x = 31;
    auto defaultProp = property<void, propType, storageType, (decltype(&test_get))0, (decltype(&test_set))0, (decltype(&test_const_get))0>(0x11);
    return ((prop - size + constProp) << 8) + ts.x - defaultProp;
}