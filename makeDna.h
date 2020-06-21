#pragma once
#include<iostream>
#include <random>
#include"variance.h"
using namespace std;

int cnt[5] = { 0 };

// A C G T 몇개 나왔는지 세는 함수
void makeCnt_File(char* cntFile_path) {
	FILE* fp = fopen(cntFile_path, "w");
	if (fp == nullptr) {
		cout << "file open failure" << endl;
		return;
	}
	for (int i = 0; i < 5; ++i) {
		fprintf(fp, "%d ", cnt[i]);
	}
	fclose(fp);
}

// cntFile 읽는 함수
void readCnt_File(char* cntFile_path) {
	FILE* fp = fopen(cntFile_path, "r");
	if (fp == nullptr) {
		cout << "file open failure" << endl;
		return;
	}
	for (int i = 0; i < 5; ++i) {
		fscanf(fp, "%d", &cnt[i]);
	}
	fclose(fp);
}

// dna 파일 만드는 함수
void makeDnaFile(char* fileName, char* info, int len) {
	FILE* fp = fopen(fileName, "w");
	if (fp == nullptr) {
		cout << "file open failure" << endl;
		return;
	}
	for (int i = 0; i < len; ++i) {
		fputc(info[i], fp);
	}
	fprintf(fp, "\n");
	fclose(fp);
}

// 나의 dna file 만들기
void makeMyDna(uniform_int_distribution<int>& dis, mt19937& gen, char* fname) {
	FILE* fp = fopen(fname, "w");   // 파일 열기
	char nucleotide[] = { 'A','C','G','T' };  // nucleotide 배열

	for (int i = 0; i < DNA_LEN; i++) {   // MAX_DNA_LEN 만큼 반복
		fputc(nucleotide[dis(gen)], fp);     // A,C,G,T 중 랜덤하게 파일에 한 문자씩 입력
	}
	fclose(fp); // 파일 닫기
}

// reference file 만들기
void makeReference(char* fname_myDna, char* fname_ref, uniform_int_distribution<int>& dis, uniform_int_distribution<int>& dis2, mt19937& gen) {
	FILE* fp = fopen(fname_myDna, "r");
	FILE* fp2 = fopen(fname_ref, "w");  // 파일 열기
	char nucleotide[] = { 'A','C','G','T' };  // nucleotide 배열

	int num_of_mutation = 0;        // 변형 수 저장
	for (int i = 0; i < DNA_LEN; i++) {  // MAX_DNA_LEN 만큼 반복
		// original DNA 에서 한 글자씩 읽어옴
		char c = fgetc(fp);
		// X_RATE 확률로 변형
		if (dis2(gen) < X_RATE) {
			char mutate;
			// A,C,G,T 중 무작위로 변형 생성. 원본과 같을 시, 재생성
			do {
				mutate = nucleotide[dis(gen)];
			} while (c == mutate);
			fputc(mutate, fp2); // 변형된 문자 쓰기
			num_of_mutation++;  // 변형 수 증가
		}
		else fputc(c, fp2);
	}

	fclose(fp); // original DNA 파일 닫기
	fclose(fp2); // reference DNA 파일 닫기
}

// 숏리드 파일 만들기
void makeShortRead(char* fname_myDna,char* fname_shortRead ,uniform_int_distribution<int>& dis3, mt19937& gen) {
	FILE* fp = fopen(fname_myDna, "r");  // original DNA 파일 읽기 권한으로 다시 열기
   // DNA 문자 배열로 저장
	char* DNA = new char[DNA_LEN + 1];
	for (int i = 0; i < DNA_LEN; i++) DNA[i] = fgetc(fp);
	DNA[DNA_LEN] = '\0';

	char* shortRead = new char[LEN_OF_SHORTREAD + 1];
	shortRead[LEN_OF_SHORTREAD] = '\0';

	//bool* covered = new bool[DNA_LEN] {}; // shortread가 커버한 위치를 표시할 배열
//	int overlap = 0;

	FILE* fp3 = fopen(fname_shortRead, "w");
	if (fp3 == nullptr) {
		cout << "file open failure" << endl;
		return;
	}

	for (int i = 0; i < NUM_OF_SHORTREAD; ++i) {
		int r = dis3(gen); // short read 길이를 고려한 임의의 위치 지정. (양 끝 shortRead 조치)
		if (r < 0) r = 0;
		if (r > DNA_LEN - LEN_OF_SHORTREAD) r = DNA_LEN - LEN_OF_SHORTREAD;

		// DNA 임의의 위치에서 읽어와 정해진 길이의 shortRead 생성.
//		if (covered[r]) overlap++; // 중복 shortRead 검사
		for (int j = 0; j < LEN_OF_SHORTREAD; j++) {
			shortRead[j] = DNA[r + j];
			//	covered[r + j] = true;
		}
		fputs(shortRead, fp3);
		fputs("\n", fp3);
	}
	fclose(fp3);
	delete[] shortRead;
	//delete[] covered;
}