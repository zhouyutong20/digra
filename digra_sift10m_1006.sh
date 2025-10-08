#!/bin/bash

nohup ./Digra 1000000 10000 128 100 400 128 \
/home/ytzhou/faiss/build/Datasets/sift10m/sift10m_base.fvecs \
/home/ytzhou/faiss/build/Datasets/sift10m/sift10m_query.fvecs \
/home/ytzhou/faiss/build/Datasets/sift10m/sift10m_labels.fvecs \
/home/ytzhou/faiss/build/Datasets/sift10m/sift10m_filters.fvecs \
6 >sift10m_m128_ef400_k100_1006_test2.log 2>&1
wait
echo "sift10m M128 EF400 Done!"

nohup ./Digra 1000000 10000 128 100 400 256 \
/home/ytzhou/faiss/build/Datasets/sift10m/sift10m_base.fvecs \
/home/ytzhou/faiss/build/Datasets/sift10m/sift10m_query.fvecs \
/home/ytzhou/faiss/build/Datasets/sift10m/sift10m_labels.fvecs \
/home/ytzhou/faiss/build/Datasets/sift10m/sift10m_filters.fvecs \
6 >sift10m_m256_ef400_k100_1006_test1.log 2>&1
wait
echo "sift10m M256 EF400 Done!"

nohup ./Digra 1000000 10000 128 100 400 512 \
/home/ytzhou/faiss/build/Datasets/sift10m/sift10m_base.fvecs \
/home/ytzhou/faiss/build/Datasets/sift10m/sift10m_query.fvecs \
/home/ytzhou/faiss/build/Datasets/sift10m/sift10m_labels.fvecs \
/home/ytzhou/faiss/build/Datasets/sift10m/sift10m_filters.fvecs \
6 >sift10m_m512_ef400_k100_1006_test1.log 2>&1
wait
echo "sift10m M512 EF400 Done!"
