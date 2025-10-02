#!/bin/bash

# cc_news M128 EF400
nohup ./Digra 620643 10000 384 100 400 128 \
        /home/ytzhou/faiss/build/Datasets/cc_news/cc_news_base.fvecs \
        /home/ytzhou/faiss/build/Datasets/cc_news/cc_news_query.fvecs \
        /home/ytzhou/faiss/build/Datasets/cc_news/cc_news.data \
        /home/ytzhou/faiss/build/Datasets/cc_news/cc_news_filters.fvecs \
        >cc_news_m128_ef400_k100_1001.log 2>&1
wait
echo "cc_news M128 EF400 Done!"

# cc_news M256 EF400
nohup ./Digra 620643 10000 384 100 400 256 \
        /home/ytzhou/faiss/build/Datasets/cc_news/cc_news_base.fvecs \
        /home/ytzhou/faiss/build/Datasets/cc_news/cc_news_query.fvecs \
        /home/ytzhou/faiss/build/Datasets/cc_news/cc_news.data \
        /home/ytzhou/faiss/build/Datasets/cc_news/cc_news_filters.fvecs \
        >cc_news_m256_ef400_k100_1001.log 2>&1
wait
echo "cc_news M256 EF400 Done!"

# cc_news M512 EF400
nohup ./Digra 620643 10000 384 100 400 512 \
        /home/ytzhou/faiss/build/Datasets/cc_news/cc_news_base.fvecs \
        /home/ytzhou/faiss/build/Datasets/cc_news/cc_news_query.fvecs \
        /home/ytzhou/faiss/build/Datasets/cc_news/cc_news.data \
        /home/ytzhou/faiss/build/Datasets/cc_news/cc_news_filters.fvecs \
        >cc_news_m512_ef400_k100_1001.log 2>&1
wait
echo "cc_news M512 EF400 Done!"

# ag_news M128 EF400
nohup ./Digra 120000 7600 384 100 400 128 \
        /home/ytzhou/faiss/build/Datasets/ag_news/ag_news_base.fvecs \
        /home/ytzhou/faiss/build/Datasets/ag_news/ag_news_query.fvecs \
        /home/ytzhou/faiss/build/Datasets/ag_news/ag_news.data \
        /home/ytzhou/faiss/build/Datasets/ag_news/ag_news_filters.fvecs \
        >ag_news_m128_ef400_k100_1001.log 2>&1
wait
echo "ag_news M128 EF400 Done!"

# ag_news M256 EF400
nohup ./Digra 120000 7600 384 100 400 256 \
        /home/ytzhou/faiss/build/Datasets/ag_news/ag_news_base.fvecs \
        /home/ytzhou/faiss/build/Datasets/ag_news/ag_news_query.fvecs \
        /home/ytzhou/faiss/build/Datasets/ag_news/ag_news.data \
        /home/ytzhou/faiss/build/Datasets/ag_news/ag_news_filters.fvecs \
        >ag_news_m256_ef400_k100_1001.log 2>&1
wait
echo "ag_news M256 EF400 Done!"

# ag_news M512 EF400
nohup ./Digra 120000 7600 384 100 400 512 \
        /home/ytzhou/faiss/build/Datasets/ag_news/ag_news_base.fvecs \
        /home/ytzhou/faiss/build/Datasets/ag_news/ag_news_query.fvecs \
        /home/ytzhou/faiss/build/Datasets/ag_news/ag_news.data \
        /home/ytzhou/faiss/build/Datasets/ag_news/ag_news_filters.fvecs \
        >ag_news_m512_ef400_k100_1001.log 2>&1
wait
echo "ag_news M512 EF400 Done!"
