#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_LINE 1024
#define MAX_SIZE 128

struct _cvor;
typedef struct _cvor* pozicija;
typedef struct _cvor
{
	char ime[MAX_SIZE];
	int br_ljudi;
	pozicija sljedeci;
}cvor;

struct _stablo;
typedef struct _stablo* position;
typedef struct _stablo
{
	char name[MAX_SIZE];
	cvor head;
	position livi;
	position desni;
}stablo;

int inicijalizacija_liste(pozicija headElement);
int citaj_iz_file(char* ime, pozicija head);
int ubaci_sortirano(pozicija novi, pozicija head);
int ubaci_nakon(pozicija prije, pozicija novi);
int ispis_liste(pozicija head);
position citaj_iz_file_za_stablo(char* ime, position root);
position ubaci_u_stablo(position novi, position trenutni);
position novi_el_stabla(char* ime);
int isprintaj_stablo(position trenutni);
int usoredba_gradova(pozicija novi, pozicija stari);
int nadi_grad(pozicija head, int broj);
position nadi_drzavu(position trenuni, char* ime, int broj);

int main()
{
	position root = NULL;
	char ime[MAX_SIZE] = { 0 };
	int broj;

	root = citaj_iz_file_za_stablo("Drzave.txt", root);

	isprintaj_stablo(root);

	printf("\nUnesite ime drzave koju zelite pretraziti: ");
	scanf("%s", ime);

	printf("Unesite broj stanovnika: ");
	scanf("%d", &broj);

	nadi_drzavu(root, ime, broj);

	return 0;
}
int nadi_grad(pozicija head, int broj)
{
	pozicija temp = head->sljedeci;

	while (temp)
	{
		if (temp->br_ljudi >= broj)
		{
			printf("ime: %s  br_ljudi: %d\n", temp->ime, temp->br_ljudi);
		}
		temp = temp->sljedeci;
	}

	return 0;
}
position nadi_drzavu(position trenutni, char* ime, int broj)
{
	if (trenutni == NULL)
	{
		printf("nepostoji drzava!\n");
		return NULL;
	}

	else if (strcmp(trenutni->name, ime) < 0)
	{
		trenutni->desni = nadi_drzavu(trenutni->desni, ime, broj);
	}
	else if (strcmp(trenutni->name, ime) > 0)
	{
		trenutni->livi = nadi_drzavu(trenutni->livi, ime, broj);
	}
	else
	{
		printf("Cities in % s that have larger population than % d:\n", ime, broj);
		nadi_grad(&trenutni->head, broj);
	}
	return trenutni;
}
int inicijalizacija_liste(pozicija headElement)
{
	memset(headElement->ime, 0, MAX_SIZE);
	headElement->br_ljudi = 0;
	headElement->sljedeci = NULL;

	return 0;
}
position citaj_iz_file_za_stablo(char* ime, position root)
{
	char buffer[MAX_LINE] = { 0 };
	FILE* pf = NULL;
	pf = fopen(ime, "r");
	if (!pf)
	{
		printf("nemoze se otvorit file!\n");
		return NULL;
	}

	while (!feof(pf))
	{
		char drzava[MAX_SIZE] = { 0 };
		char ime[MAX_SIZE] = { 0 };
		position novi = NULL;

		fgets(buffer, MAX_SIZE, pf);
		if (sscanf(buffer, " %s %s", drzava, ime) == 2)
		{
			novi = novi_el_stabla(drzava);
			citaj_iz_file(ime, &novi->head);
			root = ubaci_u_stablo(novi, root);
		}
	}
	fclose(pf);
	return root;
}
int citaj_iz_file(char* ime, pozicija head)
{
	char buffer[MAX_LINE] = { 0 };
	FILE* pf = NULL;

	pf = fopen(ime, "r");
	if (!pf)
	{
		printf("nemoze se otvorit file!\n");
		return -1;
	}

	while (!feof(pf))
	{
		pozicija novi = NULL;
		novi = (pozicija)malloc(sizeof(cvor));
		if (!novi)
		{
			perror("nemoze se alocirat");
			return -1;
		}

		fgets(buffer, MAX_SIZE, pf);
		if (sscanf(buffer, " %s %d", novi->ime, &novi->br_ljudi) == 2)
		{
			ubaci_sortirano(novi, head);
		}
	}
	fclose(pf);
	return 0;
}
int ubaci_sortirano(pozicija novi, pozicija head)
{
	pozicija temp = head;
	while (temp->sljedeci != NULL && usoredba_gradova(novi, temp->sljedeci) < 0)
	{
		temp = temp->sljedeci;
	}
	ubaci_nakon(temp, novi);

	return 0;
}
int usoredba_gradova(pozicija novi, pozicija stari)
{
	int rezultat = novi->br_ljudi - stari->br_ljudi;

	if (novi->br_ljudi == stari->br_ljudi)
	{
		rezultat = (strcmp(stari->ime, newnovi->ime) < 0);
	}
	return  rezultat;
}
int ubaci_nakon(pozicija prije, pozicija novi)
{
	novi->sljedeci = prije->sljedeci;
	prije->sljedeci = novi;

	return 0;
}
int ispis_liste(pozicija prvi)
{
	while (prvi)
	{
		printf("%s %d\n", prvi->ime, prvi->br_ljudi);
		prvi = prvi->sljedeci;
	}
	return 0;
}
position ubaci_u_stablo(position novi, position trenutni)
{
	if (trenutni == NULL)
	{
		return novi;
	}
	else if (strcmp(trenutni->name, novi->name) > 0)
	{
		trenutni->livi = ubaci_u_stablo(novi, trenutni->livi);
	}
	else if (strcmp(trenutni->name, novi->name) < 0)
	{
		trenutni->desni = ubaci_u_stablo(novi, trenutni->desni);
	}
	else
		free(novi);

	return trenutni;
}
position novi_el_stabla(char* ime)
{
	position novi = NULL;

	novi = (position)malloc(sizeof(stablo));
	if (!novi)
	{
		perror("nemoze se alocirat!\n");
		return NULL;
	}

	strcpy(novi->name, ime);
	inicijalizacija_liste(&novi->head);
	novi->livi = NULL;
	novi->desni = NULL;

	return novi;
}
int isprintaj_stablo(position trenutni)
{
	if (NULL == trenutni)
		return 0;

	isprintaj_stablo(trenutni->livi);
	printf("\n%s\n", trenutni->name);
	ispis_liste(trenutni->head.sljedeci);
	isprintaj_stablo(trenutni->desni);

	return 0;