#ifndef RANGEHNSW_DATAMAKER_HPP
#define RANGEHNSW_DATAMAKER_HPP

#include<random>
#include<vector>

#include "utils.hpp"

class DataMaker {
public:
    float *data;
    float *query;
    int *key;
    // Modified
    // int *value;
    int *valueList;
    float *value;
    // float *valueList;
    int baseNum, queryNum;
    int dim;
    // Add
    int valueDim;
    std::vector<std::vector<std::pair<float,int> > > ans;
    std::vector<std::pair<int,int> > qRange;
    // Modified
    // DataMaker(const char* baseFile, const char* queryFile, const char* dataFile, int N, int M, int d){
    DataMaker(const char* baseFile, const char* queryFile, const char* dataFile, int N, int M, int d, int vDim){
        baseNum = N;
	queryNum = M;
        dim = d;
	// Add
	valueDim = vDim;
        load_data(baseFile, data, N, dim);
        load_data(queryFile, query, M, dim);

        std::ifstream file(dataFile);
        key = new int[N];
	// Modified
        // value = new int[N];
        valueList = new int[N];
	value = new float[N * valueDim];
	// valueList = new float[N * valueDim];

        qRange.resize(M);
        ans.resize(M);
	// Modified
	/*
        for(int i = 0; i < N; i++){
            int tmp;
            file>>key[i]>>tmp;
            value[i] = tmp;
            valueList[i] = value[i];
        }
	*/
	for(int i = 0; i < N; i++){
            int tmpKey;
            file >> tmpKey;
            key[i] = tmpKey;

            for (int j = 0; j < valueDim; j++) {
                float val;
                file >> val;
                value[i * valueDim + j] = val;
                // valueList[i * valueDim + j] = val;
            }
	    valueList[i] = value[i * valueDim];
        }
        // std::sort(valueList, valueList + N);
    }
    
    /*
    DataMaker(const char* baseFile, const char* queryFile, int N, int M, int d){
        baseNum = N;
        queryNum = M;
        dim = d;
        load_data(baseFile, data, N, dim);
        load_data(queryFile, query, M, dim);

        key = new int[N];
        value = new int[N];
        valueList = new int[N];

        qRange.resize(M);
        ans.resize(M);
        
        for(int i = 0; i < N; i++){
            valueList[i] = value[i] = key[i] = i;
        }

        std::random_device rd;
        std::mt19937 g(rd());
    }
    */

    void genRange(const float* filter, int k){

        /*
        std::random_device rd;
        std::mt19937 rng(rd());
        int r = baseNum * range;
        std::uniform_int_distribution<int> ud(0, baseNum - r - 1);
        for(int i = 0; i< queryNum; i++){
            int L = ud(rng);
            int R = L + r;
            qRange[i] = {valueList[L],valueList[R]};
            ans[i] = getTopK(i, qRange[i].first, qRange[i].second, k);
        }
        */
       for(int i = 0; i< queryNum; i++){
            // int L = filter[2 * i];
            // int R = filter[2 * i + 1];
            // qRange[i] = {L, R};
            ans[i] = getTopK(i, filter, k);
        }
    }

    std::vector<std::pair<float,int> > getGt(int id){
        return ans[id];
    }
private:
    float getDistance(int query_id, int base_id){
        float ans = 0;
        for(int i = 0; i < dim; i++)
            ans+=(query[query_id*dim + i] - data[base_id * dim +i]) * (query[query_id*dim + i] - data[base_id * dim +i]);
        return ans;
    }

    std::vector<std::pair<float,int> > getTopK(int query_id, const float* filter, int k){
        // std::cout << query_id << ' ' << L << ' ' << R << std::endl;
        std::vector<std::pair<float,int> > result;
        for(int i = 0; i < baseNum; i++){
	    for (int j = 0; j < valueDim; j++){
                if(value[valueDim * i + j] >= filter[2 * valueDim * i + j] && value[valueDim * i + j] <= filter[2 * valueDim * i + j + 1]) {
                    // std::cout << i << std::endl;
                    result.push_back({getDistance(query_id,i),i});
	        }
	    }
        }
        sort(result.begin(), result.end());
        result.resize(k);
        return result;
    }


};


#endif //RANGEHNSW_DATAMAKER_HPP
