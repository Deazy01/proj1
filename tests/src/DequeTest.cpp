#include <gtest/gtest.h>
#include "Deque.h"
#include "DequeFactory.h"
#include <iostream>

template <typename T>
class DequeTest : public ::testing::Test {
    protected:
        std::unique_ptr<CDeque> DDeque = SDequeFactory<T>::Create();
};


using GDequeImplementations = ::testing::Types<CMaxSizeDeque, CVariableSizeDeque>;


TYPED_TEST_SUITE(DequeTest, GDequeImplementations);

TYPED_TEST(DequeTest, EmptyTest) {
    EXPECT_EQ(this->DDeque->Size(),0);
    EXPECT_GT(this->DDeque->MaxSize(),0);
}

TYPED_TEST(DequeTest, SimplePushPop){
    EXPECT_EQ(this->DDeque->Size(),0);
    EXPECT_TRUE(this->DDeque->PushBack(7));
    EXPECT_EQ(this->DDeque->Size(),1);
    EXPECT_EQ(std::any_cast<int>(this->DDeque->Back()),7);
    EXPECT_TRUE(this->DDeque->PopBack());
    EXPECT_EQ(this->DDeque->Size(),0);
    EXPECT_TRUE(this->DDeque->PushFront(34));
    EXPECT_EQ(this->DDeque->Size(),1);
    EXPECT_EQ(std::any_cast<int>(this->DDeque->Front()),34);
    EXPECT_TRUE(this->DDeque->PopFront());
    EXPECT_EQ(this->DDeque->Size(),0);
}

TYPED_TEST(DequeTest, FlowThrough){
    TDequeSize FlowSize = this->DDeque->MaxSize() && (this->DDeque->MaxSize() != GDequeSizeVariable) ? this->DDeque->MaxSize() - 2 : 1024;
    for(TDequeSize Index = 0; Index < FlowSize; Index++){
        EXPECT_EQ(this->DDeque->Size(),Index);
        EXPECT_TRUE(this->DDeque->PushBack(Index));
    }
    for(TDequeSize Index = 0; Index < FlowSize; Index++){
        ASSERT_EQ(this->DDeque->Size(),FlowSize - Index);
            EXPECT_EQ(std::any_cast<TDequeSize>(this->DDeque->Front()), Index);
            EXPECT_TRUE(this->DDeque->PopFront());
        }
    EXPECT_EQ(this->DDeque->Size(),0);
    for(TDequeSize Index = 0; Index < FlowSize; Index++){
        EXPECT_EQ(this->DDeque->Size(),Index);
        EXPECT_TRUE(this->DDeque->PushFront(Index));
    }
    for(TDequeSize Index = 0; Index < FlowSize; Index++){
        EXPECT_EQ(this->DDeque->Size(),FlowSize - Index);
        EXPECT_EQ(std::any_cast<TDequeSize>(this->DDeque->Back()), Index);
        EXPECT_TRUE(this->DDeque->PopBack());
    }
    EXPECT_EQ(this->DDeque->Size(),0);
}

TYPED_TEST(DequeTest, LimitsTest){
    EXPECT_FALSE(this->DDeque->PopBack());
    EXPECT_FALSE(this->DDeque->PopFront());
    if(this->DDeque->MaxSize() != GDequeSizeVariable){
        TDequeSize ExpectedSize = 0;
        while(this->DDeque->Size() < this->DDeque->MaxSize()){
            EXPECT_EQ(this->DDeque->Size(),ExpectedSize);
            EXPECT_TRUE(this->DDeque->PushBack(ExpectedSize));
            ExpectedSize++;
        }
        EXPECT_FALSE(this->DDeque->PushBack(ExpectedSize));
        EXPECT_FALSE(this->DDeque->PushFront(ExpectedSize));
        while(this->DDeque->Size()){
            EXPECT_EQ(this->DDeque->Size(),ExpectedSize);
            ExpectedSize--;
            EXPECT_EQ(std::any_cast<TDequeSize>(this->DDeque->Back()),ExpectedSize);
            EXPECT_TRUE(this->DDeque->PopBack());
        }
    }
    
}
TYPED_TEST(DequeTest, MixedFrontBackOperations) {
    EXPECT_TRUE(this->DDeque->PushBack(10));
    EXPECT_TRUE(this->DDeque->PushBack(20));
    EXPECT_TRUE(this->DDeque->PushFront(5));
    EXPECT_TRUE(this->DDeque->PushFront(1));

    EXPECT_EQ(this->DDeque->Size(), 4);
    EXPECT_EQ(std::any_cast<int>(this->DDeque->Front()), 1);
    EXPECT_EQ(std::any_cast<int>(this->DDeque->Back()), 20);

    EXPECT_TRUE(this->DDeque->PopFront());
    EXPECT_EQ(std::any_cast<int>(this->DDeque->Front()), 5);

    EXPECT_TRUE(this->DDeque->PopBack());
    EXPECT_EQ(std::any_cast<int>(this->DDeque->Back()), 10);
}
TYPED_TEST(DequeTest, CircularWrapAround) {
    TDequeSize Capacity = this->DDeque->MaxSize();

    if (Capacity != GDequeSizeVariable) {
        for (TDequeSize Index = 0; Index < Capacity; Index++) {
            EXPECT_TRUE(this->DDeque->PushBack(Index));
        }

        for (TDequeSize Index = 0; Index < Capacity / 2; Index++) {
            EXPECT_TRUE(this->DDeque->PopFront());
        }

        for (TDequeSize Index = 0; Index < Capacity / 2; Index++) {
            EXPECT_TRUE(this->DDeque->PushBack(Capacity + Index));
        }

        EXPECT_EQ(this->DDeque->Size(), Capacity);
        EXPECT_EQ(
            std::any_cast<TDequeSize>(this->DDeque->Front()),
            Capacity / 2
        );
        EXPECT_EQ(
            std::any_cast<TDequeSize>(this->DDeque->Back()),
            Capacity + Capacity / 2 - 1
        );
    }
}

TYPED_TEST(DequeTest, VariableDequeGrowth) {
    if (this->DDeque->MaxSize() == GDequeSizeVariable) {
        for (TDequeSize Index = 0; Index < 100; Index++) {
            EXPECT_TRUE(this->DDeque->PushBack(Index));
        }

        EXPECT_EQ(this->DDeque->Size(), 100);

        for (TDequeSize Index = 0; Index < 100; Index++) {
            ASSERT_EQ(this->DDeque->Size(), 100 - Index);
            EXPECT_EQ(
                std::any_cast<TDequeSize>(this->DDeque->Front()),
                Index
            );
            EXPECT_TRUE(this->DDeque->PopFront());
        }

        EXPECT_EQ(this->DDeque->Size(), 0);
    }
}

TYPED_TEST(DequeTest, GrowthAfterFrontMovement) {
    if (this->DDeque->MaxSize() == GDequeSizeVariable) {
        for (TDequeSize Index = 0; Index < 8; Index++) {
            EXPECT_TRUE(this->DDeque->PushBack(Index));
        }

        for (TDequeSize Index = 0; Index < 4; Index++) {
            EXPECT_TRUE(this->DDeque->PopFront());
        }

        for (TDequeSize Index = 8; Index < 20; Index++) {
            EXPECT_TRUE(this->DDeque->PushBack(Index));
        }

        EXPECT_EQ(this->DDeque->Size(), 16);
        EXPECT_EQ(
            std::any_cast<TDequeSize>(this->DDeque->Front()),
            4
        );
        EXPECT_EQ(
            std::any_cast<TDequeSize>(this->DDeque->Back()),
            19
        );
    }
}

TYPED_TEST(DequeTest, DifferentAnyTypes) {
    EXPECT_TRUE(this->DDeque->PushBack(42));
    EXPECT_TRUE(this->DDeque->PushBack(3.14));
    EXPECT_TRUE(this->DDeque->PushBack(std::string("hello")));

    EXPECT_EQ(std::any_cast<int>(this->DDeque->Front()), 42);

    EXPECT_TRUE(this->DDeque->PopFront());

    EXPECT_EQ(
        std::any_cast<double>(this->DDeque->Front()),
        3.14
    );

    EXPECT_TRUE(this->DDeque->PopFront());

    EXPECT_EQ(
        std::any_cast<std::string>(this->DDeque->Front()),
        "hello"
    );
}

TYPED_TEST(DequeTest, ZeroCapacityDeque) {
    if constexpr (std::is_same_v<TypeParam, CMaxSizeDeque>) {
        TypeParam deque(0);

        EXPECT_EQ(deque.Size(), 0);
        EXPECT_EQ(deque.MaxSize(), 0);
        EXPECT_FALSE(deque.PushBack(1));
        EXPECT_FALSE(deque.PushFront(2));
        EXPECT_FALSE(deque.PopBack());
        EXPECT_FALSE(deque.PopFront());
        EXPECT_FALSE(deque.Front().has_value());
        EXPECT_FALSE(deque.Back().has_value());
    }
}
TYPED_TEST(DequeTest, EmptyDequeOperations) {
    EXPECT_EQ(this->DDeque->Size(), 0);
    EXPECT_FALSE(this->DDeque->Front().has_value());
    EXPECT_FALSE(this->DDeque->Back().has_value());
    EXPECT_FALSE(this->DDeque->PopFront());
    EXPECT_FALSE(this->DDeque->PopBack());
    EXPECT_EQ(this->DDeque->Size(), 0);
}
TYPED_TEST(DequeTest, WrapAroundThenGrowth) {
    for (int index = 0; index < 8; index++) {
        EXPECT_TRUE(this->DDeque->PushBack(index));
    }

    EXPECT_TRUE(this->DDeque->PopFront());
    EXPECT_TRUE(this->DDeque->PopFront());

    for (int index = 8; index < 12; index++) {
        EXPECT_TRUE(this->DDeque->PushBack(index));
    }

    EXPECT_EQ(this->DDeque->Size(), 10);
    EXPECT_EQ(std::any_cast<int>(this->DDeque->Front()), 2);
    EXPECT_EQ(std::any_cast<int>(this->DDeque->Back()), 11);
}
