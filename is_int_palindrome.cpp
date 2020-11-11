#include <iostream>
using namespace std;
class Solution {
public:
    bool isPalindrome(int x) {
        // 负数不可能是回文
        if(x<0){
            return false;
        }
        // 非0但个位数是0的也不可能是回文
        if(x!=0&&x%10==0){
            return false;
        }
        // 可能出现整数溢出
        // int a=x;
        // int y=0;
        // while(a!=0){
        //     y = y*10 + a%10;
        //     a/=10;
        // }
        // if(x==y){
        //     return true;
        // } else {
        //     return false;
        // }
        // 只反转后半部分的位数，当原数字小于（对应奇数或者非回文偶位数）或者等于（回文偶位数）新数字时，结束反转。
        int y = 0;
        while(x>y){
            y = y*10 + x%10;
            x /= 10;
        }
        return x==y || x==y/10;
    }
};