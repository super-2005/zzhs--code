
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>   // 提供 malloc、free 函数

/* ================================================================
 * 链表节点（栈和队列共用）
 * ================================================================ */
typedef struct Snode
{
    int val;
    struct Snode* next;   // 修复：缺少分号
} Snode;

/* ================================================================
 * 栈 —— 基于链表实现（头插法/头删法）
 * ================================================================ */
typedef struct Stack
{
    Snode* top;           // 栈顶指针
    int size;             // 栈中元素个数
} Stack;

// ---------- 栈的函数声明 ----------
Stack* stack_create(void);
bool   stack_empty(Stack* st);
void   stack_push(Stack* st, int val);    // 统一使用小写 stack_push
int    stack_pop(Stack* st);
void   stack_free(Stack* st);

/* ---------- 创建空栈 ---------- */
Stack* stack_create(void)
{
    Stack* st = (Stack*)malloc(sizeof(Stack));
    st->top = NULL;
    st->size = 0;
    return st;
}

/* ---------- 判断栈是否为空 ---------- */
bool stack_empty(Stack* st)
{
    return st->top == NULL;
}

/* ---------- 入栈（头插法）---------- */
void stack_push(Stack* st, int val)
{
    Snode* node = (Snode*)malloc(sizeof(Snode));
    node->val = val;
    node->next = st->top;    // 新节点指向旧的栈顶
    st->top = node;          // 更新栈顶
    st->size++;
}

/* ---------- 出栈（头删法），返回栈顶值 ---------- */
int stack_pop(Stack* st)
{
    if (stack_empty(st))     // 防御：空栈时返回 -1
        return -1;
    Snode* node = st->top;
    int val = node->val;
    st->top = st->top->next; // 栈顶下移
    free(node);
    st->size--;
    return val;
}

/* ---------- 释放整个栈 ---------- */
void stack_free(Stack* st)
{
    while (!stack_empty(st))
        stack_pop(st);
    free(st);
}

/* ================================================================
 * 用栈反转字符串
 * ================================================================ */

 /**
  * reverse_string - 使用栈将输入字符串反转
  * @s: 源字符串（以 '\0' 结尾）
  * 返回值: 反转后的新字符串（调用者负责 free）
  */
char* reverse_string(const char* s)      // 修复：返回 char*，函数名更具可读性
{
    int len = 0;
    while (s[len])
        len++;

    char* rev = (char*)malloc(len + 1);
    rev[len] = '\0';                     // 预留结尾符

    Stack* st = stack_create();

    // 所有字符依次入栈
    for (int i = 0; i < len; i++)
        stack_push(st, (int)s[i]);

    // 出栈顺序即为反转顺序
    for (int i = 0; i < len; i++)        // 修复：i 需初始化为 0
        rev[i] = (char)stack_pop(st);

    stack_free(st);
    return rev;
}

/* ================================================================
 * 括号匹配
 * ================================================================ */

 /**
  * brackets_match - 判断括号序列是否合法
  * @s: 只含 () [] {} 的字符串
  * 返回值: true = 匹配成功, false = 失败
  *
  * 算法：遇到左括号入栈，遇到右括号时检查栈顶是否是对应左括号
  */
bool brackets_match(const char* s)
{
    Stack* st = stack_create();

    int len = 0;
    while (s[len])
        len++;

    for (int i = 0; i < len; i++)
    {
        char ch = s[i];
        // 左括号 → 入栈
        if (ch == '(' || ch == '[' || ch == '{')
        {
            stack_push(st, (int)ch);
        }
        // 右括号 → 检查栈顶是否匹配
        else if (ch == ')' || ch == ']' || ch == '}')
        {
            if (stack_empty(st))          // 栈为空，没有可匹配的左括号
            {
                free(st);
                return false;
            }
            char top = (char)stack_pop(st);
            // 检查是否配对
            if ((ch == ')' && top != '(') ||
                (ch == ']' && top != '[') ||
                (ch == '}' && top != '{'))
            {
                free(st);
                return false;
            }
        }
    }

    bool ok = stack_empty(st);            // 最后栈应为空
    free(st);
    return ok;
}

/* ================================================================
 * 最小栈 —— 用两个栈实现 getMin() O(1)
 *   data 栈存实际数据，min 栈同步存"当前栈中的最小值"
 * ================================================================ */
typedef struct
{
    Stack* min;
    Stack* data;
} MinStack;

MinStack* minStack_create(void)
{
    MinStack* st = (MinStack*)malloc(sizeof(MinStack));
    st->data = stack_create();
    st->min = stack_create();
    return st;
}

void minStack_push(MinStack* st, int val)
{
    stack_push(st->data, val);
    // 若 min 栈为空 或 val <= 当前最小，入 min 栈
    if (stack_empty(st->min) || val <= st->min->top->val)
        stack_push(st->min, val);
    else
        stack_push(st->min, st->min->top->val); // 重复当前最小值，保持同步
}

int minStack_pop(MinStack* st)
{
    stack_pop(st->min);                // min 栈同步弹出
    return stack_pop(st->data);
}

int minStack_getMin(MinStack* st)
{
    return st->min->top->val;
}

void minStack_free(MinStack* st)
{
    stack_free(st->data);
    stack_free(st->min);
    free(st);
}

/* ================================================================
 * 队列 —— 基于链表实现
 *   front 指向队头，rear 指向队尾
 *   入队 = 尾插法，出队 = 头删法
 * ================================================================ */
typedef struct Queue
{
    Snode* rear;
    Snode* front;
    int size;
} Queue;

// ---------- 队列的函数声明 ----------
Queue* queue_create(void);
bool   queue_empty(Queue* q);
void   queue_enqueue(Queue* q, int val);   // 尾插
int    queue_dequeue(Queue* q);            // 头删
void   queue_free(Queue* q);

/* ---------- 创建空队列 ---------- */
Queue* queue_create(void)
{
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->rear = NULL;
    q->front = NULL;
    q->size = 0;
    return q;
}

/* ---------- 判断队列是否为空 ---------- */
bool queue_empty(Queue* q)
{
    return q->front == NULL;          // 修复：用 front 判断（新队列 rear 也是 NULL）
}

/* ---------- 入队（尾插法）---------- */
void queue_enqueue(Queue* q, int val)
{
    Snode* node = (Snode*)malloc(sizeof(Snode));
    node->val = val;
    node->next = NULL;

    if (queue_empty(q))               // 修复：处理空队列情况
    {
        q->front = q->rear = node;    // 第一个元素，front 和 rear 都指向它
    }
    else
    {
        q->rear->next = node;         // 旧的队尾指向新节点
        q->rear = node;               // 更新队尾
    }
    q->size++;
}

/* ---------- 出队（头删法），返回队头值 ---------- */
int queue_dequeue(Queue* q)
{
    if (queue_empty(q))
        return -1;                    // 防御：空队返回 -1
    Snode* node = q->front;
    int val = node->val;
    q->front = q->front->next;        // 队头后移
    if (q->front == NULL)             // 最后一个元素出队后，rear 也要置 NULL
        q->rear = NULL;
    free(node);
    q->size--;
    return val;
}

/* ---------- 释放整个队列 ---------- */
void queue_free(Queue* q)
{
    while (!queue_empty(q))
        queue_dequeue(q);
    free(q);
}

/* ================================================================
 * 用两个栈实现队列（LeetCode 232）
 *   in 栈负责入队，out 栈负责出队
 *   出队时若 out 为空，先把 in 中所有元素倒入 out
 * ================================================================ */
typedef struct MyQueue
{
    Stack* in;
    Stack* out;
} MyQueue;

MyQueue* myQueue_create(void)
{
    MyQueue* q = (MyQueue*)malloc(sizeof(MyQueue));
    q->in = stack_create();          // 修复：应创建栈而不是队列
    q->out = stack_create();
    return q;
}

void myQueue_enqueue(MyQueue* q, int val)
{
    stack_push(q->in, val);
}

int myQueue_dequeue(MyQueue* q)
{
    // 若 out 栈为空，将 in 栈所有元素倒入 out
    if (stack_empty(q->out))
    {
        while (!stack_empty(q->in))
            stack_push(q->out, stack_pop(q->in));
    }
    return stack_pop(q->out);
}

bool myQueue_empty(MyQueue* q)
{
    return stack_empty(q->in) && stack_empty(q->out);
}

void myQueue_free(MyQueue* q)
{
    stack_free(q->in);
    stack_free(q->out);
    free(q);
}

/* ================================================================
 * 用一个队列实现栈（LeetCode 225）
 *   入栈时先正常入队，然后把队列中前面 len-1 个元素依次出队再入队
 *   这样新入栈的元素就到了队头（即栈顶）
 * ================================================================ */
typedef struct MyStack
{
    Queue* q;
} MyStack;

MyStack* myStack_create(void)
{
    MyStack* st = (MyStack*)malloc(sizeof(MyStack));
    st->q = queue_create();
    return st;
}

/**
 * myStack_push - 入栈
 * 核心：入队后把前面的元素全部移到后面，模拟"后进先出"
 */
void myStack_push(MyStack* st, int val)
{
    int len = st->q->size;
    queue_enqueue(st->q, val);               // 先正常入队

    // 把前 len 个元素依次移到队尾
    for (int i = 0; i < len; i++)
        queue_enqueue(st->q, queue_dequeue(st->q));
}

int myStack_pop(MyStack* st)
{
    return queue_dequeue(st->q);
}

int myStack_top(MyStack* st)
{
    if (queue_empty(st->q)) return -1;
    return st->q->front->val;
}

bool myStack_empty(MyStack* st)
{
    return queue_empty(st->q);
}

void myStack_free(MyStack* st)
{
    queue_free(st->q);
    free(st);
}

/* ================================================================
 * 循环队列 —— 基于数组实现
 *   留一个空位区分"队空"和"队满"
 *   队空: front == rear
 *   队满: front == (rear + 1) % cap
 * ================================================================ */
typedef struct
{
    int* data;
    int front;
    int rear;
    int cap;             // 实际容量 = cap - 1（多开一个空间）
} CircleQueue;

CircleQueue* circleQueue_create(int cap)
{
    CircleQueue* cq = (CircleQueue*)malloc(sizeof(CircleQueue));
    cq->data = (int*)malloc(sizeof(int) * (cap + 1));  // 修复：容量计算错误，多分配一个位置
    cq->front = cq->rear = 0;
    cq->cap = cap + 1;
    return cq;
}

bool circleQueue_empty(CircleQueue* cq)
{
    return cq->front == cq->rear;
}

bool circleQueue_full(CircleQueue* cq)
{
    return cq->front == (cq->rear + 1) % cq->cap;
}

/**
 * circleQueue_enqueue - 入队（尾插）
 * 返回值: true = 成功, false = 队列已满
 */
bool circleQueue_enqueue(CircleQueue* cq, int val)
{
    if (circleQueue_full(cq))          // 修复：需调用函数，不是函数指针
        return false;
    cq->data[cq->rear] = val;
    cq->rear = (cq->rear + 1) % cq->cap;
    return true;
}

/**
 * circleQueue_dequeue - 出队（头删）
 * @out: 传出参数，保存出队的值
 * 返回值: true = 成功, false = 队列为空
 */
bool circleQueue_dequeue(CircleQueue* cq, int* out)
{
    if (circleQueue_empty(cq))         // 修复：需调用函数
        return false;
    *out = cq->data[cq->front];
    cq->front = (cq->front + 1) % cq->cap;
    return true;
}

int circleQueue_size(CircleQueue* cq)
{
    return (cq->rear - cq->front + cq->cap) % cq->cap; // 修复：rear - front，不是 front - rear
}

void circleQueue_free(CircleQueue* cq)
{
    free(cq->data);
    free(cq);
}

/* ================================================================
 * 逆波兰表达式求值（LeetCode 150）
 *   遇到数字入栈，遇到运算符弹出两个数计算后入栈
 * ================================================================ */

 /**
  * eval_rpn - 计算逆波兰表达式的值
  * @s: 以空格分隔的逆波兰表达式字符串，如 "3 4 + 5 *"
  */
int eval_rpn(const char* s)
{
    Stack* st = stack_create();
    int i = 0;

    while (s[i])
    {
        if (s[i] == ' ')
        {
            i++;
            continue;
        }
        // 数字（支持多位数）
        else if (s[i] >= '0' && s[i] <= '9')         // 修复：应包括 '0' 和 '9'
        {
            int num = 0;
            while (s[i] >= '0' && s[i] <= '9')
            {
                num = num * 10 + (s[i] - '0');
                i++;
            }
            stack_push(st, num);
        }
        // 运算符
        else
        {
            int b = stack_pop(st);
            int a = stack_pop(st);
            int result = 0;
            switch (s[i])
            {
            case '+': result = a + b; break;    // 修复：应为 break 而非 return
            case '-': result = a - b; break;
            case '*': result = a * b; break;
            case '/': result = a / b; break;
            }
            stack_push(st, result);
            i++;
        }
    }

    int result = stack_pop(st);
    stack_free(st);
    return result;
}

/* ================================================================
 * 递归经典例题
 * ================================================================ */

 /** 阶乘 n! */
int factorial(int n)
{
    if (n <= 1)
        return 1;               // 修复：0! = 1! = 1，不是 return n
    else
        return n * factorial(n - 1);
}

/** 斐波那契数列第 n 项 */
int fib(int n)
{
    if (n <= 1)
        return n;
    else
        return fib(n - 1) + fib(n - 2);
}

/** 递归求数组最大值 */
int max_arr(int arr[], int n)
{
    if (n <= 1)
        return arr[0];
    int max_rest = max_arr(arr, n - 1);    // 前 n-1 个元素的最大值
    int current = arr[n - 1];            // 最后一个元素
    return (current > max_rest) ? current : max_rest;
}

/**
 * 汉诺塔
 * @n:   盘子数
 * @src: 源柱子
 * @aux: 辅助柱子
 * @dst: 目标柱子
 */
void hanoi(int n, char src, char aux, char dst)
{
    if (n == 1)
    {
        printf("1: %c -> %c\n", src, dst);
        return;
    }
    // 第一步：n-1 个从 src → aux（用 dst 做临时）
    hanoi(n - 1, src, dst, aux);           // 修复：参数顺序错误
    // 第二步：第 n 个从 src → dst
    printf("%d: %c -> %c\n", n, src, dst);
    // 第三步：n-1 个从 aux → dst（用 src 做临时）
    hanoi(n - 1, aux, src, dst);
}

/* ================================================================
 * 二叉树
 * ================================================================ */
typedef struct TreeNode
{
    struct TreeNode* left;
    struct TreeNode* right;
    int data;
} TreeNode;

/* ---------- 创建新节点 ---------- */
TreeNode* treeNode_create(int data)
{
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->left = NULL;
    node->right = NULL;
    node->data = data;
    return node;                       // 修复：缺少 return
}

/* ---------- 前序遍历：根 → 左 → 右 ---------- */
void preorder(TreeNode* root)
{
    if (root == NULL) return;          // 修复：缺少递归基（NULL 时返回）
    printf("%d ", root->data);
    preorder(root->left);
    preorder(root->right);
}

/* ---------- 中序遍历：左 → 根 → 右 ---------- */
void inorder(TreeNode* root)
{
    if (root == NULL) return;
    inorder(root->left);
    printf("%d ", root->data);
    inorder(root->right);
}

/* ---------- 后序遍历：左 → 右 → 根 ---------- */
void postorder(TreeNode* root)
{
    if (root == NULL) return;
    postorder(root->left);
    postorder(root->right);
    printf("%d ", root->data);
}

/* ---------- 翻转二叉树 ---------- */
TreeNode* invertTree(TreeNode* root)
{
    if (root == NULL) return NULL;         // 修复：缺少 NULL 判断
    // 交换左右子树
    TreeNode* temp = root->left;
    root->left = root->right;
    root->right = temp;
    // 递归翻转子树
    invertTree(root->left);
    invertTree(root->right);
    return root;                           // 修复：返回根节点
}

/* ---------- 二叉树的最大深度 ---------- */
int maxDepth(TreeNode* root)
{
    if (root == NULL)                      // 修复：应判断 root == NULL，而非 root->data == NULL
        return 0;
    int leftDepth = maxDepth(root->left);
    int rightDepth = maxDepth(root->right);
    return (leftDepth > rightDepth ? leftDepth : rightDepth) + 1;
}

/* ================================================================
 * 二叉搜索树（BST）
 *   性质：左子树所有值 < 根 < 右子树所有值
 * ================================================================ */

 /** 插入节点 */
TreeNode* bst_insert(TreeNode* root, int val)
{
    if (root == NULL)
        return treeNode_create(val);

    if (val < root->data)
        root->left = bst_insert(root->left, val);
    else if (val > root->data)              // 用 else if 避免重复判断
        root->right = bst_insert(root->right, val);
    // 相等不插入（或根据需求处理）

    return root;
}

/** 查找节点 */
TreeNode* bst_search(TreeNode* root, int val)
{
    if (root == NULL)
        return NULL;                       // 修复：返回 NULL 而非 -1
    if (root->data == val)
        return root;
    if (val < root->data)
        return bst_search(root->left, val); // 修复：函数名拼写错误 bitserach → bst_search
    else
        return bst_search(root->right, val);
}

/** 查找最小节点（一直往左走） */
TreeNode* bst_min(TreeNode* root)
{
    if (root == NULL)
        return NULL;                       // 修复：返回 NULL 而非 -1
    while (root->left != NULL)
        root = root->left;
    return root;
}

/** 删除节点（三种情况） */
TreeNode* bst_delete(TreeNode* root, int val)
{
    if (root == NULL)
        return NULL;                       // 修复：返回 NULL 而非 -1

    if (val < root->data)
        root->left = bst_delete(root->left, val);
    else if (val > root->data)
        root->right = bst_delete(root->right, val);
    else
    {
        // 情况 1 & 2：只有一个子节点或没有子节点
        if (root->left == NULL)            // 修复：去掉了多余的分号
        {
            TreeNode* rightChild = root->right;
            free(root);
            return rightChild;
        }
        if (root->right == NULL)
        {
            TreeNode* leftChild = root->left;
            free(root);
            return leftChild;
        }
        // 情况 3：有两个子节点 → 用右子树的最小节点替代
        TreeNode* minNode = bst_min(root->right);
        root->data = minNode->data;
        root->right = bst_delete(root->right, minNode->data);
    }
    return root;
}

/** 验证是否为合法的 BST */
bool bst_isValid(TreeNode* root, long min, long max)   // 用 long 避免 INT_MIN/INT_MAX 边界问题
{
    if (root == NULL)
        return true;
    if (root->data <= min || root->data >= max)
        return false;
    // 左子树的值必须在 (min, root->data) 之间
    // 右子树的值必须在 (root->data, max) 之间
    return bst_isValid(root->left, min, root->data)
        && bst_isValid(root->right, root->data, max);
}

/* ================================================================
 * 二分查找
 * ================================================================ */

 /** 递归版二分查找 */
int binarySearch_recursive(int arr[], int left, int right, int target)
{
    if (left > right)
        return -1;                         // 修复：未找到返回 -1 而非 0
    int mid = left + (right - left) / 2;
    if (arr[mid] == target)
        return mid;
    else if (arr[mid] > target)
        return binarySearch_recursive(arr, left, mid - 1, target);
    else
        return binarySearch_recursive(arr, mid + 1, right, target);
}

/** 迭代版二分查找 */
int binarySearch_iterative(int arr[], int n, int target)
{
    int left = 0, right = n - 1;
    while (left <= right)
    {
        int mid = left + (right - left) / 2;          // 修复：原为 left+(left+right)/2 是错的
        if (arr[mid] == target)                        // 修复：= 是赋值，应为 ==
            return mid;
        else if (arr[mid] > target)
            right = mid - 1;
        else
            left = mid + 1;
    }
    return -1;
}

/* ================================================================
 * 基础排序算法
 * ================================================================ */

 /** 交换两个 int */
void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * 冒泡排序
 * 每一轮把最大的元素"冒泡"到数组末尾
 * 时间复杂度 O(n²)，稳定排序
 */
void bubbleSort(int arr[], int n)
{
    for (int i = n - 1; i > 0; i--)            // i 表示每轮的终点
    {
        int swapped = 0;                        // 优化：若一轮无交换，已有序
        for (int j = 0; j < i; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                swap(&arr[j], &arr[j + 1]);
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
}

/**
 * 简单选择排序
 * 每轮选出最小元素放到已排序区末尾
 * 时间复杂度 O(n²)，不稳定排序
 */
void selectionSort(int arr[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        int minIdx = i;                        // 假设当前位置最小
        for (int j = i + 1; j < n; j++)
        {
            if (arr[j] < arr[minIdx])
                minIdx = j;                    // 更新最小值下标
        }
        if (minIdx != i)                       // 优化：不需要交换自己
            swap(&arr[i], &arr[minIdx]);
    }
}