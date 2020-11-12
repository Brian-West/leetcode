#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    int removeElement(vector<int>& nums, int val) {
        // 推荐解法，因为数组内元素顺序可以改变，因此每次遇到要删除的元素时，就用数组末尾元素代替，同时数组长度减一。这样赋值操作次数就等于待删除的元素数目
        int i=0;
        int n=nums.size();
        while(i<n) {
            if(nums[i]==val) {
                nums[i] = nums[n-1];
                n--;
            } else {
                i++;
            }
        }
        return n;
        // 我自己的解法，缺点：赋值操作太多，即使是不需要删除的元素也得重新赋值
        // int i=0;
        // for(int j=0;j<nums.size();j++) {
        //     if(nums[j]!=val) {
        //         nums[i] = nums[j];
        //         i++;
        //     }
        // }
        // return i;
    }
};