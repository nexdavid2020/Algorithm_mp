#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <ctime> 
using namespace std; 

#define max_row 100
#define max_col 100

// 알고리즘설계와분석 첫 번째 과제
// 20200110 정태현

// O(N^6)
int function_1(int arr[max_row][max_col], int row, int col){
    int ThisSum, MaxSum = -20000;
    
    int i_1, i_2, j_1, j_2, k_i, k_j;

    for(i_1 = 0; i_1 < row; i_1++){
        for(j_1 = 0; j_1 < col; j_1++){
            for(i_2 = i_1; i_2 < row; i_2++){
                for(j_2 = j_1; j_2 < col; j_2++){

                    ThisSum = 0;
                    for(k_i = i_1; k_i <= i_2; k_i++){
                        for(k_j = j_1; k_j <= j_2; k_j++){
                            ThisSum += arr[k_i][k_j];
                        }
                    }

                    if(ThisSum > MaxSum){
                        MaxSum = ThisSum;
                    }
                }
            }
        }
    }
    
    return MaxSum;
}

// O(N^4)
int function_2(int arr[max_row][max_col], int row, int col){

    int nuSum[max_row + 1][max_col + 1] = {{0}};  // 2차원 배열 누적 합 구하기!
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            nuSum[i+1][j+1] = nuSum[i+1][j] + nuSum[i][j+1] - nuSum[i][j] + arr[i][j];
        }
    }

    int ThisSum, MaxSum = -20000;
    int i_1, i_2, j_1, j_2;

    for(i_1 = 0; i_1 < row; i_1++){
        for(j_1 = 0; j_1 < col; j_1++){
            for(i_2 = i_1; i_2 < row; i_2++){
                for(j_2 = j_1; j_2 < col; j_2++){
                    
                    ThisSum = nuSum[i_2 + 1][j_2 + 1] - nuSum[i_1][j_2 + 1] - nuSum[i_2 + 1][j_1] + nuSum[i_1][j_1];

                    if(ThisSum > MaxSum){
                        MaxSum = ThisSum;
                    }
                }
            }
        }
    }
    
    return MaxSum;
}

// O(N^3)
int function_3(int arr[max_row][max_col], int row, int col){

    int MaxSum = -20000;
    int* tmp = new int[col];

    int i_1, i, i_2, j, j_1, ThisSum;

    for(i_1 = 0; i_1 < row; i_1++){
        
        for(i=0; i < col; i++){
            tmp[i] = 0; // tmp 배열 0으로 초기화
        }

        for(i_2 = i_1; i_2 < row; i_2++){

            for(j = 0; j < col; j++){
                tmp[j] += arr[i_2][j]; 
            }

            ThisSum = 0;
            for(j_1 = 0; j_1 < col; j_1++){
                ThisSum += tmp[j_1];

                if(ThisSum > MaxSum){
                    MaxSum = ThisSum;
                }

                if(ThisSum < 0){  // ThisSum이 음수이면 0으로 초기화!!
                    ThisSum = 0;
                }
            }
        }
    }

    delete[] tmp; // 동적할당한 tmp 해제 하기~ 
    
    return MaxSum;
}


int main(int argc, char *argv[]){

    int arr[max_row][max_col];
    int row, col;
    int algorithm_index;
    int final_result = 0;  // 초기값 그냥 0으로 지정
    clock_t start_time, end_time;

    if(argc != 3){
        fprintf(stderr, "usage: mp1_xxxxxx input00001.txt algorithm_index \n"); // 사용 예시 반환해주기
    }

    FILE* inputfile = fopen(argv[1], "r");
    if(inputfile == NULL){
        perror("input file open Error!\n");
        return 1;
    }

    algorithm_index = atoi(argv[2]); // 알고리즘 인덱스 저장
    
    string output_file_name = "result_";
    output_file_name += argv[1];
    // output_file_name += ".txt";

    FILE* outputfile  = fopen(output_file_name.c_str(), "w");
    if(outputfile == NULL){
        perror("output file open Error!\n");
        fclose(inputfile);  // 앞서 열려있을 inputfile 스트림 지우기!
        return 1;
    }


    // <여기서부터 프로그램 시작!>
    start_time = clock();
    
    if(fscanf(inputfile, "%d %d", &row, &col) != 2){  // input 파일의 첫 줄의 두개 가져오기!
        fprintf(stderr, "Error!\n");
        fclose(inputfile);
        fclose(outputfile);
        exit(1);
    }

    // 나머지 줄 읽으면서 배열에 담기!
    for(int i=0; i<row; i++){
        for(int j=0; j<col; j++){
            fscanf(inputfile, "%d", &arr[i][j]);
        }
    }

    // 알고리즘 인덱스에 따라 다른 함수 실행하기!

    if(algorithm_index == 1){ 
        // O(N^6)
        final_result = function_1(arr, row, col);

    }else if(algorithm_index == 2){
        // O(N^4)
        final_result = function_2(arr, row, col);

    }else if(algorithm_index == 3){
        // O(N^3)
        final_result = function_3(arr, row, col);
    }

    // 여기까지 프로그램 종료 -> end_time 뽑아내기
    end_time = clock();

    /* output 파일에 쓰기! */ 

    // (1) input파일 이름 쓰기
    fprintf(outputfile, "%s\n", output_file_name.c_str());

    // (2) 알고리즘 인덱스 쓰기
    fprintf(outputfile, "%d\n", algorithm_index);

    // (3) 2차원 배열의 row 쓰기
    fprintf(outputfile, "%d\n", row);

    // (4) 2차원 배열의 col 쓰기
    fprintf(outputfile, "%d\n", col);

    // (5) sum of the maximum sum subrectangle 쓰기 -> final_result 변수 값
    fprintf(outputfile, "%d\n", final_result);

    // (6) running time 쓰기 
    // fprintf(outputfile, "%lf\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);
    fprintf(outputfile, "%ld\n", (end_time - start_time));

    // 파일 스트림 모두 해제하기!!!
    fclose(inputfile);
    fclose(outputfile);

    return 0;
}