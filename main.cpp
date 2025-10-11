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

bool filter(idx_t id, float* value, float* filter, int vDim, int queryId) {
    float* label = &value[vDim * id];

    for (int j = 0; j < vDim; j++) {
        if (label[j] < filter[2 * vDim * queryId + 2 * j] || label[j] > filter[2 * vDim * queryId + 2 * j + 1])
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
    float *filters = new float[queryNum * 2 * vDim];
    read_fvecs(filterFilename, filters, queryNum, 2 * vDim);
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
        int dis_calculation = 0;
        for(int i = 0 ; i < queryNum; i++){
            auto ans = dataMaker.getGt(i);
            float rangeL = filters[2 * vDim * i], rangeR = filters[2 * vDim * i + 1];
            auto query_start = std::chrono::high_resolution_clock::now();
            auto neighbors = rangeHnsw.queryRange(dataMaker.query + i * dim, rangeL, rangeR, new_k, ef, dis_calculation);
            auto query_end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed_query = query_end - query_start;
            time += elapsed_query.count();

	        std::vector<idx_t> result;
            std::vector<std::pair<float, idx_t>> result_tmp;
            
            while (!neighbors.empty()) {
                idx_t neighbor = neighbors.top().second;
                float distance = neighbors.top().first;
                neighbors.pop();
                
                result_tmp.push_back(std::make_pair(distance, neighbor));
            }

            std::sort(result_tmp.begin(), result_tmp.end());

            auto filter_start = std::chrono::high_resolution_clock::now();
            for (auto p : result_tmp) {
                if (filter(p.second, dataMaker.value, filters, vDim, i)) {
                    result.push_back(p.second);
                    if (result.size() >= k)
                        break;
                }
            }
            auto filter_end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed_filter = filter_end - filter_start;
            time += elapsed_filter.count();
            
            std::vector<int>r1, r2;
            for (auto r : result) {
                r1.push_back(r);
            }
            for(int j = 0; j < k; j++) r2.push_back(ans[j].second);
            for(auto i1:r1)
                for(auto i2:r2)
                    if(i1 == i2)
                        recall+=1.0/k;
        }
        std::cout<<"ef:"<<ef<<std::endl;
        std::cout<<"recall:"<<recall/queryNum<<std::endl;
        std::cout<<"time:"<<time<<std::endl;
        std::cout<<"qps:"<<queryNum/time<<std::endl;
        std::cout<<"number of distance calculation:"<<dis_calculation<<std::endl;
       }
    // }

    return 0;
}
