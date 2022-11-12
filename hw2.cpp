#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>
#include <math.h> 
#include <ctime>
#include <algorithm>

using namespace std;
void swap(int &a, int &b){
    int temp = a;
    a = b;
    b = temp;
}

float worstRequestTimeNeighber(vector<int> &curr ,vector<vector<float> > &arr, float tou){
    float worst_rt = 0;
    int sum_of_elems = 0;
    vector<float> respond_time ;
    for(int i = 0; i < arr.size(); i ++){
        float max_trans = numeric_limits<float>::min();
        for(int j = i; j < arr.size(); j++){   //lower or same priority
            if(arr[curr[j]][1] > max_trans)
                max_trans = arr[curr[j]][1];
        }
        float block = max_trans; //find blocking time
        float q = block;
        float RHS = 0;
        while(1){
            int p_j = i; //curren priority
            float total_wating = 0;
            while(p_j > 0){ //calculate wating time
                p_j -= 1;
                total_wating += ceil((q + tou)/arr[curr[p_j]][2]) * arr[curr[p_j]][1]; 
            }
            RHS = block + total_wating;           
            if(RHS == q){   //success 
                break;
            }   
            else{
                q = RHS;    //for next run
            }
        }
        if(RHS + arr[curr[i]][1] > arr[curr[i]][2]){
            return -1;
        }
        else
            respond_time.push_back(RHS + arr[curr[i]][1]);
    }
    for(vector<float>::iterator it = respond_time.begin(); it != respond_time.end(); ++it)
        worst_rt += *it;
    return worst_rt;
}

int main(){
    ifstream infile("input.dat");
    int num, i = 0;
    float tou;
    //cout<< max_trans;
    string line;
    vector<vector<float> > arr;
    infile >> num >> tou;
    vector<float> worst_respone;
    float current_opt = 0, worst_res = 0;

    //cout<<num<<"\n"<<tou<<"\n";
    arr.resize(num);
    worst_respone.resize(num);
    while (getline(infile, line))
    {   
        istringstream iss(line);
        int priority, period;
        float trans;    
        if(iss >> priority >> trans >> period){
            arr[i].push_back(priority); //0
            arr[i].push_back(trans);    //1
            arr[i].push_back(period);   //2
            //cout<< arr[i][0] << "    " << arr[i][1] << "     " << arr[i][2]<<"\n"; 
            i += 1;
        }   
    }
    infile.close();

    for(int i = 0; i < arr.size(); i++){
        float max_trans = numeric_limits<float>::min();
        for(int j = i; j < arr.size(); j++){   //lower or same priority
            if(arr[j][1] > max_trans)
                max_trans = arr[j][1];
        }
        float block = max_trans; //find blocking time
        //cout <<"block: "<<block<<"\n";
        float q = block;
        while(1){
            //cout<<"q: "<<q<<"\n";
            int p_j = arr[i][0]; //curren priority
            float total_wating = 0, RHS;
            while(p_j > 0){ //calculate wating time
                p_j -= 1;
                total_wating += ceil((q + tou)/arr[p_j][2])*arr[p_j][1]; 
                //cout<<"max :"<<ceil((q + tou)/arr[p_j][2])<<"\n";
            }
            //cout<<"total_wating: "<< total_wating<<"\n";
            RHS = block + total_wating;
            //cout<<RHS<<"\n";
            if(RHS == q){   //success 
                worst_respone[arr[i][0]] = RHS + arr[i][1];
                //cout<< arr[i][0] << "'s worst_respone_time : " << worst_respone[arr[i][0]] <<"\n";
                current_opt += worst_respone[arr[i][0]];
                break;
            }   
            else{
                q = RHS;    //for next run
                //cout<< q<< "\n";
            }
        }
    }
    vector<int> priority;       //s
    for(int i = 0; i < arr.size(); i++){
            priority.push_back(arr[i][0]);
    }
    float best_opt = current_opt;
    float T = 5;
    vector<int> best_priority = priority;  //s*
    vector<int> cur_priority(17);   //s'
    float cost = 0;
    for(int i = 0; i < arr.size(); i++){
            cur_priority.push_back(arr[i][0]);
    }
    double x = 0;
    srand(unsigned(time(NULL)));
    while(T > 0.001){
        float cost = 0;
        cur_priority = priority; //s'
        float prob = 0 ;
        int x = rand() % 17;
        int y = rand() % 17;
        while(x == y){
            y = rand() % 17;
        }
        //cout <<"X "<< x<< " y "<<y<<endl;
        swap(cur_priority[x], cur_priority[y]);
        current_opt = worstRequestTimeNeighber(cur_priority , arr, tou);
        //cout << current_opt<< endl;
        if(current_opt == -1){ // if not schdulable
            swap(cur_priority[x], cur_priority[y]);
        }
        else{
            if(best_opt > current_opt ){
                best_opt = current_opt; 
                best_priority = cur_priority;
            }
            cost = current_opt - best_opt;
            if(cost <= 0){
                priority = cur_priority;
            }
            else{
                prob = exp (-cost/T);
                if(((double) rand() / (RAND_MAX + 1.0)) <= prob){
                    priority = cur_priority;
                }
            }      
        }
        T = T *0.99;
    }
    for(int i = 0; i < best_priority.size(); i++){
            cout << best_priority[i]<<" ";
    }
    cout<< " best_opt: "<< best_opt<<endl;
    return 0;
}