//
//  multGaussian.hpp
//  Continuous Speech
//
//  Created by hty on 11/11/2016.
//  Copyright © 2016 hty. All rights reserved.
//

#ifndef multGaussian_h
#define multGaussian_h

#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;

#define PI_GAUSS        (3.1416)
#define MIN_VARIANCE_G    (0.001)
#define TEM_BIAS        (0.5)

vector<vector<vector<vector<double>>>> multiGaussDtw2hmm(vector<vector<vector<vector<double>>>>& input, int gauss_num, vector<vector<vector<vector<int>>>>& allState, vector<vector<vector<vector<double>>>>& variance, vector<vector<vector<int>>>& transfer);
double gaussian_node_cost(vector<double>& frame, vector<vector<double>>& kernel, vector<vector<double>>& variance);
double E_dis(vector<double> input1, vector<double> input2);
double gaussian_edge_cost(int nextIndex, vector<int>& countTransfer);
vector<vector<vector<double>>> getVariance(vector<vector<vector<double>>>& input_one, vector<vector<vector<double>>>& kernel, vector<vector<vector<int>>>& segIndex);
vector<vector<int>> getTransfer(vector<vector<vector<double>>>& input_one, vector<vector<vector<double>>>& kernel, vector<vector<vector<int>>>& segIndex);

#endif /* multGaussian_hpp */
