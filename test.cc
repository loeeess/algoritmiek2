#include <vector>
#include <iostream>
using namespace std;

int main() {
  int nrSpelers = 5;
  vector<vector<int> > vrijeSpelers;
  vector<int> v;
  cout << "Voor loop" << endl;
  for (int i = 0; i < nrSpelers; i++) {
    cout << "i: " << i << endl;
    v.push_back(i);
    cout << "pushback done" << endl;
  }
  cout << "na loop" << endl;
    vrijeSpelers.push_back(v);

  return 0;
}