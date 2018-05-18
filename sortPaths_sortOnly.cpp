#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <math.h>

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
		if((x.anchor_number)<(y.anchor_number)){return false;}
		if((x.anchor_number)>(y.anchor_number)){return true;}
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

string anchor_compression(int prec_anchor_number, int anchor_number){
	unsigned int anchor_number_abs;
	std::string anchor_read;

	//Retrieving anchor absolute value
	anchor_number_abs = abs(anchor_number);
	anchor_number_abs -= abs(prec_anchor_number);

	//Compressing anchor absolute value into 4 Bytes
	//Writing anchor_number absolute value
	anchor_read += (uint32_t)anchor_number_abs;

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

	std::string fo = argv[1];
	ofstream streampaths("tri_"+fo);

	for(uint i(0);i<reads.size();++i){
		streampaths<<reads[i].anchor_number;
		if(not reads[i].path_direction.empty()){
			streampaths<<":"<<reads[i].path_direction;	
		} 
		streampaths<<':'<<reads[i].read_position<<":"<<endl;
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







