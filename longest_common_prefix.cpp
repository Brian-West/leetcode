#include <string>
#include <vector>
#include <iostream>
using namespace std;

class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        if(!strs.size()) {
            return "";
        }
        vector<string> prefises;
        string first_str = strs[0];
        int first_len = first_str.length();
        if(first_len==0) {
            return "";
        }
        int max_prefix_len = first_len;
        for(int i=0;i<first_len;i++){
            prefises.push_back(first_str.substr(0, i+1));
        }
        for(int i=1;i<strs.size();i++) {
            int cur_prefix_len = 0;
            for(int j=0;j<prefises.size();j++) {
                if(j+1>strs[i].size() || prefises[j] != strs[i].substr(0, j+1)){
                    break;
                }
                cur_prefix_len = j+1;
            }
            if(cur_prefix_len == 0){
                return "";
            }
            max_prefix_len = max_prefix_len>cur_prefix_len?cur_prefix_len:max_prefix_len;
        }
        return prefises[max_prefix_len-1];
    }
};

int main(){
    Solution solu;
    vector<string> strs;
    strs.push_back("");
    // strs.push_back("f");
    // strs.push_back("flight");
    cout<<solu.longestCommonPrefix(strs)<<endl;
    system("pause");
    return 1;
}