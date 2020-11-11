#include <iostream>
#include <string>
using namespace std;

class Solution {
public:
    int romanToInt(string s) {
        int map['Z'-'A'];
        map['I'-'A'] = 1;
        map['V'-'A'] = 5;
        map['X'-'A'] = 10;
        map['L'-'A'] = 50;
        map['C'-'A'] = 100;
        map['D'-'A'] = 500;
        map['M'-'A'] = 1000;
        int num = 0;
        int index = s.length()-1;
        while(index>=0) {
            if(s[index]=='V'||s[index]=='X'){
                if(index-1>=0 && s[index-1]=='I'){
                    num += map[s[index]-'A'] - 1;
                    index-=2;
                } else {
                    num += map[s[index]-'A'];
                    index--;
                }
            } else if(s[index]=='L'||s[index]=='C'){
                if(index-1>=0 && s[index-1]=='X'){
                    num += map[s[index]-'A'] - 10;
                    index-=2;
                } else {
                    num += map[s[index]-'A'];
                    index--;
                }
            } else if(s[index]=='D'||s[index]=='M'){
                if(index-1>=0 && s[index-1]=='C'){
                    num += map[s[index]-'A'] - 100;
                    index-=2;
                } else {
                    num += map[s[index]-'A'];
                    index--;
                }
            } else {
                num += map[s[index]-'A'];
                index--;
            }
        }
        return num;
    }
};

int main(){
    Solution solu;
    cout<<solu.romanToInt("CMXCIX")<<endl;
    system("pause");
    return 1;
}