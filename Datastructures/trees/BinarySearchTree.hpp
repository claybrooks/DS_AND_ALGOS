#pragma once

#include "Tree.hpp"

namespace Datastructures
{
    namespace Trees
    {
        template<typename Type>
        struct BSTNode
        {
            BSTNode(const Type& data) :
                left(nullptr),
                right(nullptr),
                key(data),
                count(1)
            {
            }

            bool inline operator <(const BSTNode& rhs) const
            {
                return key < rhs.key;
            }

            bool inline operator >(const BSTNode& rhs) const
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

            unsigned int count;

            Type key;

            BSTNode* left;
            BSTNode* right;
        };

        template<
            typename Type,
            typename NodeType = BSTNode<Type>
        >
            class BinarySearchTree
        {

        public:

            using node_ptr = NodeType*;

            static node_ptr MinValue(node_ptr node)
            {
                if (!node) { throw Exceptions::NullNodeException(); }

                node_ptr iter = node;
                while (iter->left)
                {
                    iter = iter->left;
                }
                return iter;
            }

            static node_ptr MaxValue(node_ptr node)
            {
                if (!node) { throw Exceptions::NullNodeException(); }

                node_ptr iter = node;
                while (iter->right)
                {
                    iter = iter->right;
                }
                return iter;
            }

            static void InOrder(node_ptr node, std::vector<node_ptr>& inorder)
            {
                if (!root) { return; }

                BinarySearchTree::InOrder(node->left, inorder);
                inorder.push_back(node);
                BinarySearchTree::InOrder(node->right, inorder);
            }

            static node_ptr Insert(node_ptr node, const Type& key)
            {

                // Current spot is empty, just return it
                if (!node) { return new NodeType(key); }

                // We need to search in left tree, node is less than current root
                if (*node > key)
                {
                    node->left = Insert(node->left, key);
                }
                // We need to search in right tree, node is gets current root
                else if (*node < key)
                {
                    node->right = Insert(node->right, key);
                }
                else
                {
                    // Increment the count of this object
                    ++(node->count);
                }

                return node;
            }

            static node_ptr Remove(node_ptr node, const Type& key)
            {
                if (!node) { return node; }

                if (*node > key)
                {
                    node->left = Remove(node->left, key);
                }
                else if (*node < key)
                {
                    node->right = Remove(node->right, key);
                }
                else
                {
                    // Decrement the count
                    --(node->count);

                    // There is still some left
                    if (node->count) { return node; }

                    // Single child cases
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

                    // Prefer successor
                    node_ptr temp = BinarySearchTree::MinValue(node->right);

                    // copy over key
                    node->key = temp->key;

                    //delete the node
                    node->right = BinarySearchTree::Remove(node->right, temp->key);
                }

                return node;
            }

        public:
            BinarySearchTree() :
                m_root(nullptr)
            {
            }

            BinarySearchTree(BinarySearchTree&&) = delete;
            BinarySearchTree(BinarySearchTree&) = delete;

            void Insert(const Type& data)
            {
                m_root = BinarySearchTree::Insert(m_root, data);
            }

            void Remove(const Type& data)
            {
                m_root = BinarySearchTree::Remove(m_root, data);
            }

            void InOrder(std::vector<node_ptr>& inorder)
            {
                BinarySearchTree::InOrder(m_root, inorder);
            }

        private:

            // Hold the actual elements
            node_ptr m_root;
        };
    }
}