#include <iostream>
#include <memory>

class AVL
{
private:
    struct Node
    {
        int value;
        Node* parent;
        std::unique_ptr<Node> left_child = nullptr;
        std::unique_ptr<Node> right_child = nullptr;

        Node(int x = 0, Node* pr = nullptr) : value(x), parent(pr) {}
        
        bool node_add(int x)
        {
            if(x < value)
            {
                if(left_child == nullptr)
                {
                    left_child = std::make_unique<Node>(x, this);
                    return true;
                }
                else
                {
                    return left_child->node_add(x);
                }
            }
            else if(x > value)
            {
                if(right_child == nullptr)
                {
                    right_child = std::make_unique<Node>(x, this);
                    return true;
                }
                else
                {
                    return right_child->node_add(x);
                }
            }
            else
            {
                return false;
            }
        }

        void node_print(std::ostream& os)
        {
            if(left_child != nullptr)
            {
                left_child->node_print(os);
            }
            os << value << ' ';
            if(right_child != nullptr)
            {
                right_child->node_print(os);
            }
        }
    };

    std::unique_ptr<Node> root = nullptr;
    size_t _size;

public:
    AVL() = default;

    void add(int x)
    {
        if(root == nullptr)
        {
            root = std::make_unique<Node>(x);
            ++_size;
        }
        else
        {
            if(root->node_add(x)) ++_size;
        }
    }

    void print(std::ostream& os)
    {
        root->node_print(os);
        os << std::endl;
    }

    size_t size() { return _size; }
};

int main()
{
    AVL tree;
    tree.add(5);
    tree.add(3);
    tree.add(6);
    tree.add(2);
    tree.add(4);
    tree.add(8);
    tree.add(7);

    tree.print(std::cout);

    return 0;
}