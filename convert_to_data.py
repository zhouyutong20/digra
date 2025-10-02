#!/usr/bin/env python3
"""
SIFT dataset converter for DIGRA format
Converts .fvecs files to .data format where key is index and value is label
"""

import numpy as np
import struct
import os
import sys

def read_fvecs(filename):
    """Read .fvecs format file"""
    vectors = []
    with open(filename, 'rb') as f:
        while True:
            # Read vector dimension (4-byte integer)
            dim_bytes = f.read(4)
            if not dim_bytes:
                break
            dim = struct.unpack('I', dim_bytes)[0]
            # Read vector data (dim * 4-byte floats)
            vec_bytes = f.read(4 * dim)
            if not vec_bytes or len(vec_bytes) != 4 * dim:
                break
            vec = struct.unpack('f' * dim, vec_bytes)
            vectors.append(np.array(vec, dtype=np.float32))
    return np.array(vectors)

def read_labels(filename):
    """Read label file, could be .fvecs or raw binary format"""
    try:
        # Try reading as .fvecs format first
        labels = read_fvecs(filename)
        if labels.shape[1] == 1:  # If single-column labels
            labels = labels.flatten()
        return labels
    except:
        # If .fvecs format fails, try raw binary
        with open(filename, 'rb') as f:
            data = f.read()
            # Try to parse as integer labels
            if len(data) % 4 == 0:
                # Probably 4-byte integers
                labels = np.frombuffer(data, dtype=np.int32)
            elif len(data) % 8 == 0:
                # Probably 8-byte integers
                labels = np.frombuffer(data, dtype=np.int64)
            else:
                # Try as floats
                labels = np.frombuffer(data, dtype=np.float32)
        return labels

def convert_to_data_format(base_vectors_file, labels_file, output_file, max_vectors=None):
    """
    Convert base vectors and labels to .data format
    Format: key(index) value(label) - both integers
    """
    print(f"Reading base vectors: {base_vectors_file}")
    base_vectors = read_fvecs(base_vectors_file)
    print(f"Reading labels: {labels_file}")
    labels = read_labels(labels_file)
    
    # Ensure vector and label counts match
    n_vectors = min(len(base_vectors), len(labels))
    if max_vectors:
        n_vectors = min(n_vectors, max_vectors)
    
    print(f"Found {n_vectors} vector-label pairs")
    print(f"Vector dimension: {base_vectors.shape[1] if len(base_vectors.shape) > 1 else 1}")
    print(f"Label dimension: {labels.shape[1] if len(labels.shape) > 1 else 1}")
    
    # Write .data format file
    print(f"Writing data to: {output_file}")
    with open(output_file, 'w') as f:
        for i in range(n_vectors):
            # Key is the index (integer), value is the label (integer)
            if len(labels.shape) > 1:
                # If label is a vector, use the first element or convert to single value
                label_value = int(labels[i][0]) if len(labels[i]) > 0 else i
            else:
                # If label is scalar, convert to integer
                label_value = int(labels[i])
            
            f.write(f"{i} {label_value}\n")
    
    print(f"Conversion completed! Converted {n_vectors} records")

def create_query_files(query_vectors_file, output_query_file, output_query_data_file):
    """Create query-related files"""
    print(f"Reading query vectors: {query_vectors_file}")
    query_vectors = read_fvecs(query_vectors_file)
    
    # Save query vectors as .fvecs format (if needed)
    print(f"Saving query vectors to: {output_query_file}")
    with open(output_query_file, 'wb') as f:
        for vec in query_vectors:
            dim = len(vec)
            f.write(struct.pack('I', dim))
            f.write(struct.pack('f' * dim, *vec))
    
    # Create query .data file (key: query index, value: query index)
    print(f"Creating query data file: {output_query_data_file}")
    with open(output_query_data_file, 'w') as f:
        for i in range(len(query_vectors)):
            f.write(f"{i} {i}\n")
    
    print(f"Query files created! Total {len(query_vectors)} query vectors")

def create_filter_file(labels_file, output_filter_file, max_vectors=None):
    """Create filter file for range filtering"""
    print(f"Reading labels for filter file: {labels_file}")
    labels = read_labels(labels_file)
    
    n_vectors = len(labels)
    if max_vectors:
        n_vectors = min(n_vectors, max_vectors)
    
    # Create filter .data file (key: vector index, value: label for filtering)
    print(f"Creating filter data file: {output_filter_file}")
    with open(output_filter_file, 'w') as f:
        for i in range(n_vectors):
            if len(labels.shape) > 1:
                filter_value = int(labels[i][0]) if len(labels[i]) > 0 else i
            else:
                filter_value = int(labels[i])
            f.write(f"{i} {filter_value}\n")
    
    print(f"Filter file created! Total {n_vectors} records")

def main():
    # File paths
    base_dir = "/home/ytzhou/faiss/build/Datasets/ag_news/"
    
    # Input files
    base_vectors_file = os.path.join(base_dir, "ag_news_base.fvecs")
    labels_file = os.path.join(base_dir, "ag_news_labels.fvecs")
    # query_vectors_file = os.path.join(base_dir, "cc_news_query.fvecs")
    # filters_file = os.path.join(base_dir, "cc_news_filters.fvecs")
    
    # Output files
    output_data_file = os.path.join(base_dir, "ag_news.data")
    # output_query_file = os.path.join(base_dir, "msong_query.fvecs")
    # output_query_data_file = os.path.join(base_dir, "msong_query.data")
    # output_filter_file = os.path.join(base_dir, "msong_filters.data")
    
    # Create output directory
    os.makedirs(base_dir, exist_ok=True)
    
    print("=== AG_NEWS Dataset Converter for DIGRA ===\n")
    
    # 1. Convert base data (key: index, value: label)
    if os.path.exists(base_vectors_file) and os.path.exists(labels_file):
        convert_to_data_format(base_vectors_file, labels_file, output_data_file, max_vectors=1000000)
    else:
        print(f"Warning: Base data files not found")
        print(f"Base vectors: {base_vectors_file} - {'Exists' if os.path.exists(base_vectors_file) else 'Missing'}")
        print(f"Labels file: {labels_file} - {'Exists' if os.path.exists(labels_file) else 'Missing'}")
    
    print("\n" + "="*50 + "\n")
    
    # 2. Process query data
    # if os.path.exists(query_vectors_file):
    #     create_query_files(query_vectors_file, output_query_file, output_query_data_file)
    # else:
    #     print(f"Warning: Query vectors file not found: {query_vectors_file}")
    
    # print("\n" + "="*50 + "\n")
    
    # 3. Create filter file for range filtering
    # if os.path.exists(filters_file):
    #     create_filter_file(filters_file, output_filter_file, max_vectors=1000000)
    # elif os.path.exists(labels_file):
    #     # Use labels as filter values if separate filter file doesn't exist
    #     print(f"Using labels file for filter data: {labels_file}")
    #     create_filter_file(labels_file, output_filter_file, max_vectors=1000000)
    # else:
    #     print(f"Warning: No filter file available")
    
    print("\n=== Conversion Completed ===")
    print(f"Generated files:")
    print(f"- Base data: {output_data_file}")
    # print(f"- Query vectors: {output_query_file}")
    # print(f"- Query data: {output_query_data_file}")
    # print(f"- Filter data: {output_filter_file}")

def verify_conversion():
    """Verify the conversion results"""
    base_dir = "/home/ytzhou/faiss/build/Datasets/sift10m"
    output_data_file = os.path.join(base_dir, "sift10m_base.data")
    
    if os.path.exists(output_data_file):
        print(f"\n=== Verification ===")
        with open(output_data_file, 'r') as f:
            lines = f.readlines()
            print(f"Total lines: {len(lines)}")
            if lines:
                # Check first few lines
                print(f"First 5 lines:")
                for i in range(min(5, len(lines))):
                    line = lines[i].strip()
                    parts = line.split()
                    print(f"  Line {i}: key={parts[0]}, value={parts[1]}")
                
                # Check format consistency
                sample_line = lines[0].strip()
                parts = sample_line.split()
                if len(parts) == 2:
                    print(f"✓ Correct format: 2 columns per line")
                else:
                    print(f"✗ Incorrect format: {len(parts)} columns found")

if __name__ == "__main__":
    main()
    # verify_conversion()
