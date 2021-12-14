#include <stdio.h>
#include <stdlib.h>

struct _stablo;
typedef struct _stablo* pozicija;
typedef struct _stablo {
    int number;
    pozicija left;
    pozicija right;

}stablo;

pozicija  kreiraj_novi(int broj)
{
    pozicija novi = (pozicija)malloc(sizeof(stablo));
    if (!novi)
    {
        printf("nije se alociralo mjesto za novi clan stabla");
        return -1;
    }
    novi->number = broj;
    novi->left = novi->right = NULL;
    return novi;
}

int print_postorder(pozicija root)
{
    if (root != NULL) {
        print_postorder(root->left);
        print_postorder(root->right);
        printf("%d ", root->number);
    }
}

int print_inorder(pozicija root)
{
    if (root != NULL) {
        print_inorder(root->left);
        printf("%d ", root->number);
        print_inorder(root->right);
    }
}

int print_preorder(pozicija root)
{
    if (root != NULL) {
        printf("%d ", root->number);
        print_preorder(root->left);
        print_preorder(root->right);
    }
}

pozicija insert(pozicija root, int broj)
{
    if (root == NULL)
        return kreiraj_novi(broj);
    if (broj < root->number)
        root->left = insert(root->left, broj);
    else
        root->right = insert(root->right, broj);

    return root;
}

pozicija min_el(pozicija root)
{
    pozicija trenutni = root;

    while (trenutni && trenutni->left != NULL)
        trenutni = trenutni->left;

    return trenutni;
}

pozicija brisi(pozicija root, int broj)
{
    if (root == NULL)
        return root;

    if (broj < root->number)
        root->left = brisi(root->left, broj);
    else if (broj> root->number)
        root->right = brisi(root->right, broj);
    else {
        if (root->left == NULL) {
            pozicija temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL) {
            struct node* temp = root->left;
            free(root);
            return temp;
        }
        pozicija temp = min_el(root->right);
        root->number = temp->number;
        root->right = brisi(root->right, temp->number);
    }
    return root;
}
pozicija trazi(pozicija root, int broj)
{
    if (root == NULL || root->number == broj)
        return root;
   
    if (root->number < broj)
        return trazi(root->right,broj);

    return trazi(root->left, broj);
}
int main()
{
    pozicija root = NULL;
    root = insert(root, 50);
    root = insert(root, 30);
    root = insert(root, 20);
    root = insert(root, 40);
    root = insert(root, 70);
    root = insert(root, 60);
    root = insert(root, 80);

    printf("inorder \n");
    print_inorder(root);

    printf("preorder\n");
    print_preorder(root);

    printf("postorder\n");
    print_postorder(root);


    printf("\nbrisi 70\n");
    root = brisi(root, 70);
    printf("inorder\n");
    print_inorder(root);

    printf("\nbrisi 20\n");
    root = brisi(root, 20);
    printf("preorder\n");
    print_preorder(root);

    printf("\nbrisi 40\n");
    root = brisi(root, 40);
    printf("postorder\n");
    print_postorder(root);

    return 0;
}