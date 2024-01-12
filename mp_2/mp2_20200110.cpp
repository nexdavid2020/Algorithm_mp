#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <string> 
#include <math.h>
using namespace std; 

// 알고리즘 두 번째 과제!
// 20200110 정태현

// 1. 버블 정렬
void bubbleSort(int* arr, int arrSize){
    int temp;
    for(int i=0; i<arrSize-1; i++){
        for(int j=0; j<arrSize-1-i; j++){
            if(arr[j] > arr[j+1]){
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1]= temp;
            }
        }
    }
}

// 2. 퀵 정렬
int partition(int* list, int left, int right){

    int temp;
    int x = list[right]; // 맨 오른쪽 값이 피벗
    int i = left - 1;
    for(int j=left; j<=right-1; j++){
        if(list[j] <= x){
            i = i+1;
            temp = list[i];
            list[i] = list[j];
            list[j] = temp;
        }
    }

    // 맨 오른쪽에 있었던 피벗을 i+1위치로 옮겨주기!
    temp = list[i+1];
    list[i+1] = list[right];
    list[right] = temp;

    return i+1;  // 피벗의 idx 반환
}

void quickSort(int* arr, int left, int right){
    int q;
    if(left < right){
        q = partition(arr, left, right);  // 여기서 반환되는 것은 피벗의 idx고 이 q위치는 정렬된 것!
        quickSort(arr, left, q-1);
        quickSort(arr, q+1, right);
    }
}


// 3. 병합정렬
void Merge(int* arr, int left, int mid, int right){

    int* sortedArr = new int[right-left+1];  // 시작인덱스 끝인덱스까지 모두 담을 만큼 동적할당 하기!
    int i = left;
    int j = mid + 1;
    int soIdx = 0;

    // 작은 순서대로 sortedArr에 담기!!
    while(i <= mid && j <= right){
        if(arr[i] <= arr[j]){
            sortedArr[soIdx++] = arr[i++]; 
        }else{
            sortedArr[soIdx++] = arr[j++];
        }
    }

    // 오른쪽 부분이 아직 안 담겼을 경우 나머지 쭉 담기
    if(i > mid){
        for(int l=j; l<=right; l++){
            sortedArr[soIdx++] = arr[l];
        }
    }

    // 왼쪽 부분이 아직 다 안 담겼을 경우 나머지 쭈욱 담기
    if(j > right){
        for(int l=i; l<=mid; l++){
            sortedArr[soIdx++] = arr[l];
        }
    }

    // 이제 sortedArr을 다시 arr에 덮어쓰기!
    int arrIdx = left;
    for(int i=0; i<right-left+1; i++){
        arr[arrIdx++] = sortedArr[i];
    }
    
    delete[] sortedArr; // 동적할당 해제
}

void mergeSort(int* arr, int left, int right){
    
    if(left >= right){ // base case
        return;
    }

    int mid = (left + right) / 2;
    mergeSort(arr, left, mid); // 여기까지 하면 left부터 mid까지는 정렬된 것
    mergeSort(arr, mid+1, right); // 여기까지 하면 mid+1부터 right까지 정렬된 것
    Merge(arr, left, mid, right); // 이제 정렬된 두 부분을 합치기!
}

// 4. 나만의 정렬 -> 제일 빠르게!!
// 내성적 정렬 - quick_sort, heap_sort, insertion_sort 를 섞은 정렬 방식! - 평균적인 경우로도 NlogN, 최악의 경우로도 NlogN의 시간복잡도

// insertion_sort
void insertion_sort(int* arr, int n){
    int i, j, key;

    for(i = 1; i < n; i++){
        key = arr[i];

        j = i-1;
        while(j>=0 && arr[j] > key){
            arr[j+1] = arr[j];
            j = j-1;
        }

        arr[j+1] = key;
        
    }
}

// heap_sort 의 Inner Sort인 heapify 구현하기
void heapify(int* arr, int n, int i) {
    int largest = i;      // 루트인 i를 largest에 초기화 하기! 
    int left = 2 * i + 1;   // 그 루트의 왼쪽 자식 인덱스 확보
    int right = 2 * i + 2;  // 그 루트의 오른쪽 자식 인덱스 확보!

    if(left < n && arr[left] > arr[largest]){
        largest = left;
    }

    if(right < n && arr[right] > arr[largest]){
        largest = right;
    }

    // 여기까지 마치면 largeset에 정말 제일 큰 largest 수가 담겨 있음!!

    if (largest != i) { // 처음에 largest를 i를 넣어줬는데 이게 largest랑 다르다면 자식 중 더 큰놈이 있었다는 것이니 바꿔줘야하고!
        int swap = arr[i];
        arr[i] = arr[largest];
        arr[largest] = swap;
        // 자식이랑 바뀐 그 largest 부분을 또 루트 노드로 삼고 그 자식들과 Max heap을 유지하는지 체크해야함! 이를 재귀로~
        heapify(arr, n, largest);
    }
}

// HeapSort
void heapSort(int* arr, int n) {
    // 1차 초벌하기!
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    // 이제 맨 앞에 있는 숫자랑(제일 큰 수)랑 맨 뒤에 숫자랑 바꾼 다음 다시 heapify()실행! -> 정렬 됨!
    for (int i = n-1; i > 0; i--) {
        // Move current root to end
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;

        // call max heapify on the reduced heap
        heapify(arr, i, 0);
    }
}

// quick_sort
int get_middle(int* arr, int left, int mid, int right) {  // 세 인덱스가 가르키는 값 중 중간값을 가르키는 인덱스 반환!

    if(arr[left] > arr[mid]){
        if (arr[left] <= arr[right]) {
            return left;
        } else if (arr[mid] <= arr[right]) {
            return right; 
        } else {
            return mid; 
        }
    }else {
        if (arr[mid] <= arr[right]) {
            return mid;
        } else if (arr[left] <= arr[right]) {
            return right; 
        } else {
            return left; 
        }
    }
}

int myPartition(int* list, int left, int right){

    // Partition 나눌 때 성능을 향상시키기 위해 pivot Idx를 맨 왼쪽, 맨 오른쪽 중간 idx중 중간 값을 가르키는 Idx가 pivot이 되도록!
    int temp;
    int mid = (left + right) / 2;
    int pivot_idx = get_middle(list, left, mid, right);
    
    // 그 pivot_idx랑 맨 오른쪽이랑 다르면 바꿔주기!
    if(pivot_idx != right){
        temp = list[pivot_idx];
        list[pivot_idx] = list[right];
        list[right] = temp;
    }

    int x = list[right]; // 맨 오른쪽 값이 피벗
    int i = left - 1;
    for(int j=left; j<=right-1; j++){
        if(list[j] <= x){
            i = i+1;
            temp = list[i];
            list[i] = list[j];
            list[j] = temp;
        }
    }

    // exchange A[j] with A[j]
    temp = list[i+1];
    list[i+1] = list[right];
    list[right] = temp;

    return i+1;  // return index of pivot
}

void introQuickSort(int* arr, int left, int right, int depthLimit){

    if(right - left < 16){ // 임계값 설정-> 부분 배열의 길이가 16이하이면 insertionSort 실행하도록!
        insertion_sort(arr + left, right-left+1); // 삽입 정렬 호출
    }else if(depthLimit == 0){ // 깊이 한계에 도달하면 힙 정렬 호출
        heapSort(arr + left, right - left + 1);
    }else{
        int q = myPartition(arr, left, right); // 피벗을 기준으로 분할
        introQuickSort(arr, left, q-1, depthLimit-1);
        introQuickSort(arr, q+1, right, depthLimit-1);
    }
}

void introSort(int* arr, int n){
    // 깊이의 제한 계산하고
    int depthLimit = 2 * log2(n); // 로그 깊이(레벨)의 2배를 힙 정렬의 depthLimit으로 설 ㅈ어
    // IntroQuickSort 실행!!
    introQuickSort(arr, 0, n-1, depthLimit);
}

void myOwnSort(int* arr, int arrSize){ 
    // 여기서 intro 실행하고!
    introSort(arr, arrSize);
}

// MAIN 함수!
int main(int argc, char *argv[]){
    
    clock_t start_time, end_time;
    
    // 인자 제대로 넣었는지 확인하기
    if(argc != 3){
        fprintf(stderr, "사용법: %s <입력 파일> <숫자>\n", argv[0]); // 프로그램 이름과 함께 사용 예시 반환
        return 1; // 오류 코드 반환
    }

    // input 파일 열기
    FILE* inputfile = fopen(argv[1], "r");
    if(inputfile == NULL){
        fprintf(stderr, "input file open Error!\n");
        return 1;
    }

    int algoIdx = atoi(argv[2]); // argv[2] 문자열을 정수로 변환

    // output 파일 이름 만들기!
    string output_file_name = "result_";
    output_file_name += argv[2];
    output_file_name += "_";
    output_file_name += argv[1];

    // output파일 열기!
    FILE* outputfile = fopen(output_file_name.c_str(), "w");
    if(outputfile == NULL){
        fprintf(stderr, "output file open Error!\n");
        fclose(inputfile);
        return 1;
    }

    int arrSize;
    if(fscanf(inputfile, "%d", &arrSize) != 1){
        fprintf(stderr, "입력 파일에서 첫 숫자 읽는데 실패함\n");
        fclose(inputfile);
        fclose(outputfile);
        return 1;
    }

    // 정렬해야할 숫자 담는 배열 동적할당으로 선언!
    int* arr = new int[arrSize];

    // input 파일에서 나머지 숫자들 다 긁어오기
    for(int i=0; i<arrSize; i++){
        if(fscanf(inputfile, "%d", &arr[i]) != 1){
            delete[] arr;
            fclose(inputfile);
            fclose(outputfile);
            return 1;
        }
    }

    // 여기서부터 시간 start
    start_time = clock();
    
    if(algoIdx == 1){ // 삽입정렬 or 버블정렬 or 선택정렬
        bubbleSort(arr, arrSize);
    }else if(algoIdx == 2){ // 퀵 정렬
        quickSort(arr, 0, arrSize-1);
    }else if(algoIdx == 3){ // 병합정렬
        mergeSort(arr, 0, arrSize-1);
    }else if(algoIdx == 4){ // 나만의 최적화된 정렬 방식
        myOwnSort(arr, arrSize);
    }

    // 여기서 시간 체크 종료
    end_time = clock();

    /* output 파일에 쓰기! */ 
    // (1) input파일 이름 쓰기
    fprintf(outputfile, "%s\n", argv[1]);
    // (2) 알고리즘 인덱스 쓰기
    fprintf(outputfile, "%d\n", algoIdx);
    // (3) Input size(the number of elements the original list) 쓰기
    fprintf(outputfile, "%d\n", arrSize);
    // (4) 러닝타임 쓰기!  
    fprintf(outputfile, "%.6f\n", (double)(end_time-start_time)/CLOCKS_PER_SEC);
    // (5) the sorted list 쓰기
    for(int i=0; i < arrSize; i++){
        fprintf(outputfile, "%d ", arr[i]);
    }

    // 파일 스트림 닫기! & 동적 할당 해제
    fclose(inputfile);
    fclose(outputfile);
    delete[] arr;

    return 0;
}