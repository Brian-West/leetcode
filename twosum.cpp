#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;

int cmp_int(const void* a, const void* b){
    return *(int*)a - *(int*)b;
}

class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        vector<int> result;
        // 此法修改了输入数组，无法获取原下标，只能输出解的值本身
        // qsort(&nums[0], nums.size(), sizeof(nums[0]), cmp_int);
        // sort(nums.begin(), nums.end());
        // for(int i=0,j=nums.size()-1;i<j;){
        //     if(nums[i]+nums[j]==target){
        //         result.push_back(i);
        //         result.push_back(j);
        //         break;
        //     }
        //     else if(nums[i]+nums[j]<target){
        //         i++;
        //     }
        //     else{
        //         j--;
        //     }
        // }
        // 遍历数组的同时建立起值与下标的映射indexmap，在indexmap中寻找与当前值“互补”的值
        map<int, int> indexmap;
        // vector使用迭代器好像不是很方便，需要使用distance函数才能获取下标
        for(vector<int>::iterator iter=nums.begin();iter!=nums.end();iter++){
            int complement = target - *iter;
            if(indexmap.find(complement)!=indexmap.end()){
                result.push_back(indexmap[complement]);
                result.push_back(distance(nums.begin(), iter));  // distance函数包含在algorithm头文件中
                break;
            }
            indexmap[*iter] = distance(nums.begin(), iter);
        }
        return result;
    }
};