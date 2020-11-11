#include <iostream>
#include <cmath>
using namespace std;

// 32位有符号整数的取值范围为-2,147,483,648到2,147,483,647
class Solution {
public:
    int reverse(int x) {
        int result = 0; // result是正数
        while(x!=0){
            int mod = x%10;
            // 在累计之前先判断累计操作是否会导致溢出
            if(result>INT_MAX/10 || (result==INT_MAX/10&&mod>7)){
                return 0;
            }
            if(result<INT_MIN/10 || (result==INT_MIN/10&&mod<-8)){
                return 0;
            }
            result = result*10+mod;
            x/=10;
        }
        return result;
    }
};

int main(){
    Solution solu;
    cout<<solu.reverse(-123)<<endl;
    system("pause");
    return 1;
}