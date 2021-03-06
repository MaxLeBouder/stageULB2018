/*****************************************************************************
 *   Bgreat : De Bruijn Graph Read Mapping Tool
 *   Authors: Antoine Limasset
 *   Contact: antoine.limasset@gmail.com
 *   Source: https://github.com/Malfoy/BGREAT
 *
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty ofF
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/
#include <thread>
#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <chrono>
#include <map>
#include <set>
#include <vector>
#include "utils.h"



string intToString(uint64_t n){
	if(n<1000){
		return to_string(n);
	}
	string end(to_string(n%1000));
	if(end.size()==3){
		return intToString(n/1000)+","+end;
	}
	if(end.size()==2){
		return intToString(n/1000)+",0"+end;
	}
	return intToString(n/1000)+",00"+end;
}



uint64_t transform_to_size_t(__uint128_t& n){
	return (uint64_t)n;
}



void printPath(const vector<int32_t>& path, ofstream* file){
	for(size_t i(0); i<path.size(); ++i){
		*file<<path[i]<<'.';
		// cout<<path[i]<<'.';
	}
	*file<<'\n';
	// cout<<'\n';
}



char revCompChar(char c) {
	switch (c) {
		case 'A': return 'T';
		case 'C': return 'G';
		case 'G': return 'C';
	}
	return 'A';
}



string getRepresent(const string& s){
	return min(s,reverseComplements(s));
}



string reverseComplements(const string& s){
	string rc(s.size(),0);
	for (int i((int)s.length() - 1); i >= 0; i--){
		rc[s.size()-1-i]= revCompChar(s[i]);
		// rc[s.size()-1-i]=char2int[(uint)s[i]];
	}
	return rc;
}



string reverseinplace(string& str){
	uint i(str.size()-1),j(0);
	for(; j<str.size()/2; --i, ++j){
		str[i] ^= 4;
		str[j] ^= 4;
		if ((str[i]&3) != 3){str[i]^= 17;}
		if ((str[j]&3) != 3){str[j]^= 17;}
		swap(str[i],str[j]);
	}
	if(str.size()%2==1){
		str[j] ^= 4;
		if ((str[j]&3) != 3){str[j]^= 17;}
	}
	return str;
}



char randNuc(){
	switch (rand()%4){
		case 0:
			return 'A';
		case 1:
			return 'C';
		case 2:
			return 'G';
		case 3:
			return 'T';
	}
	return 'A';
}



string mutate(string& read, int n){
	for(int i(0); i<n; ++i){
		int position(rand()%read.size());
		read[position]=randNuc();
	}
	return read;
}



kmer str2num(const string& str){
	kmer res(0);
	for(uint i(0);i<str.size();i++){
		res<<=2;
		switch (str[i]){
			case 'A':res+=0;break;
			case 'C':res+=1;break;
			case 'G':res+=2;break;
			default:res+=3;break;
		}
	}
	return res;
}



kmer nuc2int(char c){
	switch(c){
		//~ case 'A': return 0;
		case 'C': return 1;
		case 'G': return 2;
		case 'T': return 3;
	}
	return 0;
}



kmer nuc2intrc(char c){
	switch(c){
		case 'A': return 3;
		case 'C': return 2;
		case 'G': return 1;
		//~ case 'T': return 0;
	}
	return 0;
}



//TODO ENCODE THE NUC
string codeMiss(const string& read, const string& ref){
	string res;
	for(uint i(0);i<read.size();++i){
		if(read[i]!=ref[i]){
			res+=to_string(i);
			//~ res+=to_string(i);
			int diff_nuc(nuc2int(read[i])-nuc2int(ref[i]));
			if(diff_nuc<0){diff_nuc+=4;}
			res+=(char)diff_nuc;
			res+=":";
		}
	}
	return res;
}



uint missmatchNumberRaw(const string& seq1, const string& seq2, unsigned int n){
	uint miss(0);
	//~ return 1000;
	for(uint i(0); i<seq2.size(); ++i){
		if(seq2[i]!=seq1[i]){
			if(++miss>n){
				return miss;
			}
		}
	}
	return miss;
}



string compactionEnd(const string& seq1,const string& seq2, uint k){
	uint s1(seq1.size()),s2(seq2.size());
	if(s1==0 or s2==0){return "";}
	string rc2(reverseComplements(seq2)),end1(seq1.substr(s1-k,k)), beg2(seq2.substr(0,k));
	if(end1==beg2){return seq1+(seq2.substr(k));}
	string begrc2(rc2.substr(0,k));
	if(end1==begrc2){return seq1+(rc2.substr(k));}
	return "";
}




string compactionEndNoRC(const string& seq1,const string& seq2, uint k){
	//~ cout<<"compaction"<<endl;
	uint s1(seq1.size()),s2(seq2.size());
	if(s1==0 or s2==0 or s1<=k	or s2<=k){return "";}
	string end1(seq1.substr(s1-k,k)), beg2(seq2.substr(0,k));
	//~ cout<<"ole"<<endl;
	if(end1==beg2){return seq1+(seq2.substr(k));}
	//~ string begrc2(rc2.substr(0,k));
	//~ if(end1==begrc2){return seq1+(rc2.substr(k));}
	return "";
}



kmer rcb(kmer min,uint n){
	kmer res(0);
	kmer offset(1);
	offset<<=(2*n-2);
	for(uint i(0); i<n;++i){
		res+=(3-(min%4))*offset;
		min>>=2;
		offset>>=2;
	}
	return res;
}

void  initRc(/* arguments */){char2int[65]='T';char2int[67]='G';char2int[71]='C';char2int[84]='A';}
