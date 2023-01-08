#include <stdio.h>
#include <time.h>

unsigned char tab[65536]; //globalna tablica do przechowywania ilo�ci jedynek
						  //w s�owach b�d�cych indeksami tablicy

//	funkcje pomocnicze:
unsigned long long losuj();				// s�owo o losowej liczbie jedynek (rozk�ad normalny)
unsigned long long losujIle(int ile1);	// s�owo o zadanej liczbie jedynek
void wartosci16(unsigned char *tab);	// wype�nianie tablicy globalnej;

/*
	Pod komentarzem funkcja zliczaj�ca jedynki.
	Wywo�ywana zawsze z argumentem b = 32. 

	Poni�sza tabela pokazuje z jak� liczb� por�wnywa�
	argument b (lewa kolumna), �eby algorytm dzieli� s�owo 
	wej�ciowe na s�owa o po��danej ilo�ci bit�w (prawa kolumna):

	b:	  bity:
	32	- 64 (nie uwzgl�dniono w tej implementacji)
	16	- 32 (nie uwzgl�dniono w tej implementacji)
	8	- 16
	4	- 8
	2	- 4
	1	- 2
	0	- 1

	Na ko�cu pliku poda�em przyk�ad implementacji kt�ra na sztywno
	dzieli s�owo wej�ciowe na 4 s�owa 16 - bitowe (funkcja licz16).
	Dzia�a nieco szybciej.
*/
int licz(unsigned long long liczba, int b)
{
	if(!liczba)
		return 0;
	else if(b == 8) //mo�liwa zmiana parametru na wybrany z powy�szej tabeli
		return tab[liczba];
	else
		//podzia� s�owa wej�ciowego na wz�r drzewa binarnego:
		return licz(liczba >> b, b >> 1) + licz(liczba & 0xffffffff >> 32 - b, b >> 1);
		
}

//	Funkcja zliczaj�ca jedynki dla s�owa 256 bitowego
int licz256(unsigned long long *liczba)
{
	return licz(*liczba++, 32) + licz(*liczba++, 32) + licz(*liczba++, 32) + licz(*liczba, 32);
}

main()
{
	unsigned long long num = 0;
	unsigned long long k = 0;
	int i;
	unsigned long long num256[4] = {0};
	clock_t t;
	srand(time(NULL));
	wartosci16(tab);
	
	//	test poprawno�ci:
	for(k = 0; k < 65; k++){
		num = losujIle(k);
		if(licz(num, 32) != k){
			printf("b��d dla liczby %d\n", k);
			return;
		}		
	}
	printf("test poprawnosci: ok\n");
	
	/*	
		w poni�szych testach zmienna k jest iterowana r�n� ilo�� razy,
		bo wyniki s�u�� do por�wnania r�nych algorytm�w a nie wynik�w
		jednego algorytmu dla r�nych test�w
	*/
	
	//	test szybko�ci dla losowego slowa:
	t = clock();
	for(k = 0; k < 0x1ffffff; k++)
		licz(losuj(), 32);
	t = clock() - t;
	printf("test szybkosci 1 -> liczba cykli: %u\n", t);
	
	//	test szybko�ci dla slowa bitowego o losowej (rozklad jednostajny) liczbie jedynek:
	t = clock();
	for(k = 0; k < 0xffffff; k++){
		num = losujIle(rand() % 65);
		licz(num, 32);
	}
	t = clock() - t;
	printf("test szybkosci 2 -> liczba cykli: %u\n", t);
	
	//	test szybkosci dla jedynek tylko w jednej polowie slowa:
	t = clock();
	for(k = 0x100000000; k < 0x3ffffff00000000; k += 0x100000000)
		licz(k, 32);
	t = clock() - t;
	printf("test szybkosci 3 -> liczba cykli: %u\n", t);
	
	//	test poprawno�ci dla s�owa 256 bitowego:
	for(i = 0; i < 4; i++)
		for(k = 0; k < 65; k++){
			num256[i] = losujIle(k);
			if(licz256(num256) != k + i*64){
				printf("i = %d - b��d dla liczby %d\n",i, k);
				return;
			}		
		}
	printf("test poprawnosci 256: ok\n");
	
}

unsigned long long losuj()
{
	unsigned long long liczba;
	int i;
	unsigned char *bajt = (unsigned char*)&liczba;
	
	for(i = 0; i < 8; i++){
		*bajt++ = rand() % 256;
	}
	
	return liczba;
}

unsigned long long losujIle(int ile1)
{
	unsigned long long liczba1, liczba2;
	
	if(ile1 > 32){
		liczba1 = 0xffffffffffffffff;
		while(64 - ile1++){
			liczba2 = ~((unsigned long long)1 << rand() % 64);
			if(liczba1 == (liczba1 & liczba2))
				ile1--;
			liczba1 &= liczba2;
		}
	}else{
		liczba1 = 0;
		while(ile1--){
			liczba2 = (unsigned long long)1 << rand() % 64;
			if(liczba1 == (liczba1 | liczba2))
				ile1++;
			liczba1 |= liczba2;
		}
	}
	return liczba1;
}

void wartosci16(unsigned char *tab)
{
	int i, j;
	*tab = 0;
	tab[1] = 1;
	for(i = 2; i < 65536; i <<= 1)
		for(j = 0; j < i; j++){
			tab[i+j] = tab[j] + 1;
		}
}

int licz16(unsigned short *liczba) //za�o�enie �e short to 2 bajty
{
	return tab[*liczba++] + tab[*liczba++] + tab[*liczba++] + tab[*liczba];
}


