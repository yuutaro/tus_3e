#include <iostream>
#include <unordered_map>
#include <string>

#define HASHSIZE 101

using namespace std;

struct MyHash
{
  std::size_t operator()(const std::string &key) const
  {
    unsigned int hash = 0;
    for (char c : key)
    {
      hash = (hash << 5) - hash + c;
    }
    return hash % HASHSIZE;
  }
};

struct MyEqual
{
  bool operator()(const std::string &lhs, const std::string &rhs) const
  {
    return lhs == rhs;
  }
};

template <typename T>
class HashTable
{
private:
  std::unordered_map<std::string, T, MyHash, MyEqual> table;

public:
  // キーによる探索
  T operator()(const std::string &key) const;
  // アイテムの挿入
  void insert(const std::string &key, const T &value);
  // 削除
  void operator-=(const std::string &key);
};

// 各メソッドを実装
template <typename T>
T HashTable<T>::operator()(const std::string &key) const
{
  auto it = table.find(key);
  if (it != table.end())
  {
    return it->second;
  }
  else
  {
    throw std::runtime_error("Key not found");
  }
}

template <typename T>
void HashTable<T>::insert(const std::string &key, const T &value)
{
  table[key] = value;
}

template <typename T>
void HashTable<T>::operator-=(const std::string &key)
{
  table.erase(key);
}

int main()
{
  HashTable<int> table;

  // 挿入
  table.insert("hoge", 42);
  table.insert("foo", 100);

  // 探索
  try
  {
    printf("hoge: %d\n", table("hoge"));
    printf("foo: %d\n", table("foo"));
  }
  catch (const std::runtime_error &e)
  {
    std::cerr << e.what() << std::endl;
  }

  // 削除
  table -= "hoge";

  // 削除後の探索
  try
  {
    printf("hoge: %d\n", table("hoge"));
  }
  catch (const std::runtime_error &e)
  {
    std::cerr << e.what() << std::endl;
  }

  HashTable<std::string> table2;
  table2.insert("hoge", "fuga");
  table2.insert("foo", "bar");

  try
  {
    printf("hoge: %s\n", table2("hoge").c_str());
    printf("foo: %s\n", table2("foo").c_str());
  }
  catch (const std::runtime_error &e)
  {
    std::cerr << e.what() << std::endl;
  }

  table2 -= "hoge";

  try
  {
    printf("hoge: %s\n", table2("hoge").c_str());
  }
  catch (const std::runtime_error &e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
