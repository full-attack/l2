template<class K, class V>
Map<K, V>::Map() {
    length = 0;
}

template<class K, class V>
Map<K, V>::~Map() {
    delete root;
}

template<class K, class V>
Map<K, V>::Map(const Map<K, V> &source) {
    this->root = new Node<K, V>(*(source.root));
    this->length = source.getLength();
}

template<class K, class V>
int Map<K, V>::getLength() const {
    return this->length;
}

template<class K, class V>
void Map<K, V>::insert(const K &key, const V &value) {
    auto *newNode = new Node<K, V>{key, value};
    this->insert(newNode);
}

template<class K, class V>
void Map<K, V>::insert(Node<K, V> *node) {
    auto *newNode = node;
    bool added;
    if (this->root == nullptr) {
        added = true;
        this->root = newNode;
    } else {
        added = this->root->insert(*newNode);
    }
    if (added) {
        this->length++;
    }
}

template<class K, class V>
V &Map<K, V>::operator[](const K &key) const {
    if (this->root == nullptr) {
        throw MapKeyError();
    }

    return this->root->find(key);
}

template<class K, class V>
bool Map<K, V>::operator==(const Map<K, V> &another) const {
    if (this->getLength() != another.getLength()) {
        return false;
    }
    if (this->root != nullptr) {
        return this->root->compareTrees(*(another.root));
    } else {
        return another.root == nullptr;
    }
}

template<class K, class V>
bool Map<K, V>::check(const K &key) const {
    if (this->root == nullptr) {
        return false;
    }
    try {
        this->root->find(key);
    } catch (MapKeyError &) {
        return false;
    }
    return true;
}

template<class K, class V>
void Map<K, V>::clean() {
    delete this->root;
    this->root = nullptr;
    length = 0;
}

template<class K, class V>
void Map<K, V>::remove(const K &key) {
    if (this->root == nullptr) {
        throw NoMapError();
    }
    try {
        if (this->root->remove(key)) {
            this->root->changeNode(&(this->root));
        }
        this->length--;
    } catch (MapKeyError&) {
        return;
    }
}

template<class K, class V>
Collection<V> *Map<K, V>::ascendingOrder() const {
    Collection<V> *collection = this->root->ascendCollection();
    return collection;
}

template<class K, class V>
Collection<Node<K, V>> *Map<K, V>::ascendingOrderOfNodes() const {
    Collection<Node<K, V>> *collection = this->root->ascendNodes();
    return collection;
}

template<class K, class V>
void Map<K, V>::writeToFile(const std::string &path) const {
    QFile file(QString{path.c_str()});
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    this->root->writeToFile(out);
    file.close();
}

template<class K, class V>
void Map<K, V>::loadFromFile(const std::string &path) {
    QFile file(QString{path.c_str()});
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);
    this->clean();
    K key;
    V value;
    while (!in.atEnd()) {
        in >> key;
        in >> value;
        Node<K, V> *node = new Node<K, V>{key, value};
        this->insert(node);
    }
    file.close();
}
