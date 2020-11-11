#include <iostream>
using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};

class Solution {
public:
    bool isPalindrome(ListNode* head) {
        if(head==NULL || head->next==NULL)
            return true;
        // 第一步，快慢指针找中点。奇数情况，慢指针指向中点；偶数情况，慢指针指向中间两节点的后者
        ListNode* fast = head;
        ListNode* slow = head;
        while(fast!=NULL && fast->next!=NULL){
            fast = fast->next->next;
            slow = slow->next;
        }
        // 奇数情况，慢指针需要再往前前进一步
        if(fast!=NULL){
            slow = slow->next;
        }
        // 反转慢指针到链表尾的这一段
        ListNode* pre = NULL;   // 反转后的链表的头节点
        ListNode* cur = slow;
        while(cur!=NULL){
            ListNode* next = cur->next;
            cur->next = pre;
            pre = cur;
            cur = next;
        }
        // 比较两段链表，都是从头节点开始依次往下比较
        ListNode* left = head;
        ListNode* right = pre;
        while(right!=NULL){
            if(left->val != right->val){
                return false;
            }
            left = left->next;
            right = right->next;
        }
        return true;
    }
};