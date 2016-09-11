#ifndef LOAD_DATA_H_
#define LOAD_DATA_H_

#include <fstream>
#include <iostream>
#include <vector>
#include <set>

namespace dmlc{
namespace linear{

struct sparse_feature{
    long int idx;
    int val;
};

class Load_Data {
public:
    std::ifstream fin_;
    std::vector<std::vector<sparse_feature> > fea_matrix;
    std::vector<sparse_feature> key_val;
    sparse_feature sf;
    std::vector<int> label;
    std::string line;
    int y, value, nchar;
    long int index;
    std::set<long int> feaIdx;    
    std::set<long int>::iterator setIter;

    Load_Data(const char * file_name){
	fin_.open(file_name, std::ios::in);
	if(!fin_.is_open()) {
            std::cout << " open file error: " << file_name << std::endl;
            exit(1);
        }
    }

    ~Load_Data(){
        fin_.close();
    }

    void load_data_minibatch(const int num){
        fea_matrix.clear();
	//std::cout<<"load batch data start..."<<std::endl;
    	for(int i = 0; i < num; i++){
            std::getline(fin_, line);
	    if(fin_.eof()) break;
            key_val.clear();
            const char *pline = line.c_str();
            if(sscanf(pline, "%d%n", &y, &nchar) >= 1){
                pline += nchar;
                label.push_back(y);
                while(sscanf(pline, "%ld:%d%n", &index, &value, &nchar) >= 2){
                    pline += nchar;
                    sf.idx = index;
		    setIter = feaIdx.find(index);
		    if(setIter == feaIdx.end()) feaIdx.insert(index);
                    sf.val = value;
                    key_val.push_back(sf);
                }
            }
            fea_matrix.push_back(key_val);
	}//end for
    }

};

}//end linear
}//end dmlc
#endif
