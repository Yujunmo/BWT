#pragma once
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include"variance.h"
#include<iostream>
#include<random>
#include<time.h>
#include<deque>
#include<vector>
#include <iostream>
#include <algorithm>

using namespace std;

FILE* openFile(char* fileName, const char* mode)
{
	FILE* fp = fopen(fileName, mode);
	if (fp == NULL)
	{
		perror("Error while opening the file.\n");
		exit(EXIT_FAILURE);
	}
	return fp;
}

class CiruclarInfo {
public:
	int rank1;
	char* stringInfo;

	CiruclarInfo():stringInfo(nullptr) {
	}

	void allocate() {
		stringInfo = new char[DNA_LEN + 2];
	}

	void distroy() {
		if (stringInfo != nullptr) {
			delete[] stringInfo;
		}
	}
};

struct MinHeapNode {
	CiruclarInfo element;
	int i;
};

class BWTInfo {
	//추후 은닉하기
public:
	char front;
	char back;
	int rank1;
	int rank2;
};

class MinHeap {
	//추후 은닉하기
public:
	MinHeapNode node[NUM_FILE+1];
	int size;
public:
	MinHeap() :size(0) {}

	~MinHeap() {
		for (int i = 0; i <= size; ++i) {
			node[i].element.distroy();
		}
	}

	bool isEmpty() {
		return size == 0;
	}
	bool isFull() {
		return size == NUM_FILE;
	}

	MinHeapNode& getParent(int i) { return node[i / 2]; }
	MinHeapNode& getLeft(int i) { return node[i * 2]; }
	MinHeapNode& getRight(int i) { return node[i * 2 + 1]; }

	void push(MinHeapNode& pushed_node) {
		if (isFull()) {
			return;
		}
		else {
			int i = ++size;
			while (i != 1 && strcmp(getParent(i).element.stringInfo, pushed_node.element.stringInfo) == 1) {
				node[i] = getParent(i);
				i /= 2;
			}
			node[i] = pushed_node;		//포인터는 그냥 방향만 바꾼다
		}
	}

	void pop(MinHeapNode& popped) {
		if (isEmpty()) {
			cout << "empty" << endl;
			return;
		}

		popped = node[1];
		MinHeapNode last = node[size];

		size--;
		if (size == 0) {
			return;
		}
		int parent = 1;
		int child = 2;

		while (child <= size) {
			//find the index of smaller child
			if (child < size && strcmp(getLeft(parent).element.stringInfo, getRight(parent).element.stringInfo) == 1) {
				child++;
			}

			if (strcmp(last.element.stringInfo, node[child].element.stringInfo) != 1) break;
			else {
				node[parent] = node[child];
				parent = child;
				child *= 2;
			}
		}
		node[parent] = last;
	}

	void print() {
		for (int i = 1; i <= size; ++i) {
		
			for (int j = 0; j < DNA_LEN + 1; ++j) {
				cout << node[i].element.stringInfo[j];
			}
			cout <<" "<< node[i].element.rank1 << endl;
		}
	}
	int getSize() {
		return size;
	}
};

void makeStart(int* cnt, int* start, int len) {
	int sum = 0;
	for (int i = 0; i < len - 1; ++i) {
		sum += cnt[i];
		start[i] = sum;
	}
}

void makeRef() {
	srand(time(NULL));
	FILE* fp = fopen("input.txt", "w");
	int num;
	for (int i = 0; i < DNA_LEN; ++i) {
		num = rand() % 4;
		if (num == 0) {
			fputc('A', fp);
		}
		else if (num == 1) {
			fputc('C', fp);
		}
		else if (num == 2) {
			fputc('G', fp);
		}
		else {
			fputc('T', fp);
		}
	}
	fclose(fp);
}

void readRef(const char* fname, char* reference) {
	FILE* fp = fopen(fname, "r");
	if (fp == nullptr) {
		cout << "file open failure" << endl;
		return;
	}
	int count = 0;

	while (true) {
		reference[count] = fgetc(fp);
		count++;
		if (count == DNA_LEN) break;
	}
	reference[DNA_LEN] = '$';
	reference[DNA_LEN + 1] = '\0';
	fclose(fp);
}

// $를 한칸씩 밀면서 생성된 문자열들을 저장하는 circular file 만들기
void makeCircularFile(char* circularFile_path, char* ref) {
	FILE* fp = fopen(circularFile_path, "w");
	if (fp == nullptr) {
		cout << "error" << endl;
		return;
	}

	char temp;
	for (int i = 0; i < DNA_LEN + 1; ++i) {
		temp = ref[DNA_LEN];
		for (int j = DNA_LEN; j >= 1; --j) {
			ref[j] = ref[j - 1];
		}
		ref[0] = temp;

		for (int i = 0; i < DNA_LEN + 1; ++i) {
			fputc(ref[i], fp);
		}
		fputc(' ', fp);
		fprintf(fp, "%d\n", DNA_LEN - i);
	}
	fclose(fp);
}

//문자를 기준으로 소트하기
void bubblesort(CiruclarInfo* table, int tableLength) {
	bool sorted;
	int tmpRank;
	char* tmp;
	for (int i = tableLength - 1; i >= 1; --i) {
		sorted = true;
		for (int j = 0; j < i; ++j) {
			if (strcmp(table[j].stringInfo, table[j + 1].stringInfo) == 1) {
				sorted = false;
				tmp = table[j].stringInfo;
				tmpRank = table[j].rank1;

				table[j].stringInfo = table[j + 1].stringInfo;
				table[j].rank1 = table[j + 1].rank1;
				table[j + 1].stringInfo = tmp;
				table[j + 1].rank1 = tmpRank;
			}
		}
		if (sorted == true) {
			break;
		}
	}
}

void makeRank2(FILE* fp, char val, int* cnt) {
	if (val == '$') {
		cnt[0]++;
		fprintf(fp, " %d\n", cnt[0]);
		fflush(fp);
	}
	else if (val == 'A') {
		cnt[1]++;
		fprintf(fp, " %d\n", cnt[1]);
		fflush(fp);
	}
	else if (val == 'C') {
		cnt[2]++;
		fprintf(fp, " %d\n", cnt[2]);
		fflush(fp);
	}
	else if (val == 'G') {
		cnt[3]++;
		fprintf(fp, " %d\n", cnt[3]);
		fflush(fp);
	}
	else if (val == 'T') {
		cnt[4]++;
		fprintf(fp, " %d\n", cnt[4]);
		fflush(fp);
	}
}

// run 생성하기. 내부 정렬은 버블 소트 사용
void createInitialRuns(char* input_file, int run_size, const int num_ways)
{
	// For big input file
	FILE* in = openFile(input_file, "r");

	// output scratch files
	FILE* out[NUM_FILE];
	char fileName[FILE_NAME_DIGIT];


	for (int i = 0; i < num_ways; i++)
	{
		snprintf(fileName, sizeof(fileName), "%d.txt", i);
		out[i] = openFile(fileName, "w");
	}

	// allocate a dynamic array large enough to accommodate runs of
	// size run_size
	CiruclarInfo* arr = new CiruclarInfo[run_size];

	for (int i = 0; i < run_size; ++i) {
		arr[i].allocate();
	}
 
	bool more_input = true;
	int next_output_file = 0;

	int i;
	while (more_input)
	{
		
		// write run_size elements into arr from input file
		for (i = 0; i < run_size; i++)
		{
			int count = 0;
			char ch;
			while (true) {
				ch = fgetc(in);

				if (ch == -1) {
					more_input = false;
					break;
				}

				arr[i].stringInfo[count] = ch;
				count++;

				if (count == DNA_LEN + 1) break;
				
			}

			if (more_input == false) {
				break;
			}
			arr[i].stringInfo[count] = '\0';
			
			fgetc(in);
			fscanf(in, "%d", &arr[i].rank1);
			fgetc(in);

		}

		bubblesort(arr, i);

		// write the records to the appropriate scratch output file
		// can't assume that the loop runs to run_size
		// since the last run's length may be less than run_size
		for (int j = 0; j < i; j++) {
			for (int i = 0; i < DNA_LEN + 1; ++i) {
				fputc(arr[j].stringInfo[i], out[next_output_file]);
				fflush(out[next_output_file]);
			}
			
			fprintf(out[next_output_file], " %d\n", arr[j].rank1);
			fflush(out[next_output_file]);
		}

		next_output_file++;	//총 파일 개수

		if (more_input == false) {
			break;
		}
	}

	// deallocate memory
	delete[] arr;

	// close input and output files
	for (int i = 0; i < num_ways; i++)
		fclose(out[i]);

	fclose(in);
}

// run 외부정렬하고 bwt table 파일로 출력하기. 외부 정렬은 힙 소트 사용
void mergeFiles(char* output_file, int run_size, const int k, int* cnt)
{
	FILE* in[NUM_FILE];
	for (int i = 0; i < k; i++)
	{
		char fileName[FILE_NAME_DIGIT];

		// convert i to string
		snprintf(fileName, sizeof(fileName), "%d.txt", i);
		// Open output files in read mode.
		in[i] = openFile(fileName, "r");
	}

	// FINAL OUTPUT FILE
	FILE* out = openFile(output_file, "w");

	MinHeapNode harr[NUM_FILE];
	for (int i = 0; i < NUM_FILE; ++i) {
		harr[i].element.allocate();
	}

	MinHeap pq;

	int i;
	for (i = 0; i < k; i++) {
		int count = 0;
		char ch;
		while (true) {
			ch = fgetc(in[i]);

			if (ch == -1) {
				break;
			}
			harr[i].element.stringInfo[count] = ch;
			count++;
			if (count == DNA_LEN + 1) break;
		}
		fgetc(in[i]);
		fscanf(in[i], "%d", &harr[i].element.rank1);
		fgetc(in[i]);


		/*if (fscanf(in[i], "%d ", &harr[i].element) != 1)
			break;*/

			// Index of scratch output file
		harr[i].i = i;
		pq.push(harr[i]);										//여기...출력으로 확인
	}

	int count = 0;
	
	while (count != i)
	{
		
		MinHeapNode root;
		pq.pop(root);

			//힙에서 제일 작은걸 차례대로 out에 출력한다. //여기서 바로 BWT table 만든다.
		fputc(root.element.stringInfo[0], out); fflush(out);
		fputc(root.element.stringInfo[DNA_LEN], out); fflush(out);
		fprintf(out, " %d", root.element.rank1); fflush(out);
		makeRank2(out, root.element.stringInfo[DNA_LEN], cnt);

		int count_index = 0;
		char ch;
		bool flag = false;
		while (true) {
			ch = fgetc(in[root.i]);
			if (ch == -1) {
				flag = true;
				break;
			}

			root.element.stringInfo[count_index++] = ch;
			if (count_index == DNA_LEN + 1) break;
		}

		//eof에 닿았다면
		if (flag == true) {
			count++;
			continue;
		}

		//새로운 원소를 받았다면
		fgetc(in[root.i]);
		fscanf(in[root.i], "%d", &root.element.rank1);
		fgetc(in[root.i]);

		pq.push(root);
	}

	//남은 힙 다 비우기 //0일떄 조심
	while (!pq.isEmpty()) {

		MinHeapNode root;
		pq.pop(root);

		fputc(root.element.stringInfo[0], out); fflush(out);
		fputc(root.element.stringInfo[DNA_LEN], out); fflush(out);
		fprintf(out, " %d\n", root.element.rank1); fflush(out);
	}

	// close input and output files
	for (int i = 0; i < k; i++)
		fclose(in[i]);

	fclose(out);
}

// bwt table 읽어오기
void readBWTTable(const char* filename, BWTInfo* table) {
	FILE* fp = fopen(filename, "r");
	if (fp == nullptr) {
		cout << "file open failure" << endl;
		return;
	}

	for (int i = 0; i < DNA_LEN + 1; ++i) {
		table[i].front = fgetc(fp);
		table[i].back = fgetc(fp);
		fgetc(fp);
		fscanf(fp, "%d %d", &table[i].rank1, &table[i].rank2);
		fgetc(fp);
	}
	fclose(fp);
}

// 복원하기. 입력 데이터 : shortRead, reference, cnt, bwt table, mismatch 허용개수. reference가 수정되며 수정된 정보는 외부 파일로 출력됨.
void reconstruct(char* shortRead_File,char* reference ,int* cnt, BWTInfo* table, int threshold) {
	FILE* fp = fopen(shortRead_File, "r");
	if (fp == nullptr) {
		cout << "file open failure" << endl;
		return;
	}

	char shortRead[LEN_OF_SHORTREAD + 1];
	int start[4];	//a c g t 이 bwt 테이블 1열에서 최초로 시작되는 행(인덱스)
	makeStart(cnt, start, 5);


	for (int i = 0; i < NUM_OF_SHORTREAD; ++i) {
		bool next = false;		
		int mismatch = 0;		
		int move = 1;			//shortRead 뒤에서부터 차례대로 검사하기 위해 사용
		int check_count = 0;
		int s_row;

		//
		//puts("");
		for (int i = 0; i < LEN_OF_SHORTREAD; ++i) {
			shortRead[i] = fgetc(fp);
		}
		fgetc(fp); // 개행문자 처리
		shortRead[LEN_OF_SHORTREAD] = '\0';
		
		///cout << shortRead << endl;

		char lastChar = shortRead[LEN_OF_SHORTREAD - 1];
		if (lastChar == 'A') s_row = start[0];
		if (lastChar == 'C') s_row = start[1];
		if (lastChar == 'G') s_row = start[2];
		if (lastChar == 'T') s_row = start[3];

		int row = s_row;
		vector<char> snips;		//스닙 문자 저장
		vector<int> locs;		//스닙 위치 저장

		//읽어온 shortRead와 bwt대조 시작
		while (true){
			move++;
			check_count++;
			bool next_while = false;


			//bwt 다음문자 찾기
			char back = table[row].back;
			///cout << back;
			int rank2 = table[row].rank2;
			if (back == 'A') row = start[0] + rank2 - 1;
			if (back == 'C') row = start[1] + rank2 - 1;
			if (back == 'G') row = start[2] + rank2 - 1;
			if (back == 'T') row = start[3] + rank2 - 1;


			//상황
			// $ 만남
			// $ 이 아님  -> mismatch 발생
			//			  -> mismatch 발생안함

			//$를 만나면 bwt다음 행부터 새로 비교 시작해야 한다.
			if (back == '$') {
				next_while = true;
				mismatch = 0;
				check_count = 0;
				move = 1;
				s_row++;
				row = s_row;
				snips.clear();
				locs.clear();

				//A,C,G,T영역을 다 돌았다면 다음 행이 아닌 새로운 숏리드를 받아야한다.
				if (lastChar == 'A') {
					if (s_row - start[0] == cnt[1]) {
						next = true;
						break;	//break while 그리고 다음 숏리드 받도록
					}
				}
				if (lastChar == 'C') {
					if (s_row - start[1] == cnt[2]) {
						next = true;
						break;	//break while 그리고 다음 숏리드 받도록
					}
				}
				if (lastChar == 'G') {
					if (s_row - start[2] == cnt[3]) {
						next = true;
						break;	//break while 그리고 다음 숏리드 받도록
					}
				}
				if (lastChar == 'T') {
					if (s_row - start[3] == cnt[4]) {
						next = true;
						break;	//break while 그리고 다음 숏리드 받도록
					}
				}
			}//
			//mismatch 발생
			else if (back != shortRead[LEN_OF_SHORTREAD - move]) {
				mismatch++;

				//mistmatch허용 숫자를 넘겼을 떄
				if (mismatch > threshold) {
					//bwt 다음행부터 새로 비교 시작해야 한다.
					next_while = true;	//while 문 돌게 하는 flag. 
					mismatch = 0;
					move = 1;
					check_count = 0;
					s_row++;
					row = s_row;
					snips.clear();
					locs.clear();

					//만약 A,C,G,T 각 영역 끝까지 돌았다면 다음 행을 받으면 안되고, 새로운 숏리드를 받아야 한다.
					if (lastChar == 'A') {
						if (s_row - start[0] >= cnt[1]) {
							next = true;	//새로운 숏리드를 받게하는 flag
							break;	//break while 그리고 다음 숏리드 받도록
						}
					}
					if (lastChar == 'C') {
						if (s_row - start[1] >= cnt[2]) {
							next = true;
							break;	//break while 그리고 다음 숏리드 받도록
						}
					}
					if (lastChar == 'G') {
						if (s_row - start[2] >= cnt[3]) {
							next = true;
							break;	//break while 그리고 다음 숏리드 받도록
						}
					}
					if (lastChar == 'T') {
						if (s_row - start[3] >= cnt[4]) {
							next = true;
							break;	//break while 그리고 다음 숏리드 받도록
						}
					}
					
				}
				if (next_while == true) {
					///puts("");
					continue;
					
				}
				//threshold 아직 넘기지 않았다면 스닙 위치저장
				int rank1  = table[row].rank1;
				snips.push_back(shortRead[LEN_OF_SHORTREAD - move]);
				///char tempccc = shortRead[LEN_OF_SHORTREAD - move];
				locs.push_back(rank1);						


			}
			//mismatch 발생하지 않으면 다음 문자 계속 비교해 나간다.
	
			//-1 하는 이유는 while문 들어오기 전에 이미 한번 비교가 되었기 때문에
			if (check_count == LEN_OF_SHORTREAD - 1) {
				break;
			}
		}//읽어온 shortRead와 bwt 표 비교하는 while문 끝

		//match 성공
		if (next == false) {

			//mismatch가 있었다면
			if (!snips.empty()) {
				while (!snips.empty()) {
					char snip = snips.front();
					int  rank1 = locs.front();
					snips.pop_back();
					locs.pop_back();
					reference[rank1] = snip;	// reference의 snip을 바꿔준다. 
				}
			}
			
		}
		
		//match하지 못한 경우(next ==true)엔 새로운 숏리드를 받는다.
	} //새로운 숏리드 받는 반복문 끝
	fclose(fp);
}
