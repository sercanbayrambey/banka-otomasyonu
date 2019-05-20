#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <locale.h>
#include <time.h>

//Fonksiyonlar	
void AnaMenu();
void BankaVerileriniGuncelle();
void BankaVerileriniCek();
void MusteriVerileriniCek();
void MusteriVerileriniGuncelle();
void YeniMusteri();
int KayitSorgula(int tip);
void MusteriGirisi();
void MusteriMenu();
int MusteriGirisSorgula(char TC[20],char sifre[25],char tip[12]);
void GirisYapilanHesabiProgramaCek();
void GirisYapilanHesabiGuncelle();
void HesapAc();
void HesaplarimMenu();
void HesabaParaYatir();
void HesaptanParaCek();
void ParayiEkHesaptanCek(float eksikMiktar,float cekilecekMiktar,int anaHesapID);
void HavaleYap();
int MusteriIDBul(int hesapNo);
int HesapIDBul(int hesapNo);
void IslemGecmisineEkle(int hesapNo,float tutar,int havaleYapilanHesapNo);
void HesapOzeti();
void HesapKapat();
void RaporAl(int menuTipi); // menuTipi = ana men� den mi geldi m��teri men�den mi 0 ANA MEN� 1 M��TER� MEN�
int GunlukLimitSorgula(float cekilecekMiktar);

	
	struct Islem
	{
		int No;
		int islemYapilanHesapNo;
		int havaleYapilanHesapNo;
		int gun,ay,yil,saat,dakika;
		float tutar;
		int Tip;
	};
	
	struct Hesap
	{	
		int No;
		int id;
		float bakiye;
	};
	
	
	struct AktifMusteri
	{
		char TC[15],sifre[20],adSoyad[40],tip[10];
		int No;
		int id,hesapsayisi,islemsayisi;
		struct Hesap hesap[100];
		struct Islem islem[100];	
	};
	
	struct bireyselMusteri
	{
		char TC[15],sifre[20],adSoyad[40],tip[10];
		int No;
		int id,hesapsayisi,islemsayisi;
		struct Hesap hesap[100];
		struct Islem islem[100];
		
		
	};
	
	struct ticariMusteri
	{
		char TC[15],sifre[20],adSoyad[40],tip[10];
		int No;
		int id,hesapsayisi,islemsayisi;
		struct Hesap hesap[100];
		struct Islem islem[100];	
	};

	
	struct Banka
	{
		struct AktifMusteri aktif_musteri;
		struct bireyselMusteri bry_musteriler[100];
		struct ticariMusteri tcr_musteriler[100];
		float hazine,gelir,gider;
		int girisYapilanID; // -1 Giris Yapilmadi
		int girisYapilanHesapTipi; // 0 BIREYSEL 1 TICARI
		int havaleYapilacakHesapTipi; // 0 BIREYSEL 1 TICARI
		int bireyselMS,ticariMS; //  ms = musteri sayisi
		int guncelYilGunu,guncelYil;
	}bnk; 
	
int gun,ay,yil,yilgun,saat,dakika;

int main()
{
	// Bug�n�n tarihini �ek
	time_t t;
	struct tm *zaman;
	t = time(NULL);
	zaman = localtime(&t);
	ay = zaman->tm_mon+1;
	gun = zaman->tm_mday; 
	yil = zaman->tm_year + 1900;
	saat = zaman->tm_hour;
	dakika = zaman->tm_min;
	yilgun = zaman->tm_yday;
	// Bug�n�n tarihini �ek
	 
	bnk.girisYapilanHesapTipi = -1;
	bnk.girisYapilanID = -1;
	bnk.havaleYapilacakHesapTipi = -1;
	setlocale(LC_ALL, "Turkish"); 
	BankaVerileriniCek();
	MusteriVerileriniCek();
	
	
	
	AnaMenu();
	return 0;
}

void RaporAl(int menuTipi) // menuTipi = ana men� den mi geldi m��teri men�den mi 0 ANA MEN� 1 M��TER� MEN�
{
	char secim[20];
	system("cls");
	printf("\n\n\n<------------ Banka Gelir - Gider Raporu ------------>\n");
	printf("\n\t<!> Bankan�n Geliri: %.2f TL",bnk.gelir);
	printf("\n\t<!> Bankan�n Gideri: %.2f TL",bnk.gider);
	printf("\n\t<!> Banka Hazinesi: %.2f TL",bnk.hazine);
	printf("\n\t<!> Bir �nceki men�ye d�nmek i�in bir tu�a bas�n�z..");
	*secim = getch();
	if(!menuTipi){
			AnaMenu();
			return;
		}
	else{
			MusteriMenu();
			return;
		}
}
void AnaMenu()
{
	
	char secim[20];
	system("cls");
	if(dakika<10)
		printf("\n%d.%d.%d  %d:0%d",gun,ay,yil,saat,dakika);
	else
		printf("\n%d.%d.%d  %d:%d",gun,ay,yil,saat,dakika);
	printf("\n\n<<<<< MKA Bankasi >>>>>\n\n");
	printf("<------ Ana Menu ------>\n");
	printf("<!> 1. Yeni M��teri Kayd�\n");
	printf("<!> 2. M��teri Giri�i\n");
	printf("<!> 3. Banka Gelir - Gider Raporu\n");
	printf("<!> Secim Yapiniz: ");
	
	scanf("%s",&secim);	
	
	if(!strcmp(secim,"1") ){
		YeniMusteri();
	}
	else if(!strcmp(secim,"2"))
		MusteriGirisi();
	else if(!strcmp(secim,"3") )
		RaporAl(0);
		
	else{
		printf("<!> Hatal� se�im yapt�n�z!");
		Sleep(1500);
		AnaMenu();	
		}	
}

void YeniMusteri()
{
	char secim[20],No[8];
	int i;
	int musteriTip=0; // Bireysel mi ticari mi ? // 0 B�REYSEL // 1 T�CAR�

	system("cls");
	printf("\n\n\n<------------ Yeni M��teri Kayd� ------------>\n");
	printf("\n<!> M��teri tipinizi se�iniz:\n\t1. Bireysel\n\t2. Ticari\n\t0. Ana Menuye D�n");
	printf("\nSe�iminiz: ");
	scanf("%s",&secim);	

	//Hesap Tipini Se�iniz
	if(strcmp(secim,"1") == 0)
			musteriTip=0;
	else if(strcmp(secim,"2") == 0)
		 	musteriTip=1;
	else if(strcmp(secim,"0") == 0)
		 	AnaMenu();
	else{
		printf("Hatal� bir se�im yapt�n�z!");
		Sleep(1500);
		YeniMusteri();
		return;
		}
		
			
	if(musteriTip == 0) // BIREYSEL MUSTERI KAYDI YAP
	{
		printf("<!>Ad Soyad:");
		fflush(stdin);
		gets((bnk.bry_musteriler+bnk.bireyselMS)->adSoyad); 
		if(!isalpha(*(bnk.bry_musteriler+bnk.bireyselMS)->adSoyad))
		{
			printf("\n\t<!> Ge�ersiz bir giri� yapt�n�z..");
			Sleep(3000);
			YeniMusteri();
		}
		printf("\n<!>TC:");
		gets((bnk.bry_musteriler+bnk.bireyselMS)->TC);
	
		if(KayitSorgula(0) == -1){
				printf("\n<!> B�yle bir m��teri zaten var.");
				Sleep(1500);
				YeniMusteri();
				return;
			}
			
		if(strlen((bnk.bry_musteriler+bnk.bireyselMS)->TC)!= 11 || isdigit(*(bnk.bry_musteriler+bnk.bireyselMS)->TC) == 0){
			printf("Hatal� bir kimlik numaras� girdiniz.");
			Sleep(1500);
			YeniMusteri();
			return;	
		}
		fflush(stdin);
		printf("\n<!>�ifre:");
		gets((bnk.bry_musteriler+bnk.bireyselMS)->sifre);
		strcpy((bnk.bry_musteriler+bnk.bireyselMS)->tip,"Bireysel");
		(bnk.bry_musteriler+bnk.bireyselMS)->No = RastgeleSayiOlustur(0);
		bnk.bireyselMS++;
	
	}//Bireysel musteri kayit son
	else{
	//Ticari Musteri Kaydi yap	
		printf("<!>Ad Soyad:");
		fflush(stdin);
		gets((bnk.tcr_musteriler+bnk.ticariMS)->adSoyad);
			if(!isalpha(*(bnk.tcr_musteriler+bnk.ticariMS)->adSoyad))
		{
			printf("\n\t<!> Ge�ersiz bir giri� yapt�n�z..");
			Sleep(3000);
			YeniMusteri();
		}
		
		
		printf("\n<!>TC:");
		gets((bnk.tcr_musteriler+bnk.ticariMS)->TC);
		if(KayitSorgula(1) == -1){
				printf("\n<!> B�yle bir m��teri zaten var.");
				Sleep(1500);
				YeniMusteri();
				return;
			}
			
		if(strlen((bnk.tcr_musteriler+bnk.ticariMS)->TC)!= 11 || isdigit(*(bnk.tcr_musteriler+bnk.ticariMS)->TC) == 0){
			printf("Hatal� bir kimlik numaras� girdiniz.");
			Sleep(1000);
			YeniMusteri();
			return;	
		}
		fflush(stdin);
		printf("\n<!>�ifre:");
		gets((bnk.tcr_musteriler+bnk.ticariMS)->sifre);
		strcpy((bnk.tcr_musteriler+bnk.ticariMS)->tip,"Ticari");
		(bnk.tcr_musteriler+bnk.ticariMS)->No = RastgeleSayiOlustur(0);
		bnk.ticariMS++;
	} //ticari musteri kayit son
		
	// MUSTERI OLUSTURULDU, GEREKLI GUNCELLEMELER YAPILSIN
		BankaVerileriniGuncelle();
		MusteriVerileriniGuncelle();
	//MUSTERI OLUSTURMA BITTI
	printf("\n<!> Kayd�n�z olu�turuldu. Ana men�ye y�nlendiriliyorsunuz...");
	Sleep(2000);
	AnaMenu();
}

int KayitSorgula(int tip)
{
	int i,j;
	if(tip == 0){	
		for(i=0;i<bnk.bireyselMS;i++){
		if(strcmp((bnk.bry_musteriler+i)->TC,(bnk.bry_musteriler+bnk.bireyselMS)->TC) == 0)	
			return -1;
	}
	
		return 0;
	}
	else{	
		for(i=0;i<bnk.ticariMS;i++){
			if(strcmp((bnk.tcr_musteriler+i)->TC,(bnk.tcr_musteriler+bnk.ticariMS)->TC) == 0)	
				return -1;
		}	
		return 0;
	}

}

void MusteriGirisi()
{
	char TC[15],sifre[20];
	char tip[10];
	system("cls");
	printf("\n\n\n<------------ M��teri Giri�i ------------>\n");
	printf("<!> TC Kimlik Numaras� (0 = �ptal Et): ");
	fflush(stdin);
	gets(TC);
	if(!strcmp(TC,"0"))
		AnaMenu();
		
	printf("\n<!> �ifre:");
	gets(sifre);
	printf("\n<!> Musteri tipiniz (1 = Bireysel, 2 = Ticari): ");
	gets(tip);
	
	if(strcmp(tip,"1") != 0 && strcmp(tip,"2") != 0 ){
		printf("<!> Hatal� bir se�im yapt�n�z!");
		Sleep(1500);
		MusteriGirisi();
		return;
		}
			
	
	if(MusteriGirisSorgula(TC,sifre,tip) == -1) //-1 d�nd�yse sorgulama tamam demektir
	{
		GirisYapilanHesabiProgramaCek();
		printf("\n<!> Giri� Ba�ar�l�!!\n\n");
		Sleep(1000);
		MusteriMenu();
	}
	else{
		printf("\n<!> Giri� Ba�ar�s�z!");
		Sleep(1000);
		MusteriGirisi();
		return;
	}
	return;
}

int MusteriGirisSorgula(char TC[20],char sifre[25],char tip[12])
{
	MusteriVerileriniCek();
	int i;
	if(strcmp(tip,"1") == 0) // tip 1 se bireysel sorgula
	{
		
		for(i=0;i<bnk.bireyselMS;i++){
			if(strcmp((bnk.bry_musteriler+i)->TC,TC) == 0 && strcmp((bnk.bry_musteriler+i)->sifre,sifre) == 0 ){
				bnk.girisYapilanID = i;
				bnk.girisYapilanHesapTipi = 0; // 0 B�REYSEL // 1 T�CARI
				return -1; // Giris basarili
					
			}
		}
		
	}
	else//ticari
	{
			for(i=0;i<bnk.ticariMS;i++){
				if(strcmp((bnk.tcr_musteriler+i)->TC,TC) == 0 && strcmp((bnk.tcr_musteriler+i)->sifre,sifre) == 0 ){
					bnk.girisYapilanID = i;
					bnk.girisYapilanHesapTipi = 1;
					return -1; // Giris basarili		
			}
		}
		
	}
	return 0; // giris basarisiz
}

void MusteriMenu()
{
	char secim[20];
	system("cls");
	printf("\n\n\n<------------ Musteri Menu ------------>\n");
	printf("<> Ad Soyad: %s\n<> M��teri No: %d\n<> TC: %s\n<> M��teri Tipi: %s\n\n",bnk.aktif_musteri.adSoyad,bnk.aktif_musteri.No,bnk.aktif_musteri.TC,bnk.aktif_musteri.tip);
			
	printf("<!> 1. Hesap A�ma\n");
	printf("<!> 2. Hesaplar�m\n");
	printf("<!> 3. Para Yat�rma\n");
	printf("<!> 4. Para �ekme\n");
	printf("<!> 5. Havale Yap\n");
	printf("<!> 6. Hesap �zetlerim\n");
	printf("<!> 7. Hesap Kapatma\n");
	printf("<!> 8. Banka Gelir - Gider Raporu\n");
	printf("<!> 0. ��k�� Yap\n");
	printf("<!> Se�im Yap�n�z: ");
	scanf("%s",&secim);
	
	
	
	if(strcmp(secim,"1") == 0)
		HesapAc();
	else if(!strcmp(secim,"2"))
		HesaplarimMenu();
	else if(!strcmp(secim,"3"))
		HesabaParaYatir();
	else if(!strcmp(secim,"4"))
		HesaptanParaCek();
	else if(!strcmp(secim,"5"))
		HavaleYap();
	else if(!strcmp(secim,"6"))
		HesapOzeti();
	else if(!strcmp(secim,"7"))
		HesapKapat();
	else if(!strcmp(secim,"8"))
		RaporAl(1);
		
	else if(!strcmp(secim,"0"))
	{
		bnk.girisYapilanHesapTipi = -1; // Cikis yap
		AnaMenu();
	}
		
		
	else{
		printf("<!> Hatal� bir se�im yapt�n�z !");
		Sleep(1000);
		MusteriMenu();	
		return;
		}
		
}




void HesapAc()
{
	char secim;
	system("cls");
	printf("\n\n\n<------------ Hesap Acma ------------>\n");
	printf("<> Ad Soyad: %s\n<> Musteri No: %d\n<> TC: %s\n<> Musteri Tipi: %s\n\n",bnk.aktif_musteri.adSoyad,bnk.aktif_musteri.No,bnk.aktif_musteri.TC,bnk.aktif_musteri.tip);
	(bnk.aktif_musteri.hesap+bnk.aktif_musteri.hesapsayisi)->No = RastgeleSayiOlustur(1);
	(bnk.aktif_musteri.hesap+bnk.aktif_musteri.hesapsayisi)->id = bnk.aktif_musteri.hesapsayisi;
	(bnk.aktif_musteri.hesap+bnk.aktif_musteri.hesapsayisi)->bakiye = 0;
	printf("<!> 1. Hesap Numaraniz: %d\n",(bnk.aktif_musteri.hesap+bnk.aktif_musteri.hesapsayisi)->No);
	printf("<!> Devam etmek ve hesab�n�z� a�mak i�in bir tu�a bas�n (0 = Islemi iptal et ve men�ye d�n) ...");
	secim = getch();
	if(secim == '0')
		MusteriMenu();
	
	 // Hesap Basariyla Acildi
	bnk.aktif_musteri.hesapsayisi++;
	MusteriVerileriniGuncelle();
	printf("\n<!> Hesab�n�z ba�ar�yla a��ld�!");
	Sleep(1500);
	MusteriMenu();
	return;
}


void HesapKapat()
{
	int i,j;
	char secim[20];
	int hesapID;
	system("cls");
	printf("\n\n\n<------------ Hesap Kapatma ------------>\n");
	printf("<> Ad Soyad: %s\n<> M��teri No: %d\n<> TC: %s\n<> M��teri Tipi: %s\n\n",bnk.aktif_musteri.adSoyad,bnk.aktif_musteri.No,bnk.aktif_musteri.TC,bnk.aktif_musteri.tip);

	if(!bnk.aktif_musteri.hesapsayisi)
		printf("\n\t<!> G�sterilecek hi� bir hesab�n�z yok...\n");


	for(i=0;i<bnk.aktif_musteri.hesapsayisi;i++){
		if(bnk.aktif_musteri.hesap[i].id == -1)
			continue;
		printf("\t----------------\n");
		printf("\t<!> Hesap ID: %d\n",bnk.aktif_musteri.hesap[i].id);
		printf("\t<!> Hesap No: %d\n",bnk.aktif_musteri.hesap[i].No);
		printf("\t<!> Hesap Bakiyesi: %.2f\n",bnk.aktif_musteri.hesap[i].bakiye);
		printf("\t----------------\n\n");
		
	}
	
		printf("\n\t<!> Kapatmak istedi�iniz hesap ID'sini giriniz (Hesab�n�z� kapatmak i�in i�inde para olmamal�d�r) (-1 Men�ye D�n): ");
		scanf("%s",&secim);
		
	if(!strcmp(secim,"-1")){
		MusteriMenu();
		return;
	}
	
	if(!isdigit(*secim)){
		printf("\t<!> Hatal� bir se�im yapt�n�z.");
		Sleep(1500);
		HesabaParaYatir();
		return;
	}
	
	hesapID = atoi(secim);
	
	if(bnk.aktif_musteri.hesap[hesapID].bakiye != 0)
	{
		printf("\n\t<!> Hesab�n�zdaki para 0 de�il! Mevcut miktar: %.2f",bnk.aktif_musteri.hesap[hesapID].bakiye);
		Sleep(3000);
		HesapKapat();
		return;
	}
	printf("\n\t<!> %d No'lu hesab�n�z kapat�lacak, devam etmek istiyorsan�z bir tu�a bas�n�z.. (0= �ptal Et ve Men�ye D�n)");
	*secim = getch();
	
	if(secim == "0"){
			MusteriMenu();
			return;
		}
	printf("\n\t<!> Hesap ba�ar�yla kapat�ld�!");
	bnk.aktif_musteri.hesap[hesapID].id = -1;
	MusteriVerileriniGuncelle();
	BankaVerileriniGuncelle();
	printf("\n\t<!> Men�ye d�nmek i�in bir tu�a bas�n�z..");
	*secim = getch();
	MusteriMenu();
	return;
}


void HesaplarimMenu()
{
	int i,j;
	char secim;
	system("cls");
	printf("\n\n\n<------------ Hesaplar�m ------------>\n");
	printf("<> Ad Soyad: %s\n<> M��teri No: %d\n<> TC: %s\n<> M��teri Tipi: %s\n\n",bnk.aktif_musteri.adSoyad,bnk.aktif_musteri.No,bnk.aktif_musteri.TC,bnk.aktif_musteri.tip);

	if(!bnk.aktif_musteri.hesapsayisi)
		printf("\n\t<!> G�sterilecek hi� bir hesab�n�z yok...\n");


	for(i=0;i<bnk.aktif_musteri.hesapsayisi;i++){
		if((bnk.aktif_musteri.hesap+i)->id == -1)
			continue;
		printf("\t----------------\n");
		printf("\t<!> Hesap ID: %d\n",(bnk.aktif_musteri.hesap+i)->id);
		printf("\t<!> Hesap No: %d\n",(bnk.aktif_musteri.hesap+i)->No);
		printf("\t<!> Hesap Bakiyesi: %.2f\n",(bnk.aktif_musteri.hesap+i)->bakiye);
		printf("\t----------------\n\n");
		
	}
	printf("<!> Geriye d�nmek i�in bir tu�a bas�n�z....");
	secim = getch();
	MusteriMenu();
	return;
}

void HesabaParaYatir()
{
	int i,j;
	system("cls");
	char para[20];
	char *secim;
	int secilenID;
	float yatirilacakMiktar;
	printf("\n\n\n<------------ Para Yat�rma ------------>\n");
	
	if(!bnk.aktif_musteri.hesapsayisi)
		printf("\n\t<!> G�sterilecek hi� bir hesab�n�z yok...\n");
	
	for(i=0;i<bnk.aktif_musteri.hesapsayisi;i++){
		if((bnk.aktif_musteri.hesap+i)->id == -1)
			continue;
		printf("\t----------------\n");
		printf("\t<!> Hesap ID: %d\n",(bnk.aktif_musteri.hesap+i)->id);
		printf("\t<!> Hesap No: %d\n",(bnk.aktif_musteri.hesap+i)->No);
		printf("\t<!> Hesap Bakiyesi: %.2f\n",(bnk.aktif_musteri.hesap+i)->bakiye);
		printf("\t----------------\n\n");
		
	}
	
	printf("\n\n\t<!> Para yat�rmak istedi�iniz hesap ID sini giriniz (-1 = �nceki Men�ye D�n): ");
	scanf("%s",secim);
	
	if(!strcmp(secim,"-1")){
		MusteriMenu();
		return;
	}
	
	if(!isdigit(*secim)){
		printf("\t<!> Hatal� bir se�im yapt�n�z.");
		Sleep(1500);
		HesabaParaYatir();
		return;
	}
	
	secilenID = atoi(secim);
	
	if(secilenID>=bnk.aktif_musteri.hesapsayisi || secilenID<0){
		printf("\t<!> Ge�ersiz bir ID numaras� girdiniz.");
		Sleep(1500);
		HesabaParaYatir();
		return;
	}
	printf("\t<!> Hesap Numaraniz: %d, devam etmek i�in bir tu�a bas�n�z... (0 = I�lemi �ptal Et ve Men�ye D�n)",(bnk.aktif_musteri.hesap+secilenID)->No);
	*secim = getch();
	if(secim == "0"){
			MusteriMenu();
			return;
		}
		
	printf("\n\t<!> Yat�rmak istedi�iniz miktar� yaz�n�z: ");
	scanf("%s",&para);
	if(!isdigit(*para)){
		printf("\t<!> Hatal� bir para girdiniz.");
		Sleep(1500);
		HesabaParaYatir();
		return;
	}
	
	yatirilacakMiktar = atof(para);
	printf("\n\t<!> Hesab�n�za yat�r�lacak miktar %.2f TL, onayl�yorsan�z bir tu�a bas�n�z... (0 = �slemi �ptal Et ve Men�ye D�n)",yatirilacakMiktar);
	*secim = getch();
	if(secim == "0")
		{
			MusteriMenu();
			return;
		}
	// Paray�Hesaba Ekle
	IslemGecmisineEkle((bnk.aktif_musteri.hesap+secilenID)->No,yatirilacakMiktar,0);
	(bnk.aktif_musteri.hesap+secilenID)->bakiye+=yatirilacakMiktar;
	bnk.gelir += yatirilacakMiktar;
	MusteriVerileriniGuncelle();
	BankaVerileriniGuncelle();

	printf("\n\t<!> %.2f TL hesab�n�za yat�r�ld�. Yeni bakiyeniz: %.2f. \n\t<!>Ana men�ye d�nmek i�in bir tu�a bas�n�z...\n\n\n\n",yatirilacakMiktar,(bnk.aktif_musteri.hesap+secilenID)->bakiye);
	*secim = getch();
	MusteriMenu();
	return;
}


void HesaptanParaCek()
{
	int i,j;
	system("cls");
	char para[20];
	char *secim;
	int secilenID;
	float cekilecekMiktar;
	float eksikMiktar;
	printf("\n\n\n<------------ Para �ekme ------------>\n");
	
	if(!bnk.aktif_musteri.hesapsayisi)
		printf("\n\t<!> G�sterilecek hi� bir hesab�n�z yok...\n");

	for(i=0;i<bnk.aktif_musteri.hesapsayisi;i++)
	{
		if((bnk.aktif_musteri.hesap+i)->id == -1)
			continue;
		printf("\t----------------\n");
		printf("\t<!> Hesap ID: %d\n",(bnk.aktif_musteri.hesap+i)->id);
		printf("\t<!> Hesap No: %d\n",(bnk.aktif_musteri.hesap+i)->No);
		printf("\t<!> Hesap Bakiyesi: %.2f\n",(bnk.aktif_musteri.hesap+i)->bakiye);
		printf("\t----------------\n\n");
		
	}
	printf("\n\n\t<!> Para �ekmek istedi�iniz hesap ID sini giriniz (-1 = �nceki Men�ye D�n): ");
	scanf("%s",secim);
	if(!strcmp(secim,"-1")){
		MusteriMenu();
		return;
	}
	
	if(isdigit(*secim) == 0){
		printf("\t<!> Hatal� bir se�im yapt�n�z.");
		Sleep(1500);
		HesaptanParaCek();
		return;
	}
	
	secilenID = atoi(secim);
	if(secilenID>=bnk.aktif_musteri.hesapsayisi || secilenID<0){
		printf("\t<!> Ge�ersiz bir ID numaras� girdiniz.");
		Sleep(2000);
		HesaptanParaCek();
		return;
	}
	
	printf("\n\t<!> Hesap Numaraniz: %d, devam etmek i�in bir tu�a bas�n�z... (0 = I�lemi �ptal Et ve Men�ye D�n)",(bnk.aktif_musteri.hesap+secilenID)->No);
	*secim = getch();
	if(secim == "0"){
			MusteriMenu();
			return;
		}
		
	printf("\n\t<!> �ekmek istedi�iniz miktar� yaz�n�z: ");
	scanf("%s",&para);
	if(isdigit(*para) == 0){
		printf("\t<!> Hatal� bir para girdiniz.");
		Sleep(1500);
		HesaptanParaCek();
		return;
	}
	
	cekilecekMiktar = atof(para);
	
	if(!GunlukLimitSorgula(cekilecekMiktar))
	{
		printf("\n\t<!> G�nl�k para �ekme limitinizi ge�iyorsunuz.");
		printf("\n\t<!> Bu limit ticari hesaplarda 1500 TL, bireysel hesaplarda 750 TL'dir.");
		printf("\n\t<!> Ana men�ye d�nmek i�in bir tu�a bas�n�z...");
		*secim = getch();
		MusteriMenu();
		return;
	}

	if(cekilecekMiktar>(bnk.aktif_musteri.hesap+secilenID)->bakiye){
		eksikMiktar = cekilecekMiktar - (bnk.aktif_musteri.hesap+secilenID)->bakiye;
		printf("\n\t<!> Hesab�n�zda o kadar para yok. Eksik olan %0.2f TL'yi ek hesab�n�zdan �ekmek i�in bir tu�a bas�n.. (0 = ��lemi �ptal Et)",eksikMiktar);
		*secim = getch();
		if(secim == "0"){
			printf("<!> Para �ekme i�lemi ba�ar�s�z oldu!");
			Sleep(1500);
			MusteriMenu();
			return;
		}
		
		ParayiEkHesaptanCek(eksikMiktar,cekilecekMiktar,secilenID);
		return;
	}
	

	printf("\n\t<!> Hesab�n�zdan �ekilecek miktar %.2f TL, onayl�yorsan�z bir tu�a bas�n�z... (0 = �slemi �ptal Et ve Men�ye D�n)",cekilecekMiktar);
	*secim = getch();
	if(secim == "0"){
			MusteriMenu();
			return;
		}
	// Paray� Hesaptan Kes
	IslemGecmisineEkle((bnk.aktif_musteri.hesap+secilenID)->No,-cekilecekMiktar,0);
	(bnk.aktif_musteri.hesap+secilenID)->bakiye-=cekilecekMiktar;
	bnk.gider += cekilecekMiktar;
	MusteriVerileriniGuncelle();
	BankaVerileriniGuncelle();
	//Paray� Hesaptan Kesme Son
	printf("\n\t<!> %.2f TL hesab�n�zdan �ekildi. Yeni bakiyeniz: %.2f. \n\t<!>Ana men�ye d�nmek i�in bir tu�a bas�n�z...\n\n\n\n",cekilecekMiktar,(bnk.aktif_musteri.hesap+secilenID)->bakiye);
	*secim = getch();
	MusteriMenu();
	return;
}
void ParayiEkHesaptanCek(float eksikMiktar,float cekilecekMiktar,int anaHesapID)
{
	int i;
	int hesapID = -1;// e�er -1 kal�rsa ek hesab�nda da para yok
	char *secim;
	int gonderilecekMiktar;
	//Hesaplar� gez eksikmiktardan b�y�k olan hesap varsa ondan �ek
	for(i=0;i<bnk.aktif_musteri.hesapsayisi;i++){
			if((bnk.aktif_musteri.hesap+i)->bakiye >= eksikMiktar && (bnk.aktif_musteri.hesap+i)->id != anaHesapID){
				hesapID = i;
				break;
			}
			
		}
		
	if(hesapID == -1){
			printf("\n\t<!> Hi� bir hesab�n�zda %.2f TL bulunamadi. Ana menuye y�nlendiriliyorsunuz..",eksikMiktar);
			Sleep(5000);
			MusteriMenu();
			return;
		}
		printf("\n\t<!> Eksik olan %.2f TL, %d No'lu hesab�n�zdan �ekilecek.",eksikMiktar,bnk.aktif_musteri.hesap[hesapID].No);
		printf("\n\t<!> Bu hesab�n�zdaki mevcut bakiye %.2f TL, devam etmek istiyorsan�z bir tu�a bas�n�z..(0 = ��lemi �ptal Et)",bnk.aktif_musteri.hesap[hesapID].bakiye);
		*secim = getch();
		if(secim == "0"){
				MusteriMenu();
				return;
			}
			//Ek hesaptan ve orjinal hesaptan paray� �ek
			IslemGecmisineEkle(bnk.aktif_musteri.hesap[anaHesapID].No,-cekilecekMiktar,0);
			bnk.aktif_musteri.hesap[hesapID].bakiye-=eksikMiktar;
			bnk.aktif_musteri.hesap[anaHesapID].bakiye = 0;
			bnk.gider+= cekilecekMiktar;
			MusteriVerileriniGuncelle();
			BankaVerileriniGuncelle();
			printf("\n\t<!> %.2f TL hesaplar�n�zdan ba�ar�yla �ekildi, ana menuye d�nmek i�in bir tu�a bas�n�z..",cekilecekMiktar);
			*secim = getch();
			MusteriMenu();
			return;
}


void HavaleYap()
{
	int i,j;
	system("cls");
	char para[20];
	char tempHesapNo[20];
	char *secim;
	int secilenID;
	float gonderilecekMiktar,komisyon;
	int havaleHesapNo;
	int havaleHesapID;
	int havaleMusteriID;

	printf("\n\n\n<------------ Havale ------------>\n");
	if(!bnk.aktif_musteri.hesapsayisi)
		printf("\n\t<!> G�sterilecek hi� bir hesab�n�z yok...\n");
	for(i=0;i<bnk.aktif_musteri.hesapsayisi;i++){
		if((bnk.aktif_musteri.hesap+i)->id == -1)
			continue;
		printf("\t----------------\n");
		printf("\t<!> Hesap ID: %d\n",(bnk.aktif_musteri.hesap+i)->id);
		printf("\t<!> Hesap No: %d\n",(bnk.aktif_musteri.hesap+i)->No);
		printf("\t<!> Hesap Bakiyesi: %.2f\n",(bnk.aktif_musteri.hesap+i)->bakiye);
		printf("\t----------------\n\n");
		
	}
	
	printf("\n\n\t<!> Paran�z�n �ekilece�i hesap ID's�n� giriniz (-1 Men�ye D�n): ");
	scanf("%s",secim);
	
	if(strcmp(secim,"-1") == 0){
		MusteriMenu();
		return;
	}
	
	if(isdigit(*secim) == 0){
		printf("\t<!> Hatal� bir se�im yapt�n�z.");
		Sleep(1500);
		HavaleYap();
		return;
	}	
	secilenID = atoi(secim);
	if(secilenID>=bnk.aktif_musteri.hesapsayisi || secilenID<0){
		printf("\t<!> Ge�ersiz bir ID numaras� girdiniz.");
		Sleep(2000);
		HavaleYap();
		return;
	}
	
	printf("\n\t<!> Hesap Numaraniz: %d, devam etmek i�in bir tu�a bas�n�z... (0 = I�lemi �ptal Et ve Men�ye D�n)",(bnk.aktif_musteri.hesap+secilenID)->No);
	*secim = getch();
	if(secim == "0"){
			MusteriMenu();
			return;
		}
		
	printf("\n\t<!> G�ndermek istedi�iniz miktar� yaz�n�z (Bireysel M��terilerden %%2 islem �creti kesilir.): ");
	scanf("%s",&para);
	
	if(isdigit(*para) == 0){
		printf("\t<!> Hatal� bir para girdiniz.");
		Sleep(2000);
		HavaleYap();
		return;
	}
	
	if(bnk.girisYapilanHesapTipi == 0) // Bireysel ise %2 i�lem �creti kes
		komisyon = atof(para)*2/100;
	else
		komisyon = 0;

	gonderilecekMiktar = atof(para) + komisyon;
	
	if(gonderilecekMiktar>(bnk.aktif_musteri.hesap+secilenID)->bakiye){
		printf("\n\t<!> Hesab�n�zda o kadar para yok. Eksik olan: %0.2f TL Komisyon: %.2f TL. Men�ye y�nlendiriliyorsunuz..",(bnk.aktif_musteri.hesap+secilenID)->bakiye-gonderilecekMiktar,komisyon);
		Sleep(4000);
		MusteriMenu();
		return;
	}
	printf("\n\t<!> %.2f TL g�ndereceksiniz, i�lem �creti %.2f TL",gonderilecekMiktar-komisyon,komisyon);
	printf("\n\t<!> Paray� g�nderece�iniz Hesap No: ");
	scanf("%s",&tempHesapNo);
	if(isdigit(*tempHesapNo) == 0){
		printf("\n\t<!> Hatal� bir Hesap No girdiniz.");
		Sleep(3000);
		HavaleYap();
		return;
	}
	havaleHesapNo = atoi(tempHesapNo);
	havaleMusteriID = MusteriIDBul(havaleHesapNo);
	havaleHesapID = HesapIDBul(havaleHesapNo);
	if(havaleMusteriID == -1){
		printf("\n\t<!> B�yle bir m��teri bulunamad� veya kendi hesab�n�za havale yapmaya �al���yorsunuz! ");
		Sleep(3000);
		HavaleYap();
		return;
	}
	
	
	if(bnk.havaleYapilacakHesapTipi == 0) // Bireysel ��lemler
	{
	printf("\n\t<!> %.2f TL g�nderece�iniz Hesap No: %d, Kay�tl� Oldu�u Ad Soyad: %s",gonderilecekMiktar-komisyon,havaleHesapNo,bnk.bry_musteriler[havaleMusteriID].adSoyad);
	printf("\n\t<!> Onayl�yorsan�z bir tu�a bas�n... (0= �ptal)");
	*secim = getch();
	if(secim == "0"){
			HavaleYap();
			return;
		}
		
	//Gonderme basarili		
	IslemGecmisineEkle((bnk.aktif_musteri.hesap+secilenID)->No,gonderilecekMiktar-komisyon,bnk.bry_musteriler[havaleMusteriID].hesap[havaleHesapID].No);
	(bnk.aktif_musteri.hesap+secilenID)->bakiye-=gonderilecekMiktar;
	bnk.bry_musteriler[havaleMusteriID].hesap[havaleHesapID].bakiye+= (gonderilecekMiktar - komisyon);			
	MusteriVerileriniGuncelle();
	BankaVerileriniGuncelle();
	bnk.gelir += komisyon;
	printf("\n\t<!> %.2f TL %d nolu hesaba ba�ar�yla g�nderildi.",gonderilecekMiktar-komisyon,havaleHesapNo);
	printf("\n\t<!> Men�ye d�nmek i�in bir tu�a bas�n�z...");
	*secim = getch();
	MusteriMenu();
	return;
	}
	
	else // T�CAR� HESABA G�NDER
		{
	printf("\n\t<!> %.2f TL g�nderece�iniz Hesap No: %d, Kay�tl� Oldu�u Ad Soyad: %s",gonderilecekMiktar-komisyon,havaleHesapNo,bnk.tcr_musteriler[havaleMusteriID].adSoyad);
	printf("\n\t<!> Onayl�yorsan�z bir tu�a bas�n... (0= �ptal)");
	*secim = getch();
	if(secim == "0"){
			HavaleYap();
			return;
		}
	
	//Gonderme basarili		
	bnk.gelir += komisyon;
	IslemGecmisineEkle((bnk.aktif_musteri.hesap+secilenID)->No,gonderilecekMiktar-komisyon,bnk.tcr_musteriler[havaleMusteriID].hesap[havaleHesapID].No);
	(bnk.aktif_musteri.hesap+secilenID)->bakiye-=gonderilecekMiktar;
	bnk.tcr_musteriler[havaleMusteriID].hesap[havaleHesapID].bakiye+= (gonderilecekMiktar - komisyon);
	MusteriVerileriniGuncelle();
	BankaVerileriniGuncelle();
	printf("\n\t<!> %.2f TL %d nolu hesaba ba�ar�yla g�nderildi.",gonderilecekMiktar-komisyon,havaleHesapNo);
	printf("\n\t<!> Men�ye d�nmek i�in bir tu�a bas�n�z...");
	*secim = getch();
	MusteriMenu();
	return;
	}	
	return;	
}

int MusteriIDBul(int hesapNo)
{
	bnk.havaleYapilacakHesapTipi = -1;
	int i,j;
	int id = -1;
	for(i=0;i<bnk.bireyselMS;i++){
		for(j=0;j<(bnk.bry_musteriler+i)->hesapsayisi;j++){
			if(hesapNo == (bnk.bry_musteriler+i)->hesap[j].No && bnk.bry_musteriler[i].No != bnk.aktif_musteri.No)	{
				id = (bnk.bry_musteriler+i)->id;	
				bnk.havaleYapilacakHesapTipi = 0;			
				return id;		
				}		
		}
	}
	
	for(i=0;i<bnk.ticariMS;i++)
	{
		for(j=0;j<(bnk.tcr_musteriler+i)->hesapsayisi;j++){
			if(hesapNo == (bnk.tcr_musteriler+i)->hesap[j].No && bnk.tcr_musteriler[i].No != bnk.aktif_musteri.No){
				id = (bnk.tcr_musteriler+i)->id;
	
				bnk.havaleYapilacakHesapTipi = 1;
				return id;					
				}		
		}
	}
		
return id; // -1 d�nerse b�yle bir hesap yok !	
}


int HesapIDBul(int hesapNo)
{
	int i,j;
	int id = -1;
	for(i=0;i<bnk.bireyselMS;i++){
		for(j=0;j<(bnk.bry_musteriler+i)->hesapsayisi;j++){
			if(hesapNo == (bnk.bry_musteriler+i)->hesap[j].No){
				id = (bnk.bry_musteriler+i)->hesap[j].id;
				return id;					
				}
				
		}
	}
	
	for(i=0;i<bnk.ticariMS;i++){
		for(j=0;j<(bnk.tcr_musteriler+i)->hesapsayisi;j++){
			if(hesapNo == (bnk.tcr_musteriler+i)->hesap[j].No){
				id = (bnk.tcr_musteriler+i)->hesap[j].id;
				return id;					
				}
				
		}
	}
	return id; // -1 d�nerse b�yle bir hesap yok !
}


void HesapOzeti()
{
	int i,j;
	char secim[20];
	int gosterilenIslemSayisi=0;
	int secilenID;
	int secilenAy;
	FILE *fp;
	fp = fopen("dekont.txt","w");
	system("cls");
	printf("\n\n\n<------------ Hesaplar�m ------------>\n");
	printf("<> Ad Soyad: %s\n<> M��teri No: %d\n<> TC: %s\n<> M��teri Tipi: %s\n\n",bnk.aktif_musteri.adSoyad,bnk.aktif_musteri.No,bnk.aktif_musteri.TC,bnk.aktif_musteri.tip);

	if(!bnk.aktif_musteri.hesapsayisi)
		printf("\n\t<!> G�sterilecek hi� bir hesab�n�z yok...\n");

	for(i=0;i<bnk.aktif_musteri.hesapsayisi;i++)
	{
		if((bnk.aktif_musteri.hesap+i)->id == -1)
			continue;
		printf("\t----------------\n");
		printf("\t<!> Hesap ID: %d\n",(bnk.aktif_musteri.hesap+i)->id);
		printf("\t<!> Hesap No: %d\n",(bnk.aktif_musteri.hesap+i)->No);
		printf("\t<!> Hesap Bakiyesi: %.2f\n",(bnk.aktif_musteri.hesap+i)->bakiye);
		printf("\t----------------\n\n");	
	}
	
	printf("\n\n\t<!> ��lem kayd�n� g�rmek istedi�iniz hesap ID's�n� giriniz (-1 Men�ye D�n): ");
	scanf("%s",secim);
	
	if(strcmp(secim,"-1") == 0){
		MusteriMenu();
		return;
	}
		
	if(isdigit(*secim) == 0){
		printf("\t<!> Hatal� bir se�im yapt�n�z.");
		Sleep(2000);
		HesapOzeti();
		return;
	}
	
	secilenID = atoi(secim);
	
	if(secilenID>=bnk.aktif_musteri.hesapsayisi || secilenID<0){
		printf("\t<!> Ge�ersiz bir ID numaras� girdiniz.");
		Sleep(2000);
		HesapOzeti();
		return;
	}
	
	printf("\t<!> Islem kaydini gormek istediginiz ayin numarasini giriniz (1 = Ocak 0 = T�m ��lemler): ");
	scanf("%s",secim);
	
	if(isdigit(*secim) == 0 || atoi(secim)<0 || atoi(secim)>12) {
		printf("\t<!> Hatal� bir se�im yapt�n�z.");
		Sleep(2000);
		HesapOzeti();
		return;
	}
	
	secilenAy = atoi(secim);
	
	for(i=0;i<bnk.aktif_musteri.islemsayisi;i++)
		{
			if((bnk.aktif_musteri.hesap+secilenID)->No == (bnk.aktif_musteri.islem+i)->islemYapilanHesapNo && (secilenAy == (bnk.aktif_musteri.islem+i)->ay || secilenAy == 0) )
			{
				printf("\n\t------------------------------------------------");
				if((bnk.aktif_musteri.islem+i)->dakika<10)
				{
					printf("\n\t[%d.%d.%d %d:0%d] ",(bnk.aktif_musteri.islem+i)->gun,(bnk.aktif_musteri.islem+i)->ay,(bnk.aktif_musteri.islem+i)->yil,(bnk.aktif_musteri.islem+i)->saat,(bnk.aktif_musteri.islem+i)->dakika);	
					fprintf(fp,"\n\t[%d.%d.%d %d:0%d] ",(bnk.aktif_musteri.islem+i)->gun,(bnk.aktif_musteri.islem+i)->ay,(bnk.aktif_musteri.islem+i)->yil,(bnk.aktif_musteri.islem+i)->saat,(bnk.aktif_musteri.islem+i)->dakika);	
				}
				else
				{
					printf("\n\t[%d.%d.%d %d:%d] ",(bnk.aktif_musteri.islem+i)->gun,(bnk.aktif_musteri.islem+i)->ay,(bnk.aktif_musteri.islem+i)->yil,(bnk.aktif_musteri.islem+i)->saat,(bnk.aktif_musteri.islem+i)->dakika);
					fprintf(fp,"\n\t[%d.%d.%d %d:%d] ",(bnk.aktif_musteri.islem+i)->gun,(bnk.aktif_musteri.islem+i)->ay,(bnk.aktif_musteri.islem+i)->yil,(bnk.aktif_musteri.islem+i)->saat,(bnk.aktif_musteri.islem+i)->dakika);
				}
				
				if((bnk.aktif_musteri.islem+i)->tutar<0) // Para cekilmis
				{
					printf("%d No'lu hesaptan %.2f TL para �ekildi.",(bnk.aktif_musteri.hesap+secilenID)->No,-(bnk.aktif_musteri.islem+i)->tutar);
					fprintf(fp,"%d No'lu hesaptan %.2f TL para �ekildi.",(bnk.aktif_musteri.hesap+secilenID)->No,-(bnk.aktif_musteri.islem+i)->tutar);
				}
				else if((bnk.aktif_musteri.islem+i)->tutar>0 && (bnk.aktif_musteri.islem+i)->havaleYapilanHesapNo == 0) // para yatirilmis
				{
					printf("%d No'lu hesaba %.2f TL para yat�r�ld�.",(bnk.aktif_musteri.hesap+secilenID)->No,(bnk.aktif_musteri.islem+i)->tutar);
					fprintf(fp,"%d No'lu hesaba %.2f TL para yat�r�ld�.",(bnk.aktif_musteri.hesap+secilenID)->No,(bnk.aktif_musteri.islem+i)->tutar);
				}
				else // havale
				{
					printf("%d No'lu hesaptan '%d No'lu hesaba %.2f TL havale yap�ld�.",(bnk.aktif_musteri.hesap+secilenID)->No,(bnk.aktif_musteri.islem+i)->havaleYapilanHesapNo,(bnk.aktif_musteri.islem+i)->tutar);
					fprintf(fp,"%d No'lu hesaptan '%d No'lu hesaba %.2f TL havale yap�ld�.",(bnk.aktif_musteri.hesap+secilenID)->No,(bnk.aktif_musteri.islem+i)->havaleYapilanHesapNo,(bnk.aktif_musteri.islem+i)->tutar);
				}
				printf("\n\t------------------------------------------------");
				printf("\n");
				fprintf(fp,"\n\t------------------------------------------------\n");
				gosterilenIslemSayisi++;		
			}
			
		}
		if(gosterilenIslemSayisi == 0)
		{
			printf("\n\t<!> Belirtilen aral�kta hi� bir i�lem kayd� bulunamad�!");
			printf("\n\t<!> Ana men�ye d�nmek i�in bir tu�a bas�n�z...(0 = Ba�ka Bir Hesap �zeti ��kar)");
			*secim = getch();
			if(!strcmp(secim,"0")){
				HesapOzeti();
				return;
			}
			return;
		}	
		fclose(fp);	
		printf("\n\t<!> Dekont istiyorsan�z 0 (s�f�r) tu�una bas�n�z..(�stemiyorsan�z her hangi bir tu�a bas�n..)");
		*secim = getch();
		if(!strcmp(secim,"0"))
				printf("\n\t<!> Dekont ba�ar�yla \"dekont.txt\" dosyas�na yazd�r�ld�. ");	
		else{
			remove("dekont.txt");
		}
		printf("\n\t<!> Ana men�ye d�nmek i�in bir tu�a bas�n�z...(0 = Ba�ka Bir Hesap �zeti ��kar)");
		*secim = getch();
		if(!strcmp(secim,"0")){
				HesapOzeti();
				return;
			}
		MusteriMenu();
		return;	
}

void IslemGecmisineEkle(int hesapNo,float tutar,int havaleYapilanHesapNo) 
{
	(bnk.aktif_musteri.islem+bnk.aktif_musteri.islemsayisi)->islemYapilanHesapNo = hesapNo;
	(bnk.aktif_musteri.islem+bnk.aktif_musteri.islemsayisi)->havaleYapilanHesapNo = havaleYapilanHesapNo;
	(bnk.aktif_musteri.islem+bnk.aktif_musteri.islemsayisi)->tutar = tutar;
	(bnk.aktif_musteri.islem+bnk.aktif_musteri.islemsayisi)->gun = gun;
	(bnk.aktif_musteri.islem+bnk.aktif_musteri.islemsayisi)->ay = ay;
	(bnk.aktif_musteri.islem+bnk.aktif_musteri.islemsayisi)->yil = yil;
	(bnk.aktif_musteri.islem+bnk.aktif_musteri.islemsayisi)->saat = saat;
	(bnk.aktif_musteri.islem+bnk.aktif_musteri.islemsayisi)->dakika = dakika;
	bnk.aktif_musteri.islemsayisi++;	
	return;
}

void GirisYapilanHesabiProgramaCek()
{
	int i,j;
	if(bnk.girisYapilanHesapTipi == 0) // B�REYSEL �SE
	{
		bnk.aktif_musteri.No = (bnk.bry_musteriler+bnk.girisYapilanID)->No;
		bnk.aktif_musteri.id = bnk.girisYapilanID;
		bnk.aktif_musteri.hesapsayisi  = (bnk.bry_musteriler+bnk.girisYapilanID)->hesapsayisi;
		bnk.aktif_musteri.islemsayisi = (bnk.bry_musteriler+bnk.girisYapilanID)->islemsayisi;
		strcpy(bnk.aktif_musteri.adSoyad,(bnk.bry_musteriler+bnk.girisYapilanID)->adSoyad);
		strcpy(bnk.aktif_musteri.TC,(bnk.bry_musteriler+bnk.girisYapilanID)->TC);
		strcpy(bnk.aktif_musteri.sifre,(bnk.bry_musteriler+bnk.girisYapilanID)->sifre);
		strcpy(bnk.aktif_musteri.tip,(bnk.bry_musteriler+bnk.girisYapilanID)->tip);
		for(i=0;i<(bnk.bry_musteriler+bnk.girisYapilanID)->hesapsayisi;i++) // hesap verilerini �ek	
		{
			(bnk.aktif_musteri.hesap+i)->id = (bnk.bry_musteriler+bnk.girisYapilanID)->hesap[i].id;
			(bnk.aktif_musteri.hesap+i)->bakiye = (bnk.bry_musteriler+bnk.girisYapilanID)->hesap[i].bakiye;
			(bnk.aktif_musteri.hesap+i)->No = (bnk.bry_musteriler+bnk.girisYapilanID)->hesap[i].No;
		}
		for(i=0;i<(bnk.bry_musteriler+bnk.girisYapilanID)->islemsayisi;i++){
		(bnk.aktif_musteri.islem+i)->islemYapilanHesapNo = (bnk.bry_musteriler+bnk.girisYapilanID)->islem[i].islemYapilanHesapNo;
		(bnk.aktif_musteri.islem+i)->tutar = (bnk.bry_musteriler+bnk.girisYapilanID)->islem[i].tutar;
		(bnk.aktif_musteri.islem+i)->gun = (bnk.bry_musteriler+bnk.girisYapilanID)->islem[i].gun;
		(bnk.aktif_musteri.islem+i)->ay = (bnk.bry_musteriler+bnk.girisYapilanID)->islem[i].ay;
		(bnk.aktif_musteri.islem+i)->yil = (bnk.bry_musteriler+bnk.girisYapilanID)->islem[i].yil;
		(bnk.aktif_musteri.islem+i)->saat = (bnk.bry_musteriler+bnk.girisYapilanID)->islem[i].saat;
		(bnk.aktif_musteri.islem+i)->dakika = (bnk.bry_musteriler+bnk.girisYapilanID)->islem[i].dakika;
		(bnk.aktif_musteri.islem+i)->havaleYapilanHesapNo = (bnk.bry_musteriler+bnk.girisYapilanID)->islem[i].havaleYapilanHesapNo;
		}
		
	}
	else  //T�CAR�
		{
		bnk.aktif_musteri.No = (bnk.tcr_musteriler+bnk.girisYapilanID)->No;
		bnk.aktif_musteri.id = bnk.girisYapilanID;
		bnk.aktif_musteri.hesapsayisi  = (bnk.tcr_musteriler+bnk.girisYapilanID)->hesapsayisi;
		bnk.aktif_musteri.islemsayisi = (bnk.tcr_musteriler+bnk.girisYapilanID)->islemsayisi;
		strcpy(bnk.aktif_musteri.adSoyad,(bnk.tcr_musteriler+bnk.girisYapilanID)->adSoyad);
		strcpy(bnk.aktif_musteri.TC,(bnk.tcr_musteriler+bnk.girisYapilanID)->TC);
		strcpy(bnk.aktif_musteri.sifre,(bnk.tcr_musteriler+bnk.girisYapilanID)->sifre);
		strcpy(bnk.aktif_musteri.tip,(bnk.tcr_musteriler+bnk.girisYapilanID)->tip);
		
		for(i=0;i<(bnk.tcr_musteriler+bnk.girisYapilanID)->hesapsayisi;i++) // hesap verilerini �ek	
		{
			(bnk.aktif_musteri.hesap+i)->id = (bnk.tcr_musteriler+bnk.girisYapilanID)->hesap[i].id;
			(bnk.aktif_musteri.hesap+i)->bakiye = (bnk.tcr_musteriler+bnk.girisYapilanID)->hesap[i].bakiye;
			(bnk.aktif_musteri.hesap+i)->No = (bnk.tcr_musteriler+bnk.girisYapilanID)->hesap[i].No;
		}
		
		
		for(i=0;i<(bnk.tcr_musteriler+bnk.girisYapilanID)->islemsayisi;i++){
			(bnk.aktif_musteri.islem+i)->islemYapilanHesapNo = (bnk.tcr_musteriler+bnk.girisYapilanID)->islem[i].islemYapilanHesapNo;
			(bnk.aktif_musteri.islem+i)->tutar = (bnk.tcr_musteriler+bnk.girisYapilanID)->islem[i].tutar;
			(bnk.aktif_musteri.islem+i)->gun = (bnk.tcr_musteriler+bnk.girisYapilanID)->islem[i].gun;
			(bnk.aktif_musteri.islem+i)->ay = (bnk.tcr_musteriler+bnk.girisYapilanID)->islem[i].ay;
			(bnk.aktif_musteri.islem+i)->yil = (bnk.tcr_musteriler+bnk.girisYapilanID)->islem[i].yil;
			(bnk.aktif_musteri.islem+i)->saat = (bnk.tcr_musteriler+bnk.girisYapilanID)->islem[i].saat;
			(bnk.aktif_musteri.islem+i)->dakika = (bnk.tcr_musteriler+bnk.girisYapilanID)->islem[i].dakika;
			(bnk.aktif_musteri.islem+i)->havaleYapilanHesapNo = (bnk.tcr_musteriler+bnk.girisYapilanID)->islem[i].havaleYapilanHesapNo;
	
		}	
	}
	return;
}


int GunlukLimitSorgula(float cekilecekMiktar)
{
	int i;
	int gunlukLimit;
	float gunlukCekilenMiktar=0;
	if(!bnk.girisYapilanHesapTipi) // bireyselse
		gunlukLimit = 750;
	else
		gunlukLimit = 1500;

	for(i=0;i<bnk.aktif_musteri.islemsayisi;i++)
	{
		if((bnk.aktif_musteri.islem+i)->ay == ay && (bnk.aktif_musteri.islem+i)->gun == gun && (bnk.aktif_musteri.islem+i)->tutar<0)
		{
			gunlukCekilenMiktar+= (-(bnk.aktif_musteri.islem+i)->tutar);
		}
	}
	
	if(gunlukCekilenMiktar + cekilecekMiktar <= gunlukLimit)
		return 1;
	else
		return 0;
	
}

void GirisYapilanHesabiGuncelle()
{
	int i,j;
	
	if(bnk.girisYapilanHesapTipi == 0 || bnk.girisYapilanHesapTipi == 1) // Eger -1 se henuz kimse giris yapmam�st�r
	{
		if(bnk.girisYapilanHesapTipi == 0) // B�REYSEL �SE
		{
			(bnk.bry_musteriler+bnk.girisYapilanID)->No = bnk.aktif_musteri.No ;
			bnk.girisYapilanID =  bnk.aktif_musteri.id ;
			(bnk.bry_musteriler+bnk.girisYapilanID)->hesapsayisi = bnk.aktif_musteri.hesapsayisi;
			(bnk.bry_musteriler+bnk.girisYapilanID)->islemsayisi = bnk.aktif_musteri.islemsayisi;
			strcpy((bnk.bry_musteriler+bnk.girisYapilanID)->adSoyad,bnk.aktif_musteri.adSoyad);
			strcpy((bnk.bry_musteriler+bnk.girisYapilanID)->TC,bnk.aktif_musteri.TC);
			strcpy((bnk.bry_musteriler+bnk.girisYapilanID)->sifre,bnk.aktif_musteri.sifre);
			strcpy((bnk.bry_musteriler+bnk.girisYapilanID)->tip,bnk.aktif_musteri.tip);
			
		for(i=0;i<bnk.aktif_musteri.hesapsayisi;i++) // hesap verilerini g�ncelle	
		{
			(bnk.bry_musteriler+bnk.girisYapilanID)->hesap[i].id = (bnk.aktif_musteri.hesap+i)->id ;
			(bnk.bry_musteriler+bnk.girisYapilanID)->hesap[i].bakiye = (bnk.aktif_musteri.hesap+i)->bakiye;
			(bnk.bry_musteriler+bnk.girisYapilanID)->hesap[i].No = (bnk.aktif_musteri.hesap+i)->No ;
		}
		
		for(i=0;i<(bnk.bry_musteriler+bnk.girisYapilanID)->islemsayisi;i++){
		(bnk.bry_musteriler+bnk.girisYapilanID)->islem[i].islemYapilanHesapNo = (bnk.aktif_musteri.islem+i)->islemYapilanHesapNo;
	    (bnk.bry_musteriler+bnk.girisYapilanID)->islem[i].tutar = (bnk.aktif_musteri.islem+i)->tutar;
	    (bnk.bry_musteriler+bnk.girisYapilanID)->islem[i].gun = (bnk.aktif_musteri.islem+i)->gun;
	 	(bnk.bry_musteriler+bnk.girisYapilanID)->islem[i].ay = (bnk.aktif_musteri.islem+i)->ay;
	    (bnk.bry_musteriler+bnk.girisYapilanID)->islem[i].yil = (bnk.aktif_musteri.islem+i)->yil;
	    (bnk.bry_musteriler+bnk.girisYapilanID)->islem[i].saat = (bnk.aktif_musteri.islem+i)->saat;
	    (bnk.bry_musteriler+bnk.girisYapilanID)->islem[i].dakika = (bnk.aktif_musteri.islem+i)->dakika;
	    (bnk.bry_musteriler+bnk.girisYapilanID)->islem[i].havaleYapilanHesapNo = (bnk.aktif_musteri.islem+i)->havaleYapilanHesapNo;
		}
				
			
		}
		else  //T�CAR�
			{
			(bnk.tcr_musteriler+bnk.girisYapilanID)->No = bnk.aktif_musteri.No ;
			bnk.girisYapilanID =  bnk.aktif_musteri.id ;
			(bnk.tcr_musteriler+bnk.girisYapilanID)->hesapsayisi = bnk.aktif_musteri.hesapsayisi;
			(bnk.tcr_musteriler+bnk.girisYapilanID)->islemsayisi = bnk.aktif_musteri.islemsayisi;
			strcpy((bnk.tcr_musteriler+bnk.girisYapilanID)->adSoyad,bnk.aktif_musteri.adSoyad);
			strcpy((bnk.tcr_musteriler+bnk.girisYapilanID)->TC,bnk.aktif_musteri.TC);
			strcpy((bnk.tcr_musteriler+bnk.girisYapilanID)->sifre,bnk.aktif_musteri.sifre);
			strcpy((bnk.tcr_musteriler+bnk.girisYapilanID)->tip,bnk.aktif_musteri.tip);
			
			
		 for(i=0;i<bnk.aktif_musteri.hesapsayisi;i++) // hesap verilerini g�ncelle	
		 {
			(bnk.tcr_musteriler+bnk.girisYapilanID)->hesap[i].id = (bnk.aktif_musteri.hesap+i)->id ;
			(bnk.tcr_musteriler+bnk.girisYapilanID)->hesap[i].bakiye = (bnk.aktif_musteri.hesap+i)->bakiye;
			(bnk.tcr_musteriler+bnk.girisYapilanID)->hesap[i].No = (bnk.aktif_musteri.hesap+i)->No ;
		}
		
		for(i=0;i<(bnk.tcr_musteriler+bnk.girisYapilanID)->islemsayisi;i++){
			(bnk.tcr_musteriler+bnk.girisYapilanID)->islem[i].islemYapilanHesapNo = (bnk.aktif_musteri.islem+i)->islemYapilanHesapNo;
		    (bnk.tcr_musteriler+bnk.girisYapilanID)->islem[i].tutar = (bnk.aktif_musteri.islem+i)->tutar;
		    (bnk.tcr_musteriler+bnk.girisYapilanID)->islem[i].gun = (bnk.aktif_musteri.islem+i)->gun;
		 	(bnk.tcr_musteriler+bnk.girisYapilanID)->islem[i].ay = (bnk.aktif_musteri.islem+i)->ay;
		    (bnk.tcr_musteriler+bnk.girisYapilanID)->islem[i].yil = (bnk.aktif_musteri.islem+i)->yil;
		    (bnk.tcr_musteriler+bnk.girisYapilanID)->islem[i].saat = (bnk.aktif_musteri.islem+i)->saat;
		    (bnk.tcr_musteriler+bnk.girisYapilanID)->islem[i].dakika = (bnk.aktif_musteri.islem+i)->dakika;
		    (bnk.tcr_musteriler+bnk.girisYapilanID)->islem[i].havaleYapilanHesapNo = (bnk.aktif_musteri.islem+i)->havaleYapilanHesapNo;
		}
			
		}
	}
	
	GirisYapilanHesabiProgramaCek();
	return;
}

void BankaVerileriniGuncelle()
{
	bnk.hazine = bnk.gelir>=bnk.gider ? bnk.gelir - bnk.gider : bnk.gider-bnk.gelir;
	FILE *fp;
	fp = fopen("banka.txt","w");
	fprintf(fp,"Bireysel Musteri Sayisi: %d\nTicari Musteri Sayisi: %d",bnk.bireyselMS,bnk.ticariMS);
	fprintf(fp,"\nBanka Hazinesi: %.2f TL Gelir: %.2f Gider: %.2f",bnk.hazine,bnk.gelir,bnk.gider);
	fclose(fp);	
	BankaVerileriniCek();
	return;
}

void BankaVerileriniCek()
{
	FILE *fp;
	fp = fopen("banka.txt","r");
	if(fp == NULL){
			//Daha once hic bu dosya a��lmam��
			BankaVerileriniGuncelle();
		}
	fscanf(fp,"Bireysel Musteri Sayisi: %d\nTicari Musteri Sayisi: %d",&bnk.bireyselMS,&bnk.ticariMS);
	fscanf(fp,"\nBanka Hazinesi: %f TL Gelir: %f Gider: %f",&bnk.hazine,&bnk.gelir,&bnk.gider);
	fclose(fp);
	return;
}

void MusteriVerileriniCek()
{
	int i,j;
	FILE *bireyselF,*ticariF;
	bireyselF = fopen("bireysel_musteriler.txt","r");
	ticariF = fopen("ticari_musteriler.txt","r");
	
	if(bireyselF == NULL){
			//Daha once hic bu dosya a��lmam�� o y�zden olu�tur dosyay�
			bireyselF = fopen("bireysel_musteriler.txt","w");
			fclose(bireyselF);
			MusteriVerileriniCek();
		}
	if(ticariF == NULL){
			//Daha once hic bu dosya a��llmamis o y�zden olu�tur dosyay�
			ticariF = fopen("ticari_musteriler.txt","w");
			fclose(ticariF);
			MusteriVerileriniCek();
		}

	//Bireysel M��terileri Programa �ek
	for(i=0;i<bnk.bireyselMS;i++){
		fscanf(bireyselF,"%[^\n]\n\t\tID: %d\n\t\tNO: %d\n\t\tTC: %s\n\t\tSifre: %s\n\t\tTip: %s\n\t\tHesap Sayisi: %d",&(bnk.bry_musteriler+i)->adSoyad,&(bnk.bry_musteriler+i)->id,&(bnk.bry_musteriler+i)->No,&(bnk.bry_musteriler+i)->TC,&(bnk.bry_musteriler+i)->sifre,&(bnk.bry_musteriler+i)->tip,&(bnk.bry_musteriler+i)->hesapsayisi);
		fscanf(bireyselF,"\n\t\tIslem Sayisi: %d\n",&(bnk.bry_musteriler+i)->islemsayisi);

		for(j=0;j<(bnk.bry_musteriler+i)->islemsayisi;j++){
			fscanf(bireyselF,"\n\t\t\t%d %d %d %d %d %f %d %d",&(bnk.bry_musteriler+i)->islem[j].gun,&(bnk.bry_musteriler+i)->islem[j].ay,&(bnk.bry_musteriler+i)->islem[j].yil,&(bnk.bry_musteriler+i)->islem[j].saat,&(bnk.bry_musteriler+i)->islem[j].dakika,&(bnk.bry_musteriler+i)->islem[j].tutar,&(bnk.bry_musteriler+i)->islem[j].islemYapilanHesapNo,&(bnk.bry_musteriler+i)->islem[j].havaleYapilanHesapNo);
		}
		
		for(j=0;j<(bnk.bry_musteriler+i)->hesapsayisi;j++){
			fscanf(bireyselF,"\n\t\t\tHesap ID: %d",&(bnk.bry_musteriler+i)->hesap[j].id);
			fscanf(bireyselF,"\n\t\t\tHesap No: %d",&(bnk.bry_musteriler+i)->hesap[j].No);
			fscanf(bireyselF,"\n\t\t\tHesap Bakiyesi: %f\n",&(bnk.bry_musteriler+i)->hesap[j].bakiye);
		}
	}
	
	for(i=0;i<bnk.ticariMS;i++){
		fscanf(ticariF,"%[^\n]\n\t\tID: %d\n\t\tNO: %d\n\t\tTC: %s\n\t\tSifre: %s\n\t\tTip: %s\n\t\tHesap Sayisi: %d",&(bnk.tcr_musteriler+i)->adSoyad,&(bnk.tcr_musteriler+i)->id,&(bnk.tcr_musteriler+i)->No,&(bnk.tcr_musteriler+i)->TC,&(bnk.tcr_musteriler+i)->sifre,&(bnk.tcr_musteriler+i)->tip,&(bnk.tcr_musteriler+i)->hesapsayisi);
		fscanf(ticariF,"\n\t\tIslem Sayisi: %d\n",&(bnk.tcr_musteriler+i)->islemsayisi);
		
		for(j=0;j<(bnk.tcr_musteriler+i)->islemsayisi;j++){
			fscanf(ticariF,"\n\t\t\t%d %d %d %d %d %f %d %d",&(bnk.tcr_musteriler+i)->islem[j].gun,&(bnk.tcr_musteriler+i)->islem[j].ay,&(bnk.tcr_musteriler+i)->islem[j].yil,&(bnk.tcr_musteriler+i)->islem[j].saat,&(bnk.tcr_musteriler+i)->islem[j].dakika,&(bnk.tcr_musteriler+i)->islem[j].tutar,&(bnk.tcr_musteriler+i)->islem[j].islemYapilanHesapNo,&(bnk.tcr_musteriler+i)->islem[j].havaleYapilanHesapNo);
		}
		
		for(j=0;j<(bnk.tcr_musteriler+i)->hesapsayisi;j++){
			fscanf(ticariF,"\n\t\t\tHesap ID: %d",&(bnk.tcr_musteriler+i)->hesap[j].id);
			fscanf(ticariF,"\n\t\t\tHesap No: %d",&(bnk.tcr_musteriler+i)->hesap[j].No);
			fscanf(ticariF,"\n\t\t\tHesap Bakiyesi: %f\n",&(bnk.tcr_musteriler+i)->hesap[j].bakiye);
		}
	}
	
	fclose(ticariF);
	fclose(bireyselF);
	return;
}

void MusteriVerileriniGuncelle()
{
	GirisYapilanHesabiGuncelle();
	FILE *bireyselF,*ticariF;
	bireyselF = fopen("bireysel_musteriler.txt","w");
	ticariF = fopen("ticari_musteriler.txt","w");
	int i,j;
	for(i=0;i<bnk.bireyselMS;i++){
		fprintf(bireyselF,"%s\n\t\tID: %d\n\t\tNO: %d\n\t\tTC: %s\n\t\tSifre: %s\n\t\tTip: %s\n\t\tHesap Sayisi: %d",(bnk.bry_musteriler+i)->adSoyad,i,(bnk.bry_musteriler+i)->No,(bnk.bry_musteriler+i)->TC,(bnk.bry_musteriler+i)->sifre,(bnk.bry_musteriler+i)->tip,(bnk.bry_musteriler+i)->hesapsayisi);
		fprintf(bireyselF,"\n\t\tIslem Sayisi: %d\n",(bnk.bry_musteriler+i)->islemsayisi);
		
		for(j=0;j<(bnk.bry_musteriler+i)->islemsayisi;j++){
			fprintf(bireyselF,"\n\t\t\t%d %d %d %d %d %f %d %d",(bnk.bry_musteriler+i)->islem[j].gun,(bnk.bry_musteriler+i)->islem[j].ay,(bnk.bry_musteriler+i)->islem[j].yil,(bnk.bry_musteriler+i)->islem[j].saat,(bnk.bry_musteriler+i)->islem[j].dakika,(bnk.bry_musteriler+i)->islem[j].tutar,(bnk.bry_musteriler+i)->islem[j].islemYapilanHesapNo,(bnk.bry_musteriler+i)->islem[j].havaleYapilanHesapNo);
		}
		
		for(j=0;j<(bnk.bry_musteriler+i)->hesapsayisi;j++){	
			fprintf(bireyselF,"\n\t\t\tHesap ID: %d",(bnk.bry_musteriler+i)->hesap[j].id);
			fprintf(bireyselF,"\n\t\t\tHesap No: %d",(bnk.bry_musteriler+i)->hesap[j].No);
			fprintf(bireyselF,"\n\t\t\tHesap Bakiyesi: %.2f\n",(bnk.bry_musteriler+i)->hesap[j].bakiye);
		}
	}
	
	//Ticari Musterileri Guncelle
	for(i=0;i<bnk.ticariMS;i++){
		
		fprintf(ticariF,"%s\n\t\tID: %d\n\t\tNO: %d\n\t\tTC: %s\n\t\tSifre: %s\n\t\tTip: %s\n\t\tHesap Sayisi: %d",(bnk.tcr_musteriler+i)->adSoyad,i,(bnk.tcr_musteriler+i)->No,(bnk.tcr_musteriler+i)->TC,(bnk.tcr_musteriler+i)->sifre,(bnk.tcr_musteriler+i)->tip,(bnk.tcr_musteriler+i)->hesapsayisi);
		fprintf(ticariF,"\n\t\tIslem Sayisi: %d\n",(bnk.tcr_musteriler+i)->islemsayisi);
		
		for(j=0;j<(bnk.tcr_musteriler+i)->islemsayisi;j++){
			fprintf(ticariF,"\n\t\t\t%d %d %d %d %d %f %d %d",(bnk.tcr_musteriler+i)->islem[j].gun,(bnk.tcr_musteriler+i)->islem[j].ay,(bnk.tcr_musteriler+i)->islem[j].yil,(bnk.tcr_musteriler+i)->islem[j].saat,(bnk.tcr_musteriler+i)->islem[j].dakika,(bnk.tcr_musteriler+i)->islem[j].tutar,(bnk.tcr_musteriler+i)->islem[j].islemYapilanHesapNo,(bnk.tcr_musteriler+i)->islem[j].havaleYapilanHesapNo);
		}
		
		for(j=0;j<(bnk.tcr_musteriler+i)->hesapsayisi;j++){
			fprintf(ticariF,"\n\t\t\tHesap ID: %d",(bnk.tcr_musteriler+i)->hesap[j].id);
			fprintf(ticariF,"\n\t\t\tHesap No: %d",(bnk.tcr_musteriler+i)->hesap[j].No);
			fprintf(ticariF,"\n\t\t\tHesap Bakiyesi: %.2f\n",(bnk.tcr_musteriler+i)->hesap[j].bakiye);
		}
	}
	
	fclose(bireyselF);
	fclose(ticariF);
	MusteriVerileriniCek();	
	return;
}

int RastgeleSayiOlustur(int tip) // Hesap Numarasi Mi yoksa Musteri Numarasi Mi ? 0 Musteri No , 1 Hesap No
{
	int a,i,j;
	srand(time(NULL));
	if(tip == 0) // Musteri Numarasi Olustur
	{
		a = ((rand()%9+1) * 1000) + rand()%1000;	
		for(i=0;i<bnk.bireyselMS;i++){
			if((bnk.bry_musteriler+i)->No == a)
				RastgeleSayiOlustur(0);
				
		}
		for(i=0;i<bnk.ticariMS;i++){
			if((bnk.tcr_musteriler+i)->No == a)
				RastgeleSayiOlustur(0);
		}
		
		
		return a; // bu d�ng�lerden d�zg�n ��karsa kusursuz bir numara �retti demek
	}
	
	else
	// Hesap Numarasi Olustur
	{
		a = ((rand()%9+1) * 10000) + rand()%1000;	
		for(i=0;i<bnk.bireyselMS;i++){
			for(j=0;j<(bnk.bry_musteriler+i)->hesapsayisi;j++){
				if((bnk.bry_musteriler+i)->hesap[j].No == a)
					RastgeleSayiOlustur(1);	
			}
					
		}
		
		for(i=0;i<bnk.ticariMS;i++){
			for(j=0;j<(bnk.tcr_musteriler+i)->hesapsayisi;j++){
				if((bnk.tcr_musteriler+i)->hesap[j].No == a)
					RastgeleSayiOlustur(1);	
			}
					
		}
		
		return a; // bu dongulerden sorunsuz cikarsa kusursuz bir numara uretti demek
	}
}
