#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#define MAX_LINE 1024
#define MAX_SIZE 60

struct _polinom;
typedef struct _polinom* pointer;
typedef struct _polinom
{
	int koeficijent;
	int potencija;
	pointer next;
} polinom;

int unos_datoteke(char* fileName)
{

	printf("unesite ime datoteke koju zelite otvoriti:\n");
	scanf(" %s", fileName);

	return 0;
}
int citaj_iz_datoteke(pointer p1, pointer p2, char* fileName)
{
	FILE* file = NULL;
	char buffer[MAX_LINE] = { 0 };
	int status = EXIT_SUCCESS;

	file = fopen(fileName, "r");
	if (!file)
	{
		perror("datoteka se nemoze otvoriti\n");
		return -1;
	}

	fgets(buffer, MAX_LINE, file);
	status = prepisi_iz_stringa_u_datoteku(p1, buffer);
	if (status != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}

	fgets(buffer, MAX_LINE, file);
	status = prepisi_iz_stringa_u_datoteku(p2, buffer);
	if (status != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}

	fclose(file);

	return EXIT_SUCCESS;
}

int prepisi_iz_stringa_u_datoteku(pointer p, char* buffer)
{
	char* trenutni_buffer = buffer;
	int koeficijent = 0;
	int eksponent = 0;
	int numBytes = 0;
	int status = 0;
	pointer novi_polinom = NULL;

	while (strlen(trenutni_buffer) > 0)
	{
		status = sscanf(trenutni_buffer, " %d %d %n", &koeficijent, &eksponent, &numBytes);
		if (status != 2)
		{
			printf("datoteka nije uredu\n");
			return EXIT_FAILURE;
		}
		 
		novi_polinom = unos(p,koeficijent, eksponent);
		if (!novi_polinom)
		{
			return EXIT_FAILURE;
		}

		trenutni_buffer += numBytes;
	}

	return EXIT_SUCCESS;
}
pointer unos(pointer p, int koef, int pot)
{
	pointer prev = NULL;
	pointer q;
	prev = p;
	p = p->next;

	while (p->next != NULL && p->potencija > pot)
	{
		prev = p;
		p = p->next;
	}
	if (p != NULL && p->potencija == pot)
		p->koeficijent += koef;
	else
	{
		q = (pointer)malloc(sizeof(polinom));
		q->potencija = pot;
		q->koeficijent = koef;
		q->next = p;
		prev->next = q;
	}
	return q;
}
int produkt(pointer p1, pointer p2, pointer produkt)
{
	pointer pocetak = p2;

	while (p1 != NULL)
	{
		p2 = pocetak;
		while(p2!=NULL)
		{
			unos(produkt, p1->koeficijent * p2->koeficijent, p1->potencija + p2->potencija);
			p2 = p2->next;
		}
		p1 = p1->next;
	}
	return 0;
}
int suma(pointer p1, pointer p2, pointer zbroj)
{
	polinom temp;

	while (p1 != NULL && p2 != NULL)
	{
		if (p1->potencija == p2->potencija)
		{
			unos(zbroj, p1->koeficijent + p2->koeficijent, p1->potencija);
			p1 = p1->next;
			p2 = p2->next;
		}
		else if (p1->potencija > p2->potencija)
		{
			unos(zbroj, p1->koeficijent, p1->potencija);
			p1 = p1->next;
		}
		else(p1->potencija < p2->potencija)
		{
			unos(zbroj, p2->koeficijent, p2->potencija);
			p2 = p2->next;
		}

	}


	return 0;
}
int main()
{

	polinom p1 = { .koeficijent = 0,.potencija = 0,.next = NULL };
	polinom p2 = { .koeficijent = 0,.potencija = 0,.next = NULL };
	polinom umnozak = { .koeficijent = 0,.potencija = 0,.next = NULL };
	polinom zbroj = { .koeficijent = 0,.potencija = 0,.next = NULL };

	char filename[MAX_LINE] = { 0 };
	int izbor = 0;

	unos_datoteke(filename);

	printf("odlucite sto zelite napraviti sa polinomom:\n");
	printf("1-zbrojiti dva polinoma\n");
	printf("2-pomnoziti dva polinoma\n");
	scanf("%d", &izbor);

	switch (izbor)
	{
	case 1:
		suma(&p1, &p2, &zbroj);
	case 2: 
		produkt(&p1, &p2, &umnozak);
	}



	return 0;

}