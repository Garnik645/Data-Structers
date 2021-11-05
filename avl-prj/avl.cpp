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
        
        bool node_remove(int x)
        {
            if(x > value && right_child == nullptr) return false;
            if(x < value && left_child == nullptr) return false;

            if(x > value) return right_child->node_remove(x);
            if(x < value) return left_child->node_remove(x);
        
            if(left_child == nullptr && right_child == nullptr)
            {
                if(parent->value > x)
                {
                    parent->left_child = nullptr;
                }
                else
                {
                    parent->right_child = nullptr;
                }
            }
            else if(left_child == nullptr && right_child != nullptr)
            {
                if(parent->value > x)
                {
                    std::swap(parent->left_child, right_child);
                    right_child = nullptr;
                }
                else
                {
                    std::swap(parent->right_child, right_child);
                    right_child = nullptr;
                }
            }
            else if(left_child != nullptr && right_child == nullptr)
            {
                if(parent->value > x)
                {
                    std::swap(parent->left_child, left_child);
                    right_child = nullptr;
                }
                else
                {
                    std::swap(parent->right_child, left_child);
                    right_child = nullptr;
                }
            }
            else
            {
                value = right_child->successor();
            }
            return true;
            
        }
        
        int successor()
        {
            if(left_child != nullptr)
            {
                return left_child->successor();
            }
            else
            {
                int x = value;
                node_remove(x);
                return x;
            }
        }
    };

    std::unique_ptr<Node> root = nullptr;
    size_t sz;

public:
    AVL() = default;

    void add(int x)
    {
        if(root == nullptr)
        {
            root = std::make_unique<Node>(x);
            ++sz;
        }
        else
        {
            if(root->node_add(x)) ++sz;
        }
    }

    void print(std::ostream& os)
    {
        if(root == nullptr) return;
        
        root->node_print(os);
        os << std::endl;
    }
    
    void remove(int x)
    {
        if(root == nullptr)  return;

        if(root->value == x)
        {
            if(sz == 1)
            {
                root = nullptr;
            }
            else
            {
                root->value = root->right_child->successor();
            }
            --sz;
        }
        else
        {
            if(root->node_remove(x)) --sz;
        }
    }

    size_t size() { return sz; }
};

int main()
{
    AVL tree;
    tree.add(2);
    tree.add(1);
    tree.add(4);
    tree.add(3);
    tree.add(5);
    tree.add(6);
    tree.print(std::cout);
    tree.remove(4);
    tree.print(std::cout);
    
    return 0;
}