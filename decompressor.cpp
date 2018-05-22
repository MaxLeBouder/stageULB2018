#include <fstream>
#include <sstream>
#include <cstring>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <math.h>

using namespace std;

#define DEBUG 1

struct read_encoding{
	int anchor_number;
	int read_position;
	string path_direction;
	//string missmatches_encoding;
};

uint32_t str_bin_to_int(const string& str){
    uint32_t res(0);
    for(uint i(0);i<4;++i){
        res+=(unsigned char)(str[3-i]<<(4*i));
    }
    return res;
}

int main(int argc, char ** argv){

	if(argc<2){
		cout<<"[path file]"<<endl;
		exit(0);
	}
	string input(argv[1]);
	ifstream in(input);
	if(not in.good()){
		cout<<"Problem with file opening"<<endl;
		exit(1);
	}

	//DECOMPRESSION

	//FILE OPEN
	if(DEBUG){cout<<"Fichier ouvert"<<endl;}

	vector<read_encoding> reads;

	//INPUT

	std::string parse, parse_path, parse_pos, path;
	read_encoding read_prec;
	int anchor_value;
	int anchor_prec = 0;
	int position;
	int pos_prec = 0;

	while(not in.eof()){
		//Reading anchor absolute value
		getline(in,parse,(char)255);
		if(not parse.empty()){
			//Retrieve anchor value and convert it to integer
			std::istringstream anchor(parse);
			anchor >> anchor_value;	
			//anchor_value = str_bin_to_int(parse);

			//The anchor value is equals to the previous value and the current relative position
			anchor_value = abs(anchor_value) + abs(anchor_prec);

			anchor_prec = anchor_value;
		}

		//Reading paths for the positive anchor
		
		getline(in,parse,(char)255);
		if(not parse.empty()){											
			//if(DEBUG){cout<<"Ancre positive"<<endl;}

			//Reading path and positions
			std::stringstream parse_s(parse);
			char current;
			parse_s.get(current);
			while(not parse_s.eof()){

				/*
				if(current == (char)254){
					pos_prec = -1;
					//Read the compressed path
					getline(parse_s,path,':');
					//Read the first position
					parse_s.get(current);
					if(current == '0'){
						position = 0;
					}else{
						position = (unsigned char)current;
					}
					//Push the read with the first position
					reads.push_back({anchor_value,position,path});

					int next = parse_s.peek();
					//If there's a multiple position sign
					if(next == '#'){
						pos_prec = position;
						parse_s.get(current); //To discard the '#'
					}
				//If we're reading relative positions after a '#', then pos_prec is set
				}else if(pos_prec > -1){
					position = pos_prec + (unsigned char)current;
					reads.push_back({anchor_value,position,path});
					pos_prec = position;
				}

				//Get the next character in the parse stream
				parse_s.get(current);

				*/

				//TEMP
				if(current == (char)254){
					pos_prec = 0;
					//Read the compressed path
					getline(parse_s,path,':');

					parse_s.get(current);
				}
				if(current == (char)253){
					parse_s.get(current);
				}else{
					position = (int)current;
					if(current == '0'){
						position = 0;
					}
					position += pos_prec;
					reads.push_back({anchor_value,position,path});
					pos_prec = position;

					parse_s.get(current);
				}
				//FIN TEMP
			}
		}



		//Reading paths for the negative anchor
		getline(in,parse,(char)255);
		if(not parse.empty()){											
			//if(DEBUG){cout<<"Ancre negative"<<endl;}

			//The anchor value is negative
			anchor_value = -anchor_value;

			//Reading path and positions
			std::stringstream parse_s(parse);
			char current;
			parse_s.get(current);

			while(not parse_s.eof()){

				/*

				if(current == (char)254){
					pos_prec = -1;
					//Read the compressed path
					getline(parse_s,path,':');
					//Read the first position
					parse_s.get(current);
					if(current == '0'){
						position = 0;
					}else{
						position = (unsigned char)current;
					}
					//Push the read with the first position
					reads.push_back({anchor_value,position,path});

					int next = parse_s.peek();
					//If there's a multiple position sign
					if(next == '#'){
						pos_prec = position;
						parse_s.get(current); //To discard the '#'
					}
				//If we're reading relative positions after a '#', then pos_prec is set
				}else if(pos_prec > -1){
					position = pos_prec + (unsigned char)current;
					reads.push_back({anchor_value,position,path});
					pos_prec = position;
				}

				//Get the next character in the parse stream
				parse_s.get(current);

				*/

				//TEMP
				if(current == (char)254){
					pos_prec = 0;
					//Read the compressed path
					getline(parse_s,path,':');

					parse_s.get(current);
				}
				if(current == (char)253){
					parse_s.get(current);
				}else{
					position = (int)current;
					position += pos_prec;
					reads.push_back({anchor_value,position,path});
					pos_prec = position;

					parse_s.get(current);
				}
				//FIN TEMP
			}
		}
		
	}
	//End of reading

	//Writing the path on a file
	ofstream stream_paths("decompressedPaths_13");

	if(DEBUG){cout<<endl<<"Ecriture : "<<reads.size()<<" reads"<<endl;}
	for(uint i(0); i<reads.size(); ++i){
		//if(DEBUG){cout<<i+1<<"/"<<reads.size()<<endl;}
		stream_paths<<reads[i].anchor_number<<":"; 
		if(not reads[i].path_direction.empty()){
			stream_paths<<reads[i].path_direction;
		}
		stream_paths<<":"<<reads[i].read_position;
	
		stream_paths<<":"<<endl;
	}
}