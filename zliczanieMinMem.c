#include <stdio.h>
#include <time.h>

//	funkcje pomocnicze:
unsigned long long losuj();				// s�owo o losowej liczbie jedynek (rozk�ad normalny)
unsigned long long losujIle(int ile1);	// s�owo o zadanej liczbie jedynek

/*
	Pod komentarzem funkcja zliczaj�ca jedynki.
	Wywo�ywana zawsze z argumentem b = 32. 

	Poni�sza tabela pokazuje z jak� liczb� por�wnywa�
	argument b (lewa kolumna), �eby algorytm dzieli� s�owo 
	wej�ciowe na s�owa o po��danej ilo�ci bit�w (prawa kolumna):

	b:	  bity:
	32	- 64
	16	- 32
	8	- 16
	4	- 8
	2	- 4
	1	- 2
	0	- 1
	
	Przyk�adowo dla parametru = 16, szybciej wykonywany jest 
	"test szybko�ci 3" (testy w funkcji main) ni� dla
	parametru = 32

	Na ko�cu pliku poda�em przyk�ad implementacji kt�ra na sztywno
	dzieli s�owo wej�ciowe na 2 s�owa 32 - bitowe (funkcja licz32)
	oraz implementacji nie dziel�cej s�owa wej�ciowego (licz64)
*/
int licz(unsigned long long liczba, int b)
{
	if(!liczba)
		return 0;
	else if(b == 32){ //mo�liwa zmiana parametru, zmieni� te� poni�ej
		while(liczba){
			b += liczba & 1;
			liczba >>= 1;
		}
		return b - 32; //wpisa� ten sam parametr, co powy�ej
	}else
		//podzia� s�owa wej�ciowego na wz�r drzewa binarnego:
		return licz(liczba >> b, b >> 1) + licz(liczba & 0xffffffff >> 32 - b, b >> 1);
		
}

main()
{
	unsigned long long num = 0;
	unsigned long long k = 0;
	clock_t t;
	srand(time(NULL));
	
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

int podLicz(unsigned long liczba) //funkcja wywo�ywana przez licz32
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

