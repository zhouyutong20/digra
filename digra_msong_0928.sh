#!/bin/bash

# msong M128 EF400
nohup ./Digra 985185 10000 420 100 400 128 \
	/home/ytzhou/faiss/build/Datasets/msong/msong_base.fvecs \
	/home/ytzhou/faiss/build/Datasets/msong/msong_query.fvecs \
	/home/ytzhou/faiss/build/Datasets/msong/msong.data \
	/home/ytzhou/faiss/build/Datasets/msong/msong_filters.fvecs \
	>msong_m128_ef400_k100_0928.log 2>&1
wait
echo "msong M128 EF400 Done!"

# msong M256 EF400
nohup ./Digra 985185 10000 420 100 400 256 \
	/home/ytzhou/faiss/build/Datasets/msong/msong_base.fvecs \
	/home/ytzhou/faiss/build/Datasets/msong/msong_query.fvecs \
	/home/ytzhou/faiss/build/Datasets/msong/msong.data \
	/home/ytzhou/faiss/build/Datasets/msong/msong_filters.fvecs \
	>msong_m256_ef400_k100_0928.log 2>&1
wait
echo "msong M256 EF400 Done!"

# msong M512 EF400
nohup ./Digra 985185 10000 420 100 400 512 \
	/home/ytzhou/faiss/build/Datasets/msong/msong_base.fvecs \
	/home/ytzhou/faiss/build/Datasets/msong/msong_query.fvecs \
	/home/ytzhou/faiss/build/Datasets/msong/msong.data \
	/home/ytzhou/faiss/build/Datasets/msong/msong_filters.fvecs \
	>msong_m512_ef400_k100_0928.log 2>&1
wait
echo "msong M512 EF400 Done!"
