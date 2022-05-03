#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

typedef struct Nod
{
	unsigned int cheie;
	int inaltime;
	struct Nod* stanga;
	struct Nod* dreapta;
}Nod;


Nod* Tata(Nod* radacina, Nod* N)
{
	//daca nodul si radacina sunt nule
	if (N == NULL || radacina == NULL)
		return NULL;

	//radacina
	if (radacina->stanga == N || radacina->dreapta == N)
	{
		return radacina;
	}

	if (N->cheie < radacina->cheie)
	{
		return Tata(radacina->stanga, N);
	}
	else if (N->cheie > radacina->cheie)
	{
		return Tata(radacina->dreapta, N);
	}
	else
	{
		return NULL;
	}
}


Nod* PrimulFiu(Nod* N)
{
	//daca nodul este nul
	if (N == NULL)
		return NULL;

	//verificam daca fiul din stanga este nul
	if (N->stanga == NULL)
		//daca da, returnam fiul din dreapta
		return N->dreapta;
	else
		//daca nu, returnam fiul din stanga
		return N->stanga;
}



//functie ce roteste spre dreapta subarborele
struct Nod* RotatieDreapta(struct Nod* y)
{
	struct Nod* x = y->stanga;
	struct Nod* T2 = x->dreapta;

	// roteste
	x->dreapta = y;
	y->stanga = T2;

	// inaltimile
	y->inaltime = max(inaltime(y->stanga), inaltime(y->dreapta)) + 1;
	x->inaltime = max(inaltime(x->stanga), inaltime(x->dreapta)) + 1;

	// returnam noua radacina
	return x;
}


//functie ce roteste spre stanga subarborele
struct Nod* RotatieStanga(struct Nod* x)
{
	struct Nod* y = x->dreapta;
	struct Nod* T2 = y->stanga;

	// Roteste
	y->stanga = x;
	x->dreapta = T2;

	//Inaltimile
	x->inaltime = max(inaltime(x->stanga), inaltime(x->dreapta)) + 1;
	y->inaltime = max(inaltime(y->stanga), inaltime(y->dreapta)) + 1;

	//returnam noua radacina
	return y;
}


//aflam factorul de balansare
int factorBalansare(struct Nod* N)
{
	if (N == NULL)
		return 0;
	return inaltime(N->stanga) - inaltime(N->dreapta);
}




//functie ce gaseste nodul cheii pentru a descoperi FrateleDreapta
Nod* nodCheie(Nod* radacina, unsigned int chei)
{
	//verificam daca arborele nu este vid
	if (radacina == NULL)
		return NULL;

	//daca cheia cautata este mai mare decat cheia nodului curent
	if (radacina->cheie < chei)
	{
		//parcurgem subarborele din dreapta
		return nodCheie(radacina->dreapta, chei);
	}
	//daca cheia cautata este mai mica decat cheia nodului curent
	else if (radacina->cheie > chei)
	{
		//parcurgem subarborele din stanga
		return nodCheie(radacina->stanga, chei);
	}
	//daca cheia cautata este egala cu cheia nodului curent
	else
	{
		//returnam radacina
		return radacina;
	}
}


//functia pt operatorul FrateDreapta
unsigned int FrateDreapta(Nod* radacina, Nod* N)
{
	//verificam daca nodul exista
	if (N == NULL || radacina == NULL)
		return NULL;


	Nod* T;
	//returnam adresa parintelui pe baza cheii
	T = Tata(radacina, N);

	if (T == NULL)
		return NULL;

	//verificam daca cheia este din dreapta sau stanga
	if (T->stanga == N)
	{
		return T->dreapta;
	}
	else if (T->dreapta == N)
	{
		return NULL;
	}
}

//functie pentru operatorul Cheie
unsigned int* Cheie(Nod* N)
{
	//verificam daca nodul exista
	if (N == NULL)
		return;

	return &N->cheie;
}


//functie pentru operatorul Radacina
Nod* Radacina(Nod* N)
{
	//vericam daca nodul exista
	if (N == NULL)
		return NULL;
	return N;
}


Nod* initializeaza(Nod* radacina)
{
	if (radacina != NULL)
	{
		radacina->stanga = initializeaza(radacina->stanga);
		radacina->dreapta = initializeaza(radacina->dreapta);
		free(radacina);
		radacina = NULL;
		return radacina;
	}
}


//functie pentru a afla inaltimea unui arbore
int inaltime(struct Nod* N)
{
	if (N == NULL)
		return 0;
	return N->inaltime;
}

//functie pentru a ala maximul a 2 nr intregi
int Max(int a, int b)
{
	return (a > b) ? a : b;
}


//functie ce aloca un nou nod cu cheia data si pointerii dreapta si stanga nuli
struct Nod* nouNod(int cheie)
{
	struct Nod* N = (struct Nod*)
		malloc(sizeof(struct Nod));
	N->cheie = cheie;
	N->stanga = NULL;
	N->dreapta = NULL;
	N->inaltime = 1;  //noul nod este adugat initial ca frunza
	return(N);
}



//functie recursiva de inserare a cheii in subarborele cu radacina N si returneaza noua radacina
struct Nod* Insereaza(struct Nod* N, int cheie)
{
	//inserarea unui binary search tree
	if (N == NULL)
		return(nouNod(cheie));

	if (cheie < N->cheie)
		N->stanga = Insereaza(N->stanga, cheie);
	else if (cheie > N->cheie)
		N->dreapta = Insereaza(N->dreapta, cheie);
	else
		return N;

	//actualizam inaltimea nodului predecesor
	N->inaltime = 1 + max(inaltime(N->stanga),
		inaltime(N->dreapta));

	//aflam factorul de balansare pentru a verifca daca N a devenit nebalansat
	int balansare = factorBalansare(N);

	//daca N este nebalansat, avem 4 cazuri

	// cazul stanga stanga 
	if (balansare > 1 && cheie < N->stanga->cheie)
		return RotatieDreapta(N);

	//cazul dreapta dreapta  
	if (balansare < -1 && cheie > N->dreapta->cheie)
		return RotatieStanga(N);

	// cazul stanga dreapta 
	if (balansare > 1 && cheie > N->stanga->cheie)
	{
		N->stanga = RotatieStanga(N->stanga);
		return RotatieDreapta(N);
	}

	// cazul dreapta stanga  
	if (balansare < -1 && cheie < N->dreapta->cheie)
	{
		N->dreapta = RotatieDreapta(N->dreapta);
		return RotatieStanga(N);
	}

	return N;
}

struct Nod* Max2(struct Nod* N)
{
	struct Nod* curent = N;

	while (curent->dreapta != NULL)
		curent = curent->dreapta;
	return curent;
}





FILE* fout;
//tranvesarea in preordine a arborelui
void Preordine(struct Nod* radacina)
{
	if (radacina != NULL)
	{
		printf("%d ", radacina->cheie);
		Preordine(radacina->stanga);
		Preordine(radacina->dreapta);
	}
}

//travesarea inordine a arborelui
void Inordine(struct Nod* radacina)
{
	if (radacina != NULL)
	{
		Inordine(radacina->stanga);
		printf("%d ", radacina->cheie);
		Inordine(radacina->dreapta);
	}
}

//traversarea in postordine a arborelui
void Postordine(struct Nod* radacina)
{
	if (radacina != NULL)
	{
		Postordine(radacina->stanga);
		Postordine(radacina->dreapta);
		printf("%d ", radacina->cheie);
	}
}
/*
void printLevelOrder(struct Nod* radacina)
{
	int h = inaltine(radacina);
	int i;
	for (i = 1; i <= h; i++)
		printCurrentLevel(radacina, i);
}


void printCurrentLevel(struct Nod* radacina, int nivel)
{
	if (radacina == NULL)
		return;
	if (nivel == 1)
		printf("%d ", radacina->cheie);
	else if (nivel > 1) {
		printCurrentLevel(radacina->stanga, nivel - 1);
		printCurrentLevel(radacina->dreapta, nivel - 1);
	}
}
*/
//functie pt suprimarea celor k noduri



int main()
{
	struct Nod* radacina = NULL;
	radacina = initializeaza(radacina);

	int c[6] = { 10,20,30,40,50,25 };
	
	for (int i = 0; i < 6; i++) {
		radacina = Insereaza(radacina, c[i]);
	}


	
	Inordine(radacina);
	printf("\n\n");
	Preordine(radacina);
	printf("\n\n");
	Postordine(radacina);



	_getch();
	return 0;
}