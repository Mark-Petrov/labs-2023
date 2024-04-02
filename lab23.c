#include <stdio.h>  
#include <stdlib.h>  
#include <stdbool.h>  

typedef struct Node Node;  
typedef struct Tree Tree;  

struct Tree {  
    int id_counter;  
    Node *root;  // корень
};  

struct Node {  
    int id;  
    int val;  
    Node *child;  // потомок
    Node *side;   // след вершина на этом же уровне
};  

void FindNode(Node *t, int id, Node **res, Node **prev) {  
    if (t == NULL || *res != NULL) {  
        return;  
    }  
    if (t->id == id) {  
        *res = t;  
        return;  
    }  
    *prev = t;  
    FindNode(t->side, id, res, prev);  
    *prev = t;  
    FindNode(t->child, id, res, prev);  
}  

// блок с нахождением ширины
void GetWidthOfTree(Node* root, int level, int* count) {
    if(root == NULL) {
        return;
    }

    if(level == 1) {
        (*count)++;
    } else if(level > 1) {
        GetWidthOfTree(root->child, level-1, count);
        GetWidthOfTree(root->side, level-1, count);
    }
}

int GetTreeWidth(Tree* t) {
    if(t->root == NULL) {
        return 0;
    }

    int maxWidth = 0;

    int height = 1;
    while(true) {
        int count = 0;
        GetWidthOfTree(t->root, height, &count);

        if(count > maxWidth) {
            maxWidth = count;
        }

        // проверка на есть ли записи на след уровне
        if(count == 0) {
            break;
        }

        height++;
    }

    return maxWidth;
}

///

int AddNode(Tree *t, int parent_id, int val) {  // добавить точку исправить 
    Node *new = (Node *)malloc(sizeof(Node));  
    if (new == NULL) {  
        printf("Ошибка: недостаточно памяти\n");  
        exit(1);  
    }  
    new->side = NULL;  
    new->child = NULL;  
    new->id = (++t->id_counter);  
    new->val = val;  

    if (t->root == NULL) {  
        t->root = new;  
        return new->id;  //тут косяк 
    }  

    Node *parent = 0, *prev = 0;  
    FindNode(t->root, parent_id, &parent, &prev);  
    if (parent == NULL) {  
        printf("Ошибка: нет узла с id %d\n", parent_id);  
        exit(1);
        return 0;  
    }  
    if (parent->child == NULL) {  
        parent->child = new;  
    } else {  
        parent = parent->child;  
        while (parent->side != NULL) {  
            parent = parent->side;  
        }  
        parent->side = new;  
    }  
    return new->id;  
}  

void RemoveNodeRev(Node *n) {  
    if (n->child != NULL)  
        RemoveNodeRev(n->child);  
    if (n->side != NULL)  
        RemoveNodeRev(n->side);  
    free(n);  
}  

void RemoveNode(Tree *t, int id) {  
    Node *n = 0, *p = 0;  
    FindNode(t->root, id, &n, &p);  
    if (n == NULL) {  
        return; // такой узел не найден 
    }  
    if (p == NULL) {  
        t->root = NULL;  
        t->id_counter = 0;  
    } else if (p->child == n)  
        p->child = n->side; // вертикально  
    else if (p->side == n) {  
        p->side = n->side; // горизонтально 
    }  
    if (n->child != NULL)  
        RemoveNodeRev(n->child);  // перейти в R
    free(n);  
}  

void PrintTreeRev(Node *n, int offset) {  
    while (n != NULL) {  
        for (int i = 0; i < offset * 4; i++)  
            printf(" ");  
        printf("%d - %d\n", n->id, n->val);  
        PrintTreeRev(n->child, offset + 1);  
        n = n->side;  
    }  
}  

void PrintTree(Tree *t) {  
    if (t->root != NULL)  
        PrintTreeRev(t->root, 0);  
}

int main(void) {  
    printf("\n");
    printf("1. Добавить новый узел\n");
    printf("2. Нарисовать дерево\n");
    printf("3. Удалить узел\n");
    printf("4. Вычислить ширину дерева\n");
    printf("5. Выход\n");
    printf("Введите выбранную опцию: ");
    Tree t = {0};  
    char c;  
    int a, b;  
    while (true) { // на switch передалеть
        scanf("%c", &c);  
        if (c == '\n') {  
            continue;  
        }  
        if (c == '1') {  
            scanf("%d %d", &a, &b);  
            AddNode(&t, a, b);  
        } else if (c == '2') {  
            PrintTree(&t);  
        } else if (c == '3') {  
            scanf("%d", &a);  
            RemoveNode(&t, a);  
        } else if (c == '4') {  
    printf("Ширина дерева: %d\n", GetTreeWidth(&t));  
        } else if (c == '5') {
            return 0;  
        } else {  
            printf("Ошибка %c\n", c);  
        }  
        printf("> ");  
    }  
    printf("\n");  
    return 0;  
}