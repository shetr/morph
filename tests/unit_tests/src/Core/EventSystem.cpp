
#include <gtest/gtest.h>

#include <Core/EventSystem.hpp>

using namespace Morph;

struct TestEvent
{
    int value;
};
struct TestEvent2
{
    int value;
};

class TestEventSystem : 
    public IEventSystem<TestEvent>,
    public IEventSystem<TestEvent2>
{
private:
    EventSystem m_eventSystem;
public:

    void SendValue(int a_value) {
        TestEvent event = {a_value};
        m_eventSystem.Execute(event);
    }
    void SendValue2(int a_value) {
        TestEvent2 event = {a_value};
        m_eventSystem.Execute(event);
    }

    MORPH_EVENT_ACTIONS(TestEvent, m_eventSystem)
    MORPH_EVENT_ACTIONS(TestEvent2, m_eventSystem)
};

class TestEventListener
{
private:
    MethodAttacher<TestEvent, TestEventListener>  m_testEventAttacher;
    int m_updatedValue = 0;
public:
    TestEventListener(TestEventSystem* a_eventSystem)
        : m_testEventAttacher(a_eventSystem, this, &TestEventListener::OnTestEvent)
    {}
    void OnTestEvent(const TestEvent& a_event) {
        m_updatedValue = a_event.value;
    }
    int value() const { return m_updatedValue; }
};


TEST(CoreEventSystem, MethodHandlerValueChange) {
    TestEventSystem testEventSystem;
    TestEventListener testEventListener(&testEventSystem);
    EXPECT_EQ(testEventListener.value(), 0);
    testEventSystem.SendValue(1);
    EXPECT_EQ(testEventListener.value(), 1);
    testEventSystem.SendValue(2);
    EXPECT_EQ(testEventListener.value(), 2);
}


TEST(CoreEventSystem, FunctionHandlerValueChange) {
    TestEventSystem testEventSystem;
    int value = 0;
    FunctionAttacher<TestEvent> handler(&testEventSystem, 
        [&](const TestEvent& a_event){ value = a_event.value; }
    );
    EXPECT_EQ(value, 0);
    testEventSystem.SendValue(1);
    EXPECT_EQ(value, 1);
    testEventSystem.SendValue(2);
    EXPECT_EQ(value, 2);
}

TEST(CoreEventSystem, MultipleHandlersValueChange) {
    TestEventSystem testEventSystem;
    TestEventListener testEventListener1(&testEventSystem);
    TestEventListener testEventListener2(&testEventSystem);
    int localValue = 0;
    FunctionAttacher<TestEvent> localHandler(&testEventSystem, 
        [&](const TestEvent& a_event){ localValue = a_event.value; }
    );
    EXPECT_EQ(testEventListener1.value(), 0);
    EXPECT_EQ(testEventListener2.value(), 0);
    EXPECT_EQ(localValue, 0);
    testEventSystem.SendValue(1);
    EXPECT_EQ(testEventListener1.value(), 1);
    EXPECT_EQ(testEventListener2.value(), 1);
    EXPECT_EQ(localValue, 1);
    testEventSystem.SendValue(2);
    EXPECT_EQ(testEventListener1.value(), 2);
    EXPECT_EQ(testEventListener2.value(), 2);
    EXPECT_EQ(localValue, 2);
}

TEST(CoreEventSystem, DetachedHandlerValueNotChange) {
    TestEventSystem testEventSystem;
    int value = 0;
    {
        FunctionAttacher<TestEvent> localHandler(&testEventSystem,
            [&](const TestEvent& a_event){ value = a_event.value; }
        );
        EXPECT_EQ(value, 0);
        testEventSystem.SendValue(1);
        EXPECT_EQ(value, 1);
    }
    testEventSystem.SendValue(2);
    EXPECT_EQ(value, 1);
}

TEST(CoreEventSystem, MultipleEventsValueChange) {
    TestEventSystem testEventSystem;
    int localValue1 = 0;
    int localValue2 = 1;
    FunctionAttacher<TestEvent> localHandler1(&testEventSystem, 
        [&](const TestEvent& a_event){ localValue1 = a_event.value; }
    );
    FunctionAttacher<TestEvent2> localHandler2(&testEventSystem, 
        [&](const TestEvent2& a_event){ localValue2 = a_event.value; }
    );
    EXPECT_EQ(localValue1, 0);
    EXPECT_EQ(localValue2, 1);
    testEventSystem.SendValue(2);
    EXPECT_EQ(localValue1, 2);
    EXPECT_EQ(localValue2, 1);
    testEventSystem.SendValue2(3);
    EXPECT_EQ(localValue1, 2);
    EXPECT_EQ(localValue2, 3);
    testEventSystem.SendValue(4);
    EXPECT_EQ(localValue1, 4);
    EXPECT_EQ(localValue2, 3);
    testEventSystem.SendValue2(5);
    EXPECT_EQ(localValue1, 4);
    EXPECT_EQ(localValue2, 5);
}