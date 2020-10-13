// Copyright 2020 Petr Portnov <gh@progrm-jarvis.ru>

#ifndef INCLUDE_SIMPLE_STACK_HPP_
#define INCLUDE_SIMPLE_STACK_HPP_

#include <cstddef>
#include <type_traits>
#include <utility>

namespace simple_stack {

    template <typename Value_T>
    class Stack final {
    public:
        typedef Value_T value_t;
        typedef value_t& reference_t;
        typedef value_t const& const_reference_t;
        typedef value_t& rvalue_reference_t;
        typedef value_t* pointer_t;
        typedef value_t const* const_pointer_t;

        Stack();

        ~Stack();

        Stack(Stack const&) = delete;

        Stack& operator=(Stack const&) = delete;

        Stack(Stack&&) noexcept;

        Stack& operator=(Stack&& original) noexcept;

        [[nodiscard]] ::std::size_t size() const;

        [[nodiscard]] const_reference_t head() const;

        void push(rvalue_reference_t value);

        void push(const_reference_t value);

        void pop();

        template <typename... Args_T>
        void push_emplace(Args_T&&... args);

    private:
        struct Node {
            value_t value;
            pointer_t previous, next;
        };

        ::std::size_t size_;
        Node *tail_, *head_;
    };

    template <typename Value_T>
    Stack<Value_T>::Stack() : size_(0), tail_(nullptr), head_(nullptr) {}

    template <typename Value_T>
    Stack<Value_T>::~Stack() {
        auto current = tail_;
        while (current) {
            auto next = current->next;
            delete current;
            current = next;
        }
    }

    template <typename Value_T>
    Stack<Value_T>::Stack(Stack&& original) noexcept
        : size_(::std::exchange(original.size_, 0)),
          tail_(::std::exchange(original.tail_), nullptr),
          head_(::std::exchange(original.head_, nullptr)) {}

    template <typename Value_T>
    Stack<Value_T>& Stack<Value_T>::operator=(Stack&& original) noexcept {
        if (&original != this) {
            ::std::swap(size_, original.size_);
            ::std::swap(tail_, original.tail_);
            ::std::swap(head_, original.head_);
        }

        return *this;
    }
} // namespace simple_stack

#endif // INCLUDE_SIMPLE_STACK_HPP_
