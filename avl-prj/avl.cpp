#include <iostream>
#include <memory>
#include <cassert>

#define AVL_RESET 0
#define AVL_BALANCE 1
#define AVL_HEIGHT 2

class AVL
{
private:
    struct Node
    {
        int value;
        Node* parent;
        std::unique_ptr<Node> left_child = nullptr;
        std::unique_ptr<Node> right_child = nullptr;
        int balance = 0;
        size_t height = 0;

        Node(int x = 0, Node* pr = nullptr) : value(x), parent(pr)
        {
            // std::cout << "CONS " << value << std::endl;
        }

        ~Node()
        {
            // std::cout << "DEST " << value << std::endl;
        }
        
        void update()
        {
            height_update();
            balance_update();
        }

        void balance_update()
        {
            if(left_child == nullptr && right_child == nullptr) balance = 0;
            else if(left_child != nullptr && right_child == nullptr) balance = -(left_child->height);
            else if(left_child == nullptr && right_child != nullptr) balance = right_child->height;
            else balance = left_child->height - right_child->height;
        }

        void height_update()
        {
            if(left_child == nullptr && right_child == nullptr) height = 0;
            else if(left_child != nullptr && right_child == nullptr) height = left_child->height + 1;
            else if(left_child == nullptr && right_child != nullptr) height = right_child->height + 1;
            else height = std::max(left_child->height, right_child->height) + 1;
        }

        bool node_insert(int x)
        {
            bool result = false;
            if(x < value)
            {
                if(left_child == nullptr)
                {
                    left_child = std::make_unique<Node>(x, this);
                    result = true;
                }
                else
                {
                    result = left_child->node_insert(x);
                }
            }
            else if(x > value)
            {
                if(right_child == nullptr)
                {
                    right_child = std::make_unique<Node>(x, this);
                    result = true;
                }
                else
                {
                    result = right_child->node_insert(x);
                }
            }
            else
            {
                return false;
            }
            if(result) update();
            return result;
        }

        void node_print(size_t option, std::ostream& os)
        {
            os << value;
            if(option == 1)
            {
                os << '(' << balance << ')';
            }
            if(option == 2)
            {
                os << '(' << height << ')';
            }
            os << '[';
            if(left_child != nullptr)
            {
                left_child->node_print(option, os);
            }
            else
            {
                os << 'x';
            }
            os << ',';
            if(right_child != nullptr)
            {
                right_child->node_print(option, os);
            }
            else
            {
                os << 'x';
            }
            os << ']';
        }
        
        bool node_remove(int x)
        {
            bool result;
            if(x > value && right_child == nullptr) return false;
            if(x < value && left_child == nullptr) return false;

            if(x > value) result = right_child->node_remove(x);
            if(x < value) result = left_child->node_remove(x);

            if(x == value)
            {
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
            if(result) update();
            return result;
        }
        
        int successor(int k)
        {
            if(left_child != nullptr)
            {
                int result = left_child->successor(k);
                update();
                return result;
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

    void insert(int x)
    {
        if(root == nullptr)
        {
            root = std::make_unique<Node>(x);
            ++sz;
        }
        else
        {
            if(root->node_insert(x)) ++sz;
        }
    }

    void print(size_t option = 0, std::ostream& os = std::cout)
    {
        assert(option < 3);
        os << sz << ": ";
        if(root == nullptr)
        {
            os << "Empty" << std::endl;
            return;
        }
        
        root->node_print(option, os);
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

    return 0;
}