#include <stdio.h>
#include <stdlib.h>
#include "math.h"

typedef struct queue
{
    void **content;
    int front, rear;
    int max_size;
} Queue;
Queue *create_queue(int initial_size)
{
    Queue *ret = (Queue *)calloc(1, sizeof(Queue));

    if( ! ret ) return 0;
    ret->content = calloc(initial_size, sizeof(void *) );
    if( ! ret->content )
    {
        free(ret);
        return 0;
    }
    ret->max_size = initial_size;
    return ret;
}
void erase_queue(Queue *q)
{
    if( q )
    {
        free(q->content);
        free(q);
    }
}
int insert_into_queue(Queue *q, void *dat)
{
    if( ! q ) return 0;
    if( (q->rear + 1)%q->max_size == q->front )
    {
        void **c = (void **)calloc(q->max_size*2, sizeof(void *));
        int i, j;

        if( ! c )   return 0;
        for(i = 0, j = q->front; j != q->rear; j = (j+1) % q->max_size, i++)
            c[i] = q->content[j];

        q->front = 0; q->rear = i;
        free(q->content);
        q->content = c;
        q->max_size *= 2;
    }
    q->content[q->rear++] = dat;
    q->rear = q->rear % q->max_size;
    return 1;
}
void *remove_from_queue(Queue *q)
{
    void *ret = 0;
    if( ! q ) return 0;
    if( q->front == q->rear ) return 0;
    ret = q->content[q->front++];
    q->front = q->front % q->max_size;
    return ret;
}

typedef struct st
{
    struct st **content;
    int top;
    int max_size;
} Stack;
Stack *create_stack(int initial_size)
{
    if( initial_size < 1 ) return 0;
    else
    {
        Stack *ret = (Stack *)calloc(1, sizeof(Stack));

        if( ! ret ) return 0;
        ret->content = calloc(initial_size, sizeof(void *));
        if( ! ret->content )
        {
            free(ret);
            return 0;
        }
        ret->max_size = initial_size;
        return ret;
    }
}
int push(Stack *s, void *dat)
{
    if( ! s ) return 0;

    if( s->top == s->max_size*9/10 )
    {
        Stack **tmp=(Stack **)realloc(s->content,sizeof(void *)*s->max_size*2);
        if( ! tmp ) return 0;

        s->content = tmp;
        s->max_size *= 2;
    }
    s->content[s->top++] = dat;
    return 1;
}
void *pop(Stack *s)
{
    if( ! s ) return 0;
    if( s->top == 0 ) return 0;
    return s->content[--s->top];
}
int is_empty_stack(Stack *s)
{
    if( ! s )  return 1;
    return s->top == 0;
}

struct Node {
    int key;
    struct Node *left;
    struct Node *right;
    int balance;
};

struct Set {
    struct Node *root;
    int height;
};

struct Visit {
    struct Set *set;
    Stack *stack;
    struct Node *current;
};

struct Node *new_node(int key) {
    struct Node *node = (struct Node *) malloc(sizeof(struct Node));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->balance = 0;
    return node;
}

void menu() {
    printf("****  MENU  ****\n");
    printf("1. Make new set of integers\n");
    printf("2. Insert new number in set\n");
    printf("3. Check number in set\n");
    printf("4. Intersection of two sets\n");
    printf("5. Print set\n");
    printf("6. Delete set\n");
    printf("0. Exit\n");
}

struct Node *right_rotation(struct Node *y) {
    struct Node *x = y->left;
    struct Node *temp = x->right;
    x->right = y;
    y->left = temp;
}

struct Node *left_rotation(struct Node *x) {
    struct Node *y = x->right;
    struct Node *temp = y->left;
    y->left = x;
    x->right = temp;
}


int max(int a, int b) {
    return (a > b) ? a:b;
}

struct Node *insert_key(struct Set *set, int key) {
    struct Node *node = set->root;
    if (node == NULL) {
        return (new_node(key));
    }
    int balance;
    struct Node *p = node;
    struct Node *x = p;
    struct Node *q = NULL, *fx = NULL, *fp = NULL, *y = NULL;
    while (p != NULL) {
        if (key == p->key) {
            return node;
        }
        if (key < p->key) {
            q = p->left;
        } else {
            q = p->right;
        }
        if (q != NULL) {
            if (q->balance != 0) {
                fx = p;
                x = q;
            }
        }
        fp = p;
        p = q;
    }
    q = new_node(key);
    if (key < fp->key) {
        fp->left = q;
    } else {
        fp->right = q;
    }
    if (key < x->key) {
        p = x->left;
    } else {
        p = x->right;
    }
    y = p;
    while (p != q) {
        if (key < p->key) {
            p->balance = 1;
            p = p->left;
        } else {
            p->balance = -1;
            p = p->right;
        }
    }
    if (key < x->key) {
        balance = 1;
    } else {
        balance = -1;
    }
    if (x->balance == 0) {
        x->balance = balance;
        return node;
    }
    if (x->balance != balance) {
        x->balance = 0;
        return node;
    }
    if (y->balance == balance) {
        p = y;
        if (balance == 1) {
            right_rotation(x);
        } else {
            left_rotation(x);
        }
        x->balance = y->balance = 0;
    } else {
        if (balance == 1) {
            p = y->right;
            left_rotation(y);
            x->left = p;
            right_rotation(x);
        } else {
            p = y->left;
            x->right = p;
            right_rotation(y);
            left_rotation(x);
        }
        if (p->balance == 0) {
            x->balance = y->balance = 0;
        } else {
            if (p->balance == balance) {
                x->balance = -balance;
                y->balance = 0;
            } else {
                x->balance = 0;
                y->balance = balance;
            }
            p->balance = 0;
        }
    }
    if (fx == NULL) {
        node = p;
    } else {
        if (x == fx->right) {
            fx->right = p;
        } else {
            fx->left = p;
        }
    }
    return node;
}

int tree_height(struct Node *root) {
    if (root == NULL) {
        return 0;
    }
    int left_height = 0;
    int right_height = 0;

    struct Node *current = root;
    while (current != NULL) {
        left_height++;
        current = current->left;
    }

    current = root;
    while (current != NULL) {
        right_height++;
        current = current->right;
    }

    return (left_height > right_height) ? left_height : right_height;
}


struct Set *create_set() {
    struct Set *set = (struct Set *) calloc(1, sizeof(struct Set));
    if (!set) {
        return 0;
    }
    set->height = 0;
    return set;
}

int check_number(struct Node *node, int key) {
    while (node != NULL) {
        if (key < node->key) {
            node = node->left;
        } else if (key > node->key) {
            node = node->right;
        } else {
            return 1;
        }
    }
    return 0;
}


struct Visit *create_visitor(struct Set *set) {
    if (!set) {
        return 0;
    } else {
        struct Visit *ret = (struct Visit *) calloc(1, sizeof(struct Visit));
        if (!ret) return 0;

        ret->set = set;
        ret->stack = create_stack(set->height * 2);
        if (!ret->stack) {
            free(ret);
            return 0;
        }
        return ret;
    }
}

void visit_inorder(struct Visit *visitor) {
    if (visitor) {
        struct Node *next;

        if (is_empty_stack(visitor->stack) && visitor->current) {
            visitor->current = 0;
            return;
        }

        if (!visitor->current) {
            next = visitor->set->root;
            while (next) {
                push(visitor->stack, next);
                next = next->left;
            }
        }

        next = (struct Node *) pop(visitor->stack);
        visitor->current = next;

        next = next->right;
        while (next) {
            push(visitor->stack, next);
            next = next->left;
        }
    }
}

void empty_set(struct Set *set) {
    if (set) {
        struct Visit *visitor = create_visitor(set);
        if (!visitor) {
            return;
        }
        visit_inorder(visitor);
        while (visitor->current) {
            free(visitor->current);
            visit_inorder(visitor);
        }
        set->root = 0;
        set->height = -1;
    }
}

void delete_set(struct Set *set) {
    empty_set(set);
    free(set);
}

void print_set(struct Set *set) {
    if (!set) {
        return;
    }
    if (!set->root) {
        return;
    } else {
        Queue *q = create_queue((int) pow(2, set->height));
        int i, line_len = 62;
        int first_skip = line_len, in_between_skip;

        if (!q) {
            return;
        }
        insert_into_queue(q, set->root);
        for (i = 0; i <= set->height; i++) {
            int j = 1 << i, k, l;
            in_between_skip = first_skip;
            first_skip = (first_skip - 2) / 2;
            for (k = 0; k < first_skip; k++) putchar(' ');
            for (k = 0; k < j; k++) {
                struct Node *btn = (struct Node *) remove_from_queue(q);
                if (btn) {
                    insert_into_queue(q, btn->left);
                    insert_into_queue(q, btn->right);
                } else {
                    insert_into_queue(q, 0);
                    insert_into_queue(q, 0);
                }
                if (btn) {
                    printf("%2d", btn->key);
                } else printf("  ");
                for (l = 0; l < in_between_skip; l++) putchar(' ');
            }
            putchar('\n');
            putchar('\n');
        }
        erase_queue(q);
    }
}

struct Set *create_intersection_set() {
    return create_set();
}

void intersection_of_sets(struct Set *set1, struct Set *set2, struct Set *result) {
    int height = 0;
    if (!set1 || !set2 || !result)
        return;

    struct Visit *visitor1 = create_visitor(set1);
    struct Visit *visitor2 = create_visitor(set2);
    int value1, value2;

    visit_inorder(visitor1);
    visit_inorder(visitor2);

    while (visitor1->current && visitor2->current) {
        value1 = visitor1->current->key;
        value2 = visitor2->current->key;

        if (value1 < value2) {
            visit_inorder(visitor1);
        } else if (value1 > value2) {
            visit_inorder(visitor2);
        } else {
            result->root = insert_key(result, value1);
            height = tree_height(result->root);
            result->height = height;
            visit_inorder(visitor1);
            visit_inorder(visitor2);
        }
    }
    free(visitor1);
    free(visitor2);
}

int main() {
    int choice, flag_one, flag_two, first_choice = 1, second_choice, height;
    struct Set *set_one;
    struct Set *set_two;
    struct Set *intersection_set;
    while (1) {
        menu();
        scanf("%d", &choice);
        if (first_choice && choice != 1) {
            printf("You need to make a set first!!!\n");
            continue;
        } else {
            first_choice = 0;
        }
        switch (choice) {
            case 1:
                printf("Making new set....\n");
                set_one = create_set();
                set_two = create_set();
                break;
            case 2:
                printf("Enter element:");
                int key;
                scanf("%d", &key);
                printf("Where you want to insert this element? (set_one -> enter 1 or set_two -> enter 2):");
                scanf("%d", &second_choice);
                if (second_choice == 1) {
                    printf("Inserting new element....\n");
                    set_one->root = insert_key(set_one, key);
                    height = tree_height(set_one->root);
                    set_one->height = height;
                } else if (second_choice == 2) {
                    printf("Inserting new element....\n");
                    set_two->root = insert_key(set_two, key);
                    height = tree_height(set_two->root);
                    set_two->height = height;
                } else {
                    printf("You entered wrong value!\n");
                    break;
                }
                break;
            case 3:
                printf("Enter element you want to check:");
                int number_to_check;
                scanf("%d", &number_to_check);
                printf("Checking number....\n");
                flag_one = check_number(set_one->root, number_to_check);
                flag_two = check_number(set_two->root, number_to_check);
                if (flag_one && flag_two) {
                    printf("Number found in both set.\n");
                } else if (flag_one && !flag_two) {
                    printf("Number found in set_one.\n");
                } else if (!flag_one && flag_two) {
                    printf("Number found in set_two.\n");
                } else {
                    printf("Number not found!\n");
                }
                break;
            case 4:
                intersection_set = create_set();
                printf("Intersection of two sets....\n");
                intersection_set = create_intersection_set();
                intersection_of_sets(set_one, set_two, intersection_set);
                printf("Intersection set:\n");
                print_set(intersection_set);
                break;
            case 5:
                printf("Printing set....\n");
                printf("Set one: \n");
                print_set(set_one);
                printf("Set two: \n");
                print_set(set_two);
                break;
            case 6:
                printf("Deleting set....\n");
                delete_set(set_one);
                delete_set(set_two);
                break;
            case 0:
                printf("Exiting....\n");
                exit(0);
            default:
                printf("Wrong choice, try again!\n");
                menu();
                scanf("%d", &choice);
                break;
        }
    }
}
