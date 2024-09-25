// This file contains a template for the implementation of Robo prediction
// algorithm

#include "PredictionAlgorithm.hpp"

struct RoboPredictor::RoboMemory {
  // Place your RoboMemory content here
  // Note that the size of this data structure can't exceed 64KiB!
};

bool RoboPredictor::predictTimeOfDayOnNextPlanet(
    std::uint64_t nextPlanetID, bool spaceshipComputerPrediction,
    int nextPlanetGroupTag) {
  // Robo can consult data structures in its memory while predicting.
  // Example: access Robo's memory with roboMemory_ptr-><your RoboMemory
  // content>

  // Robo can perform computations using any data in its memory during
  // prediction. It is important not to exceed the computation cost threshold
  // while making predictions and updating RoboMemory. The computation cost of
  // prediction and updating RoboMemory is calculated by the playground
  // automatically and printed together with accuracy at the end of the
  // evaluation (see main.cpp for more details).

  // Task2 specifics: now, Robo can use additional 10 bits from the 
  // space atlas for prediction.
  // nextPlanetGroupTag

  // Simple prediction policy: follow the spaceship computer's suggestions
  return spaceshipComputerPrediction;
}

void RoboPredictor::observeAndRecordTimeofdayOnNextPlanet(
    std::uint64_t nextPlanetID, bool timeOfDayOnNextPlanet) {
  // Robo can consult/update data structures in its memory
  // Example: access Robo's memory with roboMemory_ptr-><your RoboMemory
  // content>

  // It is important not to exceed the computation cost threshold while making
  // predictions and updating RoboMemory. The computation cost of prediction and
  // updating RoboMemory is calculated by the playground automatically and
  // printed together with accuracy at the end of the evaluation (see main.cpp
  // for more details).

  // Simple prediction policy: do nothing
}

//------------------------------------------------------------------------------
// Please don't modify this file below
//
// Check if RoboMemory does not exceed 64KiB
static_assert(
    sizeof(RoboPredictor::RoboMemory) <= 65536,
    "Robo's memory exceeds 65536 bytes (64KiB) in your implementation. "
    "Prediction algorithms using so much "
    "memory are ineligible. Please reduce the size of your RoboMemory struct.");

// Declare constructor/destructor for RoboPredictor
RoboPredictor::RoboPredictor() { roboMemory_ptr = new RoboMemory; }
RoboPredictor::~RoboPredictor() { delete roboMemory_ptr; }
