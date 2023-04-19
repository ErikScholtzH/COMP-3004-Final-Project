#include "heartmeasuringelectrodes.h"

//constructor
HeartMeasuringElectrodes::HeartMeasuringElectrodes()
{

}

//gets the next coherent score
float HeartMeasuringElectrodes::getNextCoherentScore() {

    float returnVal;

    if(coherentLoc == 64){
        coherentLoc = 0;
    }
    returnVal = coherentScores[coherentLoc];
    coherentLoc += 1;
    return returnVal;
}

//get the next BPM
float HeartMeasuringElectrodes::getNextCoherentBPM() {

    float returnVal;

    if(coherentLoc == 64){
        coherentLoc = 0;
    }
    returnVal = coherentBPM[coherentLoc];
    coherentLoc += 1;
    return returnVal;
}

//get next mid coherence score
float HeartMeasuringElectrodes::getNextMidcoherentScore() {

    float returnVal;

    if(midcoherentLoc == 64){
        midcoherentLoc = 0;
    }
    returnVal = midcoherentScores[midcoherentLoc];
    midcoherentLoc += 1;
    return returnVal;
}

//get next mid coherent bpm
float HeartMeasuringElectrodes::getNextMidcoherentBPM() {

    float returnVal;

    if(midcoherentLoc == 64){
        midcoherentLoc = 0;
    }
    returnVal = midcoherentBPM[midcoherentLoc];
    midcoherentLoc += 1;
    return returnVal;
}

//get next incoherent score
float HeartMeasuringElectrodes::getNextIncoherentScore() {

    float returnVal;

    if(incoherentLoc == 64){
        incoherentLoc = 0;
    }
    returnVal = incoherentScores[incoherentLoc];
    incoherentLoc += 1;
    return returnVal;
}

//get next incoherent bpm
float HeartMeasuringElectrodes::getNextIncoherentBPM() {

    float returnVal;

    if(incoherentLoc == 64){
        incoherentLoc = 0;
    }
    returnVal = incoherentBPM[incoherentLoc];
    incoherentLoc += 1;
    return returnVal;
}

//reset variables
void HeartMeasuringElectrodes::reset() {
    coherentLoc = 0;
    midcoherentLoc = 0;
    incoherentLoc = 0;
}
