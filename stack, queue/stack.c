#include <stdio.h>

#define N 9

int graph[N][N] = {
        {0, 1, 1, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0} };


struct Stack
{
    int data[N];
    int size;
} stack;

void Pop(struct Stack* stack);
int Top(struct Stack* stack);
void Push(struct Stack* stack, int number);
int isEmpty(struct Stack* stack);


int main()
{
    int dir;

    Push(&stack, 0);

    while(!isEmpty(&stack))
    {
        dir = Top(&stack);

        printf("%d ", dir + 1);

        Pop(&stack);

        for(int i = N; i > 0; i--)
            if(graph[dir][i])
                Push (&stack, i);
    }
    return 0;
}

void Pop(struct Stack* stack)
{
    stack -> data[stack -> size] = 0;
    stack -> size--;
    return;
}

int Top(struct Stack* stack)
{
    return stack -> data[stack -> size];
}


void Push(struct Stack* stack, int number)
{
    stack -> size++;
    stack -> data[stack -> size] = number;
    return;
}

int isEmpty(struct Stack* stack)
{
    if(stack -> size == 0)
        return 1;
    else
        return 0;
}

