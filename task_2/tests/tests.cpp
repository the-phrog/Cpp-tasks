#include "gtest/gtest.h"
#include "gtest/gtest-spi.h"
#include "../modules/GameOfLife.hpp"
#include <cctype>

TEST(Keeper, default_constructor){
    Keeper k;
    EXPECT_EQ(k.getName(), "default");
}

TEST(Keeper, constructor_by_size_and_name){
    Keeper k(5, 10, "test_name");
    EXPECT_EQ(k.get().size(), 10);
    EXPECT_EQ(k.get()[0].size(), 5);
    EXPECT_EQ(k.getName(), "test_name");
}

// я не проверяю значения, выходящие за пределы вектора, по 
// той причине, что еще при чтении проводится проверка на
// выход за пределы массива, следовательно в него передаются
// уже отфильтрованные значения

TEST(Keeper, constructor_by_size_coordinates_and_name){
    Keeper k(5, 10, {{1, 1}, {4, 9}, {0, 0}, {2, 7}}, "universe");
    for(int y=0; y<10; y++)
        for(int x=0; x<5; x++){
            if((x==1 && y==1) || (x==4 && y==9) || (x==0 && y==0) || (x==2 && y == 7))
                EXPECT_TRUE(k[y][x]);
            else
                EXPECT_FALSE(k[y][x]);
        }
}

TEST(Keeper, copy_constructor){
    Keeper k1(5, 10, {{1, 1}, {4, 9}, {0, 0}, {2, 7}}, "universe");
    Keeper k2 = k1;
    EXPECT_EQ(k1.get(), k2.get());
    EXPECT_EQ(k1.getName(), k2.getName());
    for(int y=0; y<10; y++)
        for(int x=0; x<5; x++){
            if((x==1 && y==1) || (x==4 && y==9) || (x==0 && y==0) || (x==2 && y == 7))
                EXPECT_EQ(k1[y][x], k2[y][x]);
            else
                EXPECT_EQ(k1[y][x], k2[y][x]);
        }
}

TEST(Keeper, equality_operator){
    Keeper k1(5, 10, {{1, 1}, {4, 9}, {0, 0}, {2, 7}}, "universe");
    Keeper k2(5, 10, {{1, 1}, {4, 9}, {0, 0}, {2, 7}}, "universe1");
    EXPECT_EQ(k1, k2);
}

TEST(Keeper, inequality_operator){
    Keeper k1(5, 10, {{1, 1}, {4, 9}, {0, 0}, {2, 7}}, "universe");
    Keeper k2(5, 10, {{1, 1}, {4, 9}, {0, 1}, {2, 7}}, "universe");
    Keeper k3;
    EXPECT_NE(k1, k2);
    EXPECT_NE(k1, k3);
}

TEST(Keeper, assignment_operator){
    Keeper k1(5, 10, {{1, 1}, {4, 9}, {0, 0}, {2, 7}}, "universe");
    Keeper k2;
    k2 = k1;
    EXPECT_EQ(k1, k2);
}

TEST(Keeper, assignment_operator1){
    Keeper k1(5, 10, {{1, 1}, {4, 9}, {0, 0}, {2, 7}}, "universe");
    EXPECT_EQ(k1, k1);
}

TEST(Calculator, default_constructor){
    Calculator c;
    std::vector<bool> b({false, false, false, true, false, false, false, false, false});
    EXPECT_EQ(c.birth_rule(), b);
    std::vector<bool> s({false, false, true,  true, false, false, false, false, false});
    EXPECT_EQ(c.survival_rule(), s);
}

// ситуацию с некоррректными значениями и "9" не проверяю
// опять же потому, что при чтении файла регулярными выражениями
// неправильные значения отсеиваются и в конструктор поступают
// отфильтрованные списки

TEST(Calculator, constructor_by_lists){
    std::list<int> b({0, 8, 3});
    std::list<int> s({2, 4, 5, 6});
    Calculator c(s, b);
    for(int i=0; i<9; i++){
        if(i == 0 || i == 8 || i == 3)
            EXPECT_TRUE(c.birth_rule()[i]);
        else
            EXPECT_FALSE(c.birth_rule()[i]);
    }
    for(int i=0; i<9; i++){
        if(i == 2 || i == 4 || i == 5 || i == 6)
            EXPECT_TRUE(c.survival_rule()[i]);
        else
            EXPECT_FALSE(c.survival_rule()[i]);
    }
}

TEST(Calculator, constructor_by_vectors){
    std::vector<int> b({0, 8, 3});
    std::vector<int> s({2, 4, 5, 6});
    Calculator c(s, b);
    for(int i=0; i<9; i++){
        if(i == 0 || i == 8 || i == 3)
            EXPECT_TRUE(c.birth_rule()[i]);
        else
            EXPECT_FALSE(c.birth_rule()[i]);
    }
    for(int i=0; i<9; i++){
        if(i == 2 || i == 4 || i == 5 || i == 6)
            EXPECT_TRUE(c.survival_rule()[i]);
        else
            EXPECT_FALSE(c.survival_rule()[i]);
    }
}

TEST(Calculator, universe_test_1){
    Calculator c(std::list<int>({1, 2, 4}), std::list<int>({2, 3, 4}));
    Keeper k(5, 5, {{0,0},{2,0},{3,0},{4,0},{1,1},{1,2},{2,2},{0,3},
                    {1,3},{2,3},{3,3},{4,3},{3,4},{4,4}}, "test");
    Keeper k4(5, 5, {{1,0},{2,0},{3,0},{1,1},{2,1},{1,2},{2,2},{3,2},
        {0,3},{1,3},{3,3},{0,4},{1,4},{3,4},{4,4}}, "after_4_iterations");
    Keeper k0 = c.calculate(k, 4);
    EXPECT_EQ(k0, k4);
}

TEST(ArgumentParser, argument_permutation_1){
    char input[]  = "input.life";
    char output[] = "output.life";
    char iters[] = "20";
    int argc_ = 7;
    char* argv_[] = {"main", "-i", input, "-I", iters, "-o", output};
    ArgumentParser parser(argc_, argv_);
    EXPECT_EQ(parser.getInputFile(), input);
    EXPECT_EQ(parser.getOutputFile(), output);
    EXPECT_EQ(parser.getIterations(), std::stoi(iters));
}

TEST(ArgumentParser, argument_permutation_2){
    char input[]  = "input.life";
    char output[] = "output.life";
    char iters[] = "20";
    int argc_ = 7;
    char* argv_[] = {"main", "-I", iters, "-o", output, "-i", input};
    ArgumentParser parser(argc_, argv_);
    EXPECT_EQ(parser.getInputFile(), input);
    EXPECT_EQ(parser.getOutputFile(), output);
    EXPECT_EQ(parser.getIterations(), std::stoi(iters));
}

TEST(ArgumentParser, argument_permutation_3){
    char input[]  = "input.life";
    char output[] = "output.life";
    char iters[] = "20";
    int argc_ = 7;
    char* argv_[] = {"main", "-o", output, "-i", input, "-I", iters};
    ArgumentParser parser(argc_, argv_);
    EXPECT_EQ(parser.getInputFile(), input);
    EXPECT_EQ(parser.getOutputFile(), output);
    EXPECT_EQ(parser.getIterations(), std::stoi(iters));
}

TEST(ArgumentParser, one_letter_two_dashes_1){
    char input[]  = "input.life";
    char output[] = "output.life";
    char iters[] = "20";
    int argc_ = 7;
    char* argv_[] = {"main", "--i", input, "-I", iters, "-o", output};
    EXPECT_ANY_THROW(ArgumentParser parser(argc_, argv_));
}

TEST(ArgumentParser, one_letter_two_dashes_2){
    char input[]  = "input.life";
    char output[] = "output.life";
    char iters[] = "20";
    int argc_ = 7;
    char* argv_[] = {"main", "-i", input, "--I", iters, "-o", output};
    EXPECT_ANY_THROW(ArgumentParser parser(argc_, argv_));
}

TEST(ArgumentParser, one_letter_two_dashes_3){
    char input[]  = "input.life";
    char output[] = "output.life";
    char iters[] = "20";
    int argc_ = 7;
    char* argv_[] = {"main", "-i", input, "-I", iters, "--o", output};
    EXPECT_ANY_THROW(ArgumentParser parser(argc_, argv_));
}

TEST(ArgumentParser, wrong_iterations){
    char input[]  = "input.life";
    char output[] = "output.life";
    char iters[] = "ABC";
    int argc_ = 7;
    char* argv_[] = {"main", "-i", input, "-I", iters, "-o", output};
    EXPECT_ANY_THROW(ArgumentParser parser(argc_, argv_));
}

TEST(ArgumentParser, long_and_short_arguments_1){
    char input[]  = "--input=input.life";
    char output[] = "output.life";
    char iters[] = "20";
    int argc_ = 6;
    char* argv_[] = {"main", input, "-I", iters, "-o", output};
    ArgumentParser parser(argc_, argv_);
    EXPECT_EQ(parser.getInputFile(), input+8);
    EXPECT_EQ(parser.getOutputFile(), output);
    EXPECT_EQ(parser.getIterations(), std::stoi(iters));
}

TEST(ArgumentParser, long_and_short_arguments_2){
    char input[]  = "input.life";
    char output[] = "--output=output.life";
    char iters[] = "--iterations=20";
    int argc_ = 5;
    char* argv_[] = {"main", "-i", input, iters, output};
    ArgumentParser parser(argc_, argv_);
    EXPECT_EQ(parser.getInputFile(), input);
    EXPECT_EQ(parser.getOutputFile(), output+9);
    EXPECT_EQ(parser.getIterations(), std::stoi(iters+13));
}

TEST(ArgumentParser, long_and_short_arguments_3){
    char input[]  = "--input=input.life";
    char output[] = "--output=output.life";
    char iters[] = "--iterations=20";
    int argc_ = 4;
    char* argv_[] = {"main", input, iters, output};
    ArgumentParser parser(argc_, argv_);
    EXPECT_EQ(parser.getInputFile(), input+8);
    EXPECT_EQ(parser.getOutputFile(), output+9);
    EXPECT_EQ(parser.getIterations(), std::stoi(iters+13));
}

TEST(ArgumentParser, argument_permutation_0){
    char input[]  = "input.life";
    char output[] = "output.life";
    char iters[] = "20";
    int argc_ = 8;
    char* argv_[] = {"main", "-i", input, "abcde", "-I", iters, "-o", output};
    EXPECT_ANY_THROW(ArgumentParser parser(argc_, argv_));
}

TEST(ArgumentParser, wrong_arguments_1){
    char input[]  = "-i=input.life";
    char output[] = "output.life";
    char iters[] = "20";
    int argc_ = 6;
    char* argv_[] = {"main", input, "-I", iters, "-o", output};
    EXPECT_ANY_THROW(ArgumentParser parser(argc_, argv_));
}

TEST(ArgumentParser, wrong_arguments_2){
    char input[]  = "input.life";
    char output[] = "-o=output.life";
    char iters[] = "20";
    int argc_ = 6;
    char* argv_[] = {"main", "-i", input, "-I", iters, output};
    EXPECT_ANY_THROW(ArgumentParser parser(argc_, argv_));
}

TEST(ArgumentParser, wrong_arguments_3){
    char input[]  = "input.life";
    char output[] = "output.life";
    char iters[] = "-I=20";
    int argc_ = 6;
    char* argv_[] = {"main", "-i", input, iters, "-o", output};
    EXPECT_ANY_THROW(ArgumentParser parser(argc_, argv_));
}

TEST(ArgumentParser, wrong_arguments_4){
    char input[]  = "input.life";
    char output[] = "output.life";
    char iters[] = "20";
    int argc_ = 6;
    char* argv_[] = {"main", "--input", input, "-I", iters, "-o", output};
    EXPECT_ANY_THROW(ArgumentParser parser(argc_, argv_));
}

TEST(ArgumentParser, wrong_arguments_5){
    char input[]  = "input.life";
    char output[] = "output.life";
    char iters[] = "20";
    int argc_ = 6;
    char* argv_[] = {"main", "-i", input, "--iterations", iters, "-o", output};
    EXPECT_ANY_THROW(ArgumentParser parser(argc_, argv_));
}

TEST(ArgumentParser, wrong_arguments_6){
    char input[]  = "input.life";
    char output[] = "output.life";
    char iters[] = "20";
    int argc_ = 6;
    char* argv_[] = {"main", "-i", input, "-I", iters, "--output", output};
    EXPECT_ANY_THROW(ArgumentParser parser(argc_, argv_));
}

std::string mypath1 = "/home/owner/Документы/projects/c++/task_2/tests/read_tests/";
std::string mypath2 = "/home/owner/Документы/projects/c++/task_2/tests/";

// в следующих тестах возникла проблема с чтением файлов 
// по относительному пути внутри теста, поэтому пришлось
// везде передавать путь абсолютный.

// чтоб решить эту проблему, я пытался подключить стандартную 
// библиотеку filesystem, но при сборке возникала ошибка
// из-за того, что якобы filesystem не является namespace'ом

TEST(WReader, normal_file){
    WReader rw;
    Calculator c;
    Keeper k;
    k = rw.readFile(mypath1 + "normal_file.life", &c);
    EXPECT_EQ(k.getName(), "Universe");
    EXPECT_EQ(k.get().size(), 4);
    EXPECT_EQ(k[0].size(), 5);
    for(int i=0; i<9; i++){
        if(i == 1 || i == 2 || i == 3)
            EXPECT_TRUE(c.birth_rule()[i]);
        else
            EXPECT_FALSE(c.birth_rule()[i]);
    }
    for(int i=0; i<9; i++){
        if(i == 4 || i == 5 || i == 6)
            EXPECT_TRUE(c.survival_rule()[i]);
        else
            EXPECT_FALSE(c.survival_rule()[i]);
    }
    for(int y=0; y<4; y++)
        for(int x=0; x<5; x++){
            if((x==0 && y==0) || (x==1 && y==2) || (x==3 && y==1) || (x==3 && y==3))
                EXPECT_TRUE(k[y][x]);
            else
                EXPECT_FALSE(k[y][x]);
        }
}

TEST(WReader, wrong_title){
    WReader rw;
    Calculator c;
    Keeper k;
    EXPECT_ANY_THROW(k = rw.readFile(mypath1 + "wrong_title.life", &c));
}

TEST(WReader, without_numbers_of_rules){
    WReader rw;
    Calculator c;
    Keeper k;
    k = rw.readFile(mypath1 + "without_numbers_of_rules.life", &c);
    EXPECT_EQ(k.getName(), "Universe");
    EXPECT_EQ(k.get().size(), 4);
    EXPECT_EQ(k[0].size(), 5);
    for(int i=0; i<9; i++){
        if(false)
            EXPECT_TRUE(c.birth_rule()[i]);
        else
            EXPECT_FALSE(c.birth_rule()[i]);
    }
    for(int i=0; i<9; i++){
        if(false)
            EXPECT_TRUE(c.survival_rule()[i]);
        else
            EXPECT_FALSE(c.survival_rule()[i]);
    }
    for(int y=0; y<4; y++)
        for(int x=0; x<5; x++){
            if((x==0 && y==0) || (x==1 && y==2) || (x==3 && y==1) || (x==3 && y==3))
                EXPECT_TRUE(k[y][x]);
            else
                EXPECT_FALSE(k[y][x]);
        }
}

TEST(WReader, wrong_name_format){
    WReader rw;
    Calculator c;
    Keeper k;
    EXPECT_ANY_THROW(k=rw.readFile(mypath1 + "wrong_title_format.life", &c));
}

TEST(WReader, wrong_size_format){
    WReader rw;
    Calculator c;
    Keeper k;
    EXPECT_ANY_THROW(k = rw.readFile(mypath1 + "wrong_size_format.life", &c));
}

TEST(WReader, with_9_in_rules){
    WReader rw;
    Calculator c;
    Keeper k;
    EXPECT_ANY_THROW(k = rw.readFile(mypath1 + "with_9_in_rules.life", &c));
}

TEST(WReader, wrong_rules_format1){
    WReader rw;
    Calculator c;
    Keeper k;
    EXPECT_ANY_THROW(k = rw.readFile(mypath1 + "wrong_rules_format1.life", &c));
}

TEST(WReader, wrong_rules_format2){
    WReader rw;
    Calculator c;
    Keeper k;
    EXPECT_ANY_THROW(k = rw.readFile(mypath1 + "wrong_rules_format2.life", &c));
}

TEST(WReader, coordinates_out_of_scope){ 
    WReader rw;
    Calculator c;
    Keeper k;
    EXPECT_ANY_THROW(k = rw.readFile(mypath1 + "coordinates_out_of_scope.life", &c));
}

TEST(WReader, invariant_of_normal){
    WReader rw;
    Calculator c;
    Keeper k;
    k = rw.readFile(mypath1 + "invariant_of_normal.life", &c);
    EXPECT_EQ(k.getName(), "Universe");
    EXPECT_EQ(k.get().size(), 4);
    EXPECT_EQ(k[0].size(), 5);
    for(int i=0; i<9; i++){
        if(i == 1 || i == 2 || i == 3)
            EXPECT_TRUE(c.birth_rule()[i]);
        else
            EXPECT_FALSE(c.birth_rule()[i]);
    }
    for(int i=0; i<9; i++){
        if(i == 4 || i == 5 || i == 6)
            EXPECT_TRUE(c.survival_rule()[i]);
        else
            EXPECT_FALSE(c.survival_rule()[i]);
    }
    for(int y=0; y<4; y++)
        for(int x=0; x<5; x++){
            if((x==0 && y==0) || (x==1 && y==2) || (x==3 && y==1) || (x==3 && y==3))
                EXPECT_TRUE(k[y][x]);
            else
                EXPECT_FALSE(k[y][x]);
        }
}

TEST(WReader, write_test){
    WReader rw;
    Calculator c1;
    Keeper k1; // /home/owner/Документы/projects/c++/task_2/tests/

    k1 = rw.readFile(mypath2 + "input.life", &c1);
    rw.writeFile("tests/output.life", k1, c1);

    Calculator c2;
    Keeper k2;
    k2 = rw.readFile(mypath2 + "output.life", &c2);

    EXPECT_EQ(c1, c2);
    EXPECT_EQ(k1, k2);
}

TEST(Interface, print) {
    Interface interface;

    std::streambuf* orig_cin;
    std::streambuf* orig_cout;
    std::stringstream input_stream;
    std::stringstream output_stream;

    orig_cin  = std::cin.rdbuf();
    orig_cout = std::cout.rdbuf();

    std::cin.rdbuf(input_stream.rdbuf());
    std::cout.rdbuf(output_stream.rdbuf());



    input_stream.str("exit");

    int argc_ = 1;
    char* argv_[] = {"main"};
    std::string filename = mypath2 + "input.life";
    
    WReader rw;
    Calculator calc;
    Keeper k = rw.readFile(filename, &calc);

    interface.game(argc_, argv_, filename);
    
    std::string name;
    getline(output_stream, name);
    ASSERT_EQ(k.getName(), name);

    // по сути это проверка метода print

    char c;
    output_stream.get(c);
    ASSERT_EQ(c, 'B');

    output_stream.get(c);
    for(; c != '/'; output_stream.get(c)){
        ASSERT_TRUE(std::isdigit(c));
        ASSERT_TRUE(calc.birth_rule()[c-'0']);
    }
    
    output_stream.get(c);
    ASSERT_EQ(c, 'S');

    output_stream.get(c);
    for(; c != '\n'; output_stream.get(c)){
        ASSERT_TRUE(std::isdigit(c));
        ASSERT_TRUE(calc.survival_rule()[c-'0']);
    }

    for(int y=0; y<k.get().size(); y++)
        for(int x=0; x<k[0].size(); x++){
            output_stream >> c;
            if(k[y][x])
                ASSERT_EQ(c, 'O');
            else
                ASSERT_EQ(c, '.');
        }



    std::cin.rdbuf(orig_cin);
    std::cout.rdbuf(orig_cout);
}

TEST(Interface, dump) {
    Interface interface;

    std::streambuf* orig_cin;
    std::streambuf* orig_cout;
    std::stringstream input_stream;
    std::stringstream output_stream;

    orig_cin  = std::cin.rdbuf();
    orig_cout = std::cout.rdbuf();

    std::cin.rdbuf(input_stream.rdbuf());
    std::cout.rdbuf(output_stream.rdbuf());



    int argc_ = 1;
    char* argv_[] = {"main"};
    std::string input_file = mypath2 + "input.life";
    std::string output_file = mypath2 + "output.life";
    
    input_stream.str("dump tests/output.life\n"
                     "exit\n");

    interface.game(argc_, argv_, input_file);

    WReader rw;
    Calculator calc1;
    Keeper k1 = rw.readFile(input_file, &calc1);
    Calculator calc2;
    Keeper k2 = rw.readFile(output_file, &calc2);

    EXPECT_EQ(k1, k2);
    EXPECT_EQ(calc1, calc2);



    std::cin.rdbuf(orig_cin);
    std::cout.rdbuf(orig_cout);
}

TEST(Interface, help) {
    Interface interface;

    std::streambuf* orig_cin;
    std::streambuf* orig_cout;
    std::stringstream input_stream;
    std::stringstream output_stream;

    orig_cin  = std::cin.rdbuf();
    orig_cout = std::cout.rdbuf();

    std::cin.rdbuf(input_stream.rdbuf());
    std::cout.rdbuf(output_stream.rdbuf());



    int argc_ = 1;
    char* argv_[] = {"main"};
    std::string filename = mypath2 + "input.life";
    
    WReader rw;
    Calculator calc;
    Keeper k = rw.readFile(filename, &calc);

    input_stream.str("help\n"
                     "exit\n");

    interface.game(argc_, argv_, filename);
    
    std::string line;
    getline(output_stream, line);
    getline(output_stream, line);
    for(auto i : k.get())
        getline(output_stream, line);
    
    getline(output_stream, line);
    ASSERT_EQ(line, "dump <filename> - save the universe to a file");
    getline(output_stream, line);
    ASSERT_EQ(line, "tick <n=1> (t <n=1>) - calculate n iterations (default is 1)");
    getline(output_stream, line);
    ASSERT_EQ(line, "and print the universe, it's name and rule of transition");
    getline(output_stream, line);
    ASSERT_EQ(line, "exit - exit the game");
    getline(output_stream, line);
    ASSERT_EQ(line, "help - print the help about the commands");



    std::cin.rdbuf(orig_cin);
    std::cout.rdbuf(orig_cout);
}

TEST(Interface, tick) {
    Interface interface;

    std::streambuf* orig_cin;
    std::streambuf* orig_cout;
    std::stringstream input_stream;
    std::stringstream output_stream;

    orig_cin  = std::cin.rdbuf();
    orig_cout = std::cout.rdbuf();

    std::cin.rdbuf(input_stream.rdbuf());
    std::cout.rdbuf(output_stream.rdbuf());



    int argc_ = 1;
    char* argv_[] = {"main"};
    std::string filename = mypath2 + "input.life";
    
    WReader rw;
    Calculator calc;
    Keeper k = rw.readFile(filename, &calc);

    input_stream.str("tick\n"
                     "exit\n");

    interface.game(argc_, argv_, filename);

    char c;
    std::string line;
    getline(output_stream, line);
    getline(output_stream, line);
    for(auto i : k.get())
        getline(output_stream, line);

    k = calc.calculate(k);

    getline(output_stream, line);
    getline(output_stream, line);

    for(int y=0; y<k.get().size(); y++)
        for(int x=0; x<k[0].size(); x++){
            output_stream >> c;
            if(k[y][x])
                ASSERT_EQ(c, 'O');
            else
                ASSERT_EQ(c, '.');
        }



    std::cin.rdbuf(orig_cin);
    std::cout.rdbuf(orig_cout);
}

TEST(Interface, tick_n) {
    Interface interface;

    std::streambuf* orig_cin;
    std::streambuf* orig_cout;
    std::stringstream input_stream;
    std::stringstream output_stream;

    orig_cin  = std::cin.rdbuf();
    orig_cout = std::cout.rdbuf();

    std::cin.rdbuf(input_stream.rdbuf());
    std::cout.rdbuf(output_stream.rdbuf());



    int argc_ = 1;
    char* argv_[] = {"main"};
    std::string filename = mypath2 + "input.life";
    
    WReader rw;
    Calculator calc;
    Keeper k = rw.readFile(filename, &calc);

    input_stream.str("tick 3\n"
                     "exit\n");

    interface.game(argc_, argv_, filename);

    char c;
    std::string line;
    getline(output_stream, line);
    getline(output_stream, line);
    for(auto i : k.get())
        getline(output_stream, line);

    k = calc.calculate(k, 3);

    getline(output_stream, line);
    getline(output_stream, line);

    for(int y=0; y<k.get().size(); y++)
        for(int x=0; x<k[0].size(); x++){
            output_stream >> c;
            if(k[y][x])
                ASSERT_EQ(c, 'O');
            else
                ASSERT_EQ(c, '.');
        }



    std::cin.rdbuf(orig_cin);
    std::cout.rdbuf(orig_cout);
}

TEST(Interface, t) {
    Interface interface;

    std::streambuf* orig_cin;
    std::streambuf* orig_cout;
    std::stringstream input_stream;
    std::stringstream output_stream;

    orig_cin  = std::cin.rdbuf();
    orig_cout = std::cout.rdbuf();

    std::cin.rdbuf(input_stream.rdbuf());
    std::cout.rdbuf(output_stream.rdbuf());



    int argc_ = 1;
    char* argv_[] = {"main"};
    std::string filename = mypath2 + "input.life";
    
    WReader rw;
    Calculator calc;
    Keeper k = rw.readFile(filename, &calc);

    input_stream.str("t\n"
                     "exit\n");

    interface.game(argc_, argv_, filename);

    char c;
    std::string line;
    getline(output_stream, line);
    getline(output_stream, line);
    for(auto i : k.get())
        getline(output_stream, line);

    k = calc.calculate(k);

    getline(output_stream, line);
    getline(output_stream, line);

    for(int y=0; y<k.get().size(); y++)
        for(int x=0; x<k[0].size(); x++){
            output_stream >> c;
            if(k[y][x])
                ASSERT_EQ(c, 'O');
            else
                ASSERT_EQ(c, '.');
        }



    std::cin.rdbuf(orig_cin);
    std::cout.rdbuf(orig_cout);
}

TEST(Interface, t_n) {
    Interface interface;

    std::streambuf* orig_cin;
    std::streambuf* orig_cout;
    std::stringstream input_stream;
    std::stringstream output_stream;

    orig_cin  = std::cin.rdbuf();
    orig_cout = std::cout.rdbuf();

    std::cin.rdbuf(input_stream.rdbuf());
    std::cout.rdbuf(output_stream.rdbuf());



    int argc_ = 1;
    char* argv_[] = {"main"};
    std::string filename = mypath2 + "input.life";
    
    WReader rw;
    Calculator calc;
    Keeper k = rw.readFile(filename, &calc);

    input_stream.str("t 3\n"
                     "exit\n");

    interface.game(argc_, argv_, filename);

    char c;
    std::string line;
    getline(output_stream, line);
    getline(output_stream, line);
    for(auto i : k.get())
        getline(output_stream, line);

    k = calc.calculate(k, 3);

    getline(output_stream, line);
    getline(output_stream, line);

    for(int y=0; y<k.get().size(); y++)
        for(int x=0; x<k[0].size(); x++){
            output_stream >> c;
            if(k[y][x])
                ASSERT_EQ(c, 'O');
            else
                ASSERT_EQ(c, '.');
        }



    std::cin.rdbuf(orig_cin);
    std::cout.rdbuf(orig_cout);
}

TEST(Interface, wrong_format) {
    Interface interface;

    std::streambuf* orig_cin;
    std::streambuf* orig_cout;
    std::stringstream input_stream;
    std::stringstream output_stream;

    orig_cin  = std::cin.rdbuf();
    orig_cout = std::cout.rdbuf();

    std::cin.rdbuf(input_stream.rdbuf());
    std::cout.rdbuf(output_stream.rdbuf());



    int argc_ = 1;
    char* argv_[] = {"main"};
    std::string filename = mypath2 + "input.life";
    
    input_stream.str("tick10\n"
                     "exit\n");

    EXPECT_ANY_THROW(interface.game(argc_, argv_, filename));



    std::cin.rdbuf(orig_cin);
    std::cout.rdbuf(orig_cout);
}

TEST(Interface, wrong_command) {
    Interface interface;

    std::streambuf* orig_cin;
    std::streambuf* orig_cout;
    std::stringstream input_stream;
    std::stringstream output_stream;

    orig_cin  = std::cin.rdbuf();
    orig_cout = std::cout.rdbuf();

    std::cin.rdbuf(input_stream.rdbuf());
    std::cout.rdbuf(output_stream.rdbuf());



    int argc_ = 1;
    char* argv_[] = {"main"};
    std::string filename = mypath2 + "input.life";
    
    input_stream.str("abcdef\n"
                     "exit\n");

    EXPECT_ANY_THROW(interface.game(argc_, argv_, filename));



    std::cin.rdbuf(orig_cin);
    std::cout.rdbuf(orig_cout);
}

TEST(Interface, sequence) {
    Interface interface;

    std::streambuf* orig_cin;
    std::streambuf* orig_cout;
    std::stringstream input_stream;
    std::stringstream output_stream;

    orig_cin  = std::cin.rdbuf();
    orig_cout = std::cout.rdbuf();

    std::cin.rdbuf(input_stream.rdbuf());
    std::cout.rdbuf(output_stream.rdbuf());



    int argc_ = 1;
    char* argv_[] = {"main"};
    std::string filename = mypath2 + "input.life";
    
    WReader rw;
    Calculator calc;
    Keeper k = rw.readFile(filename, &calc);

    input_stream.str("t\n"
                     "tick 3\n"
                     "dump tests/output.life\n"
                     "exit\n");

    interface.game(argc_, argv_, filename);
    
    std::string name;
    std::string line;
    getline(output_stream, name);
    ASSERT_EQ(k.getName(), name);

    char c;
    output_stream.get(c);
    ASSERT_EQ(c, 'B');

    output_stream.get(c);
    for(; c != '/'; output_stream.get(c)){
        ASSERT_TRUE(std::isdigit(c));
        ASSERT_TRUE(calc.birth_rule()[c-'0']);
    }
    
    output_stream.get(c);
    ASSERT_EQ(c, 'S');

    output_stream.get(c);
    for(; c != '\n'; output_stream.get(c)){
        ASSERT_TRUE(std::isdigit(c));
        ASSERT_TRUE(calc.survival_rule()[c-'0']);
    }

    for(int y=0; y<k.get().size(); y++)
        for(int x=0; x<k[0].size(); x++){
            output_stream >> c;
            if(k[y][x])
                ASSERT_EQ(c, 'O');
            else
                ASSERT_EQ(c, '.');
        }
    getline(output_stream, line);


    
    k = calc.calculate(k);



    getline(output_stream, name);
    ASSERT_EQ(k.getName(), name);

    output_stream.get(c);
    ASSERT_EQ(c, 'B');

    output_stream.get(c);
    for(; c != '/'; output_stream.get(c)){
        ASSERT_TRUE(std::isdigit(c));
        ASSERT_TRUE(calc.birth_rule()[c-'0']);
    }
    
    output_stream.get(c);
    ASSERT_EQ(c, 'S');

    output_stream.get(c);
    for(; c != '\n'; output_stream.get(c)){
        ASSERT_TRUE(std::isdigit(c));
        ASSERT_TRUE(calc.survival_rule()[c-'0']);
    }

    for(int y=0; y<k.get().size(); y++)
        for(int x=0; x<k[0].size(); x++){
            output_stream >> c;
            if(k[y][x])
                ASSERT_EQ(c, 'O');
            else
                ASSERT_EQ(c, '.');
        }
    getline(output_stream, line);


    
    k = calc.calculate(k, 3);



    getline(output_stream, name);
    ASSERT_EQ(k.getName(), name);

    output_stream.get(c);
    ASSERT_EQ(c, 'B');

    output_stream.get(c);
    for(; c != '/'; output_stream.get(c)){
        ASSERT_TRUE(std::isdigit(c));
        ASSERT_TRUE(calc.birth_rule()[c-'0']);
    }
    
    output_stream.get(c);
    ASSERT_EQ(c, 'S');

    output_stream.get(c);
    for(; c != '\n'; output_stream.get(c)){
        ASSERT_TRUE(std::isdigit(c));
        ASSERT_TRUE(calc.survival_rule()[c-'0']);
    }

    for(int y=0; y<k.get().size(); y++)
        for(int x=0; x<k[0].size(); x++){
            output_stream >> c;
            if(k[y][x])
                ASSERT_EQ(c, 'O');
            else
                ASSERT_EQ(c, '.');
        }
    getline(output_stream, line);
    std::string line1;
    getline(output_stream, line1);



    Calculator calc1;
    Keeper k1 = rw.readFile(mypath2 + "output.life", &calc1);

    EXPECT_EQ(calc, calc1);
    EXPECT_EQ(k, k1);



    std::cin.rdbuf(orig_cin);
    std::cout.rdbuf(orig_cout);
}
