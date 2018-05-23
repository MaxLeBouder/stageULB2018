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

vector<string> split(const string &s, char delim){
	stringstream ss(s);
	string item;
	vector<string> elems;
	while (getline(ss, item, delim)) {
		elems.push_back(move(item)); 
	}
	return elems;
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

			if(DEBUG){cout<<"Ancre : "<<parse<<";";}

			anchor >> anchor_value;	

			//The anchor value is equals to the previous value and the current relative position
			anchor_value = abs(anchor_value) + abs(anchor_prec);
			anchor_prec = anchor_value;

			if(DEBUG){cout<<anchor_value<<';'<<endl;}
		}

		//Reading paths for the positive anchor
		
		getline(in,parse,(char)255);
		if(not parse.empty()){											

			//SPLIT
			vector<string> v_paths = split(parse,(char)254);
			for(uint i(0);i<v_paths.size();i++){
				std::string path = v_paths[i];
				if(not path.empty()){
					std::stringstream path_s(path);
					char current;

					pos_prec = 0;
					getline(path_s,path,':');

					std::size_t multiple_pos_separator = path_s.str().find('#');
					if(multiple_pos_separator == std::string::npos){
						getline(path_s,parse_pos);
						position = atoi(parse_pos.c_str());
						if(DEBUG){cout<<"Position initiale (unique) : parse = "<<parse_pos<<" | valeur : "<<position<<endl;}

						reads.push_back({anchor_value,position,path});
					}else{
						getline(path_s,parse_pos,'#');
						position = atoi(parse_pos.c_str());
						if(DEBUG){cout<<"Position initiale (multiple) : parse = "<<parse_pos<<" | valeur : "<<position<<endl;}
						reads.push_back({anchor_value,position,path});
						pos_prec = position;

						path_s.get(current);

						while(not path_s.eof()){
							position = pos_prec + (int)((unsigned char)current);
							pos_prec = position;

							reads.push_back({anchor_value,position,path});
							if(DEBUG){cout<<"--- Position suivante : + "<<(int)current<< " = " <<position<<endl;}

							path_s.get(current);
						}
					}
				}
			}
			//ENDSPLIT
		}


		//Reading paths for the negative anchor
		getline(in,parse,(char)255);
		if(not parse.empty()){											

			//The anchor value is negative
			anchor_value = -anchor_value;

			//SPLIT
			vector<string> v_paths = split(parse,(char)254);
			for(uint i(0);i<v_paths.size();i++){
				std::string path = v_paths[i];
				if(not path.empty()){
					std::stringstream path_s(path);
					char current;

					pos_prec = 0;
					getline(path_s,path,':');

					std::size_t multiple_pos_separator = path_s.str().find('#');
					if(multiple_pos_separator == std::string::npos){
						getline(path_s,parse_pos);
						position = atoi(parse_pos.c_str());
						if(DEBUG){cout<<"Position initiale (unique) : parse = "<<parse_pos<<" | valeur : "<<position<<endl;}

						reads.push_back({anchor_value,position,path});
					}else{
						getline(path_s,parse_pos,'#');
						position = atoi(parse_pos.c_str());
						if(DEBUG){cout<<"Position initiale (multiple) : parse = "<<parse_pos<<" | valeur : "<<position<<endl;}
						reads.push_back({anchor_value,position,path});
						pos_prec = position;

						path_s.get(current);

						while(not path_s.eof()){
							position = pos_prec + (int)((unsigned char)current);
							pos_prec = position;

							reads.push_back({anchor_value,position,path});
							if(DEBUG){cout<<"--- Position suivante : + "<<(int)current<< " = " <<position<<endl;}

							path_s.get(current);
						}
					}
				}
			}
			//ENDSPLIT
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