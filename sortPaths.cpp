#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <math.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#define DEBUG 0

struct read_encoding{
	int anchor_number;
	int read_position;
	string path_direction;
	string missmatches_encoding;
};



int charToInt(char c) {
	switch (c) {
		case 'A': return 1;
		case 'C': return 2;
		case 'G': return 3;
	}
	return 4;
}



bool acompare(read_encoding y, read_encoding x) {
	if(abs(x.anchor_number)>abs(y.anchor_number)){return true;}
	if(abs(x.anchor_number)<abs(y.anchor_number)){return false;}
	//Pour comparer les signes
	if(abs(x.anchor_number)==abs(y.anchor_number)){
		if((x.anchor_number)<(y.anchor_number)){return true;}
		if((x.anchor_number)>(y.anchor_number)){return false;}
	}

	if(x.read_position>y.read_position){return true;}
	if(x.read_position<y.read_position){return false;}

	if(x.path_direction>y.path_direction){return true;}
	if(x.path_direction<y.path_direction){return false;}



	return false;
}



uint stringToInt(const string& str){
	uint res(0), acc(1);
	for(uint i(0);i<str.size();++i){
		res+=charToInt(str[i])*acc;
		acc*=4;
	}
	return res;
}

uint32_t anchor_compression(int prec_anchor_number, int anchor_number){
	unsigned int anchor_number_abs;
	uint32_t anchor_read;

	//Retrieving anchor absolute value
	anchor_number_abs = abs(anchor_number);
	anchor_number_abs -= abs(prec_anchor_number);

	//Compressing anchor absolute value into 4 Bytes
	//Writing anchor_number absolute value
	anchor_read = (uint32_t)anchor_number_abs;
	/*
	anchor_read[0] = (uint32_t)((anchor_number_abs >> 24) & 0xFF);
	anchor_read[1] = (uint32_t)((anchor_number_abs >> 16) & 0xFF);
	anchor_read[2] = (uint32_t)((anchor_number_abs >> 8) & 0xFF);
	anchor_read[3] = (uint32_t)((anchor_number_abs) & 0xFF);*/

	return anchor_read;
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
	//FILE OPEN
	vector<read_encoding> reads;
	//INPUT

	string parse;
	int anchor_number,read_position;
	while(not in.eof()){
		getline(in,parse,':');
		if(not parse.empty()){
			anchor_number=(stoi(parse));
		}else{
			anchor_number=0;
		}
		getline(in,parse,':');
		string path_direction((parse));
		getline(in,parse,':');
		if(not parse.empty()){
			read_position=(stoi(parse));
		}else{
			read_position=0;
		}
		getline(in,parse,'\n');
		string mismatches_encoding(parse);
		reads.push_back({anchor_number,read_position,path_direction, mismatches_encoding});
	}

	sort(reads.begin(),reads.end(),acompare);
	//INPUT SORTED









	ofstream streampaths("sortedPaths_perfect13_pos");
	ofstream streammismatchs("sortedMis_perfect13_pos");

	//COMPRESSION
	int pos_diff;
	read_encoding read_prec;
	read_prec.anchor_number = 0;
	uint j;
	std::string positionMismatch;
	char letterMismatch;
	vector<int> vPos;
	vector<int> vSeq;
	bool first = false;

	uint32_t anchor_read;
	char anchor_separator = (char)255;
	char path_separator = (char)254;

	for(uint i(0);i<reads.size();++i){

	    //If anchor is 0
	    
		if(reads[i].anchor_number == 0){
			streampaths<<reads[i].anchor_number<<anchor_separator<<path_separator<<reads[i].path_direction<<':'<<reads[i].read_position<<anchor_separator;
		}else{
	        //ANCHOR NUMBER

			
			if(abs(reads[i].anchor_number) != abs(read_prec.anchor_number)){
				//If the previous anchor had positives but no negative, add a separator
				if(read_prec.anchor_number>0){
					streampaths<<anchor_separator;
				}

				//End precedent line
				streampaths<<anchor_separator;
				if(DEBUG){streampaths<<";"<<endl;}

				//Write new anchor value
				anchor_read = anchor_compression(read_prec.anchor_number, reads[i].anchor_number);
				streampaths<<anchor_read;
				streampaths<<anchor_separator;
				if(DEBUG){streampaths<<"+";}

				//If there's no positive for the anchor
				if(reads[i].anchor_number < 0){
					streampaths<<anchor_separator;
					if(DEBUG){streampaths<<"-";}
				}

				//streampaths<<anchor_read;
			}else{
				//If, for the same anchor, we're done with positives and start negatives
				if(reads[i].anchor_number < read_prec.anchor_number){
					//Mark the end of the positives
					streampaths<<anchor_separator;
					if(DEBUG){streampaths<<"-";}
				}
			}

			
		

	        //PATH DIRECTION
	        
	        	/*
	        
			if((reads[i].path_direction.compare(read_prec.path_direction) == 0) && (reads[i].anchor_number == read_prec.anchor_number)){

	            //READ POSITION
				if(reads[i].read_position != read_prec.read_position){
					//If the current position is different from the previous position for the same path, then we only display the difference between the two positions, assumed to be a single byte
			//		pos_diff = (reads[i].read_position-read_prec.read_position);
			//		pos_diff = 0;
					if(first){
						streampaths<<"#";
					}
					streampaths<<(unsigned char)(pos_diff);
					first = false;
				//There's multiple occurence of the same anchor-path-position
				}else{
					pos_diff = 0;
					if(first){
						streampaths<<"#";
					}
					streampaths<<(unsigned char)(pos_diff);
					first = false;
				}

				*/

				/*
				
			}else{
				//First position followed by a separator
			//	unsigned char pos = (unsigned char)(reads[i].read_position);
				unsigned char pos = '1';
				streampaths<<path_separator<<reads[i].path_direction<<":"<<pos;
				first = true;
			}

				*/

			

			//TEMP position ASCII + separateurs
			char pos_separator = (char)253;
			char position_ascii;

			if(reads[i].path_direction.compare(read_prec.path_direction) != 0){
				streampaths<<path_separator<<reads[i].path_direction<<":";

				//first position to ASCII

				//std::ostringstream pos;
				//pos << (int)reads[i].read_position;
				//std::string position_ascii = std::to_string(reads[i].read_position);
				//position_ascii = '0' + reads[i].read_position;

				if(reads[i].read_position >= 253 && reads[i].read_position <= 255){
					cout<<"Erreur valeur position ancre : "<<reads[i].anchor_number<<" pos : "<< reads[i].read_position <<endl;
					position_ascii = '0';
				}else{
					position_ascii = (unsigned char)reads[i].read_position;
				}
				
				streampaths << position_ascii;
				streampaths << pos_separator;

			}else{
				//next position to ASCII

				//std::ostringstream pos;
				//pos << (int)(reads[i].read_position - read_prec.read_position);
				//position_ascii = '0' + (reads[i].read_position - read_prec.read_position);
				//std::string position_ascii = std::to_string(reads[i].read_position - read_prec.read_position);

				pos_diff = (reads[i].read_position - read_prec.read_position);

				if(pos_diff >= 253 && pos_diff <= 255){
					position_ascii = '0';
				}else{
					position_ascii = (unsigned char)pos_diff;
				}
				
				streampaths << position_ascii;
				streampaths << pos_separator;
			}

			//END TEMP
			
		}


		//MISMATCH
		
		uint next_mismatch = 0;
		int pos_mismatch;
		//If there is a mismatch for this read
		if(reads[i].missmatches_encoding.compare("") != 0){
			//Mismatch reading and separation between the mismatch's position and value
			for(j=0; j<reads[i].missmatches_encoding.size(); j++){
				if(reads[i].missmatches_encoding[j] == ':'){
					letterMismatch = reads[i].missmatches_encoding[j-1];
					positionMismatch = reads[i].missmatches_encoding.substr(next_mismatch,j-1);
					if(next_mismatch == 0){
						pos_mismatch = std::stoi(positionMismatch);
						pos_mismatch = -(pos_mismatch);
					}else{
						pos_mismatch = std::stoi(positionMismatch) - pos_mismatch;
					}
					vPos.push_back(pos_mismatch);
					vSeq.push_back(letterMismatch);

					next_mismatch = j+1;
				}
			}
		}


		//Separator for the reads in the vectors
		vPos.push_back(-1);
		vSeq.push_back(-1);
		
		read_prec = reads[i];
		
	}

	//READING THE VECTORS

	

	//DEBUG
	if(DEBUG){
		streammismatchs<<"Blocs de 1000 reads"<<endl;
		streammismatchs<<"VPos :"<<vPos.size()<<endl;
		streammismatchs<<"VSeq :"<<vSeq.size()<<endl;
		streammismatchs<<endl;
	}

	//If positions vector size doesn't match mismatch vector size
	if(vPos.size() != vSeq.size()){
		streammismatchs<<"Position vector size and Sequence vector size not matching."<<endl<<endl;
	}

	uint vSize = max(vPos.size(), vSeq.size());
	uint cmpt_v; 
	uint a;
	unsigned char mismatch4;
	int cmpt_bit = 8;

	for(a=0; a<vSize; a = a+cmpt_v){

		//DEBUG
		if(DEBUG){
			streammismatchs<<"==================== "<< a << " = " << endl << endl;
		}

		//If there's more than 1000 reads left to read, write 1000 reads
		if((vSize - a) >= 1000){
			cmpt_v = 1000;
		//Else read the rest
		}else{
			cmpt_v = vSize-a;
		}

		//Positions
		//Loop for at most 1000 reads
		for(uint k=a; k<(a+cmpt_v); k++){
			if(vPos[k] != -1){	//If not a separator
				//Current position is written as a char for compression
				char pos = '0' + vPos[k];
				streammismatchs<<pos;
			}
		}

		//DEBUG
		if(DEBUG){
			streammismatchs<<endl<<endl;
		}

		//Mismatch Values
		//Loop for at most 1000 reads
		for(uint k=a; k<(a+cmpt_v); k++){
			if(vSeq[k] != -1){	//If not a separator
				//Mismatch value added to a character, after the character's bits are moved two places to the left
				mismatch4 <<= 2;
				mismatch4 += vSeq[k]-1;
				cmpt_bit -=2;

				//If the character as been completed
				if(cmpt_bit == 0){
					streammismatchs<<mismatch4;
					cmpt_bit = 8;
				}
			}
		}

		//DEBUG
		if(DEBUG){
			streammismatchs<<endl<<endl;
		}

	}


	//Dernière marque pour la fin
	streampaths<<anchor_separator;
	if(DEBUG){streampaths<<";";}
}

/*

--MaJ Git
git pull

--Rebuild
make rebuild

--Créations des chemins
./bgreat -u sim1.fa -k 31 -g ecoli_31.unitigs.fa -C

--Tri des chemins
./sortstreamPaths streampaths0 > sortedstreamPaths0

--Compression gzip -1
gzip -1 sortedstreamPaths0 -c > sortedstreamPaths01.gz

--list + tailles
ll -h

--Compression gzip -9
gzip -9 sortedstreamPaths0 -c > sortedstreamPaths09.gz

*/







