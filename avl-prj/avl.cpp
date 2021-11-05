#include <iostream>
#include <memory>
#include <cassert>
#include <limits>

#define AVL_RESET 0
#define AVL_BALANCE 1
#define AVL_HEIGHT 2
#define AVL_PARENT 3

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
        int height = 0;

        Node(int x = 0, Node* pr = nullptr) : value(x), parent(pr)
        {
            /*
            if(value == std::numeric_limits<int>::min())
                std::cout << "CONS DUMMY" << std::endl;
            else
                std::cout << "CONS " << value << std::endl;
            */
        }

        ~Node()
        {
            /*
            if(value == std::numeric_limits<int>::min())
                std::cout << "DEST DUMMY" << std::endl;
            else
                std::cout << "DEST " << value << std::endl;
            */
        }

        void node_update()
        {
            height_check();
            balance_check();
        }

        void balance_check()
        {
            if(left_child == nullptr && right_child == nullptr) balance = 0;
            else if(left_child != nullptr && right_child == nullptr) balance = -(left_child->height + 1);
            else if(left_child == nullptr && right_child != nullptr) balance = right_child->height + 1;
            else balance = right_child->height - left_child->height;
        }

        void height_check()
        {
            if(left_child == nullptr && right_child == nullptr) height = 0;
            else if(left_child != nullptr && right_child == nullptr) height = left_child->height + 1;
            else if(left_child == nullptr && right_child != nullptr) height = right_child->height + 1;
            else height = std::max(left_child->height, right_child->height) + 1;
        }

        void node_balance()
        {
            std::unique_ptr<Node>* ptr;
            if(parent->value > value) ptr = &(parent->left_child);
            else ptr = &(parent->right_child);

            if(balance > 0)
            {
                if(right_child->balance > 0)
                {
                    left_rotate(*ptr);
                }
                else
                {
                    right_child->right_rotate(right_child);
                    left_rotate(*ptr);
                }
            }
            else
            {
                if(left_child->balance > 0)
                {
                    left_child->left_rotate(left_child);
                    right_rotate(*ptr);
                }
                else
                {
                    right_rotate(*ptr);
                }
            }
        }

        void left_rotate(std::unique_ptr<Node>& par)
        {
            std::unique_ptr<Node> temp = nullptr;
            std::swap(right_child, temp);
            std::swap(temp, par);
            std::swap(par->left_child, temp->right_child);
            std::swap(par->left_child, temp);
            std::swap(par->parent, par->left_child->parent);
            par->left_child->parent = par.get();
            par->left_child->node_update();
            par->node_update();
            if(par->left_child->right_child != nullptr)
                par->left_child->right_child->parent = par->left_child.get();
        }

        void right_rotate(std::unique_ptr<Node>& par)
        {
            std::unique_ptr<Node> temp = nullptr;
            std::swap(left_child, temp);
            std::swap(temp, par);
            std::swap(par->right_child, temp->left_child);
            std::swap(par->right_child, temp);
            std::swap(par->parent, par->right_child->parent);
            par->right_child->parent = par.get();
            par->right_child->node_update();
            par->node_update();
            if(par->right_child->left_child != nullptr)
                par->right_child->left_child->parent = par->right_child.get();
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
            if(result)
            {
                node_update();
                if(std::abs(balance) > 1)
                {
                    if(parent != nullptr)
                    {
                        node_balance();
                    }
                }
            }
            return result;
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
                        right_child->parent = parent;
                        std::swap(parent->left_child, right_child);
                        right_child = nullptr;
                    }
                    else
                    {
                        right_child->parent = parent;
                        std::swap(parent->right_child, right_child);
                        right_child = nullptr;
                    }
                }
                else if(left_child != nullptr && right_child == nullptr)
                {
                    if(parent->value > x)
                    {
                        left_child->parent = parent;
                        std::swap(parent->left_child, left_child);
                        right_child = nullptr;
                    }
                    else
                    {
                        left_child->parent = parent;
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
            if(result)
            {
                node_update();
                if(std::abs(balance) > 1)
                {
                    if(parent != nullptr)
                    {
                        node_balance();
                    }
                }
            }
            return result;
        }
        
        int successor(int k)
        {
            if(left_child != nullptr)
            {
                int result = left_child->successor(k);
                node_update();
                if(std::abs(balance) > 1)
                {
                    if(parent != nullptr)
                    {
                        node_balance();
                    }
                }
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

        void node_print(int option, std::ostream& os)
        {
            os << value;
            if(option == 1)
            {
                os << '(' << balance << ')';
            }
            else if(option == 2)
            {
                os << '(' << height << ')';
            }
            else if(option == 3)
            {
                if(parent->value == std::numeric_limits<int>::min())
                    os << "(root)";
                else
                    os << '(' << parent->value << ')';
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

        bool node_find(int x)
        {
            if(x > value && right_child == nullptr) return false;
            if(x < value && left_child == nullptr) return false;

            if(x > value) return right_child->node_find(x);
            if(x < value) return left_child->node_find(x);
            return true;
        }
    };

    std::unique_ptr<Node> dummy;
    size_t sz = 0;

public:
    AVL() : dummy(std::make_unique<Node>(std::numeric_limits<int>::min())) {}
    
    void insert(int x)
    {
        if(dummy->node_insert(x)) ++sz;
    }
    
    void remove(int x)
    {
        if(dummy->node_remove(x)) --sz;
    }
    
    size_t size() { return sz; }
    
    void print(int option = 1, std::ostream& os = std::cout)
    {
        assert(option < 4);
        os << sz << ": ";
        if(dummy->right_child == nullptr)
        {
            os << "Empty" << std::endl;
            return;
        }
        dummy->right_child->node_print(option, os);
        os << std::endl;
    }
    
    bool find(int x)
    {
        return dummy->node_find(x);
    }
};

int main()
{
    AVL tree;
    for(int i = 0; i < 1000; ++i)
    {
        tree.insert(i);
    }
    tree.remove(253);
    std::cout << tree.find(253);
}