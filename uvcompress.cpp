/* uvcompress.cpp
   CSC 485B/CSC 578B

   Placeholder starter code for UVCompress 

   B. Bird - 2023-05-01
*/
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
using namespace std;
vector<bool> buffer;
int max_bits = 16;

int num_bits = 9;
int pad;

void output_stream(int s){
    
    // char to bitstream
    vector<bool> bits_stream(num_bits,0);
    int counter = 0;
    while (s>0){  
    	bits_stream[counter++] = (s & 1);
	    s >>= 1;
    }
    
    // concat any leftover bits from last the symbol
    vector<bool> withbuffer = buffer;
    withbuffer.insert(withbuffer.end(), bits_stream.begin(), bits_stream.end());

    unsigned char byte_to_write;
    int numToWrite = withbuffer.size()/8;
    pad = withbuffer.size() % 8;
    
    for (int i=0;i<numToWrite;i++){
	    byte_to_write = 0;

        // group into bytes
	    for (int j=0;j<8;j++){
            bool val = withbuffer.front();
            if (val){
		        byte_to_write = byte_to_write + pow(2,j);
	        }
        
        withbuffer.erase(withbuffer.begin());  
        }
        cout << byte_to_write;
    } 
    //save bitstream leftover to buffer when its size is less than a byte
    buffer = withbuffer;
    
}


//function inspired by https://www.geeksforgeeks.org/how-to-find-index-of-a-given-element-in-a-vector-in-cpp/
int getIndex(vector<string> v, string K)
{
    vector<string>::iterator it = find(v.begin(), v.end(), K);
    if (it != v.end())
    {
        int index = it - v.begin();
        return index;
    }
    else {
        return -1;
    }
}
 

int main(){
    char magic1 = 0x1f;
    char magic2 = 0x9d;
    char mode = 0x90;
    cout << magic1 << magic2 << mode;

    char c;
    int next_symbol = 257;
    
    // intialize tables to save ascii char and symbols
    vector<string> symbol_table1;
    vector<string> symbol_table2;
    
    
    // fill table with ascii characters
    for (int i=0;i<256;++i){
        string str(1,char(i));
        symbol_table1.push_back(str);
    }
    
    
    string working(16,'\0');
    working = "";
    string augmented(16,'\0');
    int i;
    // for each char in the bitstream
    while(std::cin.get(c)){
        augmented = working + c;
        i = getIndex(symbol_table1, working);
        if (i == -1){
            i = getIndex(symbol_table2,working);    
            if (i!=-1){
                i = i + 257;
            }
        }

        if ((find(symbol_table1.begin(), symbol_table1.end(), augmented) != symbol_table1.end()) ||
        (find(symbol_table2.begin(), symbol_table2.end(), augmented) != symbol_table2.end()))
        {
            working = augmented;
        }else if (next_symbol >= pow(2,max_bits)){
            output_stream(i);
            working = c;
        }else{
            symbol_table2.push_back(augmented);
            output_stream(i);
            working = c;
            next_symbol ++;
            if (next_symbol > pow(2,num_bits)){
                num_bits ++;
            }
        }
    }
    
    if (working.size() >= 0) {
        i = getIndex(symbol_table1, working);
        if (i == -1){
            i = getIndex(symbol_table2,working);
            i = i + 257;
        }
        output_stream(i);
    }
    

    // pad the last byte
    char byte_to_write = 0;
    int power = 0;
    bool checked = false; 
    while (buffer.size()!= 0){
        bool val = buffer.front();
        if (val){
            byte_to_write += pow(2,power);
            checked = true;
        }
        power++;
        buffer.erase(buffer.begin());  
    }
    if (pad != 0){
        cout << byte_to_write;

    }
        //cout<< 0x00;
 
    return 0;
}
