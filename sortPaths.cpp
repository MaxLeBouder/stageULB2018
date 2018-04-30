#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <math.h>



using namespace std;



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

	ofstream streampaths("sortedPaths");
	ofstream streammismatchs("sortedMis");

	//A MODIFIER POUR LA COMPRESSION
	int anchor_number_abs;
	int pos_diff;
	read_encoding read_prec;
	bool sign_change = false;
	uint j;
	std::string positionMismatch;
	char letterMismatch;
	vector<int> vPos;
	vector<int> vSeq;

	for(uint i(0);i<reads.size();++i){
		sign_change = false;

	    //If anchor is 0
		if(reads[i].anchor_number == 0){
			streampaths<<reads[i].anchor_number<<":"<<reads[i].path_direction<<":"<<reads[i].read_position;
		}else{
	        //ANCHOR NUMBER
			if(abs(reads[i].anchor_number) != abs(read_prec.anchor_number)){
	            //Retrieving absolute value
				anchor_number_abs = abs(reads[i].anchor_number);
				anchor_number_abs -= abs(read_prec.anchor_number);

	           //Writing anchor_number absolute value
				streampaths<<";";
				streampaths<<endl;
				streampaths<<anchor_number_abs<<":"; 

				//Retrieving the first sign
				sign_change = true;
				if(reads[i].anchor_number < 0){
					streampaths<<"-"; 
				}else{
					streampaths<<"+"; 
				}

	        //ANCHOR SIGN
	        //If sign changes from positive to negative
			}else if(reads[i].anchor_number < 0 && read_prec.anchor_number > 0){
				sign_change = true;
				streampaths<<"-"; 
	        //If sign changes from negative to positive
			}else if(reads[i].anchor_number > 0 && read_prec.anchor_number < 0){
				sign_change = true;
				streampaths<<"+"; 
			}

	        //PATH DIRECTION
			if(reads[i].path_direction.compare(read_prec.path_direction) == 0 && !sign_change){
	            //READ POSITION
				if(reads[i].read_position != read_prec.read_position){
					pos_diff = (reads[i].read_position-read_prec.read_position);
					streampaths<<"#"<<pos_diff;
				}
			}else{
				streampaths<<"|"<<reads[i].path_direction<<":"<<reads[i].read_position;
			}

		}
	    //Ajout du reste
	    //cout<<":"<<reads[i].path_direction<<":"<<reads[i].read_position<<endl;

		//Traitement du mismatch
		//Si il y a un mismatch pour ce read
		uint next_mismatch = 0;
		if(reads[i].missmatches_encoding.compare("") != 0){
			//Lecture et separation du missmatch
			for(j=0; j<reads[i].missmatches_encoding.size(); j++){
				if(reads[i].missmatches_encoding[j] == ':'){
					letterMismatch = reads[i].missmatches_encoding[j-1];
					positionMismatch = reads[i].missmatches_encoding.substr(next_mismatch,j-1);
					vPos.push_back(std::stoi(positionMismatch));
					vSeq.push_back(letterMismatch);

					next_mismatch = j+1;
				}
			}
			//TEMPORAIRE
			//streammismatchs<<anchor_number_abs<<":";
			//streammismatchs<<positionMismatch;
			//streammismatchs<<letterMismatch;
			
		}


		//Séparateurs pour les mismatchs dans le vecteur
		vPos.push_back(-1);
		vSeq.push_back(-1);

		//test
		//streammismatchs<<reads[i].missmatches_encoding<<endl;

		read_prec = reads[i];
	}


	//Parcours des vecteurs par bloc de 1000 
	/* Temporaires */
	streammismatchs<<"1000"<<endl;
	streammismatchs<<vPos.size()<<endl;
	streammismatchs<<vSeq.size()<<endl;

	//Parcours du vecteur des positions de mismatch, encodage des positions en ASCII
	for(uint i=0; i<vPos.size(); i++){
		if(vPos[i] != -1){
			char pos = '0' + vPos[i];
			streammismatchs<<pos;
		}
		else{
			streammismatchs<<";";
		}
	}

	streammismatchs<<endl<<endl;

	//Parcours du vecteur de valeur de mismatch, encodage des mismatch 4 par 4
	unsigned char mismatch4;
	int cmpt_bit = 8;
	for(uint i=0; i<vSeq.size(); i++){
		if(vSeq[i] != -1){
			mismatch4 += vSeq[i]-1;
			mismatch4 <<= 2 ;
			cmpt_bit -=2;

			if(cmpt_bit == 0){
				streammismatchs<<mismatch4;
				cmpt_bit = 8;
			}
		}
	}
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







