/*
 *   BSD LICENSE
 *
 *   (C) Copyright 2023 Huawei Technologies Research & Development (UK) Ltd
 *   All rights reserved.
 *   Prepared by Artemiy Margaritov <artemiy.margaritov@huawei.com>
 */

#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

#define GROUP_TAG_MAX 1023

void printProgress(double percentage) {
  int val = (int)(percentage * 100);
  int lpad = (int)(percentage * PBWIDTH);
  int rpad = PBWIDTH - lpad;
  printf("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
  fflush(stdout);
}

struct PlanetInfo {
  std::uint64_t planetID;
  bool timeOfDay;
  int planetGroupTag;
};

bool convertTimeOfDayToBool(std::string timeOfDay) {
  if (timeOfDay == "DAY") return true;
  if (timeOfDay == "NIGHT") return false;
  return false;
}

struct Route {
  std::string route_filename;
  std::ifstream route_file;
  int totalNumberOfPlanets;
  int numberOfVisitedPlanets;
  int numberOfCorrectPredictions;

  Route(std::string& filename) {
    numberOfVisitedPlanets = 0;
    numberOfCorrectPredictions = 0;

    // Count number of lines in the input
    std::string curLine;
    int lineCount = 0;
    std::ifstream file(filename);
    while (getline(file, curLine)) lineCount++;
    totalNumberOfPlanets = lineCount - 1;  // ignore the header line

    route_filename = filename;
    route_file.open(filename, std::ifstream::in);
    // Ignore the header line
    std::getline(route_file, curLine);
  }

  uint64_t getTotalNumberOfPlanets() { return totalNumberOfPlanets; }

  bool readLineFromFile(PlanetInfo& planet) {
    if (!route_file.is_open()) {
      std::cerr << "Error: Could not open route file " << route_filename
                << std::endl
                << std::endl;
      return false;
    }

    if (!route_file.good()) {
      return false;
    }
    std::string line;
    if (std::getline(route_file, line)) {
      // Find the position of the tab character
      size_t tabPos = line.find('\t');

      if (tabPos != std::string::npos) {
        // Extract the planet ID from the beginning of the line
        planet.planetID = std::stoul(line.substr(0, tabPos));

        // Extract the string value after the tab character
        std::string str = line.substr(tabPos + 1);

        if ((str != "DAY") && (str != "NIGHT")) {
          std::cerr << "Error: Could not parse time-of-day for planet "
                    << planet.planetID << ". Received time-of-day " << str
                    << std::endl;
          return false;  // incorrect format
        }

        // Extract time-of-day
        planet.timeOfDay = convertTimeOfDayToBool(str);
        numberOfVisitedPlanets++;

        return true;
      }
    }
    return false;  // Line not found or incorrect format
  }

  bool readLineFromAtlasFile(PlanetInfo& planet) {
    if (!route_file.is_open()) {
      std::cerr << "Error: Could not open route file " << route_filename
                << std::endl
                << std::endl;
      return false;
    }

    if (!route_file.good()) {
      return false;
    }
    std::string line;
    if (std::getline(route_file, line)) {
        std::istringstream iss(line);
        std::string element1, element2, element3;

        if (std::getline(iss, element1, '\t') &&
            std::getline(iss, element2, '\t') && std::getline(iss, element3)) {
          // Extract the planet ID from the beginning of the line
          planet.planetID = std::stoul(element1);

          // Extract time-of-day
          if ((element2 != "DAY") && (element2 != "NIGHT")) {
            std::cerr << "Error: Could not parse time-of-day for planet "
                      << planet.planetID << ". Received time-of-day " << element2
                      << std::endl;
            return false;  // incorrect format
          }
          planet.timeOfDay = convertTimeOfDayToBool(element2);

          // Extract the group tag from the string value after the tab character
          planet.planetGroupTag = std::stoul(element3);
          if ((planet.planetGroupTag < 0) ||
              (planet.planetGroupTag > GROUP_TAG_MAX)) {
            std::cerr
                << "Group tag is outside of the allowed range while processing "
                   "planet number "
                << (numberOfVisitedPlanets + 1)
                << " in the input atlas route. Please check the input file format."
                << std::endl;
            return false;
          }
        } else {
          // Failed to parse all three elements
          std::cerr << "Can't parse the input atlas route file: format is wrong "
                       "while processing planet number "
                    << (numberOfVisitedPlanets + 1)
                    << ". Please check the input file format." << std::endl;
          return false;
        }
        numberOfVisitedPlanets++;
        return true;
    } else {
        return false; //file end
    }
  }

  void displayProgressBar() {
    if ((numberOfVisitedPlanets & 1023) == 0) {
      printProgress(float(numberOfVisitedPlanets) / totalNumberOfPlanets);
    }
  }

  void updatePredictionAccuracyStatistics(bool prediction,
                                          bool correctOutcome) {
    numberOfCorrectPredictions += (prediction == correctOutcome);
  }

  void printFinalPredictionAccuracy() {
    std::cout << std::endl;
    std::cout << "Total number of planets visited " << numberOfVisitedPlanets
              << std::endl;
    std::cout << "Prediction accuracy " << std::setprecision(4)
              << (float)numberOfCorrectPredictions / totalNumberOfPlanets * 100
              << "%" << std::endl;
  }
};
