#include <iostream>
#include <fstream>
#include <string>
using namespace std;

typedef struct {
    int count;
    string type; 
} data_feature;

//data structure for trained data
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
        long double get_only_prob(string word);
        int get_amount();
        int get_word_amount();
        void quick_sort();
};
trained::trained(){
    amount = 0;
    word_amount=0;
}
void trained::add_data(string line){
    word_amount++;
    //if empty
    if(amount==0){
        amount++;
        data = new data_feature[amount];
        data[0].count = 1;
        data[0].type.assign(line);
        return;
    }//if there is a word
    for(int i=0;i<amount;i++){
        if(data[i].type.compare(line)==0){
            data[i].count++;
            return;
        }
    }
    //make more spaces
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

void trained::print(){//print out whole data
    for(int i=0;i<amount;i++){
        cout << data[i].count << " " << data[i].type << "/\n";
    }
    cout << word_amount << endl;
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

long double trained::get_only_prob(string word){
    long double prob=0.00002;
    for(int i=0;i<amount;i++){
        if((data[i].type)==word){
            prob = (long double)data[i].count/(long double)word_amount;
        }
    }
    return prob;
}


void swap(data_feature* a, data_feature* b)
{
    data_feature temp;
    temp = *a;
    a->count = b->count;
    a->type = b->type;
    b->count = temp.count;
    b->type = temp.type;
}

int partition(data_feature arr[], int low, int high)
{
    int pivot = arr[high].count; 
    int i = (low- 1); 
    for (int j = low; j <= high - 1; j++) {
        if (arr[j].count > pivot) {
            i++; 
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(data_feature arr[], int low, int high)
{
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void trained::quick_sort(){
    int n = amount-1;
    quickSort(data, 0, n - 1);
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
        long double get_only_prob(string word);
        void sort();
        void print();
        data_feature get_data(int i);
        int get_word_amount();
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

long double trained_data::get_only_prob(string word){
    return data.get_only_prob(word);
}

void trained_data::sort(){
    data.quick_sort();
}

void trained_data::print(){
    data.print();
}

data_feature trained_data::get_data(int i){
    return data.get_data(i);
}


int trained_data::get_word_amount(){
    return data.get_word_amount();
}

class test{
    private:
        int set_data;
        trained *data;
        string *label;
    public:
        test(string line, int number);
        int get_amount(int i);
        data_feature get_word(int col, int row);
        void assign_label(string label, int i);
        int get_set_data();
};

test::test(string file, int number){
    set_data = number;
    data = new trained[set_data];
    label = new string[set_data];
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
    while(getline(openFile,line)&&count<number){
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

data_feature test::get_word(int cols, int rows){
    data_feature temp;
    temp = data[cols].get_data(rows);
    return temp;
}

void test::assign_label(string p_label, int i){
    label[i].assign(p_label);
}

int test::get_set_data(){
    return set_data;
}
int main(){
    long double th = 0.0; //receive threshold value from user
    long double p_cond_spam = 1.0;
    long double p_cond_ham = 1.0;
    long double result=0.0;
   
   do{
    cout << "put threshold value (0~1): ";
    cin >> th;
   }while(th>1);
    
    cout<< "=========data processing============" << endl;

    //get data set and sort
    trained_data train_ham("csv/train/dataset_ham_train100.csv");
    train_ham.sort();
    trained_data train_spam("csv/train/dataset_spam_train100.csv");
    train_spam.sort();


    cout<< "=========data processing============" << endl;

    //enter the number of test cases and file name including paths 
    test test_ham("csv/test/dataset_ham_test20.csv",20);

   //enter the number of test cases and file name including paths 
    test test_spam("csv/test/dataset_spam_test20.csv",20);

     
    
    //result test ham
    for(int k=0;k<test_ham.get_set_data();k++){
        p_cond_ham = 1.0;
        p_cond_spam = 1.0;
        for(int i=0;i<test_ham.get_amount(k);i++){
            for(int j=0;j<test_ham.get_word(k,i).count;j++){
                p_cond_ham = p_cond_ham * (train_ham.get_only_prob(test_ham.get_word(k,i).type));
                p_cond_spam = p_cond_spam * (train_spam.get_only_prob(test_ham.get_word(k,i).type)); 
            }
        }
        result = p_cond_spam/(p_cond_ham+p_cond_spam);
          if(result > th){
            cout << "Test ham Mail " << k+1 << " is spam mail" << endl;
            test_ham.assign_label("Spam",k);
            // cout << k+1 << ":" << result << endl;
        }
        else{
            cout << "Test ham mail " << k+1 << " is not spam mail" << endl;
            test_ham.assign_label("Not spam",k);
            //  cout << k+1 << ":" << result << endl;
        }
    }

    //result test spam
    for(int k=0;k<test_spam.get_set_data();k++){
        p_cond_ham = 1.0;
        p_cond_spam = 1.0;
        for(int i=0;i<test_spam.get_amount(k);i++){
            for(int j=0;j<test_spam.get_word(k,i).count;j++){
                p_cond_ham = p_cond_ham * (train_ham.get_only_prob(test_spam.get_word(k,i).type));
                p_cond_spam = p_cond_spam * (train_spam.get_only_prob(test_spam.get_word(k,i).type)); 
            }
        }
        result = p_cond_spam/(p_cond_ham+p_cond_spam);
        if(result > th){
            cout << "Test spam Mail " << k+1 << " is spam mail" << endl;
             test_spam.assign_label("Spam",k);
            // cout << k+1 << ":" << result << endl;
        }
        else{
             cout << "Test spam mail " << k+1 << " is not spam mail" << endl;
             test_spam.assign_label("Not spam",k);
            //  cout << k+1 << ":" << result << endl;
        }
        
    }    
    return 0;
}
