// #include<iostream>
// #include<sstream>
// #include<fstream>
// #include<vector>
// #include <bits/stdc++.h> 
// #define FIFO_PATH "/tmp/os_ca2"
// #define LOAD_BALANCER_TAG "IAMLB"
#include"defines.hpp"

using namespace std;
struct Trie { 
    string key; 
    int cnt; 
    unordered_map<char, Trie*> map; 
}; 
  
/* Function to return a new Trie node */
Trie* getNewTrieNode() 
{ 
    Trie* node = new Trie; 
    node->cnt = 0; 
    return node; 
} 
  
/* function to insert a string */
void insert(Trie*& root, string &str) 
{ 
    // start from root node 
    Trie* temp = root; 
  
    for (int i=0; i<str.length(); i++) { 
          
        char x = str[i];  
  
        /*a new node if path doesn't exists*/
        if (temp->map.find(x) == temp->map.end()) 
            temp->map[x] = getNewTrieNode(); 
  
        // go to next node 
        temp = temp->map[x]; 
    } 
  
    // store key and its count in leaf nodes 
    temp->key = str; 
    temp->cnt += 1; 
} 
  
/* function for preorder traversal */
bool preorder(Trie* temp, int& maxcnt, string& key) 
{ 
    if (temp == NULL) 
        return false; 
  
    for (auto it : temp->map) { 
  
        /*leaf node will have non-zero count*/
        if (maxcnt < it.second->cnt) { 
            key = it.second->key; 
            maxcnt = it.second->cnt; 
        } 
  
        // recurse for current node children 
        preorder(it.second, maxcnt, key); 
    } 
} 
  
string mostFrequentWord(vector<string> arr, int n) 
{ 
    // Insert all words in a Trie 
    Trie* root = getNewTrieNode(); 
    for (int i = 0; i < n; i++) 
        insert(root, arr[i]); 
  
    // Do preorder traversal to find the  
    // most frequent word 
    string key; 
    int cnt = 0; 
    preorder(root, cnt, key); 
  
    return key; 
} 
// int mostFrequent(vector<int> arr, int n) 
// { 
//     // Sort the array 
//     sort(arr, arr + n); 
  
//     // find the max frequency using linear traversal 
//     int max_count = 1, res = arr[0], curr_count = 1; 
//     for (int i = 1; i < n; i++) { 
//         if (arr[i] == arr[i - 1]) 
//             curr_count++; 
//         else { 
//             if (curr_count > max_count) { 
//                 max_count = curr_count; 
//                 res = arr[i - 1]; 
//             } 
//             curr_count = 1; 
//         } 
//     } 
  
//     // If last element is most frequent 
//     if (curr_count > max_count) 
//     { 
//         max_count = curr_count; 
//         res = arr[n - 1]; 
//     } 
  
//     return res; 
// } 
// vector <int>  change_vector_to_int_vector(vector <string> words){
//     vector <int> numbers;
//     for(int i = 0; i < words.size(); i++)
//     {
//         numbers.push_back(atoi(words[i].c_str));
//     }
//     return numbers;
// }
int main(int argc, char const *argv[])
{
    cout<<"Peresnter started working"<<endl<<endl;
    ifstream pipe_stream(FIFO_PATH, ifstream::in | ifstream::out);
    string input_of_presenter;
    vector<string> workers_response;
    string valid_data;
    int count = 0;
    int num_of_workers;
    while(1){
                
        getline(pipe_stream, input_of_presenter);
        if (input_of_presenter == LOAD_BALANCER_TAG ) {
            getline(pipe_stream,input_of_presenter);
            stringstream geek(input_of_presenter);
            geek >> num_of_workers; 

        }
        
        count+=1;
        workers_response.push_back(input_of_presenter);
        if(count == num_of_workers){
            valid_data = mostFrequentWord(workers_response,workers_response.size());
            // valid_data = mostFrequent(change_vector_to_int_vector(workers_response), 393);
            cout << "--- Valid Data Is : "<<valid_data<<" ---"<<endl<<endl;
            workers_response.clear();
            count = 0;
        }
             
        // cout<<"IN PRESENTER RECIVED : "<<input_of_presenter<<endl;
    }
    pipe_stream.close();
    return 0;
}
