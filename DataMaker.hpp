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
    int *valueList;
    float *value;
    // float *valueList;
    int baseNum, queryNum;
    int dim;
    int valueDim;
    std::vector<std::vector<std::pair<float,int> > > ans;
    std::vector<std::pair<int,int> > qRange;
    DataMaker(const char* baseFile, const char* queryFile, const char* dataFile, int N, int M, int d, int vDim)
    {
        baseNum = N;
	    queryNum = M;
        dim = d;
	    // Add
    	valueDim = vDim;
        load_data(baseFile, data, N, dim);
        load_data(queryFile, query, M, dim);

        key = new int[N];
	    // Modified
        // value = new int[N];
        valueList = new int[N];
	    value = new float[N * valueDim];
	    // valueList = new float[N * valueDim];

        qRange.resize(M);
        ans.resize(M);

        read_fvecs(dataFile, value, N, vDim);
        for(int i = 0; i < N; i++){
            int tmpKey;
            key[i] = i;
            valueList[i] = (int)value[i * valueDim];
        }
    }
    void genRange(const float* filter, int k)
    {
       for(int i = 0; i< queryNum; i++){
            ans[i] = getTopK(i, filter, k);
        }
    }

    std::vector<std::pair<float,int> > getGt(int id)
    {
        return ans[id];
    }
private:
    float getDistance(int query_id, int base_id)
    {
        float ans = 0;
        for(int i = 0; i < dim; i++)
            ans+=(query[query_id*dim + i] - data[base_id * dim +i]) * (query[query_id*dim + i] - data[base_id * dim +i]);
        return ans;
    }

    std::vector<std::pair<float,int> > getTopK(int query_id, const float* filter, int k)
    {
        std::vector<std::pair<float,int> > result;
        for(int i = 0; i < baseNum; i++){
            int ans = 1;
            for (int j = 0; j < valueDim; j++) {
                if(value[valueDim * i + j] < filter[2 * valueDim * query_id +  2 * j] || value[valueDim * i + j] > filter[2 * valueDim * query_id + 2 * j + 1]) 
                {
                    ans = 0;
                }
            }
            if (ans) {
                result.push_back({getDistance(query_id,i),i});
            }
        }
        sort(result.begin(), result.end());
        result.resize(k);
        return result;
    }
};


#endif //RANGEHNSW_DATAMAKER_HPP
