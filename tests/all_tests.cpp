//#define NULLPTR nullptr
//#define _destructor_override

#include "CppUTest/CommandLineTestRunner.h"

int main(int ac, char **av)
{
    return (CommandLineTestRunner::RunAllTests(ac, av));
}
