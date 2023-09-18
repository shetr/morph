
#ifndef MORPH_ARRAYS_HPP
#define MORPH_ARRAYS_HPP

#include "Utils.hpp"

namespace Morph {

template<typename T, usize X, usize Y>
class array2d
{
    using array_type = array<T, X*Y>;
private:
    array_type m_data;
public:
    array2d() { fill(T()); }
    array2d(const T& v) { fill(v); }
    array2d(init_list<T> init) {
        assert(init.size() == m_data.size());
        std::copy(init.begin(), init.end(), m_data.begin());
    }
    inline usize index(uvec2 i) const { return i.x + i.y*X; }
    inline usize index(usize x, usize y) const { return x + y*X; }
    inline T& operator[](usize i) { return m_data[i]; }
    inline const T& operator[](usize i) const { return m_data[i]; }
    inline T& operator[](uvec2 i) { return m_data[index(i)]; }
    inline const T& operator[](uvec2 i) const { return m_data[index(i)]; }
    inline T& operator()(usize x, usize y) { return m_data[index(x, y)]; }
    inline const T& operator()(usize x, usize y) const { return m_data[index(x, y)]; }

    inline const T* data() const { return m_data.data(); }
    inline T* data() { return m_data.data(); }
    inline const array_type& flattend() const { return m_data; }
    inline array_type& flattend() { return m_data; }

    inline typename array_type::iterator begin() { return m_data.begin(); }
    inline typename array_type::iterator end() { return m_data.end(); }
    inline const typename array_type::iterator begin() const { return m_data.begin(); }
    inline const typename array_type::iterator end() const { return m_data.end(); }

    inline bool empty() const { return m_data.empty(); }
    inline usize size() const { return m_data.size(); }
    inline uvec2 dim() const { return uvec2(X, Y); }

    inline void fill(const T& v) { m_data.fill(v); }
    inline void swap(array2d<T, X, Y>& other) { m_data.swap(other.m_data); }
};


template<typename T, size_t X, size_t Y, size_t Z>
class array3d
{
    using array_type = array<T, X*Y*Z>;
private:
    array_type m_data;
public:
    array3d() { fill(T()); }
    array3d(const T& v) { fill(v); }
    array3d(init_list<T> init) {
        assert(init.size() == m_data.size());
        std::copy(init.begin(), init.end(), m_data.begin());
    }

    inline usize index(uvec3 i) const { return i.x + i.y*X + i.z*X*Y; }
    inline usize index(usize x, usize y, usize z) const { return x + y*X + z*X*Y; }
    inline T& operator[](usize i) { return m_data[i]; }
    inline const T& operator[](usize i) const { return m_data[i]; }
    inline T& operator[](uvec3 i) { return m_data[index(i)]; }
    inline const T& operator[](uvec3 i) const { return m_data[index(i)]; }
    inline T& operator()(usize x, usize y, usize z) { return m_data[index(x, y, z)]; }
    inline const T& operator()(usize x, usize y, usize z) const { return m_data[index(x, y, z)]; }

    inline const T* data() const { return m_data.data(); }
    inline T* data() { return m_data.data(); }
    inline const array_type& flattend() const { return m_data; }
    inline array_type& flattend() { return m_data; }

    inline typename array_type::iterator begin() { return m_data.begin(); }
    inline typename array_type::iterator end() { return m_data.end(); }
    inline const typename array_type::iterator begin() const { return m_data.begin(); }
    inline const typename array_type::iterator end() const { return m_data.end(); }

    inline bool empty() const { return m_data.empty(); }
    inline usize size() const { return m_data.size(); }
    inline uvec3 dim() const { return uvec3(X, Y, Z); }

    inline void fill(const T& v) { m_data.fill(v); }
    inline void swap(array3d<T, X, Y, Z>& other) { m_data.swap(other.m_data); }
};

template<typename T, size_t X, size_t Y, size_t Z, size_t W>
class array4d
{
    using array_type = array<T, X*Y*Z*W>;
private:
    array_type m_data;
public:
    array4d() { fill(T()); }
    array4d(const T& v) { fill(v); }
    array4d(init_list<T> init) {
        assert(init.size() == m_data.size());
        std::copy(init.begin(), init.end(), m_data.begin());
    }

    inline usize index(uvec4 i) const { return i.x + i.y*X + i.z*X*Y + i.w*X*Y*Z; }
    inline usize index(usize x, usize y, usize z, usize w) const { return x + y*X + z*X*Y + w*X*Y*Z; }
    inline T& operator[](usize i) { return m_data[i]; }
    inline const T& operator[](usize i) const { return m_data[i]; }
    inline T& operator[](uvec4 i) { return m_data[index(i)]; }
    inline const T& operator[](uvec4 i) const { return m_data[index(i)]; }
    inline T& operator()(usize x, usize y, usize z, usize w) { return m_data[index(x, y, z, w)]; }
    inline const T& operator()(usize x, usize y, usize z, usize w) const { return m_data[index(x, y, z, w)]; }

    inline const T* data() const { return m_data.data(); }
    inline T* data() { return m_data.data(); }
    inline const array_type& flattend() const { return m_data; }
    inline array_type& flattend() { return m_data; }

    inline typename array_type::iterator begin() { return m_data.begin(); }
    inline typename array_type::iterator end() { return m_data.end(); }
    inline const typename array_type::iterator begin() const { return m_data.begin(); }
    inline const typename array_type::iterator end() const { return m_data.end(); }

    inline bool empty() const { return m_data.empty(); }
    inline usize size() const { return m_data.size(); }
    inline uvec4 dim() const { return uvec4(X, Y, Z, W); }

    inline void fill(const T& v) { m_data.fill(v); }
    inline void swap(array4d<T, X, Y, Z, W>& other) { m_data.swap(other.m_data); }
};


template<typename T>
class vector2d
{
private:
    uvec2 m_dim;
    vector<T> m_data;
public:
    vector2d() : m_dim(uvec2(0,0)) {}
    vector2d(uvec2 a_dim, const T& v) : m_dim(a_dim), m_data(a_dim.x*a_dim.y, v) {}
    vector2d(uvec2 a_dim) : m_dim(a_dim), m_data(a_dim.x*a_dim.y) {}
    template<class InputIt>
    vector2d(uvec2 a_dim, InputIt first, InputIt last) : m_dim(a_dim), m_data(first, last) {
        assert(a_dim.x*a_dim.y == m_data.size());
    }
    vector2d(uvec2 a_dim, init_list<T> init) : m_dim(a_dim), m_data(init) {
        assert(a_dim.x*a_dim.y == m_data.size());
    }
    void assign(uvec2 a_dim, const T& v) {
        m_dim = a_dim;
        m_data.assign(m_dim.x*m_dim.y, v);
    }
    template<class InputIt>
    void assign(uvec2 a_dim, InputIt first, InputIt last ) {
        m_dim = a_dim;
        m_data.assign(first, last);
        assert(a_dim.x*a_dim.y == m_data.size());
    }
    void assign(uvec2 a_dim, init_list<T> init ) {
        m_dim = a_dim;
        m_data.assign(init);
        assert(a_dim.x*a_dim.y == m_data.size());
    }

    inline usize index(uvec2 i) const { return i.x + i.y*m_dim.x; }
    inline usize index(usize x, usize y) const { return x + y*m_dim.x; }
    inline T& operator[](usize i) { return m_data[i]; }
    inline const T& operator[](usize i) const { return m_data[i]; }
    inline T& operator[](uvec2 i) { return m_data[index(i)]; }
    inline const T& operator[](uvec2 i) const { return m_data[index(i)]; }
    inline T& operator()(usize x, usize y) { return m_data[index(x, y)]; }
    inline const T& operator()(usize x, usize y) const { return m_data[index(x, y)]; }

    inline const T* data() const { return m_data.data(); }
    inline T* data() { return m_data.data(); }
    inline const vector<T>& flattend() const { return m_data; }
    inline vector<T>& flattend() { return m_data; }

    inline typename vector<T>::iterator begin() { return m_data.begin(); }
    inline typename vector<T>::iterator end() { return m_data.end(); }
    inline const typename vector<T>::iterator begin() const { return m_data.begin(); }
    inline const typename vector<T>::iterator end() const { return m_data.end(); }

    inline bool empty() const { return m_data.empty(); }
    inline usize size() const { return m_data.size(); }
    inline uvec2 dim() const { return m_dim; }
    inline void reserve(usize cap) { m_data.reserve(cap); }
    inline void reserve(uvec2 a_dim) { m_data.reserve(a_dim.x*a_dim.y); }
    inline usize capacity() const { return m_data.capacity(); }
    inline void shrink_to_fit() { m_data.shrink_to_fit(); }

    inline void clear() {
        m_data.clear();
        m_dim = uvec2(0, 0);
    }
    inline void resize(uvec2 a_dim) {
        if(m_dim.x == a_dim.x) {
            m_data.resize(a_dim.x*a_dim.y);
        } else {
            vector<T> n_data(a_dim.x*a_dim.y);
            for(usize y = 0; y < a_dim.y; ++y) {
                if(y < m_dim.y) {
                    for(usize x = 0; x < a_dim.x; ++x) {
                        if(x < m_dim.x) {
                            std::swap(n_data[x + y*a_dim.x], m_data[x + y*m_dim.x]);
                        }
                    }
                }
            }
            n_data.swap(m_data);
        }
        m_dim = a_dim;
    }
    inline void resize(uvec2 a_dim, const T& v) {
        if(m_dim.x == a_dim.x) {
            m_data.resize(a_dim.x*a_dim.y, v);
        } else {
            vector<T> n_data(a_dim.x*a_dim.y, v);
            for(usize y = 0; y < a_dim.y; ++y) {
                if(y < m_dim.y) {
                    for(usize x = 0; x < a_dim.x; ++x) {
                        if(x < m_dim.x) {
                            std::swap(n_data[x + y*a_dim.x], m_data[x + y*m_dim.x]);
                        }
                    }
                }
            }
            n_data.swap(m_data);
        }
        m_dim = a_dim;
    }
    inline void swap(vector2d<T>& other) { 
        m_data.swap(other.m_data);
        std::swap(m_dim, other.m_dim);
    }
};

template<typename T>
class vector3d
{
private:
    uvec3 m_dim;
    vector<T> m_data;
public:
    vector3d() : m_dim(uvec3(0,0,0)) {}
    vector3d(uvec3 a_dim, const T& v) : m_dim(a_dim), m_data(a_dim.x*a_dim.y*a_dim.z, v) {}
    vector3d(uvec3 a_dim) : m_dim(a_dim), m_data(a_dim.x*a_dim.y*a_dim.z) {}
    template<class InputIt>
    vector3d(uvec3 a_dim, InputIt first, InputIt last) : m_dim(a_dim), m_data(first, last) {
        assert(a_dim.x*a_dim.y*a_dim.z == m_data.size());
    }
    vector3d(uvec3 a_dim, init_list<T> init) : m_dim(a_dim), m_data(init) {
        assert(a_dim.x*a_dim.y*a_dim.z == m_data.size());
    }
    void assign(uvec3 a_dim, const T& v) {
        m_dim = a_dim;
        m_data.assign(m_dim.x*m_dim.y*m_dim.z, v);
    }
    template<class InputIt>
    void assign(uvec3 a_dim, InputIt first, InputIt last ) {
        m_dim = a_dim;
        m_data.assign(first, last);
        assert(a_dim.x*a_dim.y*m_dim.z == m_data.size());
    }
    void assign(uvec3 a_dim, init_list<T> init ) {
        m_dim = a_dim;
        m_data.assign(init);
        assert(a_dim.x*a_dim.y*m_dim.z == m_data.size());
    }

    inline usize index(uvec3 i) const { return i.x + i.y*m_dim.x + i.z*m_dim.x*m_dim.y; }
    inline usize index(usize x, usize y, usize z) const { return x + y*m_dim.x + z*m_dim.x*m_dim.y; }
    inline T& operator[](usize i) { return m_data[i]; }
    inline const T& operator[](usize i) const { return m_data[i]; }
    inline T& operator[](uvec3 i) { return m_data[index(i)]; }
    inline const T& operator[](uvec3 i) const { return m_data[index(i)]; }
    inline T& operator()(usize x, usize y, usize z) { return m_data[index(x, y, z)]; }
    inline const T& operator()(usize x, usize y, usize z) const { return m_data[index(x, y, z)]; }

    inline const T* data() const { return m_data.data(); }
    inline T* data() { return m_data.data(); }
    inline const vector<T>& flattend() const { return m_data; }
    inline vector<T>& flattend() { return m_data; }

    inline typename vector<T>::iterator begin() { return m_data.begin(); }
    inline typename vector<T>::iterator end() { return m_data.end(); }
    inline const typename vector<T>::iterator begin() const { return m_data.begin(); }
    inline const typename vector<T>::iterator end() const { return m_data.end(); }

    inline bool empty() const { return m_data.empty(); }
    inline usize size() const { return m_data.size(); }
    inline uvec3 dim() const { return m_dim; }
    inline void reserve(usize cap) { m_data.reserve(cap); }
    inline void reserve(uvec3 a_dim) { m_data.reserve(a_dim.x*a_dim.y*a_dim.z); }
    inline usize capacity() const { return m_data.capacity(); }
    inline void shrink_to_fit() { m_data.shrink_to_fit(); }

    inline void clear() {
        m_data.clear();
        m_dim = uvec3(0, 0, 0);
    }
    inline void resize(uvec3 a_dim) {
        if(m_dim.x == a_dim.x && m_dim.y == a_dim.y) {
            m_data.resize(a_dim.x*a_dim.y*a_dim.z);
        } else {
            vector<T> n_data(a_dim.x*a_dim.y*a_dim.z);
            for(usize z = 0; z < a_dim.z; ++z) {
                if(z < m_dim.z) {
                    for(usize y = 0; y < a_dim.y; ++y) {
                        if(y < m_dim.y) {
                            for(usize x = 0; x < a_dim.x; ++x) {
                                if(x < m_dim.x) {
                                    std::swap(n_data[x + y*a_dim.x + z*a_dim.x*a_dim.y], m_data[x + y*m_dim.x + z*m_dim.x*m_dim.y]);
                                }
                            }
                        }
                    }
                }
            }
            n_data.swap(m_data);
        }
        m_dim = a_dim;
    }
    inline void resize(uvec3 a_dim, const T& v) {
        if(m_dim.x == a_dim.x && m_dim.y == a_dim.y) {
            m_data.resize(a_dim.x*a_dim.y*a_dim.z, v);
        } else {
            vector<T> n_data(a_dim.x*a_dim.y*a_dim.z, v);
            for(usize z = 0; z < a_dim.z; ++z) {
                if(z < m_dim.z) {
                    for(usize y = 0; y < a_dim.y; ++y) {
                        if(y < m_dim.y) {
                            for(usize x = 0; x < a_dim.x; ++x) {
                                if(x < m_dim.x) {
                                    std::swap(n_data[x + y*a_dim.x + z*a_dim.x*a_dim.y], m_data[x + y*m_dim.x + z*m_dim.x*m_dim.y]);
                                }
                            }
                        }
                    }
                }
            }
            n_data.swap(m_data);
        }
        m_dim = a_dim;
    }
    inline void swap(vector3d<T>& other) { 
        m_data.swap(other.m_data);
        std::swap(m_dim, other.m_dim);
    }
};

template<typename T>
class raw
{
private:
    usize m_count;
    T* m_data;
public:
    raw() : m_count(0), m_data(nullptr) {}
    raw(usize dataSize, void* data) : m_count(dataSize / sizeof(T)), m_data((T*)data) {}
    raw(usize count, T* data) : m_count(count), m_data(data) {}

    inline operator bool() const { return m_data; }

    inline T& operator[](usize i) { return m_data[i]; }
    inline const T& operator[](usize i) const { return m_data[i]; }

    inline const T* data() const { return m_data; }
    inline T* data() { return m_data; }

    inline T* begin() { return m_data; }
    inline T* end() { return m_data + m_count; }
    inline const T* begin() const { return m_data; }
    inline const T* end() const { return m_data + m_count; }

    inline usize size() const { return m_count * sizeof(T); }
    inline usize count() const { return m_count; }

    inline void swap(raw<T>& other) { 
        std::swap(m_data, other.m_data);
        std::swap(m_count, other.m_count);
    }
};

template<typename T>
class raw2d
{
private:
    uvec2 m_dim;
    T* m_data;
public:
    raw2d() : m_dim(uvec2(0)), m_data(nullptr) {}
    raw2d(uvec2 dim, T* data) : m_dim(dim), m_data(data) {}

    inline operator bool() const { return m_data; }

    inline usize index(uvec2 i) const { return i.x + i.y*m_dim.x; }
    inline usize index(usize x, usize y) const { return x + y*m_dim.x; }
    inline T& operator[](usize i) { return m_data[i]; }
    inline const T& operator[](usize i) const { return m_data[i]; }
    inline T& operator[](uvec2 i) { return m_data[index(i)]; }
    inline const T& operator[](uvec2 i) const { return m_data[index(i)]; }
    inline T& operator()(usize x, usize y) { return m_data[index(x, y)]; }
    inline const T& operator()(usize x, usize y) const { return m_data[index(x, y)]; }

    inline const T* data() const { return m_data; }
    inline T* data() { return m_data; }

    inline T* begin() { return m_data; }
    inline T* end() { return m_data + count(); }
    inline const T* begin() const { return m_data; }
    inline const T* end() const { return m_data + count(); }

    inline usize size() const { return count() * sizeof(T); }
    inline usize count() const { return m_dim.x * m_dim.y; }
    inline uvec2 dim() const { return m_dim; }

    inline void swap(raw2d<T>& other) { 
        std::swap(m_data, other.m_data);
        std::swap(m_dim, other.m_dim);
    }
};

template<typename T>
class raw3d
{
private:
    uvec3 m_dim;
    T* m_data;
public:
    raw3d() : m_dim(uvec3(0)), m_data(nullptr) {}
    raw3d(uvec3 dim, T* data) : m_dim(dim), m_data(data) {}

    inline operator bool() const { return m_data; }

    inline usize index(uvec3 i) const { return i.x + i.y*m_dim.x + i.z*m_dim.x*m_dim.y; }
    inline usize index(usize x, usize y, usize z) const { return x + y*m_dim.x + z*m_dim.x*m_dim.y; }
    inline T& operator[](usize i) { return m_data[i]; }
    inline const T& operator[](usize i) const { return m_data[i]; }
    inline T& operator[](uvec3 i) { return m_data[index(i)]; }
    inline const T& operator[](uvec3 i) const { return m_data[index(i)]; }
    inline T& operator()(usize x, usize y, usize z) { return m_data[index(x, y, z)]; }
    inline const T& operator()(usize x, usize y, usize z) const { return m_data[index(x, y, z)]; }

    inline const T* data() const { return m_data; }
    inline T* data() { return m_data; }

    inline T* begin() { return m_data; }
    inline T* end() { return m_data + count(); }
    inline const T* begin() const { return m_data; }
    inline const T* end() const { return m_data + count(); }

    inline usize size() const { return count() * sizeof(T); }
    inline usize count() const { return m_dim.x * m_dim.y * m_dim.z; }
    inline uvec3 dim() const { return m_dim; }

    inline void swap(raw3d<T>& other) { 
        std::swap(m_data, other.m_data);
        std::swap(m_dim, other.m_dim);
    }
};

template<typename T>
class raw4d
{
private:
    uvec4 m_dim;
    T* m_data;
public:
    raw4d() : m_dim(uvec4(0)), m_data(nullptr) {}
    raw4d(uvec4 dim, T* data) : m_dim(dim), m_data(data) {}

    inline operator bool() const { return m_data; }

    inline usize index(uvec4 i) const { return i.x + i.y*m_dim.x + i.z*m_dim.x*m_dim.y + i.w*m_dim.x*m_dim.y*m_dim.z; }
    inline usize index(usize x, usize y, usize z, usize w) const { return x + y*m_dim.x + z*m_dim.x*m_dim.y + w*m_dim.x*m_dim.y*m_dim.z; }
    inline T& operator[](usize i) { return m_data[i]; }
    inline const T& operator[](usize i) const { return m_data[i]; }
    inline T& operator[](uvec4 i) { return m_data[index(i)]; }
    inline const T& operator[](uvec4 i) const { return m_data[index(i)]; }
    inline T& operator()(usize x, usize y, usize z, usize w) { return m_data[index(x, y, z, w)]; }
    inline const T& operator()(usize x, usize y, usize z, usize w) const { return m_data[index(x, y, z, w)]; }

    inline const T* data() const { return m_data; }
    inline T* data() { return m_data; }

    inline T* begin() { return m_data; }
    inline T* end() { return m_data + count(); }
    inline const T* begin() const { return m_data; }
    inline const T* end() const { return m_data + count(); }

    inline usize size() const { return count() * sizeof(T); }
    inline usize count() const { return m_dim.x * m_dim.y * m_dim.z * m_dim.w; }
    inline uvec4 dim() const { return m_dim; }

    inline void swap(raw4d<T>& other) { 
        std::swap(m_data, other.m_data);
        std::swap(m_dim, other.m_dim);
    }
};

template<typename T>
class craw
{
private:
    usize m_count;
    const T* m_data;
public:
    craw() : m_count(0), m_data(nullptr) {}
    craw(usize dataSize, const void* data) : m_count(dataSize / sizeof(T)), m_data((const T*)data) {}
    craw(usize count, const T* data) : m_count(count), m_data(data) {}

    inline operator bool() const { return m_data; }

    inline const T& operator[](usize i) const { return m_data[i]; }

    inline const T* data() const { return m_data; }

    inline const T* begin() const { return m_data; }
    inline const T* end() const { return m_data + m_count; }

    inline usize size() const { return m_count * sizeof(T); }
    inline usize count() const { return m_count; }

    inline void swap(craw<T>& other) { 
        std::swap(m_data, other.m_data);
        std::swap(m_count, other.m_count);
    }
};

template<typename T>
class craw2d
{
private:
    uvec2 m_dim;
    const T* m_data;
public:
    craw2d() : m_dim(uvec2(0)), m_data(nullptr) {}
    craw2d(uvec2 dim, const T* data) : m_dim(dim), m_data(data) {}

    inline operator bool() const { return m_data; }

    inline usize index(uvec2 i) const { return i.x + i.y*m_dim.x; }
    inline usize index(usize x, usize y) const { return x + y*m_dim.x; }
    inline const T& operator[](usize i) const { return m_data[i]; }
    inline const T& operator[](uvec2 i) const { return m_data[index(i)]; }
    inline const T& operator()(usize x, usize y) const { return m_data[index(x, y)]; }

    inline const T* data() const { return m_data; }

    inline const T* begin() const { return m_data; }
    inline const T* end() const { return m_data + count(); }

    inline usize size() const { return count() * sizeof(T); }
    inline usize count() const { return m_dim.x * m_dim.y; }
    inline uvec2 dim() const { return m_dim; }

    inline void swap(craw2d<T>& other) { 
        std::swap(m_data, other.m_data);
        std::swap(m_dim, other.m_dim);
    }
};

template<typename T>
class craw3d
{
private:
    uvec3 m_dim;
    const T* m_data;
public:
    craw3d() : m_dim(uvec3(0)), m_data(nullptr) {}
    craw3d(uvec3 dim, const T* data) : m_dim(dim), m_data(data) {}

    inline operator bool() const { return m_data; }

    inline usize index(uvec3 i) const { return i.x + i.y*m_dim.x + i.z*m_dim.x*m_dim.y; }
    inline usize index(usize x, usize y, usize z) const { return x + y*m_dim.x + z*m_dim.x*m_dim.y; }
    inline const T& operator[](usize i) const { return m_data[i]; }
    inline const T& operator[](uvec3 i) const { return m_data[index(i)]; }
    inline const T& operator()(usize x, usize y, usize z) const { return m_data[index(x, y, z)]; }

    inline const T* data() const { return m_data; }

    inline const T* begin() const { return m_data; }
    inline const T* end() const { return m_data + count(); }

    inline usize size() const { return count() * sizeof(T); }
    inline usize count() const { return m_dim.x * m_dim.y * m_dim.z; }
    inline uvec3 dim() const { return m_dim; }

    inline void swap(craw3d<T>& other) { 
        std::swap(m_data, other.m_data);
        std::swap(m_dim, other.m_dim);
    }
};

template<typename T>
class craw4d
{
private:
    uvec4 m_dim;
    const T* m_data;
public:
    craw4d() : m_dim(uvec4(0)), m_data(nullptr) {}
    craw4d(uvec4 dim, const T* data) : m_dim(dim), m_data(data) {}

    inline operator bool() const { return m_data; }

    inline usize index(uvec4 i) const { return i.x + i.y*m_dim.x + i.z*m_dim.x*m_dim.y + i.w*m_dim.x*m_dim.y*m_dim.z; }
    inline usize index(usize x, usize y, usize z, usize w) const { return x + y*m_dim.x + z*m_dim.x*m_dim.y + w*m_dim.x*m_dim.y*m_dim.z; }
    inline const T& operator[](usize i) const { return m_data[i]; }
    inline const T& operator[](uvec4 i) const { return m_data[index(i)]; }
    inline const T& operator()(usize x, usize y, usize z, usize w) const { return m_data[index(x, y, z, w)]; }

    inline const T* data() const { return m_data; }

    inline const T* begin() const { return m_data; }
    inline const T* end() const { return m_data + count(); }

    inline usize size() const { return count() * sizeof(T); }
    inline usize count() const { return m_dim.x * m_dim.y * m_dim.z * m_dim.w; }
    inline uvec4 dim() const { return m_dim; }

    inline void swap(craw4d<T>& other) { 
        std::swap(m_data, other.m_data);
        std::swap(m_dim, other.m_dim);
    }
};

}

#endif // MORPH_ARRAYS_HPP