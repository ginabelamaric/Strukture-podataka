#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_LINE 1024
#define MAX_SIZE 128

struct _stablo;
typedef struct _stablo* pozicija;
typedef struct _stablo
{
	char ime[MAX_SIZE];
	int br_ljudi;
	pozicija livi;
	pozicija desni;
}stablo;

struct _cvor;
typedef struct _cvor* pozicija;
typedef struct _cvor
{
	char ime[MAX_SIZE];
	pozicija root;
	pozicija sljedeci;
}cvor;

int citaj_iz_file(char* ime, pozicija head);
int sortirano(pozicija novi, pozicija head);
int umetni_iza(pozicija prije, pozicija novi);
int isprintaj_listu(pozicija prvi);
pozicija iz_file_u_stablo(char* ime, pozicija root);
pozicija umetni_u_stablo(pozicija novi, pozicija trenutni);
pozicija novi_el_stablo(char* ime, int broj);
int isprintaj_stablo(pozicija trenutni);
pozicija pronadi_grad(pozicija trenutni, int broj);
int pronadi_drzavu(pozicija head, char* drzava, int broj);

int main()
{
	cvor head = { .ime = { 0 },.root = NULL,.next = NULL };

	pozicija p = &head;
	char ime[MAX_SIZE] = { 0 };
	int broj;

	citaj_iz_file("Drzave.txt", p);

	printf("Unesite neku drzavu i broj stanovnika: \nDrzava: ");
	scanf(" %s", ime);

	printf("Broj stanovnika: ");
	scanf("%d", &broj);

	pronadi_drzavu(p, ime, broj);

	return 0;
}
int pronadi_drzavu(pozicija head, char* drzava, int broj)
{
	pozicija temp = head->sljedeci;

	while (temp != NULL && strcmp(drzava, temp->ime) != 0)
		temp = temp->sljedeci;

	if (temp)
	{
		printf("gradovi u %s imaju vise ljudi od %d: \n", drzava, broj);
		temp->root = pronadi_grad(temp->root, broj);
	}

	else
		printf("nema takve drzave u file-u!\n");

	return 0;
}
pozicija pronadi_grad(pozicija trenutni, int broj)
{
	if (trenutni == NULL)
	{
		return NULL;
	}

	else if (trenutni->br_ljudi < broj)
	{
		trenutni->desni = pronadi_grad(trenutni->desni, broj);
	}
	else if (trenutni->br_ljudi >= broj)
	{
		trenutni->livi = pronadi_grad(trenutni->livi, broj);
		printf("ime:%s br_ljudi: %d\n", trenutni->ime, trenutni->br_ljudi);
		trenutni->desni = pronadi_grad(trenutni->desni, broj);
	}

	return trenutni;
}
int citaj_iz_file(char* ime, pozicija head)
{
	char buffer[MAX_LINE] = { 0 };
	char ime[MAX_LINE] = { 0 };
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

		if (sscanf(buffer, " %s %s", novi->ime, ime) == 2)
		{
			novi->root = NULL;
			novi->root = iz_file_u_stablo(ime, novi->root);
			sortirano(novi, head);
		}
	}
	fclose(pf);
	return 0;
}

pozicija iz_file_u_stablo(char* ime, pozicija root)
{
	char buffer[MAX_LINE] = { 0 };
	FILE* pf = NULL;
	pf = fopen(ime, "r");
	if (!pf)
	{
		printf("Failed opening of the file!\n");
		return -1;
	}

	while (!feof(pf))
	{
		int broj;
		char drzava[MAX_SIZE] = { 0 };
		pozicija novi = NULL;

		fgets(buffer, MAX_SIZE, pf);
		if (sscanf(buffer, " %s %d", drzava, &broj) == 2)
		{
			novi = novi_el_stablo(drzava, broj);
			root = umetni_u_stablo(novi, root);
		}
	}
	fclose(pf);
	return root;
}
int sortirano(pozicija novi, pozicija head)
{
	pozicija temp = head;
	while (temp->sljedeci != NULL && strcmp(temp->sljedeci->ime, novi->ime) < 0)
		temp = temp->sljedeci;

	umetni_iza(temp, novi);

	return 0;
}
int umetni_iza(pozicija prije, pozicija novi)
{
	novi->sljedeci = prije->sljedeci;
	prije->sljedeci = novi;

	return 0;
}
int isprintaj_listu(pozicija prvi)
{
	while (prvi)
	{
		printf("\nIme:%s\nGradovi:\n", prvi->ime);
		if (prvi->root)
			isprintaj_stablo(prvi->root);

		else
			printf("nema gradova!\n");

		prvi = prvi->sljedeci;
	}
	return 0;
}
pozicija umetni_u_stablo(pozicija novi, pozicija trenutni)
{
	if (trenutni == NULL)
	{
		return novi;
	}
	else if (trenutni->br_ljudi > novi->br_ljudi)
	{
		trenutni->livi = umetni_u_stablo(novi, trenutni->livi);
	}
	else if (trenutni->br_ljudi < novi->br_ljudi)
	{
		trenutni->desni = umetni_u_stablo(novi, trenutni->desni);
	}
	else
	{
		if (strcmp(trenutni->ime, novi->ime) > 0)
		{
			trenutni->livi = umetni_u_stablo(novi, trenutni->livi);
		}

		else if (strcmp(trenutni->ime, novi->ime) < 0)
		{
			trenutni->desni = umetni_u_stablo(novi, trenutni->desni);
		}
		else 
			free(novi);
	}
	return trenutni;
}
pozicija novi_el_stablo(char* ime, int broj)
{
	pozicija novi = NULL;

	novi = (pozicija)malloc(sizeof(stablo));
	if (!novi)
	{
		perror("nemoze se alocirat!\n");
		return NULL;
	}

	strcpy(novi->ime, ime);
	novi->br_ljudi = broj;
	novi->livi = NULL;
	novi->desni = NULL;

	return new;
}
int isprintaj_stablo(pozicija trenutni)
{
	if (NULL == trenutni)
		return 0;

	isprintaj_stablo(trenutni->livi);
	printf("%s %d\n", trenutni->ime, trenutni->br_ljudi);
	isprintaj_stablo(trenutni->desni);

	return 0;
}