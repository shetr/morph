
#ifndef MORPH_EVENT_SYSTEM_HPP
#define MORPH_EVENT_SYSTEM_HPP

#include "Types.hpp"

namespace Morph {

class IEventID
{
private:
    uint m_id;
	bool m_attached;
public:
    IEventID()
        : m_id(0), m_attached(false) {}

    inline bool IsAttached() { 
        return m_attached;
    }
    inline void SetID(uint id) { 
        m_id = id; 
		m_attached = true;
    }
    inline void MarkDetached(){
		m_attached = false;
    }
    inline uint GetID() { 
        return m_id; 
    }
};

template<typename Event>
class IEventHandler : public IEventID
{
public:
    virtual void OnEvent(const Event& event) = 0;

};

class EventHandlers
{
private:
    uint m_nextID;
    unord_set<uint> m_freeIDs;
    unord_map<uint, void*> m_handlers;
public:
    EventHandlers() : m_nextID(0) {}
    inline uint Add(void* handler) {
        uint id;
        if(m_freeIDs.empty()) {
            id = m_nextID++;
        } else {
            id = *m_freeIDs.begin();
            m_freeIDs.erase(id);
        }
        m_handlers.insert({id, handler});
        return id;
    }
    inline void Remove(uint id) {
        m_handlers.erase(id);
        m_freeIDs.insert(id);
    }
    inline unord_map<uint, void*>::iterator begin() { return m_handlers.begin(); }
    inline unord_map<uint, void*>::iterator end() { return m_handlers.end(); }
    inline unord_map<uint, void*>::const_iterator begin() const { return m_handlers.begin(); }
    inline unord_map<uint, void*>::const_iterator end() const { return m_handlers.end(); }
};

template<typename Event, typename Listener>
class MethodHandler : public IEventHandler<Event>
{
    typedef void (Listener::*Handler) (const Event&);
private:
    Handler m_handler;
    Listener* m_listener;
public:
    MethodHandler() : m_listener(nullptr), m_handler(nullptr) {}
    MethodHandler(Listener* listener, Handler handler)
        : m_listener(listener), m_handler(handler) {}
    MethodHandler(MethodHandler&& other) 
        : m_handler(std::move(other.m_handler)), m_listener(std::move(other.m_listener)) {
        other.m_handler = nullptr;
        other.m_listener = nullptr;
    }
    MethodHandler& operator=(MethodHandler&& other) {
        m_handler = std::move(other.m_handler);
        m_listener = std::move(other.m_listener);
        other.m_handler = nullptr;
        other.m_listener = nullptr;
        return *this;
    }
    operator bool() const {
        return m_handler;
    }
    
    inline void OnEvent(const Event& event) override { 
        (m_listener->*m_handler)(event); 
    }
};

template<typename Event>
class FunctionHandler : public IEventHandler<Event>
{
private:
    function<void, const Event&> m_handler;
public:
    FunctionHandler() {}
    FunctionHandler(function<void, const Event&> handler)
        : m_handler(handler) {}
    FunctionHandler(FunctionHandler&& other) 
        : m_handler(std::move(other.m_handler)) {
        other.m_handler = nullptr;
    }
    FunctionHandler& operator=(FunctionHandler&& other) {
        m_handler = std::move(other.m_handler);
        other.m_handler = nullptr;
        return *this;
    }
    operator bool() const {
        return static_cast<bool>(m_handler);
    }
    
    inline void OnEvent(const Event& event) override { 
        if(m_handler) m_handler(event);
    }
};

template<typename Event>
class IEventSystem
{
public:
    virtual void Attach(IEventHandler<Event>* handler) = 0;

    virtual void Detach(IEventHandler<Event>* handler) = 0;
};

#define MORPH_EVENT_ACTIONS(eventType, eventSystem) \
    inline void Attach(IEventHandler<eventType>* handler) override { \
        eventSystem.Attach(handler); \
    } \
    inline void Detach(IEventHandler<eventType>* handler) override { \
        eventSystem.Detach(handler); \
    }

template<typename Event>
class Attacher
{
private:
    IEventSystem<Event>* m_system;
    unique<IEventHandler<Event>> m_handler;
public:
    Attacher() : m_system(nullptr) {}
    Attacher(IEventSystem<Event>* system, unique<IEventHandler<Event>>&& handler)
        : m_system(system), m_handler(std::move(handler))
    {
        m_system->Attach(m_handler.get());
    }
    Attacher(Attacher<Event>&& attacher) 
        : m_system(std::move(attacher.m_system)), m_handler(std::move(attacher.m_handler)) {
        attacher.m_system = nullptr;
        attacher.m_handler = nullptr;
    }
    Attacher& operator=(Attacher<Event>&& attacher) {
        m_system = std::move(attacher.m_system);
        m_handler = std::move(attacher.m_handler);
        attacher.m_system = nullptr;
        attacher.m_handler = nullptr;
        return *this;
    }
    ~Attacher() 
    {
        if(m_handler && m_handler->IsAttached()) {
            m_system->Detach(m_handler.get());
        }
    }
};

template<typename Event>
class FunctionAttacher
{
private:
    IEventSystem<Event>* m_system;
    unique<FunctionHandler<Event>> m_handler;
public:
    FunctionAttacher() : m_system(nullptr) {}
    FunctionAttacher(IEventSystem<Event>* system, function<void, const Event&> handler)
        : m_system(system), m_handler(std::make_unique<FunctionHandler<Event>>(handler))
    {
        m_system->Attach(m_handler.get());
    }
    FunctionAttacher(FunctionAttacher<Event>&& attacher) 
        : m_system(std::move(attacher.m_system)), m_handler(std::move(attacher.m_handler)) {
        attacher.m_system = nullptr;
        attacher.m_handler = nullptr;
    }
    FunctionAttacher<Event>& operator=(FunctionAttacher<Event>&& attacher) {
        m_system = std::move(attacher.m_system);
        m_handler = std::move(attacher.m_handler);
        attacher.m_system = nullptr;
        attacher.m_handler = nullptr;
        return *this;
    }
    ~FunctionAttacher() 
    {
        if(m_handler && m_handler->IsAttached()) {
            m_system->Detach(m_handler.get());
        }
    }
};

template<typename Event, typename Listener>
class MethodAttacher
{
    typedef void (Listener::*Handler) (const Event&);
private:
    IEventSystem<Event>* m_system;
    unique<MethodHandler<Event, Listener>> m_handler;
public:
    MethodAttacher() : m_system(nullptr) {}
    MethodAttacher(IEventSystem<Event>* system, Listener* listener, Handler handler)
        : m_system(system), m_handler(std::make_unique<MethodHandler<Event, Listener>>(listener, handler))
    {
        m_system->Attach(m_handler.get());
    }
    MethodAttacher(MethodAttacher<Event, Listener>&& attacher) 
        : m_system(std::move(attacher.m_system)), m_handler(std::move(attacher.m_handler)) {
        attacher.m_system = nullptr;
        attacher.m_handler = nullptr;
    }
    MethodAttacher<Event, Listener>& operator=(MethodAttacher<Event, Listener>&& attacher) {
        m_system = std::move(attacher.m_system);
        m_handler = std::move(attacher.m_handler);
        attacher.m_system = nullptr;
        attacher.m_handler = nullptr;
        return *this;
    }
    ~MethodAttacher() 
    {
        if(m_handler && m_handler->IsAttached()) {
            m_system->Detach(m_handler.get());
        }
    }
};

class EventSystem
{
private:
    unord_map<type_index, EventHandlers> m_handlersMap;
public:
    EventSystem() : m_handlersMap() {}
    ~EventSystem() {
        for(pair<type_index, EventHandlers> handler_pair: m_handlersMap){
            EventHandlers& handlers = handler_pair.second;
            for(pair<uint, void*> id_pair: handlers) {
                static_cast<IEventID*>(id_pair.second)->MarkDetached();
            }
        }
    }

    template<typename Event>
    inline void Attach(IEventHandler<Event>* handler)
    {
        type_index typeID = type_index(typeid(Event));
        auto it = m_handlersMap.find(typeID);
        if(it == m_handlersMap.end()) {
            m_handlersMap.insert({typeID, {}});
        }
        EventHandlers& handlers = m_handlersMap[typeID];
        uint id = handlers.Add(handler);
        handler->SetID(id);
    }
    template<typename Event>
    inline void Detach(IEventHandler<Event>* handler)
    {
        type_index typeID = type_index(typeid(Event));
        auto it = m_handlersMap.find(typeID);
        if(it == m_handlersMap.end()) {
            m_handlersMap.insert({typeID, {}});
        }
        EventHandlers& handlers = m_handlersMap[typeID];
        handlers.Remove(handler->GetID());
        handler->MarkDetached();
    }
    template<typename Event>
    inline void Execute(const Event& event)
    {
        type_index typeID = type_index(typeid(Event));
        auto it = m_handlersMap.find(typeID);
        if(it != m_handlersMap.end()) {
            EventHandlers& handlers = it->second;
            for(pair<uint, void*> p: handlers) {
                static_cast<IEventHandler<Event>*>(p.second)->OnEvent(event);
            }
        }
    }
};

}

#endif // MORPH_EVENT_SYSTEM_HPP