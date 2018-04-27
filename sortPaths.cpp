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
	//~ string missmatches_encoding;
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
		//~ string mismatches_encoding(parse);
		reads.push_back({anchor_number,read_position,path_direction});
	}

	sort(reads.begin(),reads.end(),acompare);
	//INPUT SORTED




	//A MODIFIER POUR LA COMPRESSION
	int anchor_number_abs;
	int pos_diff;
	read_encoding read_prec;
	bool sign_change = false;

	for(uint i(0);i<reads.size();++i){
		sign_change = false;

	    //If anchor is 0
		if(reads[i].anchor_number == 0){
			cout<<reads[i].anchor_number<<":"<<reads[i].path_direction<<":"<<reads[i].read_position;
		}else{
	        //ANCHOR NUMBER
			if(abs(reads[i].anchor_number) != abs(read_prec.anchor_number)){
	            //Retrieving absolute value
				anchor_number_abs = abs(reads[i].anchor_number);
				anchor_number_abs -= abs(read_prec.anchor_number);

	           //Writing anchor_number absolute value
				cout<<";";
				cout<<endl;
				cout<<anchor_number_abs<<":"; 

				//Retrieving the first sign
				sign_change = true;
				if(reads[i].anchor_number < 0){
					cout<<"-"; 
				}else{
					cout<<"+"; 
				}

	        //ANCHOR SIGN
	        //If sign changes from positive to negative
			}else if(reads[i].anchor_number < 0 && read_prec.anchor_number > 0){
				sign_change = true;
				cout<<"-"; 
	        //If sign changes from negative to positive
			}else if(reads[i].anchor_number > 0 && read_prec.anchor_number < 0){
				sign_change = true;
				cout<<"+"; 
			}

	        //PATH DIRECTION
			if(reads[i].path_direction.compare(read_prec.path_direction) == 0 && !sign_change){
	            //READ POSITION
				if(reads[i].read_position != read_prec.read_position){
					pos_diff = (reads[i].read_position-read_prec.read_position);
					cout<<"#"<<pos_diff;
				}
			}else{
				cout<<"|"<<reads[i].path_direction<<":"<<reads[i].read_position;
			}

		}
	    //Ajout du reste
	    //cout<<":"<<reads[i].path_direction<<":"<<reads[i].read_position<<endl;
		read_prec = reads[i];
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
./sortPaths paths0 > sortedPaths0

--Compression gzip -1
gzip -1 sortedPaths0 -c > sortedPaths01.gz

--list + tailles
ll -h

--Compression gzip -9
gzip -9 sortedPaths0 -c > sortedPaths09.gz

*/







