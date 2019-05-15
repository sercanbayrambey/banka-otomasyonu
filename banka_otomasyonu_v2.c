#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <locale.h>

/* 
-************Yeni Müþteri Ekleme
    -Müþteri No (Special)
    -Bireysel / Ticari (Farklý Dosyalar)

****************-Hesap Açtýrma
    -Müþteri istediði kadar hesap açar
    -Hesap numaralarý eþsiz

-Para Çekme
    -Bireysel max günlük para çekme: 750, ticari: 1500
    ************-Hesabýnda yoksa ek hesap

**************-Para Yatýrma
TODO: KENDÝNE HAVALE YAPAMASIN
************************************-Hesaba Havale
    -Havale yapacagý kiþinin hesap no
    -Ticari müþtelerilerden havale ücreti kesilmez, bireysel müþteriler %2 kesilir.

-Banka Gelir & Gider Raporu
    -bankadan giden, gelen ve bankada bulunan toplam para miktarý vb
    -Kullanýcý sistem üzerinden rapor al kýsmýný seçerse, rapor isimli dosyadan okuma yapýlýp ekranda gösterilecektir.

-Hesap Özeti
    -Belirtilen tarih aralýðýnda
    -Çekilen, yatýrýlan, havale yapýlmýþsa kime yapýldýðý ve miktarý, baþka bir hesaptan havale para geldiyse kimden geldiði ve miktarý gibi 
    bilgiler ve bu iþlemlerin tarihleri görüntülenmelidir. Ve dekont.txt isimli dosyaya yazýlacaktýr.

-Hesap Kapama
    -Bakiye 0 olmasý gerekli, kapatýldýðýnda müþteri bilgileri de silinecek!

	
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
void HesaptanParaCek();
void ParayiEkHesaptanCek(float eksikMiktar,float cekilecekMiktar,int anaHesapID);
void HavaleYap();
int MusteriIDBul(int hesapNo);
int HesapIDBul(int hesapNo);
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
		int girisYapilanHesapTipi; // 0 BIREYSEL 1 TICARI
		int havaleYapilacakHesapTipi; // 0 BIREYSEL 1 TICARI
		int bireyselMS,ticariMS; //  ms = musteri sayisi		
	}bnk; 
	

	
	
	
int main()
{
	bnk.girisYapilanHesapTipi = -1;
	bnk.girisYapilanID = -1;
	bnk.havaleYapilacakHesapTipi = -1;
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
	printf("\n\n\n<------------ Ana Menu ------------>\n");
	printf("<!> 1. Yeni Müþteri Kaydý\n");
	printf("<!> 2. Müþteri Giriþi\n");
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
		printf("<!> Hatalý seçim yaptýnýz!");
		Sleep(1500);
		AnaMenu();	
		}
		
}


void YeniMusteri()
{
	char secim[20],No[8];
	int i;
	int musteriTip=0; // Bireysel mi ticari mi ? // 0 Bï¿½REYSEL // 1 Tï¿½CARï¿½

	system("cls");
	printf("\n\n\n<------------ Yeni Müþteri Kaydý ------------>\n");
	printf("\n<!> Müþteri tipinizi seçiniz:\n\t1. Bireysel\n\t2. Ticari\n\t0. Ana Menuye Dön");
	printf("\nSeçiminiz: ");
	scanf("%s",&secim);	
	
	//Hesap Tipini Seçiniz
	if(strcmp(secim,"1") == 0)
			musteriTip=0;
	else if(strcmp(secim,"2") == 0)
		 	musteriTip=1;
	else if(strcmp(secim,"0") == 0)
		 	AnaMenu();
	else{
		printf("Hatalý bir seçim yaptýnýz!");
		Sleep(1500);
		YeniMusteri();
		return;
		}
		
			
	if(musteriTip == 0) // BIREYSEL MUSTERI KAYDI YAP
	{
		
		printf("<!>Ad Soyad:");
		fflush(stdin);
		gets(bnk.bry_musteriler[bnk.bireyselMS].adSoyad); // TODO: NUMARA GIRMEYI ENGELLE
		printf("\n<!>TC:");
		gets(bnk.bry_musteriler[bnk.bireyselMS].TC);
	
		if(KayitSorgula(0) == -1)
			{
				printf("\n<!> Böyle bir müþteri zaten var.");
				Sleep(1500);
				YeniMusteri();
				return;
			}
	
	
		if(strlen(bnk.bry_musteriler[bnk.bireyselMS].TC)!= 11 || isdigit(*bnk.bry_musteriler[bnk.bireyselMS].TC) == 0)
		{
			printf("Hatalý bir kimlik numarasý girdiniz.");
			Sleep(1500);
			YeniMusteri();
			return;	
		}
		fflush(stdin);
	

		printf("\n<!>Þifre:");
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
				printf("\n<!> Böyle bir müþteri zaten var.");
				Sleep(1500);
				YeniMusteri();
				return;
			}


	
		if(strlen(bnk.tcr_musteriler[bnk.ticariMS].TC)!= 11 || isdigit(*bnk.tcr_musteriler[bnk.ticariMS].TC) == 0)
		{
			printf("Hatalý bir kimlik numarasý girdiniz.");
			Sleep(1000);
			YeniMusteri();
			return;	
		}
		fflush(stdin);
		printf("\n<!>Þifre:");
		gets(bnk.tcr_musteriler[bnk.ticariMS].sifre);
		strcpy(bnk.tcr_musteriler[bnk.ticariMS].tip,"Ticari");
	
		bnk.tcr_musteriler[bnk.ticariMS].No = RastgeleSayiOlustur(0);
		bnk.ticariMS++;
	
	
	} //ticari musteri kayit son
		
	
	
		
	
	// MUSTERI OLUSTURULDU, GEREKLI GUNCELLEMELER YAPILSIN
	
		BankaVerileriniGuncelle();
		MusteriVerileriniGuncelle();
	//MUSTERI OLUSTURMA BITTI
	printf("\n<!> Kaydýnýz oluþturuldu. Ana menüye yönlendiriliyorsunuz...");
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
		if(strcmp(bnk.tcr_musteriler[i].TC,bnk.tcr_musteriler[bnk.ticariMS].TC) == 0)	
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
	printf("\n\n\n<------------ Müþteri Giriþi ------------>\n");
	printf("<!> TC Kimlik Numarasý (0 = Ýptal Et): ");
	fflush(stdin);
	gets(TC);
	if(strcmp(TC,"0") == 0)
		AnaMenu();
		
	printf("\n<!> Þifre:");
	gets(sifre);
	printf("\n<!> Musteri tipiniz (1 = Bireysel, 2 = Ticari): ");
	gets(tip);
	
	if(strcmp(tip,"1") != 0 && strcmp(tip,"2") != 0 )
		{
		printf("<!> Hatalý bir seçim yaptýnýz!");
		Sleep(1500);
		MusteriGirisi();
		return;
		}
			
	
	if(MusteriGirisSorgula(TC,sifre,tip) == -1) //-1 dï¿½ndï¿½yse sorgulama tamam demektir
	{
		GirisYapilanHesabiProgramaCek();
		printf("\n<!> Giriþ Baþarýlý!!\n\n");
		Sleep(1000);
		MusteriMenu();
	}
	else
	{
		printf("\n<!> Giriþ Baþarýsýz!");
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
		
		for(i=0;i<bnk.bireyselMS;i++)
		{
			if(strcmp(bnk.bry_musteriler[i].TC,TC) == 0 && strcmp(bnk.bry_musteriler[i].sifre,sifre) == 0 )
			{
				bnk.girisYapilanID = i;
				bnk.girisYapilanHesapTipi = 0; // 0 Bï¿½REYSEL // 1 Tï¿½CARI
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
	printf("\n\n\n<------------ Musteri Menu ------------>\n");
	printf("<> Ad Soyad: %s\n<> Müþteri No: %d\n<> TC: %s\n<> Müþteri Tipi: %s\n<> Hesap Sayýsý: %d\n\n",bnk.aktif_musteri.adSoyad,bnk.aktif_musteri.No,bnk.aktif_musteri.TC,bnk.aktif_musteri.tip,bnk.aktif_musteri.hesapsayisi);
			
	printf("<!> 1. Hesap Açma\n");
	printf("<!> 2. Hesaplarým\n");
	printf("<!> 3. Para Yatýrma\n");
	printf("<!> 4. Para Çekme\n");
	printf("<!> 5. Havale Yap\n");
	printf("<!> 4. Banka Gelir - Gider Raporu\n");
	printf("<!> 0. Çýkýþ Yap\n");
	printf("<!> Seçim Yapýnýz: ");
	scanf("%s",&secim);
	
	
	if(strcmp(secim,"1") == 0)
		HesapAc();
	else if(strcmp(secim,"2") == 0)
		HesaplarimMenu();
	else if(strcmp(secim,"3") == 0)
		HesabaParaYatir();
	else if(strcmp(secim,"4") == 0)
		HesaptanParaCek();
	else if(strcmp(secim,"5") == 0)
		HavaleYap();
		
	else if(strcmp(secim,"0") == 0)
	{
		bnk.girisYapilanHesapTipi = -1; // Cikis yap
		AnaMenu();
	}
		
		
	else{
		printf("<!> Hatalý bir seçim yaptýnýz !");
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
	printf("<> Ad Soyad: %s\n<> Musteri No: %d\n<> TC: %s\n<> Musteri Tipi: %s\n<> Hesap Sayisi: %d\n\n",bnk.aktif_musteri.adSoyad,bnk.aktif_musteri.No,bnk.aktif_musteri.TC,bnk.aktif_musteri.tip,bnk.aktif_musteri.hesapsayisi);
	bnk.aktif_musteri.hesap[bnk.aktif_musteri.hesapsayisi].No = RastgeleSayiOlustur(1);
	bnk.aktif_musteri.hesap[bnk.aktif_musteri.hesapsayisi].id = bnk.aktif_musteri.hesapsayisi;
	bnk.aktif_musteri.hesap[bnk.aktif_musteri.hesapsayisi].bakiye = 0;
	printf("<!> 1. Hesap Numaraniz: %d\n",bnk.aktif_musteri.hesap[bnk.aktif_musteri.hesapsayisi].No);
	printf("<!> Devam etmek ve hesabýnýzý açmak için bir tuþa basýn (0 = Islemi iptal et ve menüye dön) ...");
	secim = getch();
	if(secim == '0')
		MusteriMenu();
	
	 // Hesap Basariyla Acildi
	bnk.aktif_musteri.hesapsayisi++;
	MusteriVerileriniGuncelle();
	printf("\n<!> Hesabýnýz baþarýyla açýldý!");
	Sleep(1500);
	MusteriMenu();
	
}


void HesaplarimMenu()
{
	int i,j;
	char secim;
	system("cls");
	printf("\n\n\n<------------ Hesaplarým ------------>\n");
	printf("<> Ad Soyad: %s\n<> Müþteri No: %d\n<> TC: %s\n<> Müþteri Tipi: %s\n<> Hesap Sayisi: %d\n\n",bnk.aktif_musteri.adSoyad,bnk.aktif_musteri.No,bnk.aktif_musteri.TC,bnk.aktif_musteri.tip,bnk.aktif_musteri.hesapsayisi);

	for(i=0;i<bnk.aktif_musteri.hesapsayisi;i++)
	{
		printf("\t----------------\n");
		printf("\t<!> Hesap ID: %d\n",bnk.aktif_musteri.hesap[i].id);
		printf("\t<!> Hesap No: %d\n",bnk.aktif_musteri.hesap[i].No);
		printf("\t<!> Hesap Bakiyesi: %.2f\n",bnk.aktif_musteri.hesap[i].bakiye);
		printf("\t----------------\n\n");
		
	}
	
	printf("<!> Geriye dönmek için bir tuþa basýnýz....");
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
	printf("\n\n\n<------------ Para Yatýrma ------------>\n");
	for(i=0;i<bnk.aktif_musteri.hesapsayisi;i++)
	{
		printf("\t----------------\n");
		printf("\t<!> Hesap ID: %d\n",bnk.aktif_musteri.hesap[i].id);
		printf("\t<!> Hesap No: %d\n",bnk.aktif_musteri.hesap[i].No);
		printf("\t<!> Hesap Bakiyesi: %.2f\n",bnk.aktif_musteri.hesap[i].bakiye);
		printf("\t----------------\n\n");
		
	}
	
	printf("\n\n\t<!> Para yatýrmak istediðiniz hesap ID sini giriniz (-1 = Önceki Menüye Dön): ");
	scanf("%s",secim);
	
	if(strcmp(secim,"-1") == 0)
		MusteriMenu();
	
	if(isdigit(*secim) == 0)
	{
		printf("\t<!> Hatalý bir seçim yaptýnýz.");
		Sleep(1500);
		HesabaParaYatir();
		return;
	}
	
	secilenID = atoi(secim);
	
	if(secilenID>=bnk.aktif_musteri.hesapsayisi || secilenID<0)
	{
		printf("\t<!> Geçersiz bir ID numarasý girdiniz.");
		Sleep(1500);
		HesabaParaYatir();
		return;
	}
	
	
	printf("\t<!> Hesap Numaraniz: %d, devam etmek için bir tuþa basýnýz... (0 = Iþlemi Ýptal Et ve Menüye Dön)",bnk.aktif_musteri.hesap[secilenID].No);
	
	*secim = getch();
	
	if(secim == "0")
		{
			MusteriMenu();
			return;
		}
		

	printf("\n\t<!> Yatýrmak istediðiniz miktarý yazýnýz: ");
	scanf("%s",&para);
	
	
	if(isdigit(*para) == 0)
	{
		printf("\t<!> Hatalý bir para girdiniz.");
		Sleep(1500);
		HesabaParaYatir();
		return;
	}
	
	
	yatirilacakMiktar = atof(para);
	printf("\n\t<!> Hesabýnýza yatýrýlacak miktar %.2f TL, onaylýyorsanýz bir tuþa basýnýz... (0 = Ýslemi Ýptal Et ve Menüye Dön)",yatirilacakMiktar);
	
	*secim = getch();
	if(secim == "0")
		{
			MusteriMenu();
			return;
		}

	// ParayýHesaba Ekle
	bnk.aktif_musteri.hesap[secilenID].bakiye+=yatirilacakMiktar;
	bnk.hazine += yatirilacakMiktar;
	MusteriVerileriniGuncelle();
	BankaVerileriniGuncelle();
	//Parayï¿½ Hesaba Ekleme Son
	
	printf("\n\t<!> %.2f TL hesabýnýza yatýrýldý. Yeni bakiyeniz: %.2f. \n\t<!>Ana menüye dönmek için bir tuþa basýnýz...\n\n\n\n",yatirilacakMiktar,bnk.aktif_musteri.hesap[secilenID].bakiye);
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
	
	
	printf("\n\n\n<------------ Para Çekme ------------>\n");
	for(i=0;i<bnk.aktif_musteri.hesapsayisi;i++)
	{
		printf("\t----------------\n");
		printf("\t<!> Hesap ID: %d\n",bnk.aktif_musteri.hesap[i].id);
		printf("\t<!> Hesap No: %d\n",bnk.aktif_musteri.hesap[i].No);
		printf("\t<!> Hesap Bakiyesi: %.2f\n",bnk.aktif_musteri.hesap[i].bakiye);
		printf("\t----------------\n\n");
		
	}
	
	printf("\n\n\t<!> Para çekmek istediðiniz hesap ID sini giriniz (-1 = Önceki Menüye Dön): ");
	scanf("%s",secim);
	
	if(strcmp(secim,"-1") == 0)
		MusteriMenu();
	
	if(isdigit(*secim) == 0)
	{
		printf("\t<!> Hatalý bir seçim yaptýnýz.");
		Sleep(1500);
		HesaptanParaCek();
		return;
	}
	
	secilenID = atoi(secim);
	
	if(secilenID>=bnk.aktif_musteri.hesapsayisi || secilenID<0)
	{
		printf("\t<!> Geçersiz bir ID numarasý girdiniz.");
		Sleep(2000);
		HesaptanParaCek();
		return;
	}
	
	
	printf("\n\t<!> Hesap Numaraniz: %d, devam etmek için bir tuþa basýnýz... (0 = Iþlemi Ýptal Et ve Menüye Dön)",bnk.aktif_musteri.hesap[secilenID].No);
	
	*secim = getch();
	
	if(secim == "0")
		{
			MusteriMenu();
			return;
		}
		

	printf("\n\t<!> Çekmek istediðiniz miktarý yazýnýz: ");
	scanf("%s",&para);
	
	
	if(isdigit(*para) == 0)
	{
		printf("\t<!> Hatalý bir para girdiniz.");
		Sleep(1500);
		HesaptanParaCek();
		return;
	}
	
	
	cekilecekMiktar = atof(para);
	
	if(cekilecekMiktar>bnk.aktif_musteri.hesap[secilenID].bakiye){
		eksikMiktar = cekilecekMiktar - bnk.aktif_musteri.hesap[secilenID].bakiye;
		printf("\n\t<!> Hesabýnýzda o kadar para yok. Eksik olan %0.2f TL'yi ek hesabýnýzdan çekmek için bir tuþa basýn.. (0 = Ýþlemi Ýptal Et)",eksikMiktar);
		*secim = getch();
		if(secim == "0"){
			printf("<!> Para çekme iþlemi baþarýsýz oldu!");
			Sleep(1500);
			MusteriMenu();
			return;
		}
		
		ParayiEkHesaptanCek(eksikMiktar,cekilecekMiktar,secilenID);
		return;
	}
	
	
	
	printf("\n\t<!> Hesabýnýzdan çekilecek miktar %.2f TL, onaylýyorsanýz bir tuþa basýnýz... (0 = Ýslemi Ýptal Et ve Menüye Dön)",cekilecekMiktar);
	
	*secim = getch();
	if(secim == "0")
		{
			MusteriMenu();
			return;
		}

	// Parayý Hesaptan Kes
	bnk.aktif_musteri.hesap[secilenID].bakiye-=cekilecekMiktar;
	bnk.hazine -= cekilecekMiktar;
	MusteriVerileriniGuncelle();
	BankaVerileriniGuncelle();
	//Parayý Hesaptan Kesme Son
	
	printf("\n\t<!> %.2f TL hesabýnýzdan çekildi. Yeni bakiyeniz: %.2f. \n\t<!>Ana menüye dönmek için bir tuþa basýnýz...\n\n\n\n",cekilecekMiktar,bnk.aktif_musteri.hesap[secilenID].bakiye);
	*secim = getch();
	MusteriMenu();
	return;
	
	
}



void ParayiEkHesaptanCek(float eksikMiktar,float cekilecekMiktar,int anaHesapID)
{
	int i;
	int hesapID = -1;// eðer -1 kalýrsa ek hesabýnda da para yok
	char *secim;
	int gonderilecekMiktar;
	//Hesaplarý gez eksikmiktardan büyük olan hesap varsa ondan çek
	for(i=0;i<bnk.aktif_musteri.hesapsayisi;i++)
		{
			if(bnk.aktif_musteri.hesap[i].bakiye >= eksikMiktar)
			{
				hesapID = i;
				break;
			}
			
		}
		
		
	if(hesapID == -1){
			printf("\n\t<!> Hiç bir hesabýnýzda %.2f TL bulunamadi. Ana menuye yönlendiriliyorsunuz..",eksikMiktar);
			Sleep(3000);
			MusteriMenu();
			return;
		}
		
		printf("\n\t<!> Eksik olan %.2f TL, %d No'lu hesabýnýzdan çekilecek.",eksikMiktar,bnk.aktif_musteri.hesap[hesapID].No);
		printf("\n\t<!> Bu hesabýnýzdaki mevcut bakiye %.2f TL, devam etmek istiyorsanýz bir tuþa basýnýz..(0 = Ýþlemi Ýptal Et)",bnk.aktif_musteri.hesap[hesapID].bakiye);
		
		*secim = getch();
		if(secim == "0")
			{
				MusteriMenu();
				return;
			}
			
			//Ek hesaptan ve orjinal hesaptan parayý çek
			bnk.aktif_musteri.hesap[hesapID].bakiye-=eksikMiktar;
			bnk.aktif_musteri.hesap[anaHesapID].bakiye = 0;
			bnk.hazine-= cekilecekMiktar;
			
			MusteriVerileriniGuncelle();
			BankaVerileriniGuncelle();
			
			printf("\n\t<!> %.2f TL hesaplarýnýzdan baþarýyla çekildi, ana menuye dönmek için bir tuþa basýnýz..",cekilecekMiktar);
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
	for(i=0;i<bnk.aktif_musteri.hesapsayisi;i++)
	{
		printf("\t----------------\n");
		printf("\t<!> Hesap ID: %d\n",bnk.aktif_musteri.hesap[i].id);
		printf("\t<!> Hesap No: %d\n",bnk.aktif_musteri.hesap[i].No);
		printf("\t<!> Hesap Bakiyesi: %.2f\n",bnk.aktif_musteri.hesap[i].bakiye);
		printf("\t----------------\n\n");
		
	}
	
	printf("\n\n\t<!> Paranýzýn çekileceði hesap ID'sýný giriniz (-1 Menüye Dön): ");
	scanf("%s",secim);
	
	if(strcmp(secim,"-1") == 0)
		MusteriMenu();
	
	if(isdigit(*secim) == 0)
	{
		printf("\t<!> Hatalý bir seçim yaptýnýz.");
		Sleep(1500);
		HavaleYap();
		return;
	}
	
	secilenID = atoi(secim);
	
	if(secilenID>=bnk.aktif_musteri.hesapsayisi || secilenID<0)
	{
		printf("\t<!> Geçersiz bir ID numarasý girdiniz.");
		Sleep(2000);
		HavaleYap();
		return;
	}
	
	
	printf("\n\t<!> Hesap Numaraniz: %d, devam etmek için bir tuþa basýnýz... (0 = Iþlemi Ýptal Et ve Menüye Dön)",bnk.aktif_musteri.hesap[secilenID].No);
	
	*secim = getch();
	
	if(secim == "0")
		{
			MusteriMenu();
			return;
		}
		

	printf("\n\t<!> Göndermek istediðiniz miktarý yazýnýz (Bireysel Müþterilerden %%2 islem ücreti kesilir.): ");
	scanf("%s",&para);

	
	if(isdigit(*para) == 0)
	{
		printf("\t<!> Hatalý bir para girdiniz.");
		Sleep(2000);
		HavaleYap();
		return;
	}
	
	if(bnk.girisYapilanHesapTipi == 0) // Bireysel ise %2 iþlem ücreti kes
		komisyon = atof(para)*2/100;
	else
		komisyon = 0;

	gonderilecekMiktar = atof(para) + komisyon;
	
	if(gonderilecekMiktar>bnk.aktif_musteri.hesap[secilenID].bakiye){
		printf("\n\t<!> Hesabýnýzda o kadar para yok. Eksik olan: %0.2f TL Komisyon: %.2f TL. Menüye yönlendiriliyorsunuz..",bnk.aktif_musteri.hesap[secilenID].bakiye-gonderilecekMiktar,komisyon);
		Sleep(4000);
		MusteriMenu();
		return;
	}
	
	printf("\n\t<!> %.2f TL göndereceksiniz, iþlem ücreti %.2f TL",gonderilecekMiktar-komisyon,komisyon);
	printf("\n\t<!> Parayý göndereceðiniz Hesap No: ");
	scanf("%s",&tempHesapNo);
	if(isdigit(*tempHesapNo) == 0)
	{
		printf("\n\t<!> Hatalý bir Hesap No girdiniz.");
		Sleep(3000);
		HavaleYap();
		return;
	}
	havaleHesapNo = atoi(tempHesapNo);
	havaleMusteriID = MusteriIDBul(havaleHesapNo);
	havaleHesapID = HesapIDBul(havaleHesapNo);
	if(havaleMusteriID == -1)
	{
		printf("\n\t<!> Böyle bir müþteri bulunamadý veya kendi hesabýnýza havale yapmaya çalýþýyorsunuz! ");
		Sleep(3000);
		HavaleYap();
		return;
	}
	
	
	if(bnk.havaleYapilacakHesapTipi == 0) // Bireysel Ýþlemler
	{
	printf("\n\t<!> %.2f TL göndereceðiniz Hesap No: %d, Kayýtlý Olduðu Ad Soyad: %s",gonderilecekMiktar,havaleHesapNo,bnk.bry_musteriler[havaleMusteriID].adSoyad);
	printf("\n\t<!> Onaylýyorsanýz bir tuþa basýn... (0= Ýptal)");
	*secim = getch();
	if(secim == "0")
		{
			HavaleYap();
			return;
		}
		
	//Gonderme basarili		
	bnk.aktif_musteri.hesap[secilenID].bakiye-=gonderilecekMiktar;
	if(bnk.havaleYapilacakHesapTipi == 0)
		bnk.bry_musteriler[havaleMusteriID].hesap[havaleHesapID].bakiye+= (gonderilecekMiktar - komisyon);	
	else
		bnk.tcr_musteriler[havaleMusteriID].hesap[havaleHesapID].bakiye+= (gonderilecekMiktar - komisyon);
		
		
	MusteriVerileriniGuncelle();
	BankaVerileriniGuncelle();
	//Gönderme tamamlandý
	printf("\n\t<!> %.2f TL %d nolu hesaba baþarýyla gönderildi.",gonderilecekMiktar-komisyon,havaleHesapNo);
	printf("\n\t<!> Menüye dönmek için bir tuþa basýnýz...");
	*secim = getch();
	MusteriMenu();
	return;
	}
	else // TÝCARÝ HESABA GÖNDER
		{
	printf("\n\t<!> %.2f TL göndereceðiniz Hesap No: %d, Kayýtlý Olduðu Ad Soyad: %s",gonderilecekMiktar,havaleHesapNo,bnk.tcr_musteriler[havaleMusteriID].adSoyad);
	printf("\n\t<!> Onaylýyorsanýz bir tuþa basýn... (0= Ýptal)");
	*secim = getch();
	if(secim == "0")
		{
			HavaleYap();
			return;
		}
		
	//Gonderme basarili		
	bnk.aktif_musteri.hesap[secilenID].bakiye-=gonderilecekMiktar;
	bnk.tcr_musteriler[havaleMusteriID].hesap[havaleHesapID].bakiye+= (gonderilecekMiktar - komisyon);	
	MusteriVerileriniGuncelle();
	BankaVerileriniGuncelle();
	printf("\n\t<!> %.2f TL %d nolu hesaba baþarýyla gönderildi.",gonderilecekMiktar-komisyon,havaleHesapNo);
	printf("\n\t<!> Menüye dönmek için bir tuþa basýnýz...");
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
	
	for(i=0;i<bnk.bireyselMS;i++)
	{
		for(j=0;j<bnk.bry_musteriler[i].hesapsayisi;j++)
		{
			if(hesapNo == bnk.bry_musteriler[i].hesap[j].No  )
				{
				id = bnk.bry_musteriler[i].id;
				bnk.havaleYapilacakHesapTipi = 0;			
				return id;		
				}
				
		}
	}
	
	for(i=0;i<bnk.ticariMS;i++)
	{
		for(j=0;j<bnk.tcr_musteriler[i].hesapsayisi;j++)
		{
			if(hesapNo == bnk.tcr_musteriler[i].hesap[j].No )
				{
				id = bnk.tcr_musteriler[i].id;
				bnk.havaleYapilacakHesapTipi = 1;
				return id;					
				}
				
		}
	}
	
	
	return id; // -1 dönerse böyle bir hesap yok !
	
}


int HesapIDBul(int hesapNo)
{
	int i,j;
	int id = -1;
	for(i=0;i<bnk.bireyselMS;i++)
	{
		for(j=0;j<bnk.bry_musteriler[i].hesapsayisi;j++)
		{
			if(hesapNo == bnk.bry_musteriler[i].hesap[j].No)
				{
				id = bnk.bry_musteriler[i].hesap[j].id;
				return id;					
				}
				
		}
	}
	
	for(i=0;i<bnk.ticariMS;i++)
	{
		for(j=0;j<bnk.tcr_musteriler[i].hesapsayisi;j++)
		{
			if(hesapNo == bnk.tcr_musteriler[i].hesap[j].No)
				{
				id = bnk.tcr_musteriler[i].hesap[j].id;
				return id;					
				}
				
		}
	}
	
	
	return id; // -1 dönerse böyle bir hesap yok !
	
}


void GirisYapilanHesabiProgramaCek()
{
	int i,j;
	
	
	if(bnk.girisYapilanHesapTipi == 0) // Bï¿½REYSEL ï¿½SE
	{
		bnk.aktif_musteri.No = bnk.bry_musteriler[bnk.girisYapilanID].No;
		bnk.aktif_musteri.id = bnk.girisYapilanID;
		bnk.aktif_musteri.hesapsayisi  = bnk.bry_musteriler[bnk.girisYapilanID].hesapsayisi;
		strcpy(bnk.aktif_musteri.adSoyad,bnk.bry_musteriler[bnk.girisYapilanID].adSoyad);
		strcpy(bnk.aktif_musteri.TC,bnk.bry_musteriler[bnk.girisYapilanID].TC);
		strcpy(bnk.aktif_musteri.sifre,bnk.bry_musteriler[bnk.girisYapilanID].sifre);
		strcpy(bnk.aktif_musteri.tip,bnk.bry_musteriler[bnk.girisYapilanID].tip);
		
		for(i=0;i<bnk.bry_musteriler[bnk.girisYapilanID].hesapsayisi;i++) // hesap verilerini ï¿½ek	
		{
			bnk.aktif_musteri.hesap[i].id = bnk.bry_musteriler[bnk.girisYapilanID].hesap[i].id;
			bnk.aktif_musteri.hesap[i].bakiye = bnk.bry_musteriler[bnk.girisYapilanID].hesap[i].bakiye;
			bnk.aktif_musteri.hesap[i].No = bnk.bry_musteriler[bnk.girisYapilanID].hesap[i].No;
		}
		
	}
	else  //Tï¿½CARï¿½
		{
		bnk.aktif_musteri.No = bnk.tcr_musteriler[bnk.girisYapilanID].No;
		bnk.aktif_musteri.id = bnk.girisYapilanID;
		bnk.aktif_musteri.hesapsayisi  = bnk.tcr_musteriler[bnk.girisYapilanID].hesapsayisi;
		strcpy(bnk.aktif_musteri.adSoyad,bnk.tcr_musteriler[bnk.girisYapilanID].adSoyad);
		strcpy(bnk.aktif_musteri.TC,bnk.tcr_musteriler[bnk.girisYapilanID].TC);
		strcpy(bnk.aktif_musteri.sifre,bnk.tcr_musteriler[bnk.girisYapilanID].sifre);
		strcpy(bnk.aktif_musteri.tip,bnk.tcr_musteriler[bnk.girisYapilanID].tip);
		
		
		
		for(i=0;i<bnk.tcr_musteriler[bnk.girisYapilanID].hesapsayisi;i++) // hesap verilerini ï¿½ek	
		{
			bnk.aktif_musteri.hesap[i].id = bnk.tcr_musteriler[bnk.girisYapilanID].hesap[i].id;
			bnk.aktif_musteri.hesap[i].bakiye = bnk.tcr_musteriler[bnk.girisYapilanID].hesap[i].bakiye;
			bnk.aktif_musteri.hesap[i].No = bnk.tcr_musteriler[bnk.girisYapilanID].hesap[i].No;
		}
		
	}
	
	return;
}


void GirisYapilanHesabiGuncelle()
{
	int i,j;
	
	
	if(bnk.girisYapilanHesapTipi == 0 || bnk.girisYapilanHesapTipi == 1) // Eger -1 se henuz kimse giris yapmamï¿½stï¿½r
	{
		if(bnk.girisYapilanHesapTipi == 0) // Bï¿½REYSEL ï¿½SE
		{
			bnk.bry_musteriler[bnk.girisYapilanID].No = bnk.aktif_musteri.No ;
			bnk.girisYapilanID =  bnk.aktif_musteri.id ;
			bnk.bry_musteriler[bnk.girisYapilanID].hesapsayisi = bnk.aktif_musteri.hesapsayisi;
			strcpy(bnk.bry_musteriler[bnk.girisYapilanID].adSoyad,bnk.aktif_musteri.adSoyad);
			strcpy(bnk.bry_musteriler[bnk.girisYapilanID].TC,bnk.aktif_musteri.TC);
			strcpy(bnk.bry_musteriler[bnk.girisYapilanID].sifre,bnk.aktif_musteri.sifre);
			strcpy(bnk.bry_musteriler[bnk.girisYapilanID].tip,bnk.aktif_musteri.tip);
			
		for(i=0;i<bnk.aktif_musteri.hesapsayisi;i++) // hesap verilerini gï¿½ncelle	
		{
			bnk.bry_musteriler[bnk.girisYapilanID].hesap[i].id = bnk.aktif_musteri.hesap[i].id ;
			bnk.bry_musteriler[bnk.girisYapilanID].hesap[i].bakiye = bnk.aktif_musteri.hesap[i].bakiye;
			bnk.bry_musteriler[bnk.girisYapilanID].hesap[i].No = bnk.aktif_musteri.hesap[i].No ;
		}
				
			
		}
		else  //Tï¿½CARï¿½
			{
			bnk.tcr_musteriler[bnk.girisYapilanID].No = bnk.aktif_musteri.No ;
			bnk.girisYapilanID =  bnk.aktif_musteri.id ;
			bnk.tcr_musteriler[bnk.girisYapilanID].hesapsayisi = bnk.aktif_musteri.hesapsayisi;
			strcpy(bnk.tcr_musteriler[bnk.girisYapilanID].adSoyad,bnk.aktif_musteri.adSoyad);
			strcpy(bnk.tcr_musteriler[bnk.girisYapilanID].TC,bnk.aktif_musteri.TC);
			strcpy(bnk.tcr_musteriler[bnk.girisYapilanID].sifre,bnk.aktif_musteri.sifre);
			strcpy(bnk.tcr_musteriler[bnk.girisYapilanID].tip,bnk.aktif_musteri.tip);
			
			
		 for(i=0;i<bnk.aktif_musteri.hesapsayisi;i++) // hesap verilerini gï¿½ncelle	
		{
			bnk.tcr_musteriler[bnk.girisYapilanID].hesap[i].id = bnk.aktif_musteri.hesap[i].id ;
			bnk.tcr_musteriler[bnk.girisYapilanID].hesap[i].bakiye = bnk.aktif_musteri.hesap[i].bakiye;
			bnk.tcr_musteriler[bnk.girisYapilanID].hesap[i].No = bnk.aktif_musteri.hesap[i].No ;
		}
			
		}
	}
	
	
	GirisYapilanHesabiProgramaCek();
	return;
}



void BankaVerileriniGuncelle()
{
	FILE *fp;
	fp = fopen("banka.txt","w");
	fprintf(fp,"Bireysel Musteri Sayisi: %d\nTicari Musteri Sayisi: %d",bnk.bireyselMS,bnk.ticariMS);
	fprintf(fp,"\nBanka Hazinesi: %.2f TL",bnk.hazine);
	fclose(fp);	
	BankaVerileriniCek();
	
	return;
}





void BankaVerileriniCek()
{
	
	FILE *fp;
	fp = fopen("banka.txt","r");
	if(fp == NULL)
		{
			//Daha once hic bu dosya acï¿½lmamis o yï¿½zden sï¿½fï¿½rlayarak aï¿½
			BankaVerileriniGuncelle();
		}
	fscanf(fp,"Bireysel Musteri Sayisi: %d\nTicari Musteri Sayisi: %d",&bnk.bireyselMS,&bnk.ticariMS);
	fscanf(fp,"\nBanka Hazinesi: %f TL",&bnk.hazine);
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
			//Daha once hic bu dosya acï¿½lmamis o yï¿½zden bi olï¿½uï¿½tur dosyayï¿½
			bireyselF = fopen("bireysel_musteriler.txt","w");
			fclose(bireyselF);
			MusteriVerileriniCek();
		}
	if(ticariF == NULL){
			//Daha once hic bu dosya acï¿½lmamis o yï¿½zden bi olï¿½uï¿½tur dosyayï¿½
			ticariF = fopen("ticari_musteriler.txt","w");
			fclose(ticariF);
			MusteriVerileriniCek();
		}

	
	//Bireysel Mï¿½ï¿½terileri Programa ï¿½ek
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

		fscanf(ticariF,"%[^\n]\n\t\tID: %d\n\t\tNO: %d\n\t\tTC: %s\n\t\tSifre: %s\n\t\tTip: %s\n\t\tHesap Sayisi: %d\n",&bnk.tcr_musteriler[i].adSoyad,&bnk.tcr_musteriler[i].id,&bnk.tcr_musteriler[i].No,&bnk.tcr_musteriler[i].TC,&bnk.tcr_musteriler[i].sifre,&bnk.tcr_musteriler[i].tip,&bnk.tcr_musteriler[i].hesapsayisi);
		for(j=0;j<bnk.tcr_musteriler[i].hesapsayisi;j++)
		{
			fscanf(ticariF,"\n\t\t\tHesap ID: %d",&bnk.tcr_musteriler[i].hesap[j].id);
			fscanf(ticariF,"\n\t\t\tHesap No: %d",&bnk.tcr_musteriler[i].hesap[j].No);
			fscanf(ticariF,"\n\t\t\tHesap Bakiyesi: %f\n",&bnk.tcr_musteriler[i].hesap[j].bakiye);
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
	return;
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
		
		
		return a; // bu dï¿½ngï¿½lerden dï¿½zgï¿½n ï¿½ï¿½karsa kusursuz bir numara ï¿½retti demek
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
		
		
		return a; // bu dongulerden sorunsuz cikarsa kusursuz bir numara uretti demek
	}
	
	

	
	
	
}
