#pragma once

#include "Tree.hpp"
#include "BinarySearchTree.hpp"

namespace Datastructures
{
namespace Trees
{   
    template<
        typename Type
    >
    class AVLTree
    {

    public:

        struct Node
        {
            Node(const Type& data) :
                left(nullptr),
                right(nullptr),
                key(data),
                height(1),
                count(1)
            {
            }

            bool inline operator <(const Node& rhs) const
            {
                return key < rhs.key;
            }

            bool inline operator >(const Node& rhs) const
            {
                return key > rhs.key;
            }

            bool inline operator <(const Type& rhs) const
            {
                return key < rhs;
            }

            bool inline operator >(const Type& rhs) const
            {
                return key > rhs;
            }

            unsigned int height;
            unsigned int count;

            Type key;

            Node* left;
            Node* right;
        };

        using node_ptr = Node*;

        static inline unsigned int NodeHeight(node_ptr node)
        {
            if (!node) { return 0; }
            return node->height;
        }

        static inline void UpdateHeight(node_ptr node)
        {
            if (!node) { return; }
            node->height = std::max(AVLTree::NodeHeight(node->left), AVLTree::NodeHeight(node->right)) + 1;
        }

        static inline int BalanceFactor(node_ptr node)
        {
            if (!node) { return 0; }
            return AVLTree::NodeHeight(node->left) - AVLTree::NodeHeight(node->right);
        }
        
        static node_ptr RR(node_ptr A)
        {
            node_ptr B  = A->right;
            A->right    = B->left;
            B->left     = A;

            // Update the heights
            AVLTree::UpdateHeight(A);
            AVLTree::UpdateHeight(B);

            return B;
        }

        static node_ptr LL(node_ptr A)
        {
            node_ptr B  = A->left;
            A->left     = B->right;
            B->right    = A;

            // Update the heights
            AVLTree::UpdateHeight(A);
            AVLTree::UpdateHeight(B);

            return B;
        }

        static node_ptr RL(node_ptr A)
        {
            A->right = LL(A->right);
            return RR(A);
        }

        static  node_ptr LR(node_ptr A)
        {
            A->left = RR(A->left);
            return LL(A);
        }

        static node_ptr Insert(node_ptr node, const Type& key, bool& balance)
        {

            // Current spot is empty, just return it
            if (!node) { return new Node(key); }

            // We need to search in left tree, node is less than current root
            if (*node > key)
            {
                node->left = Insert(node->left, key, balance);
            }
            // We need to search in right tree, node is gets current root
            else if (*node < key)
            {
                node->right = Insert(node->right, key, balance);
            }
            else
            {
                // Just increment a count.  No need to balance, just return all the way up
                // The assumption here is that the tree is balanced before the insert.  Incrementing the counter
                // won't unbalance the trees
                ++(node->count);
                balance = false;
                return node;
            }

            // If we don't need to balance anymore, just unwind the callstack
            if (!balance)
            {
                return node;
            }

            // Update the new heights
            AVLTree::UpdateHeight(node);

            // Get the balance factor of the root
            int balance_factor = AVLTree::BalanceFactor(node);

            // there is an imbalance on the left side
            if (balance_factor >= 2)
            {
                if (*(node->left) > key)
                {
                    node = AVLTree::LL(node);
                }
                else
                {
                    node = AVLTree::LR(node);
                }

                // We don't need to balance anymore
                balance = false;
            }
            // there is an imbalance on the right side
            else if (balance_factor <= -2)
            {
                if (*(node->right) < key)
                {
                    node = AVLTree::RR(node);
                }
                else
                {
                    node = AVLTree::RL(node);
                }

                // We don't need to balance anymore
                balance = false;
            }
            else if (!balance_factor)
            {
                // We don't need to balance anymore
                balance = false;
            }

            return node;
        }

        static node_ptr Remove(node_ptr node, const Type& key, bool& balance)
        {
            if (!node) { return node; }

            if (*node > key)
            {
                node->left = AVLTree::Remove(node->left, key, balance);
            }
            else if (*node < key)
            {
                node->right = AVLTree::Remove(node->right, key, balance);
            }
            else
            {
                --(node->count);

                if (node->count)
                {
                    balance = false;
                    return node;
                }

                if (!node->left)
                {
                    node_ptr temp = node->right;
                    delete node;
                    return temp;
                }
                else if (!node->right)
                {
                    node_ptr temp = node->left;
                    delete node;
                    return temp;
                }

                node_ptr temp = BinarySearchTree<Type, AVLTree::Node>::MinValue(node->right);
                node->key = temp->key;
                node->right = AVLTree::Remove(node->right, key, balance);
            }

            if (!node) { return node; }

            // Update the height
            AVLTree::UpdateHeight(node);

            // Get the balance factor
            int balance_factor = AVLTree::BalanceFactor(node);

            if (balance_factor >= 2)
            {
                int left_balance = AVLTree::BalanceFactor(node->left);

                if (left_balance >= 0)
                {
                    node = AVLTree::LL(node);
                }
                else if (left_balance < 0)
                {
                    node = AVLTree::LR(node);
                }
            }
            else if (balance_factor <= -2)
            {
                int right_balance = AVLTree::BalanceFactor(node->right);

                if (right_balance <= 0)
                {
                    node = RR(node);
                }
                else if (right_balance > 0)
                {
                    node = RL(node);
                }
            }

            return node;
        }

    public:
        AVLTree():
            m_root(nullptr)
        {

        }

        AVLTree(AVLTree&&) = delete;
        AVLTree(AVLTree&) = delete;

        void Insert(const Type& data)
        {
            // feed in the boolean to track balancing
            bool balance = true;
            m_root = AVLTree::Insert(m_root, data, balance);
        }

        void Remove(const Type& data)
        {
            bool balance = true;
            AVLTree::Remove(m_root, data, balance);
        }

    private:
        
        // Hold the actual elements
        node_ptr m_root;
    };
}
}