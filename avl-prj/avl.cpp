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

        Node(int x = 0, Node* pr = nullptr) : value(x), parent(pr) { std::cout << "CONS " << value << std::endl; }

        ~Node() { std::cout << "DEST " << value << std::endl; }
        
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
            os << value << '[';
            if(left_child != nullptr)
            {
                left_child->node_print(os);
            }
            else
            {
                os << 'x';
            }
            os << ',';
            if(right_child != nullptr)
            {
                right_child->node_print(os);
            }
            else
            {
                os << 'x';
            }
            os << ']';
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
                value = right_child->successor(value);
            }
            return true;
            
        }
        
        int successor(int k)
        {
            if(left_child != nullptr)
            {
                return left_child->successor(k);
            }
            else
            {
                int x = value;
                value = k;
                node_remove(value);
                return x;
            }
        }
    };

    std::unique_ptr<Node> root = nullptr;
    size_t sz = 0;

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
        os << sz << ": ";
        if(root == nullptr)
        {
            os << "Empty" << std::endl;
            return;
        }
        
        root->node_print(os);
        os << std::endl;
    }
    
    void remove(int x)
    {
        if(root == nullptr)  return;

        if(root->value == x)
        {
            if(root->left_child == nullptr && root->right_child == nullptr)
            {
                root = nullptr;
            }
            else if(root->left_child != nullptr && root->right_child == nullptr)
            {
                Node* temp = root.get();
                std::swap(root->left_child, root);
                temp->left_child = nullptr;
            }
            else if(root->left_child == nullptr && root->right_child != nullptr)
            {
                Node* temp = root.get();
                std::swap(root->right_child, root);
                temp->right_child = nullptr;
            }
            else
            {
                root->value = root->right_child->successor(root->value);
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
    tree.print(std::cout);
    tree.add(2);
    tree.print(std::cout);
    tree.add(3);
    tree.add(1);
    tree.print(std::cout);
    tree.remove(2);
    tree.print(std::cout);
    tree.remove(3);
    tree.print(std::cout);
    tree.remove(1);
    tree.print(std::cout);
    return 0;
}