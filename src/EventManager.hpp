#ifndef __EVENTMANAGER_HPP__
#define __EVENTMANAGER_HPP__

#include <vector>
#include <winnt.h>

// 使用时需要注意线程安全问题，事件发起者不能同时来自多个线程
template<typename T>
class EventManager
{
public:
    typedef EventManager<T> ThisType;
    typedef void (*Callback)(ThisType *em, T arg);
    typedef void (*Callback0)();
    typedef void (*Callback1)(T arg);

    template<Callback0 callback>
    struct Adaptor0
    {
        static void handler(ThisType* em, T arg)
        {
            UNREFERENCED_PARAMETER(em);
            UNREFERENCED_PARAMETER(arg);
            return callback();
        }
    };

    template<Callback1 callback>
    struct Adaptor1
    {
        static void handler(ThisType* em, T arg)
        {
            UNREFERENCED_PARAMETER(em);
            return callback(arg);
        }
    };

    // 有条件的回调
    template<Callback callback, size_t A, size_t B = 0>
    struct Condition
    {
        static void handler(ThisType *em, T arg)
        {
            if (em->count() % A == B)
            {
                callback(em, arg);
            }
        }
    };

    // 接收方: 订阅一个事件
    template<Callback callback>
    inline void subscribe()
    {
        callbacks_.push_back(callback);
    }

    template<Callback0 callback>
    inline void subscribe()
    {
        callbacks_.push_back(Adaptor0<callback>::handler);
    }

    template<Callback1 callback>
    inline void subscribe()
    {
        callbacks_.push_back(Adaptor1<callback>::handler);
    }

    template<Callback callback, size_t A, size_t B>
    inline void subscribe()
    {
        callbacks_.push_back(Condition<callback, A, B>::handler);
    }

    template<Callback0 callback, size_t A, size_t B>
    inline void subscribe()
    {
        callbacks_.push_back(
            Condition<Adaptor0<callback>::handler, A, B>::handler
        );
    }

    template<Callback1 callback, size_t A, size_t B>
    inline void subscribe()
    {
        callbacks_.push_back(
            Condition<Adaptor1<callback>::handler, A, B>::handler
        );
    }

    // 发起方: 通知接收者
    void notify(T arg)
    {
        ++count_;
        for (size_t i = 0; i < callbacks_.size(); ++i)
        {
            callbacks_[i](this, arg);
        }
    }

    inline size_t count() { return count_; }

private:
    std::vector<Callback> callbacks_;
    size_t                count_;
};

#endif /* __EVENTMANAGER_HPP__ */
