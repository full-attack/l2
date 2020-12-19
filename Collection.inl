template<class V>
Collection<V>::Collection() {
    length = 0;
    collection = new V *[size];
}

template<class V>
void Collection<V>::addValue(V &value) {
    if (length == size) {
        size += 10;
        auto **new_collection = new V *[size];
        for (int index = 0; index < length; index++) {
            new_collection[index] = collection[index];
        }
        delete[] collection;
        collection = new_collection;
    }
    collection[length] = &value;
    length++;
}

template<class V>
V &Collection<V>::at(int pos) const {
    assert(pos < length);
    return *collection[pos];
}

template<class V>
int Collection<V>::getLength() const {
    return length;
}

template<class V>
Collection<V>::~Collection() {
    delete[] collection;
}

template<class V>
void Collection<V>::deleteValue(int pos) {
    assert(pos >= 0 && pos < length);
    if (pos != length - 1) {
        for (int i = pos; i < length - 1; i++) {
            collection[i] = collection[i + 1];
        }
    }
    length--;
}

template<class V>
Collection<V>::Collection(const Collection <V> &source) {
    length = source.length;
    collection = new V *[source.size];
    for (int index = 0; index < source.length; index++) {
        collection[index] = source.collection[index];
    }
}

template<class V>
void Collection<V>::erase() {
    size = 10;
    length = 0;
    delete[] collection;
    collection = new V *[size];
}

template<class V>
void Collection<V>::setAt(const V &value, int pos) {
    assert(pos >= 0);
    assert(pos < length);
    *collection[pos] = value;
}

template<class V>
void Collection<V>::insertAt(const V &value, int pos) {
    assert(pos >= 0);
    assert(pos < length);
    if (this->length == this->size) {
        this->size += 10;
        auto *new_collection = new V *[this->size];
        for (int index = 0; index < this->length; index++) {
            new_collection[index] = this->collection[index];
        }
        delete[] this->collection;
        this->collection = new_collection;
    }
    for (int i = length - 1; i >= pos; i--) {
        this->collection[i + 1] = this->collection[i];
    }
    *collection[pos] = value;
    this->length++;
}

