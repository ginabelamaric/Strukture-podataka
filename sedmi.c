#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#pragma warning(disable:4996)
#define MAX_SIZE 128
#define MAX_LINE 1024

struct _cvor;
typedef struct _cvor* pozicija;
typedef struct _cvor
{
	char name[MAX_SIZE];
	pozicija brat;
	pozicija dite;
}cvor;

struct _stog;
typedef struct _stog* stog_pozicija;
typedef struct _stog
{
	pozicija direktorij;
	stog_pozicija next;
}stog;

pozicija stvori_dir(char* ime, pozicija trenutni)
{
	pozicija novi = NULL;

	novi = (pozicija)malloc(sizeof(cvor));

	if (!novi)
	{
		printf("nuspjela alokacija!\n");
		return NULL;
	}

	strcpy(novi->name, ime);
	novi->dite = NULL;
	novi->brat = NULL;

	trenutni->dite = ubaci_dir(trenutni->dite, novi);

	return trenutni;
}

pozicija ubaci_dir(pozicija trenutni, pozicija novi)
{
	if (trenutni == NULL)
		return novi;

	else if (strcmp(trenutni->name, novi->name) > 0)
	{
		novi->brat = trenutni;
		return novi;
	}

	else if (strcmp(trenutni->name, novi->name) < 0)
	{
		trenutni->brat = ubaci_dir(trenutni->brat, novi);
	}

	else
	{
		printf("postoji vec s istim imenom!\n");
		free(novi);
	}

	return trenutni;
}
int push_stog(stog_pozicija head, pozicija direktorij)
{
	stog_pozicija novi = NULL;

	novi = (stog_pozicija)malloc(sizeof(stog));

	if (!novi)
	{
		printf("neuspjela alokacija!\n");
		return -1;
	}

	stog_pozicija zadnji = nadi_zadnji(head);

	novi->next = zadnji->next;
	zadnji->next = novi;
	novi->direktorij = direktorij;

	return 0;
}

int pop_stog(stog_pozicija head)
{
	if (!head->next)
		return 0;

	stog_pozicija prije = nadi_prije(head);
	stog_pozicija toDelete = NULL;

	toDelete = prije->next;
	prije->next = toDelete->next;
	free(toDelete);

	return 0;
}

stog_pozicija nadi_zadnji(stog_pozicija head)
{
	stog_pozicija temp = head;
	while (temp->next != NULL)
	{
		temp = temp->next;
	}

	return temp;
}

stog_pozicija nadi_prije(stog_pozicija head)
{
	stog_pozicija temp = head;
	while (temp->next->next != NULL)
	{
		temp = temp->next;
	}

	return temp;
}

pozicija promjena_dir(stog_pozicija head, pozicija trenutni, char* ime)
{
	if (NULL == trenutni->dite)
	{
		printf("nemoguce pronac\n");
		return trenutni;
	}
	else
	{
		pozicija temp = trenutni->dite;
		while (temp != NULL && strcmp(temp->name, ime) != 0)
			temp = temp->brat;

		if (temp == NULL)
		{
			printf("nemoguce pronac\n");
			return trenutni;
		}
		else
		{
			push_stog(head, trenutni->dite);
			return trenutni->dite;
		}
	}
}
int prikaz(pozicija trenutni)
{
	if (NULL == trenutni->dite)
	{
		printf("Directorij je pratan.\n");
	}

	else
	{
		pozicija temp = trenutni->dite;
		while (temp != NULL)
		{
			printf("<DIR>\t\t %s\n", temp->name);
			temp = temp->brat;
		}
	}

	return 0;
}
int izlaz(pozicija trenuni)
{

	if (trenuni == NULL)
	{
		return 0;
	}

	izlaz(trenuni->brat);
	izlaz(trenuni->dite);
	izlaz(trenuni->brat);

	free(trenuni);

	return 0;
}
int ispis_dir(stog_pozicija head)
{
	stog_pozicija temp = head->next;
	char line[MAX_LINE] = { 0 };

	while (temp != NULL)
	{
		strcat(line, temp->direktorij->name);
		strcat(line, "/");
		temp = temp->next;
	}

	strcat(line, ">");

	printf("%s ", line);

	return 0;
}
int Menu(pozicija trenutni, stog_pozicija head)
{
	pozicija first = trenutni;
	char name[MAX_LINE] = { 0 };
	char command[MAX_LINE] = { 0 };
	char line[MAX_LINE] = { 0 };

	ispis_dir(head);

	while (1)
	{
		fgets(line, MAX_LINE, stdin);
		sscanf(line, "%s %s", command, name);

		if (!strcmp(command, "md"))
		{
			trenutni = stvori_dir(name, trenutni);
			ispis_dir(head);
		}

		else if (!strcmp(command, "cd"))
		{
			trenutni = promjena_dir(head, trenutni, name);
			ispis_dir(head);
		}

		else if (!strcmp(command, "cd.."))
		{

			pop_stog(head);
			ispis_dir(head);
		}

		else if (!strcmp(command, "dir"))
		{
			prikaz(trenutni);
			ispis_dir(head);
		}

		else if (!strcmp(command, "exit"))
		{
			Exit(trenutni);
			break;
		}

		else
		{
			printf("'%s' nije prepoznato.\n", command);
			ispis_dir(head);
		}
	}
	return 0;
}

int main()
{
	cvor root;
	stog head;

	strcpy(root.name, "C:");
	root.dite = NULL;
	root.brat = NULL;

	head.direktorij = NULL;
	head.next = NULL;

	push_stog(&head, &root);
	Menu(&root, &head);

	return 0;

}
