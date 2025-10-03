#ifndef RANGEHNSW_UTILS_HPP
#define RANGEHNSW_UTILS_HPP

#include <iostream>
#include <fstream>

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

void load_data(const char* filename, float*& data, int num, int dim) {
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open()) {
        std::cout << "open file error" << std::endl;
        exit(-1);
    }
    in.read((char*)&dim, 4);
    in.seekg(0, std::ios::end);	
    std::ios::pos_type ss = in.tellg();
    size_t fsize = (size_t)ss;
    num = (unsigned)(fsize / (dim + 1) / 4);	
    data = new float[(size_t)num * (size_t)dim];

    in.seekg(0, std::ios::beg);
    for (size_t i = 0; i < num; i++) {
        in.seekg(4, std::ios::cur);	
        in.read((char*)(data + i * dim), dim * 4);	
    }
    in.close();
}


#endif //RANGEHNSW_UTILS_HPP
