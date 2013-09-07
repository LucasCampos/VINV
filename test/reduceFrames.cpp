#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>

using namespace std;

void GetFrame(ifstream& ent, int N, vector<double>& x, vector<double>& y) {

	for (int i=0; i<N; i++){
		double lX,lY;
		ent >> lX >> lY;
		if (!ent.eof()){
			x.push_back(lX);
			y.push_back(lY);
		}
	}
}

int main(int argc, char** argv) {

	if(argc < 4){
		cout << "Usage: " << argv[0] << " inFile outFile nParticles skipNFrames" << endl;
		exit(0);
	}

	cout << "Changing file " << argv[1] << " into " << argv[2] << ". This file contains " << argv[3] << " and one in " << argv[4] << " frames will be written" << endl;

	vector<double> x,y;
	
	ifstream entrada(argv[1]);
	ofstream saida(argv[2]);
	int N = atoi(argv[3]);
	int skipN = atoi(argv[4]);

	while(!entrada.eof()) {
		for (int i=0; i<skipN; i++){
			x.clear();
			y.clear();
			GetFrame(entrada,N,x,y);
		}
		for (int j=0; j<N; j++)
			saida << x[j] << " " << y[j] << endl;
		saida << endl;
	}
}
