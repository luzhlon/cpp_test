
#include <Windows.h>
#include "EventManager.hpp"
#include <stdio.h>

typedef EventManager<int> EM;

void receiver(EM *em, int arg)
{
    printf("Receiver\n");
}

void receiver0()
{
    printf("Receiver0\n");
}

void receiver1(int)
{
    printf("Receiver1\n");
}

void receiver_2(int)
{
    printf("Receiver_2\n");
}

int main(int argc, char *argv[])
{
    EM em;

    em.subscribe<receiver>();
    em.subscribe<receiver0>();
    em.subscribe<receiver1>();
    em.subscribe<receiver_2, 2, 0>();

    for (size_t i = 0; i < 4; ++i)
    {
        em.notify(0);
        printf("-----------\n");
    }
    return 0;
}
