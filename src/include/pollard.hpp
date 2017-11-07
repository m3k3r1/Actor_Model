#ifndef _POLLARD_H_
#define _POLLARD_H_

#include <iostream>

using namespace std;

int gcd(int a, int b) {
	int remainder;
	while (b != 0) {
		remainder = a % b;
		a = b;
		b = remainder;
	}
	return a;
}

void p_factorize(vector<int> *fct, int n){
  srand((unsigned)time(0));
  int a = rand() % n  + 1;
  int x = rand() % n  + 1;
  int y = x, p, d;
  int s = 10;

  do {
    x = ((x^2) + 1) % n;
    y = ((y^2) + 1) % n;
    d = (y-x) % n;
    p = gcd(d, n);

    if(p != n)
      fct->push_back(p);

  } while(--s);

}

#endif
