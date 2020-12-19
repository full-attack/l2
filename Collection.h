#include "string"
#include "fstream"
#include "cassert"
#include <sstream>

template<class V>
class Collection {
private:
    int length;
    int size = 10;
    V **collection;

public:
    Collection();

    Collection(const Collection<V> &source);

    ~Collection();

    void addValue(V &value);

    void setAt(const V &value, int pos);

    void insertAt(const V &value, int pos);

    void deleteValue(int pos);

    void erase();

    V &at(int pos) const;

    int getLength() const;

    void print() const;
};

#include "Collection.inl"