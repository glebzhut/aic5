#include <iostream>
#include <vector>
#include <conio.h>

const std::string red = "\u001B[31m";
const std::string white = "\u001B[34m";

/*
Class implements persistent set based on red-black tree
time of insertion, deletion and needed memory - O(log n) in worst case
 */

template <typename T>
class PersistentSet
{
private:
    //Class implements red-black tree node
    class Node
    {
    public:
        T value;
        Node* parent;
        Node* left;
        Node* right;
        bool isBlack{};

        //Constructors
        Node()
        {
            left = right = parent = nullptr;
            isBlack = true;
        }

        explicit Node(T& value)
        {
            this->value = value;
            this->left = nullptr;
            this->right = nullptr;
            this->parent = nullptr;
            this->isBlack = false;
        }

        explicit Node(Node* toCopy)
        {
            if (toCopy == nullptr)
                return;
            this->value = toCopy->value;
            this->left = toCopy->left;
            this->right = toCopy->right;
            this->parent = toCopy->parent;
            this->isBlack = toCopy->isBlack;
            if (left != nullptr)
                left->parent = this;
            if (right != nullptr)
                right->parent = this;

            if (parent != nullptr)
            {
                if (parent->left != nullptr)
                {
                    if (parent->left->value == value)
                        parent->left = this;
                    else parent->right = this;
                }
                else parent->right = this;
            }
        }

        ~Node() = default;

        void Print(std::ostream& os)
        {
            if (!isBlack)
                os << red;
            os << "Value: " << value;
            os << white;
        }
    };


    //Addition function for insertion
    void InsertCase1(Node* toFix)
    {
        if (toFix->parent == nullptr)
            toFix->isBlack = true;
        else if (!toFix->parent->isBlack)
            InsertCase2(toFix);
    }

    void InsertCase2(Node* toFix)
    {
        Node* uncle;
        Node* grandfather = new Node(toFix->parent->parent);
        if (grandfather->parent == nullptr)
            roots[roots.size() - 1] = grandfather;
        Node* parent = new Node(toFix->parent);
        if (grandfather == nullptr)
            uncle = nullptr;
        else if (toFix->parent == grandfather->left)
        {
            if (grandfather->right != nullptr)
            {
                uncle = new Node(grandfather->right);
                grandfather->right = uncle;
            }
            else uncle = nullptr;
        }
        else
        {
            if (grandfather->left != nullptr)
            {
                uncle = new Node(grandfather->left);
                grandfather->left = uncle;
            }
            else uncle = nullptr;
        }

        if (uncle != nullptr && !uncle->isBlack)
        {
            grandfather->isBlack = false;
            parent->isBlack = true;
            uncle->isBlack = true;
            InsertCase1(grandfather);
        }
        else InsertCase3(toFix);
    }

    void InsertCase3(Node* toFix)
    {
        Node* grandfather = new Node(toFix->parent->parent);
        if (grandfather->parent == nullptr)
            roots[roots.size() - 1] = grandfather;

        if (toFix == toFix->parent->right && toFix->parent == grandfather->left)
        {
            Node* saved_parent = new Node(grandfather->left);
            Node* saved_brother = nullptr;
            if (toFix->left != nullptr)
                saved_brother = new Node(toFix->left);
            grandfather->left = toFix;
            toFix->parent = grandfather;
            toFix->left = saved_parent;
            saved_parent->parent = toFix;
            saved_parent->right = saved_brother;
            if (saved_brother != nullptr)
                saved_parent->right->parent = saved_parent;
            toFix = toFix->left;
        }

        else if (toFix == toFix->parent->left && toFix->parent == grandfather->right)
        {
            Node* saved_parent = new Node(grandfather->right);
            Node* saved_brother = nullptr;
            if (toFix->right != nullptr)
                saved_brother = new Node(toFix->right);
            grandfather->right = toFix;
            toFix->parent = grandfather;
            toFix->right = saved_parent;
            saved_parent->parent = toFix;
            saved_parent->left = saved_brother;
            if (saved_brother != nullptr)
                saved_parent->left->parent = saved_parent;
            toFix = toFix->right;
        }
        InsertCase4(toFix);
    }

    void InsertCase4(Node* toFix)
    {
        Node* grandfather = new Node(toFix->parent->parent);
        if (grandfather->parent == nullptr)
            roots[roots.size() - 1] = grandfather;
        Node* parent = new Node(toFix->parent);
        parent->isBlack = true;
        grandfather->isBlack = false;
        if (toFix == toFix->parent->left && toFix->parent == grandfather->left)
            RightRotate(grandfather);
        else
            LeftRotate(grandfather);
    }

    //Rotations
    void LeftRotate(Node* toRotate)
    {
        Node* pivot = new Node(toRotate->right);
        pivot->parent = toRotate->parent;
        if (toRotate->parent != nullptr)
        {
            if (toRotate->parent->left == toRotate)
                toRotate->parent->left = pivot;
            else toRotate->parent->right = pivot;
        }
        else this->roots[roots.size() - 1] = pivot;
        toRotate->right = pivot->left;
        if (pivot->left != nullptr)
            pivot->left->parent = toRotate;
        toRotate->parent = pivot;
        pivot->left = toRotate;
    }

    void RightRotate(Node* toRotate)
    {
        Node* pivot = new Node(toRotate->left);
        pivot->parent = toRotate->parent;
        if (toRotate->parent != nullptr)
            if (toRotate->parent->left == toRotate)
                toRotate->parent->left = pivot;
            else toRotate->parent->right = pivot;
        else this->roots[roots.size() - 1] = pivot;
        toRotate->left = pivot->right;
        if (pivot->right != nullptr)
            pivot->right->parent = toRotate;
        toRotate->parent = pivot;
        pivot->right = toRotate;
    }

    //Additional functions for removing
    void Erase(Node* node, const T& t)
    {
        if (node == nullptr) return;
        Node* copy = new Node(node);
        if (copy->parent == nullptr)
            roots.emplace_back(copy);
        if (t < node->value)
            Erase(copy->left, t);
        else if (t > node->value)
            Erase(copy->right, t);
        else DeleteNode(copy);
    }

    void DeleteNode(Node* toDelete)
    {
        if (toDelete->right == nullptr && toDelete->left == nullptr)
        {
            if (toDelete == roots[roots.size() - 1])
            {
                roots[roots.size() - 1] == nullptr;
                return;
            }
            bool sideRight;
            Node* parent = new Node(toDelete->parent);
            if (parent->parent == nullptr)
                roots[roots.size() - 1] = parent;
            if (toDelete == toDelete->parent->right)
            {
                parent->right = nullptr;
                sideRight = true;
            }
            else
            {
                parent->left = nullptr;
                sideRight = false;
            }
            if (toDelete->isBlack)
                DeleteFix(parent, sideRight);
            delete toDelete;
            return;
        }
        if (toDelete->right == nullptr && toDelete->left != nullptr)
        {
            Node* left = new Node(toDelete->left);
            if (toDelete == roots[roots.size() - 1])
            {
                roots[roots.size() - 1] = left;
                left->parent = nullptr;
                delete toDelete;
                return;
            }

            Node* parent = new Node(toDelete->parent);
            if (parent->parent == nullptr)
                roots[roots.size() - 1] = parent;

            if (toDelete == parent->right)
            {
                parent->right = left;
                left->parent = parent;
            }
            else
            {
                parent->left = left;
                left->parent = parent;
            }
            left->isBlack = toDelete->isBlack;
            delete toDelete;
            return;
        }
        if (toDelete->right != nullptr && toDelete->left == nullptr)
        {
            Node* right = new Node(toDelete->right);
            if (toDelete == roots[roots.size() - 1])
            {
                roots[roots.size() - 1] = right;
                right->parent = nullptr;
                delete toDelete;
                return;
            }

            Node* parent = new Node(toDelete->parent);
            if (parent->parent == nullptr)
                roots[roots.size() - 1] = parent;

            if (toDelete == parent->right)
            {
                parent->right = right;
                right->parent = parent;
            }
            else
            {
                parent->left = right;
                right->parent = parent;

            }
            right->isBlack = toDelete->isBlack;
            delete toDelete;
            return;
        }
        else
        {
            Node* successor = toDelete;
            successor = new Node(successor->left);

            while (true)
            {
                if (successor->right != nullptr)
                    successor = new Node(successor->right);
                else break;
            }
            toDelete->value = successor->value;
            DeleteNode(successor);
            return;
        }
    }

    void DeleteFix(Node* toFix, bool& sideRight)
    {
        Node* brother;
        if (sideRight)
            brother = new Node(toFix->left);
        else
            brother = new Node(toFix->right);

        Node* leftGrandson = brother->left;
        Node* rightGrandson = brother->right;

        if (leftGrandson == nullptr)
            leftGrandson = new Node();
        else
            leftGrandson = new Node(brother->left);

        if (rightGrandson == nullptr)
            rightGrandson = new Node();
        else
            rightGrandson = new Node(brother->right);

        if (!toFix->isBlack && brother->isBlack)
        {
            if (leftGrandson->isBlack && rightGrandson->isBlack)
            {
                toFix->isBlack = true;
                brother->isBlack = false;
            }
            else
            {
                if (sideRight)
                {
                    if (!leftGrandson->isBlack)
                    {
                        toFix->isBlack = true;
                        brother->isBlack = false;
                        leftGrandson->isBlack = true;
                        RightRotate(toFix);
                    }
                    else
                    {
                        toFix->isBlack = true;
                        rightGrandson->isBlack = true;
                        LeftRotate(brother);
                        RightRotate(toFix);
                    }
                }
                else {

                    if (!rightGrandson->isBlack)
                    {
                        toFix->isBlack = true;
                        brother->isBlack = false;
                        rightGrandson->isBlack = true;
                        LeftRotate(toFix);
                    }
                    else
                    {
                        toFix->isBlack = true;
                        leftGrandson->isBlack = true;
                        RightRotate(brother);
                        LeftRotate(toFix);
                    }
                }
            }
        }
        else if (toFix->isBlack && !brother->isBlack)
        {

            Node* leftGreatGrandson;
            Node* rightGreatGrandson;
            if (sideRight)
            {
                leftGreatGrandson = rightGrandson->left;
                rightGreatGrandson = rightGrandson->right;
            }
            else
            {
                leftGreatGrandson = leftGrandson->left;
                rightGreatGrandson = leftGrandson->right;
            }
            if (leftGreatGrandson == nullptr)
                leftGreatGrandson = new Node();
            else if (sideRight)
                leftGreatGrandson = new Node(rightGrandson->left);
            else leftGreatGrandson = new Node(leftGrandson->left);
            if (rightGreatGrandson == nullptr)
                rightGreatGrandson = new Node();
            else if (sideRight)
                rightGreatGrandson = new Node(rightGrandson->right);
            else rightGreatGrandson = new Node(leftGrandson->right);
            if (sideRight)
            {
                if (leftGreatGrandson->isBlack && rightGreatGrandson->isBlack)
                {
                    brother->isBlack = true;
                    rightGrandson->isBlack = false;
                    RightRotate(toFix);
                }
                else if (!leftGreatGrandson->isBlack)
                {
                    LeftRotate(brother);
                    RightRotate(toFix);
                    leftGrandson->isBlack = true;
                }
            }
            else
            {
                if (leftGreatGrandson->isBlack && rightGreatGrandson->isBlack)
                {
                    brother->isBlack = true;
                    leftGrandson->isBlack = false;
                    LeftRotate(toFix);
                }
                else if (!rightGreatGrandson->isBlack)
                {
                    RightRotate(brother);
                    LeftRotate(toFix);
                    rightGrandson->isBlack = true;
                }
            }
        }
        else
        {
            if (sideRight)
            {
                if (!rightGrandson->isBlack)
                {
                    rightGrandson->isBlack = true;
                    LeftRotate(brother);
                    RightRotate(toFix);
                }
                else if (!leftGrandson->isBlack && rightGrandson->parent == nullptr)
                {
                    leftGrandson->isBlack = true;
                    RightRotate(toFix);
                }
            }
            else if (!sideRight)
            {
                if (!leftGrandson->isBlack)
                {
                    leftGrandson->isBlack = true;
                    RightRotate(brother);
                    LeftRotate(toFix);
                }
                else if (!rightGrandson->isBlack && leftGrandson->parent == nullptr)
                {
                    rightGrandson->isBlack = true;
                    LeftRotate(toFix);
                }
            }
            else
            {
                brother->isBlack = false;
                if (toFix->parent != nullptr)
                {
                    Node* parent = new Node(toFix->parent);
                    if (toFix == parent->right)
                        sideRight = true;
                    else
                        sideRight = false;
                    DeleteFix(parent, sideRight);
                }
            }
        }
    }

    void BackupTree(Node* toBackup, Node* parent)
    {
        if (toBackup == nullptr)
            return;
        toBackup->parent = parent;
        BackupTree(toBackup->left, toBackup);
        BackupTree(toBackup->right, toBackup);
    }

    //Interactive output
    Node* ChooseNode(Node* current_node, int i)
    {
        if (current_node)
        {
            int position = 0;
            bool end = false;

            while (!end)
            {
                system("cls");
                std::cout << white << "------------------------" << i << " version---------------------------\n";
                std::cout << "Parent Node: ";
                if (current_node->parent)
                {
                    if (!current_node->parent->isBlack)
                        std::cout << red << current_node->parent->value << white;
                    else std::cout << current_node->parent->value;
                }
                std::cout << std::endl;
                if (position == 0)
                    std::cout << "-> Current Node: ";
                else std::cout << "   Current Node: ";
                current_node->Print(std::cout);
                std::cout << std::endl;
                if (position == 1)
                {
                    if (current_node->left)
                        if (!current_node->left->isBlack)
                            std::cout << red << "-> Left Node " << current_node->left->value << std::endl << white;
                        else std::cout << "-> Left Node " << current_node->left->value << std::endl;
                    else
                        std::cout << white << "-> NULL" << std::endl;
                }
                else
                {
                    if (current_node->left)
                        if (!current_node->left->isBlack)
                            std::cout << red << "   Left Node " << current_node->left->value << std::endl << white;
                        else std::cout << "   Left Node " << current_node->left->value << std::endl;
                    else
                        std::cout << white << "   NULL" << std::endl;
                }
                if (position == 2)
                {
                    if (current_node->right)
                        if (!current_node->right->isBlack)
                            std::cout << red << "-> Right Node " << current_node->right->value << std::endl << white;
                        else std::cout << "-> Right Node " << current_node->right->value << std::endl;
                    else std::cout << white << "-> NULL" << std::endl;
                }
                else
                {
                    if (current_node->right)
                        if (!current_node->right->isBlack)
                            std::cout << red << "   Right Node " << current_node->right->value << std::endl << white;
                        else std::cout << "   Right Node " << current_node->right->value << std::endl;
                    else std::cout << white << "   NULL" << std::endl;
                }
                if (position == 3)
                    std::cout << "-> Back;" << std::endl;
                else std::cout << "   Back;" << std::endl;

                char key;
                key = _getch();
                if ((int)key != 13)
                {
                    if ((int)key == -32)
                    {
                        key = _getch();
                        if ((int)key == 72)
                        {
                            if (position > 0)
                                position--;
                            else position = 3;
                        }

                        if ((int)key == 80)
                        {
                            if (position < 3)
                                position++;
                            else position = 0;
                        }
                    }
                }
                else
                {
                    switch (position)
                    {
                    case 0:
                        return current_node;
                    case 1:
                        if (current_node->left)
                            return ChooseNode(current_node->left, i);
                        break;
                    case 2:
                        if (current_node->right)
                            return ChooseNode(current_node->right,i);
                        break;
                    default:
                        if (current_node->parent)
                            return ChooseNode(current_node->parent, i);
                        break;
                    }
                }
            }
        }
    }

    std::vector<Node*> roots;

public:
    //Constructor
    PersistentSet() : roots({ nullptr }) {};

    //Insertion
    void Insert(T& t) 
    {
        Node* temp = roots[roots.size() - 1];
        Node* futureParent = nullptr;
        Node* toInsert = new Node(t);
        while (temp != nullptr) 
        {
            futureParent = new Node(temp);
            if (futureParent->parent == nullptr)
                roots.emplace_back(futureParent);
            if (toInsert->value < temp->value)
                temp = temp->left;
            else temp = temp->right;
        }
        toInsert->parent = futureParent;
        if (futureParent == nullptr)
            roots.emplace_back(toInsert);
        else if (toInsert->value < futureParent->value)
            futureParent->left = toInsert;
        else futureParent->right = toInsert;
        InsertCase1(toInsert);
    }

    //Removing
    void Remove(const T& t) 
    {
        Erase(roots[roots.size() - 1], t);
    }

    //Output in console
    void Print() 
    {
        for (int i = 0; i < roots.size(); i++) {
            BackupTree(roots[i], nullptr);
            auto t = ChooseNode(roots[i], i);
            system("pause");
        }
    }
};