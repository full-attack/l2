#include <string>
#include "Collection.h"
#include "QFile"
#include "QDataStream"
#include "QIODevice"


class MapKeyError : public std::exception {
private:
    std::string message;
public:
    MapKeyError() {
        message = std::string{"No such key"};
    }

    const char *what() const noexcept override { return message.c_str(); }
};

class NoMapError : public std::exception {
private:
    std::string message;
public:
    NoMapError() {
        message = std::string{"No map!"};
    }

    const char *what() const noexcept override { return message.c_str(); }
};

template<class K, class V>
class Node {
private:
    Node<K, V> *left = nullptr;
    Node<K, V> *right = nullptr;
    K key;
    V value;

    bool addToRight(Node<K, V> &newNode) {
        if (this->right != nullptr) {
            return this->right->insert(newNode);
        } else {
            this->right = &newNode;
        }
        return true;
    }

    bool addToLeft(Node<K, V> &newNode) {
        if (this->left != nullptr) {
            return this->left->insert(newNode);
        } else {
            this->left = &newNode;
        }
        return true;
    }

    Node<K, V> *getLeftestRoot() {
        if (this->right->left == nullptr) {
            return this;
        } else {
            auto *node = this->right->left;
            for (; node->left->left != nullptr; node = node->left);
            return node;
        }
    }

    Node() {

    }

    Node<K, V> *getReplacement(Node<K, V> &toRemove) const {
        if (toRemove.left == nullptr && toRemove.right == nullptr) {
            return nullptr;
        } else if (toRemove.right == nullptr) {
            return toRemove.left;
        } else if (toRemove.left == nullptr) {
            return toRemove.right;
        } else {
            Node<K, V> *leftestRoot = toRemove.getLeftestRoot();
            Node<K, V> *leftest;
            Node<K, V> **pointerToDiscard;
            if (leftestRoot == &toRemove) {
                leftest = toRemove.right;
                pointerToDiscard = &(toRemove.right);
            } else {
                leftest = leftestRoot->left;
                pointerToDiscard = &leftestRoot->left;
            }
            auto *replacement = new Node<K, V>();
            replacement->key = leftest->key;
            replacement->value = leftest->value;
            replacement->left = toRemove.left;
            replacement->right = toRemove.right;
            if (leftest->right == nullptr) {
                *pointerToDiscard = nullptr;
            } else {
                **pointerToDiscard = *(leftest->right);
            }
            return replacement;
        }
    }

public:
    Node(K key, V value) {
        this->key = key;
        this->value = value;
    }

    Node(const Node<K, V> &source) {
        this->key = K{source.key};
        this->value = V{source.value};
        if (source.left != nullptr) {
            this->left = new Node<K, V>(*(source.left));
        }
        if (source.right != nullptr) {
            this->right = new Node<K, V>(*(source.right));
        }
    }

    ~Node() {
        if (left != nullptr) {
            delete left;
            left = nullptr;
        }
        if (right != nullptr) {
            delete right;
            right = nullptr;
        }
    }

    V getValue() {
        return this->value;
    }


    K getKey() {
        return this->key;
    }

    bool insert(Node<K, V> &newNode) {
        bool isAdded;
        if (newNode.key > this->key) {
            isAdded = addToRight(newNode);
        } else if (this->key > newNode.key) {
            isAdded = addToLeft(newNode);
        } else {
            this->value = newNode.value;
            isAdded = false;
        }
        return isAdded;
    }

    V &find(const K &searchKey) {
        if (this->key > searchKey) {
            if (this->left == nullptr) {
                throw MapKeyError();
            }
            this->left->find(searchKey);
        } else if (searchKey > this->key) {
            if (this->right == nullptr) {
                throw MapKeyError();
            }
            this->right->find(searchKey);
        } else {
            return this->value;
        }
    }

    bool compareTrees(const Node<K, V> &rootNode) {
        try {
            V thisValue = this->find(rootNode.key);
            if (thisValue != rootNode.value) {
                return false;
            }
            const Node<K, V> *leftTree = rootNode.left;
            const Node<K, V> *rightTree = rootNode.right;
            if (leftTree != nullptr && rightTree != nullptr) {
                return this->compareTrees(*leftTree) &&
                        this->compareTrees(*rightTree);
            }
            return true;
        } catch (MapKeyError &) {
            return false;
        }
    }

    void changeNode(Node<K, V> **node) {
        auto *replacement = this->getReplacement(**node);
        if (replacement != nullptr) {
            (*node)->key = replacement->key;
            (*node)->value = replacement->value;
            (*node)->right = replacement->right;
            (*node)->left = replacement->left;
        } else {
            (*node) = nullptr;
        }
    }

    bool remove(const K &removeKey) {
        if (removeKey > this->key) {
            if (this->right == nullptr) {
                throw MapKeyError();
            }
            if (this->right->remove(removeKey)) {
                this->changeNode(&(this->right));
            }
        } else if (this->key > removeKey) {
            if (this->left == nullptr) {
                throw MapKeyError();
            }
            if (this->left->remove(removeKey)) {
                this->changeNode(&(this->left));
            }
        } else {
            return true;
        }
        return false;
    }

    Collection<V> *ascendCollection(
            Collection<V> *collection = new Collection<V>{}) {
        if (this == nullptr) {
            return nullptr;
        }
        this->left->ascendCollection(collection);
        collection->addValue(this->value);
        this->right->ascendCollection(collection);
        return collection;
    }

    Collection<Node<K, V>> *ascendNodes(
            Collection<Node<K, V>> *collection = \
            new Collection<Node<K, V>>{}) {
        if (this == nullptr) {
            return nullptr;
        }
        this->left->ascendNodes(collection);
        K copyKey{this->key};
        V copyValue{this->value};
        Node<K, V> *node = new Node<K, V>{copyKey, copyValue};
        collection->addValue(*node);
        this->right->ascendNodes(collection);
        return collection;
    }

    void writeToFile(QDataStream &file) const {
        if (this == nullptr) {
            return;
        }
        file << this->key << this->value;
        this->left->writeToFile(file);
        this->right->writeToFile(file);
    }

    static Node<K, V> *parseNode(std::string &line) {
        K key = K{line.substr(0, line.find(": "))};
        V value = V{line.substr(line.rfind(": ") + 2,
                                line.length())};
        auto *newNode = new Node<K, V>{key, value};
        return newNode;
    }
};
