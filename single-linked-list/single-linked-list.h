#include <cassert>
#include <cstddef>
#include <string>
#include <utility>
#include <iostream>

template <typename Type>
class SingleLinkedList {
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };
    
     template <typename ValueType>
    class BasicIterator {
    
        friend class SingleLinkedList;
        explicit BasicIterator(Node* node) 
            : node_(node)
        {}

    public:

        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;

        BasicIterator() = default;

        BasicIterator(const BasicIterator<Type>& other) noexcept 
            : node_(other.node_)
        {}

        BasicIterator& operator=(const BasicIterator& rhs) = default;

        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return this->node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return !(this->node_ == rhs.node_);
        }

        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return this->node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return !(this->node_ == rhs.node_);
        }

        BasicIterator& operator++() noexcept {
            assert(node_ != nullptr);
            node_= node_->next_node;
            return *this;
        }

        BasicIterator operator++(int) noexcept {
            assert(node_ != nullptr);
            auto old_value(*this); 
            ++(*this);
            return old_value;

        }

        [[nodiscard]] reference operator*() const noexcept {
            assert(node_ != nullptr);
            return node_->value;
        }

        [[nodiscard]] pointer operator->() const noexcept {
            assert(node_ != nullptr);
            return &(node_->value);
        }

    private:
        Node* node_ = nullptr;
    };
    
    public:
    SingleLinkedList() {
        size_ = 0;
    }
    
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;

    [[nodiscard]] Iterator begin() noexcept {
        return Iterator{ head_.next_node };
        return {};
    }

    [[nodiscard]] Iterator end() noexcept {
        return Iterator{ nullptr };
        return {};
    }

    [[nodiscard]] ConstIterator begin() const noexcept {
        return ConstIterator{ head_.next_node };
        return {};
    }

    [[nodiscard]] ConstIterator end() const noexcept {
        return ConstIterator{ nullptr };
        return {};
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return begin();
        return {};
    }

    [[nodiscard]] ConstIterator cend() const noexcept {
        return end();
        return {};
    }
    
    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator{&head_};
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator{const_cast<Node*>(&head_)};
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return ConstIterator{&head_};
    }
    
    SingleLinkedList(std::initializer_list<Type> values) {
        for (auto it = values.end(); it != values.begin();) {
            --it;
            PushFront(*it);
        }
    }

    SingleLinkedList(const SingleLinkedList& other) {
        assert(size_ == 0 && head_.next_node == nullptr);
        SingleLinkedList temp;
        for (size_t i = other.size_; i > 0; --i) {
            auto it = other.cbegin();
            for (size_t j = i - 1; j > 0; --j) {
                ++it;
            }
            temp.PushFront(*it);
        }
        swap(temp);
    }
    
    ~SingleLinkedList() {
        Clear();
    }
    
    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }
    
     Iterator InsertAfter(ConstIterator pos, const Type& value) {
        assert(&pos != nullptr);
        pos.node_->next_node = new Node(value, pos.node_->next_node);
        ++size_;
        return Iterator{ pos.node_->next_node };
    }

    void PopFront() noexcept {
        assert(size_ > 0);
        auto to_remove = cbefore_begin().node_->next_node;
        cbefore_begin().node_->next_node = to_remove->next_node;
        delete to_remove;
        --size_;
    }

    Iterator EraseAfter(ConstIterator pos) noexcept {
        assert(&pos != nullptr);
        auto to_remove = pos.node_->next_node;
        assert(&to_remove != nullptr);
        pos.node_->next_node = to_remove->next_node;
        delete to_remove;
        --size_;
        return Iterator{pos.node_->next_node};
    }

    void Clear() noexcept {
        while (head_.next_node != nullptr) {
            Node* next = head_.next_node->next_node;
            delete head_.next_node;
            head_.next_node = next;
        }
        size_ = 0;
    }
    
    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    [[nodiscard]] bool IsEmpty() const noexcept {
        return size_ == 0;
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        if (this != &rhs) {
            SingleLinkedList temp(rhs);
            swap(temp);
        }
        return *this;
    }
    
    void swap(SingleLinkedList& other) noexcept {
        std::swap(head_.next_node, other.head_.next_node);
        std::swap(size_, other.size_);
    }

private:
    Node head_;
    size_t size_ = 0;
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    if (lhs.GetSize() != rhs.GetSize()) {
        return false;
    }
    else {
        auto it1 = lhs.begin();
        auto it2 = rhs.begin();
        while (it1 != lhs.end() && it2 != rhs.end()) {
            if (*it1 != *it2) {
                return false;
            }
            ++it1;
            ++it2;
        }
        return true;
    }
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    auto it1 = lhs.begin();
    auto it2 = rhs.begin();

    while (it1 != lhs.end() && it2 != rhs.end()) {
        if (*it1 != *it2) {
            return *it1 < *it2;
        }
        ++it1;
        ++it2;
    }
    return (lhs.GetSize() < rhs.GetSize());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return (lhs < rhs) || (lhs == rhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return (rhs < lhs);
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs > lhs) || (lhs == rhs);
}
