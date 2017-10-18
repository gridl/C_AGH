#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char* pobierzString (void) {
	
	char *tekst = NULL, *tmp = NULL;
    int wielkosc = 0, index = 0, delta;
    char znak;
    delta = sizeof(char);
	
	int flaga = 1;
    do {
        znak = getc(stdin);

        /* sprawdzam czy wczytany znak to znak konca wiersza, jezeli tak to zmieniam flage na 0 -> to otatni obrot petli */
        if (znak == EOF || znak == '\n')
            flaga = 0;

        /* powiekszam pamiec o kolejny znak*/
        wielkosc += delta;
        tmp = realloc(tekst, wielkosc);
        if (!tmp) {
            free(tekst);
            tekst = NULL;
            break;
        }
        tekst = tmp;
        

        /* zapisuje znak w pamieci*/
        tekst[index] = znak;
        index ++;
    } while (flaga);
    
    return tekst;
	
}

typedef struct tagElement {
	/* Struktura dzialajaca jako pojedynczy element listy. Element przechowuje wymagane dane, oraz dwa wskazniki - do elementu wczesniejszego oraz do kolejnego.*/
	
	struct tagElement* next; /*Wskaznik do elementu kolejnego*/
	struct tagElement* prev; /*Wskaznik do elementu wczesniejszego*/
	
	/* Dane przenoszone wewnatrz pojedynczego elementu listy [zgodnie z trescia zadania] */
	char* autor;
	char* tytul;
	int numerKatalogowy;
	double cena;

}Element;

typedef struct tagList {
	/* Struktura pelniaca role listy. Sklada sie ze wskaznikow do struktur o nazwie Element oraz indexu pelniacego role licznika.*/
	
	Element* head; /* Pierwszy element listy */
	Element* tail; /* Ostatni element listy */
	
	int size; //Rozmiar listy [ilosc elementow]
	
}Lista;

void init(Lista* list) {
	/* Funkcja tworzaca liste o podanej nazwie. Parametry listy zostaja ustawione na 0.*/
	
	list->head=0;
	list->tail=0;
	list->size=0;
}

void zbudujElement(Element* ksiazka) {
	
	fseek(stdin,0,SEEK_END);
	
	printf("Podaj imie oraz nazwisko autora ksiazki: ");
	ksiazka->autor = pobierzString();
	
	printf("Podaj tutul kiazki: ");
	ksiazka->tytul = pobierzString();
	
	fseek(stdin,0,SEEK_END);
	
	printf("Podaj numer katalogowy [liczba]: ");
	scanf("%d", &(ksiazka->numerKatalogowy));
	
	printf("Podaj cene: ");
	scanf("%lf", &(ksiazka->cena));
	
}

void dodajFront(Lista *list) {
	
	Element* element = (Element*)malloc(sizeof(Element));
	zbudujElement(element);
	element->prev = 0;
	element->next = list->head;
	
	
	if (list->size == 0){
		list->head = element;
		list->tail = element;
		list->size++;
	}
	else {
		list->head->prev = element;
		list->head = element;
		list->size++;
	}
		
}

void printFront (Lista *list) {
	
	Element* bierzacy= list->head;
	
	int index;
	for (index = 0; index < list->size; index++) {
		printf("Tytul: %s", bierzacy->autor);
		printf("Autor: %s", bierzacy->tytul);
		printf("Numer Katalogowy: %d\n", bierzacy->numerKatalogowy);
		printf("Cena: %.2lf\n", bierzacy->cena);
		printf("---------------------\n");
		bierzacy=bierzacy->next;
	}
}

void dodaj(Lista *list, Element* nowy) {
	
	Element* element = (Element*)malloc(sizeof(Element));
	*element = *nowy;
	
	
	element->prev = 0;
	element->next = list->head;
	
	
	if (list->size == 0){
		list->head = element;
		list->tail = element;
		list->size++;
	}
	else {
		list->head->prev = element;
		list->head = element;
		list->size++;
	}
	printf("%s", list -> head -> autor);
		
}

void dodajBackZPliku(Lista *list, Element* nowy) {
	
	Element* element = (Element*)malloc(sizeof(Element));
	
	*element = *nowy;
	
	element->prev = list->tail;
	element->next = 0;
	
	
	if (list->size == 0){
		list->head = element;
		list->tail = element;
		list->size++;
	}
	else {
		list->tail->next = element;
		list->tail = element;
		list->size++;	
	}
}

void stworzListe(Lista *list) {
	
	init(list);
	
	char ch;
	init(list);
	
	do {	
		ch = 0;
	
		do {
			system("cls");
			printf("Czy chcesz wprowadzic nowy element ksiegozbioru? [t/n]");
			ch = getch();
		} while ((ch != 't') && (ch != 'n'));
		
		if (ch == 't') {
			system("cls");
			dodajFront(list);
		}
		
	} while (ch != 'n');
}

void binarnyZapisListy(Lista *list) {
	
	FILE *ptr_myfile;
	Element* n = list -> head;
	
	int counter;
	
	ptr_myfile=fopen("C:\\Users\\AFGHAN\\Desktop\\C\\LAB10\\test.bin","wb");
	if (!ptr_myfile) {
		printf("Unable to open file!");
		
	}
	else {
	
		for ( counter=1; counter <= list -> size; counter++) {
			
			fwrite(n, sizeof(Element), 1, ptr_myfile);
			
			if (counter != list -> size) {
				n = n->next;
			}
			
		}
	}
fclose(ptr_myfile);
}

void binarnyOdczytListy(Lista *Nowa) {
	
	FILE *ptr_myfile;
	Element* n = (Element*)malloc(sizeof(Element));
	
	int counter;
	
	ptr_myfile=fopen("C:\\Users\\AFGHAN\\Desktop\\C\\LAB10\\test.bin","rb");
	if (!ptr_myfile) {
		printf("Unable to open file!");
	}
	else {
		
		fseek(ptr_myfile, 0L, SEEK_END);
		int iloscStruktur = ftell(ptr_myfile)/sizeof(Element);
		
		
		
		//Lista Nowa;
		//init (&Nowa);
		
		fseek(ptr_myfile, 0L, SEEK_SET);
		
		for ( counter=1; counter <= iloscStruktur; counter++) {
			
		
			
			fread(n, sizeof(Element),1,ptr_myfile);
			
			//printf("%s",n->autor);
			//printf("%lf",n->cena);
			
			dodajBackZPliku(Nowa, n);
			//printf("glowa :%d\n", Nowa->head);
			//printf("nastepny :%d\n", Nowa->head->next);
		
		}
	}
	fclose(ptr_myfile);	
}

int main (void) {
	
	Lista Ksiazki;
	stworzListe(&Ksiazki);
	
	binarnyZapisListy(&Ksiazki);
	Lista Nowa;
	init (&Nowa);
	binarnyOdczytListy(&Nowa);
	
	/*
	FILE *ptr_myfile;
	Element* n;
	
	int counter;
	
	ptr_myfile=fopen("C:\\Users\\AFGHAN\\Desktop\\C\\LAB10\\test.bin","rb");
	if (!ptr_myfile) {
		printf("Unable to open file!");
		return 1;
	}
	
	fseek(ptr_myfile, 0L, SEEK_END);
	int iloscStruktur = ftell(ptr_myfile)/sizeof(Element);
	
	Lista Nowa;
	init (&Nowa);
	
	fseek(ptr_myfile, 0L, SEEK_SET);
	
	for ( counter=1; counter <= iloscStruktur; counter++) {
		
		//fseek(ptr_myfile,sizeof(Element)*counter,SEEK_SET);
		fread(n, sizeof(Element),1,ptr_myfile);
		
		//printf("%s",n->autor);
		//printf("%lf",n->cena);
		
		dodajBackZPliku(&Nowa, n);
		printf("glowa :%d\n", Nowa.head);
		printf("nastepny :%d\n", Nowa.head->next);
	}
	fclose(ptr_myfile);*/	
	
	printFront(&Nowa);
	
	return 0;
}