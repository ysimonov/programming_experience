#include <iostream>
using namespace std;

/*
Given the head of a singly linked list, return true if it is a palindrome.

Example 1:

Input: head = [1,2,2,1]
Output: true


Example 2:

Input: head = [1,2]
Output: false
*/

struct ListNode
{
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr)
    {
    }
    ListNode(int x) : val(x), next(nullptr)
    {
    }
    ListNode(int x, ListNode *next) : val(x), next(next)
    {
    }
};

class Solution
{
  public:
    void push_front(ListNode **head, int val)
    {

        // create new node
        ListNode *node = new ListNode;

        // add value to new node
        node->val = val;

        // link the old list off the new node
        node->next = *head;

        // move the head to point to new node
        *head = node;
    }

    bool isPalindrome(ListNode *head)
    {

        ListNode *single_step = head;
        ListNode *double_step = head;
        ListNode *reverse_head;

        push_front(&reverse_head, single_step->val);

        while (double_step != nullptr && double_step->next != nullptr)
        {
            single_step = single_step->next;
            double_step = double_step->next->next;
            if (double_step != nullptr)
                push_front(&reverse_head, single_step->val);
        }

        if (double_step != nullptr && double_step->next != nullptr)
        {
            push_front(&reverse_head, single_step->val);
        }

        bool is_palindrome = true;
        while (reverse_head != nullptr && single_step != nullptr)
        {
            if (single_step->val != reverse_head->val)
            {
                is_palindrome = false;
                break;
            }
            single_step = single_step->next;
            reverse_head = reverse_head->next;
        }

        return is_palindrome;
    }
};

void deallocateList(ListNode *head)
{
    ListNode *current = head;
    while (current != nullptr)
    {
        ListNode *temp = current;
        current = current->next;
        delete temp;
    }
}

void printList(struct ListNode *head)
{
    struct ListNode *temp = head;

    // iterate the entire linked list and print the data
    while (temp != nullptr)
    {
        printf("%d->", temp->val);
        temp = temp->next;
    }
    printf("nullptr\n");
}

void push_back(struct ListNode **head, int val)
{

    // create a new node
    struct ListNode *node = new ListNode;
    node->val = val;
    node->next = nullptr;

    // if head is null, it is an empty list
    if (*head == nullptr)
    {
        *head = node;
    }
    else
    {
        // find the  last node and add the new node
        struct ListNode *last_node = *head;

        // last node's next address will be a nullptr
        while (last_node->next != nullptr)
        {
            last_node = last_node->next;
        }

        // add the new node at the end of linked list
        last_node->next = node;
    }
}

int main()
{
    auto sol = Solution();

    // allocated node
    struct ListNode *head = nullptr;

    for (int i = 1; i < 4; i++)
    {
        push_back(&head, i);
    }

    for (int i = 3; i > 0; i--)
    {
        push_back(&head, i);
    }

    cout << "Original Linked List: " << endl;
    printList(head);

    auto res = sol.isPalindrome(head);

    if (res)
        cout << "List is palindrome." << endl;
    else
        cout << "List is not palindrome." << endl;

    // deallocate linked list
    deallocateList(head);

    return EXIT_SUCCESS;
}