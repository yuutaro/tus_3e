  #include <iostream> 
  using namespace std; 

  template<typename T> class last2 { 
    T v1, v2; 
    
  public: 
    last2(T x, T y) { 
	    v1 = x; 
	    v2 = y; 
    } 
    
    void put(T v) { 
      v2 = v1; 
      v1 = v; 
    } 
    
    T get() { 
      T x = v1; 
      v1 = v2; 
      return x; 
    } 
  }; 

  int main() { 
    last2<int> a(0, 0); 
    a.put(1); a.put(2); a.put(3); 
    cout << a.get() << '\n'; 
    cout << a.get() << '\n'; 
    cout << a.get() << '\n'; 
    
    last2<const char*> b("", ""); 
    b.put("ab"); b.put("cd"); b.put("ef"); 
    cout << b.get() << '\n'; 
    cout << b.get() << '\n'; 
    cout << b.get() << '\n'; 
  }
  