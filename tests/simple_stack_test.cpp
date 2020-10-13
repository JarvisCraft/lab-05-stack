// Copyright 2020 Petr Portnov <gh@progrm-jarvis.ru>

#include <gtest/gtest.h>

#include <simple_stack.hpp>
#include <string>

namespace simple_stack {

    struct Entry {
        int int_value;
        ::std::string string_value;

        bool operator==(Entry const& other) const {
            return int_value == other.int_value && string_value == other.string_value;
        }
    };

    TEST(ENTRY, equality_operator) {
        ASSERT_EQ((Entry{1, "foo"}), (Entry{1, "foo"}));
        ASSERT_FALSE((Entry{1, "foo"}) == (Entry{2, "foo"}));
        ASSERT_FALSE((Entry{1, "foo"}) == (Entry{1, "bar"}));
        ASSERT_FALSE((Entry{1, "foo"}) == (Entry{2, "bar"}));
    }

    template <typename T>
    inline static void const* discard_(T const& value) {
        return static_cast<void const*>(&value);
    }

    TEST(Test, discard_) {
        ASSERT_NO_THROW(discard_(123));
        ASSERT_NO_THROW(discard_(nullptr));
        ASSERT_NO_THROW(discard_(Entry{1, "omagad"}));
    }

    TEST(Stack, empty__empty) {
        const Stack<Entry> stack;
        EXPECT_EQ(stack.size(), 0);
    }

    TEST(Stack, empty__not_empty) {
        Stack<Entry> stack;
        EXPECT_TRUE(stack.empty());

        EXPECT_NO_THROW(stack.push({123, "hello"}));
        EXPECT_FALSE(stack.empty());

        EXPECT_NO_THROW(stack.push({456, "world"}));
        EXPECT_FALSE(stack.empty());

        EXPECT_NO_THROW(stack.push({3478, "!!!"}));
        EXPECT_FALSE(stack.empty());

        EXPECT_NO_THROW(stack.pop());
        EXPECT_FALSE(stack.empty());

        EXPECT_NO_THROW(stack.pop());
        EXPECT_FALSE(stack.empty());

        EXPECT_NO_THROW(stack.pop());
        EXPECT_TRUE(stack.empty());
    }

    TEST(Stack, size__empty) {
        const Stack<Entry> stack;
        EXPECT_EQ(0, stack.size());
    }

    TEST(Stack, size__not_empty) {
        Stack<Entry> stack;
        EXPECT_EQ(stack.size(), 0);

        EXPECT_NO_THROW(stack.push({123, "hello"}));
        EXPECT_EQ(stack.size(), 1);

        EXPECT_NO_THROW(stack.push({456, "world"}));
        EXPECT_EQ(stack.size(), 2);

        EXPECT_NO_THROW(stack.push({3478, "!!!"}));
        EXPECT_EQ(stack.size(), 3);

        EXPECT_NO_THROW(stack.pop());
        EXPECT_EQ(stack.size(), 2);

        EXPECT_NO_THROW(stack.pop());
        EXPECT_EQ(stack.size(), 1);

        EXPECT_NO_THROW(stack.pop());
        EXPECT_EQ(stack.size(), 0);
    }

    TEST(Stack, push__const_ref) {
        Stack<Entry> stack;

        Entry value{12, "twelve"};
        EXPECT_NO_THROW(stack.push(value));
        EXPECT_EQ(stack.head(), (Entry{12, "twelve"}));

        value = {96, "ninety six"};
        EXPECT_NO_THROW(stack.push(value));
        EXPECT_EQ(stack.head(), (Entry{96, "ninety six"}));

        value = {0xCAFE, "Ja"};
        EXPECT_NO_THROW(stack.push(value));
        EXPECT_EQ(stack.head(), (Entry{0xCAFE, "Ja"}));

        value = {0xBABE, "Va"};
        EXPECT_NO_THROW(stack.push(value));
        EXPECT_EQ(stack.head(), (Entry{0xBABE, "Va"}));
    }

    TEST(Stack, push__rvalue_ref) {
        Stack<Entry> stack;

        EXPECT_NO_THROW(stack.push({12, "twelve"}));
        EXPECT_EQ(stack.head(), (Entry{12, "twelve"}));

        EXPECT_NO_THROW(stack.push({96, "ninety six"}));
        EXPECT_EQ(stack.head(), (Entry{96, "ninety six"}));

        EXPECT_NO_THROW(stack.push({0xCAFE, "Ja"}));
        EXPECT_EQ(stack.head(), (Entry{0xCAFE, "Ja"}));

        EXPECT_NO_THROW(stack.push({0xBABE, "Va"}));
        EXPECT_EQ(stack.head(), (Entry{0xBABE, "Va"}));
    }

    TEST(Stack, push_emplace) {
        Stack<Entry> stack;

        EXPECT_NO_THROW(stack.push_emplace(12, "twelve"));
        EXPECT_EQ(stack.head(), (Entry{12, "twelve"}));

        EXPECT_NO_THROW(stack.push_emplace(96, "ninety six"));
        EXPECT_EQ(stack.head(), (Entry{96, "ninety six"}));

        EXPECT_NO_THROW(stack.push_emplace(0xCAFE, "Ja"));
        EXPECT_EQ(stack.head(), (Entry{0xCAFE, "Ja"}));

        EXPECT_NO_THROW(stack.push_emplace(0xBABE, "Va"));
        EXPECT_EQ(stack.head(), (Entry{0xBABE, "Va"}));
    }

    TEST(Stack, pop__empty) {
        Stack<Entry> stack;
        EXPECT_NO_THROW(stack.pop()); // no-op
    }

    TEST(Stack, pop__not_empty) {
        Stack<Entry> stack;

        EXPECT_NO_THROW(stack.push({1, "somebody"}));
        EXPECT_NO_THROW(stack.pop());
        EXPECT_THROW(discard_(stack.head()), ::std::out_of_range);

        EXPECT_NO_THROW(stack.push({1, "somebody"}));

        EXPECT_NO_THROW(stack.push({3, "once"}));
        EXPECT_NO_THROW(stack.push({3, "told"}));
        EXPECT_NO_THROW(stack.push({7, "me"}));

        EXPECT_EQ(stack.head(), (Entry{7, "me"}));

        EXPECT_NO_THROW(stack.pop());
        EXPECT_EQ(stack.head(), (Entry{3, "told"}));

        EXPECT_NO_THROW(stack.pop());
        EXPECT_EQ(stack.head(), (Entry{3, "once"}));

        EXPECT_NO_THROW(stack.pop());
        EXPECT_EQ(stack.head(), (Entry{1, "somebody"}));

        EXPECT_NO_THROW(stack.pop()); // no-op
        EXPECT_THROW(discard_(stack.head()), ::std::out_of_range);
    }
} // namespace simple_stack
