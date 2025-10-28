#include <iostream>
#include <vector>
using namespace std;

template <typename T>
class Array {
private:
    vector<T> data;
    template <typename U>
    friend ostream& operator<<(ostream& os, const Array<U>& ary);

public:
    Array() = default;
    Array(initializer_list<T> list) : data(list) {}

    size_t size() const {
        return data.size();
    }

    void append(const T& value) {
        data.push_back(value);
    }

    void remove(const T& value) {
        auto it = find(data.begin(), data.end(), value);
        if (it != data.end()) {
            data.erase(it);
        }
    }

    Array<T> operator+(const Array<T>& other) const {
        Array<T> result = *this;
        result.data.insert(result.data.end(), other.data.begin(), other.data.end());
        return result;
    }
};

template <typename T>
ostream& operator<<(ostream& os, const Array<T>& ary) {
    os << "[";
    for (size_t i = 0; i < ary.data.size(); i++) {
        os << ary.data[i];
        if (i < ary.data.size() - 1) {
            os << " ";
        }
    }
    os << "]";
    return os;
}
template <typename T>
size_t len(const Array<T>& ary) {
    return ary.size();
}

int main(void) {
    Array<int> ary1 = {1, 2, 3, 4, 5};
    cout << ary1 << endl;
    cout << len(ary1) << endl;
    Array<int> ary2 = {6, 7};
    ary1 = ary1 + ary2;
    cout << ary1 << endl;
    return 0;
}