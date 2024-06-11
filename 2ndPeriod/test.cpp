#include <iostream>
using namespace std;

//ここにヘッダ部分
class Rational { // a/b
  int a, b;
  int gcd(int x, int y);
  
  public:
    Rational(int x);
    Rational(int x, int y);
    
    Rational operator+(const Rational& r) const;
    Rational operator-(const Rational& r) const;
    Rational operator*(const Rational& r) const;
    Rational operator/(const Rational& r) const;
    
    friend ostream& operator<<(ostream& o, Rational& r);
    friend istream& operator>>(istream& i, Rational& r);
};

//ここに実装部分
int Rational::gcd(int x, int y) {
  while(x != y) if(x > y) x -= y; else y -= x;
  return x;
}

Rational::Rational(int x) { a = x; b = 1; }

Rational::Rational(int x, int y) {
  if(y == 0) {a = b = 0; return; }
  if(x == 0) {a = 0; b = 1; return; }
  if(y < 0) {x =-x; y= -y; }
  if(x == 0) {
    a = x; b = y;
  } else if(x > 0) {
    a = x / gcd(x,y); b = y / gcd(x,y);
  } else {
    a = x / gcd(-x,y); b = y / gcd(-x,y);
  }
}

Rational Rational::operator+(const Rational& r) const {
  return Rational(a*r.b + r.a*b, r.b*b);
}

Rational Rational::operator-(const Rational& r) const {
  return Rational(a*r.b - r.a*b, r.b*b);
}

Rational Rational::operator*(const Rational& r) const {
  return Rational(a*r.a, r.b*b);
}

Rational Rational::operator/(const Rational& r) const {
  return Rational(a*r.b, r.a*b);
}

ostream& operator<<(ostream& o, Rational& r) {
  if(r.b == 0) o << "NaN";
  else o << r.a << '/' << r.b;
  return o;
}

istream& operator>>(istream& i, Rational& r) {
  int a, b; i >> a >> b;
  r = Rational(a, b); return i;
}

int main(void) {
  Rational r(1), x(0);
  while(true) {
    char c; cout << "? "; cin >> c;
    if(c == 'q') return 0;
    switch(c) {
    case 'q': return 0;
    case '=': cin>>x; r = x; break;
    case '+': cin>>x; r = r + x; break;
    case '-': cin>>x; r = r - x; break;
    case '*': cin>>x; r = r * x; break;
    case '/': cin>>x; r = r / x; break;
    default: continue;
    }
    cout << r << '\n';
  }
}
