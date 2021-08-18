#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <cmath>
#include <cstdlib>
#include <tuple>
#include <algorithm>

using namespace std;

void Read_word(list<tuple<int, int, string>> & data, pair<int, int> & pos, bool & eof){
  int c = cin.get();
  while (isspace(c) && c != EOF){
    if (c == '\n'){
      pos.first++;
      pos.second = 1;
    }
    c = cin.get();
  }
  if (c == EOF){
    eof = true;
    return;
  }
  cin.unget();
  string str;
  cin >> str;
  transform(str.begin(), str.end(), str.begin(), ::tolower);
  data.push_back(make_tuple(pos.first, pos.second, str));
  pos.second++;
}

int main(){
  vector <tuple <int, int, string>> pat;
  vector <int> z_func;
  list <tuple <int, int, string>> data;
  pair<int, int> pos;
  bool eof = false;
  pos.first = 1;
  pos.second = 1;


  int c = cin.get();
  while (c != EOF && isspace(c)) c = cin.get();
  while (c != '\n' && c != EOF){
    string str;
    while (c != EOF && c != '\n' && isspace(c)) c = cin.get();
    if (c == '\n' || c == EOF) {
      if (c == EOF) eof = true;
      break;
    }
    cin.unget();
    cin >> str;
    transform(str.begin(), str.end(), str.begin(), ::tolower); 
    pat.push_back(make_tuple(0, pos.second, str));         
    ++pos.second;
    c = cin.get();
  }
  pos.second = 1;


  z_func.resize(pat.size());
  int left = 0, right = 0;
  for (int i = 0; i < pat.size(); ++i){
    if (i < right) z_func[i] = min(right - i + 1, z_func[i - left]);
    while (i + z_func[i] < pat.size()
      && get<2>(pat[z_func[i]]) == get<2>(pat[i + z_func[i]]))
        ++z_func[i];
    if (z_func[i] + i > right){
      right = i + z_func[i];
      left = i;
    }
  }

 
  for (int i = right + 1; !eof || data.size() > pat.size(); ++i){
    if (!eof) Read_word(data, pos, eof);
    if (eof && data.size() < pat.size()) break;;
    int k = 0;
    if (i < right) k = min(right - i + 1, z_func[i - left]);
    auto iter = data.begin();
    while (data.size() <= k && !eof) Read_word(data, pos, eof);
    advance(iter, k);
    while (k < pat.size()
      && k < data.size()
      && get<2>(*iter) == get<2>(pat[k])){
        if (k == data.size() - 1)
          if (!eof) Read_word(data, pos, eof);
        ++k;
        ++iter;
    }
    if (k == pat.size())
      cout << get<0>(data.front()) << ",\t" << get<1>(data.front()) << "\n";
    if (k + i > right){
      right = k + i;
      left = i;
    }
    data.pop_front();
  }
  
  return 0;
}