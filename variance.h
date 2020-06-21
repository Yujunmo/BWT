#pragma once
// 시연 시간에 건드려야할 변수 
#define DNA_LEN 1000						//DNA 길이
#define NUM_OF_SHORTREAD 30				// short read 개수
#define LEN_OF_SHORTREAD 50					// short read 길이
//시연 파일 경로
char* myDna_File_Path = (char*)"Mydna_1000.txt";
char* ref_File_Path = (char*)"test_ref_1000.txt";
char* shortRead_File_Path = (char*)"shortread_1000.txt";



#define RUN_SIZE 100						//run 크기
#define FILE_NAME_DIGIT 10					//파일 이름 생성에 필요한 글자 자리수 << 건들지 말것.
#define NUM_FILE DNA_LEN/RUN_SIZE+1			//run파일 개수
#define X_RATE	5							// DNA의 변형 정도(%)
#define THRESHOLD 1							// mismatch 허용 개수 (N이면 N개까지 허용)

char* circular_File_Path = (char*)"circular.txt";
char* BWT_File_Path = (char*)"BWT.txt";
char* recon_File = (char*)"recon.txt";
char* cnt_File = (char*)"cnt.txt";
//run파일들이 만들어지는 경로는 bwt.h 에서 createInitialRuns 함수에 있음