#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <locale.h>

/* 
-Yeni M��teri Ekleme
    -M��teri No (Special)
    -Bireysel / Ticari (Farkl� Dosyalar)

-Hesap A�t�rma
    -M��teri istedi�i kadar hesap a�ar
    -Hesap numaralar� e�siz

-Para �ekme
    -Bireysel max g�nl�k para �ekme: 750, ticari: 1500
    -Hesab�nda yoksa ek hesap

-Para Yat�rma

-Hesaba Havale
    -Havale yapacag� ki�inin hesap no
    -Ticari m��telerilerden havale �creti kesilmez, bireysel m��teriler %2 kesilir.

-Banka Gelir & Gider Raporu
    -bankadan giden, gelen ve bankada bulunan toplam para miktar� vb
    -Kullan�c� sistem �zerinden rapor al k�sm�n� se�erse, rapor isimli dosyadan okuma yap�l�p ekranda g�sterilecektir.

-Hesap �zeti
    -Belirtilen tarih aral���nda
    -�ekilen, yat�r�lan, havale yap�lm��sa kime yap�ld��� ve miktar�, ba�ka bir hesaptan havale para geldiyse kimden geldi�i ve miktar� gibi 
    bilgiler ve bu i�lemlerin tarihleri g�r�nt�lenmelidir. Ve dekont.txt isimli dosyaya yaz�lacakt�r.

-Hesap Kapama
    -Bakiye 0 olmas� gerekli, kapat�ld���nda m��teri bilgileri de silinecek!

	
	*/
	
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
//
	

	
	
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
		int id,hesapsayisi;
		struct Hesap hesap[100];
		
		
	};
	
	struct bireyselMusteri
	{
		char TC[15],sifre[20],adSoyad[40],tip[10];
		int No;
		int id,hesapsayisi;
		struct Hesap hesap[100];
		
		
	};
	
	struct ticariMusteri
	{
		char TC[15],sifre[20],adSoyad[40],tip[10];
		int No;
		int id,hesapsayisi;
		struct Hesap hesap[100];
		
		
	};

	
	struct Banka
	{
		struct AktifMusteri aktif_musteri;
		struct bireyselMusteri bry_musteriler[100];
		struct ticariMusteri tcr_musteriler[100];
		float hazine;
		int girisYapilanID; // -1 Giris Yapilmadi
		int girisYapilanHesapTipi; // 0 B�REYSEL 1 T�CAR� 
		int bireyselMS,ticariMS; //  ms = musteri sayisi		
	}bnk; 
	

	
	
	
int main()
{
	bnk.girisYapilanHesapTipi = -1;
	bnk.girisYapilanID = -1;
	setlocale(LC_ALL, "Turkish"); 
	BankaVerileriniCek();
	MusteriVerileriniCek();
	AnaMenu();

	return 0;
}


void AnaMenu()
{
	
	char secim[20];
	system("cls");
	printf("\n\n\n------------ Ana Menu ------------\n");
	printf("<!> 1. Yeni M��teri Kayd�\n");
	printf("<!> 2. M��teri Giri�i\n");
	printf("<!> 3. Banka Gelir - Gider Raporu\n");
	printf("<!> Secim Yapiniz: ");
	
	scanf("%s",&secim);	
	
	if(strcmp(secim,"1") == 0)
	{
		YeniMusteri();
	}
	else if(strcmp(secim,"2") == 0)
		MusteriGirisi();
	else if(strcmp(secim,"3") == 0);
		//AnaMenu();
		
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
	printf("\n\n\n------------ Yeni M��teri Kayd� ------------\n");
	printf("\n<!> M��teri tipinizi se�iniz:\n\t1. Bireysel\n\t2. Ticari");
	printf("\nSeçiminiz: ");
	scanf("%s",&secim);	
	
	//Hesap Tipini Se�iniz
	if(strcmp(secim,"1") == 0)
			musteriTip=0;
	else if(strcmp(secim,"2") == 0)
		 	musteriTip=1;
	else{
		printf("Hatal� bir se�im yapt�n�z!");
		Sleep(1500);
		YeniMusteri();
		return;
		}
		
			
	if(musteriTip == 0) // B�REYSEL MUSTER� KAYDI YAP
	{
		
		printf("<!>Ad Soyad:");
		fflush(stdin);
		gets(bnk.bry_musteriler[bnk.bireyselMS].adSoyad); // TODO: NUMARA G�RMEY� ENGELLE
		printf("\n<!>TC:");
		gets(bnk.bry_musteriler[bnk.bireyselMS].TC);
	
		if(KayitSorgula(0) == -1)
			{
				printf("\n<!> B�yle bir m��teri zaten var.");
				Sleep(1500);
				YeniMusteri();
				return;
			}
	
	
		if(strlen(bnk.bry_musteriler[bnk.bireyselMS].TC)!= 11 || isdigit(*bnk.bry_musteriler[bnk.bireyselMS].TC) == 0)
		{
			printf("Hatal� bir kimlik numaras� girdiniz.");
			Sleep(1500);
			YeniMusteri();
			return;	
		}
		fflush(stdin);
	

		printf("\n<!>�ifre:");
		gets(bnk.bry_musteriler[bnk.bireyselMS].sifre);
		strcpy(bnk.bry_musteriler[bnk.bireyselMS].tip,"Bireysel");
		bnk.bry_musteriler[bnk.bireyselMS].No = RastgeleSayiOlustur(0);
		bnk.bireyselMS++;
	
	}//Bireysel musteri kayit son
	else{
	//Ticari Musteri Kaydi yap
				
		printf("<!>Ad Soyad:");
		fflush(stdin);
		gets(bnk.tcr_musteriler[bnk.ticariMS].adSoyad);
		printf("\n<!>TC:");
		gets(bnk.tcr_musteriler[bnk.ticariMS].TC);
		if(KayitSorgula(1) == -1)
			{
				printf("\n<!> B�yle bir m��teri zaten var.");
				Sleep(1500);
				YeniMusteri();
				return;
			}


	
		if(strlen(bnk.tcr_musteriler[bnk.ticariMS].TC)!= 11 || isdigit(*bnk.tcr_musteriler[bnk.ticariMS].TC) == 0)
		{
			printf("Hatal� bir kimlik numaras� girdiniz.");
			Sleep(1000);
			YeniMusteri();
			return;	
		}
		fflush(stdin);
		printf("\n<!>�ifre:");
		gets(bnk.tcr_musteriler[bnk.ticariMS].sifre);
		strcpy(bnk.tcr_musteriler[bnk.ticariMS].tip,"Ticari");
	
		bnk.tcr_musteriler[bnk.ticariMS].No = RastgeleSayiOlustur(0);
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
	
	if(tip == 0)
	{
		
		for(i=0;i<bnk.bireyselMS;i++)
	{
		if(strcmp(bnk.bry_musteriler[i].TC,bnk.bry_musteriler[bnk.bireyselMS].TC) == 0)	
			return -1;
	}
	
		return 0;
	}
	else
	{
		
		for(i=0;i<bnk.ticariMS;i++)
	{
		if(strcmp(bnk.tcr_musteriler[i].TC,bnk.tcr_musteriler[bnk.bireyselMS].TC) == 0)	
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
	printf("\n\n\n------------ M��teri Giri�i ------------\n");
	printf("<!> TC Kimlik Numaras�:");
	fflush(stdin);
	gets(TC);
	printf("\n<!> �ifre:");
	gets(sifre);
	printf("\n<!> Musteri tipiniz (1 = Bireysel, 2 = Ticari): ");
	gets(tip);
	
	if(strcmp(tip,"1") != 0 && strcmp(tip,"2") != 0 )
		{
		printf("<!> Hatal� bir se�im yapt�n�z!");
		Sleep(1500);
		AnaMenu();
		return;
		}
			
	
	if(MusteriGirisSorgula(TC,sifre,tip) == -1) //-1 d�nd�yse sorgulama tamam demektir
	{
		GirisYapilanHesabiProgramaCek();
		printf("\n<!> Giri� Ba�ar�l�!!\n\n");
		Sleep(1000);
		MusteriMenu();
	}
	else
	{
		printf("\n<!> Giri� Ba�ar�s�z!");
		Sleep(1000);
		MusteriGirisi();
		return;
	}
	
	
	
}

int MusteriGirisSorgula(char TC[20],char sifre[25],char tip[12])
{
	MusteriVerileriniCek();
	int i;
	if(strcmp(tip,"1") == 0) // tip 1 se bireysel sorgula
	{
		
		for(i=0;i<bnk.bireyselMS;i++)
		{
			if(strcmp(bnk.bry_musteriler[i].TC,TC) == 0 && strcmp(bnk.bry_musteriler[i].sifre,sifre) == 0 )
			{
				bnk.girisYapilanID = i;
				bnk.girisYapilanHesapTipi = 0; // 0 B�REYSEL // 1 T�CARI
				return -1; // Giris basarili
					
			}
		}
		
	}
	else//ticari
	{
		
			for(i=0;i<bnk.ticariMS;i++)
		{
			if(strcmp(bnk.tcr_musteriler[i].TC,TC) == 0 && strcmp(bnk.tcr_musteriler[i].sifre,sifre) == 0 )
			{
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
	printf("\n\n\n------------ Musteri Menu ------------\n");
	printf("<> Ad Soyad: %s\n<> M��teri No: %d\n<> TC: %s\n<> M��teri Tipi: %s\n<> Hesap Say�s�: %d\n\n",bnk.aktif_musteri.adSoyad,bnk.aktif_musteri.No,bnk.aktif_musteri.TC,bnk.aktif_musteri.tip,bnk.aktif_musteri.hesapsayisi);
			
	printf("<!> 1. Hesap A�ma\n");
	printf("<!> 2. Hesaplar�m\n");
	printf("<!> 3. Hesab�ma Para Yat�r\n");
	printf("<!> 4. Banka Gelir - Gider Raporu\n");
	printf("<!> 5. ��k�� Yap\n");
	printf("<!> Se�im Yap�n�z: ");
	scanf("%s",&secim);
	
	
	if(strcmp(secim,"1") == 0)
		HesapAc();
	else if(strcmp(secim,"2") == 0)
		HesaplarimMenu();
	else if(strcmp(secim,"3") == 0)
		HesabaParaYatir();
		
	else if(strcmp(secim,"5") == 0)
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
	printf("\n\n\n------------ Hesap Acma ------------\n");
	printf("<> Ad Soyad: %s\n<> Musteri No: %d\n<> TC: %s\n<> Musteri Tipi: %s\n<> Hesap Sayisi: %d\n\n",bnk.aktif_musteri.adSoyad,bnk.aktif_musteri.No,bnk.aktif_musteri.TC,bnk.aktif_musteri.tip,bnk.aktif_musteri.hesapsayisi);
	bnk.aktif_musteri.hesap[bnk.aktif_musteri.hesapsayisi].No = RastgeleSayiOlustur(1);
	bnk.aktif_musteri.hesap[bnk.aktif_musteri.hesapsayisi].id = bnk.aktif_musteri.hesapsayisi;
	printf("<!> 1. Hesap Numaraniz: %d\n",bnk.aktif_musteri.hesap[bnk.aktif_musteri.hesapsayisi].No);
	printf("<!> Devam etmek ve hesab�n�z� a�mak i�in bir tu�a bas�n (0 = Islemi iptal et ve men�ye d�n) ...");
	secim = getch();
	if(secim == '0')
		MusteriMenu();
		
	bnk.aktif_musteri.hesapsayisi++; // Hesap Basariyla Acildi
	
	MusteriVerileriniGuncelle();
	printf("\n<!> Hesab�n�z ba�ar�yla a��ld�!");
	Sleep(1500);
	MusteriMenu();
	
}


void HesaplarimMenu()
{
	int i,j;
	char secim;
	system("cls");
	printf("\n\n\n<------------ Hesaplar�m ------------>\n");
	printf("<> Ad Soyad: %s\n<> M��teri No: %d\n<> TC: %s\n<> M��teri Tipi: %s\n<> Hesap Sayisi: %d\n\n",bnk.aktif_musteri.adSoyad,bnk.aktif_musteri.No,bnk.aktif_musteri.TC,bnk.aktif_musteri.tip,bnk.aktif_musteri.hesapsayisi);

	for(i=0;i<bnk.aktif_musteri.hesapsayisi;i++)
	{
		printf("\t----------------\n");
		printf("\t<!> Hesap ID: %d\n",bnk.aktif_musteri.hesap[i].id);
		printf("\t<!> Hesap No: %d\n",bnk.aktif_musteri.hesap[i].No);
		printf("\t<!> Hesap Bakiyesi: %.2f\n",bnk.aktif_musteri.hesap[i].bakiye);
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
	printf("\n\n\n<------------ Hesaplar�m ------------>\n");
	for(i=0;i<bnk.aktif_musteri.hesapsayisi;i++)
	{
		printf("\t----------------\n");
		printf("\t<!> Hesap ID: %d\n",bnk.aktif_musteri.hesap[i].id);
		printf("\t<!> Hesap No: %d\n",bnk.aktif_musteri.hesap[i].No);
		printf("\t<!> Hesap Bakiyesi: %.2f\n",bnk.aktif_musteri.hesap[i].bakiye);
		printf("\t----------------\n\n");
		
	}
	
	printf("\n\n\t<!> Para yat�rmak istedi�iniz hesap ID sini giriniz:");
	scanf("%s",secim);
	
	if(isdigit(*secim) == 0)
	{
		printf("\t<!> Hatal� bir se�im yapt�n�z.");
		Sleep(1500);
		HesabaParaYatir();
		return;
	}
	
	secilenID = atoi(secim);
	
	if(secilenID>=bnk.aktif_musteri.hesapsayisi || secilenID<0)
	{
		printf("\t<!> Ge�ersiz bir ID numaras� girdiniz.");
		Sleep(1500);
		HesabaParaYatir();
		return;
	}
	
	printf("\t<!> Hesap Numaraniz: %d, devam etmek i�in bir tu�a bas�n�z... (0 = I�lemi �ptal Et ve Men�ye D�n)",bnk.aktif_musteri.hesap[secilenID].No);
	
	*secim = getch();
	
	if(secim == "0")
		{
			MusteriMenu();
			return;
		}
		

	printf("\n\t<!> Yat�rmak istedi�iniz miktar� yaz�n�z: ");
	scanf("%s",&para);
	
	
	if(isdigit(*para) == 0)
	{
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

	// Paray� Hesaba Ekle
	bnk.aktif_musteri.hesap[secilenID].bakiye+=yatirilacakMiktar;
	bnk.hazine += yatirilacakMiktar;
	MusteriVerileriniGuncelle();
	//Paray� Hesaba Ekleme Son
	
	printf("\t<!> %.2f TL hesab�n�za yat�r�ld�. Yeni bakiyeniz: %.2f. \n\t<!>Ana men�ye d�nmek i�in bir tu�a bas�n�z...\n\n\n\n",yatirilacakMiktar,bnk.aktif_musteri.hesap[secilenID].bakiye);
	*secim = getch();
	MusteriMenu();
	return;


}













void GirisYapilanHesabiProgramaCek()
{
	int i,j;
	
	
	if(bnk.girisYapilanHesapTipi == 0) // B�REYSEL �SE
	{
		bnk.aktif_musteri.No = bnk.bry_musteriler[bnk.girisYapilanID].No;
		bnk.aktif_musteri.id = bnk.girisYapilanID;
		bnk.aktif_musteri.hesapsayisi  = bnk.bry_musteriler[bnk.girisYapilanID].hesapsayisi;
		strcpy(bnk.aktif_musteri.adSoyad,bnk.bry_musteriler[bnk.girisYapilanID].adSoyad);
		strcpy(bnk.aktif_musteri.TC,bnk.bry_musteriler[bnk.girisYapilanID].TC);
		strcpy(bnk.aktif_musteri.sifre,bnk.bry_musteriler[bnk.girisYapilanID].sifre);
		strcpy(bnk.aktif_musteri.tip,bnk.bry_musteriler[bnk.girisYapilanID].tip);
		
		for(i=0;i<bnk.bry_musteriler[bnk.girisYapilanID].hesapsayisi;i++) // hesap verilerini �ek	
		{
			bnk.aktif_musteri.hesap[i].id = bnk.bry_musteriler[bnk.girisYapilanID].hesap[i].id;
			bnk.aktif_musteri.hesap[i].bakiye = bnk.bry_musteriler[bnk.girisYapilanID].hesap[i].bakiye;
			bnk.aktif_musteri.hesap[i].No = bnk.bry_musteriler[bnk.girisYapilanID].hesap[i].No;
		}
		
	}
	else  //T�CAR�
		{
		bnk.aktif_musteri.No = bnk.tcr_musteriler[bnk.girisYapilanID].No;
		bnk.aktif_musteri.id = bnk.girisYapilanID;
		bnk.aktif_musteri.hesapsayisi  = bnk.tcr_musteriler[bnk.girisYapilanID].hesapsayisi;
		strcpy(bnk.aktif_musteri.adSoyad,bnk.tcr_musteriler[bnk.girisYapilanID].adSoyad);
		strcpy(bnk.aktif_musteri.TC,bnk.tcr_musteriler[bnk.girisYapilanID].TC);
		strcpy(bnk.aktif_musteri.sifre,bnk.tcr_musteriler[bnk.girisYapilanID].sifre);
		strcpy(bnk.aktif_musteri.tip,bnk.tcr_musteriler[bnk.girisYapilanID].tip);
		
		
		
		for(i=0;i<bnk.tcr_musteriler[bnk.girisYapilanID].hesapsayisi;i++) // hesap verilerini �ek	
		{
			bnk.aktif_musteri.hesap[i].id = bnk.tcr_musteriler[bnk.girisYapilanID].hesap[i].id;
			bnk.aktif_musteri.hesap[i].bakiye = bnk.tcr_musteriler[bnk.girisYapilanID].hesap[i].bakiye;
			bnk.aktif_musteri.hesap[i].No = bnk.tcr_musteriler[bnk.girisYapilanID].hesap[i].No;
		}
		
	}
}


void GirisYapilanHesabiGuncelle()
{
	
	int i,j;
	
	
	if(bnk.girisYapilanHesapTipi == 0 || bnk.girisYapilanHesapTipi == 1) // Eger -1 se henuz kimse giris yapmam�st�r
	{
		if(bnk.girisYapilanHesapTipi == 0) // B�REYSEL �SE
		{
			bnk.bry_musteriler[bnk.girisYapilanID].No = bnk.aktif_musteri.No ;
			bnk.girisYapilanID =  bnk.aktif_musteri.id ;
			bnk.bry_musteriler[bnk.girisYapilanID].hesapsayisi = bnk.aktif_musteri.hesapsayisi;
			strcpy(bnk.bry_musteriler[bnk.girisYapilanID].adSoyad,bnk.aktif_musteri.adSoyad);
			strcpy(bnk.bry_musteriler[bnk.girisYapilanID].TC,bnk.aktif_musteri.TC);
			strcpy(bnk.bry_musteriler[bnk.girisYapilanID].sifre,bnk.aktif_musteri.sifre);
			strcpy(bnk.bry_musteriler[bnk.girisYapilanID].tip,bnk.aktif_musteri.tip);
			
		for(i=0;i<bnk.aktif_musteri.hesapsayisi;i++) // hesap verilerini g�ncelle	
		{
			bnk.bry_musteriler[bnk.girisYapilanID].hesap[i].id = bnk.aktif_musteri.hesap[i].id ;
			bnk.bry_musteriler[bnk.girisYapilanID].hesap[i].bakiye = bnk.aktif_musteri.hesap[i].bakiye;
			bnk.bry_musteriler[bnk.girisYapilanID].hesap[i].No = bnk.aktif_musteri.hesap[i].No ;
		}
			
			
			
			
			
		}
		else  //T�CAR�
			{
			bnk.tcr_musteriler[bnk.girisYapilanID].No = bnk.aktif_musteri.No ;
			bnk.girisYapilanID =  bnk.aktif_musteri.id ;
			bnk.tcr_musteriler[bnk.girisYapilanID].hesapsayisi = bnk.aktif_musteri.hesapsayisi;
			strcpy(bnk.tcr_musteriler[bnk.girisYapilanID].adSoyad,bnk.aktif_musteri.adSoyad);
			strcpy(bnk.tcr_musteriler[bnk.girisYapilanID].TC,bnk.aktif_musteri.TC);
			strcpy(bnk.tcr_musteriler[bnk.girisYapilanID].sifre,bnk.aktif_musteri.sifre);
			strcpy(bnk.tcr_musteriler[bnk.girisYapilanID].tip,bnk.aktif_musteri.tip);
			
			
		 for(i=0;i<bnk.aktif_musteri.hesapsayisi;i++) // hesap verilerini g�ncelle	
		{
			bnk.tcr_musteriler[bnk.girisYapilanID].hesap[i].id = bnk.aktif_musteri.hesap[i].id ;
			bnk.tcr_musteriler[bnk.girisYapilanID].hesap[i].bakiye = bnk.aktif_musteri.hesap[i].bakiye;
			bnk.tcr_musteriler[bnk.girisYapilanID].hesap[i].No = bnk.aktif_musteri.hesap[i].No ;
		}
			
		}
	}
	
	
	
	
}



void BankaVerileriniGuncelle()
{
	FILE *fp;
	fp = fopen("banka.txt","w");
	fprintf(fp,"Bireysel Musteri Sayisi: %d\nTicari Musteri Sayisi: %d",bnk.bireyselMS,bnk.ticariMS);
	fclose(fp);	
	BankaVerileriniCek();
}





void BankaVerileriniCek()
{
	
	FILE *fp;
	fp = fopen("banka.txt","r");
	if(fp == NULL)
		{
			//Daha once hic bu dosya ac�lmamis o y�zden s�f�rlayarak a�
			BankaVerileriniGuncelle();
		}
	fscanf(fp,"Bireysel Musteri Sayisi: %d\nTicari Musteri Sayisi: %d",&bnk.bireyselMS,&bnk.ticariMS);
	fclose(fp);

}


void MusteriVerileriniCek()
{
	int i,j;
	FILE *bireyselF,*ticariF;
	bireyselF = fopen("bireysel_musteriler.txt","r");
	ticariF = fopen("ticari_musteriler.txt","r");
	
	if(bireyselF == NULL){
			//Daha once hic bu dosya ac�lmamis o y�zden bi ol�u�tur dosyay�
			bireyselF = fopen("bireysel_musteriler.txt","w");
			fclose(bireyselF);
			MusteriVerileriniCek();
		}
	if(ticariF == NULL){
			//Daha once hic bu dosya ac�lmamis o y�zden bi ol�u�tur dosyay�
			ticariF = fopen("ticari_musteriler.txt","w");
			fclose(ticariF);
			MusteriVerileriniCek();
		}

	
	//Bireysel M��terileri Programa �ek
	for(i=0;i<bnk.bireyselMS;i++){
		fscanf(bireyselF,"%[^\n]\n\t\tID: %d\n\t\tNO: %d\n\t\tTC: %s\n\t\tSifre: %s\n\t\tTip: %s\n\t\tHesap Sayisi: %d\n",&bnk.bry_musteriler[i].adSoyad,&bnk.bry_musteriler[i].id,&bnk.bry_musteriler[i].No,&bnk.bry_musteriler[i].TC,&bnk.bry_musteriler[i].sifre,&bnk.bry_musteriler[i].tip,&bnk.bry_musteriler[i].hesapsayisi);
		
		for(j=0;j<bnk.bry_musteriler[i].hesapsayisi;j++)
		{
		fscanf(bireyselF,"\n\t\t\tHesap ID: %d",&bnk.bry_musteriler[i].hesap[j].id);
		fscanf(bireyselF,"\n\t\t\tHesap No: %d",&bnk.bry_musteriler[i].hesap[j].No);
		fscanf(bireyselF,"\n\t\t\tHesap Bakiyesi: %f\n",&bnk.bry_musteriler[i].hesap[j].bakiye);
		}
	}
	
	for(i=0;i<bnk.ticariMS;i++){

	fscanf(ticariF,"%[^\n]\n\t\tID: %d\n\t\tNO: %d\n\t\tTC: %s\n\t\tSifre: %s\n\t\tTip: %s\n\t\tHesap Sayisi: %d\n",&bnk.tcr_musteriler[i].adSoyad,&bnk.bry_musteriler[i].id,&bnk.bry_musteriler[i].No,&bnk.tcr_musteriler[i].TC,&bnk.tcr_musteriler[i].sifre,&bnk.tcr_musteriler[i].tip,&bnk.tcr_musteriler[i].hesapsayisi);
		for(j=0;j<bnk.tcr_musteriler[i].hesapsayisi;j++)
		{
			fprintf(ticariF,"\n\t\t\tHesap ID: %d",&bnk.tcr_musteriler[i].hesap[j].id);
			fprintf(ticariF,"\n\t\t\tHesap No: %d",&bnk.tcr_musteriler[i].hesap[j].No);
			fprintf(ticariF,"\n\t\t\tHesap Bakiyesi: %f\n",&bnk.tcr_musteriler[i].hesap[j].bakiye);
		}
	}
	
	fclose(ticariF);
	fclose(bireyselF);
	
	
	
}


void MusteriVerileriniGuncelle()
{
	GirisYapilanHesabiGuncelle();
	
	
	FILE *bireyselF,*ticariF;
	bireyselF = fopen("bireysel_musteriler.txt","w");
	ticariF = fopen("ticari_musteriler.txt","w");
	int i,j;
	
	for(i=0;i<bnk.bireyselMS;i++){
		
		fprintf(bireyselF,"%s\n\t\tID: %d\n\t\tNO: %d\n\t\tTC: %s\n\t\tSifre: %s\n\t\tTip: %s\n\t\tHesap Sayisi: %d\n",bnk.bry_musteriler[i].adSoyad,i,bnk.bry_musteriler[i].No,bnk.bry_musteriler[i].TC,bnk.bry_musteriler[i].sifre,bnk.bry_musteriler[i].tip,bnk.bry_musteriler[i].hesapsayisi);
		for(j=0;j<bnk.bry_musteriler[i].hesapsayisi;j++)
		{
			fprintf(bireyselF,"\n\t\t\tHesap ID: %d",bnk.bry_musteriler[i].hesap[j].id);
			fprintf(bireyselF,"\n\t\t\tHesap No: %d",bnk.bry_musteriler[i].hesap[j].No);
			fprintf(bireyselF,"\n\t\t\tHesap Bakiyesi: %.2f\n",bnk.bry_musteriler[i].hesap[j].bakiye);
		}
	}
	
	//Ticari Musterileri Guncelle
	
	for(i=0;i<bnk.ticariMS;i++){
		fprintf(ticariF,"%s\n\t\tID: %d\n\t\tNO: %d\n\t\tTC: %s\n\t\tSifre: %s\n\t\tTip: %s\n\t\tHesap Sayisi: %d\n",bnk.tcr_musteriler[i].adSoyad,i,bnk.tcr_musteriler[i].No,bnk.tcr_musteriler[i].TC,bnk.tcr_musteriler[i].sifre,bnk.tcr_musteriler[i].tip,bnk.tcr_musteriler[i].hesapsayisi);
		for(j=0;j<bnk.tcr_musteriler[i].hesapsayisi;j++)
		{
			fprintf(ticariF,"\n\t\t\tHesap ID: %d",bnk.tcr_musteriler[i].hesap[j].id);
			fprintf(ticariF,"\n\t\t\tHesap No: %d",bnk.tcr_musteriler[i].hesap[j].No);
			fprintf(ticariF,"\n\t\t\tHesap Bakiyesi: %.2f\n",bnk.tcr_musteriler[i].hesap[j].bakiye);
		}
	}
	
	fclose(bireyselF);
	fclose(ticariF);
	MusteriVerileriniCek();	
}


int RastgeleSayiOlustur(int tip) // Hesap Numarasi Mi yoksa Musteri Numarasi Mi ? 0 Musteri No , 1 Hesap No
{
	int a,i,j;
	srand(time(NULL));
	if(tip == 0) // Musteri Numarasi Olustur
	{
		a = ((rand()%9+1) * 1000) + rand()%1000;	
		for(i=0;i<bnk.bireyselMS;i++)
		{
			if(bnk.bry_musteriler[i].No == a)
				RastgeleSayiOlustur(0);
				
		}
		for(i=0;i<bnk.ticariMS;i++)
		{
			if(bnk.tcr_musteriler[i].No == a)
				RastgeleSayiOlustur(0);
		}
		
		
		return a; // bu d�ng�lerden d�zg�n ��karsa kusursuz bir numara �retti demek
	}
	
	
	else
	// Hesap Numarasi Olustur
	{
		a = ((rand()%9+1) * 10000) + rand()%1000;	
		for(i=0;i<bnk.bireyselMS;i++)
		{
			for(j=0;j<bnk.bry_musteriler[i].hesapsayisi;j++)
			{
				if(bnk.bry_musteriler[i].hesap[j].No == a)
					RastgeleSayiOlustur(1);	
			}
					
		}
		
		for(i=0;i<bnk.ticariMS;i++)
		{
			for(j=0;j<bnk.tcr_musteriler[i].hesapsayisi;j++)
			{
				if(bnk.tcr_musteriler[i].hesap[j].No == a)
					RastgeleSayiOlustur(1);	
			}
					
		}
		
		
		return a; // bu d�ng�lerden d�zg�n ��karsa kusursuz bir numara �retti demek
	}
	
	

	
	
	
}