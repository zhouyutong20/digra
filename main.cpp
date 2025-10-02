#include <iostream>
#include "hnswlib/hnswlib.h"
#include "DataMaker.hpp"
#include "TreeHNSW.hpp"

#include <chrono>
#include <string>

#include <fstream>
#include <sstream>

#include <sys/resource.h>

// Add
#include <cstdint>
using idx_t = uint32_t;

int stringTonum(char *ch){
    int len = strlen(ch);
    int res = 0;
    for(int i = 0; i< len; i++){
        res = res * 10 + ch[i] - '0';
    }
    return res;
}

inline
void read_fvecs(const std::string& filename, float* data, size_t num, size_t dim) {
    std::ifstream in(filename, std::ios::binary);
    if (!in) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        std::exit(1);
    }
    for (size_t i = 0; i < num; ++i) {
        int d = 0;
        in.read(reinterpret_cast<char*>(&d), sizeof(int));
        if (d != static_cast<int>(dim)) {
            // std::cerr << "Dimension mismatch! expected: d" << d << "get: " << dim << "for: " << filename << std::endl;
            d = dim;
        }
        in.read(reinterpret_cast<char*>(data + i * dim), sizeof(float) * dim);
    }
}

bool filter(idx_t id, float* value, float* filter, int vDim, int queryId) {
    float* label = &value[vDim * id];

    for (int j = 0; j < vDim; j++) {
        if (label[j] < filter[2 * vDim * queryId + j] || label[j] > filter[2 * vDim * queryId + j + 1])
            return false;
    }

    return true;
}

int main(int argc, char** argv){
    puts("read begin");
    int baseNum =stringTonum(argv[1]);
    int queryNum =stringTonum(argv[2]);
    int dim =stringTonum(argv[3]);
    int k =stringTonum(argv[4]);
    int ef_con = stringTonum(argv[5]);
    int m =stringTonum(argv[6]);
    char *baseFilename = argv[7];
    char *queryFilename = argv[8];
    char *dataFilename = argv[9];
    // Add filter
    char *filterFilename = argv[10];
    // Add value dim
    int vDim = stringTonum(argv[11]);
    DataMaker dataMaker(baseFilename, queryFilename, dataFilename, baseNum, queryNum, dim, vDim);

    auto start = std::chrono::high_resolution_clock::now();
    RangeHNSW rangeHnsw(dim,baseNum,baseNum,dataMaker.data,dataMaker.key,dataMaker.valueList, m , ef_con);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout<<"build time:" << elapsed.count() <<std::endl;
    puts("buildOK");

    // Add filter
    float *filters = new float[queryNum * 2];
    read_fvecs(filterFilename, filters, queryNum, 2);
    // std::vector<float> r = {0.01,0.05,0.1,0.2,0.4};
    // for(auto range:r){
    // Prepare for post filter
    int ratio = 4;     // sift 12, tiny 10
    int new_k = ratio * k;
    dataMaker.genRange(filters, k);
    // std::cout<<"----------"<<range<<"--------"<<std::endl;
    for(int ef = 200; ef <= 500; ef += 100){
        float recall = 0;

        float time = 0;
        for(int i = 0 ; i < queryNum; i++){
            auto ans = dataMaker.getGt(i);
                
            // filter L, R
            float rangeL = filters[vDim * i], rangeR = filters[vDim * i + 1];
            auto start = std::chrono::high_resolution_clock::now();
	    // Modified
            // auto result = rangeHnsw.queryRange(dataMaker.query + i * dim, rangeL, rangeR, k, ef);
            auto neighbors = rangeHnsw.queryRange(dataMaker.query + i * dim, rangeL, rangeR, new_k, ef);
	    std::vector<idx_t> result;
            // Modified post filter
	    /*
	    for (auto neighbor : neighbors) {
                if (filter(neighbor, dataMaker.value, filter, vDim, i)) {
                    res.push_back(neighbor);
                    if (result.size() >= k)
                    break;
                }
            }
	    */
	    while (!neighbors.empty()) {
                idx_t neighbor = neighbors.top().second;
                neighbors.pop();

                if (filter(neighbor, dataMaker.value, filters, vDim, i)) {
                    result.push_back(neighbor);
                    if (result.size() >= k)
                        break;
                }
            }

            auto end = std::chrono::high_resolution_clock::now();
            
            std::vector<int>r1, r2;
            while(!result.empty()){
		// Modified
                // r1.push_back(result.top().second);
                // result.pop();
		r1 = std::vector<int>(result.begin(), result.end());
            }
            for(int j = 0; j < k; j++) r2.push_back(ans[j].second);
            for(auto i1:r1)
                for(auto i2:r2)
                    if(i1 == i2)
                        recall+=1.0/k;
            std::chrono::duration<double> elapsed = end - start;
            time += elapsed.count();
        }
        std::cout<<"ef:"<<ef<<std::endl;
        std::cout<<"recall:"<<recall/queryNum<<std::endl;
        std::cout<<"time:"<<time<<std::endl;
        std::cout<<"qps:"<<queryNum/time<<std::endl;
       }
    // }

    return 0;
}
