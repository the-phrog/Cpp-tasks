#include "gtest/gtest.h"
#include "gtest/gtest-spi.h"
#include "../modules/circular_buffer.hpp"

TEST(constructors, default_constructor){
    CircularBuffer buf;
    EXPECT_ANY_THROW(buf.at(0));
}

TEST(constructors, by_capacity){
    CircularBuffer buf(10);
    EXPECT_NO_THROW(buf.at(0) = 15);
    EXPECT_NO_THROW(buf.at(9) = 20);
    EXPECT_ANY_THROW(buf.at(10) = 25);
}

TEST(constructors, by_capacity_and_item){
    CircularBuffer buf(8, 'Z');
    EXPECT_TRUE(buf[0] == 'Z');
    EXPECT_TRUE(buf[7] == 'Z');
    EXPECT_ANY_THROW(buf.at(8) == 'Z');
}

TEST(constructors, copying){
    CircularBuffer buf1;
    CircularBuffer buf2(5, 'a');
    buf2[2] = 'b';
    buf1 = buf2;
    EXPECT_TRUE(buf1[2] == 'b');
    buf2[3] = 'c';
    EXPECT_TRUE(buf1[3] != 'c');
}

TEST(constructors, destructor){
    CircularBuffer buf1(10, 'd');
    EXPECT_TRUE(buf1[8] == 'd');
    {
        EXPECT_TRUE(buf1[6] == 'd');
        CircularBuffer buf1;
        CircularBuffer buf2(5, 'a');
        buf2[2] = 'b';
        buf1 = buf2;
        EXPECT_TRUE(buf1[2] == 'b');
        buf2[3] = 'c';
        EXPECT_TRUE(buf1[3] != 'c');
    }
    EXPECT_TRUE(buf1[0] != 'a');
    EXPECT_TRUE(buf1[2] != 'b');
}

TEST(methods, at){
    CircularBuffer buf(7, 'a');
    buf.at(2) = 'b';
    EXPECT_TRUE(buf[2] == 'b');
}

TEST(methods, at_ERROR){
    CircularBuffer buf(3, ' ');
    EXPECT_ANY_THROW(buf.at(3));
}

TEST(methods, front_back){
    CircularBuffer buf(7, 100);
    EXPECT_TRUE(buf.back() == buf.front());
    buf.back() = 95;
    buf.front() = 107;
    EXPECT_TRUE(buf.back() == buf.front()-12);
}

TEST(methods, front_ERROR){
    CircularBuffer buf(0, 50);
    EXPECT_ANY_THROW(buf.front());
}

TEST(methods, back_ERROR){
    CircularBuffer buf(0, 50);
    EXPECT_ANY_THROW(value_type a = buf.back());
}

TEST(methods, push){
    CircularBuffer buf(5);
    buf.push_back('a');
    buf.push_back('b');
    buf.push_back('c');
    buf.push_back('d');
    buf.push_back('e');
    buf.push_back('f');
    EXPECT_TRUE(buf[0] == 'f');
    buf.push_front('A');
    EXPECT_TRUE(buf[0] == 'A');
}

TEST(methods, push_ERROR){
    CircularBuffer buf(0);
    EXPECT_ANY_THROW(buf.push_back('a'));
}

TEST(methods, pop){
    CircularBuffer buf(7);
    buf.push_back('a');
    buf.push_back('b');
    buf.push_back('c');
    buf.push_back('d');
    buf.push_back('e');
    buf.push_back('f');
    buf.push_back('g');
    buf.pop_back();
    buf.pop_back();
    buf.pop_back();
    buf.pop_back();
    buf.pop_front();
    buf.pop_front();
    EXPECT_TRUE(buf[1] == 0);
    EXPECT_TRUE(buf[2] == 'c');
    EXPECT_TRUE(buf[3] == 0);
    EXPECT_TRUE(buf.size() == 1);
}

TEST(methods, pop_ERROR){
    CircularBuffer buf(4, 100);
    buf.pop_front();
    buf.pop_front();
    buf.pop_front();
    buf.pop_front();
    EXPECT_ANY_THROW(buf.pop_front());
}

TEST(methods, size){
    {
        CircularBuffer buf(7);
        buf.push_back('a');
        buf.pop_front();
        buf.push_back('a');
        buf.pop_front();
        buf.push_back('a');
        buf.push_back('b');
        buf.push_back('c');
        buf.push_back('d');
        EXPECT_TRUE(buf.size() == 4);
    }
    {
        CircularBuffer buf(7);
        buf.push_back('d');
        buf.push_back('e');
        buf.push_front('c');
        buf.push_front('b');
        buf.push_front('a');
        EXPECT_TRUE(buf.size() == 5);
    }
    {
        CircularBuffer buf(5, 'a');
        EXPECT_TRUE(buf.size() == 5);
    }
    {
        CircularBuffer buf(5);
        EXPECT_TRUE(buf.size() == 0);
    }
}

TEST(methods, empty){
    {
        CircularBuffer buf(7);
        buf.push_back('a');
        buf.pop_front();
        buf.push_back('a');
        buf.pop_front();
        buf.push_back('a');
        buf.push_back('b');
        buf.push_back('c');
        buf.push_back('d');
        EXPECT_FALSE(buf.empty());
    }
    {
        CircularBuffer buf(5, 'a');
        EXPECT_FALSE(buf.empty());
    }
    {
        CircularBuffer buf(5);
        EXPECT_TRUE(buf.empty());
    }
}

TEST(methods, full){
    {
        CircularBuffer buf(7);
        buf.push_back('a');
        buf.pop_front();
        buf.push_back('a');
        buf.pop_front();
        buf.push_back('a');
        buf.push_back('b');
        buf.push_back('c');
        buf.push_back('d');
        EXPECT_FALSE(buf.full());
    }
    {
        CircularBuffer buf(5, 'a');
        EXPECT_TRUE(buf.full());
    }
    {
        CircularBuffer buf(5);
        EXPECT_FALSE(buf.full());
    }
}

TEST(methods, reserve){
    {
        CircularBuffer buf(7);
        buf.push_back('a');
        buf.pop_front();
        buf.push_back('a');
        buf.pop_front();
        buf.push_back('a');
        buf.push_back('b');
        buf.push_back('c');
        buf.push_back('d');
        EXPECT_TRUE(buf.reserve() == 3);
    }
    {
        CircularBuffer buf(7);
        buf.push_back('d');
        buf.push_back('e');
        buf.push_front('c');
        buf.push_front('b');
        buf.push_front('a');
        EXPECT_TRUE(buf.reserve() == 2);
    }
    {
        CircularBuffer buf(5, 'a');
        EXPECT_TRUE(buf.reserve() == 0);
    }
    {
        CircularBuffer buf(5);
        EXPECT_TRUE(buf.reserve() == 5);
    }
}

TEST(methods, capacity){
    {
        CircularBuffer buf(7);
        buf.push_back('a');
        buf.pop_front();
        buf.push_back('a');
        buf.pop_front();
        buf.push_back('a');
        buf.push_back('b');
        buf.push_back('c');
        buf.push_back('d');
        EXPECT_TRUE(buf.capacity() == 7);
    }
    {
        CircularBuffer buf(7);
        buf.push_back('d');
        buf.push_back('e');
        buf.push_front('c');
        buf.push_front('b');
        buf.push_front('a');
        EXPECT_TRUE(buf.capacity() == 7);
    }
    {
        CircularBuffer buf(5, 'a');
        EXPECT_TRUE(buf.capacity() == 5);
    }
    {
        CircularBuffer buf(5);
        EXPECT_TRUE(buf.capacity() == 5);
    }
}

TEST(methods, linearize){
    {
        CircularBuffer buf(7);
        buf.push_back(' ');
        buf.pop_front();
        buf.push_back(' ');
        buf.pop_front();
        buf.push_back('a');
        buf.push_back('b');
        buf.push_back('c');
        buf.push_back('d');
        EXPECT_FALSE(buf.is_linearized());
        value_type *arr = buf.linearize();
        EXPECT_TRUE(arr[0] == 'a');
        EXPECT_TRUE(arr[1] == 'b');
        EXPECT_TRUE(arr[2] == 'c');
        EXPECT_TRUE(arr[3] == 'd');
        EXPECT_TRUE(arr[4] ==  0 );
        EXPECT_TRUE(arr[6] ==  0 );
    }
    {
        CircularBuffer buf(7);
        buf.push_back('d');
        buf.push_back('e');
        buf.push_front('c');
        buf.push_front('b');
        buf.push_front('a');
        EXPECT_FALSE(buf.is_linearized());
        value_type *arr = buf.linearize();
        EXPECT_TRUE(arr[0] == 'a');
        EXPECT_TRUE(arr[1] == 'b');
        EXPECT_TRUE(arr[2] == 'c');
        EXPECT_TRUE(arr[3] == 'd');
        EXPECT_TRUE(arr[4] == 'e');
        EXPECT_TRUE(arr[5] ==  0 );
        EXPECT_TRUE(arr[6] ==  0 );
    }
    {
        CircularBuffer buf(5, '5');
        buf.pop_back();
        buf.pop_back();
        EXPECT_TRUE(buf.is_linearized());
        value_type *arr = buf.linearize();
        EXPECT_TRUE(arr[0] == '5');
        EXPECT_TRUE(arr[1] == '5');
        EXPECT_TRUE(arr[2] == '5');
        EXPECT_TRUE(arr[3] ==  0 );
        EXPECT_TRUE(arr[4] ==  0 );
    }
}

TEST(methods, rotate){
    {
        CircularBuffer buf(7);
        buf[0] = 'a';
        buf[1] = 'b';
        buf[2] = 'c';
        buf[3] = 'd';
        buf[4] = 'e';
        buf.rotate(3);
        EXPECT_TRUE(buf[0] == 'd');
        EXPECT_TRUE(buf[1] == 'e');
        EXPECT_TRUE(buf[2] ==  0 );
        EXPECT_TRUE(buf[3] ==  0 );
        EXPECT_TRUE(buf[4] == 'a');
        EXPECT_TRUE(buf[5] == 'b');
        EXPECT_TRUE(buf[6] == 'c');
        buf.rotate(2);
        EXPECT_TRUE(buf[0] ==  0 );
        EXPECT_TRUE(buf[1] ==  0 );
        EXPECT_TRUE(buf[2] == 'a');
        EXPECT_TRUE(buf[3] == 'b');
        EXPECT_TRUE(buf[4] == 'c');
        EXPECT_TRUE(buf[5] == 'd');
        EXPECT_TRUE(buf[6] == 'e');
    }
    {
        CircularBuffer buf(1, '1');
        buf.rotate(0);
        EXPECT_TRUE(buf[0] == '1');
    }
}

TEST(methods, set_capacity){
    CircularBuffer buf(7);
    buf.push_back('a');
    buf.push_back('b');
    buf.push_back('c');
    buf.push_back('d');
    buf.push_back('e');
    CircularBuffer buf1(buf);
    buf1.set_capacity(10);
    EXPECT_TRUE(buf1[7] == 0);
    EXPECT_TRUE(buf1[9] == 0);
    buf1.set_capacity(4);
    EXPECT_TRUE(buf1[3] == 'd');
    buf1.push_back('K');
    EXPECT_TRUE(buf1[0] == 'K');
    EXPECT_ANY_THROW(buf1.at(5));
}

TEST(methods, resize){
    CircularBuffer buf(7);
    buf.push_back('a');
    buf.push_back('b');
    buf.push_back('c');
    buf.push_back('d');
    buf.push_back('e');
    buf.resize(10, 'X');
    EXPECT_TRUE(buf[5] == 'X');
    EXPECT_TRUE(buf[9] == 'X');
    buf.resize(3);
    buf.push_back('Z');
    buf.push_front('Y');
    EXPECT_TRUE(buf[9] == 'Y');
    EXPECT_TRUE(buf[3] == 'Z');
}

TEST(operators, assignment_op){
    CircularBuffer buf1(8, 'a');
    buf1.push_front('c');
    buf1.push_front('b');
    CircularBuffer buf2;
    buf2 = buf1;
    EXPECT_TRUE(buf2.front() == 'b');
    CircularBuffer buf3(3, 's');
    buf3 = buf1;
    EXPECT_TRUE(buf3.front() == 'b');
}

TEST(methods, swap){
    CircularBuffer buf1(5);
    for(int i=0; i<5; i++) buf1[i] = 'a'+i;
    CircularBuffer buf2(9);
    for(int i=0; i<9; i++) buf2[i] = '1'+i;
    buf1.swap(buf2);
    EXPECT_TRUE(buf1[5] == '6');
    EXPECT_TRUE(buf2[4] == 'e');
}

TEST(methods, erase){
    CircularBuffer buf(10, '1');
    buf.erase(4, 10);
    EXPECT_TRUE(buf[0] == '1');
    EXPECT_TRUE(buf[3] == '1');
    EXPECT_TRUE(buf[4] ==  0 );
    EXPECT_TRUE(buf[9] ==  0 );
}

TEST(methods, insert){
    CircularBuffer buf(10, 'e');
    buf.pop_back();
    buf.pop_back();
    buf.pop_back();
    buf.pop_front();
    buf.insert(5, 'W');
    EXPECT_TRUE(buf[5] == 'W');
    EXPECT_ANY_THROW(buf.insert(11, 'N'));
}

TEST(methods, clear){
    CircularBuffer buf(8, 'Q');
    EXPECT_TRUE(buf[3] == 'Q');
    buf.clear();
    EXPECT_TRUE(buf[3] == 0);
}

TEST(operators, eq_neq){
    CircularBuffer buf1(5, 'a');
    CircularBuffer buf2(5, 'a');
    EXPECT_TRUE(buf1 == buf2);
    buf2[2] = 'b';
    EXPECT_TRUE(buf1 != buf2);
}
