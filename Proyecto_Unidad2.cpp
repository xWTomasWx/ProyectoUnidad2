#include<iostream>
#include<string>
#include<vector>
#include<sstream>
#include<fstream>
#include<cstring>
#include<map>

using namespace std;

struct Guardian{
	string name;
	int powerLvl;
	string mainMaster;
	string village;
	vector <Guardian*> aprendices;
};

struct Village{
	string name;
	vector<string> roads;
};


class GuardiansTree {
public:
    GuardiansTree() : root(nullptr) {}

    void insertGuardian(const string& name, const string& powerLevel, const string& mainMaster, const string& village) {
        Guardian* guardian = new Guardian;
        guardian->name = name;
        guardian->powerLvl = stoi(powerLevel);
        guardian->mainMaster = mainMaster;
        guardian->village = village;
        guardians.push_back(guardian);

        if (root == nullptr) {
            root = guardian;
            cout << "raiz" << std::endl;
        }
        else {
            Guardian* master = findGuardian(mainMaster);
            if(master != nullptr) {
                master->aprendices.push_back(guardian);
            }
        }
    }

    void loadGuardiansFromFile(const string& filename) {
        ifstream file(filename);
        if (!file) {
            cerr << "Error en abrir el archivo: " << filename << endl;
            return;
        }
        string line;
        getline(file, line);
        while (getline(file, line)) {
            istringstream iss(line);
            string name, mainMaster, village;
            string powerLevel;
            getline(iss, name, ',');
            getline(iss, powerLevel , ',');
            getline(iss, mainMaster, ',');
            getline(iss, village);
            insertGuardian(name, powerLevel, mainMaster, village);
            
        }
        file.close();
    }

    void printGuardians() {
        printGuardian(root, 0);
    }
    
    Guardian* getRoot(){
    	return root;
	}

	Guardian* findGuardianFromVillage(Guardian* guardian, const string& village) {
        if (guardian != nullptr) {
            if (strcmp(guardian->village.c_str(),village.c_str()) == 0) {
                return guardian;
            }
            for (Guardian* aprendiz : guardian->aprendices) {
                return findGuardianFromVillage(aprendiz, village);
            }
        }
        return nullptr;
    }

private:
    vector<Guardian*> guardians;
    Guardian* root;

    Guardian* findGuardian(const string& name) {
        for (Guardian* guardian : guardians) {
            if (guardian->name == name) {
                return guardian;
            }
        }
        return nullptr;
    }

    void updateTreeStructure() {
        for (Guardian* guardian : guardians) {
            if (guardian->mainMaster != "") {
                Guardian* master = findGuardian(guardian->mainMaster);
                if (master != nullptr) {
                    master->aprendices.push_back(guardian);
                }
            }
            else {
                root = guardian;
            }
        }
    }

    void printGuardian(Guardian* guardian, int indent) {
        if (guardian != nullptr) {
            std::cout << std::string(indent, ' ') << "- " << guardian->name << " (Nivel de poder: " << guardian->powerLvl << ", Aldea: " << guardian->village << ")" << endl;
            for (Guardian* aprendiz : guardian->aprendices) {
                printGuardian(aprendiz, indent + 4);
            }
        }
    }
};


class VillageGraph{
private:
	map<string, Village> villages;
	
public:
	void addVillage(const string& name){
		Village current;
		current.name;
		villages[name] = current;
	}
	
	void addRoad(const string& villageI, const string& villageD){
		villages[villageI].roads.push_back(villageD);
	}
	
	void showRoads(){
		for(const auto& village : villages){
			cout << "Aldea" << village.first << ":";
			for(const string& road : village.second.roads){
				cout << road << " ";
			}
			cout << endl;
		}
	}
	
	void loadVillageFromFile(const string& fileName){
		ifstream file(fileName);
		if (!file) {
            cerr << "Error en abrir el archivo: " << fileName << endl;
            return;
        }
		string line;
		while(getline(file,line)){
			stringstream ss(line);
			string village, conectedVillages;
			if(getline(ss, village, ',') && getline(ss, conectedVillages)){
				addVillage(village);
				
				stringstream ssRoads(conectedVillages);
				string conectedVillage;
				
				while(getline(ssRoads, conectedVillage)){
					addRoad(village, conectedVillage);
				}
			}
		}
		file.close();
	}
	
	
};

int main(){
	VillageGraph Vgraph;
	Vgraph.loadVillageFromFile("Aldeas.csv");
	Vgraph.showRoads();
	GuardiansTree tree;
	tree.loadGuardiansFromFile("Guardianes.csv");
	tree.printGuardians();
	
	
	return 0;
}
