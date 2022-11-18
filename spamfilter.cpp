#include <iostream>
#include <fstream>
#include <string>
using namespace std;

typedef struct {
    int count;
    string type; 
} data_feature;

typedef struct{
    double prob;
    string type;
} probability;

//trained data
class trained{
    private:
        int amount;
        data_feature *data;
        int word_amount;
    public:
        trained();
        void add_data(string line);
        void print();
        data_feature get_data(int i);
        probability get_prob(int i);
        probability get_prob(string word);
        int get_amount();
        int get_word_amount();
};
trained::trained(){
    amount = 0;
    word_amount=0;
}
void trained::add_data(string line){
    word_amount++;
    if(amount==0){//0개면
        amount++;
        data = new data_feature[amount];
        data[0].count = 1;
        data[0].type.assign(line);
        return;
    }//맞는게 있다면
    for(int i=0;i<amount;i++){
        if(data[i].type.compare(line)==0){
            data[i].count++;
            return;
        }
    }
    //맞는게 없으면
    amount++;
    data_feature *new_data = new data_feature[amount];
    for(int i=0;i<amount-1;i++){
        new_data[i].count = data[i].count;
        new_data[i].type.assign(data[i].type);
    }
    new_data[amount-1].count = 1;
    new_data[amount-1].type.assign(line);
    delete []data;
    data = NULL;
    data = new_data;
    return;
}
void trained::print(){
    for(int i=0;i<amount;i++){
        cout << data[i].count << " " << data[i].type << "/\n";
    }
}
data_feature trained::get_data(int i){
    return data[i];
}
int trained::get_amount(){
    return amount;
}
int trained::get_word_amount(){
    return word_amount;
}
probability trained::get_prob(int i){
    probability prob;
    prob.prob = (double)data[i].count/(double)amount;
    prob.type = data[i].type;
    return prob;
}
probability trained::get_prob(string word){
    probability prob;
    for(int i=0;i<amount;i++){
        if((data[i].type)==word){
            prob.prob = (double)data[i].count/(double)amount;
            prob.type = data[i].type;
        }
    }
    return prob;
}

string eliminate(string line){ //eliminiate special characters, organize sentences
    string eli_line;
    string final_line;
    for(int i=0;i<line.length();i++){
        if((line[i]>='A'&&line[i]<='Z')||(line[i]>='a'&&line[i]<='z')||(line[i]>='0'&&line[i]<='9')||(line[i]==' ')){
            eli_line = eli_line + line[i];
        }
    }
    for(int i=0;i<eli_line.length();i++){
        if(eli_line[i]>='A'&&eli_line[i]<='Z'){
            eli_line[i] = eli_line[i]+32;
        }
    }
    for(int i=0;i<eli_line.length();i++){
        if(eli_line[i]==' '&&eli_line[i+1]==' ') continue;
        final_line = final_line+eli_line[i];
    }
    return final_line;
}

trained parse_string(string line, trained data){
    string parse;
    int count=0, decision=0;
    int prev_count = 0;
    while(decision!=-1){
        prev_count = count;
        count = line.find(" ",count);
        if(count==0){
            count++;
            continue;
        }
        parse = line.substr(prev_count, count-prev_count);
        if(parse.empty()){
            decision = line.find(" ",count++);
            continue;
        } 
        decision = line.find(" ",count++);
        data.add_data(parse);
    }
    return data;
}

class trained_data{
    private:
        trained data;
    public:
        trained_data(string line);
        int get_amount();
        probability get_prob(int i);
        probability get_prob(string word);
};

trained_data::trained_data(string file){
    string line;
    string data_line;
    int parse;
    ifstream openFile(file);
    if(openFile.is_open()){
        getline(openFile,line);
    } else{
        cout << "no file" << endl;
        return;
    }
    while(getline(openFile,line)){ //get lines
        if(line.find(",")!=-1){
            parse = line.rfind(",");
            data_line = line.substr(parse+2);
            data_line = eliminate(data_line);
            data = parse_string(data_line,data);
            while(getline(openFile,line)){
                if((line.find("\"")==-1)){
                    data_line = eliminate(line);
                    data = parse_string(data_line,data);
                }
                else{
                    parse = line.find("\"");
                    if(line.find("\"",parse+1)==-1){
                        data_line = eliminate(line);
                        data = parse_string(data_line,data);
                        break;
                    }
                    else{
                        data_line = eliminate(line);
                        data = parse_string(data_line,data);
                    }
                }
            }
        }
    }
    openFile.close();
}

int trained_data::get_amount(){
    return data.get_amount();
}

probability trained_data::get_prob(int i){
    return data.get_prob(i);
}
probability trained_data::get_prob(string word){
    return data.get_prob(word);
}

class test{
    private:
        int set_data;
        trained *data;
    public:
        test(string line, int number);
        int get_amount(int i);
        probability get_prob(int i);
        probability get_prob(int i, string word);
};

test::test(string file, int number){
    number = set_data;
    data = new trained[set_data];
    string line;
    string data_line;
    int parse;
    int count=0;
    ifstream openFile(file);
    if(openFile.is_open()){
        getline(openFile,line);
    } else{
        cout << "no file" << endl;
        return;
    }
    while(getline(openFile,line)&&count<=number){
        if(line.find(",")!=-1){
            parse = line.rfind(",");
            data_line = line.substr(parse+2);
            data_line = eliminate(data_line);
            data[count] = parse_string(data_line,data[count]);
            while(getline(openFile,line)){
                if((line.find("\"")==-1)){
                    data_line = eliminate(line);
                    data[count] = parse_string(data_line,data[count]);
                }
                else{
                    parse = line.find("\"");
                    if(line.find("\"",parse+1)==-1){
                        data_line = eliminate(line);
                        data[count] = parse_string(data_line,data[count]);
                        break;
                    }
                    else{
                        data_line = eliminate(line);
                        data[count] = parse_string(data_line,data[count]);
                    }
                }
            }
        }
        count++;
    }
    openFile.close();
}

int test::get_amount(int i){
    return data[i].get_amount();
}

probability test::get_prob(int i, string word){
    return data[i].get_prob(word);
}

int main(){
    // probability cond_train_ham[ham.get_amount()];
    trained_data train_ham("csv/train/dataset_ham_train100.csv");
    
    cout << train_ham.get_prob("subject").prob << endl;

    // // probability cond_train_spam[spam.get_amount()];
    // trained_data train_spam("csv/train/dataset_spam_train100.csv");
    
    // //enter the number of test cases and file name including paths 
    // test test_ham("csv/test/dataset_ham_test20.csv",20);

    // //enter the number of test cases and file name including paths 
    // test test_spam("csv/test/dataset_spam_test20.csv",20);

    return 0;
}






// class test_ham{
//     private:
//         element *data;
//     public:
//         test_ham();
//         string get_data();
//         string get_type();
//         int get_num();
// };

// test_ham::test_ham(){
//     int count=0;
//     int decision = 0;
//     int parse_1 =0, parse_2=0, parse_3=0;
//     ifstream openFile("csv/test/dataset_ham_test20.csv");
//     data = new element[20];
// 	if(openFile.is_open()){
// 		string line;
//         string mail_data;
//         getline(openFile, line); //eliminate first line
// 		// while(getline(openFile, line)){
//             getline(openFile, line);
// 			if(line.find(",")){
//                 parse_1 = line.find(",");
//                 // data[count].num = stoi(line.substr(0,parse_1));
//                 parse_2 = line.find(",",parse_1+1) - parse_1;
//                 parse_3 = line.find(",",parse_1+1);
//                 // data[count].type = line.substr(parse_1+1,parse_2-1);
//                 mail_data = mail_data.append(line.substr(parse_3+2));
//                 while(getline(openFile, line)){
//                     cout << line << endl;
//                     if(line.find("\"")<=0){
//                         mail_data = mail_data+line.substr(0,line.length()-1);
//                         break;
//                     } else{
//                         mail_data = mail_data+ line;
//                     }
//                 }
                
//             // }
// 		}
//         // cout << mail_data << endl;
// 		openFile.close();
// 	}
// }