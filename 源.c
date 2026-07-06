#include<stdio.h>
#include<stdbool.h>
/*反转字符串*/
typedef struct Snode
{
	int val;
	struct Snode* next
}Snode;

typedef struct Stack
{
	Snode* top;
	int size;
}Stack;
Stack* stack_create()
{
	Stack* st = (Stack*)malloc(sizeof(Stack));
	st->top = NULL;
	st->size = 0;
	return st;
}
bool stack_empty(Stack* st)
{
	return st->top == NULL;
}

int char_list(const char* s)
{
	int len = 0;
	while (s[len])
	{
		len++;
	}
	char* rev = (char*)malloc(len + 1);
	rev[len] = '\0';
	Stack* st = stack_create();
	for (int i = 0;i < len;i++)
	{
		Stack_push(st, (int)s[i]);
	}
	for (int i;i < len;i++)
	{
		rev[i] = stack_pop(st);
	}
	return rev;
}
/*头查法*/
Stack* Stack_push(Stack* st, int val)
{
	Snode* node = (Snode*)malloc(sizeof(Snode));
	node->val = val;
	node->next = st->top;
	st->top = node;
	st->size++;
}
/*头删法*/
Stack* stack_pop(Stack* st)
{
	Snode* node = st->top;
	int val = node->val;
	st->top = st->top->next;
	free(node);
	st->size--;
	return val;
}

typedef struct Queue
{
	Snode* rear;
	Snode* front;
	int size;
}Queue;

Queue* queue_create()
{
	Queue* q = (Queue*)malloc(sizeof(Queue));
	q->rear = NULL;
	q->front = NULL;
	q->size = 0;
	return q;
}
bool empty_queue(Queue* q)
{
	return q->rear == NULL;
}
/*尾插法*/
void insertlast(Queue* q, int num)
{
	Snode* node = (Snode*)malloc(sizeof(Snode));
	node->next = NULL;
	node->val = num;
	q->rear->next = node;
	q->rear = node;
	q->size++;
}
/*头删法*/
Queue* firstdelete(Queue* q)
{
	Snode* node = q->front;
	int val = node->val;
	q->front = q->front->next;
	free(node);
	q->size--;
	return val;
}
bool brackets_match(const char* s)
{
	Stack* st = stack_create();

	int len = 0;
	while (s[len])
	{
		len++;
	}
	int i = 0;

	for (i = 0;i < len;i++)
	{

		if (s[i] == '(' || s[i] == '[' || s[i] == '{')
		{
			stack_push(st, (int)s[i]);
		}
		if (s[i] == ')' && st->top == '(' || s[i] == ']' && st->top == '[' || s[i] == '}' && st->top == '{');
		stack_pop(st);
		bool ok = stack_empty(st);
		free(st);
		return ok;
	}
}
typedef struct
{
	Stack* min;
	Stack* data;
}minStack;

minStack* min_create()
{
	minStack* st = (minStack*)malloc(sizeof(minStack));
	st->data = stack_create();
	st->min = stack_create();
	return st;
}
void minStack_push(minStack* st, int val)
{
	stack_push(st->data, val);
	if (st->min->top->val >= val)
	{
		stack_push(st->min, val);
	}
	else
	{
		stack_push(st->min, st->min->top->val);
	}
}
int minStack_pop(minStack* st)
{
	stack_pop(st->min);
	return stack_pop(st->data);
}

typedef struct myqueue
{
	Stack* in;
	Stack* out;
}myqueue;

myqueue* myqueue_create()
{
	myqueue* q = (myqueue*)malloc(sizeof(myqueue));
	q->in = queue_create();
	q->out = queue_create();
}
void myqueue_enqueue(myqueue* q, int val)
{
	stack_push(q->in, val);
}
int myqueue_dequeue(myqueue* q)
{
	if (stack_empty(q->out))
		while (!stack_empty(q->in))
		{
			stack_push(q->out, stack_pop(q->in));
		}
	return stack_pop(q->out);
}
void myqueue_free(myqueue* q)
{
	free(q->in);
	free(q->out);
	free(q);
}
/*队列改栈*/

typedef struct mystack
{
	Queue* q;
}mystack;
mystack* mystack_create()
{
	mystack* st = (mystack*)malloc(sizeof(mystack));
	st->q = queue_create();
	return st;
}
void mystack_open(mystack* st, int val)
{
	insertlast(st->q, val);
	int len = st->q->size;

	for (int i = 0;i < len;i++)
	{
		insertlast(st->q, firstdelete(st->q));
	}

}

typedef struct
{
	int* data;
	int front;
	int rear;
	int cap;
}circlequeue;

circlequeue* circlequeueu_create(int cap)
{
	circlequeue* cq = (circlequeue*)malloc(sizeof(circlequeue));
	cq->data = (int*)malloc(sizeof(int) * cq->cap + 1);
	cq->front = cq->rear = 0;
	cq->cap = cap + 1;
	return cq;
}
bool empty_circlequeue(circlequeue* cq)
{
	return cq->front == cq->rear;
}
bool full_circlequeue(circlequeue* cq)
{
	return cq->front == (cq->rear + 1) % cq->cap;
}

bool cq_insertlast(circlequeue* cq, int val)
{
	if (empty_circlequeue || full_circlequeue)
		return false;
	else
	{
		cq->data[cq->rear] = val;
		cq->rear++;
		cq->rear = (cq->rear) % cq->cap;
	}
}
bool cq_firstdelete(circlequeue* cq, int* out)
{
	if (empty_circlequeue)
		return false;
	else
	{
		*out = cq->data[cq->front];
		cq->front++;
		cq->front = (cq->front) % cq->cap;
	}
}
int cq_size(circlequeue* cq)
{
	return (cq->front - cq->rear + cq->cap) % cq->cap;
}

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
		else if (s[i] > '0' && s[i] < '9')
		{
			int num = 0;
			while (s[i] > '0' && s[i] < '9')
			{

				num = num * 10 + s[i++] - '0';
			}
			stack_push(st, num);
		}
		else
		{
			int b = stack_pop(st);
			int a = stack_pop(st);
			switch (s[i])
			{
			case '+':	return a + b;
			case'-': return a - b;
			case'*':return a * b;
			case'/':return a / b;
			}
		}
		i++;
	}
	int result = stack_pop(st);
	free(st);
	return result;

}
int factorial(int n)
{
	if (n <= 1)
		return n;
	else
		return n * factorial(n - 1);
}

int fib(int n)
{
	if (n <= 1)
		return n;
	else
		return fib(n - 1) + fib(n - 2);
}

int max_arr(int arr[], int n)
{
	if (n <= 1)
		return arr[0];
	int max_relef = max_arr(arr, n - 1);
	int current = arr[n - 1];
	if (current > max_relef)
		return current;
	else
		return max_relef;
}
void hanoi(int n, char src, char aux, char dst) {
	if (n == 1) {
		printf("1: %c -> %c\n", src, dst);
		return;
	}
	hanoi(n - 1, src, aux, dst);
	printf("%d: %c -> %c\n", n, src, dst);

	// 第三步：n-1 个从 aux 移到 dst（用 src 当临时）
	hanoi(n - 1, aux, src, dst);
}

typedef struct treenode
{
	struct treenode* left;
	struct treenode* right;
	int data;
}treenode;

treenode* treenode_create(int data)
{
	treenode* q = (treenode*)malloc(sizeof(treenode));
	q->left = NULL;
	q->right = NULL;
	q->data = data;

}
void firsttreenode(treenode* q)
{
	printf("%d", q->data);
	firsttreenode(q->left);
	firsttreenode(q->right);
}

void midtreenode(treenode* q)
{
	midtreenode(q->left);
	printf("%d", q->data);
	midtreenode(q->right);
}

void lasttreenode(treenode* q)
{
	lasttreenode(q->left);
	lasttreenode(q->right);
	printf("%d", q->data);
}


treenode* rev_treenode(treenode* q)
{
	treenode* temp = q->left;
	q->left = q->right;
	q->right = temp;
	rev_treenode(q->left);
	rev_treenode(q->right);
}

int deep_treenode(treenode* q)
{
	if (q->data == NULL)
		return 0;
	int right = deep_treenode(q->right);
	int left = deep_treenode(q->left);
	if (right > left)
	{
		return right + 1;
	}
	else
		return left + 1;
}
int mid_search(int arr[], int left, int right, int target)
{
	if (left > right)
		return 0;
	int mid = left + (right - left) / 2;
	if (arr[mid] == target)
		return mid;
	else if (arr[mid] > target)
		return mid_search(arr, left, mid - 1, target);
	else
		return mid_search(arr, mid + 1, right, target);
}

int newmid_search(int arr[], int n, int target)
{
	int left = 0;int right = n - 1;
	while (left <= right)
	{
		int mid = left + (left + right) / 2;
		if (arr[mid] > target)
			right = mid - 1;
		if (arr[mid] < target)
			left = mid + 1;
		if (arr[mid] = target)
			return mid;
	}
	return -1;
}
int main() {


}