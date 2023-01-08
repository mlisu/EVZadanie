#include <stdio.h>
#include <time.h>

//	funkcje pomocnicze:
unsigned long long losuj();				// s³owo o losowej liczbie jedynek (rozk³ad normalny)
unsigned long long losujIle(int ile1);	// s³owo o zadanej liczbie jedynek

/*
	Pod komentarzem funkcja zliczaj¹ca jedynki.
	Wywo³ywana zawsze z argumentem b = 32. 

	Poni¿sza tabela pokazuje z jak¹ liczb¹ porównywaæ
	argument b (lewa kolumna), ¿eby algorytm dzieli³ s³owo 
	wejœciowe na s³owa o po¿¹danej iloœci bitów (prawa kolumna):

	b:	  bity:
	32	- 64
	16	- 32
	8	- 16
	4	- 8
	2	- 4
	1	- 2
	0	- 1
	
	Przyk³adowo dla parametru = 16, szybciej wykonywany jest 
	"test szybkoœci 3" (testy w funkcji main) ni¿ dla
	parametru = 32

	Na koñcu pliku poda³em przyk³ad implementacji która na sztywno
	dzieli s³owo wejœciowe na 2 s³owa 32 - bitowe (funkcja licz32)
	oraz implementacji nie dziel¹cej s³owa wejœciowego (licz64)
*/
int licz(unsigned long long liczba, int b)
{
	if(!liczba)
		return 0;
	else if(b == 32){ //mo¿liwa zmiana parametru, zmieniæ te¿ poni¿ej
		while(liczba){
			b += liczba & 1;
			liczba >>= 1;
		}
		return b - 32; //wpisaæ ten sam parametr, co powy¿ej
	}else
		//podzia³ s³owa wejœciowego na wzór drzewa binarnego:
		return licz(liczba >> b, b >> 1) + licz(liczba & 0xffffffff >> 32 - b, b >> 1);
		
}

main()
{
	unsigned long long num = 0;
	unsigned long long k = 0;
	clock_t t;
	srand(time(NULL));
	
	//	test poprawnoœci:
	for(k = 0; k < 65; k++){
		num = losujIle(k);
		if(licz(num, 32) != k){
			printf("b³¹d dla liczby %d\n", k);
			return;
		}		
	}
	printf("test poprawnosci: ok\n");
	
	/*	
		w poni¿szych testach zmienna k jest iterowana ró¿n¹ iloœæ razy,
		bo wyniki s³u¿¹ do porównania ró¿nych algorytmów a nie wyników
		jednego algorytmu dla ró¿nych testów
	*/
	
	//	test szybkoœci dla losowego slowa:
	t = clock();
	for(k = 0; k < 0x1ffffff; k++)
		licz(losuj(), 32);
	t = clock() - t;
	printf("test szybkosci 1 -> liczba cykli: %u\n", t);
	
	//	test szybkoœci dla slowa bitowego o losowej (rozklad jednostajny) liczbie jedynek:
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

int licz64(unsigned long long liczba)
{
	int ile = 0;
	while(liczba){
		ile += liczba & 1;
		liczba >>= 1;
	}
	return ile;
}

int podLicz(unsigned long liczba) //funkcja wywo³ywana przez licz32
{
	int ile = 0;
	while(liczba){
		ile += liczba & 1;
		liczba >>= 1;
	}
	return ile;
}

int licz32(unsigned long *liczba)
{	
	if(!*(unsigned long long*)liczba){
		return 0;
	}
		
	return podLicz(*liczba++) + podLicz(*liczba);
}

