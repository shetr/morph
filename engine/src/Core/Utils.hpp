
#ifndef MORPH_UTILS_HPP
#define MORPH_UTILS_HPP

#include "Types.hpp"

namespace Morph {

#define MORPH_VOID(action) (([&]()->Void {action; return Void();})())

struct Void {};

template<typename T>
class IteratorWrap
{
private:
    ref<T> m_storage;
public:
    IteratorWrap(ref<T> storage) : m_storage(storage) {}

    typename T::iterator begin() { return m_storage.get().begin(); }
    typename T::iterator end() { return m_storage.get().end(); }
    typename T::const_iterator begin() const { return m_storage.get().begin(); }
    typename T::const_iterator end() const { return m_storage.get().end(); }
};

template<typename Key, typename Value>
class UnordMapIteratorWrap
{
private:
    ref<unord_map<Key, Value>> m_storage;
public:
    UnordMapIteratorWrap(ref<unord_map<Key, Value>> storage) : m_storage(storage) {}

    class Iterator {
    private:
        typename unord_map<Key, Value>::iterator m_it;
    public:
        Iterator(typename unord_map<Key, Value>::iterator it) : m_it(it) {}
        inline Iterator& operator++() {
            ++m_it;
            return *this;
        }
        inline const Value& operator*() const {
            const pair<Key, Value>& p = *m_it;
            return p.second; 
        }
        inline Value& operator*(){
            pair<Key, Value>& p = *m_it;
            return p.second; 
        }
    };

    Iterator begin() { return Iterator(m_storage.get().begin()); }
    Iterator end() { return Iterator(m_storage.get().end()); }
    const Iterator begin() const { return Iterator(m_storage.get().begin()); }
    const Iterator end() const { return Iterator(m_storage.get().end()); }
};

template<typename Key, typename Value>
class MapIteratorWrap
{
private:
    ref<map<Key, Value>> m_storage;
public:
    MapIteratorWrap(ref<map<Key, Value>> storage) : m_storage(storage) {}

    class Iterator {
    private:
        typename map<Key, Value>::iterator m_it;
    public:
        Iterator(typename map<Key, Value>::iterator it) : m_it(it) {}
        inline Iterator& operator++() {
            ++m_it;
            return *this;
        }
        inline const Value& operator*() const {
            const pair<Key, Value>& p = *m_it;
            return p.second; 
        }
        inline Value& operator*(){
            pair<Key, Value>& p = *m_it;
            return p.second; 
        }
    };

    Iterator begin() { return Iterator(m_storage.get().begin()); }
    Iterator end() { return Iterator(m_storage.get().end()); }
    const Iterator begin() const { return Iterator(m_storage.get().begin()); }
    const Iterator end() const { return Iterator(m_storage.get().end()); }
};

template<typename IteratorType>
class IterableMaker
{
private:
    IteratorType m_begin;
    IteratorType m_end;
public:
    IterableMaker(IteratorType a_begin, IteratorType a_end) : m_begin(a_begin), m_end(a_end) {}
    IteratorType begin() { return m_begin; }
    IteratorType end() { return m_end; }
    const IteratorType begin() const { return m_begin; }
    const IteratorType end() const { return m_end; }

    inline friend std::ostream& operator<<(std::ostream& os, const IterableMaker<IteratorType>& iterable) {
        for(IteratorType it = iterable.begin(); it != iterable.end();) {
            os << *it;
            ++it;
            if(it != iterable.end()) {
                os << ", ";
            }
        }
        return os;
    }
};

template<typename EnumType, typename ValueType = int>
class EnumMap
{
private:
    array<ValueType, enum_count<EnumType>()> m_enumToValue;
    unord_map<ValueType, EnumType> m_valueToEnum;
public:
    EnumMap(array<ValueType, enum_count<EnumType>()> values) : m_enumToValue(values) {
        for(int i = 0; i < enum_count<EnumType>(); ++i) {
            m_valueToEnum.insert({m_enumToValue[i], enum_value<EnumType>(i)});
        }
    }
    EnumMap(init_list<ValueType> init) {
        assert(init.size() == m_enumToValue.size());
        std::copy(init.begin(), init.end(), m_enumToValue.begin());
        for(int i = 0; i < enum_count<EnumType>(); ++i) {
            m_valueToEnum.insert({m_enumToValue[i], enum_value<EnumType>(i)});
        }
    }
    inline ValueType ToValue(EnumType e) { return m_enumToValue[enum_index(e).value()]; }
    inline EnumType ToEnum(ValueType v) { return m_valueToEnum[v]; }
};

template<typename EnumType, typename ValueType = uint>
class EnumBits
{
private:
    ValueType m_value;
public:
    EnumBits() : m_value() {}
    EnumBits(EnumType value) : m_value(static_cast<ValueType>(value)) {}

    operator ValueType() const { return m_value; }
    ValueType value() const { return m_value; }

    friend EnumBits operator|(const EnumBits& lBits, const EnumBits& rBits) {
        return EnumBits(lBits.m_value | rBits.m_value);
    }
    friend EnumBits operator&(const EnumBits& lBits, const EnumBits& rBits) {
        return EnumBits(lBits.m_value & rBits.m_value);
    }

    EnumBits& operator|=(const EnumBits& bits) {
        m_value |= bits.m_value;
        return *this;
    }
    EnumBits& operator&=(const EnumBits& bits) {
        m_value &= bits.m_value;
        return *this;
    }

    EnumBits operator~() const {
        return EnumBits(~m_value);
    }

private:
    EnumBits(ValueType value) : m_value(static_cast<ValueType>(value)) {}
};

template<typename EnumType, typename ValueType = uint>
EnumBits<EnumType, ValueType> bits(EnumType value) {
    return value;
}

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

template<typename VariantType, typename... HandlersTypes>
auto match(VariantType&& variant, HandlersTypes&&... handlers) {
    return std::visit(
        overloaded{std::forward<HandlersTypes>(handlers)...},
        std::forward<VariantType>(variant)
    );
}

template<typename FormatString, typename... Args>
inline void critical(const FormatString &fmt, const Args &... args) {
    spdlog::critical(fmt, args...);
}
template<typename FormatString, typename... Args>
inline void panic(const FormatString &fmt, const Args &... args) {
    spdlog::critical(fmt, args...);
    exit(EXIT_FAILURE);
}

template<typename ValueType, typename FormatString, typename... Args>
inline ValueType&& value_or_panic(opt<ValueType>&& optValue, const FormatString &fmt, const Args &... args) {
    if(!optValue) {
        panic(fmt, args...);
    }
    return std::move(optValue.value());
}

template<typename ValueType, typename ErrorType>
using Result = variant<ValueType, ErrorType>;

template<typename ValueType, typename ErrorType>
inline ValueType value_or_panic(Result<ValueType, ErrorType>&& res) {
    if(ErrorType* pres = std::get_if<ErrorType>(&res)) {
        panic("{}", *pres);
        return ValueType();
    }
    return std::move(std::get<ValueType>(res));
}

template<typename ClassType, typename MemberType>
class MemberOffset
{
private:
    usize m_offset;
private:
    MemberOffset(usize offset) : m_offset(offset) {}
public:
    operator usize() const { return m_offset; }
    static MemberOffset Get(MemberType ClassType::*member) {
         return MemberOffset((char*)&((ClassType*)nullptr->*member) - (char*)nullptr);
    }
};

template<typename ClassType, typename MemberType> usize member_offset(MemberType ClassType::*member) {
    return (char*)&((ClassType*)nullptr->*member) - (char*)nullptr;
}

template<typename Key, typename Value>
Value find_or(unord_map<Key, Value> a_map, Key a_key, Value a_or_value) {
    auto mapIt = a_map.find(a_key);
    if(mapIt == a_map.end()) {
        return a_or_value;
    } else {
        return mapIt->second;
    }
}

}

template<typename T>
inline std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    for(Morph::usize i = 0; i < v.size(); ++i) {
        os << v[i] << (i + 1 == v.size() ? "" : ", ");
    }
    return os;
}

template<typename T, Morph::usize N>
inline std::ostream& operator<<(std::ostream& os, const std::array<T, N>& v) {
    for(Morph::usize i = 0; i < v.size(); ++i) {
        os << v[i] << (i + 1 == v.size() ? "" : ", ");
    }
    return os;
}

template<glm::length_t L, typename T, glm::qualifier Q>
inline std::ostream& operator<<(std::ostream& os, const glm::vec<L, T, Q>& v) {
    for(glm::length_t l = 0; l < L; ++l) {
        os << v[l] << (l + 1 == L ? "" : ", ");
    }
    return os;
}

template<glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline std::ostream& operator<<(std::ostream& os, const glm::mat<C, R, T, Q>& m) {
    for(glm::length_t r = 0; r < R; ++r) {
        os << "\n";
        for(glm::length_t c = 0; c < C; ++c) {
            os << m[c][r] << (c + 1 == C ? "" : ", ");
        }
    }
    return os;
}

#endif // MORPH_UTILS_HPP