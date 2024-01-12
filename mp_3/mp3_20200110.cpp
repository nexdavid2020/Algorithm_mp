#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <cstring> 
#include <math.h>
#include <queue>
#include <vector>
#include <unordered_map>
#include <bitset>
using namespace std; 

// 알고리즘 세 번째 과제!
// 20200110 정태현

int count_arr[129] = {0,};

struct HuffmanNode {
    int symbol; // char 대신 int 사용
    int freq;
    HuffmanNode* left;
    HuffmanNode* right;

    // 생성자!
    HuffmanNode(int symbol, int freq) : symbol(symbol), freq(freq), left(nullptr), right(nullptr) {}
};

struct Compare {
    bool operator()(HuffmanNode* a, HuffmanNode* b) {
        return a->freq > b->freq;
    }
};

// 아스키코드별 Codeword 만드는 재귀 함수!
void generateCode(HuffmanNode* root, string str, unordered_map<int, string>& huffmanCode) {
    if (root == nullptr) {
        return;
    }

    // 리프 노드에 도달했을 때
    if (root->left == nullptr && root->right == nullptr) {
        huffmanCode[root->symbol] = str;
    }

    generateCode(root->left, str + "0", huffmanCode);
    generateCode(root->right, str + "1", huffmanCode);
}

int main(int argc, char *argv[]){

    // 인자 제대로 넣었는지 확인하기
    if(argc != 3){
        fprintf(stderr, "사용법: %s <-c or -d> <인풋파일>\n", argv[0]); // 프로그램 이름과 함께 사용 예시 반환
        return 1; // 오류 코드 반환
    }

    if(strcmp(argv[1], "-c") == 0){ // -c인 경우

        // input 파일 열기
        FILE* inputfile = fopen(argv[2], "r");
        if(inputfile == NULL){
            fprintf(stderr, "input file open Error!\n");
            return 1;
        }

        // 각 character 의 빈도수를 체크하고 저장하기!!
        int ch;
        while ((ch = fgetc(inputfile)) != EOF) {
            count_arr[ch]++;
        }

        count_arr[128] = -1;  // 128번 인덱스는 EOF_SYMBOL!! -> 제일 긴 codeword 할당 받도록!

        fclose(inputfile);

        // 우선순위 큐 생성
        priority_queue <HuffmanNode*, vector<HuffmanNode*>, Compare> pq;

        for (int i = 0; i < 129; i++) {
            if (count_arr[i] > 0 || count_arr[i] == -1) {
                pq.push(new HuffmanNode(i, count_arr[i]));
            }
        }

        while (pq.size() > 1) {
            HuffmanNode *left = pq.top(); 
            pq.pop();
            HuffmanNode *right = pq.top(); 
            pq.pop();

            HuffmanNode *node = new HuffmanNode(-1, left->freq + right->freq); // 중간 노드들은 심볼에 -1!
            node->left = left;
            node->right = right;

            pq.push(node);
        }

        // 허프만 트리의 루트는 이제 pq.top()에 있음
        HuffmanNode *root = pq.top();

        // 여기에 Map을 선언하고 저 트리에서 하나씩 뽑아 prefix free codeword를 저장!

        // 예를 들어 a의 빈도가 제일 많다면 -> a가 root에 있을 거임! 그럼 얘는 a : 0이고
        // 왼쪽 자식은 0, 오른쪽 자식은 1이라고 잡고
        // 순회하면서 각 단말 노드별로 codeword를 저장!! 

        unordered_map<int, string> huffmanCode;
        generateCode(root, "", huffmanCode);

        // // 허프만 코드 출력 (테스트 용)
        // for (auto pair : huffmanCode) {
        //     if (pair.first >= 0 || pair.first == -1) {
        //         cout << pair.first << " (" << (char)pair.first << ") : " << pair.second << endl;
        //     }
        // }


        // input 파일 다시 열기
        inputfile = fopen(argv[2], "r");
        if (inputfile == NULL) {
            fprintf(stderr, "input file open Error!\n");
            return 1;
        }

        string outpub_file_name = argv[2];  // output파일 이름 설정
        outpub_file_name += ".zz";

        // Output file 스트림 열기
        FILE* outputfile = fopen(outpub_file_name.c_str(), "wb");
        if (outputfile == NULL) {
            fprintf(stderr, "output file open Error!\n");
            return 1;
        }

        // 매핑 정보 쓰기  key:value 로 입력하기!
        for (auto pair : huffmanCode) {
            string mapping = to_string(pair.first) + ":" + pair.second + "\n";
            fwrite(mapping.c_str(), sizeof(char), mapping.size(), outputfile);
        }

        // 매핑 정보의 끝을 나타내는 표시
        const char* mapping_end = "END\n";
        fwrite(mapping_end, sizeof(char), strlen(mapping_end), outputfile);

        unsigned char buffer = 0; // 8비트 버퍼
        int bitCount = 0; // 현재 버퍼에 저장된 비트 수
        
        while ((ch = fgetc(inputfile)) != EOF) {
            string code = huffmanCode[ch];

            for (char bit : code) {
                buffer = buffer << 1; 
                buffer = buffer | (bit - '0'); // 비트 추가

                bitCount++;
                if (bitCount == 8) { // 버퍼가 가득 찼을 때

                    fwrite(&buffer, 1, 1, outputfile); // 버퍼를 파일에 쓰기
                    buffer = 0; // 버퍼 초기화
                    bitCount = 0;
                }
            }
        }

        if(bitCount == 0){ // EOF_SYMBOL만 넣어주면 되는 경우!
            string code = huffmanCode[128]; // EOF_SYMBOL의 코드워드 가져오기
            
            if(code.size() <= 8){

                for (char bit : code) {
                    buffer = buffer << 1; 
                    buffer = buffer | (bit - '0'); // 비트 추가
                    bitCount++;
                }

                if(bitCount == 8){
                    fwrite(&buffer, 1, 1, outputfile); // 버퍼를 파일에 쓰기
                    buffer = 0; // 버퍼 초기화
                    bitCount = 0;
                }else{
                    buffer = buffer << (8 - bitCount); // 나머지 비트를 왼쪽으로 이동
                    fwrite(&buffer, 1, 1, outputfile); // 버퍼를 파일에 쓰기
                    buffer = 0; 
                    bitCount = 0;
                }

            }else{ // 8 이상이라면
                for (char bit : code) {
                    buffer = buffer << 1; 
                    buffer = buffer | (bit - '0'); // 비트 추가
                    bitCount++;
                    if (bitCount == 8) { // 버퍼가 가득 찼을 때
                        fwrite(&buffer, 1, 1, outputfile); // 버퍼를 파일에 쓰기
                        buffer = 0; // 버퍼 초기화
                        bitCount = 0;
                    }
                }
            }
            
        }else if(bitCount > 0){
            string code = huffmanCode[128]; // EOF_SYMBOL의 코드워드 가져오기
            for (char bit : code) {
                buffer = buffer << 1; 
                buffer = buffer | (bit - '0'); // 비트 추가
                bitCount++;
                if (bitCount == 8) { // 버퍼가 가득 찼을 때

                    fwrite(&buffer, 1, 1, outputfile); // 버퍼를 파일에 쓰기
                    buffer = 0; // 버퍼 초기화
                    bitCount = 0;
                }
            }
        }

        // 마지막 바이트 처리 (패딩 추가)
        if (bitCount > 0) {
            buffer = buffer << (8 - bitCount); // 나머지 비트를 왼쪽으로 이동
            fwrite(&buffer, 1, 1, outputfile); // 버퍼를 파일에 쓰기
        }

        fclose(inputfile);
        fclose(outputfile);

    }else if(strcmp(argv[1], "-d") == 0){  // -d인 경우

        // codeword:아스키 쌍을 담을 맵 생성
        unordered_map<string, int> mp;

        // input 파일 열기
        FILE* inputfile = fopen(argv[2], "r");
        if(inputfile == NULL){
            fprintf(stderr, "input file open Error!\n");
            return 1;
        }

        char line[256];
        long mapping_end_position = 0; // 매핑 정보 끝 위치를 저장할 변수

        while (fgets(line, sizeof(line), inputfile)) {
            string str = line;

            // END 문자열을 만나면 반복 종료
            if (str.substr(0, 3) == "END") {
                mapping_end_position = ftell(inputfile);
                break;
            }

            // ':'를 기준으로 문자열 분리
            size_t pos = str.find(':'); 
            if (pos != string::npos) {
                string key = str.substr(pos + 1);
                key.pop_back(); // 줄바꿈 문자 제거
                char value = static_cast<char>(stoi(str.substr(0, pos)));
                mp[key] = value;
            }
        }

        fclose(inputfile);

        // // 허프만 코드 출력 (테스트 용)
        // for (auto pair : mp) {
        //     cout << pair.first << " " << pair.second << "\n";
        // }


        // 바이너리 모드로 파일 열기
        FILE* binfile = fopen(argv[2], "rb");
        if (binfile == NULL) {
            fprintf(stderr, "binary file open Error!\n");
            return 1;
        }

        // 파일 포인터를 매핑 정보 끝으로 이동
        fseek(binfile, mapping_end_position, SEEK_SET);

        string outpub_file_name_2 = argv[2];  // output파일 이름 설정
        outpub_file_name_2 += ".yy";

        // 출력 파일 열기
        FILE* outfile = fopen(outpub_file_name_2.c_str(), "w");
        if (outfile == NULL) {
            fprintf(stderr, "output file open Error!\n");
            fclose(binfile);
            return 1;
        }

        unsigned char buffer;
        string currentBits;
        bool is_EOF_SYMBOL = false;

        while (fread(&buffer, 1, 1, binfile)) {
            bitset<8> bits(buffer);
            for (int i = 7; i >= 0; --i) {   // bitset에서 7부터 읽어야 함!
                currentBits += (bits[i] ? '1' : '0');
                
                if (mp.find(currentBits) != mp.end()) {

                    if(mp[currentBits] == -128){
                        is_EOF_SYMBOL = true;
                        break;
                    }

                    if(is_EOF_SYMBOL == false){
                        fputc(mp[currentBits], outfile);
                        currentBits.clear();
                    }

                }
            }

            if(is_EOF_SYMBOL == true){
                break;
            }
        }

        fclose(binfile);
        fclose(outfile);

    }

    return 0;
}