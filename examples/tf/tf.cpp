/* 
 * File:   tf.cpp
 * Author: dlangenk
 * 
 * Created on 13. September 2011, 11:06
 */

#include <vector>
#include <string>
#include <cmath>
#include <iostream>
#include <fstream>

//is part of a non accepted library of boost but works for me
#include "combination.hpp"

#include "/home/bzcdress/repos/elapsd/include/elapsd/elapsd.h"

using namespace std;


//Only for testing purposes 

vector < string > genKmers(int k) {
    vector <string> kmers(std::pow(4, k), string());

    std::vector<char> v_int(k, 0);
    int N = 0;
    do {
        for (int i = 0; i < v_int.size(); i++) {
            string tmp;
            switch ((int) v_int[i]) {
                case 0:
                    tmp = "A";
                    break;
                case 1:
                    tmp = "C";
                    break;
                case 2:
                    tmp = "G";
                    break;
                case 3:
                    tmp = "T";
                    break;
            }
            kmers[N] += (tmp);
        }
        N++;
    } while (boost::next_mapping(v_int.begin(), v_int.end(), (char) 0, (char) 4));
    return kmers;
}

//-----------------------1----------------------

// Mark Kernel
__attribute__((annotate("kernel")))
void tf(int k, std::string &genome, vector<string> &kmer, vector<int> &count) {
    //For each kmer
    for (int j = 0; j < kmer.size(); j++) {
        int n = 0;
        string::iterator it;
        it = search(genome.begin(), genome.end(), kmer[j].begin(), kmer[j].end());
        //Search in genome for current kmer (full length match)
        while (it != genome.end()) {
            n++;
            it = search(it + 1, genome.end(), kmer[j].begin(), kmer[j].end());
        }
        count.push_back(n);
    }
}

int main() {
    //test case
    //
    //A genome is just a DNA-String

    //length k of the kmers
    int k = 4;
    vector < string > kmer = genKmers(k);
    
    std::cout << "test case" << std::endl;

    string genome = "AACCTCGGACGGGCTTCGGCATCAATTTGAACGTGGTGCTTGGGATCAAATTAGACTAGTACCACGTCGCTAACTGCTTATGGAAGGTGA";
    
    //vector storing the count of each kmer
    vector<int> count;

	// Sizes
	
	cout << "k: 4, genome: " << genome.size() << ", kmer: " << kmer.size() << ", count: " << count.size() << endl;

    int s = 0;
    for (std::vector<std::string>::iterator it = kmer.begin(); it != kmer.end(); ++it) {
        s += it->size();
    }

    s += sizeof(k);
    s += genome.size();
    s += count.size() * sizeof(int);

    cout << "Total size: " << s << " Bytes\n";
//	cout << "Total: " << 4 + genome.size() + kmer.size() + count.size() * sizeof(int) << endl;

    ENHANCE::elapsd e("/home/bzcdress/repos/papers/kdv/data/europar.db", "TF");

    e.addKernel(2, "TF");
    e.addDevice(0, "CPU");

    e.startTimer(2,0);
    tf(k, genome,kmer, count);
    e.stopTimer(2,0);
    
    e.commitToDB();

    //print the counts for testing
    for (int i = 0; i < count.size(); i++) {
        cout << kmer[i] << ": " << count[i] << "\t";
    }
	cout << endl;

}


//Entry points for parallel processing 
//1 - function tf - count word frequencies in parallel
//2 - for loop with different genomes - process all genomes in parallel

//Comments
//Almost no assumptions about the genomes can be made. They are mostly 150 to 5000 characters long.
//k is choosen mostly to be 3-8 (most of the times 3,4)
