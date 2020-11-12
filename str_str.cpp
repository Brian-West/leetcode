#include <iostream>
#include <string>
#include <math.h>
#include <map>
using namespace std;

class Solution {
public:
    int strStr(string haystack, string needle) {
        int n=haystack.size();
        int m=needle.size();
        // 推荐解法一，双指针解法，最优情况（每次首字符比较都失败）时间复杂度为O(n)
        // if(m==0) return 0;     // 由于下面每次都判断首字符是否相等，因此m==0的情况无法包含在里面
        // // int i=0;
        // // while(i<n-m+1) {
        // //     while(i<n-m+1 && haystack[i]!=needle[0]) {  // 每次回溯之后都重新寻找首字符相等的下标
        // //         i++;
        // //     }
        // //     if(i==n-m+1) return -1;     // 不加这一步的话后面的循环有可能会继续进行，直到i>=n回溯之后才能不符合外层循环条件i<n-m+1
        // //     int j=0;
        // //     int curlen=0;
        // //     while(j<m && i<n && haystack[i]==needle[j]) { // 找到首字符相等的坐标之后，逐个字符比较，三个变量都递增。前提是haystack和needle都还没到结尾
        // //         curlen++;
        // //         if(curlen==m) {
        // //             return i-curlen+1;
        // //         }
        // //         i++;
        // //         j++;
        // //     }
        // //     // 无法全部匹配时回溯，i回溯至相等首字符的下一个；needle从头开始，curlen清零（这两个变量都重新定义，可节省代码）
        // //     i = i-curlen+1;
        // // }
        // // 解法一的精简版，同样无法覆盖m=0的情况
        // for(int i=0;i<n-m+1;i++) {
        //     for(int j=0;j<m;j++) {
        //         if(haystack[i+j]!=needle[j]) {
        //             break;
        //         }
        //         if(j==m-1) {
        //             return i;
        //         }
        //     }
        // }
        // return -1;
        // 推荐解法二，滚动哈希。核心思想是用一个哈希值代表一段子串，子串的比较转为哈希值的比较。由于是滚动的，因此从第二个子串开始，哈希值可以在常数时间内计算得到。假定字符串仅由26个小写英文字母构成。
        // m=0时hash_a=hash_b=0因此返回0；m>n时第一个循环会报错，因此需要单独判断。
        // if(m>n) return -1;
        // int base = 26;
        // long long hash_a = 0;
        // long long hash_b = 0;
        // long long module = pow(2, 31);    // 模值的取值是另一门学问
        // for(int i=0;i<m;i++) {
        //     hash_a = (hash_a*base + (haystack[i]-'a'))%module;
        //     hash_b = (hash_b*base + (needle[i] - 'a'))%module;
        // }
        // if(hash_a==hash_b || haystack.substr(0, m)==needle) {   // 由于取模导致哈希值可能冲突，因此哈希值相等时需要判断字符串是否相等
        //     return 0;
        // }
        // long long basem = 0;
        // for(int i=0;i<m;i++) {
        //     basem = (basem*base) % module;
        // }
        // for(int i=1;i<n-m+1;i++) {
        //     hash_a = (hash_a*base - (haystack[i-1]-'a')*basem + haystack[i+m-1]-'a') % module;
        //     if(hash_a==hash_b || haystack.substr(i, m)==needle) {
        //         return i;
        //     }
        // }
        // return -1;
        // 推荐解法三。Sunday算法。每次匹配两个子串，如果不匹配，则查看haystack中下一个字符是否在needle中出现，按照偏移表的值将窗口向后滑动。之所以查看下一个字符，是因为窗口向后移动时必然会包含进该字符，如果该字符根本不在needle中出现的话，就没必要只移动一个窗口位了；如果出现的话，就让needle中该字符位于最后面的那个与haystack下一个字符对齐，这样就不会错过任何一个有可能的匹配
        map<char, int> shifts;  // 偏移表
        for(int i=m-1;i>-1;i--) {
            if(shifts.find(needle[i]) == shifts.end()) {
                shifts.insert(pair<char, int>(needle[i], m-i));
            }
        }
        int i=0;
        while(i<n-m+1) {
            if(haystack.substr(i, m) == needle) {
                return i;
            }
            if(i+m >= n) return -1;     // 防止haystack越界
            map<char, int>::iterator iter = shifts.find(haystack[i+m]);
            if(iter == shifts.end()) {
                i+=m+1;
            } else {
                i+=iter->second;
            }
        }
        return -1;
        // 我自己的解法，时间复杂度固定为O(m*(n-m))
        // m=0和m>n的情况也可以包含在下面的循环中
        // for(int i=0;i<n-m+1;i++) {
        //     if(haystack.substr(i,m)==needle) {
        //         return i;
        //     }
        // }
        // return -1;
    }
};