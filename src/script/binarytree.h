// Copyright (c) 2018 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#ifndef BITCOIN_BINARYTREE_H
#define BITCOIN_BINARYTREE_H


#include <script/script.h>
#include <util/memory.h>
#include <memory>


template <typename T>
class BinaryTree
{
protected:
    BinaryTree* parent = nullptr;
    std::unique_ptr<BinaryTree> left = nullptr;
    std::unique_ptr<BinaryTree> right = nullptr;
    std::unique_ptr<T> obj = nullptr;

public:
    bool IsRoot() const { return parent == nullptr; }
    bool IsLeaf() const { return obj != nullptr; }
    bool IsNode() const { return !IsLeaf(); }
    bool IsEmpty() const { return !left && !right && !IsLeaf(); }
    bool IsFull() const { return (left && right) || IsLeaf(); }

    const T* GetObj() const
    {
        if (!IsLeaf()) return nullptr;
        return obj.get();
    }

    bool InsertLeaf(std::unique_ptr<T> sc)
    {
        if (!IsEmpty()) return false;
        if (!sc) return false;
        obj = std::move(sc);
        return true;
    }
    bool InsertRight(BinaryTree child)
    {
        if (right) return false;
        child.parent = this;
        right = MakeUnique<BinaryTree>(std::move(child));
        return true;
    }
    bool InsertLeft(BinaryTree child)
    {
        if (left) return false;
        child.parent = this;
        left = MakeUnique<BinaryTree>(std::move(child));
        return true;
    }
    BinaryTree* GetInitLeftChild()
    {
        if (IsLeaf()) return nullptr;
        if (!left) left = MakeUnique<BinaryTree>(this);
        return left.get();
    }
    BinaryTree* GetInitRightChild()
    {
        if (!IsNode()) return nullptr;
        if (!right) right = MakeUnique<BinaryTree>(BinaryTree(this));
        return right.get();
    }

    BinaryTree* GetInitRightSibling()
    {
        if (!parent) return nullptr;
        if (!parent->right) parent->right = MakeUnique<BinaryTree>(BinaryTree(parent));
        if ((&*parent->right) == this) return nullptr;
        return parent->right.get();
    }
    BinaryTree* GetInitLeftSibling()
    {
        if (!parent) return nullptr;
        if (!parent->left) parent->left = MakeUnique<BinaryTree>(BinaryTree(parent));
        return parent->left.get();
    }

    bool RemoveLeftChild()
    {
        if (!left) return false;
        left.reset();
        return true;
    }

    std::unique_ptr<T> ReplaceLeaf(std::unique_ptr<T> sc)
    {
        obj.swap(sc);
        return sc;
    }

    BinaryTree* GetRawRightChild() const { return right.get(); }
    BinaryTree* GetRawLeftChild() const { return left.get(); }
    BinaryTree* GetParent() const { return parent; }

    explicit BinaryTree() = default;
    explicit BinaryTree(T sc) {obj = MakeUnique(std::move(sc));}
    explicit BinaryTree(std::unique_ptr<T> sc) {obj = std::move(sc);}
    explicit BinaryTree(BinaryTree* par) {parent = par;}

};


#endif
