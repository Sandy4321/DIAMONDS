// Class for nested sampling inference
// Enrico Corsaro @ IvS - 24 January 2013
// e-mail: enrico.corsaro@ster.kuleuven.be
// Header file "NestedSampler.h"
// Implementation contained in "NestedSampler.cpp"

#ifndef NESTEDSAMPLER_H
#define NESTEDSAMPLER_H

#include <iostream>
#include <iomanip>
#include <cfloat>
#include <ctime>
#include <cmath>
#include <vector>
#include <cassert>
#include <limits>
#include <algorithm>
#include <Eigen/Dense>
#include "Functions.h"
#include "Prior.h"
#include "Likelihood.h"
#include "Metric.h"
#include "Clusterer.h"


using namespace std;
using namespace Eigen;
typedef Eigen::Ref<Eigen::ArrayXd> RefArrayXd;
typedef Eigen::Ref<Eigen::ArrayXi> RefArrayXi;
typedef Eigen::Ref<Eigen::ArrayXXd> RefArrayXXd;

class NestedSampler
{

    public:
        
        ArrayXXd posteriorSample;                // parameter values for sampling the posterior
        ArrayXd logLikelihoodOfPosteriorSample;  // logLikelihood values corresponding to the posterior sample 
        ArrayXd logWeightOfPosteriorSample;      // logWeights corresponding to the posterior sample

        NestedSampler(const bool printOnTheScreen, const int initialNobjects, const int minNobjects, vector<Prior*> ptrPriors, 
                      Likelihood &likelihood, Metric &metric, Clusterer &clusterer); 
        ~NestedSampler();
        
        void run(const double maxRatioOfRemainderToActualEvidence = 0.05, const int NinitialIterationsWithoutClustering = 100, 
                 const int NiterationsWithSameClustering = 10, const int maxNdrawAttempts = 5000);

        virtual bool drawWithConstraint(const RefArrayXXd sample, const int Nclusters, const vector<int> &clusterIndices,
                                        const vector<int> &clusterSizes, RefArrayXd drawnPoint, 
                                        double &logLikelihoodOfDrawnPoint, const int maxNdrawAttempts) = 0;
        
        int getNiterations();
        double getLogEvidence();
        double getLogEvidenceError();
        double getInformationGain();
        double getComputationalTime();


    protected:

        vector<Prior*> ptrPriors;
        Likelihood &likelihood;
        Metric &metric;
        Clusterer &clusterer;
        bool printOnTheScreen;
        int Ndimensions;
        int Nobjects;                           // Total number of objects at a given iteration
        double worstLiveLogLikelihood;          // The worst likelihood value of the current live sample
        double logCumulatedPriorMass;           // The total (cumulated) prior mass at a given nested iteration
        double logRemainingPriorMass;           // The remaining width in prior mass at a given nested iteration (log X_k)

        mt19937 engine;
        uniform_real_distribution<> uniform;  
        

	private:

        int minNobjects;                         // Minimum number of live points allowed
        int Niterations;                         // Counter saving the number of nested loops used
        double informationGain;                  // Information gain in moving from prior to posterior PDF
        double logEvidence;                      // Skilling's evidence
        double logEvidenceError;                 // Skilling's error on evidence (based on information gain)
        double logMeanLikelihoodOfLivePoints;    // The logarithm of the mean likelihood value of the actual set of live points
        double logMaxLikelihoodOfLivePoints;     // The logarithm of the maxumum likelihood value of the actual set of live points
        double logMaxEvidenceContribution;       // The logarithm of the maximum evidence contribution at a given iteration of the nesting process
        double computationalTime;                // Computational time of the process
        ArrayXd logLikelihood;                   // log-likelihood values of the actual set of live points
        ArrayXXd nestedSample;                   // parameters values (the free parameters of the problem) of the actual set of live points

        bool updateNobjects(double logMaxEvidenceContributionNew, double maxRatioOfRemainderToActualEvidence);
        void printComputationalTime(const double startTime);
}; 

#endif
