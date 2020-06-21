#define _CRT_SECURE_NO_WARINGS
#include"bwt.h"
#include"makeDna.h"
#include<time.h>
using namespace std;

char reference[DNA_LEN + 2];
CiruclarInfo TempTable[RUN_SIZE];
BWTInfo table[DNA_LEN + 1];

void readFile(char* fname, char* dnaInfo);
void compare(char* file1, char* file2, char* explain);
void printDNA(char* info);


void prepareBWTData() {
	cout << "making bwt table" << endl;
	// 1) 순환 파일 만들기
	readRef(ref_File_Path, reference);
	makeCircularFile(circular_File_Path, reference);

	// 2) run 파일 만들기
	createInitialRuns(circular_File_Path, RUN_SIZE, NUM_FILE);

	// 3) 외부 정렬하기
	mergeFiles(BWT_File_Path, RUN_SIZE, NUM_FILE, cnt);

	makeCnt_File(cnt_File);
	
	cout << "bwt table complete" << endl;
	puts(""); puts(""); puts("");
}

int main()
{	
	prepareBWTData();
	//--------------BWT 완료------------

	// 4) 비교
	readCnt_File(cnt_File);
	readRef(ref_File_Path, reference);
	readBWTTable(BWT_File_Path, table);

	time_t start, end;
	start = clock();
	reconstruct(shortRead_File_Path,reference ,cnt, table, THRESHOLD);	//실행하면 reference가 복원된 dna가 됨
	cout << "recon comeplete" << endl;
	makeDnaFile(recon_File, reference, DNA_LEN);	
	cout << "recon-make file comeplete" << endl;
	end = clock();
	printf(" time cost : %.50f\n", (double)(end-start)/ CLOCKS_PER_SEC);


	//결과 출력
	compare(ref_File_Path, myDna_File_Path,(char*) "reference and myDna compare");
	compare(recon_File, myDna_File_Path, (char*)"reconstructed_myDna and myDna compare");
	
	return 0;
}



void readFile(char* fname, char* dnaInfo) {
	FILE* fp = fopen(fname, "r");
	for (int i = 0; i < DNA_LEN; ++i) {
		dnaInfo[i] = fgetc(fp);
	}
	dnaInfo[DNA_LEN] = '\0';
}

void compare(char* file1, char* file2, char* explain) {
	cout << explain << endl;
	char dna_1[DNA_LEN + 1];
	char dna_2[DNA_LEN + 1];

	readFile(file1, dna_1);
	readFile(file2, dna_2);
	int count = 0;
	for (int i = 0; i < DNA_LEN; ++i) {
		if (dna_1[i] != dna_2[i]) {
			//cout << i << " ";
			count++;
		}
	}
	cout << "how many mismtach : " << count << endl;
	cout << ((DNA_LEN-count)/(double)DNA_LEN)* 100 <<"%"<<endl;
	puts("");
}

void printDNA(char* info) {
	for (int i = 0; i < DNA_LEN; ++i) {
		cout << info[i];
	}
	puts("");
}
