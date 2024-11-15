#include "gtest/gtest.h"
#include "gtest/gtest-spi.h"
#include "../modules/circular_buffer.hpp"

TEST(circular_buffer, constructors){
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

TEST(circular_buffer, at){
    CircularBuffer buf(7, 'a');
    buf.at(2) = 'b';
    EXPECT_TRUE(buf[2] == 'b');
}

TEST(circular_buffer, at_ERROR){
    CircularBuffer buf(3, ' ');
    EXPECT_ANY_THROW(buf.at(3));
}

TEST(circular_buffer, front_back){
    CircularBuffer buf(7, 100);
    EXPECT_TRUE(buf.back() == buf.front());
    buf.back() = 95;
    buf.front() = 107;
    EXPECT_TRUE(buf.back() == buf.front()-12);
}

TEST(circular_buffer, front_ERROR){
    CircularBuffer buf(0, 50);
    EXPECT_ANY_THROW(buf.front());
}

TEST(circular_buffer, back_ERROR){
    CircularBuffer buf(0, 50);
    EXPECT_ANY_THROW(value_type a = buf.back());
}

TEST(circular_buffer, push){
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

TEST(circular_buffer, push_ERROR){
    CircularBuffer buf(0);
    EXPECT_ANY_THROW(buf.push_back('a'));
}

TEST(circular_buffer, pop){
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

TEST(circular_buffer, pop_ERROR){
    CircularBuffer buf(4, 100);
    buf.pop_front();
    buf.pop_front();
    buf.pop_front();
    buf.pop_front();
    EXPECT_ANY_THROW(buf.pop_front());
}

TEST(circular_buffer, buffer_contents){
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
        EXPECT_FALSE(buf.empty());
        EXPECT_FALSE(buf.full());
        EXPECT_TRUE(buf.reserve() == 3);
        EXPECT_TRUE(buf.capacity() == 7);
    }
    {
        CircularBuffer buf(7);
        buf.push_back('d');
        buf.push_back('e');
        buf.push_front('c');
        buf.push_front('b');
        buf.push_front('a');
        EXPECT_TRUE(buf.size() == 5);
        EXPECT_TRUE(buf.reserve() == 2);
        EXPECT_TRUE(buf.capacity() == 7);
    }
    {
        CircularBuffer buf(5, 'a');
        EXPECT_TRUE(buf.size() == 5);
        EXPECT_FALSE(buf.empty());
        EXPECT_TRUE(buf.full());
        EXPECT_TRUE(buf.reserve() == 0);
        EXPECT_TRUE(buf.capacity() == 5);
    }
    {
        CircularBuffer buf(5);
        EXPECT_TRUE(buf.size() == 0);
        EXPECT_TRUE(buf.empty());
        EXPECT_FALSE(buf.full());
        EXPECT_TRUE(buf.reserve() == 5);
        EXPECT_TRUE(buf.capacity() == 5);
    }
}

TEST(circular_buffer, linearize){
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

TEST(circular_buffer, rotate){
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

TEST(circular_buffer, set_capacity){
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

TEST(circular_buffer, resize){
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

TEST(circular_buffer, assignment_op){
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

TEST(circular_buffer, swap){
    CircularBuffer buf1(5);
    for(int i=0; i<5; i++) buf1[i] = 'a'+i;
    CircularBuffer buf2(9);
    for(int i=0; i<9; i++) buf2[i] = '1'+i;
    buf1.swap(buf2);
    EXPECT_TRUE(buf1[5] == '6');
    EXPECT_TRUE(buf2[4] == 'e');
}

TEST(circular_buffer, erase){
    CircularBuffer buf(10, '1');
    buf.erase(4, 10);
    EXPECT_TRUE(buf[0] == '1');
    EXPECT_TRUE(buf[3] == '1');
    EXPECT_TRUE(buf[4] ==  0 );
    EXPECT_TRUE(buf[9] ==  0 );
}

TEST(circular_buffer, insert){
    CircularBuffer buf(10, 'e');
    buf.pop_back();
    buf.pop_back();
    buf.pop_back();
    buf.pop_front();
    buf.insert(5, 'W');
    EXPECT_TRUE(buf[5] == 'W');
    EXPECT_ANY_THROW(buf.insert(11, 'N'));
}

TEST(circular_buffer, clear){
    CircularBuffer buf(8, 'Q');
    EXPECT_TRUE(buf[3] == 'Q');
    buf.clear();
    EXPECT_TRUE(buf[3] == 0);
}

TEST(circular_buffer, eq_neq){
    CircularBuffer buf1(5, 'a');
    CircularBuffer buf2(5, 'a');
    EXPECT_TRUE(buf1 == buf2);
    buf2[2] = 'b';
    EXPECT_TRUE(buf1 != buf2);
}
