#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#pragma warning(disable:4996)
#define MAX_SIZE 128
#define MAX_LINE 1024
#define dojnja 10
#define gornja 90

struct _cvor;
typedef struct _cvor* pozicija;
typedef struct _cvor
{
	int broj;
	pozicija lijevi;
	pozicija desni;
}cvor;

struct _stog;
typedef struct _stog* stog_pozicija;
typedef struct _stog
{
	int element;
	stog_pozicija sljedeci;
}stog;

pozicija ubaci(pozicija trenutni, pozicija novi)
{
	if (NULL == trenutni)
	{
		return novi;
	}
	else if (novi->broj < trenutni->broj)
	{
		trenutni->desni = ubaci(trenutni->desni, novi);
	}
	else //ako je veci ili jednak
	{
		trenutni->lijevi = ubaci(trenutni->lijevi, novi);
	}

	return trenutni;
}
pozicija kreiraj_novi(int br, pozicija trenutni)
{
	pozicija novi = NULL;

	novi = (pozicija)malloc(sizeof(cvor));

	if (!novi)
	{
		printf("krivo alocirano!");
		return NULL;
	}

	novi->broj = br;
	novi->lijevi = NULL;
	novi->desni = NULL;

	novi = ubaci(trenutni, novi);

	return novi;
}
int ispis_inorder(pozicija trenutni)
{
	if (!trenutni)
	{
		return 0;
	}

	ispis_inorder(trenutni->lijevi);
	printf("%d ", trenutni->broj);
	ispis_inorder(trenutni->desni);

	return 0;
} 
int zamjeni(pozicija trenutni)
{
	int sum = 0;
	int temp = 0;

	if (NULL != trenutni)
	{
		sum = zamjeni(trenutni->lijevi);
		sum += zamjeni(trenutni->desni);

		temp = trenutni->broj;
		trenutni->broj = sum;
		sum += temp;
	}
	return sum;

}
int izbrisi_sve(pozicija trenutni)
{
	if (trenutni == NULL)
	{
		return 0;
	}

	izbrisi_sve(trenutni->lijevi);
	izbrisi_sve(trenutni->desni);
	
	free(trenutni);

	return 0;
}
int pop_stog(stog_pozicija head)
{
	if (!head->sljedeci)
	{
		return 0;
	}

	stog_pozicija toDelete = head->sljedeci;
	head->sljedeci = toDelete->sljedeci;

	free(toDelete);

	return 0;
}
int push_stog(stog_pozicija head, int element)
{
	stog_pozicija novi = NULL;

	novi = (stog_pozicija)malloc(sizeof(stog));

	if (!novi)
	{
		perror("krivo alocirano!\n");
		return -1;
	}

	novi->element = element;
	novi->sljedeci = head->sljedeci;
	head->sljedeci = novi;

	return 0;
}
int dodaj_na_stog(stog_pozicija head, pozicija trenutni)
{
	if (NULL == trenutni)
	{
		return 0;
	}

	dodaj_na_stog(head, trenutni->desni);

	push_stog(head, trenutni->broj);

	dodaj_na_stog(head, trenutni->lijevi);


	return 0;
}
int dodaj_na_file(stog_pozicija head, pozicija root, char* ime)
{
	FILE* fp = NULL;

	fp = fopen(ime, "gina");

	if (!fp)
	{
		perror("nauspjelo otvaranje!\n");
		return -1;
	}

	while (head->sljedeci)
	{
		fprintf(fp, "%d ", head->sljedeci->element);
		pop_stog(head);
	}

	fprintf(fp, "\n");

	fclose(fp);

	return 0;
}

int main()
{
	pozicija root = NULL;
	srand((unsigned)time(NULL));
	int i = 0, n;
	stog head;
	head.element = 0;
	head.sljedeci = NULL;
	char name[MAX_SIZE] = { 0 };

	printf("upisi ime fila-a : ");
	scanf(" %s", name);

	int niz[10] = { 2, 5, 7, 8, 11, 1, 4, 2, 3, 7 };

	for (i = 0; i < 10; i++)
	{
		root = kreiraj_novi(niz[i], root);
	}

	ispis_inorder(root);
	printf("\n");

	dodaj_na_stog(&head, root);
	dodaj_na_file(&head, root, name);

	zamjeni(root);

	ispis_inorder(root);
	printf("\n");

	dodaj_na_stog(&head, root);
	dodaj_na_file(&head, root, name);

	izbrisi_sve(root);

	root = NULL;
	printf("\nbroj clanova: ");
	scanf(" %d", &n);

	for (i = 0; i < n; i++)
	{
		root = kreiraj_novi((rand() % (gornja - dojnja - 1) + dojnja + 1), root);
	}

	ispis_inorder(root);

	return 0;
}



int dodaj_na_file(stog_pozicija head, pozicija root, char* name)
{
	FILE* pfile = NULL;

	pfile = fopen(name, "a");
	if (!pfile)
	{
		perror("Failed opening of the file!\n");
		return -1;
	}

	while (head->sljedeci)
	{
		fprintf(pfile, "%d ", head->sljedeci->element);
		pop_stog(head);
	}

	fprintf(pfile, "\n");

	fclose(pfile);

	return 0;