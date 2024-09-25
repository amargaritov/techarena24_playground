/*
 *   BSD LICENSE
 *
 *   (C) Copyright 2023 Huawei Technologies Research & Development (UK) Ltd
 *   All rights reserved.
 *   Prepared by Artemiy Margaritov <artemiy.margaritov@huawei.com>
 */

#include <boost/program_options.hpp>
#include <iostream>
#include <string>

namespace po = boost::program_options;
namespace po_style = boost::program_options::command_line_style;

// Prepare variables for command-line arguments
struct CmdlineOptions {
  bool isVerboseOutputEnabled;
  bool isWithoutProgressBar;
  std::string inFile;
};

/* Parse the command-line options and place them to cmdlineOptions
 * Returns true if parsing is successful and false otherwise
 */

bool parseComdlineOptions(int argc, char **argv, CmdlineOptions &cmdline_opts) {
  // Declare all allowed options using the boost::options_description class
  po::options_description option_desc(
      "Usage: ./task1 --route <PATH_TO_ROUTE_FILE> <OPTIONS> ");
  po::options_description generic("Generic");
  generic.add_options()("help,h", "produce help message");
  po::options_description input("Input");
  input.add_options()("route,r", po::value<std::string>(&cmdline_opts.inFile),
                      "path to the input file with route, required");
  po::options_description parameters("Parameters");
  parameters.add_options()("verbose,v",
                           po::bool_switch(&cmdline_opts.isVerboseOutputEnabled)
                               ->default_value(false),
                           "enable verbose output during evaluation");
  parameters.add_options()(
      "without-progress-bar,p",
      po::bool_switch(&cmdline_opts.isWithoutProgressBar)->default_value(false),
      "disable evaluation progress bar");
  option_desc.add(generic).add(input).add(parameters);

  // Read the command-line options
  po::command_line_parser parser(argc, argv);
  po::variables_map cmdline;
  po::store(po::command_line_parser(argc, argv)
                .options(option_desc)
                .style(po_style::unix_style | po_style::case_insensitive)
                .run(),
            cmdline);
  po::notify(cmdline);

  // -h[--help] produce help message
  if (cmdline.count("help")) {
    std::cout << option_desc << std::endl;
    return false;
  }
  // check if -r[--route] is provided
  if (!cmdline.count("route")) {
    std::cerr << "Error: the option '--route' is required but missing"
              << std::endl
              << std::endl
              << option_desc << std::endl;
    return false;
  }

  return true;
}

namespace boost {
#ifdef BOOST_NO_EXCEPTIONS
BOOST_NORETURN void throw_exception(std::exception const &e) {
  std::cerr << e.what() << std::endl;
  abort();
};
#endif
}  // namespace boost
