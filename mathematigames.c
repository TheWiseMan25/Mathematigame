/*
Andi Farhan (2006521616)
Aryoshi Wicaksono (2006532140)
Gemilang Bagas Ramadhani (2006535205)
Nabil Mafaza (2006529133)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <time.h>
#include <ctype.h>

struct soal
{
	char pertanyaan[100];
	char opsiA[10];
	char opsiB[10];
	char opsiC[10];
	char opsiD[10];
	char jawabanPilgan[10];
	char jawabanIsian[10];
	int random;
	struct soal *link;
};

struct data
{
	char nama[20];
	char tingkatKesulitan[6];
	double score;
	struct data *link;
};

void inputInt(int *input);
int cleanInput();
struct soal *tambahSoal(struct soal *head);								 //Fungsi untuk menambah soal dan jawaban.
struct soal *modifikasiSoal(struct soal *ptr);							 // Fungsi untuk memodifikasi soal.
void hapusSoal(struct soal **ptr);										 //Fungsi untuk menghapus soal dan jawaban.
struct soal *soal_rb(struct soal *head);								 //Fungsi untuk ambil soal dari .bin
struct soal *soal_next_rb(struct soal *head, FILE *filesoal);			 // Fungsi untuk mencari soal selanjutnya yang ada di .bin
void soal_wb(struct soal *head);										 // Fungsi untuk menyimpan soal di .bin
struct data *hi_score_rd(struct data *stackptr);						 //Ambil data data dari .bin
struct data *hi_score_new(struct data *newEntry, struct data *stackptr); //Buat entry baru di data
void hi_score_pr(struct data *stackptr);								 //Cetak data ke output
void hi_score_wb(struct data *stackptr);								 //Tulis kembali ke file bin
void game(struct data **startData, struct soal *startSoal);				 //Fugnsi untuk mengerjakan soal
void randomSoal(struct soal *head);										 // Fungsi untuk mengacak urutan soal yang ditampilkan

int main(void)
{
	srand(time(0));
	int selectMenu;
	char kataSandi[12];
	struct soal *headSoal = NULL;
	struct data *headData = NULL;

#pragma omp parallel sections
	{
#pragma omp section
		headSoal = soal_rb(headSoal);
#pragma omp section
		headData = hi_score_rd(headData);
	}

	do
	{

		system("cls");

		printf("\n\n\n\t\t\tSilahkan Memilih Menu Yang Anda Inginkan\n\n\n");
		printf("\t\t\t1.Informasi Permainan\n");
		printf("\t\t\t2.Memulai Permainan\n");
		printf("\t\t\t3.Memodifikasi Library Soal\n");
		printf("\t\t\t4.Tambah Soal\n");
		printf("\t\t\t5.Delete Soal\n");
		printf("\t\t\t6.Menampilkan Highscore\n");
		printf("\t\t\t7.Keluar\n\n\n");
		printf("\t\t\tMenu yang dipilih: ");
		//men-scan menu yang dipilih
		scanf("%d", &selectMenu);
		//Fungsi yang melaksanakan tugas berdasarkan pilihan menu dari pengguna.
		switch (selectMenu)
		{
		case 1:
			system("cls");
			printf("\n\n\n\t\t\tMathematigame adalah program yang memungkinkan admin\n");
			printf("\t\t\tuntuk menuliskan soal-soal matematika beserta jawaban\n");
			printf("\t\t\tdan menjadikan soal-soal yang telah ditulis sebagai kuis\n");
			printf("\t\t\tuntuk dikerjakan oleh pengguna lain. Skor akhir pengguna\n");
			printf("\t\t\takan diurutkan dengan nilai tertinggi terletak pada puncak\n");
			printf("\t\t\tdan ditulis pada file .txt.\n\n");
			system("pause");
			break;

		case 2:
			system("cls");
			randomSoal(headSoal);
			game(&headData, headSoal);

			break;

		case 3:
			system("cls");
			printf("Masukkan kata sandi: ");
			scanf("  %[^\n]s", kataSandi);
			if (strcmp("proglanasyik", kataSandi) != 0)
			{
				printf("Kata sandi yang Anda masukkan salah. Anda akan dikembalikan ke menu awal.\n");
				system("pause");
				break;
			}
			system("cls");
			headSoal = modifikasiSoal(headSoal);
			//Mengambil input nama dan pilihan tingkat kesulitan
			//bumper
			//memulai permainan
			break;

		case 4:
			system("cls");
			printf("Masukkan kata sandi: ");
			scanf("  %[^\n]s", kataSandi);
			if (strcmp("proglanasyik", kataSandi) != 0)
			{
				printf("Kata sandi yang Anda masukkan salah. Anda akan dikembalikan ke menu awal.\n");
				system("pause");
				break;
			}
			system("cls");
			headSoal = tambahSoal(headSoal);
			break;

		case 5:
			system("cls");
			printf("Masukkan kata sandi: ");
			scanf("  %[^\n]s", kataSandi);
			if (strcmp("proglanasyik", kataSandi) != 0)
			{
				printf("Kata sandi yang Anda masukkan salah. Anda akan dikembalikan ke menu awal.\n");
				system("pause");
				break;
			}
			system("cls");
			hapusSoal(&headSoal);
			break;

		case 6:
			system("cls");
			hi_score_pr(headData);
			break;

		case 7:
			break;
		}
	} while (selectMenu != 7);

#pragma omp parallel sections
	{
#pragma omp section
		soal_wb(headSoal);
#pragma omp section
		hi_score_wb(headData);
	}
	return 0;
}

struct soal *modifikasiSoal(struct soal *ptr)
//Fungsi untuk memodifikasi soal dan jawaban dalam struct soal.
{
	int i = 1;
	int urutan = 1;
	struct soal *temp = ptr;

	if (temp != NULL)
	{

		while (temp != NULL) //Mencetak seluruh soal yang ada.
		{
			printf("%d)\n", i);
			printf("Soal: %s\n", temp->pertanyaan);
			printf("Opsi jawaban A: %s\n", temp->opsiA);
			printf("Opsi jawaban B: %s\n", temp->opsiB);
			printf("Opsi jawaban C: %s\n", temp->opsiC);
			printf("Opsi jawaban D: %s\n", temp->opsiD);
			printf("Jawaban pilihan ganda: %s\n", temp->jawabanPilgan);
			printf("Jawaban isian: %s\n\n", temp->jawabanIsian);
			i++;
			temp = temp->link;
		}
		i--;

		temp = ptr;

		printf("Masukkan urutan soal yang Anda ingin ganti: "); //Memasukkan urutan soal yang ingin dimodifikasi.
		inputInt(&urutan);
		while (urutan > i)
		{
			printf("Soal tidak ditemukan! Harap masukkan angka yang benar!\n");
			inputInt(&urutan);
		}
		i = 1;

		while (i != urutan)
		{
			i++;
			temp = temp->link;
		}
		system("cls");

		printf("Masukkan soal baru:\n"); //Memodifikasi soal yang telah dipilih.
		scanf(" %[^\n]s", temp->pertanyaan);

		printf("Masukkan opsi jawaban A baru:\n");
		scanf(" %[^\n]s", temp->opsiA);

		printf("Masukkan opsi jawaban B baru:\n");
		scanf(" %[^\n]s", temp->opsiB);

		printf("Masukkan opsi jawaban C baru:\n");
		scanf(" %[^\n]s", temp->opsiC);

		printf("Masukkan opsi jawaban D baru:\n");
		scanf(" %[^\n]s", temp->opsiD);

		printf("Masukkan jawaban pilihan ganda baru:\n");
		scanf(" %[^\n]s", temp->jawabanPilgan);

		printf("Masukkan jawaban isian baru:\n");
		scanf(" %[^\n]s", temp->jawabanIsian);
	}
	else
	{
		printf("Soal tidak ditemukan\n");
		getch();
	}

	return ptr;
}

void inputInt(int *input)
{
	char newline;
	while ((scanf("%d%c", input, &newline) != 2 || newline != '\n') && cleanInput())
	{
		puts("Masukkan angka yang benar.");
	}
}

int cleanInput()
{
	while (getchar() != '\n')
		;
	return 1;
}

void game(struct data **startData, struct soal *startSoal)
{
	int nyawa;
	char jawab[10];
	float jawabIsian;
	int jumlahBenar = 0;
	int urutan;
	char namaPemain[20];

	struct soal *currentSoal = startSoal;
	struct data *currentData = malloc(sizeof(struct data));
	currentData->link = NULL;

	printf("Nama : ");
	scanf(" %[^\n]s", currentData->nama);
	printf("Tingkat Kesulitan (Mudah, Sedang, Sulit) :");
	scanf("%s", currentData->tingkatKesulitan);

	//Mengecek input kesulitan agar sesuai
	while (strcmpi(currentData->tingkatKesulitan, "MUDAH") != 0 && strcmpi(currentData->tingkatKesulitan, "SEDANG") != 0 && strcmpi(currentData->tingkatKesulitan, "SULIT") != 0)
	{
		printf("Masukkan tingkat kesulitan yang benar!\n");
		printf("Tingkat Kesulitan :");
		scanf("%s", currentData->tingkatKesulitan);
	}

	//untuk tingkat kesulitan mudah akan diberi 5 nyawa, sedangkan yang lain 3 nyawa
	if (strcmpi(currentData->tingkatKesulitan, "MUDAH") == 0)
	{
		nyawa = 5;
	}
	else
		nyawa = 3;

	if (currentSoal != NULL)
	{
		//Menampilkan soal  5 soal atau kurang
		while (currentSoal != NULL && urutan != 5)
		{
			//Apabila nyawa habis maka masuk penghitungan skor
			if (nyawa == 0)
			{
				break;
			}
			++urutan;
			// Apabila difficulty MUDAH atau SEDANG, jawaba soal merupakan pilihan ganda
			if (strcmpi(currentData->tingkatKesulitan, "MUDAH") == 0 || strcmpi(currentData->tingkatKesulitan, "SEDANG") == 0)
			{
				printf("\n%s\n\n", currentSoal->pertanyaan);
				printf("A. %-10s C. %-10s\n", currentSoal->opsiA, currentSoal->opsiC);
				printf("B. %-10s D. %-10s\n", currentSoal->opsiB, currentSoal->opsiD);
			}
			else
			{ // Apabila difficulty SULIT, maka jawaban soal merupakan isian
				printf("%s\n", currentSoal->pertanyaan);
			}

			printf("\nSisa nyawa : %d \n\n", nyawa);
			printf("Jawab : ");
			scanf("%s", jawab);

			//Mencocokkan input jawaban dengan jawaban soal
			if (strcmpi(jawab, currentSoal->jawabanPilgan) == 0 || strcmpi(jawab, currentSoal->jawabanIsian) == 0)
			{
				printf("\n\tBENAR!");
				jumlahBenar += 1;
			}
			else
			{
				printf("\n\tSALAH!");
				--nyawa;
			}
			getch();
			system("cls");

			currentSoal = currentSoal->link;
		}

		//Penghitungan skor
		if (strcmpi(currentData->tingkatKesulitan, "MUDAH") == 0)
		{
			currentData->score = jumlahBenar * 10;
		}
		else if (strcmpi(currentData->tingkatKesulitan, "SEDANG") == 0)
		{
			currentData->score = jumlahBenar * 15;
		}
		else
		{
			currentData->score = jumlahBenar * 20;
		}

		//Memasukkan data ke sorting Highscore
		*startData = hi_score_new(currentData, *startData);
		//Mencetak hasil skor
		printf("\n Skor : %1f \n\n", currentData->score);
		getch();
	}
	else
	{
		printf("Maaf, soal tidak ditemukan \n");
	}
}

struct soal *tambahSoal(struct soal *head)
//Fungsi untuk menambah soal dan jawaban dalam struct soal.
{

	struct soal *temp = malloc(sizeof(struct soal)); //Membuat struct soal dengan nama temp untuk digunakan di dalam fungsi.
	temp->link = NULL;

	temp->random = rand();

	printf("Masukkan soal baru:\n"); //Meminta admin untuk memasukkan soal beserta jawaban-jawaban
	scanf(" %[^\n]s", temp->pertanyaan);

	printf("Masukkan opsi jawaban A baru:\n");
	scanf(" %[^\n]s", temp->opsiA);

	printf("Masukkan opsi jawaban B baru:\n");
	scanf(" %[^\n]s", temp->opsiB);

	printf("Masukkan opsi jawaban C baru:\n");
	scanf(" %[^\n]s", temp->opsiC);

	printf("Masukkan opsi jawaban D baru:\n");
	scanf(" %[^\n]s", temp->opsiD);

	printf("Masukkan jawaban pilihan ganda baru:\n");
	scanf(" %[^\n]s", temp->jawabanPilgan);

	printf("Masukkan jawaban isian baru:\n");
	scanf(" %[^\n]s", temp->jawabanIsian);

	if (head == NULL) //Memasukkan soal baru pada linked list.
	{
		head = temp;
	}
	else
	{
		struct soal *currentSoal;
		currentSoal = head;
		while (currentSoal->link != NULL)
		{
			currentSoal = currentSoal->link;
		}
		currentSoal->link = temp;
	}
	return head;
}

void hapusSoal(struct soal **ptr)
//Fungsi untuk mengapus soal dalam struct soal.
{
#pragma omp parallel
	{
#pragma omp single
		{
			int i = 0, posisi;
			struct soal *sebelum = *ptr;
			struct soal *sekarang = *ptr;
			struct soal *head = *ptr;

			while (head != NULL) //Mencetak semua soal yang ada.
			{
				i++;
				printf("%d)\n", i);
				printf("Soal: %s\n", head->pertanyaan);
				printf("Opsi jawaban A: %s\n", head->opsiA);
				printf("Opsi jawaban B: %s\n", head->opsiB);
				printf("Opsi jawaban C: %s\n", head->opsiC);
				printf("Opsi jawaban D: %s\n", head->opsiD);
				printf("Jawaban pilihan ganda: %s\n", head->jawabanPilgan);
				printf("Jawaban isian: %s\n\n", head->jawabanIsian);
				head = head->link;
			}

			printf("Masukkan urutan soal yang ingin Anda hapus: "); //Meminta admin untuk memasukkan urutan soal yang ingin dihapus.
			inputInt(&posisi);
			if (posisi > i)
			{
				printf("\nSoal tidak ditemukan! Masukkan urutan soal yang ingin Anda hapus: ");
				inputInt(&posisi);
			}

			if (*ptr == NULL)
			{
				printf("Tidak ada soal.\n\n");
				system("pause");
			}

			else if (posisi == 1) //Menghapus soal apabila urutan soal yang ingin dihapus adalah soal ke-1.
			{
				*ptr = sekarang->link;
				free(sekarang);
				sekarang = NULL;
			}

			else
			{
				while (posisi != 1) //Menghapus soal apabila urutan soal yang ingin dihapus bukan soal ke-1.
				{
					sebelum = sekarang;
					sekarang = sekarang->link;
					posisi--;
				}
				sebelum->link = sekarang->link;
				free(sekarang);
				sekarang = NULL;
			}
		}
	}
}

void soal_wb(struct soal *head)
{
	FILE *filesoal;

	filesoal = fopen("soal.bin", "wb");

	if (filesoal != NULL)
	{
		struct soal *soalSekarang = head;

		struct soal *temp = NULL;

		while (soalSekarang != NULL)
		{
			temp = soalSekarang->link; //simpan address currentsiswa	selanjutnya

			soalSekarang->link = NULL; //kosongin link currentsiswa

			fseek(filesoal, 0, SEEK_END);
			fwrite(soalSekarang, sizeof(struct soal), 1, filesoal);

			soalSekarang = temp; //lanjut ke list selanjutnya

			temp = NULL;
		}
		fclose(filesoal);
		filesoal = NULL;
	}
	else
	{
		printf("FILE OPEN ERROR\n");
	}
}

struct soal *soal_next_rb(struct soal *head, FILE *filesoal)
{

	if (head == NULL)
	{
		head = malloc(sizeof(struct soal));
		fread(head, sizeof(struct soal), 1, filesoal);
		head->link = NULL;
	}
	else
	{
		struct soal *indexData = head;
		struct soal *newData = malloc(sizeof(struct soal));
		while (indexData->link != NULL)
		{
			indexData = indexData->link;
		}
		fread(newData, sizeof(struct soal), 1, filesoal);
		indexData->link = newData;
		newData->link = NULL;
	}
	return head;
}

struct soal *soal_rb(struct soal *head)
{

	FILE *filesoal;
	filesoal = fopen("soal.bin", "rb");
	if (filesoal != NULL)
	{

		head = NULL;

		fseek(filesoal, 0, SEEK_END);
		long fileSize = ftell(filesoal);
		rewind(filesoal);

		//Mencari jumlah soal yang diinput
		int jumlahsoal = (int)(fileSize / (sizeof(struct soal)));

		int i = 0;
		//memasukkan soal dari file ke linkedlist
		for (i = 0; i < jumlahsoal; ++i)
		{
			fseek(filesoal, (sizeof(struct soal) * i), SEEK_SET);
			head = soal_next_rb(head, filesoal);
		}
		fclose(filesoal);
	}
	else
	{
		printf("FILE OPEN ERROR FOR READ\n");
	}

	return head;
}

struct data *hi_score_rd(struct data *stackptr)
/* Membaca file .bin yang menyimpan leaderboard */
{
	int i = 0;
	int seek = 1;
	FILE *hiscore;
	struct data *player = NULL;

	hiscore = fopen("hiscore.bin", "rb");
	if (hiscore == NULL)
	{
		puts("data still empty...");
	}
	else
	{
		player = malloc(sizeof(struct data));
		while (fgetc(hiscore) != '\t')
		/*
    Read nama satu per satu sampai \t ditemukan
		sebagai tanda akhir nama, lihat hi_score_wb untuk detilnya
    */
		{

			fseek(hiscore, -1, SEEK_CUR);
			fread(&player->nama[i], 1, 1, hiscore);
			i++;
		}
		player->nama[i] = '\0'; // Memastikan string berhenti di akhir nama
		fread(&player->tingkatKesulitan, sizeof(player->tingkatKesulitan), 1, hiscore);
		fread(&player->score, sizeof(double), 1, hiscore);
		i = 0;

		player->link = NULL;
		stackptr = player;
	}

	while (fgetc(hiscore) != EOF)
	{

		player = malloc(sizeof(struct data));
		fseek(hiscore, -1, SEEK_CUR);
		while (fgetc(hiscore) != '\t')
		{
			fseek(hiscore, -1, SEEK_CUR);
			fread(&player->nama[i], 1, 1, hiscore);
			i++;
		}
		player->nama[i] = '\0';
		fread(&player->tingkatKesulitan, sizeof(player->tingkatKesulitan), 1, hiscore);
		fread(&player->score, sizeof(double), 1, hiscore);
		i = 0;

		player->link = stackptr;
		stackptr = player;
	}

	fclose(hiscore);

	puts("END of file read..");

	return stackptr;
}

struct data *hi_score_new(struct data *newEntry, struct data *stackptr)
/* New entry ke leaderboard */
{
	struct data *prev = NULL;
	struct data *topstack;

	topstack = stackptr;

	do
	{
		if (topstack == NULL) // Entry pertama
		{
			topstack = newEntry;

			break;
		}
		else if (newEntry->score < stackptr->score) //Sorting
		{
			if (stackptr->link == NULL) //Nilai paling kecil
			{
				stackptr->link = newEntry;
				newEntry->link = NULL;
				break;
			}
			else //Swap
			{
				prev = stackptr;
				stackptr = stackptr->link;
			}
		}
		else if (stackptr == topstack && newEntry->score > stackptr->score) //Nilai paling besar
		{
			newEntry->link = stackptr;
			topstack = newEntry;
			break;
		}
		else // Nilai bukan terkecil maupun terbesar
		{
			prev->link = newEntry;
			newEntry->link = stackptr;
			break;
		}
	} while (stackptr != NULL);

	// List/node terakhir adalah nilai paling kecil

	return topstack;
}

void hi_score_pr(struct data *stackptr)
/* Printing leaderboard ke output */
{
	int rank = 1;
	int i;
	char uppercase[6];

	puts("\n\n\n\t=================================================");
	puts("\t| No |         Nama         | Kesulitan | Nilai |");
	puts("\t=================================================");

	while (stackptr != NULL)
	{
		printf("\t| %i. | %-20s |   ", rank, stackptr->nama);
		strcpy(uppercase, stackptr->tingkatKesulitan);
		for (i = 0; i < 6; i++)
			printf("%c", toupper(uppercase[i]));
		if (strcmpi(uppercase, "sedang") == 0)
			printf("  "); //Keperluan formatting tabel
		else
			printf("   "); //Keperluan formatting tabel
		printf("| %5.1lf |\n", stackptr->score);
		stackptr = stackptr->link;
		rank++;
	}
	puts("\t=================================================");
	getch();
}

void hi_score_wb(struct data *stackptr)
/*
Menulis leaderboard ke dalam file .bin. Peringkat terakhir
ditulis terlebih dahulu agar pada saat pembacaan di run berikutnya
peringkat terakhir ada di list/node terakhir dari stack
*/
{
	struct data *toWrite, *topstack;
	struct data *prev = NULL;
	FILE *hiscore;

	if (stackptr != NULL)
	{
		hiscore = fopen("hiscore.bin", "wb");
		topstack = stackptr;

		while (toWrite != topstack) // Bagian ini mengambil alamat terakhir dari list/node.
		{
			while (1)
			{
				if (stackptr->link == NULL)
				{
					toWrite = stackptr;
					break;
				}
				else
				{
					prev = stackptr;
					stackptr = stackptr->link;
				}
			}

			puts("Writing to file . . .");
			fwrite(&toWrite->nama, sizeof(toWrite->nama), 1, hiscore);
			fputc('\t', hiscore); //Menuliskan \t setelah nama sebagai penanda akhir dari nama
			fwrite(&toWrite->tingkatKesulitan, sizeof(toWrite->tingkatKesulitan), 1, hiscore);
			fwrite(&toWrite->score, sizeof(toWrite->score), 1, hiscore);

			if (prev != NULL)
				prev->link = NULL;
			stackptr = topstack;
			puts("DONE !");
		}

		fclose(hiscore);
		puts("ALL DONE !");
	}
}

void randomSoal(struct soal *head)
{
	/* 
Fungsi ini untuk mengacak soal agar soal yang dimunculkan selalu berbeda urutan.
Fungsi ini menggunakan bubblesort dengan memanfaatkan variabel random untuk diurutkan. 
*/

	char tempPertanyaan[100];
	char tempA[10];
	char tempB[10];
	char tempC[10];
	char tempD[10];
	char tempJawabanPilgan[10];
	char tempJawabanIsian[10];
	int flag;

	struct soal *last = NULL, *current = head;
	while (current != NULL)
	{
		current->random = rand();
		current = current->link;
	}

	do
	{
		flag = 0;
		current = head;

		while (current->link != last)
		{
			if (current->random > current->link->random)
			{

				strcpy(tempPertanyaan, current->pertanyaan);
				strcpy(current->pertanyaan, current->link->pertanyaan);
				strcpy(current->link->pertanyaan, tempPertanyaan);

				strcpy(tempA, current->opsiA);
				strcpy(current->opsiA, current->link->opsiA);
				strcpy(current->link->opsiA, tempA);

				strcpy(tempB, current->opsiB);
				strcpy(current->opsiB, current->link->opsiB);
				strcpy(current->link->opsiB, tempB);

				strcpy(tempC, current->opsiC);
				strcpy(current->opsiC, current->link->opsiC);
				strcpy(current->link->opsiC, tempC);

				strcpy(tempD, current->opsiD);
				strcpy(current->opsiD, current->link->opsiD);
				strcpy(current->link->opsiD, tempD);

				strcpy(tempJawabanPilgan, current->jawabanPilgan);
				strcpy(current->jawabanPilgan, current->link->jawabanPilgan);
				strcpy(current->link->jawabanPilgan, tempJawabanPilgan);

				strcpy(tempJawabanIsian, current->jawabanIsian);
				strcpy(current->jawabanIsian, current->link->jawabanIsian);
				strcpy(current->link->jawabanIsian, tempJawabanIsian);

				flag = 1;
			}
			current = current->link;
		}
		last = current;
	} while (flag);
}

