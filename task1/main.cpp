/*
 *   BSD LICENSE
 *
 *   (C) Copyright 2023 Huawei Technologies Research & Development (UK) Ltd
 *   All rights reserved.
 *   Prepared by Artemiy Margaritov <artemiy.margaritov@huawei.com>
 */

#include "CmdlineArgumentParser.hpp"
#include "DynamicInstructionCounting/DynamicInstructionCounting_API.hpp"
#include "PredictionAlgorithm/PredictionAlgorithm.hpp"
#include "Route.hpp"
#include "SpaceshipComputer/SpaceshipComputer.hpp"

int main(int argc, char **argv) {
  // Parse command-line options
  CmdlineOptions cmdline_opts;
  if (!parseComdlineOptions(argc, argv, cmdline_opts)) {
    std::cout << "Can't parse command-line arguments" << std::endl;
    return 1;
  }
  // Open a file with a route and initialize the route structure
  std::cout << "Loading Robo's route for evaluation from "
            << cmdline_opts.inFile << " file..." << std::endl;
  Route route(cmdline_opts.inFile);
  // Create and initialize spaceshipComputer object
  SpaceshipComputer spaceshipComputer;
  // Create and initialize roboPredictor object
  RoboPredictor roboPredictor;

  std::cout << "Starting evaluation of Robo's prediction algorithm... "
            << std::endl;
  PlanetInfo nextPlanet;
  while (route.readLineFromFile(nextPlanet)) {
    // Ask Spaceship computer for help
    bool spaceshipComputerPrediction =
        spaceshipComputer.predict(nextPlanet.planetID);

    // Dynamic instruction counting is required to check if the compute cost
    // limit was not violated while making predictions and updating Robo's
    // memory
    enableDynamicInstructionCounting();

    //----------------------------------------------------------------------------------------
    //---------The functions called below are to be implemented by contestants-----
    // Make a prediction of time-of-day on the next planet
    bool prediction = roboPredictor.predictTimeOfDayOnNextPlanet(
        nextPlanet.planetID, spaceshipComputerPrediction);

    // Arrive on the planet and learn the actual time-of-day there.
    // Record the patterns in Robo's internal memory
    roboPredictor.observeAndRecordTimeofdayOnNextPlanet(nextPlanet.planetID,
                                                        nextPlanet.timeOfDay);
    //---------End of the section with the functions that are to be implemented by contestants
    //----------------------------------------------------------------------------------------

    // Dynamic instruction counting is no longer required
    disableDynamicInstructionCounting();
    // Notify the spaceship computer about the actual time-of-day outcome, so it
    // can update it's internal memory
    spaceshipComputer.update(nextPlanet.planetID, nextPlanet.timeOfDay);

    // Update accuracy statistics
    route.updatePredictionAccuracyStatistics(prediction, nextPlanet.timeOfDay);
    // verbose output
    if (cmdline_opts.isVerboseOutputEnabled) {
      std::cout << "Visited planet with ID " << nextPlanet.planetID
                << " Predicted time-of-day " << prediction
                << " Actual observed time-of-day " << nextPlanet.timeOfDay
                << std::endl;
    } else if (!cmdline_opts.isWithoutProgressBar) {
      // not verbose output
      route.displayProgressBar();
    }
  }
  // Print total prediction accuracy
  route.printFinalPredictionAccuracy();
  // Print computational cost
  printInstructionCountingStatistics(route.getTotalNumberOfPlanets());
}
