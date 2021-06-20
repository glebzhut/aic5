#include <iostream>
#include "tree.hpp"
#include "classes.h"

int main()
{
    std::cout << white;
    auto* persistentSet = new PersistentSet<Group>;
    Group man1 = Group("A");
    Group man2 = Group("B");
    Group man3 = Group("C");
    Group man4 = Group("D");
    Group man5 = Group("E");
    Group man6 = Group("F");
    Group man7 = Group("G");
    Group man8 = Group("H");
    Group man9 = Group("I");
    Group man10 = Group("J");

    Group sorted[10] = { man1, man2, man3, man4, man5, man6, man7, man8, man9, man10 };
    Group ar[10] = { man4, man2, man3, man6, man10, man5, man1, man8, man7, man9 };

    for (int i = 0; i < 10; i++)
    {
        persistentSet->Insert(ar[i]);
        std::cout << "------------insert---------\n";
    }

    for (int i = 1; i < 10; i++)
    {
        persistentSet->Remove(sorted[i]);
        std::cout << "------------delete---------\n";
    }

    persistentSet->Print();
}



