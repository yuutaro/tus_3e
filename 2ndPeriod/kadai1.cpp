#include <iostream>
#include <vector>
#include <string>

template<typename T>
class HashTable {
private:
  static const int SIZE = 100;
  std::vector<std::pair<std::string, T>> table[SIZE];

  // ハッシュ関数
  int hash(const std::string& key) {
    int hash = 0;
    for (char c : key) {
      hash += c;
    }
    return hash % SIZE;
  }

public:
  // コンストラクタ
  HashTable() {}

  // キーによる探索
  T operator()(const std::string& key) {
    int index = hash(key);
    for (const auto& item : table[index]) {
      if (item.first == key) {
        return item.second;
      }
    }
      // キーが見つからない場合はデフォルト値を返す
    return T();
  }

  // アイテムの挿入
  void insert(const std::string& key, const T& value) {
    int index = hash(key);
    table[index].push_back({key, value});
  }

  // アイテムの削除
  void operator-=(const std::string& key) {
    int index = hash(key);
    auto& items = table[index];
    for (auto it = items.begin(); it != items.end(); ++it) {
      if (it->first == key) {
        items.erase(it);
        break;
      }
    }
  }
};

int main() {
    HashTable<int> table;

    // 挿入
    table.insert("hoge", 10);
    table.insert("foo", 20);
    table.insert("bar", 30);

    // 探索
    std::cout << "hoge: " << table("hoge") << std::endl;
    std::cout << "foo: " << table("foo") << std::endl;
    std::cout << "baz: " << table("baz") << std::endl;

    // 削除
    table -= "foo";
    std::cout << "foo: " << table("foo") << std::endl;

    return 0;
}
