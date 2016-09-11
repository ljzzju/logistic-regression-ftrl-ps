#include <iostream>
#include "load_data.h"
#include "ps.h"

namespace dmlc{
namespace linear{

class Worker : public ps::App{
    public:
        Worker(const char *filepath) : file_path(filepath){ }
        ~Worker(){
            delete data;
        } 

        virtual void ProcessRequest(ps::Message* request){
	    //do nothing.
	}

        float sigmoid(float x){
            if(x < -30) return 1e-6;
            else if(x > 30) return 1.0;
            else{
                double ex = pow(2.718281828, x);
                return ex / (1.0 + ex);
            }
        }

	virtual bool Run(){
	    Process();
	}

        virtual void Process(){
	    rank = ps::MyRank();
	    snprintf(data_path, 1024, "%s-%05d", file_path, rank);
	    data = new Load_Data(data_path);
	    
        std::cout<<"i am rank "<<rank<<std::endl;
            for(int i = 0; i < step; i++){
                data->load_data_minibatch(10);
	        if(data->fea_matrix.size() == 0) break;
                std::vector<float> mb_w;
                std::vector<float> mb_g;
                std::vector<ps::Key> mb_keys;
                std::vector<float> mb_values;
                for(int i = 0; i < data->fea_matrix.size(); i++){
                    mb_keys.clear(); mb_values.clear();
                    float wx = bias;
                    for(int j = 0; j < data->fea_matrix[i].size(); j++){
                        long int index = data->fea_matrix[i][j].idx;
                        mb_keys.push_back(index);
                        float value = data->fea_matrix[i][j].val;
                        mb_values.push_back(value);
                    }
                    kv_.Wait(kv_.Pull(mb_keys, &mb_w));
		    //for(int i = 0; i < keys.size(); i++){
		    //	std::cout<<keys[i]<<":"<<w[i]<<std::endl;	
		    //}
                    for(int j = 0; j < mb_w.size(); j++){
                        wx += mb_w[j] * mb_values[j];
                    }
                    float pctr = sigmoid(wx);
                    mb_g.resize(mb_keys.size());
                    for(int j = 0; j < mb_keys.size(); j++){
                        mb_g[j] += (pctr - data->label[i]) * mb_values[j];
                    }
		    kv_.Wait(kv_.Push(mb_keys, mb_g));
                }//end for
            }//end for
	    std::set<long int>::iterator iter;
	    for(iter = data->feaIdx.begin(); iter != data->feaIdx.end(); iter++){
		fea_all.push_back(*iter);	
	    }
	    std::cout<<"feaIdx size = "<<data->feaIdx.size()<<std::endl;
	    kv_.Wait(kv_.Pull(fea_all, &w_all));
	    for(int i = 0; i < fea_all.size(); i++){
                continue;
		//std::cout<<fea_all[i]<<":"<<w_all[fea_all[i]]<<std::endl;
	    }
        }
    std::vector<ps::Key> fea_all;
    std::vector<float> w_all;	
    Load_Data *data;
    const char *file_path;
    char data_path[1024];
    int rank;
    float alpha = 1.0;
    float beta = 1.0;
    float lambda1 = 0.0;
    float lambda2 = 1.0;
    float bias = 0.1;
    int step = 1000;
    ps::KVWorker<float> kv_;
};

}
}
