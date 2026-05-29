#pragma once

#include <coroutine>

template<typename T>
struct Generator {
    struct promise_type {
        T value;
        
        Generator get_return_object() {
            return Generator{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(T val) {
            value = val;
            return {};
        }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };
    
    struct iterator {
        std::coroutine_handle<promise_type> handle;
        bool done;
        
        iterator(std::coroutine_handle<promise_type> h, bool d) : handle(h), done(d) {
            if (!done) {
                handle.resume();
                done = handle.done();
            }
        }
        
        iterator& operator++() {
            if (!done) {
                handle.resume();
                done = handle.done();
            }
            return *this;
        }
        
        bool operator!=(const iterator& other) const {
            return done != other.done;
        }
        
        T operator*() const {
            return handle.promise().value;
        }
    };
    
    std::coroutine_handle<promise_type> handle;
    
    Generator(std::coroutine_handle<promise_type> h) : handle(h) {}
    ~Generator() { if (handle) handle.destroy(); }
    
    iterator begin() { return iterator{handle, false}; }
    iterator end() { return iterator{handle, true}; }
};
