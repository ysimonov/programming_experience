#include <iostream>

#include <boost/asio.hpp>

/*
Problem Statement:

Input: head = [1,2,3,4,5]
Output: [3,4,5]
Explanation: The middle node of the list is node 3.

Input: head = [1,2,3,4,5,6]
Output: [4,5,6]
Explanation: Since the list has two middle nodes with values 3 and 4, we return
the second one.
*/

using namespace std;

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
    ListNode *middleNode(ListNode *head)
    {
        // count the number of nodes in the linked list
        int count = 0;
        // point to the first node
        ListNode *current = head;
        // find the number of nodes
        while (current != nullptr)
        {
            // point to the next node
            current = current->next;
            // print value of the linked list at current node
            // cout << current->val << endl;
            count++;
        }
        // find middle position of the list
        int middle_idx = count / 2;
        // traverse linked list again, until middle idx is reached
        current = head;
        count = 0;
        while (count != middle_idx)
        {
            // point to the node
            current = current->next;
            count++;
        }
        return current;
    }
};

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

int main()
{
    auto sol = Solution();

    // allocated node
    struct ListNode *head = nullptr;

    for (int i = 1; i < 6; i++)
    {
        // push_front(&head, i);
        push_back(&head, i);
    }

    cout << "Original Linked List: " << endl;
    printList(head);

    auto res = sol.middleNode(head);

    cout << "Linked List Starting from Middle" << endl;
    printList(res);

    // deallocate linked list
    deallocateList(head);

    return EXIT_SUCCESS;
}