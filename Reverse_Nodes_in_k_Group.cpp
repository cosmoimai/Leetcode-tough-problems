/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution
{

    ListNode *revList(ListNode *curr)
    {
        ListNode *prev = NULL;
        ListNode *nxt = NULL;

        while (curr)
        {
            nxt = curr->next;
            curr->next = prev;
            prev = curr;
            curr = nxt;
        }

        return prev;
    }

public:
    ListNode *reverseKGroup(ListNode *head, int k)
    {

        ListNode *ret = new ListNode(-1);
        ListNode *ans = ret;

        while (head)
        {
            ListNode *st = head;
            ListNode *prev = head;

            bool flag = false;

            for (int i = 0; i < k; i++)
            {
                if (head == NULL)
                {
                    flag = true;
                    break;
                }

                prev = head;
                head = head->next;
            }

            prev->next = NULL;

            if (flag == true)
            {
                ret->next = st;
                break;
            }

            ret->next = revList(st);
            while (ret->next)
                ret = ret->next;
        }

        return ans->next;
    }
};