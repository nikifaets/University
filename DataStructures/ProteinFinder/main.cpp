#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <assert.h>
#include <fstream>

using namespace std;

unordered_set<int> untranslatable_proteins;
const char STOP_CODON = '*';

/// It is not specified in the problem description, therefore I assume one protein can appear multiple times in the DNA, hence the vector.
void search_protein(string& DNA, const string& protein, vector<int>*& appearances){

    int protein_len = protein.size();
    for(int i=0; i<DNA.size(); i++){

        int it = 0;
        while(DNA[i+it] == protein[it]){

            it++;
            if(it >= protein_len){
                
                appearances->push_back(i);
                i += it;
                break;
            }

        }

    }
}

void protein_to_amino(string& amino_seq, const string& protein, unordered_map<string, char>& amino_names){

    assert(protein.size() % 3 == 0);
    string res;
    for(int i=0; i<protein.size(); i+=3){

        string amino = protein.substr(i, 3);

        
        assert(amino_names.count(amino) > 0);
        if(amino_names[amino] == STOP_CODON){

            if(i+3 < protein.size()) return;
            break;
        }

        res += amino_names[amino];

    }

    amino_seq = res;
}

/// Make a map that associates protein_id with aminoacid sequence.
void fill_protein_aminos(
    unordered_map<int, string>& proteins,
    unordered_map<string, char>& amino_names,
    unordered_map<int, string>& protein_id_amino){

    unordered_map<int, string>::iterator it = proteins.begin();

    for(it; it != proteins.end(); ++it){

        string protein_amino;
        protein_to_amino(protein_amino, it->second, amino_names);

        if(protein_amino == ""){

            cout << "Protein " << it->first << " has wrong amino sequence." << endl;
            untranslatable_proteins.insert(it->first);
            continue;
        }
        protein_id_amino[it->first] = protein_amino;
        
    }
}

/// Remember all the start indices of each protein. Map protein_id to index.
void fill_protein_appearances(
    unordered_map<int, string>& proteins,
    string& DNA,
    unordered_map<int, vector<int>*>& appearances){

    unordered_map<int, string>::iterator it = proteins.begin();

    for(it; it != proteins.end(); ++it){

        vector<int>* curr_apps = new vector<int>();
        assert(curr_apps != nullptr);
        search_protein(DNA, it->second, curr_apps);
        if(curr_apps->size() > 0) appearances[it->first] = curr_apps;
    }
        
}

void delete_appearances(unordered_map<int, vector<int>*> apps){

    unordered_map<int, vector<int>*>::iterator it = apps.begin();
    for(it; it != apps.end(); ++it){

        delete it->second;
    }
}

void print_appearances(int id, unordered_map<int, vector<int>*>& appearances){

    cout << "Starting indices of protein " << id << ": ";
    vector<int>* curr_apps = appearances[id];
    for(int i=0; i<curr_apps->size(); i++){

        cout << curr_apps->at(i) << " ";
    }
    cout << endl;
}

bool read_DNA(string& filename, string& DNA){

    ifstream in(filename);
    if(! in.good()) return false;

    if(! (in >> DNA)) return false;

    return true;
}

bool read_proteins(string& filename, unordered_map<int, string>& proteins){

    ifstream in(filename);

    if(! in.good()) return false;
    while(in.good()){

        int id;
        string protein;

        if(! (in >> id)) return false;
        if(! (in >> protein)) return false;

        proteins[id] = protein;
    }

    return true;
}

bool read_amino(string& filename, unordered_map<string, char>& amino_names){

    fstream in(filename);
    if(! in.good()) return false;

    while( in.good()){

        string amino;
        string name;

        if(! (in >> amino)) return false;
        if(! (in >> name)) return false;
        amino_names[amino] = name[0];
    }
    
    
    return true;
}

int main(){


    
    string DNA_f;
    string proteins_f;
    string amino_f;

    cout << "Provide file names for DNA sequence, proteins and aminoacids." << endl;
    cin >> DNA_f >> proteins_f >> amino_f;

    string DNA;
    unordered_map<int, string> proteins;
    unordered_map<string, char> amino_names;
    unordered_map<int, string> protein_aminos;
    unordered_map<int, vector<int>*> protein_appearances;

    if(!(
        read_DNA(DNA_f, DNA) &&
        read_proteins(proteins_f, proteins) &&
        read_amino(amino_f, amino_names))){

            cout << "Error reading files. " << endl;
            return 0;
        }

    ///Preprocess everything so that each requests takes average O(1) to execute
    fill_protein_appearances(proteins, DNA, protein_appearances);
    fill_protein_aminos(proteins, amino_names, protein_aminos);

    int n;
    cin >> n;
    
    ///Start processing requests
    for(int i=0; i<n;  i++){

        int req_id;
        cin >> req_id;

        if(proteins.count(req_id) == 0){

            cout << "No protein with id " << req_id << "." << endl;
            continue;
        }

        if(protein_appearances.count(req_id) == 0){

            cout << "Protein is not present in the provided DNA sample." << endl;
            continue;
        }

        print_appearances(req_id, protein_appearances);
        if(untranslatable_proteins.count(req_id) > 0){

            cout << "This protein is untranslatable. " << endl;
        }

        else{

            cout << "Amino sequence: " << protein_aminos[req_id];
        }
    }

    delete_appearances(protein_appearances);
    return 0;
    
}