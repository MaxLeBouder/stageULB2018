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

			//Spliting each path into a vector
			vector<string> v_paths = split(parse,(char)254);
			for(uint i(0);i<v_paths.size();i++){
				std::string path = v_paths[i];
				if(not path.empty()){
					//Using a stringstream to read the path
					std::stringstream path_s(path);
					char current;

					pos_prec = 0;
					//Retrieving the path until the start of positions delimiter
					getline(path_s,path,':');

					//If there's a # delimiter, then there will be multiple positions for this path
					std::size_t multiple_pos_separator = path_s.str().find('#');
					//If there's a single position
					if(multiple_pos_separator == std::string::npos){
						//Then all the remaining string in the path stream is the position
						getline(path_s,parse_pos);
						//Conversion from string to int
						position = atoi(parse_pos.c_str());
						if(DEBUG){cout<<"Position initiale (unique) : parse = "<<parse_pos<<" | valeur : "<<position<<endl;}

						//Pushing the read
						reads.push_back({anchor_value,position,path});
					}else{
						//Retrieving the initial position, until the multiple position delimiter, which is discarded
						getline(path_s,parse_pos,'#');
						position = atoi(parse_pos.c_str());
						if(DEBUG){cout<<"Position initiale (multiple) : parse = "<<parse_pos<<" | valeur : "<<position<<endl;}

						//Pushing the read with the initial position
						reads.push_back({anchor_value,position,path});
						pos_prec = position;

						//Get the first relative position
						path_s.get(current);

						//While there's still positions to read in the path stream
						while(not path_s.eof()){
							//Since the current position is relative to the previous one
							position = pos_prec + (int)((unsigned char)current);
							pos_prec = position;

							//Push the read with the result position
							reads.push_back({anchor_value,position,path});
							if(DEBUG){cout<<"--- Position suivante : + "<<(int)current<< " = " <<position<<endl;}

							//Get next relative position
							path_s.get(current);
						}
					}
				}
			}
			//end of split paths
		}


		//Reading paths for the negative anchor
		getline(in,parse,(char)255);
		if(not parse.empty()){											

			//The anchor value is negative
			anchor_value = -anchor_value;

			//Spliting each path into a vector
			vector<string> v_paths = split(parse,(char)254);
			for(uint i(0);i<v_paths.size();i++){
				std::string path = v_paths[i];
				if(not path.empty()){
					//Using a stringstream to read the path
					std::stringstream path_s(path);
					char current;

					pos_prec = 0;
					//Retrieving the path until the start of positions delimiter
					getline(path_s,path,':');

					//If there's a # delimiter, then there will be multiple positions for this path
					std::size_t multiple_pos_separator = path_s.str().find('#');
					//If there's a single position
					if(multiple_pos_separator == std::string::npos){
						//Then all the remaining string in the path stream is the position
						getline(path_s,parse_pos);
						//Conversion from string to int
						position = atoi(parse_pos.c_str());
						if(DEBUG){cout<<"Position initiale (unique) : parse = "<<parse_pos<<" | valeur : "<<position<<endl;}

						//Pushing the read
						reads.push_back({anchor_value,position,path});
					}else{
						//Retrieving the initial position, until the multiple position delimiter, which is discarded
						getline(path_s,parse_pos,'#');
						position = atoi(parse_pos.c_str());
						if(DEBUG){cout<<"Position initiale (multiple) : parse = "<<parse_pos<<" | valeur : "<<position<<endl;}

						//Pushing the read with the initial position
						reads.push_back({anchor_value,position,path});
						pos_prec = position;

						//Get the first relative position
						path_s.get(current);

						//While there's still positions to read in the path stream
						while(not path_s.eof()){
							//Since the current position is relative to the previous one
							position = pos_prec + (int)((unsigned char)current);
							pos_prec = position;

							//Push the read with the result position
							reads.push_back({anchor_value,position,path});
							if(DEBUG){cout<<"--- Position suivante : + "<<(int)current<< " = " <<position<<endl;}

							//Get next relative position
							path_s.get(current);
						}
					}
				}
			}
			//end of split paths
		}
		
	}
	//End of reading

	//Writing the path on a file
	ofstream stream_paths("decompressedPaths_ecoli63_100x");

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