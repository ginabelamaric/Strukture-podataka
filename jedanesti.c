#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_LINE 1024
#define MAX_SIZE 128
#define HASH_SIZE 11
#pragma warning(disable:4996)

struct _stablo;
typedef struct _stablo* position;
typedef struct _stablo
{
	char name[MAX_SIZE];
	int br_ljudi;
	position livi;
	position desni;
}stablo;

struct _cvor;
typedef struct _cvor* pozicija;
typedef struct _cvor
{
	char ime[MAX_SIZE];
	position root;
	pozicija sljedeci;
}cvor;

int citaj_file(char* ime, pozicija head, cvor HashTablica[]);
int ubaci_sort(pozicija new, pozicija head);
int ubaci_nakon(pozicija prije, pozicija new);
int printaj_listu(pozicija head);
position iz_fila_za_stablo(char* ime, position root);
position ubaci_stablo(position new, position tr);
position novi_el_stablo(char* ime, int br);
int print_stablo(position tr);
position pronadi_grad(position tr, int br);
int  pronadi_drzavu(cvor HashTablica[], char* drzava, int br);
int hash_tablica(cvor HashTablica[], position new);
int hash_br(char* drzava);
int init_liste(pozicija head);
int brisi_stablo(position trenutni);
int brisi_listu(pozicija head);

int main()
{
	cvor head = { .ime = { 0 },.root = NULL,.sljedeci = NULL };
	pozicija p = &head;
	char ime[MAX_SIZE] = { 0 };
	int broj;

	cvor HashTablica[HASH_SIZE];

	for (int i = 0; i < HASH_SIZE; i++)
	{
		init_liste(&HashTablica[i]);
	}

	citaj_file("Drzave.txt", p, &HashTablica);

	printf("Unesite ime drzave koju zelite pretraziti: ");
	scanf("%s", ime);

	printf("Unesite broj stanovnika: ");
	scanf("%d", &broj);

	pronadi_drzavu(HashTablica, ime, broj);

	return 0;
}

int init_liste(pozicija head)
{
	memset(head->ime, 0, MAX_SIZE);
	head->root = NULL;
	head->sljedeci = NULL;

	return 0;
}
int pronadi_drzavu(cvor HashTablica[], char* drzava, int br)
{
	int num = hash_br(drzava);

	pozicija temp = &HashTablica[num];
	while (temp != NULL && strcmp(drzava, temp->ime) != 0)
		temp = temp->sljedeci;

	if (temp)
	{
		printf("gradovi u %s imaju vise ljudi od %d: \n", drzava, br);
		temp->root = pronadi_grad(temp->root, br);
	}

	else
		printf("ne postoji!\n");

	return 0;
}
position pronadi_grad(position tr, int br)
{
	if (tr == NULL)
	{
		return NULL;
	}

	else if (tr->br_ljudi < br)
	{
		tr->desni = pronadi_grad(tr->desni, br);
	}
	else if (tr->br_ljudi >= br)
	{
		tr->livi = pronadi_grad(tr->livi, br);
		printf("grad:%s  br_ljudi: %d\n", tr->name, tr->br_ljudi);
		tr->desni = pronadi_grad(tr->desni, br);
	}

	return tr;
}
int citaj_file(char* ime, pozicija head, cvor HashTablica[])
{
	char buffer[MAX_LINE] = { 0 };
	char ime[MAX_LINE] = { 0 };
	int num;

	FILE* pfile = NULL;
	pfile = fopen(ime, "r");

	if (!pfile)
	{
		printf("falilo otvaranje!\n");
		return -1;
	}

	while (!feof(pfile))
	{
		pozicija new = NULL;
		new = (pozicija)malloc(sizeof(cvor));

		if (!new)
		{
			perror("falila alokacija");
			return -1;
		}

		fgets(buffer, MAX_SIZE, pfile);

		if (sscanf(buffer, " %s %s", new->ime, ime) == 2)
		{
			new->root = NULL;
			new->root = iz_fila_za_stablo(ime, new->root);
			hash_tablica(HashTablica, new);
		}
	}
	fclose(pfile);
	return 0;
}
position iz_fila_za_stablo(char* ime, position root)
{
	char buffer[MAX_LINE] = { 0 };
	FILE* pfile = NULL;
	pfile = fopen(ime, "r");
	if (!pfile)
	{
		printf("%s\n", ime);
		printf("falilo otvaranje!\n");
		return -1;
	}

	while (!feof(pfile))
	{
		int br;
		char drzava[MAX_SIZE] = { 0 };
		position new = NULL;

		fgets(buffer, MAX_SIZE, pfile);
		if (sscanf(buffer, " %s %d", drzava, &br) == 2)
		{
			new = novi_el_stablo(drzava, br);
			root = ubaci_stablo(new, root);
		}
	}
	fclose(pfile);
	return root;
}
int ubaci_sort(pozicija new, pozicija head)
{

	pozicija temp = head;
	while (temp->sljedeci != NULL && strcmp(temp->sljedeci->ime, new->ime) < 0)
		temp = temp->sljedeci;

	ubaci_nakon(temp, new);

	return 0;
}
int ubaci_nakon(pozicija prije, pozicija new)
{
	new->sljedeci = prije->sljedeci;
	prije->sljedeci = new;

	return 0;
}
int printaj_listu(pozicija head)
{
	pozicija prvi = head->sljedeci;
	while (prvi)
	{
		printf("\nIme:%s\nGradovi:\n", prvi->ime);
		if (prvi->root)
			print_stablo(prvi->root);

		else
			printf("nepostoji!\n");

		prvi = prvi->sljedeci;
	}
	return 0;
}
position ubaci_stablo(position new, position tr)
{
	if (tr == NULL)
	{
		return new;
	}
	else if (tr->br_ljudi > new->br_ljudi)
	{
		tr->livi = ubaci_stablo(new, tr->livi);
	}
	else if (tr->br_ljudi < new->br_ljudi)
	{
		tr->desni = ubaci_stablo(new, tr->desni);
	}
	else
	{
		if (strcmp(tr->name, new->name) > 0)
		{
			tr->livi = ubaci_stablo(new, tr->livi);
		}

		else if (strcmp(tr->name, new->name) < 0)
		{
			tr->desni = ubaci_stablo(new, tr->desni);
		}
		else
			free(new);
	}
	return tr;
}
position novi_el_stablo(char* ime, int br)
{
	position new = NULL;

	new = (position)malloc(sizeof(stablo));
	if (!new)
	{
		perror("falila alokacija!\n");
		return NULL;
	}

	strcpy(new->name, ime);
	new->br_ljudi = br;
	new->livi = NULL;
	new->desni = NULL;

	return new;
}
int print_stablo(position tr)
{
	if (NULL == tr)
		return 0;

	print_stablo(tr->livi);
	printf("%s %d\n", tr->name, tr->br_ljudi);
	print_stablo(tr->desni);

	return 0;
}
int hash_br(char* drzava)
{
	int num = 0;

	num = drzava[0] + drzava[1] + drzava[2] + drzava[3] + drzava[4];
	return num % 11;
}
int hash_tablica(cvor HashTablica[], position new)
{
	int num = hash_br(new->name);

	ubaci_sort(new, &HashTablica[num]);

	return 0;
}
int brisi_listu(pozicija head)
{
	pozicija toDelete = NULL;
	while (head->sljedeci != NULL)
{
		toDelete = head->sljedeci;
		brisi_stablo(head->sljedeci->root);
	head->sljedeci = toDelete->sljedeci;
	free(toDelete);
}
return 0;
}
int brisi_stablo(position trenutni)
{
	if (trenutni == NULL)
		return 0;
	brisi_stablo(trenutni->livi);
	brisi_stablo(trenutni->desni);
	free(trenutni);

	return 0;
}
