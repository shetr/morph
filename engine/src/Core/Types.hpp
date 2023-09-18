
#ifndef MORPH_TYPES_HPP
#define MORPH_TYPES_HPP

#include "Common.hpp"

namespace Morph {

using uint = unsigned int;

template<typename T>
using unique = std::unique_ptr<T>;

template<typename T>
using shared = std::shared_ptr<T>;

template<typename T>
using weak = std::weak_ptr<T>;

template<typename T, std::size_t N>
using array = std::array<T, N>;

template<typename T>
using vector = std::vector<T>;

template<typename T>
using list = std::list<T>;

template<typename T>
using queue = std::queue<T>;

template<typename K, typename V>
using unord_map = std::unordered_map<K, V>;

template<typename K>
using unord_set = std::unordered_set<K>;

template<typename K, typename V>
using map = std::map<K, V>;

template<typename K>
using set = std::set<K>;

template<typename T>
using opt = std::optional<T>;

template<typename T, typename U>
using pair = std::pair<T, U>;

using type_index = std::type_index;

using string = std::string;

using any = std::any;

template <class... Types>
using variant = std::variant<Types...>;

template <class... Types>
using tuple = std::tuple<Types...>;

template<typename T>
using init_list = std::initializer_list<T>;

template<typename Res, typename... ArgTypes>
using function = std::function<Res(ArgTypes...)>;

//template< class T >
//using ref = std::reference_wrapper<T>;

template< class T >
class ref {
private:
    std::reference_wrapper<T> m_ref;
public:
    template< class U >
    ref(U&& x) : m_ref(x) {}
    ref(const ref& other) : m_ref(other.m_ref) {}
    ref& operator=(const ref& other) {
        m_ref = other.m_ref;
        return *this;
    }
    T& set(const T& other) {
        T& x = m_ref;
        x = other;
        return x;
    }
    operator T& () const { return m_ref; }
    T& get() const { return m_ref; }
    T& operator*() const { return m_ref.get(); }
    T* operator->() const { T& x = m_ref; return &x;}
};

template< class T >
using cref = ref<const T>;

using thread = std::thread;
using mutex = std::mutex;

using isize = long;
using usize = size_t;

using f32 = glm::f32;
using f64 = glm::f64;
using u8  = glm::u8;
using u16 = glm::u16;
using u32 = glm::u32;
using u64 = glm::u64;
using i8  = glm::i8;
using i16 = glm::i16;
using i32 = glm::i32;
using i64 = glm::i64;

using vec1 = glm::vec1;
using vec2 = glm::vec2;
using vec3 = glm::vec3;
using vec4 = glm::vec4;
using fvec1 = glm::fvec1;
using fvec2 = glm::fvec2;
using fvec3 = glm::fvec3;
using fvec4 = glm::fvec4;
using dvec1 = glm::dvec1;
using dvec2 = glm::dvec2;
using dvec3 = glm::dvec3;
using dvec4 = glm::dvec4;
using ivec1 = glm::ivec1;
using ivec2 = glm::ivec2;
using ivec3 = glm::ivec3;
using ivec4 = glm::ivec4;
using uvec1 = glm::uvec1;
using uvec2 = glm::uvec2;
using uvec3 = glm::uvec3;
using uvec4 = glm::uvec4;

using mat2 = glm::mat2;
using mat3 = glm::mat3;
using mat4 = glm::mat4;
using mat2x3 = glm::mat2x3;
using mat2x4 = glm::mat2x4;
using mat3x2 = glm::mat3x2;
using mat3x4 = glm::mat3x4;
using mat4x2 = glm::mat4x2;
using mat4x3 = glm::mat4x3;
using fmat2 = glm::fmat2;
using fmat3 = glm::fmat3;
using fmat4 = glm::fmat4;
using fmat2x3 = glm::fmat2x3;
using fmat2x4 = glm::fmat2x4;
using fmat3x2 = glm::fmat3x2;
using fmat3x4 = glm::fmat3x4;
using fmat4x2 = glm::fmat4x2;
using fmat4x3 = glm::fmat4x3;
using dmat2 = glm::dmat2;
using dmat3 = glm::dmat3;
using dmat4 = glm::dmat4;
using dmat2x3 = glm::dmat2x3;
using dmat2x4 = glm::dmat2x4;
using dmat3x2 = glm::dmat3x2;
using dmat3x4 = glm::dmat3x4;
using dmat4x2 = glm::dmat4x2;
using dmat4x3 = glm::dmat4x3;
using imat2 = glm::imat2x2;
using imat3 = glm::imat3x3;
using imat4 = glm::imat4x3;
using imat2x3 = glm::imat2x3;
using imat2x4 = glm::imat2x4;
using imat3x2 = glm::imat3x2;
using imat3x4 = glm::imat3x4;
using imat4x2 = glm::imat4x2;
using imat4x3 = glm::imat4x3;
using umat2 = glm::umat2x2;
using umat3 = glm::umat3x3;
using umat4 = glm::umat4x4;
using umat2x3 = glm::umat2x3;
using umat2x4 = glm::umat2x4;
using umat3x2 = glm::umat3x2;
using umat3x4 = glm::umat3x4;
using umat4x2 = glm::umat4x2;
using umat4x3 = glm::umat4x3;

using quat = glm::quat;

using namespace magic_enum;
using namespace nameof;

using namespace magic_enum::ostream_operators;

}

#endif // MORPH_TYPES_HPP