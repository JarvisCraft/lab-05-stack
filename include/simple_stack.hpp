// Copyright 2020 Petr Portnov <gh@progrm-jarvis.ru>

#ifndef INCLUDE_SIMPLE_STACK_HPP_
#define INCLUDE_SIMPLE_STACK_HPP_

#include <cstddef>
#include <stdexcept>
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

        [[nodiscard]] bool empty() const;

        [[nodiscard]] ::std::size_t size() const;

        [[nodiscard]] const_reference_t head() const;

        void push(const_reference_t value);

        void push(rvalue_reference_t value);

        template <typename... Args_T>
        void push_emplace(Args_T&&... args);

        void pop();

    private:
        struct Node {
            value_t value;
            Node *previous, *next;
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

    template <typename Value_T>
    bool Stack<Value_T>::empty() const {
        return size_ == 0;
    }

    template <typename Value_T>
    ::std::size_t Stack<Value_T>::size() const {
        return size_;
    }

    template <typename Value_T>
    typename Stack<Value_T>::const_reference_t Stack<Value_T>::head() const {
        auto const head = head_;
        if (head == nullptr) throw ::std::out_of_range("attempt to call `head() const` on empty stack");

        return head_->value;
    }

    template <typename Value_T>
    void Stack<Value_T>::push(const_reference_t value) {
        static_assert(::std::is_copy_constructible<value_t>::value);

        if (size_ == 0) {
            tail_ = head_ = new Node{::std::move(value), nullptr, nullptr};
            size_ = 1;
        } else {
            head_ = new Node{::std::move(value), head_, nullptr};
            ++size_;
        }
    }

    template <typename Value_T>
    void Stack<Value_T>::push(rvalue_reference_t value) {
        static_assert(::std::is_move_constructible<value_t>::value);

        if (size_ == 0) {
            tail_ = head_ = new Node{value, nullptr, nullptr};
            size_ = 1;
        } else {
            auto const old_head = head_;
            old_head->next = head_ = new Node{value, head_, nullptr};
            ++size_;
        }
    }

    template <typename Value_T>
    template <typename... Args_T>
    void Stack<Value_T>::push_emplace(Args_T&&... args) {
        if (size_ == 0) {
            tail_ = head_ = new Node{Value_T{::std::forward<Args_T>(args)...}, nullptr, nullptr};
            size_ = 1;
        } else {
            auto const old_head = head_;
            old_head->next = head_ = new Node{Value_T{::std::forward<Args_T>(args)...}, head_, nullptr};
            ++size_;
        }
    }

    template <typename Value_T>
    void Stack<Value_T>::pop() {
        auto const size = size_;
        switch (size) {
            case 0: return; // no-op
            case 1: {
                delete head_;

                tail_ = head_ = nullptr;
                size_ = 0;

                return;
            }
            case 2: {
                delete head_;

                (head_ = tail_)->next = nullptr;
                size_ = 1;

                return;
            }
            default: {
                auto const old_head = head_;
                head_ = old_head->previous;
                delete old_head;

                size_ = size - 1;
            }
        }
    }
} // namespace simple_stack

#endif // INCLUDE_SIMPLE_STACK_HPP_
