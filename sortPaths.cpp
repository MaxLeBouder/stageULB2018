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