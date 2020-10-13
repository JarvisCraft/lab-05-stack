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
            Node *previous;
        };

        ::std::size_t size_;
        Node *head_;
    };

    template <typename Value_T>
    Stack<Value_T>::Stack() : size_(0),  head_(nullptr) {}

    template <typename Value_T>
    Stack<Value_T>::~Stack() {
        auto top = head_;
        while (top != nullptr) {
            auto next = top->previous;
            delete top;
            top = next;
        }
    }

    template <typename Value_T>
    Stack<Value_T>::Stack(Stack&& original) noexcept
        : size_(::std::exchange(original.size_, 0)),
          head_(::std::exchange(original.head_, nullptr)) {}

    template <typename Value_T>
    Stack<Value_T>& Stack<Value_T>::operator=(Stack&& original) noexcept {
        if (&original != this) {
            ::std::swap(size_, original.size_);
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

        head_ = new Node{value, head_};
        ++size_;
    }

    template <typename Value_T>
    void Stack<Value_T>::push(rvalue_reference_t value) {
        static_assert(::std::is_move_constructible<value_t>::value);

        head_ = new Node{::std::move(value), head_};
        ++size_;
    }

    template <typename Value_T>
    template <typename... Args_T>
    void Stack<Value_T>::push_emplace(Args_T&&... args) {
        head_ = new Node{Value_T{::std::forward<Args_T>(args)...}, head_};
        ++size_;
    }

    template <typename Value_T>
    void Stack<Value_T>::pop() {
        auto const old_head = head_;
        if (old_head == nullptr) return; // no-op

        auto new_head = old_head->previous;
        if (new_head == nullptr) { // size was 1
            head_ = nullptr;
            size_ = 0;
        } else {
            head_ = new_head;
            --size_;
        }
        delete old_head;
    }
} // namespace simple_stack

#endif // INCLUDE_SIMPLE_STACK_HPP_
