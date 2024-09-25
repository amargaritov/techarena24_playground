/*
 *   BSD LICENSE
 *
 *   (C) Copyright 2023 Huawei Technologies Research & Development (UK) Ltd
 *   All rights reserved.
 *   Prepared by Artemiy Margaritov <artemiy.margaritov@huawei.com>
 */

#include <limits.h> /* CHAR_BIT */

#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

#define BIT_MASK(__TYPE__, __ONE_COUNT__) \
  ((__TYPE__)(-((__ONE_COUNT__) != 0))) & \
      (((__TYPE__)-1) >> ((sizeof(__TYPE__) * CHAR_BIT) - (__ONE_COUNT__)))

const int MAX_HISTORY_TABLE_SIZE =
    4096;  // Total number of entries in the history table
const int NUM_HISTORY_TABLES = 1;  // Number of history tables
const int TAG_LENGTH = 12;         // Maximum tag length
const int MAX_CONFIDENCE = 3;      // Maximum confidence counter value

class SpaceshipComputer {
 public:
  SpaceshipComputer() { initializeTables(); }

  // Predict the outcome
  bool predict(uint64_t planetID) {
    // Compute tag and index for each table
    uint64_t tags[NUM_HISTORY_TABLES];
    uint64_t indices[NUM_HISTORY_TABLES];

    for (int table_num = 0; table_num < NUM_HISTORY_TABLES; table_num++) {
      int tableSize = MAX_HISTORY_TABLE_SIZE >> table_num;
      BIT_MASK(uint64_t, TAG_LENGTH);
      indices[table_num] =
          (planetID ^
           (ghist &
            ((table_num > 0) ? BIT_MASK(uint64_t, (1 << table_num)) : 0))) %
          tableSize;
      tags[table_num] =
          (planetID ^
           (ghist &
            ((table_num > 0) ? BIT_MASK(uint64_t, (1 << table_num)) : 0))) &
          BIT_MASK(uint64_t, TAG_LENGTH);
    }

    // Perform predictions and compute the final prediction
    bool final_prediction = false;
    bool found = false;

    for (int i = 0; i < NUM_HISTORY_TABLES; i++) {
      if (tables[i][indices[i]].tag == tags[i]) {
        int confidence = tables[i][indices[i]].counter;
        if (confidence >= (MAX_CONFIDENCE + 1) / 2) {
          final_prediction = true;
        }
        num_of_table_given_prev_prediction = i;
        prev_prediction = final_prediction;
        found = true;
        break;
      }
    }
    if (!found) num_of_table_given_prev_prediction = -1;

    return final_prediction;
  }

  // Update the predictor with the actual outcome of time-of-day
  void update(uint64_t planetID, bool outcome) {
    // Compute tag and index for each table
    uint64_t tags[NUM_HISTORY_TABLES];
    uint64_t indices[NUM_HISTORY_TABLES];

    for (int table_num = 0; table_num < NUM_HISTORY_TABLES; table_num++) {
      int tableSize = MAX_HISTORY_TABLE_SIZE >> table_num;
      BIT_MASK(uint64_t, TAG_LENGTH);
      indices[table_num] =
          (planetID ^
           (ghist &
            ((table_num > 0) ? BIT_MASK(uint64_t, (1 << table_num)) : 0))) %
          tableSize;
      tags[table_num] =
          (planetID ^
           (ghist &
            ((table_num > 0) ? BIT_MASK(uint64_t, (1 << table_num)) : 0))) &
          BIT_MASK(uint64_t, TAG_LENGTH);
    }

    // Update the recently seen history accumulator
    ghist = (ghist << 1) | (outcome ? 1 : 0);

    // Update the tables
    int i = 0;  // a number of a table to update
    if (num_of_table_given_prev_prediction >= 0) {
      if (prev_prediction == outcome) {  // correct prediction was made
        i = num_of_table_given_prev_prediction;
        // increment counter
        if (outcome && (tables[i][indices[i]].counter < MAX_CONFIDENCE)) {
          tables[i][indices[i]].counter++;
        }
        // decrement counter
        else if (!outcome && (tables[i][indices[i]].counter > 0)) {
          tables[i][indices[i]].counter--;
        }
      } else {  // incorrect prediction was made
        if (num_of_table_given_prev_prediction <
            NUM_HISTORY_TABLES - 1) {  // allocate to next table
          i = num_of_table_given_prev_prediction + 1;
          tables[i][indices[i]].tag = tags[i];
          tables[i][indices[i]].counter = (outcome) ? 2 : 1;
        } else {  // if already in the last table,
                  // update the entry in the last table
          i = num_of_table_given_prev_prediction;
          // increment counter
          if (outcome && tables[i][indices[i]].counter < MAX_CONFIDENCE) {
            tables[i][indices[i]].counter++;
          }
          // decrement counter
          else if (!outcome && (tables[i][indices[i]].counter > 0)) {
            tables[i][indices[i]].counter--;
          }
        }
      }
    } else {  // was not found last time
              // allocate to the first table
      int i = 0;
      tables[i][indices[i]].tag = tags[i];
      tables[i][indices[i]].counter = (outcome) ? 2 : 1;
    }
  }

 private:
  // Data structure for a history table entry
  struct TableEntry {
    uint64_t tag;
    int counter;
  };

  int num_of_table_given_prev_prediction = -1;
  bool prev_prediction = 0;

  // History tables
  vector<vector<TableEntry>> tables;

  // Stores history of recently seen outcomes (as a shift register)
  uint64_t ghist = 0;

  // Initialize the history tables
  void initializeTables() {
    for (int table_num = 0; table_num < NUM_HISTORY_TABLES; table_num++) {
      int tableSize = MAX_HISTORY_TABLE_SIZE >> table_num;
      tables.push_back(vector<TableEntry>(tableSize));
      for (int entry_num = 0; entry_num < tableSize; entry_num++) {
        tables[table_num][entry_num].tag = 0;
        tables[table_num][entry_num].counter = 0;
      }
    }
  }
};
