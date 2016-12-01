#include <stdio.h>

#define MAX_SIZE_QUEUE 100
#define N 9

int graph[N][N] = {
        {0, 1, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 1, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0} };

struct Queue
{
    int data[MAX_SIZE_QUEUE];
    int top;
    int down;
} queue;

void Push(struct Queue* queue, int number);
void Pop(struct Queue* queue);
int Head(struct Queue* queue);
int Back(struct Queue* queue);
int isEmpty(struct Queue* queue);

int main()
{
    int dir;

    Push(&queue, 0);

    queue.down = 1;

    while(!isEmpty(&queue))
    {
        dir = Back(&queue);

        printf("%d ", dir + 1);

        Pop(&queue);

        for(int i = 0; i < N; i++)
            if(graph[dir][i])
                Push(&queue, i);
    }
    return 0;
}

void Push(struct Queue* queue, int number)
{
    queue -> top++;
    queue -> data[queue -> top] = number;
    return;
}

void Pop(struct Queue* queue)
{
    queue -> data[queue -> down] = 0;
    queue -> down++;
    return;
}

int Head(struct Queue* queue)
{
    return queue -> data[queue -> top];
}

int Back(struct Queue* queue)
{
    return queue -> data[queue -> down];
}

int isEmpty(struct Queue* queue)
{
    if (queue -> top - queue-> down == -1)
        return 1;
    else
        return 0;
}