#include <string>
#include "Node.h"

template<class K, class V>
class Map {
private:
    Node<K, V> *root = nullptr;
    int length;

    void insert(Node<K, V> *node);

public:
    Map();

    ~Map();

    Map(const Map<K, V> &source);

    void clean();

    int getLength() const;

    void writeToFile(const std::string &path) const;

    void loadFromFile(const std::string &path);

    void insert(const K &key, const V &value);

    void remove(const K &key);

    bool check(const K &key) const;

    Collection<V> *ascendingOrder() const;

    Collection<Node<K, V>> *ascendingOrderOfNodes() const;

    bool operator==(const Map<K, V> &another) const;

    V &operator[](const K &key) const;
};

#include "Map.inl"
