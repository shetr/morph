
#ifndef MORPH_IDENTIFICATION_HPP
#define MORPH_IDENTIFICATION_HPP

#include "Utils.hpp"

namespace Morph {

using ID = uint;

class Identifiable
{
private:
    ID m_id;
public:
    Identifiable(ID id) : m_id(id) {}
    const ID& id() const { return m_id; }
};

class IdGen
{
private:
    ID m_nextID;
    unord_set<ID> m_freeIDs;
public:
    IdGen() : m_nextID(0) {}
    inline ID Gen() {
        ID id;
        if(m_freeIDs.empty()) {
            id = m_nextID++;
        } else {
            id = *m_freeIDs.begin();
            m_freeIDs.erase(id);
        }
        return id;
    }
    inline void Remove(ID id) {
        m_freeIDs.insert(id);
    }
};

template<typename ValueType>
class Identificator
{
private:
    ID m_nextID;
    unord_set<ID> m_freeIDs;
    unord_map<ID, ValueType> m_storage;
public:
    Identificator() : m_nextID(0) {}
    inline ID Store(ValueType value) {
        ID id;
        if(m_freeIDs.empty()) {
            id = m_nextID++;
        } else {
            id = *m_freeIDs.begin();
            m_freeIDs.erase(id);
        }
        m_storage.insert_or_assign(id, value);
        return id;
    }
    inline bool Remove(ID id) {
        size_t removedCount = m_storage.erase(id);
        if(removedCount > 0) {
            m_freeIDs.insert(id);
            return true;
        }
        return false;
    }
    inline opt<ref<ValueType>> Find(ID id) {
        auto it = m_storage.find(id);
        if(it != m_storage.end()) {
            return it->second;
        }
        return {};
    }
    inline typename unord_map<ID, ValueType>::iterator begin() { return m_storage.begin(); }
    inline typename unord_map<ID, ValueType>::iterator end() { return m_storage.end(); }
    inline typename unord_map<ID, ValueType>::const_iterator begin() const { return m_storage.begin(); }
    inline typename unord_map<ID, ValueType>::const_iterator end() const { return m_storage.end(); }
};

}

#endif // MORPH_IDENTIFICATION_HPP